/*---------------------------------------------------------------------------

  mapname.c

  This routine changes TOPS-20, VMS, and MSDOS-style filenames into normal
  Unix names (and vice versa, in some cases); it also creates any necessary 
  directories, unless the -j switch was specified.

  ---------------------------------------------------------------------------

  Notes:

   - This routine REALLY needs to be rewritten (different routines for each
     output OS, with different rules for different parts of the path name).
   - The strcpy and strcat operations on both cdp and filename may overwrite
     memory, since they don't check lengths.  With a kilobyte in which to
     work, this is probably not that big a deal, but it could cause problems
     eventually.

  ---------------------------------------------------------------------------*/

#include "ziplib.h"
#include "unzip.h"


/*******************/
/* Mapname Defines */
/*******************/

#ifdef VMS
#  define PERMS   0
#else
#  define PERMS   0777
#endif

#ifndef NO_MKDIR
#  if defined(DOS_OS2) && (!defined(__GO32__))
#    if (_MSC_VER >= 600)          /* have special MSC mkdir prototype */
#      include <direct.h>
#    else                          /* own prototype because dir.h conflicts? */
       int mkdir(const char *path);
#    endif /* ?(MSC 6.0 or later) */
#    define MKDIR(path,mode)   mkdir(path)
#  else /* !DOS_OS2 || __GO32__ */
#    ifdef MACOS
#      define MKDIR(path,mode)     macmkdir(path,gnVRefNum,glDirID)
#    else /* !MACOS */
#      if defined(AMIGA) || defined(WIN32)
#        define MKDIR(path,mode)   _mkdir(path)
#      else /* !AMIGA */
#        define MKDIR(path,mode)   mkdir(path,mode)
#      endif /* ?AMIGA */
#    endif /* ?MACOS */
#  endif /* ?(DOS_OS2 && !__GO32__) */
#endif /* !NO_MKDIR */




/************************/
/*  Function mapname()  */
/************************/

