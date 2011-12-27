/*---------------------------------------------------------------------------

  extract.c

  This file contains the high-level routines ("driver routines") for extrac-
  ting and testing zipfile members.  It calls the low-level routines in files
  explode.c, inflate.c, unreduce.c and unshrink.c.

  ---------------------------------------------------------------------------*/

#include "ziplib.h"
#include "unzip.h"
#include "crypt.h"
#ifdef  MSWIN
#  include "wizunzip.h"
#  include "replace.h"
#endif


/************************************/
/*  Extract Local Prototypes, etc.  */
/************************************/

static int store_info __((void));
static int extract_or_test_member __((void));

static uch *mem_i_buffer;
static uch *mem_o_buffer;
static ulg mem_i_size, mem_i_offset;
static ulg mem_o_size, mem_o_offset;

static char *VersionMsg =
  " skipping: %-22s  need %s compat. v%u.%u (can do v%u.%u)\n";
static char *ComprMsg =
  " skipping: %-22s  compression method %d\n";
static char *FilNamMsg =
  "%s:  bad filename length (%s)\n";
static char *ExtFieldMsg =
  "%s:  bad extra field length (%s)\n";
static char *OffsetMsg =
  "file #%d:  bad zipfile offset (%s)\n";





/**************************************/
/*  Function extract_or_test_files()  */
/**************************************/

