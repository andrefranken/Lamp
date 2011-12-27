/*---------------------------------------------------------------------------

  unzip.h

  This header file is used by all of the UnZip source files.  Its contents
  are divided into seven more-or-less separate sections:  predefined macros,
  OS-dependent includes, (mostly) OS-independent defines, typedefs, function 
  prototypes (or "forward declarations," in the case of non-ANSI compilers),
  macros, and global-variable declarations.

  ---------------------------------------------------------------------------*/



#ifndef __unzip_h   /* prevent multiple inclusions */
#define __unzip_h

#define boolean IGBOOL

/* Define CONSOLE_APP if you want to print diagnostics */
/*   This controlls wether standard output and standard input are used */
#define CONSOLE_APP 

/*****************************************/
/*  Predefined, Machine-specific Macros  */
/*****************************************/

#if defined(__GO32__) && defined(unix)   /* DOS extender */
#  undef unix
#endif

#if defined(unix) || defined(__convexc__) || defined(M_XENIX)
#  ifndef UNIX
#    define UNIX
#  endif /* !UNIX */
#endif /* unix || __convexc__ || M_XENIX */

#ifdef __COMPILER_KCC__
#  include <c-env.h>
#  ifdef SYS_T20
#    define TOPS20
#  endif
#endif /* __COMPILER_KCC__ */

/* define MSDOS for Turbo C (unless OS/2) and Power C as well as Microsoft C */
#ifdef __POWERC
#  define __TURBOC__
#  define MSDOS
#endif /* __POWERC */
#if defined(__MSDOS__) && (!defined(MSDOS))   /* just to make sure */
#  define MSDOS
#endif

#if defined(linux) && (!defined(LINUX))
#  define LINUX
#endif

/* use prototypes and ANSI libraries if __STDC__, or Microsoft or Borland C, or
 * Silicon Graphics, or Convex?, or IBM C Set/2, or GNU gcc/emx, or Watcom C,
 * or Macintosh, or Windows NT, or Sequent, or Atari.
 */
#if __STDC__ || defined(MSDOS) || defined(sgi)  /* || defined(__convexc__) */
#  ifndef PROTO
#    define PROTO
#  endif
#  define MODERN
#endif
#if defined(__IBMC__) || defined(__EMX__) || defined(__WATCOMC__)
#  ifndef PROTO
#    define PROTO
#  endif
#  define MODERN
#endif
#if defined(THINK_C) || defined(MPW) || defined(WIN32) || defined(_SEQUENT_)
#  ifndef PROTO
#    define PROTO
#  endif
#  define MODERN
#endif
#if defined(ATARI_ST)
#  ifndef PROTO
#    define PROTO
#  endif
#  define MODERN
#endif

/* turn off prototypes if requested */
#if defined(NOPROTO) && defined(PROTO)
#  undef PROTO
#endif

/* used to remove arguments in function prototypes for non-ANSI C */
#ifdef PROTO
#  define OF(a) a
#else /* !PROTO */
#  define OF(a) ()
#endif /* ?PROTO */

/* bad or (occasionally?) missing stddef.h: */
#if defined(M_XENIX) || defined(DNIX) || (defined(__GNUC__) && defined(sun))
#  define NO_STDDEF_H
#endif

/* cannot depend on MODERN for presence of stdlib.h */
#if defined(__GNUC__) || defined(apollo)   /* both define __STDC__ */
#  if (!defined(__EMX__) && !defined(__386BSD__) && !defined(LINUX))
#    define NO_STDLIB_H
#  endif
#endif /* __GNUC__ || apollo */

#if defined(__SYSTEM_FIVE) || defined(M_SYSV) || defined(M_SYS5)
#  ifndef SYSV
#    define SYSV
#  endif /* !SYSV */
#endif /* __SYSTEM_FIVE || M_SYSV || M_SYS5 */

#if defined(SYSV) || defined(CRAY) || defined(LINUX)
#  ifndef TERMIO
#    define TERMIO
#  endif /* !TERMIO */
#endif /* SYSV || CRAY || LINUX */

#if defined(ultrix) || defined(bsd4_2) || defined(sun) || defined(pyr)
#  if (!defined(BSD)) && (!defined(SYSV))
#    define BSD
#  endif /* !BSD && !SYSV */
#endif /* ultrix || bsd4_2 || sun || pyr */
#if defined(__convexc__) || defined(TOPS20) || defined(__386BSD__)
#  if (!defined(BSD)) && (!defined(SYSV))
#    define BSD
#  endif /* !BSD && !SYSV */
#endif /* __convexc__ || TOPS20 || __386BSD__ */

#ifdef pyr  /* Pyramid */
#  ifndef ZMEM
#    define ZMEM
#  endif /* !ZMEM */
#endif /* pyr */

#ifdef CRAY
#  ifdef ZMEM
#    undef ZMEM
#  endif /* ZMEM */
#endif /* CRAY */

/* AMIGADOS2 implies AMIGA (but usually predefined by compiler) */
#if defined(AMIGADOS2) && (!defined(AMIGA))
#  define AMIGA
#endif

/* the i386 test below is to catch SCO Unix (which has redefinition
 * warnings if param.h is included), but it probably doesn't hurt if
 * other 386 Unixes get nailed, too...except now that 386BSD and BSDI
 * exist.  Sigh.  <sys/param.h> is mostly included for "BSD", I think.
 * [An alternate fix for SCO Unix is below.]
 */
#if defined(MINIX) || defined(__386BSD__) || (defined(i386) && defined(unix))
#  define NO_PARAM_H
#endif /* MINIX || (i386 && unix && !BSD) */





/***************************/
/*  OS-Dependent Includes  */    /* GRR:  this needs overhaul!! */
/***************************/

#ifndef MINIX            /* Minix needs it after all the other includes (?) */
#  include <stdio.h>
#endif
#include <ctype.h>       /* skip for VMS, to use tolower() function? */
#include <errno.h>       /* used in mapname() */
#ifdef VMS
#  include <types.h>     /* (placed up here instead of in VMS section below */
#  include <stat.h>      /* because types.h is used in some other headers) */
#else /* !VMS */
#  ifdef AMIGA
#    ifdef AZTEC_C
#      include <clib/dos_protos.h>
#      include <pragmas/dos_lib.h>
#      include "amiga/z-stat.h"
#      define MODERN
#      define O_BINARY 0
#    else /* !AZTEC_C */
#      include <sys/types.h>         /* must come before stat.h */
#      include <sys/stat.h>
#      include <proto/dos.h>
#      define O_BINARY   O_RAW
#    endif /* ?AZTEC_C */
#    include <time.h>
#    define dup
#  else /* !AMIGA */
#    if (!defined(THINK_C)) && (!defined(MPW)) && (!defined(ATARI_ST))
#      include <sys/types.h>         /* off_t, time_t, dev_t, ... */
#      include <sys/stat.h>
#    endif /* !THINK_C && !MPW */
#  endif /* ?AMIGA */
#endif /* ?VMS */