int mapname(create_dirs)   /* return 0 if no error, 1 if caution (filename */
    int create_dirs;       /*  truncated), 2 if warning (skip file because */
{                          /*  dir doesn't exist), 3 if error (skip file) */
#if defined(NO_MKDIR) && (!defined(TOPS20))
    char command[FILNAMSIZ+40]; /* buffer for system() call */
#endif
#ifdef DOS_T20_VMS
    char *last_dot=NULL;        /* last dot not converted to underscore */
#ifdef T20_VMS
    int stat_val;               /* temp. holder for stat() return value */
    char *dp, *xp;              /* pointers to directory name */
#ifdef VMS
    char *np;                   /* pointer into filename */
#endif /* VMS */
#endif /* T20_VMS */
#endif /* DOS_T20_VMS */
#ifdef OS2
    char *last;
    int longdir;
#endif /* OS2 */
    char name[FILNAMSIZ];       /* file name buffer */
    char *pp, *cp, *cdp;        /* character pointers */
    char delim = '\0';          /* directory delimiter */
    int quote = FALSE;          /* flags */
    int indir = FALSE;
    int done = FALSE;
    int created = FALSE;
    register unsigned workch;   /* hold the character being tested */


/*---------------------------------------------------------------------------
    Initialize various pointers and counters and stuff.
  ---------------------------------------------------------------------------*/

#ifdef MAP_DEBUG
    fprintf(stderr, "%s ", output_filename);   /* echo name of this file */
#endif
    cdp = (char *)NULL;
    pp = name;                  /* point to translation buffer */
    *name = '\0';               /* initialize buffer */
    if (!jflag) {               /* -j => junk pathnames */
        cdp = (char *)malloc(strlen(output_filename) + 13); /* place for holding */
        if (cdp == (char *)NULL) {                    /*  directory name */
#ifdef OLD_MSGS
            fprintf(stderr, "mapname:  out of memory [%s]\n", output_filename);
#endif
            return 3;
        }
#ifdef T20_VMS
        *cdp++ = DIR_BEG;       /* '[' or '<' */
        xp = cdp;               /* always points to last non-NULL char */
        *cdp++ = '.';
#endif /* T20_VMS */
#ifdef MACOS
        *cdp = ':';             /* the Mac uses ':' as a directory separator */
        cdp[1] = '\0';
#else /* !MACOS */
        *cdp = '\0';
#endif /* ?MACOS */
    }

/*---------------------------------------------------------------------------
    Begin main loop through characters in filename.
  ---------------------------------------------------------------------------*/

    for (cp = output_filename;
         (workch = (unsigned char) *cp++) != 0  &&  !done; ) {

        if (quote) {                 /* if char quoted, */
            *pp++ = (char) workch;   /*  include it literally */
            quote = FALSE;
        } else if (indir) {          /* if in directory name, */
            if (workch == (unsigned)delim)  /*  look for end delimiter */
                indir = FALSE;
        } else
            switch (workch) {
            case '<':                /* discard DEC-20 directory name */
                indir = TRUE;
                delim = '>';
                break;
            case '[':                /* discard VMS directory name */
                indir = TRUE;
                delim = ']';
                break;
            case '/':                /* discard Unix path name  */
            case '\\':               /*  or MS-DOS path name... */
                                     /*  iff -j flag was given  */
                /*
                 * Special processing case:  if -j flag was not specified on
                 * command line and create_dirs is TRUE, create any necessary
                 * directories included in the pathname.  Creation of dirs is
                 * straightforward on BSD and MS-DOS machines but requires use
                 * of the system() command on SysV systems (or any others which
                 * don't have mkdir()).  The stat() check is necessary with
                 * MSC because it doesn't have an EEXIST errno, and it saves
                 * the overhead of multiple system() calls on SysV machines.
                 */

                if (!jflag) {
                    *pp = '\0';
#ifdef T20_VMS
                    dp = name;
                    while (*++xp = *dp++);  /* copy name to cdp */
                    last_dot = NULL;        /* dir name:  no dots allowed */
                    strcpy(xp, DIR_EXT);    /* add extension for stat check */
                    stat_val = stat(cdp, &statbuf);
                    *xp = '\0';             /* remove extension for all else */
                    if (stat_val) {         /* doesn't exist, so create */
#else /* !T20_VMS */
#ifdef MSDOS
                    if (last_dot != NULL) {  /* one dot in dir name is legal */
                        *last_dot = '.';
                        last_dot = NULL;
                    }
#endif /* MSDOS */
                    strcat(cdp, name);
#ifdef OS2
                    if ((longdir = !IsFileNameValid(cdp)) != 0) {
                        last = strrchr(cdp, '/');
                        strcpy(longfilename, last ? last + 1 : cdp);
#ifdef OLD_MSGS
                        fprintf(stderr, "renaming directory \"%s\"", cdp);
#endif /* OLD_MSGS */
                        ChangeNameForFAT(cdp);
#ifdef OLD_MSGS
                        fprintf(stderr, " to \"%s\"\n", cdp);
#endif /* OLD_MSGS */
                    }
#endif /* OS2 */
                    if (_stat(cdp, &statbuf)) {  /* doesn't exist, so create */
#endif /* ?T20_VMS */
                        if (!create_dirs) /* told not to create (freshening) */
                            return 2;
#ifdef NO_MKDIR
#ifndef TOPS20 /* for now */
                        sprintf(command,
                          "IFS=\" \t\n\" /bin/mkdir %s 2>/dev/null", cdp);
                        if (system(command)) {
#else  /* TOPS20 */
                        if (FALSE) {
#endif /* ?TOPS20 */
#else /* !NO_MKDIR */
                        if (MKDIR(cdp, PERMS) == -1) {
#endif /* ?NO_MKDIR */
                            ZIP_DiagFile(ZIP_ERR_CREATE_DIR, cdp);
#ifdef OLD_MSGS
                            perror(cdp);
                            fprintf(stderr,
                              "mapname:  unable to process [%s]\n", 
                              output_filename);
#endif /* OLD_MSGS */
                            free(cdp);
                            return 3;
                        }
                        created = TRUE;
#ifdef OS2
                        if (longdir)
                            SetLongNameEA(cdp, longfilename);
#endif /* OS2 */
                    } else if (!(statbuf.st_mode & S_IFDIR)) {
#ifdef OLD_MSGS
                        fprintf(stderr,
                          "mapname:  %s exists but is not a directory\n", cdp);
#endif /* OLD_MSGS */
                        free(cdp);
#ifdef OLD_MSGS
                        fprintf(stderr, "mapname:  unable to process [%s]\n",
                          output_filename);
#endif
                        return 3;
                    }
#ifdef T20_VMS
                    *xp = '/';  /* for now... (mkdir()) */
#else /* !T20_VMS */
#ifdef MACOS
                    strcat(cdp, ":");
#else /* !MACOS */
#ifdef ATARI_ST
                    strcat(cdp, "\\");
#else /* !ATARI_ST */
                    strcat(cdp, "/");
#endif /* ?ATARI_ST */
#endif /* ?MACOS */
#endif /* ?T20_VMS */
                }
                pp = name;
                break;

            case ':':
#ifdef UNIX                       /* colon is a valid character in Unix */
                *pp++ = workch;   /* filenames, so keep it; anywhere else, */
#else /* !UNIX */                 /* change it to an underscore (node and */
                *pp++ = '_';      /* drive names not stored in zipfile) */
#endif /* ?UNIX */
                break;

            case '.':             /* TOPS-20 generation number or */
#ifdef DOS_T20_VMS                /*  DOS, VMS or TOPS-20 separator */
                last_dot = pp;    /* point at last dot so far... */
                *pp++ = '_';      /* convert dot to underscore */
#else /* !DOS_T20_VMS */
                *pp++ = workch;
#endif /* ?DOS_T20_VMS */
                break;

            case ';':             /* VMS generation or DEC-20 attrib */
#ifdef MACOS
                if (V_flag || macflag)
#else /* !MACOS */
                if (V_flag)                /* if requested, save VMS ";##" */
#endif /* ?MACOS */                        /*  version info or Macintosh */
                    *pp++ = (char)workch;  /*  (?) info; otherwise discard */
                else                       /*  everything starting with */
                    done = TRUE;           /*  semicolon.  (Worry about */
                break;                     /*  TOPS-20 later.) */

            case '\026':          /* control-V quote for special chars */
                quote = TRUE;     /* set flag for next character */
                break;

            case ' ':
#if defined(T20_VMS) || defined(MTS)
                *pp++ = '_';      /* change spaces to underscore under */
#else /* !(T20_VMS || MTS) */     /*  VMS, TOPS-20 and MTS, and under */
#ifdef DOS_OS2                    /*  DOS and OS/2 if -s not specified */
                if (!sflag)
                    *pp++ = '_';
                else
#endif /* DOS_OS2 */
                *pp++ = (char)workch;   /* Unix, etc.:  leave as spaces */
#endif /* ?(T20_VMS || MTS) */
                break;

            default:
#ifdef MACOS
                if ((macflag && ((unsigned)workch > 0x1F)) || isprint(workch))
#else /* !MACOS */
#if defined(DOS_OS2) || defined(UNIX)
                /* allow European characters in filenames: */
                if (isprint(workch) || (128 <= workch && workch <= 254))
#else /* !(DOS_OS2 || UNIX) */
                if (isprint(workch))    /* other printable, just keep */
#endif /* ?(DOS_OS2 || UNIX) */
#endif /* ?MACOS */
                    *pp++ = (char) workch;
            } /* end switch */

    } /* end for loop */

    *pp = '\0';                   /* done with name:  terminate it */
#ifdef DOS_T20_VMS                /*  and put a dot back in if VMS */
    if (last_dot != NULL)         /*  or MS-DOS or TOPS-20 */
        *last_dot = '.';
#endif /* DOS_T20_VMS */

/*---------------------------------------------------------------------------
    We COULD check for existing names right now, create a "unique" name, etc.
    At present, we do this in extract_or_test_files() (immediately after we
    return from here).  If conversion went bad, the name'll either be nulled
    out (in which case we'll return non-0), or following procedures won't be
    able to create the extracted file and other error messages will result.
  ---------------------------------------------------------------------------*/

    if (output_filename[strlen(output_filename) - 1] == '/') {
        /* A directory was extracted. It had a trailing /, 
         * don't report the error below. */
        if (created) {
#ifdef OLD_MSGS
            printf("   Creating: %s", output_filename);
#endif /* OLD_MSGS */
#ifdef OS2
            SetPathInfo(output_filename, lrec.last_mod_file_date,
                                         lrec.last_mod_file_time, -1);
            if (extra_field)
                SetEAs(output_filename, extra_field);
#endif
#ifdef OLD_MSGS 
            printf("\n");
#endif /* OLD_MSGS */
        }
        return 2; /* but skip file */
    }

    if (*name == '\0') {
#ifdef OLD_MSGS
        fprintf(stderr, "mapname:  conversion of [%s] failed\n",
                output_filename);
#endif /* OLD_MSGS */
        return 3;
    }

#ifdef OS2
    if (!longname && ((longname = !IsFileNameValid(name)) != 0)) {
        /* in case of second call after user renamed the file, skip this */
        last = strrchr(name, '/');      /* THIS time, save for file_io */
        last = last ? last + 1 : name;  /* only last component */
        strcpy(longfilename, last);
# ifdef OLD_MSGS
        fprintf(stderr, "renaming \"%s\"", name);
# endif /* OLD_MSGS */
        ChangeNameForFAT(last);
# ifdef OLD_MSGS
        fprintf(stderr, " to \"%s\"\n", name);
# endif /* OLD_MSGS */
    }
#endif /* OS2 */

#ifdef VMS
    /* convert filename to legal VMS one, substituting underscores for
     * all invalid characters */
    for (np = name;  *np;  ++np)
        if (!(isdigit(*np) || isalpha(*np) || (*np == '$') ||
            (*np == '-') || (*np == '_') || (*np == '.') || (*np == ';')))
            *np = '_';
#endif /* VMS */

    if (!jflag) {
#ifdef T20_VMS
        *xp++ = DIR_END;             /* proper end-of-dir-name delimiter */
        if (xp == cdp) {             /* no path-name stuff, so... */
            strcpy(output_filename, name);  /* copy file name into global and */
            cdp -= 2;                /*   prepare to free malloc'd space */
        } else {                     /* we've added path-name stuff... */
            *xp = '\0';              /*   so terminate and convert to */
            dp = cdp;                /*   VMS subdir separators (skip */
            while (*++dp)            /*   first char:  better not be */
                if (*dp == '/')      /*   "/"):  change all slashes */
                    *dp = '.';       /*   to dots */
            cdp -= 2;                /*   include leading bracket and dot */
            strcpy(output_filename, cdp);   /* copy VMS-style path name into global */
            strcat(output_filename, name);  /* concatenate file name to global */
        }
#else /* !T20_VMS */
        strcpy(output_filename, cdp);  /* either "" or slash-terminated path */
        strcat(output_filename, name); /* append file name to path name */
#endif /* ?T20_VMS */
        free(cdp);
    } else
        strcpy(output_filename, name); /* copy converted name into global */

#if PATH_MAX < (FILNAMSIZ - 1)
    /* check the length of the file name and truncate if necessary */
    if (PATH_MAX < strlen(output_filename)) {
        output_filename[PATH_MAX] = '\0';
        ZIP_Diag( ZIP_ERR_TRUNC_FILENAME );
#ifdef OLD_MSGS
        fprintf(stderr, "caution:  truncating filename\n");
        fprintf(stderr, "[ %s ]\n", output_filename);
#endif
        return 1;             /* 1:  warning error */
    }
#endif

    return 0;
}