int extract_or_test_files()    /* return PK-type error code */
{
    uch *cd_inptr;
    int cd_incnt, error, error_in_archive=PK_COOL;
    int renamed, query, filnum=(-1), blknum=0;
    int *fn_matched=NULL, *xn_matched=NULL;
    ush i, j, members_remaining, num_skipped=0, num_bad_pwd=0;
    longint cd_bufstart, bufstart, inbuf_offset, request;
    min_info info[DIR_BLKSIZ];

#ifdef OLD_MSGS
    int len;
#endif    

/*---------------------------------------------------------------------------
    The basic idea of this function is as follows.  Since the central di-
    rectory lies at the end of the zipfile and the member files lie at the
    beginning or middle or wherever, it is not very desirable to simply
    read a central directory entry, jump to the member and extract it, and
    then jump back to the central directory.  In the case of a large zipfile
    this would lead to a whole lot of disk-grinding, especially if each mem-
    ber file is small.  Instead, we read from the central directory the per-
    tinent information for a block of files, then go extract/test the whole
    block.  Thus this routine contains two small(er) loops within a very
    large outer loop:  the first of the small ones reads a block of files
    from the central directory; the second extracts or tests each file; and
    the outer one loops over blocks.  There's some file-pointer positioning
    stuff in between, but that's about it.  Btw, it's because of this jump-
    ing around that we can afford to be lenient if an error occurs in one of
    the member files:  we should still be able to go find the other members,
    since we know the offset of each from the beginning of the zipfile.

    Begin main loop over blocks of member files.  We know the entire central
    directory is on this disk:  we would not have any of this information un-
    less the end-of-central-directory record was on this disk, and we would
    not have gotten to this routine unless this is also the disk on which
    the central directory starts.  In practice, this had better be the ONLY
    disk in the archive, but maybe someday we'll add multi-disk support.
  ---------------------------------------------------------------------------*/

    pInfo = info;
    members_remaining = ecrec.total_entries_central_dir;

    /* malloc space for check on unmatched filespecs (OK if one or both NULL) */
    if (filespecs > 0  &&
        (fn_matched=(int *)malloc(filespecs*sizeof(int))) != NULL)
        for (i = 0;  i < filespecs;  ++i)
            fn_matched[i] = FALSE;
    if (xfilespecs > 0  &&
        (xn_matched=(int *)malloc(xfilespecs*sizeof(int))) != NULL)
        for (i = 0;  i < xfilespecs;  ++i)
            xn_matched[i] = FALSE;

    while (members_remaining) {
        j = 0;

        /*
         * Loop through files in central directory, storing offsets, file
         * attributes, and case-conversion flags until block size is reached.
         */

        while (members_remaining && (j < DIR_BLKSIZ)) {
            --members_remaining;
            pInfo = &info[j];

            if (readbuf(sig, 4) <= 0) {
                error_in_archive = PK_EOF;
                members_remaining = 0;  /* ...so no more left to do */
                break;
            }
            if (strncmp(sig, central_hdr_sig, 4)) {  /* just to make sure */
                ZIP_Diag(ZIP_ERR_ARCHIVE);
# ifdef OLD_MSGS
                fprintf(stderr, CentSigMsg, j);  /* sig not found */
                fprintf(stderr, ReportMsg);   /* check binary transfers */
# endif /* OLD_MSGS */
                error_in_archive = PK_BADERR;
                members_remaining = 0;  /* ...so no more left to do */
                break;
            }
            /* process_cdir_file_hdr() sets pInfo->hostnum, pInfo->lcflag */
            if ((error = process_cdir_file_hdr()) != PK_COOL) {
                error_in_archive = error;   /* only PK_EOF defined */
                members_remaining = 0;  /* ...so no more left to do */
                break;
            }
            if ((error = do_string(crec.filename_length,FILENAME)) != PK_COOL) {
                if (error > error_in_archive)
                    error_in_archive = error;
                if (error > PK_WARN) {  /* fatal:  no more left to do */
                    ZIP_DiagFile(ZIP_ERR_FILE_NAME_SIZE, filename);
#ifdef OLD_MSGS
                    fprintf(stderr, FilNamMsg, filename, "central");
#endif /* OLD_MSGS */
                    members_remaining = 0;
                    break;
                }
            }
            if ((error = do_string(crec.extra_field_length, EXTRA_FIELD)) != 0)
            {
                if (error > error_in_archive)
                    error_in_archive = error;
                if (error > PK_WARN) {  /* fatal */
                    ZIP_DiagFile( ZIP_ERR_ARCHIVE, filename );
#ifdef OLD_MSGS
                    fprintf(stderr, ExtFieldMsg, filename, "central");
#endif /* OLD_MSGS */
                    members_remaining = 0;
                    break;
                }
            }
            if ((error = do_string(crec.file_comment_length,SKIP)) != PK_COOL) {
                if (error > error_in_archive)
                    error_in_archive = error;
                if (error > PK_WARN) {  /* fatal */
                    ZIP_DiagFile( ZIP_ERR_COMM_SIZE, filename );
#ifdef OLD_MSGS
                    fprintf(stderr, "\n%s:  bad file comment length\n",
                            filename);
#endif /* OLD_MSGS */
                    members_remaining = 0;
                    break;
                }
            }
            if (process_all_files) {
                if (store_info())
                    ++j;  /* file is OK; info[] stored; continue with next */
                else
                    ++num_skipped;
            } else {
                int   do_this_file = FALSE;
                char  **pfn = pfnames-1;

                while (*++pfn)
                    if (match(filename, *pfn)) {
                        do_this_file = TRUE;
                        if (fn_matched)
                            fn_matched[pfn-pfnames] = TRUE;
                        break;       /* found match, so stop looping */
                    }
                if (do_this_file) {  /* check if this is an excluded file */
                    char  **pxn = pxnames-1;

                    while (*++pxn)
                        if (match(filename, *pxn)) {
                            do_this_file = FALSE;
                            if (xn_matched)
                                xn_matched[pxn-pxnames] = TRUE;
                            break;
                        }
                }
                if (do_this_file)
                    if (store_info())
                        ++j;            /* file is OK */
                    else
                        ++num_skipped;  /* unsupp. compression or encryption */
            } /* end if (process_all_files) */


        } /* end while-loop (adding files to current block) */

        /* save position in central directory so can come back later */
        cd_bufstart = cur_zipfile_bufstart;
        cd_inptr = inptr;
        cd_incnt = incnt;

    /*-----------------------------------------------------------------------
        Second loop:  process files in current block, extracting or testing
        each one.
      -----------------------------------------------------------------------*/

        for (i = 0; i < j; ++i) {
            filnum = i + blknum*DIR_BLKSIZ;
            pInfo = &info[i];
            /*
             * if the target position is not within the current input buffer
             * (either haven't yet read far enough, or (maybe) skipping back-
             * ward) skip to the target position and reset readbuf().
             */
            /* LSEEK(pInfo->offset):  */
            request = pInfo->offset + extra_bytes;
            inbuf_offset = request % INBUFSIZ;
            bufstart = request - inbuf_offset;

            if (request < 0) {
#ifdef OLD_MSGS
                fprintf(stderr, SeekMsg, ReportMsg);
#endif
                error_in_archive = PK_BADERR;
                continue;   /* but can still go on */
            } else if (bufstart != cur_zipfile_bufstart) {
                cur_zipfile_bufstart = _lseek(zipfd, bufstart, SEEK_SET);
                if ((incnt = _read(zipfd,(char *)inbuf,INBUFSIZ)) <= 0) {
                    ZIP_Diag( ZIP_ERR_BAD_OFFSET );
#ifdef OLD_MSGS
                    fprintf(stderr, OffsetMsg, filnum, "lseek");
#endif /* OLD_MSGS */
                    error_in_archive = PK_BADERR;
                    continue;   /* can still do next file */
                }
                inptr = inbuf + (int)inbuf_offset;
                incnt -= (int)inbuf_offset;
            } else {
                incnt += (inptr-inbuf) - (int)inbuf_offset;
                inptr = inbuf + (int)inbuf_offset;
            }

            /* should be in proper position now, so check for sig */
            if (readbuf(sig, 4) <= 0) {  /* bad offset */
                ZIP_Diag( ZIP_ERR_BAD_OFFSET );
#ifdef OLD_MSGS
                fprintf(stderr, OffsetMsg, filnum, "EOF");
#endif /* OLD_MSGS */
                error_in_archive = PK_BADERR;
                continue;   /* but can still try next one */
            }
            if (strncmp(sig, local_hdr_sig, 4)) {
                ZIP_Diag( ZIP_ERR_LOCAL_HDR );
#ifdef OLD_MSGS
                fprintf(stderr, OffsetMsg, filnum,
                        "can't find local header sig");   /* bad offset */
#endif /* OLD_MSGS */
                error_in_archive = PK_BADERR;
                continue;
            }
            if ((error = process_local_file_hdr()) != PK_COOL) {
                ZIP_Diag( ZIP_ERR_LOCAL_HDR );
#ifdef OLD_MSGS
                fprintf(stderr, "\nfile #%d:  bad local header\n", filnum);
#endif /* OLD_MSGS */
                error_in_archive = error;   /* only PK_EOF defined */
                continue;   /* can still try next one */
            }
            if ((error = do_string(lrec.filename_length,FILENAME)) != PK_COOL) {
                if (error > error_in_archive)
                    error_in_archive = error;
                if (error > PK_WARN) {
                    ZIP_DiagFile( ZIP_ERR_FILE_NAME_SIZE, filename );
#ifdef OLD_MSGS
                    fprintf(stderr, FilNamMsg, filename, "local");
#endif /* OLD_MSGS */
                    continue;   /* go on to next one */
                }
            }
            if (extra_field != (uch *)NULL) {
                free(extra_field);
                extra_field = (uch *)NULL;
            }
            if ((error = do_string(lrec.extra_field_length,EXTRA_FIELD)) != 0) {
                if (error > error_in_archive)
                    error_in_archive = error;
                if (error > PK_WARN) {
                    ZIP_DiagFile( ZIP_ERR_FILE, filename );
#ifdef OLD_MSGS
                    fprintf(stderr, ExtFieldMsg, filename, "local");
#endif /* OLD_MSGS */
                    continue;   /* go on */
                }
            }

            /*
             * just about to extract file:  if extracting to disk, check if
             * already exists, and if so, take appropriate action according to
             * fflag/uflag/overwrite_all/etc. (we couldn't do this in upper
             * loop because we don't store the possibly renamed filename[] in
             * info[])
             */
            if (!tflag && !cflag) {
                renamed = FALSE;   /* user hasn't renamed output file yet */
#ifdef OS2
                longname = FALSE;  /* no long name has yet been stored */
#endif

#if defined(OLD_MSGS) || defined(MSWIN)
startover:
#endif
                query = FALSE;
#ifdef MACOS
                macflag = (pInfo->hostnum == MAC_);
#endif
                /* NEW for zip library */
                /* if different_output_file_name is non-zero, then */
                /* output_filename is already set */
                if( different_output_filename==0 )
                {
                    strcpy(output_filename, filename);
                }

                /* if we are not using different output filename, use mapname */
                if( !different_output_filename ) {
                    /* mapname can create dirs if not freshening or if renamed */
                    if ((error = mapname(!fflag || renamed)) > PK_WARN) {
                        if ((error > PK_ERR) && (error_in_archive < PK_ERR))
                            error_in_archive = PK_ERR;
                        continue;   /* go on to next file */
                    }
                }

                /* End NEW */

                switch (check_for_newer(filename,output_filename)) {
                    case DOES_NOT_EXIST:
                        if (fflag && !renamed)  /* don't skip if just renamed */
                            continue;   /* freshen (no new files):  skip */
                        break;
                    case EXISTS_AND_OLDER:
                        if (overwrite_none)
                        {
                            if(error_in_archive < PK_EXISTS) {
                                error_in_archive = PK_EXISTS;
                            }
                            continue;   /* never overwrite:  skip file */
                        }
                        if (!overwrite_all && !force_flag)
                            query = TRUE;
                        break;
                    case EXISTS_AND_NEWER:             /* (or equal) */
                        if (overwrite_none) {
                            if(error_in_archive < PK_EXISTS) {
                                error_in_archive = PK_EXISTS;
                            }
                            continue;
                        }
                        
                        if (uflag && !renamed)
                            continue;  /* skip if update/freshen & orig name */
                        if (!overwrite_all && !force_flag)
                            query = TRUE;
                        break;
                }
                if (query) {
#ifdef MSWIN
                    FARPROC lpfnprocReplace;
                    int ReplaceDlgRetVal;   /* replace dialog return value */

                    ShowCursor(FALSE);      /* turn off cursor */
                    SetCursor(hSaveCursor); /* restore the cursor */
                    lpfnprocReplace = MakeProcInstance(ReplaceProc, hInst);
                    ReplaceDlgRetVal = DialogBoxParam(hInst, "Replace",
                      hWndMain, lpfnprocReplace, (DWORD)(LPSTR)output_filename);
                    FreeProcInstance(lpfnprocReplace);
                    hSaveCursor = SetCursor(hHourGlass);
                    ShowCursor(TRUE);
                    switch (ReplaceDlgRetVal) {
                        case IDM_REPLACE_RENAME:
                            renamed = TRUE;
                            goto startover;
                        case IDM_REPLACE_YES:
                            break;
                        case IDM_REPLACE_ALL:
                            overwrite_all = TRUE;
                            overwrite_none = FALSE;  /* just to make sure */
                            break;
                        case IDM_REPLACE_NONE:
                            overwrite_none = TRUE;
                            overwrite_all = FALSE;  /* make sure */
                            force_flag = FALSE;     /* ditto */
                            /* FALL THROUGH, skip */
                        case IDM_REPLACE_NO:
                            continue;
                    }
#else /* !MSWIN */

/* NOTE: This propting has been removed for the zip library.  This is ok,
 *       Because the extract routine always specifies not to overwrite any
 *       files.
 */
#ifdef OLD_MSGS

reprompt:
                    fprintf(stderr,
                      "replace %s? [y]es, [n]o, [A]ll, [N]one, [r]ename: ",
                      output_filename);
                    FFLUSH(stderr);
                    fgets(answerbuf, 9, stdin);
                    switch (*answerbuf) {
                        case 'A':   /* dangerous option:  force caps */
                            overwrite_all = TRUE;
                            overwrite_none = FALSE;  /* just to make sure */
                            break;
                        case 'r':
                        case 'R':
                            do {
                                fprintf(stderr, "new name: ");
                                FFLUSH(stderr);
                                fgets(filename, FILNAMSIZ, stdin);
                                /* usually get \n here:  better check for it */
                                len = strlen(filename);
                                if (filename[len-1] == '\n')
                                    filename[--len] = 0;
                            } while (len == 0);
                            renamed = TRUE;
                            goto startover;   /* sorry for a goto */
                        case 'y':
                        case 'Y':
                            break;
                        case 'N':
                            overwrite_none = TRUE;
                            overwrite_all = FALSE;  /* make sure */
                            force_flag = FALSE;     /* ditto */
                            /* FALL THROUGH, skip */
                        case 'n':
                            continue;   /* skip file */
                        default:
                            fprintf(stderr, "error:  invalid response [%c]\n",
                              *answerbuf);   /* warn the user */
                            goto reprompt;   /* why not another goto? */
                    } /* end switch (*answerbuf) */
#else /* OLD_MSGS */
                    continue; /* Skip file if it has to be replaced */
#endif /* OLD_MSGS */

#endif /* ?MSWIN */
                } /* end if (query) */
            } /* end if (extracting to disk) */

#ifdef CRYPT
            if (pInfo->encrypted && (error = decrypt_member()) != PK_COOL) {
                if (error == PK_MEM2) {
                    if (error > error_in_archive)
                        error_in_archive = error;
                    ZIP_DiagFile( ZIP_ERR_CRYPT, filename );
#ifdef OLD_MSGS
                    fprintf(stderr,
                      " skipping: %-22s  unable to get password\n", filename);
#endif /* OLD_MSGS */
                } else {  /* (error == PK_WARN) */
                    ZIP_DiagFile( ZIP_ERR_CRYPT, filename );
#ifdef OLD_MSGS
                    fprintf(stderr,
                      " skipping: %-22s  incorrect password\n", filename);
#endif /* OLD_MSGS */
                    ++num_bad_pwd;
                }
                continue;   /* go on to next file */
            }
#endif /* CRYPT */
            disk_full = 0;
            if ((error = extract_or_test_member()) != PK_COOL) {
                if (error > error_in_archive)
                    error_in_archive = error;       /* ...and keep going */
                if (disk_full > 1)
                    return error_in_archive;        /* (unless disk full) */
            }
        } /* end for-loop (i:  files in current block) */


        /*
         * Jump back to where we were in the central directory, then go and do
         * the next batch of files.
         */

        cur_zipfile_bufstart = _lseek(zipfd, cd_bufstart, SEEK_SET);
        _read(zipfd, (char *)inbuf, INBUFSIZ);  /* were there b4 ==> no error */
        inptr = cd_inptr;
        incnt = cd_incnt;
        ++blknum;

#ifdef TEST
        printf("\ncd_bufstart = %ld (%.8lXh)\n", cd_bufstart, cd_bufstart);
        printf("cur_zipfile_bufstart = %ld (%.8lXh)\n", cur_zipfile_bufstart,
          cur_zipfile_bufstart);
        printf("inptr-inbuf = %d\n", inptr-inbuf);
        printf("incnt = %d\n\n", incnt);
#endif

    } /* end while-loop (blocks of files in central directory) */

/*---------------------------------------------------------------------------
    Check for unmatched filespecs on command line and print warning if any
    found.
  ---------------------------------------------------------------------------*/

    if (fn_matched) {
        for (i = 0;  i < filespecs;  ++i)
            if (!fn_matched[i])
            {
               ZIP_DiagFile( ZIP_ERR_FILE_NOT_FOUND, pfnames[i] );
#ifdef OLD_MSGS
               fprintf(stderr, "caution: filename not matched:  %s\n",
                  pfnames[i]);
#endif /* OLD_MSGS */
               if(error_in_archive < PK_FIND)
               {
                  error_in_archive = PK_FIND;
               }               
            }  
        free(fn_matched);
    }
    if (xn_matched) {
        for (i = 0;  i < xfilespecs;  ++i)
            if (!xn_matched[i])
#ifdef OLD_MSGS
                fprintf(stderr, "caution: excluded filename not matched:  %s\n",
                  pxnames[i]);
#endif /* OLD_MSGS */
        free(xn_matched);
    }

/*---------------------------------------------------------------------------
    Double-check that we're back at the end-of-central-directory record, and
    print quick summary of results, if we were just testing the archive.  We
    send the summary to stdout so that people doing the testing in the back-
    ground and redirecting to a file can just do a "tail" on the output file.
  ---------------------------------------------------------------------------*/

    readbuf(sig, 4);
    if (strncmp(sig, end_central_sig, 4)) {     /* just to make sure again */
        ZIP_Diag( ZIP_ERR_ARCHIVE );
#ifdef OLD_MSGS
        fprintf(stderr, EndSigMsg);  /* didn't find end-of-central-dir sig */
        fprintf(stderr, ReportMsg);  /* check binary transfers */
#endif
        if (!error_in_archive)       /* don't overwrite stronger error */
            error_in_archive = PK_WARN;
    }
    if (tflag) {
        int num=filnum+1 - num_bad_pwd;

#ifdef OLD_MSGS
        if (quietflg == 1) {
            if (error_in_archive)
                printf("At least one error was detected in %s.\n", zipfn);
            else if (num == 0)
                printf("Caution:  zero files tested in %s.\n", zipfn);
            else if (process_all_files && (num_skipped+num_bad_pwd == 0))
                printf("No errors detected in compressed data of %s.\n", zipfn);
            else
                printf("No errors detected in %s for the %d file%s tested.\n",
                  zipfn, num, (num==1)? "":"s");
            if (num_skipped > 0)
                printf("%d file%s skipped because of unsupported compression \
or encoding.\n", num_skipped, (num_skipped==1)? "":"s");
# ifdef CRYPT
            if (num_bad_pwd > 0)
                printf("%d file%s skipped because of incorrect password.\n",
                  num_bad_pwd, (num_bad_pwd==1)? "":"s");
# endif /* CRYPT */
        } else if ((quietflg == 0) && !error_in_archive && (num == 0))
            printf("Caution:  zero files tested in %s.\n", zipfn);
#endif /* OLD_MSGS */
    }

    /* give warning if files not tested or extracted */
    if ((num_skipped > 0) && !error_in_archive)
        error_in_archive = PK_WARN;
#ifdef CRYPT
    if ((num_bad_pwd > 0) && !error_in_archive)
        error_in_archive = PK_WARN;
#endif /* CRYPT */

    return error_in_archive;

} /* end function extract_or_test_files() */





