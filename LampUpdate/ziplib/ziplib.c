/*T***************************************************************************

       (C) 1995 Copyright, Informative Graphics Corp.
           All rights reserved.

   File:    ziplib.c

   System:  IFS File System

   $Id: ziplib.c,v 1.1.1.1 2007/08/01 17:59:30 IGC\kenp Exp $

   This file contains the C source code for the zip library interface
       
*****************************************************************************/
#ifdef DEBUG
#include <stdio.h>
#endif

#define _ZIP_LIB_C_

#include "ziplib.h"
#include "unzip.h"

int g_bEnumItems = 0;
int g_EnumCount = 0;
char **g_pItems = NULL;


/*****************************************************************************
   STATIC FUNCTIONS
 *****************************************************************************/
static int alloc_zip_buffers(void);
static void free_zip_buffers(void);
static void init_global_flags(void);


void ZIP_EnableEnumAll( int value )
{
   g_bEnumItems = value;
}

void ZIP_GetEnumItemValues( int *enumCount, char ***pItems )
{
   *enumCount = g_EnumCount;
   *pItems = g_pItems;
}

void ZIP_FreeEnumItemValues( void )
{
   int i;
   if(g_pItems != NULL &&
      g_EnumCount > 0)
   {
      for(i = 0; i < g_EnumCount; i++)
      {
         free(g_pItems[i]);
      }
      free(g_pItems);
   }
}


/*FG**************************************************************************

   ZIP_Extract - GLOBAL

   Description:

      This function extracts a single file from a zip file.

   Return:
      int (out) -
         PK_XXXX status (see unzip.h).  The more important ones are:
            PK_COOL - all went ok.
            PK_WARN - minor warnings, but ok.
            PK_ERR  - error in archive.

               
   Parameters:
      char *ZipFileName (in) -
         The name of the zip archive to extract a file from.
      char *ArchiveFilePath (in) -
         The name of the file to extract from the zip archive.
      char *OutputFile (in) -
         The complete path name to write the extracted file to.

 *****************************************************************************/
 /* EXPORTAPI */
int ZIP_Extract(
	const char *ZipFileName,
	const char *ArchiveFilePath,
	const char *OutputFile)
{
	char ConvertedArchivePath[FILNAMSIZ];
   char *FileNameList[2];
	char *cptr;
   int  status;

	FileNameList[0] = ConvertedArchivePath;
	FileNameList[1] = NULL;

#ifdef DEBUG
   char ErrorString[ZIP_ERROR_MAX];

	printf("ZIP_Extract: ZipFileName =     \"%s\"\n", ZipFileName );
	printf("             ArchiveFilePath = \"%s\"\n", ArchiveFilePath );
	printf("             OutputFile =      \"%s\"\n", OutputFile );
#endif

    init_global_flags();

	/* Name of ".zip" archive file: */
	strcpy(zipfn, ZipFileName);

	/* Check to see if the ".zip" file exists */
	if (_stat(zipfn, &statbuf) || (statbuf.st_mode & S_IFMT) == S_IFDIR)
	{
        strcat(zipfn, ZSUFX);
	}

	/* Try stat again (possibly with ".zip" appended) */
	if (_stat(zipfn, &statbuf)) {
		/* Can't find zip file */
        RETURN(PK_NOZIP);
    } else {
        ziplen = statbuf.st_size;
	}

	/*
	 * Setup Names of files to extract (there is only one file) and convert DOS '\'
	 * to '/'.
	 */
	strncpy(ConvertedArchivePath, ArchiveFilePath, FILNAMSIZ);
	for(cptr = ConvertedArchivePath; *cptr != '\0'; cptr++)
	{
		if( *cptr == '\\' )
		{
			*cptr = '/';
		}
	}

	pfnames    = FileNameList;	      /* list of files to extract */
   pxnames    = &FileNameList[1];   /* exclude list */
	filespecs  = 1;						/* One file to extract */
	xfilespecs = 0;						/* No files to exclude */

   /* don't overwrite existing files when extracting */
   overwrite_none = 0;  // kp: The temp file will exist and should be overwritten.
   
   different_output_filename = 1;   /* write output to differnet filename */
   strcpy( output_filename, OutputFile );


	status = alloc_zip_buffers();
   if( status != PK_COOL ) {
#ifdef DEBUG
      fprintf(stderr, "ZIP_Extract: allocate_zip_buffers failed: %d\n", status);
#endif
      return status;
   }

	status = process_zipfile();

   /* just keep going, we'll return the status - buffers should be freed */
   /* in any case */

	free_zip_buffers();

#ifdef DEBUG
   printf("ZIP_Extract: return status: %d\n", status);
   ZIP_GetErrorString( status, ErrorString, ZIP_ERROR_MAX );
   printf("   Status description: %s\n", ErrorString );
#endif

	return status;
}