#ifdef MODERN
#  ifndef NO_STDDEF_H
#    include <stddef.h>
#  endif
#  ifndef NO_STDLIB_H
#    include <stdlib.h>    /* standard library prototypes, malloc(), etc. */
#  endif
#  if defined(__386BSD__) || defined(LINUX)
#    include <unistd.h>
#  endif
#  include <string.h>      /* defines strcpy, strcmp, memcpy, etc. */
   typedef size_t extent;
   typedef void voidp;
#else /* !MODERN */
   char *malloc();
   char *strchr(), *strrchr();
   long lseek();
   typedef unsigned int extent;
#  define void int
   typedef char voidp;
#endif /* ?MODERN */

/* this include must be down here for SysV.4, for some reason... */
#include <signal.h>      /* used in unzip.c, file_io.c */

/*---------------------------------------------------------------------------
    Next, a word from our Unix (mostly) sponsors:
  ---------------------------------------------------------------------------*/

#if defined(UNIX) || defined(TOPS20)
#  ifndef NO_PARAM_H
#    if 0  /* [GRR: this is an alternate fix for SCO's redefinition bug] */
#    ifdef NGROUPS_MAX
#      undef NGROUPS_MAX       /* SCO bug:  defined again in <param.h> */
#    endif /* NGROUPS_MAX */
#    endif /* 0 */
#    include <sys/param.h>     /* conflict with <sys/types.h>, some systems? */
#  endif /* !NO_PARAM_H */

#  ifdef BSD
#    include <sys/time.h>
#    include <sys/timeb.h>
#  else /* !BSD */
#    ifndef MINIX
#      define NO_MKDIR   /* for mapname() */
#    endif
     struct tm *gmtime(), *_localtime32();
#  endif /* ?BSD */

#  if defined(_AIX) || defined(LINUX) || (!defined(BSD))
#    include <time.h>
#  endif

#  if defined(__386BSD__) || defined(_POSIX_SOURCE) || defined(MAINWIN)
#    include <utime.h>
#  else
#   ifndef _HPUX_SOURCE
     struct utimbuf {
         __time32_t actime;     /* new access time */
         __time32_t modtime;    /* new modification time */
     };
#   endif /* _HPUX_SOURCE */
#  endif /* ?(__386BSD__ || _POSIX_SOURCE) */
#endif /* UNIX || TOPS20 */

#if defined(V7) || defined(BSD)
#  define strchr    index
#  define strrchr   rindex
#endif

#if 0   /* GRR:  is this even used? */
#ifndef BSIZE
#  ifdef MINIX
#    define BSIZE         1024       /* disk block size */
#  else /* !MINIX */
#    ifdef DEV_BSIZE
#      define BSIZE       DEV_BSIZE
#    else
#      define BSIZE       512
#    endif
#  endif /* ?MINIX */
#endif /* !BSIZE */
#endif /* 0 */

/*---------------------------------------------------------------------------
    And now, our MS-DOS and OS/2 corner:
  ---------------------------------------------------------------------------*/

#ifdef MSDOS
#  include <dos.h>            /* for REGS macro (TC) or _dos_setftime (MSC) */
#  ifdef __TURBOC__           /* includes Power C                           */
#    include <sys/timeb.h>    /* for structure ftime                        */
#    ifndef __BORLANDC__      /* there appears to be a bug (?) in Borland's */
#      include <mem.h>        /*   MEM.H related to __STDC__ and far poin-  */
#    endif                    /*   ters. (dpk)  [mem.h included for memcpy] */
#  else /* !__TURBOC__ */
#    ifndef MSC               /* assume Microsoft's compiler and fake ID if */
#      define MSC             /*   necessary (it is in 5.0; apparently not  */
#    endif                    /*   in 5.1 and 6.0)                          */
#  endif /* ?__TURBOC__ */
#endif /* MSDOS */

#if defined(_MSC_VER) && (!defined(MSC))
#  define MSC
#endif

#if defined(__IBMC__) && defined(__OS2__)
#  define S_IFMT 0xF000
#  define timezone _timezone
#endif

#ifdef __WATCOMC__
#  define __32BIT__
#  undef far
#  define far
#endif

#ifdef __EMX__
#  ifndef __32BIT__
#    define __32BIT__
#  endif
#  define far
#endif /* __EMX__ */

#ifdef MSC                    /* should be defined for all versions of MSC */
#  ifndef _MSC_VER            /* new with 5.1 or 6.0 ... */
#    define DECLARE_ERRNO     /* not declared in errno.h in 5.0 or earlier? */
#  endif
#endif

#if defined(MSDOS) || defined(OS2)
#  include <io.h>             /* lseek(), open(), setftime(), dup(), creat() */
#  include <time.h>           /* _localtime32() */
#  ifdef __GO32__
     int setmode(int, int);   /* not in djgcc's include files */
#  endif
#endif

#ifdef OS2                    /* defined for all OS/2 compilers */
#  ifdef MSDOS
#    undef MSDOS
#  endif
#  ifdef isupper
#    undef isupper
#  endif
#  ifdef tolower
#    undef tolower
#  endif
#  define isupper(x)   IsUpperNLS((unsigned char)(x))
#  define tolower(x)   ToLowerNLS((unsigned char)(x))
#endif

#ifdef WIN32
/* S.B. Dont know why this was here, bad on NT: #  define getch() getchar() */
#  include <io.h>             /* read(), open(), etc. */
#  include <time.h>
#  include <memory.h>
#  include <direct.h>         /* mkdir() */
#  include <windows.h>
#ifdef FILE_IO_C
#     include <fcntl.h>
#     include <conio.h>
#     ifdef MAINWIN
#       include <sys/types.h>
#       include <utime.h>
#     else
#       include <sys\types.h>
#       include <sys\utime.h>
#     endif
#endif
#endif

/*---------------------------------------------------------------------------
    Followed by some VMS (and other) stuff:
  ---------------------------------------------------------------------------*/

#ifdef VMS
#  include <time.h>               /* the usual non-BSD time functions */
#  include <file.h>               /* same things as fcntl.h has */
#  include <rms.h>
#  define _MAX_PATH NAM$C_MAXRSS  /* to define FILNAMSIZ below */
#  define RETURN    return_VMS    /* VMS interprets return codes incorrectly */
#  define DIR_BEG  '['
#  define DIR_END  ']'
#  define DIR_EXT  ".dir"
#else /* !VMS */
#  ifndef THINK_C
#    define RETURN  return        /* only used in main() */
#  else
#    define RETURN(v) { int n; n=(v); fprintf(stderr,\
       "\npress <return> to continue "); while (getc(stdin)!='\n');\
       putc('\n', stderr); InitCursor(); goto start;}