/***************************/
/*  Function store_info()  */
/***************************/

static int store_info()   /* return 0 if skipping, 1 if OK */
{
#define UNKN_COMPR \
   (crec.compression_method>IMPLODED && crec.compression_method!=DEFLATED)

/*---------------------------------------------------------------------------
    Check central directory info for version/compatibility requirements.
  ---------------------------------------------------------------------------*/

    pInfo->encrypted = crec.general_purpose_bit_flag & 1;    /* bit field */
    pInfo->ExtLocHdr = (crec.general_purpose_bit_flag & 8) == 8;  /* bit */
    pInfo->text = crec.internal_file_attributes & 1;         /* bit field */
    pInfo->crc = crec.crc32;
    pInfo->compr_size = crec.csize;

    if (crec.version_needed_to_extract[1] == VMS_) {
        if (crec.version_needed_to_extract[0] > VMS_VERSION) {
        ZIP_Diag( ZIP_ERR_BAD_VERSION );
#ifdef OLD_MSGS
            fprintf(stderr, VersionMsg, filename, "VMS",
              crec.version_needed_to_extract[0] / 10,
              crec.version_needed_to_extract[0] % 10,
              VMS_VERSION / 10, VMS_VERSION % 10);
#endif /* OLD_MSGS */
            return 0;
        }
#ifndef VMS   /* won't be able to use extra field, but still have data */
        else if (!tflag && !force_flag) {  /* if forcing, extract regardless */
            ZIP_DiagFile( ZIP_ERR_VMS, filename );
#  ifdef OLD_MSGS
            fprintf(stderr,
              "\n%s:  stored in VMS format.  Extract anyway? (y/n) ",
              filename);
            FFLUSH(stderr);
            fgets(answerbuf, 9, stdin);
            if ((*answerbuf != 'y') && (*answerbuf != 'Y'))
                return 0;
#  endif /* OLD_MSGS */
        }
#endif /* !VMS */
    /* usual file type:  don't need VMS to extract */
    } else if (crec.version_needed_to_extract[0] > UNZIP_VERSION) {
        ZIP_DiagFile( ZIP_ERR_BAD_VERSION, filename );
#ifdef OLD_MSGS
        fprintf(stderr, VersionMsg, filename, "PK",
          crec.version_needed_to_extract[0] / 10,
          crec.version_needed_to_extract[0] % 10,
          UNZIP_VERSION / 10, UNZIP_VERSION % 10);
#endif /* OLD_MSGS */
        return 0;
    }

    if UNKN_COMPR {
        ZIP_DiagFile( ZIP_ERR_COMPR, filename );
#ifdef OLD_MSGS
        fprintf(stderr, ComprMsg, filename, crec.compression_method);
#endif /* OLD_MSGS */
        return 0;
    }
#ifndef CRYPT
    if (pInfo->encrypted) {
        ZIP_DiagFile( ZIP_ERR_CRYPT, filename );
#ifdef OLD_MSGS
        fprintf(stderr, " skipping: %-22s  encrypted (not supported)\n",
          filename);
#endif /* OLD_MSGS */
        return 0;
    }
#endif /* !CRYPT */

    /* map whatever file attributes we have into the local format */
/*    mapattr();   IGC *//* GRR:  worry about return value later */

    pInfo->offset = (longint) crec.relative_offset_local_header;
    return 1;

} /* end function store_info() */





