/*---------------------------------------------------------------------------

  unzip.c

  UnZip - a zipfile extraction utility.  See below for make instructions, or
  read the comments in Makefile and the various Contents files for more de-
  tailed explanations.  To report a bug, send a *complete* description to
  zip-bugs@cs.ucla.edu; include machine type, operating system and version,
  compiler and version, and reasonably detailed error messages or problem
  report.  To join Info-ZIP, send a message to izip-mgr@wkuvx1.bitnet (or
  izip-mgr%wkuvx1.bitnet@ukcc.uky.edu).

  UnZip 5.x is a greatly expanded and partially rewritten successor to 4.x,
  which in turn was almost a complete rewrite of version 3.x.  For a detailed
  revision history, see UnzpHist.zip at quest.jpl.nasa.gov.  For a list of
  the many (near infinite) contributors, see "CONTRIBS" in the UnZip source
  distribution.

  ---------------------------------------------------------------------------

  To compile (partial instructions):

     under Unix (cc):  make <system name>
       (type "make list" for a list of valid names, or read Makefile for 
       details.  "make unzip" works for most systems.  If you have a NEW
       system, not covered by any of the existing targets, send FULL infor-
       mation--hardware, OS, versions, etc.--to zip-bugs@cs.ucla.edu)

     under VMS (VAX C or GNU C):  @make_vaxc_unzip  or  @make_gcc_unzip
       (from VMS sub-archive; can also use MMS or MAKE/VMS; see VMS.notes)

     under OS/2 (MSC, gcc, IBM C Set/2, Watcom C):  make -f makefile.os2
       (from OS2 sub-archive; for MSC, use NMAKE)

     under MS-DOS (MSC, Turbo C, or Borland C++):  use the makefiles or
       project files in the MSDOS sub-archive; edit or otherwise modify
       as appropriate.  For MSC, use NMAKE.

     under MS Windows 3.1:  get wunz13sr.{zip | zoo | whatever} and use
       the included makefile

     under Windows NT:  use makefile.nt (from NT sub-archive)

     under Macintosh OS:  double click on unzip.make; press <Command>-M
       (from MAC sub-archive)

     under AmigaDOS:  lmk -f amiga/LMKFile all (from AMIGA sub-archive;
       see also MakeScript)

     under TOPS-20:  use make.mic and "do make"

     under Atari TOS:  needs considerable work yet...

  ---------------------------------------------------------------------------

  Version:  unzip51.{tar.Z | zip | zoo} for Unix, VMS, OS/2, MS-DOS, Windows,
              Windows NT, Macintosh, Amiga and TOPS-20.  Decryption requires
              sources in zcrypt20.zip, and Windows (not NT) support requires
              sources in wunz13sr.zip.  See accompanying file "Where" in the
              main source distribution for ftp, uucp and mail-server sites.
  Copyrights:  see accompanying file "COPYING" in UnZip source distribution.

  ---------------------------------------------------------------------------*/




#include "ziplib.h"
#include "unzip.h"               /* includes, typedefs, macros, etc. */
#include "crypt.h"

extern int g_bEnumItems;
extern int g_EnumCount;
extern char **g_pItems;


#ifdef MSWIN
#  include "wizunzip.h"
#endif

/* #define RELEASE */
#ifdef RELEASE
#  define VERSION  "v5.0 of 21 August 1992"    /* official release version */
#else
#  define VERSION  "v5.1d3 BETA of 18 Feb 93"   /* internal beta level */
#endif

#ifdef MSWIN
#  include "winsetup.c"   /* duplicates some of code in main() */
#else /* !MSWIN */

/******************/
/*  Main program  */
/******************/