#  endif
#  ifdef V7
#    define O_RDONLY  0
#    define O_WRONLY  1
#    define O_RDWR    2
#  else /* !V7 */
#    ifdef MTS
#      include <sys/file.h>     /* MTS uses this instead of fcntl.h */
#      include <timeb.h>
#      include <time.h>
#    else /* !MTS */
#      ifdef COHERENT           /* Coherent 3.10/Mark Williams C */
#        include <sys/fcntl.h>
#        define SHORT_NAMES
#        define tzset  settz
#      else /* !COHERENT */
#        ifndef ATARI_ST
#          include <fcntl.h>    /* O_BINARY for open() w/o CR/LF translation */
#        endif
#        ifdef TOPS20
           extern int open(), close(), read(), write();
           extern int stat(), unlink(), jsys(), fcntl();
           extern long lseek(), dup(), creat();
#          include <sys/file.h>
#          include <timex.h>
#          include <monsym.h>   /* get amazing monsym() macro */
#          define REALLY_SHORT_NAMES
#          define NO_MKDIR
#          define DIR_BEG  '<'
#          define DIR_END  '>'
#          define DIR_EXT  ".directory"
#        endif /* TOPS20 */
#      endif /* ?COHERENT */
#    endif /* ?MTS */
#  endif /* ?V7 */
#endif /* ?VMS */

/*---------------------------------------------------------------------------
    And some Mac stuff for good measure:
  ---------------------------------------------------------------------------*/

#ifdef THINK_C
#  define MACOS
#  ifndef __STDC__            /* if Think C hasn't defined __STDC__ ... */
#    define __STDC__ 1        /*   make sure it's defined: it needs it */
#  else /* __STDC__ defined */
#    if !__STDC__             /* sometimes __STDC__ is defined as 0; */
#      undef __STDC__         /*   it needs to be 1 or required header */
#      define __STDC__ 1      /*   files are not properly included. */
#    endif /* !__STDC__ */
#  endif /* ?defined(__STDC__) */
#endif /* THINK_C */

#ifdef MPW
#  define MACOS
#  include <Errors.h>
#  include <Files.h>
#  include <Memory.h>
#  include <Quickdraw.h>
#  include <ToolUtils.h>
#  define CtoPstr c2pstr
#  define PtoCstr p2cstr
#  ifdef CR
#    undef  CR
#  endif
#endif /* MPW */

#ifdef MACOS
#  define open(x,y) macopen(x,y, gnVRefNum, glDirID)
#  define close macclose
#  define read macread
#  define write macwrite
#  define lseek maclseek
#  define creat(x,y) maccreat(x, gnVRefNum, glDirID, gostCreator, gostType)
#  define stat(x,y) macstat(x,y,gnVRefNum, glDirID)
#  define dup

#  ifdef THINK_C
#    define CREATOR     'KAHL'
#  else
#    ifdef MCH_MACINTOSH
#      define CREATOR   'Manx'
#    else
#      define CREATOR   'MPS '
#    endif /* ?MCH_MACINTOSH */
#  endif /* ?THINK_C */

#  ifndef isascii
#    define isascii(c) ((unsigned char)(c) <= 0x3F)
#  endif

#  include "macstat.h"

typedef struct _ZipExtraHdr {
    unsigned short header;    /*    2 bytes */
    unsigned short data;      /*    2 bytes */
} ZIP_EXTRA_HEADER;

typedef struct _MacInfoMin {
    unsigned short header;    /*    2 bytes */
    unsigned short data;      /*    2 bytes */
    unsigned long signature;  /*    4 bytes */
    FInfo finfo;              /*   16 bytes */
    unsigned long lCrDat;     /*    4 bytes */
    unsigned long lMdDat;     /*    4 bytes */
    unsigned long flags ;     /*    4 bytes */
    unsigned long lDirID;     /*    4 bytes */
                              /*------------*/
} MACINFOMIN;                 /* = 40 bytes for size of data */

typedef struct _MacInfo {
    unsigned short header;    /*    2 bytes */
    unsigned short data;      /*    2 bytes */
    unsigned long signature;  /*    4 bytes */
    FInfo finfo;              /*   16 bytes */
    unsigned long lCrDat;     /*    4 bytes */
    unsigned long lMdDat;     /*    4 bytes */
    unsigned long flags ;     /*    4 bytes */
    unsigned long lDirID;     /*    4 bytes */
    char rguchVolName[28];    /*   28 bytes */
                              /*------------*/
} MACINFO;                    /* = 68 bytes for size of data */
#endif /* MACOS */

/*---------------------------------------------------------------------------
    Atari ST section:
  ---------------------------------------------------------------------------*/

#ifdef ATARI_ST
#  ifdef __TURBOC__
#    include <ext.h>   /* stat() */
#    include <tos.h>   /* OS-specific functions (Fdup) */
#  endif
#  include <time.h>
#  ifndef S_IFMT
#    define S_IFMT     (S_IFCHR | S_IFREG | S_IFDIR)
#  endif
#  ifndef O_BINARY
#    define O_BINARY   0
#  endif
#  define dup     Fdup
#  define mkdir   Dcreate
#endif

/*---------------------------------------------------------------------------
    And finally, some random extra stuff:
  ---------------------------------------------------------------------------*/

#ifdef MINIX
#  include <stdio.h>
#endif

#ifdef SHORT_NAMES                  /* Mark Williams C, ...? */
#  define extract_or_test_files     xtr_or_tst_files
#  define extract_or_test_member    xtr_or_tst_member
#endif

#ifdef REALLY_SHORT_NAMES           /* TOPS-20 linker:  first 6 chars */
#  define process_end_central_dir   XXpecd
#  define process_cdir_file_hdr     XXpcdfh
#  define process_local_file_hdr    XXplfh
#  define process_central_dir       XXpcd
#  define extract_or_test_files     XXxotf  /* necessary? */
#  define extract_or_test_member    XXxotm  /* necessary? */
#  define IsUpperNLS                XXiu
#  define ToLowerNLS                XXtl
#  define UpdateCRC                 XXuc
#  define check_for_newer           XXcfn
#  define overwrite_all             XXoa
#  define process_all_files         XXpaf
#  define extra_field               XXef
#  define explode_lit8              XXel8
#  define explode_lit4              XXel4
#  define explode_nolit8            XXnl8
#  define explode_nolit4            XXnl4
#  define cpdist8                   XXcpdist8
#  define ccinflate_codes             XXic
#  define inflate_stored            XXis
#  define inflate_fixed             XXif
#  define inflate_dynamic           XXid
#  define inflate_block             XXib
#  define maxcodemax                XXmax
#endif

