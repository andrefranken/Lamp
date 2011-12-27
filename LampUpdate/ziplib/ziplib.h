/*****************************************************************************
 * FILE: ziplib.h
 * AUTHOR: Scott Blomquist
 * CREATED: June 18, 1995
 * DESCRIPTION:
 *   This file is the header file for the zip library interface.  This
 * interface provides several C functions for examining and extracting files
 * from zip archives.
 *   Under Win32/Windows, this library is intended to build a DLL.
 *
 * --- add other heading stuff
 *
 *****************************************************************************/
#ifndef _ZIP_LIB_H_
#define _ZIP_LIB_H_

#define PK_COOL           0    /* no error */
#define PK_WARN           1    /* warning error */
#define PK_EXISTS         2    /* File exists, and don't overwrite is set */
#define PK_FIND           3    /* no files found */
#define PK_ERR            4    /* error in zipfile */
#define PK_BADERR         5    /* severe error in zipfile */
#define PK_MEM            6    /* insufficient memory */
#define PK_MEM2           7    /* insufficient memory */
#define PK_MEM3           8    /* insufficient memory */
#define PK_MEM4           9    /* insufficient memory */
#define PK_MEM5           10   /* insufficient memory */
#define PK_NOZIP          11   /* zipfile not found */
#define PK_PARAM          12   /* bad or illegal parameters specified */
#define PK_DISK           50   /* disk full */
#define PK_EOF            51   /* unexpected EOF */

#define ZIP_ERROR_MAX     80   /* max len of zip error msg */

/* ZIP Error Codes -- These go along with the messages in ziperrs.txt */
#define ZIP_ERR_BAD_VERSION      1  /* Incompatible version */
#define ZIP_ERR_COMPR            2  /* Uncompatible Compressions */
#define ZIP_ERR_FILE_NAME_SIZE   3  /* Bad filename length */
#define ZIP_ERR_ARCHIVE          4  /* Error in archive */
#define ZIP_ERR_COMM_SIZE        5  /* Bad comment size */
#define ZIP_ERR_BAD_OFFSET       6  /* Bad zipfile offset */ 
#define ZIP_ERR_LOCAL_HDR        7  /* Error in local header */
#define ZIP_ERR_FILE             8  /* Error in file within archive */
#define ZIP_ERR_CRYPT            9  /* Encryption not supported */
#define ZIP_ERR_FILE_NOT_FOUND  10  /* File not matched/found */
#define ZIP_ERR_VMS             11  /* File is a VMS file */
#define ZIP_ERR_FILE_CORRUPT    12  /* File probably corrupt */
#define ZIP_ERR_BAD_CRC         13  /* Bad CRC in file */
#define ZIP_ERR_DEL_FILE        14  /* Error removing output file */
#define ZIP_ERR_CREATE          15  /* Error creating file */
#define ZIP_ERR_CHTYPE_BIN      16  /* Error changing file to binary */
#define ZIP_ERR_WRITE_FULL      17  /* Write error (Disk Full?) */
#define ZIP_ERR_EMPTY           18  /* Zip file is empty */
#define ZIP_ERR_MULTI_DISK      19  /* zipfile is multi-disk, not supported */
#define ZIP_ERR_TRUNC_FILENAME  20  /* truncating filename */
#define ZIP_ERR_ZIP_NOT_FOUND   21  /* Zipfile not found */
#define ZIP_ERR_CREATE_DIR      22  /* Error createing directory */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

void ZIP_EnableEnumAll( int value );

void ZIP_GetEnumItemValues( int *enumCount, char ***pItems );

void ZIP_FreeEnumItemValues( void );

/*****************************************************************************
  ZIP_Extract() -
    Extracts a single file from the ZIP archive given by ZipFileName.  The
    full path to the file in the archive is give as ArchiveFilePath.  The
    output file is written to OutputFile.
 *****************************************************************************/
/* EXPORTAPI  */
int ZIP_Extract(
	const char *ZipFilePath,
	const char *ArchiveFilePath,
	const char *OutputFile);

/*****************************************************************************
  ZIP_Access() -
    Returns information about the accessability of a file.
 *****************************************************************************/
/* EXPORTAPI  */
int ZIP_Access(
	const char *ZipFileName,
	const char *ArchiveFilePath );

/*****************************************************************************
  ZIP_GetErrorString() -
    Returns a textual description of a zip status.
 *****************************************************************************/
/* EXPORTAPI  */
void ZIP_GetErrorString(
   int  Error,
   char *ErrorStringOut,
   int  MaxStringSize );


void ZIP_Diag( int ErrorCode );
void ZIP_DiagFile( int ErrorCode, char *FileName );

/***************************************************************************
   ZIP_List
      This function lists (-l) the files in the archive.
 *****************************************************************************/
 /* EXPORTAPI */
int ZIP_ListOneFile(
	const char *ZipFileName,
	char *File, int filesize);

#ifdef __cplusplus
}                 // end of 'extern "C"'
#endif
#endif