int main(argc, argv)   /* return PK-type error code (except under VMS) */
    int argc;
    char *argv[];
{
    char *s;
    int c, error=FALSE, negative=0;


#if defined(MACOS) || defined(MALLOC_WORK)
    area.Slide = (uch *)calloc(8193, sizeof(short)+sizeof(char)+sizeof(char));
    area.shrink.Prefix_of = (short *)area.Slide;
    area.shrink.Suffix_of = area.Slide + (sizeof(short)*(HSIZE+1));
    area.shrink.Stack = area.Slide + (sizeof(short) + sizeof(char))*(HSIZE+1);
#endif

/*---------------------------------------------------------------------------
    Macintosh initialization code.
  ---------------------------------------------------------------------------*/

#ifdef MACOS
#ifdef THINK_C
#   include <console.h>
    static char *argstr[30], args[30*64];
    Point p;
    SFTypeList sfT;
    EventRecord theEvent;
    short eMask;
    SFReply fileRep;
#endif /* THINK_C */
    int a;

    for (a = 0;  a < 4;  ++a)
        rghCursor[a] = GetCursor(a+128);
    giCursor = 0;

#ifdef THINK_C   
    for (a = 0;  a < 30;  ++a)
        argstr[a] = &args[a*64];
start:
    tflag=vflag=cflag=aflag=jflag=U_flag=quietflg=fflag=uflag=zflag = 0;
    local_hdr_sig[1] = central_hdr_sig[1] = end_central_sig[1] = '\0';
/*  extd_local_sig[1] = '\0';  */
    error = FALSE;

    argc = ccommand(&argv);
    SetPt(&p, 40, 40);

    SFGetFile(p, "\pSpecify ZIP file:", 0L, -1, sfT, 0L, &fileRep);
    if (fileRep.good) {
        macfstest(fileRep.vRefNum);
        ResolveMacVol(fileRep.vRefNum, &gnVRefNum, &glDirID, NULL);
        for (a = 1;  a < argc;  ++a)
            if (argv[a][0] == '-')
                BlockMove(argv[a], argstr[a], (strlen(argv[a]) > 63) ? 64 :
                   strlen(argv[a])+1);
            else
                break;
        PtoCstr((char *)fileRep.fName);
        strcpy(argstr[a], (char *)fileRep.fName);
        for (;  a < argc;  ++a)
            BlockMove(argv[a], argstr[a+1], (strlen(argv[a]) > 63) ? 64 :
               strlen(argv[a])+1);
        ++argc;
        argv = argstr;

        if (hfsflag == FALSE)  /* can't support directories:  junk pathnames */
            jflag = 1;
    }
#endif /* THINK_C */
#endif /* MACOS */

/*---------------------------------------------------------------------------
    Set signal handler for restoring echo, warn of zipfile corruption, etc.
  ---------------------------------------------------------------------------*/

    signal(SIGINT, handler);
#ifdef SIGTERM                 /* Amiga has no SIGTERM; maybe others */
    signal(SIGTERM, handler);
#endif
#ifdef SIGBUS
    signal(SIGBUS, handler);
#endif
#ifdef SIGSEGV
    signal(SIGSEGV, handler);
#endif

/*---------------------------------------------------------------------------
    Debugging info for checking on structure padding:
  ---------------------------------------------------------------------------*/

#ifdef DEBUG_STRUC
    printf("local_file_hdr size: %X\n",
           sizeof(local_file_hdr));
    printf("local_byte_hdr size: %X\n",
           sizeof(local_byte_hdr));
    printf("actual size of local headers: %X\n", LREC_SIZE);

    printf("central directory header size: %X\n",
           sizeof(cdir_file_hdr));
    printf("central directory byte header size: %X\n",
           sizeof(cdir_byte_hdr));
    printf("actual size of central dir headers: %X\n", CREC_SIZE);

    printf("end central dir record size: %X\n",
           sizeof(ecdir_rec));
    printf("end central dir byte record size: %X\n",
           sizeof(ec_byte_rec));
    printf("actual size of end-central-dir record: %X\n", ECREC_SIZE);
#endif /* DEBUG_STRUC */

/*---------------------------------------------------------------------------
    Put environment-variable options into the queue, then rip through any
    command-line options lurking about...
  ---------------------------------------------------------------------------*/

    envargs(&argc, &argv, ENV_UNZIP);

    while (--argc > 0 && (*++argv)[0] == '-') {
        s = argv[0] + 1;
        while ((c = *s++) != 0) {    /* "!= 0":  prevent Turbo C warning */
            switch (c) {
                case ('-'):
                    ++negative;
                    break;
                case ('a'):
                    if (negative)
                        aflag = FALSE, negative = 0;
                    else
                        aflag = TRUE;
                    break;
#if 0
                case ('b'):    /* force binary mode */
                    if (negative)
                        bflag = FALSE, negative = 0;
                    else
                        bflag = TRUE;
                    break;
#endif
                case ('c'):
                    if (negative) {
                        cflag = FALSE, negative = 0;
#ifdef NATIVE
                        aflag = FALSE;
#endif
                    } else {
                        cflag = TRUE;
#ifdef NATIVE
                        aflag = TRUE;  /* so you can read it on the screen */
#endif
                    }
                    break;
                case ('d'):    /* re-create directory structure (default) */
                    if (negative)
                        jflag = TRUE, negative = 0;
                    break;
                case ('e'):    /* just ignore -e, -x options (extract) */
                    break;
                case ('f'):    /* "freshen" (extract only newer files) */
                    if (negative)
                        fflag = uflag = FALSE, negative = 0;
                    else
                        fflag = uflag = TRUE;
                    break;
                case ('j'):    /* junk pathnames/directory structure */
                    if (negative)
                        jflag = FALSE, negative = 0;
                    else
                        jflag = TRUE;
                    break;
                case ('l'):
                    if (negative) {
                        vflag = MAX(vflag-negative,0);
                        negative = 0;
                    } else
                        ++vflag;
                    break;
                case ('n'):    /* don't overwrite any files */
                    if (negative)
                        overwrite_none = FALSE, negative = 0;
                    else
                        overwrite_none = TRUE;
                    break;
                case ('o'):    /* OK to overwrite files without prompting */
                    if (negative) {
                        overwrite_all = MAX(overwrite_all-negative,0);
                        force_flag = MAX(force_flag-negative,0);
                        negative = 0;
                    } else {
                        ++overwrite_all;
                        ++force_flag;  /* (share -o for now) force to cont. */
                    }
                    break;
                case ('p'):    /* pipes:  stdout, no tranlation, no messages */
                    if (negative) {
                        cflag = FALSE;
                        quietflg = MAX(quietflg-999,0);
                        negative = 0;
                    } else {
                        cflag = TRUE;
                        quietflg += 999;
                    }
                    break;
                case ('q'):    /* quiet:  fewer comments/messages */
                    if (negative) {
                        quietflg = MAX(quietflg-negative,0);
                        negative = 0;
                    } else
                        ++quietflg;
                    break;
#ifdef DOS_OS2
                case ('s'):    /* spaces in filenames:  allow by default */
                    if (negative)
                        sflag = TRUE, negative = 0;
                    else
                        sflag = FALSE;
                    break;
#endif
                case ('t'):
                    if (negative)
                        tflag = FALSE, negative = 0;
                    else
                        tflag = TRUE;
                    break;
                case ('U'):    /* Uppercase (don't convert to all-lower) */
                    if (negative)
                        U_flag = FALSE, negative = 0;
                    else
                        U_flag = TRUE;
                    break;
                case ('u'):    /* update (extract only new and newer files) */
                    if (negative)
                        uflag = FALSE, negative = 0;
                    else
                        uflag = TRUE;
                    break;
                case ('V'):    /* Version (retain VMS/DEC-20 file versions) */
                    if (negative)
                        V_flag = FALSE, negative = 0;
                    else
                        V_flag = TRUE;
                    break;
                case ('v'):    /* verbose */
                    if (negative) {
                        vflag = MAX(vflag-negative,0);
                        negative = 0;
                    } else if (vflag)
                        ++vflag;
                    else
                        vflag = 2;
                    break;
#ifdef VMS
                case ('X'):   /* restore owner/protection info (need privs?) */
                    if (negative)
                        secinf = FALSE, negative = 0;
                    else
                        secinf = TRUE;
                    break;
#endif /* VMS */
                case ('x'):    /* extract:  default */
                    break;
                case ('z'):    /* display only the archive comment */
                    if (negative) {
                        zflag -= negative;
                        negative = 0;
                    } else
                        ++zflag;
                    break;
                default:
                    error = TRUE;
                    break;

            } /* end switch */
        } /* end while (not end of argument string) */
    } /* end while (not done with switches) */

/*---------------------------------------------------------------------------
    Make sure we aren't trying to do too many things here.  [This seems like
    kind of a brute-force way to do things; but aside from that, isn't the
    -a option useful when listing the directory (i.e., for reading zipfile
    comments)?  It's a modifier, not an action in and of itself, so perhaps
    it should not be included in the test--certainly, in the case of zipfile
    testing, it can just be ignored.]
  ---------------------------------------------------------------------------*/

    if ((aflag && tflag) || (aflag && vflag) || (cflag && tflag) ||
        (cflag && uflag) || (cflag && vflag) || (tflag && uflag) ||
        (tflag && vflag) || (uflag && vflag) || (fflag && overwrite_none)) {
        fprintf(stderr, "error:\
  -at, -av, -ct, -cu, -cv, -fn, -tu, -tv, -uv combinations not allowed\n");
        error = TRUE;
    }
    if (quietflg && (zflag > 0))  /* quietflg makes no sense */
        quietflg = 0;
    if (overwrite_all && overwrite_none) {
        fprintf(stderr, "caution:  both -n and -o specified; ignoring -o\n");
        overwrite_all = FALSE;
    }
    if ((argc-- == 0) || error)
        RETURN(usage(error));

/*---------------------------------------------------------------------------
    Now get the zipfile name from the command line and see if it exists as a
    regular (non-directory) file.  If not, append the ".zip" suffix.  We don't
    immediately check to see if this results in a good name, but we will do so
    later.  In the meantime, see if there are any member filespecs on the com-
    mand line, and if so, set the filename pointer to point at them.
  ---------------------------------------------------------------------------*/

    strcpy(zipfn, *argv++);
    if (_stat(zipfn, &statbuf) || (statbuf.st_mode & S_IFMT) == S_IFDIR)
        strcat(zipfn, ZSUFX);
#ifdef UNIX  /* no extension on Unix exe's--might find zip, not zip.zip; etc. */
    else if (statbuf.st_mode & S_IXUSR)
        fprintf(stderr, "note:  file [ %s ] may be an executable\n\n", zipfn);
#endif

    if (_stat(zipfn, &statbuf)) {   /* try again */
        fprintf(stderr, "error:  can't find zipfile [ %s ]\n", zipfn);
        RETURN(PK_NOZIP);
    } else
        ziplen = statbuf.st_size;

    filespecs = argc;
    xfilespecs = 0;

    if (argc != 0) {
        char **pp = argv-1;
#ifdef DOS_OS2
        char *q;

        pfnames = argv;
        /* convert MSDOS-style directory separators to Unix-style ones */
        while (*pfnames != NULL) {
            for (q = *pfnames;  *q;  ++q)
                if (*q == '\\')
                    *q = '/';
            ++pfnames;
        }
#endif
        pfnames = argv;
        while (*++pp)
            if (!strcmp(*pp, "-x")) {
                if (pp > argv) {
                    *pp = 0;           /* terminate pfnames */
                    filespecs = pp - pfnames;
                } else {
                    pfnames = fnames;  /* defaults */
                    filespecs = 0;
                }
                pxnames = pp + 1;      /* excluded-names ptr starts after -x */
                xfilespecs = argc - filespecs - 1;
                break;                 /* skip rest of args */
            }
        process_all_files = FALSE;
    } else
        process_all_files = TRUE;       /* for speed */

/*---------------------------------------------------------------------------
    Okey dokey, we have everything we need to get started.  Let's roll.
  ---------------------------------------------------------------------------*/

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
        fprintf(stderr, "error:  can't allocate unzip buffers\n");
        RETURN(PK_MEM);
    }
    hold = &inbuf[INBUFSIZ];    /* to check for boundary-spanning signatures */

    RETURN(process_zipfile());  /* keep passing errors back... */

} /* end main() */