/*FG**************************************************************************

   ZIP_Access - GLOBAL

   Description:
      This function checks to see if a give file is in a zip archive.

   Return:
      int -
         < 0 - error - PK_XXX error code returned.
         = 0 - ok.
               
   Parameters:
      char *ZipFileName (in) -
         The name of the zip file to look for the file in.
      char *ArchiveFilePath (in) -
         The name of the file to look for in the zip archive.
 *****************************************************************************/
 /* EXPORTAPI */
int ZIP_Access(
	const char *ZipFileName,
	const char *ArchiveFilePath )
{
	char ConvertedArchivePath[FILNAMSIZ];
   char *FileNameList[2];
	char *cptr;
   int  status;

	FileNameList[0] = ConvertedArchivePath;
	FileNameList[1] = NULL;

#ifdef DEBUG
   char ErrorString[ZIP_ERROR_MAX];

	printf("ZIP_Access: ZipFileName = \"%s\"\n", ZipFileName );
	printf("            ArchiveFilePath = \"%s\"\n", ArchiveFilePath );
#endif

   init_global_flags();

	/* Name of ".zip" archive file: */
	strcpy(zipfn, ZipFileName);

	/* Check to see if the ".zip" file exists */
	if (_stat(zipfn, &statbuf) || (statbuf.st_mode & S_IFMT) == S_IFDIR)
	{
        strcat(zipfn, ZSUFX);
	}

	/* Try stat again (possibly with ".zip" appended) */
	if (_stat(zipfn, &statbuf)) {
		/* Can't find zip file */
        RETURN(PK_NOZIP);
    } else {
        ziplen = statbuf.st_size;
	}

	/*
	 * Setup Names of files to check (there is only one file) and convert DOS '\'
	 * to '/' (this is how it is in the archive).
	 */
	strncpy(ConvertedArchivePath, ArchiveFilePath, FILNAMSIZ);
	for(cptr = ConvertedArchivePath; *cptr != '\0'; cptr++)
	{
		if( *cptr == '\\' )
		{
			*cptr = '/';
		}
	}

	pfnames    = FileNameList;	      /* list of files to extract */
   pxnames    = &FileNameList[1];   /* exclude list */
	filespecs  = 1;						/* One file to extract */
	xfilespecs = 0;						/* No files to exclude */
   tflag = 1;                       /* tflag - test each file */

	status = alloc_zip_buffers();
   if( status != PK_COOL ) {
      return status;
   }

	status = process_zipfile();

   /* just keep going, we'll return the status - buffers should be freed */
   /* in any case */

	free_zip_buffers();

#ifdef DEBUG
   printf("ZIP_Access: return status: %d\n", status);
   ZIP_GetErrorString( status, ErrorString, ZIP_ERROR_MAX );
   printf("   Status description: %s\n", ErrorString );
#endif

	return status;
}


/*FG**************************************************************************

   ZIP_GetErrorString - GLOBAL

   Description:
      This function returns a simple message describing a PK_XXX status.
      The maximum size of this string is 80 characters (so far).

   Return:
      None.

   Parameters:
      int Error (in) -
         The PK_XXXX status to get the error string for.
      char *ErrorStringOut (out) -
         The error string.
      int MaxStringSize (in) -
         The maximum number of characters to put in the error string;

 *****************************************************************************/