#ifdef MTS
#  include <unix.h>                 /* some important non-ANSI routines */
#  define mkdir(s,n) (-1)           /* no "make directory" capability */
#  define EBCDIC                    /* set EBCDIC conversion on */
#endif





/*************/
/*  Defines  */
/*************/

#if defined(MSDOS) || defined(WIN32) || defined(OS2)
#  define DOS_NT_OS2
#endif

#if defined(MSDOS) || defined(OS2)
#  define DOS_OS2
#endif

#if defined(MSDOS) || defined(OS2) || defined(ATARI_ST)
#  define DOS_OS2_TOS
#endif

#if defined(MSDOS) || defined(ATARI_ST)
#  define DOS_TOS
#endif

#if defined(MSDOS) || defined(TOPS20) || defined(VMS)
#  define DOS_T20_VMS
#endif

#if defined(TOPS20) || defined(VMS)
#  define T20_VMS
#endif

#ifndef WSIZE
#  define WSIZE 0x8000       /* window size--must be a power of two, and */
#endif /* !WSIZE */          /*  at least 32K for zip's deflate method */

#define DIR_BLKSIZ    64     /* number of directory entries per block
                              *  (should fit in 4096 bytes, usually) */
#ifndef INBUFSIZ
#  define INBUFSIZ    2048   /* works for MS-DOS small model */
#endif /* !INBUFSIZ */

/*
 * If <limits.h> exists on most systems, should include that, since it may
 * define some or all of the following:  NAME_MAX, PATH_MAX, _POSIX_NAME_MAX,
 * _POSIX_PATH_MAX.
 */
#ifdef DOS_OS2_TOS
#  include <limits.h>
#endif

#ifdef _MAX_PATH
#  define FILNAMSIZ       (_MAX_PATH)
#else /* !_MAX_PATH */
#  define FILNAMSIZ       1025
#endif /* ?_MAX_PATH */

#ifndef PATH_MAX
#  ifdef MAXPATHLEN                /* defined in <sys/param.h> some systems */
#    define PATH_MAX      MAXPATHLEN
#  else
#    if FILENAME_MAX > 255         /* used like PATH_MAX on some systems */
#      define PATH_MAX    FILENAME_MAX
#    else
#      define PATH_MAX    (FILNAMSIZ - 1)
#    endif
#  endif /* ?MAXPATHLEN */
#endif /* !PATH_MAX */

#define OUTBUFSIZ         INBUFSIZ

#define ZSUFX             ".zip"
#define CENTRAL_HDR_SIG   "\113\001\002"   /* the infamous "PK" signature */
#define LOCAL_HDR_SIG     "\113\003\004"   /*  bytes, sans "P" (so unzip */
#define END_CENTRAL_SIG   "\113\005\006"   /*  executable not mistaken for */
#define EXTD_LOCAL_SIG    "\113\007\010"   /*  zipfile itself) */

#define SKIP              0    /* choice of activities for do_string() */
#define DISPLAY           1
#define FILENAME          2
#define EXTRA_FIELD       3

#define DOES_NOT_EXIST    -1   /* return values for check_for_newer() */
#define EXISTS_AND_OLDER  0
#define EXISTS_AND_NEWER  1

/* version_made_by codes (central dir):  make sure these */
/*  are not defined on their respective systems!! */
#define FS_FAT_           0    /* filesystem used by MS-DOS, OS/2, NT */
#define AMIGA_            1
#define VMS_              2
#define UNIX_             3
#define VM_CMS_           4
#define ATARI_            5
#define FS_HPFS_          6    /* filesystem used by OS/2, NT */
#define MAC_              7
#define Z_SYSTEM_         8
#define CPM_              9
#define TOPS20_           10
#define FS_NTFS_          11   /* filesystem used by Windows NT */
/* #define QDOS_          12?  */
#define NUM_HOSTS         12   /* index of last system + 1 */

#define STORED            0    /* compression methods */
#define SHRUNK            1
#define REDUCED1          2
#define REDUCED2          3
#define REDUCED3          4
#define REDUCED4          5
#define IMPLODED          6
#define TOKENIZED         7
#define DEFLATED          8
#define NUM_METHODS       9    /* index of last method + 1 */
/* don't forget to update list_files() appropriately if NUM_METHODS changes */

#define DF_MDY            0    /* date format 10/26/91 (USA only) */
#define DF_DMY            1    /* date format 26/10/91 (most of the world) */
#define DF_YMD            2    /* date format 91/10/26 (a few countries) */

#define UNZIP_VERSION     20   /* compatible with PKUNZIP 2.0 */
#define VMS_VERSION       42   /* if OS-needed-to-extract is VMS:  can do */

/*---------------------------------------------------------------------------
    True sizes of the various headers, as defined by PKWARE--so it is not
    likely that these will ever change.  But if they do, make sure both these
    defines AND the typedefs below get updated accordingly.
  ---------------------------------------------------------------------------*/
#define LREC_SIZE     26    /* lengths of local file headers, central */
#define CREC_SIZE     42    /*  directory headers, and the end-of-    */
#define ECREC_SIZE    18    /*  central-dir record, respectively      */

#define MAX_BITS      13                 /* used in unShrink() */
#define HSIZE         (1 << MAX_BITS)    /* size of global work area */

#define LF      10    /* '\n' on ASCII machines; must be 10 due to EBCDIC */
#define CR      13    /* '\r' on ASCII machines; must be 13 due to EBCDIC */
#define CTRLZ   26    /* DOS & OS/2 EOF marker (used in file_io.c, vms.c) */

#ifdef EBCDIC
#  define ascii_to_native(c)   ebcdic[(c)]
#  define NATIVE    "EBCDIC"
#endif

#ifdef MPW
#  define FFLUSH(f)   putc('\n',f)
#else /* !MPW */
#  define FFLUSH      fflush
#endif /* ?MPW */

#ifdef VMS
#  define ENV_UNZIP     "UNZIP_OPTS"      /* name of environment variable */
#  define ENV_ZIPINFO   "ZIPINFO_OPTS"
#else /* !VMS */
#  define ENV_UNZIP     "UNZIP"
#  define ENV_ZIPINFO   "ZIPINFO"
#endif /* ?VMS */

#ifdef QQ  /* Newtware version */
#  define QCOND   (!quietflg)   /* for no file comments with -vq or -vqq */
#else      /* (original) Bill Davidsen version  */
#  define QCOND   (which_hdr)   /* no way to kill file comments with -v, -l */
#endif

#ifndef TRUE
#  define TRUE      1   /* sort of obvious */
#endif
#ifndef FALSE
#  define FALSE     0
#endif

#ifndef SEEK_SET
#  define SEEK_SET  0
#  define SEEK_CUR  1
#  define SEEK_END  2
#endif