/**********************/
/*  Function usage()  */
/**********************/

int usage(error)   /* return PK-type error code */
    int error;
{
#ifdef VMS
#  define QUOT '\"'
#  define EXAMPLE2 "vms.c"
#  define EXAMPLE1 \
"unzip \"-V\" foo \"Bar\" => must quote uppercase options and filenames in VMS"
#else
#  define QUOT ' '
#  define EXAMPLE2 "ReadMe"
#  define EXAMPLE1 \
"unzip -p foo | more  => send contents of foo.zip via pipe into program more"
#endif

#ifdef NATIVE
#ifdef EBCDIC
    char *astring = "-a  convert ASCII to EBCDIC";
#else /* !EBCDIC */
    char *astring = "-a  convert ASCII to native chars";
#endif /* ?EBCDIC */
/*  char *astring = "-a  convert ASCII to " NATIVE;  (ANSI C concatenation)  */
    char *loc_str = "";
#else /* !NATIVE */
#ifdef DOS_OS2
    char *astring = "-a  convert text (LF => CR LF)";
    char *loc_str = " -s  spaces in filenames => _";
#else /* !DOS_OS2 */
#ifdef MACOS
    char *astring = "-a  convert text (CR LF => CR)";
    char *loc_str = "";
#else /* !MACOS:  UNIX, VMS, etc. */
    char *astring = "-a  convert text (CR LF => LF)";
#ifdef VMS
    char *loc_str = "\"-X\" restore owner/protection info";
#else /* !VMS */
    char *loc_str = "";
#endif /* ?VMS */
#endif /* ?MACOS */
#endif /* ?DOS_OS2 */
#endif /* ?NATIVE */
    FILE *usagefp;


/*---------------------------------------------------------------------------
    If user requested usage, send it to stdout; else send to stderr.
  ---------------------------------------------------------------------------*/

    if (error)
        usagefp = (FILE *)stderr;
    else
        usagefp = (FILE *)stdout;

    fprintf(usagefp, "\
UnZip %s, by Info-ZIP.  Portions (c) 1989 by S. H. Smith.\n\
Bug reports ONLY to zip-bugs%%wkuvx1.bitnet@ukcc.uky.edu; see README for detail\
s\n\n", VERSION);

    fprintf(usagefp, "\
Usage: unzip [ -options[modifiers] ] file[.zip] [list] [-x xlist]\n\
  Default action is to extract all specified files except for those in xlist.\n\
  -c  extract files to stdout/screen (CRT)   -l  list files (short format)\n\
  -p  extract files to pipe, no messages     -v  list files (verbose format)\n\
  -f  freshen existing files, create none    -t  test compressed archive data\n\
  -u  update files, create if necessary      -z  display archive comment\n\
                                             -x  exclude files which follow\n\
modifiers:\n\
  -n  never overwrite existing files        %s\n", loc_str);
    fprintf(usagefp, "\
  -o  overwrite files WITHOUT prompting      %s\n\
  -j  junk paths (don't make directories)   %c-U%c don't make names lowercase\n\
  -q  quiet mode (-qq => quieter)           %c-V%c retain VMS version numbers\
\n\n\
Examples: (See unzip.doc for more information)\n\
  unzip data1 -x joe   => extract all files except joe from zipfile data1.zip\n\
  %s\n\
  unzip -fo foo %-6s => quietly replace existing %s if archive file newer\n",
    astring, QUOT,QUOT, QUOT,QUOT, EXAMPLE1, EXAMPLE2,EXAMPLE2);

    if (error)
        return PK_PARAM;
    else
        return PK_COOL;     /* just wanted usage screen: no error */

} /* end function usage() */