/* EXPORTAPI */
void ZIP_GetErrorString(
   int  Error,
   char *ErrorStringOut,
   int  MaxStringSize )
{
   char *msg;

   switch(Error)
   {
      case PK_COOL:        /* no error */
         msg="OK";
         break;

      case PK_WARN:        /* minor error */
         msg="Minor errors encountered";
         break;

      case PK_EXISTS:      /* output file exists */
         msg="Output file exists";
         break;

      case PK_FIND:        /* no files found */
         msg="File not found in Zip archive";
         break;

      case PK_ERR:         /* error in zipfile */
         msg="Error in Zip file";
         break;

      case PK_BADERR:      /* severe error in zipfile */
         msg="Sever error in Zip file";
         break;

      case PK_MEM:         /* insufficient memory */
      case PK_MEM2:        /* insufficient memory */
      case PK_MEM3:        /* insufficient memory */
      case PK_MEM4:        /* insufficient memory */
      case PK_MEM5:        /* insufficient memory */
         msg="Insufficient memory";
         break;

      case PK_NOZIP:       /* zipfile not found */
         msg="Could not find Zip file";
         break;

      case PK_PARAM:       /* bad or illegal parameters specified */
         msg="Bad parameters for unzip";
         break;

      case PK_DISK:        /* disk full */
         msg="Disk full";
         break;

      case PK_EOF:         /* unexpected EOF */
         msg="Unexpected end of file";
         break;

      default:
         msg="Unknown error";
         break;

   }

   strncpy(ErrorStringOut, msg, MaxStringSize);
   ErrorStringOut[MaxStringSize-1]='\0';

   return;
}


/*FL**************************************************************************
   alloc_zip_buffers() - STATIC

   Description:
      Allocates some buffers needed by the unzip code.

   Return:
      int - return code
         = 0 (PK_KOOL) - ok.
         < 0 - error
                        
   Parameters:
      None
 *****************************************************************************/
static int alloc_zip_buffers(void)
{
    inbuf = (uch *)malloc(INBUFSIZ + 4);    /* 4 extra for hold[] (below) */
    outbuf = (uch *)malloc(OUTBUFSIZ + 1);  /* 1 extra for string termin. */
#ifndef DOS_OS2
    if (aflag)                  /* if need an ascebc scratch, */
        outout = (uch *)malloc(OUTBUFSIZ);
    else                        /*  allocate it... */
#endif /* !DOS_OS2 */
        outout = outbuf;        /*  else just point to outbuf */

    if ((inbuf == (uch *)NULL) || (outbuf == (uch *)NULL) ||
        (outout == (uch *)NULL)) {
        /* Failed to allocate buffer */
        RETURN(PK_MEM);
    }
    hold = &inbuf[INBUFSIZ];    /* to check for boundary-spanning signatures */
    
    return PK_COOL;
}


/*FL**************************************************************************

   free_zip_buffers() - LOCAL

   Description:

      Frees up dynamic buffers used by unzip.

   Return:
      None.

   Parameters:
      None.

 *****************************************************************************/
static void free_zip_buffers(void)
{
    free(inbuf);
    free(outbuf);

#ifndef DOS_OS2
    if(outout != outbuf) {
        free(outout);
    }
#endif /* !DOS_OS2 */

}


/*FL**************************************************************************

   init_global_flags - LOCAL

   Description:

      Initializes global flags used by the unzip routines.  The 
      unzip code has a ton of global flags that must be initialized each
      time a call is made to one of the public functions in this file
      (ZIP_Extract, ZIP_Access, etc).

      In the original unzip code, these were initialized in the variable
      definitions.  Because there may be multiple calls to the these
      zip library routines by a given application, these flags must be
      reset each time.

   Return:
      None.

   Parameters:
      None.

 *****************************************************************************/