#ifndef S_IRUSR
#  define S_IRWXU       00700       /* read, write, execute: owner */
#  define S_IRUSR       00400       /* read permission: owner */
#  define S_IWUSR       00200       /* write permission: owner */
#  define S_IXUSR       00100       /* execute permission: owner */
#  define S_IRWXG       00070       /* read, write, execute: group */
#  define S_IRGRP       00040       /* read permission: group */
#  define S_IWGRP       00020       /* write permission: group */
#  define S_IXGRP       00010       /* execute permission: group */
#  define S_IRWXO       00007       /* read, write, execute: other */
#  define S_IROTH       00004       /* read permission: other */
#  define S_IWOTH       00002       /* write permission: other */
#  define S_IXOTH       00001       /* execute permission: other */
#endif /* !S_IRUSR */

/* Define OS-specific attributes for use on ALL platforms (the S_xxxx
 * versions of these are defined differently by different compilers and
 * operating systems) */

#ifdef ZIPINFO
#  define UNX_IFMT       0170000     /* Unix file type mask */
#  define UNX_IFDIR      0040000     /* Unix directory */
#  define UNX_IFREG      0100000     /* Unix regular file */
#  define UNX_IFSOCK     0140000     /* socket */  /* BSD, not SysV or Amiga */
#  define UNX_IFLNK      0120000     /* symbolic link */  /* not SysV, Amiga */
#  define UNX_IFBLK      0060000     /* block special */        /* not Amiga */
#  define UNX_IFCHR      0020000     /* character special */    /* not Amiga */
#  define UNX_IFIFO      0010000     /* fifo */     /* BCC, not MSC or Amiga */
#  define UNX_ISUID      04000       /* set user id on execution */
#  define UNX_ISGID      02000       /* set group id on execution */
#  define UNX_ISVTX      01000       /* directory permissions control */
#  define UNX_ENFMT      UNX_ISGID   /* record locking enforcement flag */
#  define AMI_IFMT       06000       /* Amiga file type mask */
#  define AMI_IFDIR      04000       /* Amiga directory */
#  define AMI_IFREG      02000       /* Amiga regular file */
#  define AMI_IHIDDEN    00200       /* to be supported in AmigaDOS 3.x */
#  define AMI_ISCRIPT    00100       /* executable script (text command file) */
#  define AMI_IPURE      00040       /* allow loading into resident memory */
#  define AMI_IARCHIVE   00020       /* not modified since bit was last set */
#  define AMI_IREAD      00010       /* can be opened for reading */
#  define AMI_IWRITE     00004       /* can be opened for writing */
#  define AMI_IEXECUTE   00002       /* executable image, a loadable runfile */
#  define AMI_IDELETE    00001       /* can be deleted */
#else /* !ZIPINFO */
#  if (defined(UNIX) && defined(S_IFLNK) && !defined(MTS))
#    define SYMLINKS
#    ifndef S_ISLNK
#      define S_ISLNK(m)   (((m) & S_IFMT) == S_IFLNK)
#    endif
#  endif /* UNIX && S_IFLNK && !MTS */
#endif /* ?ZIPINFO */





/**************/
/*  Typedefs  */
/**************/

/* defined by windows.h
typedef char              boolean;
*/
typedef long              longint;
typedef unsigned char     uch;  /* code assumes unsigned bytes; these type-  */
typedef unsigned short    ush;  /*  defs replace byte/UWORD/ULONG (which are */
typedef unsigned long     ulg;  /*  predefined on some systems) & match zip  */

typedef struct min_info {
    unsigned file_attr;      /* local flavor, as used by creat(), chmod()... */
    int hostnum;
    longint offset;
    ulg compr_size;          /* compressed size (needed if extended header) */
    ulg crc;                 /* crc (needed if extended header) */
    unsigned encrypted : 1;  /* file encrypted: decrypt before uncompressing */
    unsigned ExtLocHdr : 1;  /* use time instead of CRC for decrypt check */
    unsigned text : 1;       /* file is text or binary */
    unsigned lcflag : 1;     /* convert filename to lowercase */
} min_info;

typedef struct VMStimbuf {
    char *revdate;           /* (both correspond to Unix modtime/st_mtime) */
    char *credate;
} VMStimbuf;

/*---------------------------------------------------------------------------
    Zipfile work area declarations.
  ---------------------------------------------------------------------------*/

#if defined(MACOS) || defined(MALLOC_WORK)
   union work {
     struct {
       short *Prefix_of;            /* (8193 * sizeof(short)) */
       uch *Suffix_of;
       uch *Stack;
     } shrink;                      /* unShrink() */
     uch *Slide;                    /* explode(), inflate(), unReduce() */
   };

#else /* !(MACOS || MALLOC_WORK) */
   union work {
     struct {
#ifdef HSIZE2    /* needed to avoid errors on some machines? */
       short Prefix_of[HSIZE + 2];  /* (8194 * sizeof(short)) */
       uch Suffix_of[HSIZE + 2];    /* also s-f length_nodes (smaller) */
       uch Stack[HSIZE + 2];        /* also s-f distance_nodes (smaller) */
#else /* !HSIZE2 */
       short Prefix_of[HSIZE];      /* (8192 * sizeof(short)) */
       uch Suffix_of[HSIZE];        /* also s-f length_nodes (smaller) */
       uch Stack[HSIZE];            /* also s-f distance_nodes (smaller) */
#endif /* ?HSIZE2 */
     } shrink;
     uch Slide[WSIZE];
   };
#endif /* ?(MACOS || MALLOC_WORK) */

#define prefix_of   area.shrink.Prefix_of
#define suffix_of   area.shrink.Suffix_of
#define stack       area.shrink.Stack
#define slide       area.Slide

/*---------------------------------------------------------------------------
    Zipfile layout declarations.  If these headers ever change, make sure the
    xxREC_SIZE defines (above) change with them!
  ---------------------------------------------------------------------------*/

   typedef uch   local_byte_hdr[ LREC_SIZE ];
#      define L_VERSION_NEEDED_TO_EXTRACT_0     0
#      define L_VERSION_NEEDED_TO_EXTRACT_1     1
#      define L_GENERAL_PURPOSE_BIT_FLAG        2
#      define L_COMPRESSION_METHOD              4
#      define L_LAST_MOD_FILE_TIME              6
#      define L_LAST_MOD_FILE_DATE              8
#      define L_CRC32                           10
#      define L_COMPRESSED_SIZE                 14
#      define L_UNCOMPRESSED_SIZE               18
#      define L_FILENAME_LENGTH                 22
#      define L_EXTRA_FIELD_LENGTH              24

   typedef uch   cdir_byte_hdr[ CREC_SIZE ];