#endif /* ?MSWIN */




/********************************/
/*  Function process_zipfile()  */
/********************************/

int process_zipfile()    /* return PK-type error code */
{
    int error, error_in_archive;


/*---------------------------------------------------------------------------
    Open the zipfile for reading and in BINARY mode to prevent CR/LF trans-
    lation, which would corrupt the bitstreams.
  ---------------------------------------------------------------------------*/

#ifdef VMS
    if (check_format())         /* check for variable-length format */
        return PK_ERR;
#endif /* VMS */

    if (open_input_file())      /* this should never happen, given the */
        return PK_NOZIP;        /*   stat() test in main(), but... */

/*---------------------------------------------------------------------------
    Reconstruct the various PK signature strings, and find and process the
    end-of-central-directory header.  Need only check last 65557 bytes of
    zipfile:  comment may be up to 65535, end-of-central-directory record
    is 18 bytes, and signature itself is 4 bytes.  Add some to allow for
    appended garbage.
  ---------------------------------------------------------------------------*/

    if(strlen(local_hdr_sig) < 2) // must check, multiple load of dll will cause multple concatination
      strcat(local_hdr_sig, LOCAL_HDR_SIG);
    if(strlen(central_hdr_sig) < 2)
       strcat(central_hdr_sig, CENTRAL_HDR_SIG);
    if(strlen(end_central_sig) < 2)
       strcat(end_central_sig, END_CENTRAL_SIG);
/*  strcat(extd_local_sig, EXTD_LOCAL_SIG);  */

    if ((error_in_archive = find_end_central_dir(MIN(ziplen,66000L))) != 0  ||
        (error_in_archive = process_end_central_dir()) > PK_WARN) {
        close(zipfd);
        return error_in_archive;
    }
    if (zflag > 0) {
        close(zipfd);
        return error_in_archive;
    }

/*---------------------------------------------------------------------------
    Test the end-of-central-directory info for incompatibilities and incon-
    sistencies.
  ---------------------------------------------------------------------------*/

    error = ((ecrec.number_this_disk == 1) &&
             (ecrec.num_disk_with_start_central_dir == 1));

    if ((ecrec.number_this_disk == 0) || error) {
        if (error) {
            fprintf(stderr,
     "\n     Warning:  zipfile claims to be disk 2 of a two-part archive;\n\
     attempting to process anyway.  If no further errors occur, this\n\
     archive was probably created by PAK v2.51 or earlier.  This bug\n\
     was reported to NoGate in March 1991 and was supposed to have been\n\
     fixed by mid-1991; as of mid-1992 it still hadn't been.\n\n");
            error_in_archive = PK_WARN;
        }
        if ((extra_bytes = real_ecrec_offset - expect_ecrec_offset) < 0) {
            fprintf(stderr, "\nerror:  missing %ld bytes in zipfile (\
attempting to process anyway)\n\n", -extra_bytes);
            error_in_archive = PK_ERR;
        } else if (extra_bytes > 0) {
            if ((ecrec.offset_start_central_directory == 0) &&
                (ecrec.size_central_directory != 0))   /* zip 1.5 -go bug */
            {
                fprintf(stderr, "\nerror:  NULL central directory offset (\
attempting to process anyway)\n\n");
                ecrec.offset_start_central_directory = extra_bytes;
                extra_bytes = 0;
                error_in_archive = PK_ERR;
            } else {
                fprintf(stderr, "\nwarning:  extra %ld bytes at beginning or\
 within zipfile\n          (attempting to process anyway)\n\n", extra_bytes);
                error_in_archive = PK_WARN;
            }
        }

    /*-----------------------------------------------------------------------
        Check for empty zipfile and exit now if so.
      -----------------------------------------------------------------------*/

        if (expect_ecrec_offset == 0L  &&  ecrec.size_central_directory == 0) {
            fprintf(stderr, "warning:  zipfile is empty\n");
            close(zipfd);
            return (error_in_archive > PK_WARN)? error_in_archive : PK_WARN;
        }

    /*-----------------------------------------------------------------------
        Compensate for missing or extra bytes, and seek to where the start
        of central directory should be.  If header not found, uncompensate
        and try again (necessary for at least some Atari archives created
        with STZIP, as well as archives created by J.H. Holm's ZIPSPLIT 1.1).
      -----------------------------------------------------------------------*/

        LSEEK( ecrec.offset_start_central_directory )
        if ((readbuf(sig, 4) <= 0) || strncmp(sig, central_hdr_sig, 4)) {
            longint tmp = extra_bytes;

            extra_bytes = 0;
            LSEEK( ecrec.offset_start_central_directory )
            if ((readbuf(sig, 4) <= 0) || strncmp(sig, central_hdr_sig, 4)) {
                fprintf(stderr,
            "error:  start of central directory not found; zipfile corrupt.\n");
                fprintf(stderr, ReportMsg);
                close(zipfd);
                return PK_BADERR;
            }
            fprintf(stderr, "error:  reported length of central directory is \
%d bytes too long\n        (Atari STZIP zipfile?  J.H.Holm ZIPSPLIT 1.1 \
zipfile?).\n        Compensating...\n\n", -tmp);
            error_in_archive = PK_ERR;
        }

    /*-----------------------------------------------------------------------
        Seek to the start of the central directory one last time, since we
        have just read the first entry's signature bytes; then list, extract
        or test member files as instructed, and close the zipfile.
      -----------------------------------------------------------------------*/

        LSEEK( ecrec.offset_start_central_directory )
        if (vflag)
            error = list_files();               /* LIST 'EM */
        else
            error = extract_or_test_files();    /* EXTRACT OR TEST 'EM */
        if (error > error_in_archive)   /* don't overwrite stronger error */
            error_in_archive = error;   /*  with (for example) a warning */
    } else {
        fprintf(stderr, "\nerror:  zipfile is part of multi-disk archive \
(sorry, not supported).\n");
        error_in_archive = PK_FIND;
    }

    close(zipfd);
    return error_in_archive;

} /* end function process_zipfile() */