#ifdef OLD_QQ
#  define QCOND2   (quietflg < 2)
#else
#  define QCOND2   (!quietflg)
#endif

/***************************************/
/*  Function extract_or_test_member()  */
/***************************************/

static int extract_or_test_member()    /* return PK-type error code */
{
    int error = PK_COOL;
    ush b;
    int r;



/*---------------------------------------------------------------------------
    Initialize variables, buffers, etc.
  ---------------------------------------------------------------------------*/

    bits_left = 0;
    bitbuf = 0L;
    outpos = 0L;
    outcnt = 0;
    outptr = outbuf;
    zipeof = 0;
    crc32val = 0xFFFFFFFFL;
    newfile = TRUE;

#ifdef SYMLINKS
    /* if file came from Unix and is a symbolic link and we are extracting
     * to disk, prepare to restore the link */
    if (S_ISLNK(pInfo->file_attr) && (pInfo->hostnum == UNIX_) && !tflag &&
        !cflag && (lrec.ucsize > 0))
        symlnk = TRUE;
    else
        symlnk = FALSE;
#endif /* SYMLINKS */

    memset(outbuf, 0xAA, OUTBUFSIZ);
#if (!defined(DOS_OS2)) || defined(MSWIN)
    if (aflag)                  /* if we have a scratchpad, clear it out */
#ifdef MSWIN
        _fmemset(outout, 0xAA, OUTBUFSIZ);
#else /* !MSWIN */
        memset(outout, 0xAA, OUTBUFSIZ);
#endif /* ?MSWIN */
#endif /* !DOS_OS2 || MSWIN */

    if (tflag) {
#ifdef OLD_MSGS
        if (!quietflg) {
            fprintf(stdout, "  Testing: %-22s ", filename);
            fflush(stdout);
        }
#endif /* OLD_MSGS */
    } else {
        if (cflag) {            /* output to stdout (or copy of it) */
            outfd = _dup(1);
#ifdef DOS_OS2
            if (!aflag)
                setmode(outfd, O_BINARY);
#endif /* DOS_OS2 */
#ifdef VMS
            if (create_output_file())   /* VMS version required for stdout! */
                return PK_DISK;
#endif
        } else if (create_output_file())
            return PK_DISK;

    } /* endif (!tflag) */

/*---------------------------------------------------------------------------
    Unpack the file.
  ---------------------------------------------------------------------------*/

    switch (lrec.compression_method) {

    case STORED:
#ifdef OLD_MSGS
        if (!tflag && QCOND2) {
            fprintf(stdout, " Extracting: %-22s ", filename);
            if (cflag)
                fprintf(stdout, "\n");
            fflush(stdout);
        }
#endif /* OLD_MSGS */

        while (ReadByte(&b) && !disk_full)
            OUTB(b)
        break;

    case SHRUNK:
#ifdef OLD_MSGS
        if (!tflag && QCOND2) {
            fprintf(stdout, "UnShrinking: %-22s ", filename);
            if (cflag)
                fprintf(stdout, "\n");
            fflush(stdout);
        }
#endif /* OLD_MSGS */
        unShrink();
        break;

    case REDUCED1:
    case REDUCED2:
    case REDUCED3:
    case REDUCED4:
#ifdef OLD_MSGS
        if (!tflag && QCOND2) {
            fprintf(stdout, " unReducing: %-22s ", filename);
            if (cflag)
                fprintf(stdout, "\n");
            fflush(stdout);
        }
#endif /* OLD_MSGS */
        unReduce();
        break;

    case IMPLODED:
#ifdef OLD_MSGS
        if (!tflag && QCOND2) {
            fprintf(stdout, "  Exploding: %-22s ", filename);
            if (cflag)
                fprintf(stdout, "\n");
            fflush(stdout);
        }
#endif /* OLD_MSGS */
        if (((r = explode()) != 0) && (r != 5)) {   /* ignore 5 if seekable */
            if ((tflag && quietflg) || (!tflag && !QCOND2)) {
#ifdef OLD_MSGS
                fprintf(stderr, "  error:  %s%s\n", r == 3?
                  "not enough memory to explode " :
                  "invalid compressed (imploded) data for ", filename);
#endif /* OLD_MSGS */
            }
            else {
#ifdef OLD_MSGS
                fprintf(stderr, "\n  error:  %s\n", r == 3?
                  "not enough memory for explode operation" :
                  "invalid compressed data for explode format");
#endif /* OLD_MSGS */
            }
            error = (r == 3)? PK_MEM2 : PK_ERR;
        }
        break;

    case DEFLATED:
        if (!tflag && QCOND2) {
#ifdef OLD_MSGS
            fprintf(stdout, "  Inflating: %-22s ", filename);
            if (cflag)
                fprintf(stdout, "\n");
            fflush(stdout);
#endif /* OLD_MSGS */
        }
        if ((r = ccinflate()) != 0) {
            if ((tflag && quietflg) || (!tflag && !QCOND2)) {
#ifdef OLD_MSGS
                fprintf(stderr, "  error:  %s%s\n", r == 3?
                  "not enough memory to inflate " :
                  "invalid compressed (deflated) data for ", filename);
#endif /* OLD_MSGS */
            }
            else {
#ifdef OLD_MSGS
                fprintf(stderr, "\n  error:  %s\n", r == 3?
                  "not enough memory for inflate operation" :
                  "invalid compressed data for inflate format");
#endif /* OLD_MSGS */
            }
            error = (r == 3)? PK_MEM2 : PK_ERR;
        }
        break;

    default:   /* should never get to this point */
        ZIP_DiagFile( ZIP_ERR_COMPR, filename );
#ifdef OLD_MSGS
        fprintf(stderr, "%s:  unknown compression method\n", filename);
#endif /* OLD_MSGS */
        /* close and delete file before return? */
        return PK_WARN;

    } /* end switch (compression method) */

    if (disk_full) {            /* set by FlushOutput()/OUTB() macro */
        if (disk_full > 1)
            return PK_DISK;
        error = PK_WARN;
    }

/*---------------------------------------------------------------------------
    Write the last partial buffer, if any; set the file date and time; and
    close the file (not necessarily in that order).  Then make sure CRC came
    out OK and print result.
  ---------------------------------------------------------------------------*/

    if (!disk_full && FlushOutput())
        if (disk_full > 1)
            return PK_DISK;
        else {                  /* disk_full == 1 */
            ZIP_DiagFile( ZIP_ERR_FILE_CORRUPT, output_filename);
#ifdef OLD_MSGS
            fprintf(stderr, "%s:  probably corrupt\n", output_filename);
#endif /* OLD_MSGS */
            error = PK_WARN;
        }

    if (!tflag)
#ifdef VMS
        CloseOutputFile();
#else /* !VMS */
#ifdef MTS                      /* MTS can't set file time */
        close(outfd);
#else /* !MTS */
        /* set_file_time_and_close(); IGC*/
        _close(outfd);
#endif /* ?MTS */
#endif /* ?VMS */

    if (error > PK_WARN)  /* don't print redundant CRC error if error already */
        return error;

    /* logical-AND crc32val for 64-bit machines */
    if ((crc32val = ((~crc32val) & 0xFFFFFFFFL)) != lrec.crc32) {
        /* if quiet enough, we haven't output the filename yet:  do it */
#ifdef OLD_MSGS
        if ((tflag && quietflg) || (!tflag && !QCOND2))
            fprintf(stderr, "%-22s ", filename);
        fprintf(stderr, " bad CRC %08lx  (should be %08lx)\n", crc32val,
                lrec.crc32);
        FFLUSH(stderr);
#endif /* OLD_MSGS */
        ZIP_DiagFile( ZIP_ERR_BAD_CRC, filename );
        error = PK_WARN;
    } else if (tflag) {
#ifdef OLD_MSGS
        if (!quietflg)
            fprintf(stdout, " OK\n");
#endif /* OLD_MSGS */
    } else {
#ifdef OLD_MSGS
        if (QCOND2 && !error)
            fprintf(stdout, "\n");
#endif /* OLD_MSGS */
    }

    return error;

} /* end function extract_or_test_member() */