#      define C_VERSION_MADE_BY_0               0
#      define C_VERSION_MADE_BY_1               1
#      define C_VERSION_NEEDED_TO_EXTRACT_0     2
#      define C_VERSION_NEEDED_TO_EXTRACT_1     3
#      define C_GENERAL_PURPOSE_BIT_FLAG        4
#      define C_COMPRESSION_METHOD              6
#      define C_LAST_MOD_FILE_TIME              8
#      define C_LAST_MOD_FILE_DATE              10
#      define C_CRC32                           12
#      define C_COMPRESSED_SIZE                 16
#      define C_UNCOMPRESSED_SIZE               20
#      define C_FILENAME_LENGTH                 24
#      define C_EXTRA_FIELD_LENGTH              26
#      define C_FILE_COMMENT_LENGTH             28
#      define C_DISK_NUMBER_START               30
#      define C_INTERNAL_FILE_ATTRIBUTES        32
#      define C_EXTERNAL_FILE_ATTRIBUTES        34
#      define C_RELATIVE_OFFSET_LOCAL_HEADER    38

   typedef uch   ec_byte_rec[ ECREC_SIZE+4 ];
/*     define SIGNATURE                         0   space-holder only */
#      define NUMBER_THIS_DISK                  4
#      define NUM_DISK_WITH_START_CENTRAL_DIR   6
#      define NUM_ENTRIES_CENTRL_DIR_THS_DISK   8
#      define TOTAL_ENTRIES_CENTRAL_DIR         10
#      define SIZE_CENTRAL_DIRECTORY            12
#      define OFFSET_START_CENTRAL_DIRECTORY    16
#      define ZIPFILE_COMMENT_LENGTH            20


   typedef struct local_file_header {                 /* LOCAL */
       uch version_needed_to_extract[2];
       ush general_purpose_bit_flag;
       ush compression_method;
       ush last_mod_file_time;
       ush last_mod_file_date;
       ulg crc32;
       ulg csize;
       ulg ucsize;
       ush filename_length;
       ush extra_field_length;
   } local_file_hdr;

   typedef struct central_directory_file_header {     /* CENTRAL */
       uch version_made_by[2];
       uch version_needed_to_extract[2];
       ush general_purpose_bit_flag;
       ush compression_method;
       ush last_mod_file_time;
       ush last_mod_file_date;
       ulg crc32;
       ulg csize;
       ulg ucsize;
       ush filename_length;
       ush extra_field_length;
       ush file_comment_length;
       ush disk_number_start;
       ush internal_file_attributes;
       ulg external_file_attributes;
       ulg relative_offset_local_header;
   } cdir_file_hdr;

   typedef struct end_central_dir_record {            /* END CENTRAL */
       ush number_this_disk;
       ush num_disk_with_start_central_dir;
       ush num_entries_centrl_dir_ths_disk;
       ush total_entries_central_dir;
       ulg size_central_directory;
       ulg offset_start_central_directory;
       ush zipfile_comment_length;
   } ecdir_rec;





/*************************/
/*  Function Prototypes  */
/*************************/

#ifndef __
#  define __   OF
#endif

/*---------------------------------------------------------------------------
    Functions in unzip.c and/or zipinfo.c:
  ---------------------------------------------------------------------------*/

int    usage                     __((int error));
int    process_zipfile           __((void));
int    process_end_central_dir   __((void));
int    list_files                __((void));                      /* unzip.c */
int    process_cdir_file_hdr     __((void));                      /* unzip.c */
int    process_local_file_hdr    __((void));                      /* unzip.c */
int    process_central_dir       __((void));                    /* zipinfo.c */
int    long_info                 __((void));                    /* zipinfo.c */
int    short_info                __((void));                    /* zipinfo.c */
char   *zipinfo_time             __((ush *datez, ush *timez));  /* zipinfo.c */

/*---------------------------------------------------------------------------
    Functions in shared.c:
  ---------------------------------------------------------------------------*/

int    open_input_file           __((void));
int    readbuf                   __((char *buf, register unsigned len));
int    find_end_central_dir      __((longint searchlen));
int    get_cdir_file_hdr         __((void));
int    do_string                 __((unsigned int len, int option));
ush    makeword                  __((uch *b));
ulg    makelong                  __((uch *sig));
#ifdef ZMEM   /* MUST be ifdef'd because of conflicts with the standard def. */
   char *memset __((register char *, register char, register unsigned int));
   char *memcpy __((register char *, register char *, register unsigned int));
#endif /* ZMEM */

/*---------------------------------------------------------------------------
    Functions in extract.c:
  ---------------------------------------------------------------------------*/

int    extract_or_test_files     __((void));
/* static int   store_info               __((void)); */
/* static int   extract_or_test_member   __((void)); */
int    memextract                __((uch *, ulg, uch *, ulg));
int    FlushMemory               __((void));
int    ReadMemoryByte            __((ush *x));

/*---------------------------------------------------------------------------
    Decompression functions:
  ---------------------------------------------------------------------------*/

int    explode                   __((void));                    /* explode.c */

int    ccinflate                   __((void));                    /* inflate.c */

void   unReduce                  __((void));                   /* unreduce.c */
/* static void  LoadFollowers    __((void));                    * unreduce.c */

void   unShrink                  __((void));                   /* unshrink.c */
/* static void  partial_clear    __((void));                    * unshrink.c */

/*---------------------------------------------------------------------------
    Functions in file_io.c:
  ---------------------------------------------------------------------------*/

int    create_output_file        __((void));             /* file_io.c, vms.c */
int    FillBitBuffer             __((void));                    /* file_io.c */
int    ReadByte                  __((ush *x));                  /* file_io.c */
int    FlushOutput               __((void));             /* file_io.c, vms.c */
/* static int   dos2unix         __((unsigned char *, int));     * file_io.c */
void   handler                   __((int signal));              /* file_io.c */

/*---------------------------------------------------------------------------
    Macintosh file_io functions:
  ---------------------------------------------------------------------------*/

#ifdef MACOS
/* static int   IsHFSDisk        __((int)); */
   void     macfstest            __((int));
   int      macmkdir             __((char *, short, long));
   void     ResolveMacVol        __((short, short *, long *, StringPtr));
   short    macopen              __((char *, short, short, long));
   short    maccreat             __((char *, short, long, OSType, OSType));
   short    macread              __((short, char *, unsigned));
   short    macwrite             __((short, char *, unsigned));
   short    macclose             __((short));
   long     maclseek             __((short, long, short));
#endif

/*---------------------------------------------------------------------------
    OS/2 file_io functions:
  ---------------------------------------------------------------------------*/