/**************************************/
/* Function process_end_central_dir() */
/**************************************/

int process_end_central_dir()    /* return PK-type error code */
{
    int error = PK_COOL;


/*---------------------------------------------------------------------------
    Get the zipfile comment (up to 64KB long), if any, and print it out.
    Then position the file pointer to the beginning of the central directory
    and fill buffer.
  ---------------------------------------------------------------------------*/

#ifdef MSWIN
    cchComment = ecrec.zipfile_comment_length; /* save for comment button */
    if (ecrec.zipfile_comment_length && (zflag > 0)) {
#else /* !MSWIN */
    if (ecrec.zipfile_comment_length && !(zflag < 0) && !quietflg) {
        if (zflag == 0)
            printf("[%s] comment:\n", zipfn);
#endif /* ?MSWIN */
        if (do_string(ecrec.zipfile_comment_length,DISPLAY)) {
            fprintf(stderr, "\ncaution:  zipfile comment truncated\n");
            error = PK_WARN;
        }
    }
    return error;

} /* end function process_end_central_dir() */





/* also referenced in UpdateListBox() in updatelb.c (Windows version) */
char *Headers[][2] = {
    {" Length    Date    Time    Name",
     " ------    ----    ----    ----"},
    {" Length  Method   Size  Ratio   Date    Time   CRC-32     Name",
     " ------  ------   ----  -----   ----    ----   ------     ----"}
};

/*************************/
/* Function list_files() */
/*************************/

int list_files()    /* return PK-type error code */
{
    int do_this_file=FALSE, ratio, error, error_in_archive=PK_COOL;
    int which_hdr=(vflag>1), date_format;
    ush j, yr, mo, dy, hh, mm, members=0;
    ulg tot_csize=0L, tot_ucsize=0L;
#ifdef OS2
    ulg tot_easize=0L, tot_eafiles=0L, ea_size;
#endif
#ifdef MSWIN
    PSTR psLBEntry;  /* list box entry */
#endif
    min_info info;
    static char *method[NUM_METHODS+1] =
        {"Stored", "Shrunk", "Reduce1", "Reduce2", "Reduce3", "Reduce4",
         "Implode", "Token", "Deflate", unkn};



/*---------------------------------------------------------------------------
    Unlike extract_or_test_files(), this routine confines itself to the cen-
    tral directory.  Thus its structure is somewhat simpler, since we can do
    just a single loop through the entire directory, listing files as we go.

    So to start off, print the heading line and then begin main loop through
    the central directory.  The results will look vaguely like the following:

  Length  Method   Size  Ratio   Date    Time   CRC-32     Name ("^" ==> case
  ------  ------   ----  -----   ----    ----   ------     ----   conversion)
   44004  Implode  13041  71%  11-02-89  19:34  8b4207f7   Makefile.UNIX
    3438  Shrunk    2209  36%  09-15-90  14:07  a2394fd8  ^dos-file.ext
  ---------------------------------------------------------------------------*/

    pInfo = &info;
    date_format = dateformat();

#ifndef MSWIN
    if (quietflg < 2)
        if (U_flag)
            printf("%s\n%s\n", Headers[which_hdr][0], Headers[which_hdr][1]);
        else
            printf("%s (\"^\" ==> case\n%s   conversion)\n", 
              Headers[which_hdr][0], Headers[which_hdr][1]);
#endif /* !MSWIN */

    if(g_bEnumItems && ecrec.total_entries_central_dir > 0)
    {
       g_EnumCount = ecrec.total_entries_central_dir;
       g_pItems = malloc(sizeof(char*) * g_EnumCount);
    }

    for (j = 0; j < ecrec.total_entries_central_dir; ++j) {

        if (readbuf(sig, 4) <= 0)
            return PK_EOF;
        if (strncmp(sig, central_hdr_sig, 4)) {  /* just to make sure */
            fprintf(stderr, CentSigMsg, j);  /* sig not found */
            fprintf(stderr, ReportMsg);   /* check binary transfers */
            return PK_BADERR;
        }
        /* process_cdir_file_hdr() sets pInfo->lcflag: */
        if ((error = process_cdir_file_hdr()) != PK_COOL)
            return error;       /* only PK_EOF defined */

        /*
         * We could DISPLAY the filename instead of storing (and possibly trun-
         * cating, in the case of a very long name) and printing it, but that
         * has the disadvantage of not allowing case conversion--and it's nice
         * to be able to see in the listing precisely how you have to type each
         * filename in order for unzip to consider it a match.  Speaking of
         * which, if member names were specified on the command line, check in
         * with match() to see if the current file is one of them, and make a
         * note of it if it is.
         */

        if ((error = do_string(crec.filename_length, FILENAME)) != PK_COOL) {
            error_in_archive = error;             /*  ^--(uses pInfo->lcflag) */
            if (error > PK_WARN)   /* fatal:  can't continue */
                return error;
        }
        if (extra_field != (uch *)NULL)
            free(extra_field);
        extra_field = (uch *)NULL;
        if ((error = do_string(crec.extra_field_length, EXTRA_FIELD)) != 0) {
            error_in_archive = error;  
            if (error > PK_WARN)      /* fatal */
                return error;
        }
        if (!process_all_files) {   /* check if specified on command line */
            char **pfn = pfnames-1;

            do_this_file = FALSE;
            while (*++pfn)
                if (match(filename, *pfn)) {
                    do_this_file = TRUE;
                    break;       /* found match, so stop looping */
                }
            if (do_this_file) {  /* check if this is an excluded file */
                char **pxn = pxnames-1;

                while (*++pxn)
                    if (match(filename, *pxn)) {
                        do_this_file = FALSE;
                        break;
                    }
            }
        }
        /*
         * If current file was specified on command line, or if no names were
         * specified, do the listing for this file.  Otherwise, get rid of the
         * file comment and go back for the next file.
         */

        if (process_all_files || do_this_file) {

            yr = (((crec.last_mod_file_date >> 9) & 0x7f) + 80) % (unsigned)100;
            mo = (crec.last_mod_file_date >> 5) & 0x0f;
            dy = crec.last_mod_file_date & 0x1f;

            /* twist date so it displays according to national convention */
            switch (date_format) {
                case DF_YMD:
                    hh = mo; mo = yr; yr = dy; dy = hh; break;
                case DF_DMY:
                    hh = mo; mo = dy; dy = hh;
            }
            hh = (crec.last_mod_file_time >> 11) & 0x1f;
            mm = (crec.last_mod_file_time >> 5) & 0x3f;

            csize = (longint) crec.csize;
            ucsize = (longint) crec.ucsize;
            if (crec.general_purpose_bit_flag & 1)
                csize -= 12;    /* if encrypted, don't count encrypt hdr */

            ratio = (ucsize == 0) ? 0 :   /* .zip can have 0-length members */
                ((ucsize > 2000000L) ?    /* risk signed overflow if mult. */
                (int) ((ucsize-csize) / (ucsize/1000L)) + 5 :   /* big */
                (int) ((1000L*(ucsize-csize)) / ucsize) + 5);   /* small */

#if 0       /* GRR/Euro:  add this?  define p above */
#if defined(DOS_OS2) || defined(UNIX)
            for (p = filename;  *p;  ++p)
                if (!isprint(*p))
                    *p = '?';  /* change non-printable chars to '?' */
#endif /* DOS_OS2 || UNIX */
#endif /* 0 */

#ifdef MSWIN
#ifdef NEED_EARLY_REDRAW
            /* turn on listbox redrawing just before adding last line */
            if (j == (ecrec.total_entries_central_dir-1))
                (void)SendMessage(hWndList, WM_SETREDRAW, TRUE, 0L);
#endif /* NEED_EARLY_REDRAW */
            psLBEntry =
              (PSTR)LocalAlloc(LMEM_FIXED, FILNAMSIZ+LONG_FORM_FNAME_INX);
            switch (which_hdr) {
                case 0:   /* short form */
                    OemToAnsi(filename, filename);  /* translate to ANSI */
                    wsprintf(psLBEntry, "%7ld  %02u-%02u-%02u  %02u:%02u  %c%s",
                      (long)ucsize, mo, dy, yr, hh, mm, (pInfo->lcflag?'^':' '),
                      (LPSTR)filename);
                    SendMessage(hWndList, LB_ADDSTRING, 0,
                      (LONG)(LPSTR)psLBEntry);
                    break;
                case 1:   /* verbose */
                    OemToAnsi(filename, filename);  /* translate to ANSI */
                    wsprintf(psLBEntry, 
                 "%7ld  %-7s%7ld %3d%%  %02u-%02u-%02u  %02u:%02u  %08lx  %c%s",
                      (long)ucsize, (LPSTR)method[methnum], (long)csize,
                      ratio/10, mo, dy, yr, hh, mm, (unsigned long)crec.crc32,
                      (pInfo->lcflag?'^':' '), (LPSTR)filename);
                    SendMessage(hWndList, LB_ADDSTRING, 0,
                      (LONG)(LPSTR)psLBEntry);
            }
            LocalFree((HANDLE)psLBEntry);
#else /* !MSWIN */
            if (which_hdr == 0)   /* short form */
                printf("%7ld  %02u-%02u-%02u  %02u:%02u  %c%s\n", ucsize,
                  mo, dy, yr, hh, mm, (pInfo->lcflag?'^':' '), filename);
            else                  /* verbose */
                printf(
              "%7ld  %-7s%7ld %3d%%  %02u-%02u-%02u  %02u:%02u  %08lx  %c%s\n",
                  ucsize, method[methnum], csize, ratio/10, mo, dy, yr,
                  hh, mm, crec.crc32, (pInfo->lcflag?'^':' '), filename);
#endif /* ?MSWIN */

            error = do_string(crec.file_comment_length, QCOND? DISPLAY : SKIP);
            if (error) {
                error_in_archive = error;  /* might be just warning */
                if (error > PK_WARN)       /* fatal */
                    return error;
            }
            tot_ucsize += (ulg)ucsize;
            tot_csize += (ulg)csize;
            ++members;
#ifdef OS2
            if ((ea_size = SizeOfEAs(extra_field)) != 0) {
                tot_easize += ea_size;
                tot_eafiles++;
            }
#endif
        } else {        /* not listing this file */
            SKIP_(crec.file_comment_length)
        }


        if(g_bEnumItems)
        {
           g_pItems[j] = malloc(strlen(filename)+1);
           strcpy(g_pItems[j],filename);
        }
    } /* end for-loop (j: files in central directory) */

/*---------------------------------------------------------------------------
    Print footer line and totals (compressed size, uncompressed size, number
    of members in zipfile).
  ---------------------------------------------------------------------------*/

    ratio = (tot_ucsize == 0) ? 
        0 : ((tot_ucsize > 4000000L) ?   /* risk unsigned overflow if mult. */
        (int)((tot_ucsize - tot_csize) / (tot_ucsize/1000L)) + 5 :
        (int)((tot_ucsize - tot_csize) * 1000L / tot_ucsize) + 5 );

    if (quietflg < 2) {
#ifdef MSWIN
        /* Display just the totals since the dashed lines get displayed
         * in UpdateListBox(). Get just enough space to display total. */
        if (which_hdr)   /* verbose */
            wsprintf(lpumb->szTotalsLine, 
              "%7lu         %7lu %3d%%                              %-7u", 
              (ulg)tot_ucsize, (ulg)tot_csize, ratio/10, members);
        else             /* short */
            wsprintf(lpumb->szTotalsLine, "%7lu                    %-7u", 
              (ulg)tot_ucsize, members);
#else /* !MSWIN */
        if (which_hdr)   /* verbose */
            printf(" ------          ------  ---                \
              -------\n%7lu         %7lu %3d%%                \
              %-7u\n", tot_ucsize, tot_csize, ratio/10, members);
        else             /* short */
            printf(" ------                    -------\n%7lu      \
              %-7u\n", tot_ucsize, members);
#endif /* ?MSWIN */
#ifdef OS2
        if (tot_eafiles && tot_easize)
            printf("\n%ld file%s %ld bytes of EA's attached.\n", tot_eafiles, 
              tot_eafiles == 1 ? " has" : "s have a total of", tot_easize);
#endif
    }
/*---------------------------------------------------------------------------
    Double check that we're back at the end-of-central-directory record.
  ---------------------------------------------------------------------------*/

    readbuf(sig, 4);
    if (strncmp(sig, end_central_sig, 4)) {     /* just to make sure again */
        fprintf(stderr, EndSigMsg);  /* didn't find end-of-central-dir sig */
        error_in_archive = PK_WARN;
    }
    return error_in_archive;

} /* end function list_files() */





/**************************************/
/*  Function process_cdir_file_hdr()  */
/**************************************/

int process_cdir_file_hdr()    /* return PK-type error code */
{
    int error;


/*---------------------------------------------------------------------------
    Get central directory info, save host and method numbers, and set flag
    for lowercase conversion of filename, depending on the OS from which the
    file is coming.
  ---------------------------------------------------------------------------*/

    if ((error = get_cdir_file_hdr()) != 0)
        return error;

    pInfo->hostnum = MIN(crec.version_made_by[1], NUM_HOSTS);
/*  extnum = MIN(crec.version_needed_to_extract[1], NUM_HOSTS); */
    methnum = MIN(crec.compression_method, NUM_METHODS);
    if (methnum == NUM_METHODS)
        sprintf(unkn, "Unk:%03d", crec.compression_method);

    pInfo->lcflag = 0;
    if (!U_flag)   /* as long as user hasn't specified case-preservation */
        switch (pInfo->hostnum) {
            case FS_FAT_:     /* PKZIP and zip -k store in uppercase */
            case VM_CMS_:     /* all caps? */
            case CPM_:        /* like DOS, right? */
        /*  case ATARI_:      ? */
        /*  case Z_SYSTEM_:   ? */
            case TOPS20_:
        /*  case QDOS_:       ? */
                pInfo->lcflag = 1;   /* convert filename to lowercase */
                break;

        /*  case VMS_:   VMS Zip converts filenames to lowercase */
            default:     /* AMIGA_, UNIX_, (ATARI_), FS_HPFS_, MAC_, */
                break;   /*  (Z_SYSTEM_), FS_NTFS_:  no conversion */
        }

    return PK_COOL;

} /* end function process_cdir_file_hdr() */





/***************************************/
/*  Function process_local_file_hdr()  */
/***************************************/

int process_local_file_hdr()    /* return PK-type error code */
{
    local_byte_hdr byterec;


/*---------------------------------------------------------------------------
    Read the next local file header and do any necessary machine-type con-
    versions (byte ordering, structure padding compensation--do so by copy-
    ing the data from the array into which it was read (byterec) to the
    usable struct (lrec)).
  ---------------------------------------------------------------------------*/

    if (readbuf((char *)byterec, LREC_SIZE) <= 0)
        return PK_EOF;

    lrec.version_needed_to_extract[0] = byterec[L_VERSION_NEEDED_TO_EXTRACT_0];
    lrec.version_needed_to_extract[1] = byterec[L_VERSION_NEEDED_TO_EXTRACT_1];

    lrec.general_purpose_bit_flag = makeword(&byterec[L_GENERAL_PURPOSE_BIT_FLAG]);
    lrec.compression_method = makeword(&byterec[L_COMPRESSION_METHOD]);
    lrec.last_mod_file_time = makeword(&byterec[L_LAST_MOD_FILE_TIME]);
    lrec.last_mod_file_date = makeword(&byterec[L_LAST_MOD_FILE_DATE]);
    lrec.crc32 = makelong(&byterec[L_CRC32]);
    lrec.csize = makelong(&byterec[L_COMPRESSED_SIZE]);
    lrec.ucsize = makelong(&byterec[L_UNCOMPRESSED_SIZE]);
    lrec.filename_length = makeword(&byterec[L_FILENAME_LENGTH]);
    lrec.extra_field_length = makeword(&byterec[L_EXTRA_FIELD_LENGTH]);

    csize = (longint) lrec.csize;
    ucsize = (longint) lrec.ucsize;

    if ((lrec.general_purpose_bit_flag & 8) != 0) {
        /* can't trust local header, use central directory: */
        lrec.crc32 = pInfo->crc;
        csize = (longint)(lrec.csize = pInfo->compr_size);
    }

    return PK_COOL;

} /* end function process_local_file_hdr() */