/*******************************/
/*  Function ReadMemoryByte()  */
/*******************************/

int ReadMemoryByte(x)   /* return 8 if byte available, 0 if not */
    ush *x;
{
    if (mem_i_offset < mem_i_size) {
        *x = (ush)mem_i_buffer[(ush)mem_i_offset];
        mem_i_offset++;
        return 8;
    } else
        return 0;
}





/****************************/
/*  Function FlushMemory()  */
/****************************/

int FlushMemory()   /* return PK-type error code */
{
    if (outcnt == 0)
        return PK_COOL;

    if (mem_o_offset + outcnt <= mem_o_size) {
        memcpy((char *)(mem_o_buffer+(ush)mem_o_offset), (char *)outbuf,
          outcnt);
        mem_o_offset += outcnt;
        return PK_COOL;
    } else
        return PK_DISK;
}





/***************************/
/*  Function memextract()  */
/***************************/

int memextract(tgt, tgtsize, src, srcsize)   /* extract compressed extra */
    uch *tgt, *src;                          /*  field block; return 0 if */
    ulg tgtsize, srcsize;                    /*  success, 1 if not */
{
    ush method, error=0;
    ulg crc, oldcrc;
    int r;

    method = makeword(src);
    crc = makelong(src+2);

    mem_i_buffer = src + 2 + 4;      /* method and crc */
    mem_i_size   = srcsize - 2 - 4;
    mem_i_offset = 0;
  
    mem_o_buffer = tgt;
    mem_o_size   = tgtsize;
    mem_o_offset = 0;

    mem_mode = 1;

    bits_left = 0;
    bitbuf = 0L;
    outpos = 0L;
    outcnt = 0;
    outptr = outbuf;
    zipeof = 0;

    switch (method) {
        case STORED:
            memcpy(tgt, src + 2 + 4, (extent) (srcsize - 2 - 4));
            break;
        case DEFLATED:        /* GRR:  change aflag/bflag temporarily? */
            if ((r = ccinflate()) != 0) {
#ifdef OLD_MSGS
                fprintf(stderr, "error:  %s\n", r == 3 ?
                  "not enough memory for inflate operation" :
                  "invalid compressed data for the inflate format");
#endif /* OLD_MSGS */
                error = (r == 3)? PK_MEM2 : PK_ERR;
            }
            FlushOutput();
            break;
        default:
#ifdef OLD_MSGS
            fprintf(stderr,
              "warning:  unsupported extra field compression type--skipping\n");
            error = 1;   /* GRR:  this should be passed on up via SetEAs() */
#endif /* OLD_MSGS */
            break;
    }

    mem_mode = 0;

    if (!error) {
        oldcrc = crc32val;
        crc32val = 0xFFFFFFFFL;
        UpdateCRC((unsigned char *)mem_o_buffer, (int)mem_o_size);
        crc32val = (~crc32val) & 0xFFFFFFFFL;

        if (crc32val != crc) {
            ZIP_Diag( ZIP_ERR_BAD_CRC );
#ifdef OLD_MSGS
            printf("(Bad extra field CRC %08lx, should be %08lx)\n", crc32val,
              crc);
#endif
            error = 1;
        }
        crc32val = oldcrc; /* grrr ... this ugly kludge should be fixed */
    }

    return error;
}