#ifdef OS2
void     ChangeNameForFAT  __((char *name));                   /* os2unzip.c */
int      IsFileNameValid   __((char *name));                   /* os2unzip.c */
int      GetCountryInfo    __((void));                         /* os2unzip.c */
long     GetFileTime       __((char *name));                   /* os2unzip.c */
void     SetPathInfo      __((char *path, ush moddate, ush modtime, int flags));
int      SetLongNameEA     __((char *name, char *longname));   /* os2unzip.c */
int      IsEA              __((void *extra_field));            /* os2unzip.c */
ulg      SizeOfEAs         __((void *extra_field));            /* os2unzip.c */
void     SetEAs            __((char *path, void *eablock));    /* os2unzip.c */
int      IsUpperNLS        __((int nChr));                     /* os2unzip.c */
int      ToLowerNLS        __((int nChr));                     /* os2unzip.c */
#endif
/*---------------------------------------------------------------------------
    VMS file_io functions:
  ---------------------------------------------------------------------------*/
#ifdef VMS
int      check_format      __((void));                              /* vms.c */
int      find_vms_attrs    __((void));                              /* vms.c */
int      CloseOutputFile   __((void));                              /* vms.c */
/* static uch *extract_block  __((struct extra_block *, int *, uch *, int)); */
/* static int  _flush_blocks  __((int final_flag));                  * vms.c */
/* static int  _flush_records __((int final_flag));                  * vms.c */
/* static int  WriteBuffer    __((unsigned char *buf, int len));     * vms.c */
/* static int  WriteRecord    __((unsigned char *rec, int len));     * vms.c */
/* static void message        __((int string, char *status));        * vms.c */
#endif
/*---------------------------------------------------------------------------
    Functions in match.c, misc.c, etc.:
  ---------------------------------------------------------------------------*/

int      match             __((char *string, char *pattern));     /* match.c */

void     UpdateCRC         __((register unsigned char *s, register int len));
__time32_t   dos_to_unix_time  __((unsigned ddate, unsigned dtime));   /* misc.c */
int      check_for_newer   __((char *filename, char *output_filename)); /* misc.c */
int      dateformat        __((void));                             /* misc.c */
void     return_VMS        __((int zip_error));                    /* misc.c */

void     envargs           __((int *, char ***, char *));       /* envargs.c */

int      mapname           __((int create_dirs));               /* mapname.c */
int      mapattr           __((void));                              /* local */
void     set_file_time_and_close  __((void));                       /* local */

#ifdef AMIGA
   LONG  FileDate          __((char *, struct DateStamp *));   /* filedate.c */
#endif

#ifdef TOPS20
   int upper               __((char *s));                        /* tops20.c */
   int enquote             __((char *s));                        /* tops20.c */
   int dequote             __((char *s));                        /* tops20.c */
   int fnlegal             __(());  /* error if prototyped(?) */ /* tops20.c */
#endif





/************/
/*  Macros  */
/************/

#ifndef MAX
#  define MAX(a,b)   ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#  define MIN(a,b)   ((a) < (b) ? (a) : (b))
#endif


#define LSEEK(abs_offset) {longint request=(abs_offset)+extra_bytes,\
   inbuf_offset=request%INBUFSIZ, bufstart=request-inbuf_offset;\
   if(request<0) {fprintf(stderr, SeekMsg, ReportMsg); return(3);}\
   else if(bufstart!=cur_zipfile_bufstart)\
   {cur_zipfile_bufstart=_lseek(zipfd,bufstart,SEEK_SET);\
   if((incnt=_read(zipfd,(char *)inbuf,INBUFSIZ))<=0) return(51);\
   inptr=inbuf+(int)inbuf_offset; incnt-=(int)inbuf_offset;} else\
   {incnt+=(inptr-inbuf)-(int)inbuf_offset; inptr=inbuf+(int)inbuf_offset;}}

/*
 *  Seek to the block boundary of the block which includes abs_offset,
 *  then read block into input buffer and set pointers appropriately.
 *  If block is already in the buffer, just set the pointers.  This macro
 *  is used by process_end_central_dir (unzip.c) and do_string (misc.c).
 *  A slightly modified version is embedded within extract_or_test_files
 *  (unzip.c).  ReadByte and readbuf (file_io.c) are compatible.
 *
 *  macro LSEEK(abs_offset)
 *      ulg abs_offset;
 *  {
 *      longint   request = abs_offset + extra_bytes;
 *      longint   inbuf_offset = request % INBUFSIZ;
 *      longint   bufstart = request - inbuf_offset;
 *
 *      if (request < 0) {
 *          fprintf(stderr, SeekMsg, ReportMsg);
 *          return(3);             /-* 3:  severe error in zipfile *-/
 *      } else if (bufstart != cur_zipfile_bufstart) {
 *          cur_zipfile_bufstart = lseek(zipfd, bufstart, SEEK_SET);
 *          if ((incnt = read(zipfd,inbuf,INBUFSIZ)) <= 0)
 *              return(51);        /-* 51:  unexpected EOF *-/
 *          inptr = inbuf + (int)inbuf_offset;
 *          incnt -= (int)inbuf_offset;
 *      } else {
 *          incnt += (inptr-inbuf) - (int)inbuf_offset;
 *          inptr = inbuf + (int)inbuf_offset;
 *      }
 *  }
 *
 */


#define SKIP_(length) if(length&&((error=do_string(length,SKIP))!=0))\
  {error_in_archive=error; if(error>1) return error;}

/*
 *  Skip a variable-length field, and report any errors.  Used in zipinfo.c
 *  and unzip.c in several functions.
 *
 *  macro SKIP_(length)
 *      ush length;
 *  {
 *      if (length && ((error = do_string(length, SKIP)) != 0)) {
 *          error_in_archive = error;   /-* might be warning *-/
 *          if (error > 1)              /-* fatal *-/
 *              return (error);
 *      }
 *  }
 *
 */


#define OUTB(intc) {*outptr++=(uch)(intc); if (++outcnt==OUTBUFSIZ)\
  FlushOutput();}

/*
 *  macro OUTB(intc)
 *  {
 *      *outptr++ = (uch)(intc);
 *      if (++outcnt == OUTBUFSIZ)
 *          FlushOutput();
 *  }
 *
 */


#define READBIT(nbits,zdest) {if(nbits>bits_left) FillBitBuffer();\
  zdest=(int)((ush)bitbuf&mask_bits[nbits]);bitbuf>>=nbits;bits_left-=nbits;}

/*
 * macro READBIT(nbits,zdest)
 *  {
 *      if (nbits > bits_left)
 *          FillBitBuffer();
 *      zdest = (int)((ush)bitbuf & mask_bits[nbits]);
 *      bitbuf >>= nbits;
 *      bits_left -= nbits;
 *  }
 *
 */


#define PEEKBIT(nbits) (nbits>bits_left? (FillBitBuffer(),\
  (ush)bitbuf & mask_bits[nbits]) : (ush)bitbuf & mask_bits[nbits])