static void init_global_flags(void)
{
   /* flag initialization: */
   different_output_filename=0;
   aflag=0;
   cflag=0;
   fflag=0;
   jflag=0;
   overwrite_none=0;
   overwrite_all=1;
   force_flag=0;
   quietflg=0;
#ifdef DOS_OS2
   sflag=1;       
#endif
   tflag=0;
   uflag=0;
   U_flag=0;
   vflag=0; 
   V_flag=0;     
#ifdef VMS
   secinf=0;    
#endif
   zflag=0;         
   process_all_files = 0;
   extra_bytes = 0;
#ifdef MACOS
   giCursor = 0;
#endif /* MACOS */
   mem_mode = 0;

   cur_zipfile_bufstart = 0;

   /* string & pointer intialization */
   pInfo=info;

   return;
}

/*FG**************************************************************************

   ZIP_Diag - GLOBAL

   Description:

      This function is called internally by the zip code to log a zip error.
      This takes the place of diagnostics being printed to standard output.

   Return:

      None.
            
   Parameters:
      int ErrorCode (in) -
         The error code that corresponds to the zip error in the resource file.
 *****************************************************************************/
void ZIP_Diag (
   int ErrorCode )
{
#ifdef DEBUG
   fprintf(stderr, "*** ZIP_Diag: code = %d\n", ErrorCode);
#endif

}


/*FG**************************************************************************

   ZIP_DiagFile - GLOBAL

   Description:
      This function is called internally by the zip code to report an error
      related to a given file in the zip archive.  

   Return:
      None.

   Parameters:
      int ErrorCode (in) -
         The error code for the error.  This is used as an index into a resource
         file.
      char *FileName (in) -
         The name of the file that the error relates to.

 *****************************************************************************/
void ZIP_DiagFile(
   int ErrorCode,
   char *FileName )
{
#ifdef DEBUG
   fprintf(stderr, "*** ZIP_DiagFile: File = \"%s\", Code = %d\n",
      FileName,
      ErrorCode );   
#endif /* DEBUG */

}


/*dmd**************************************************************************

   ZIP_List - GLOBAL

   Description:
      This function lists (-l) the files in the archive.

   Return:
      int -
         < 0 - error - PK_XXX error code returned.
         = 0 - ok.
               
   Parameters:
      char *ZipFileName (in) -
         The name of the zip archive to list
      char **List (out) -
         The resulting list


  NOTICE!: free() must be called with the pointer returned in List, by the caller
      to release the memory.
 *****************************************************************************/
 /* EXPORTAPI */

int ZIP_ListOneFile(
	const char *ZipFileName,
	char *File, int filesize )
{
	char ConvertedArchivePath[FILNAMSIZ];
   char *FileNameList[2];
   int  status;

	FileNameList[0] = ConvertedArchivePath;
	FileNameList[1] = NULL;

#ifdef DEBUG
   char ErrorString[ZIP_ERROR_MAX];

	printf("ZIP_Access: ZipFileName = \"%s\"\n", ZipFileName );
#endif

   init_global_flags();

	/* Name of ".zip" archive file: */
	strcpy(zipfn, ZipFileName);

	/* Check to see if the ".zip" file exists */
	if (_stat(zipfn, &statbuf) || (statbuf.st_mode & S_IFMT) == S_IFDIR)
	{
        strcat(zipfn, ZSUFX);
	}

	/* Try stat again (possibly with ".zip" appended) */
	if (_stat(zipfn, &statbuf)) {
		/* Can't find zip file */
        RETURN(PK_NOZIP);
    } else {
        ziplen = statbuf.st_size;
	}

   //pfnames    = FileNameList;	      /* list of files to extract */
   //pxnames    = NULL; //&FileNameList[1];   /* exclude list */
	filespecs  = 0;						/*  file to extract */
	xfilespecs = 0;						/* No files to exclude */
   vflag = 3;                       /* list files */

	status = alloc_zip_buffers();
   if( status != PK_COOL ) {
      return status;
   }

	status = process_zipfile();

   /* just keep going, we'll return the status - buffers should be freed */
   /* in any case */

	free_zip_buffers();
   if(File != NULL && filename != NULL && filename[0] != '\0')
   {
      memcpy(File,filename,filesize);
   }

#ifdef DEBUG
   printf("ZIP_Access: return status: %d\n", status);
   ZIP_GetErrorString( status, ErrorString, ZIP_ERROR_MAX );
   printf("   Status description: %s\n", ErrorString );
#endif

	return status;
}