#define NUKE_CRs(buf,len) {register int i,j; for (i=j=0; j<len;\
  (buf)[i++]=(buf)[j++]) if ((buf)[j]=='\r') ++j; len=i;}

/*
 *  Remove all the ASCII carriage returns from buffer buf (length len),
 *  shortening as necessary (note that len gets modified in the process,
 *  so it CANNOT be an expression).  This macro is intended to be used
 *  BEFORE A_TO_N(); hence the check for CR instead of '\r'.  NOTE:  The
 *  if-test gets performed one time too many, but it doesn't matter.
 *
 *  macro NUKE_CRs(buf,len)
 *  {
 *      register int   i, j;
 *
 *      for (i = j = 0;  j < len;  (buf)[i++] = (buf)[j++])
 *          if ((buf)[j] == CR)
 *              ++j;
 *      len = i;
 *  }
 *
 */


#define TOLOWER(str1,str2) {char *ps1,*ps2; ps1=(str1)-1; ps2=(str2);\
  while(*++ps1) *ps2++=(char)(isupper((int)(*ps1))?tolower((int)(*ps1)):*ps1);\
  *ps2='\0';}

/*
 *  Copy the zero-terminated string in str1 into str2, converting any
 *  uppercase letters to lowercase as we go.  str2 gets zero-terminated
 *  as well, of course.  str1 and str2 may be the same character array.
 *
 *  macro TOLOWER( str1, str2 )
 *  {
 *      char  *ps1, *ps2;
 *
 *      ps1 = (str1) - 1;
 *      ps2 = (str2);
 *      while (*++ps1)
 *          *ps2++ = (char)(isupper((int)(*ps1))? tolower((int)(*ps1)) : *ps1);
 *      *ps2='\0';
 *  }
 *
 *  NOTES:  This macro makes no assumptions about the characteristics of
 *    the tolower() function or macro (beyond its existence), nor does it
 *    make assumptions about the structure of the character set (i.e., it
 *    should work on EBCDIC machines, too).  The fact that either or both
 *    of isupper() and tolower() may be macros has been taken into account;
 *    watch out for "side effects" (in the C sense) when modifying this
 *    macro.
 */


#ifndef ascii_to_native
#  define ascii_to_native(c)   (c)
#  define A_TO_N(str1)
#else
#  ifndef NATIVE
#    define NATIVE     "native chars"
#  endif
#  define A_TO_N(str1) {register unsigned char *ps1;\
     for (ps1=str1; *ps1; ps1++) *ps1=ascii_to_native(*ps1);}
#endif

/*
 *  Translate the zero-terminated string in str1 from ASCII to the native
 *  character set. The translation is performed in-place and uses the
 *  ascii_to_native macro to translate each character.
 *
 *  macro A_TO_N( str1 )
 *  {
 *      register unsigned char *ps1;
 *
 *      for (ps1 = str1;  *ps1;  ++ps1)
 *          *ps1 = ascii_to_native(*ps1);
 *  }
 *
 *  NOTE:  Using the ascii_to_native macro means that is it the only part of
 *    unzip which knows which translation table (if any) is actually in use
 *    to produce the native character set.  This makes adding new character
 *    set translation tables easy, insofar as all that is needed is an
 *    appropriate ascii_to_native macro definition and the translation
 *    table itself.  Currently, the only non-ASCII native character set
 *    implemented is EBCDIC, but this may not always be so.
 */





/*************/
/*  Globals  */
/*************/

/* NEW for ziplib.dll */
   extern int       different_output_filename;
#ifdef MSWIN
   extern char      *output_filename;
#else
   extern char      output_filename[];
#endif

/* end NEW for ziplib.dll */

   extern int       aflag;
/* extern int       bflag;   reserved */
   extern int       cflag;
   extern int       fflag;
   extern int       jflag;
   extern int       overwrite_none;
   extern int       overwrite_all;
   extern int       force_flag;
   extern int       quietflg;
#ifdef DOS_OS2
   extern int       sflag;
#endif
   extern int       tflag;
   extern int       uflag;
   extern int       U_flag;
   extern int       vflag;
   extern int       zflag;
   extern int       V_flag;
#ifdef VMS
   extern int       secinf;
#endif
#ifdef MACOS
   extern int       hfsflag;
#endif
   extern int       filespecs;
   extern int       xfilespecs;
   extern int       process_all_files;
   extern longint   real_ecrec_offset;
   extern longint   expect_ecrec_offset;
   extern longint   csize;
   extern longint   ucsize;

   extern char *fnames[];

   extern char      **pfnames;
   extern char      **pxnames;
   extern char      sig[];
   extern char      answerbuf[];
   extern min_info  *pInfo;
   extern min_info  info[];
#ifdef OS2
   extern int       longname;
   extern char      longfilename[];
#endif
   extern char      *key;

   extern union work area;

   extern ulg       crc32val;
   extern ush       mask_bits[];

   extern uch       *inbuf;
   extern uch       *inptr;
   extern int       incnt;
   extern ulg       bitbuf;
   extern int       bits_left;
   extern boolean   zipeof;
#ifdef MSWIN
   extern char      *zipfn;
#else
   extern char      zipfn[];
#endif
   extern int       zipfd;
   extern longint   ziplen;
   extern longint   extra_bytes;
   extern longint   cur_zipfile_bufstart;
   extern uch       *extra_field;
   extern uch       *hold;
   extern char      local_hdr_sig[];
   extern char      central_hdr_sig[];
   extern char      end_central_sig[];
   extern local_file_hdr  lrec;
   extern cdir_file_hdr   crec;
   extern ecdir_rec       ecrec;
   extern struct _stat    statbuf;

   extern uch       *outbuf;
   extern uch       *outptr;
#ifdef MSWIN
   extern uch       *outout;
   extern char      *filename;
#else
   extern uch       *outout;
   extern char      filename[];
#endif
   extern longint   outpos;
   extern int       outcnt;
   extern int       outfd;
   extern int       mem_mode;
   extern int       disk_full;
   extern int       newfile;
#ifdef SYMLINKS
   extern int       symlnk;
#endif
   extern char unkn[];
   extern ush methnum;

   extern char      *EndSigMsg;
   extern char      *CentSigMsg;
   extern char      *SeekMsg;
   extern char      *ReportMsg;

#ifdef DECLARE_ERRNO
   extern int       errno;
#endif

#ifdef EBCDIC
   extern uch       ebcdic[];
#endif

#ifdef MACOS
   extern short     gnVRefNum;
   extern long      glDirID;
   extern OSType    gostCreator;
   extern OSType    gostType;
   extern boolean   fMacZipped;
   extern boolean   macflag;
   extern short     giCursor;
   extern CursHandle rghCursor[];
#endif

#endif /* !__unzip_h */
