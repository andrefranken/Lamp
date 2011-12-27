/*---------------------------------------------------------------------------

  shared.c

  This file contains general routines shared by both UnZip and ZipInfo.
  [Routines orginally were in unzip.c, zipinfo.c, file_io.c and misc.c.]

  ---------------------------------------------------------------------------

  Copyrights:  see accompanying file "COPYING" in UnZip source distribution.

  ---------------------------------------------------------------------------*/

#include "ziplib.h"
#include "unzip.h"
#ifdef MSWIN
#  include "wizunzip.h"
#endif


/******************************/
/* Function open_input_file() */
/******************************/

int open_input_file()    /* return 1 if open failed */
{
    /*
     *  open the zipfile for reading and in BINARY mode to prevent cr/lf
     *  translation, which would corrupt the bitstreams
     */

#if defined(UNIX) || defined(TOPS20)
    zipfd = open(zipfn, O_RDONLY);
#else /* !(UNIX || TOPS20) */
#ifdef VMS
    zipfd = open(zipfn, O_RDONLY, 0, "ctx=stm");
#else /* !VMS */
#ifdef MACOS
    zipfd = open(zipfn, 0);
#else /* !MACOS */
    zipfd = _open(zipfn, O_RDONLY | O_BINARY);
#endif /* ?MACOS */
#endif /* ?VMS */
#endif /* ?(UNIX || TOPS20) */
    if (zipfd < 1) {
        ZIP_DiagFile( ZIP_ERR_ZIP_NOT_FOUND, zipfn );
#ifdef OLD_MSGS
        fprintf(stderr, "error:  can't open zipfile [ %s ]\n", zipfn);
#endif /* OLD_MSGS */
        return 1;
    }
    return 0;

} /* end function open_input_file() */





/**********************/
/* Function readbuf() */
/**********************/

int readbuf(buf, size)   /* return number of bytes read into buf */
    char *buf;
    register unsigned size;
{
    register int count;
    int n;

    n = size;
    while (size) {
        if (incnt == 0) {
            if ((incnt = _read(zipfd, (char *)inbuf, INBUFSIZ)) <= 0)
                return (n-size);
            /* buffer ALWAYS starts on a block boundary:  */
            cur_zipfile_bufstart += INBUFSIZ;
            inptr = inbuf;
        }
        count = MIN(size, (unsigned)incnt);
        memcpy(buf, inptr, count);
        buf += count;
        inptr += count;
        incnt -= count;
        size -= count;
    }
    return n;

} /* end function readbuf() */





/***********************************/
/* Function find_end_central_dir() */
/***********************************/

int find_end_central_dir(searchlen)   /* return PK-class error */
    longint searchlen;
{
    int i, numblks, found=FALSE;
    longint tail_len;
    ec_byte_rec byterec;


/*---------------------------------------------------------------------------
    Treat case of short zipfile separately.
  ---------------------------------------------------------------------------*/

    if (ziplen <= INBUFSIZ) {
        _lseek(zipfd, 0L, SEEK_SET);
        if ((incnt = _read(zipfd,(char *)inbuf,(unsigned int)ziplen)) ==
             (int)ziplen)

            /* 'P' must be at least 22 bytes from end of zipfile */
            for (inptr = inbuf+(int)ziplen-22;  inptr >= inbuf;  --inptr)
                if ((ascii_to_native(*inptr) == 'P')  &&
                     !strncmp((char *)inptr, end_central_sig, 4)) {
                    incnt -= inptr - inbuf;
                    found = TRUE;
                    break;
                }

/*---------------------------------------------------------------------------
    Zipfile is longer than INBUFSIZ:  may need to loop.  Start with short
    block at end of zipfile (if not TOO short).
  ---------------------------------------------------------------------------*/

    } else {
        if ((tail_len = ziplen % INBUFSIZ) > ECREC_SIZE) {
            cur_zipfile_bufstart = _lseek(zipfd, ziplen-tail_len, SEEK_SET);
            if ((incnt = _read(zipfd,(char *)inbuf,(unsigned int)tail_len)) !=
                 (int)tail_len)
                goto fail;      /* shut up; it's expedient */

            /* 'P' must be at least 22 bytes from end of zipfile */
            for (inptr = inbuf+(int)tail_len-22;  inptr >= inbuf;  --inptr)
                if ((ascii_to_native(*inptr) == 'P')  &&
                     !strncmp((char *)inptr, end_central_sig, 4)) {
                    incnt -= inptr - inbuf;
                    found = TRUE;
                    break;
                }
            /* sig may span block boundary: */
            strncpy((char *)hold, (char *)inbuf, 3);
        } else
            cur_zipfile_bufstart = ziplen - tail_len;

    /*-----------------------------------------------------------------------
        Loop through blocks of zipfile data, starting at the end and going
        toward the beginning.  In general, need not check whole zipfile for
        signature, but may want to do so if testing.
      -----------------------------------------------------------------------*/

        numblks = (int)((searchlen - tail_len + (INBUFSIZ-1)) / INBUFSIZ);
        /*               ==amount=   ==done==   ==rounding==    =blksiz=  */

        for (i = 1;  !found && (i <= numblks);  ++i) {
            cur_zipfile_bufstart -= INBUFSIZ;
            _lseek(zipfd, cur_zipfile_bufstart, SEEK_SET);
            if ((incnt = _read(zipfd,(char *)inbuf,INBUFSIZ)) != INBUFSIZ)
                break;          /* fall through and fail */

            for (inptr = inbuf+INBUFSIZ-1;  inptr >= inbuf;  --inptr)
                if ((ascii_to_native(*inptr) == 'P')  &&
                     !strncmp((char *)inptr, end_central_sig, 4)) {
                    incnt -= inptr - inbuf;
                    found = TRUE;
                    break;
                }
            /* sig may span block boundary: */
            strncpy((char *)hold, (char *)inbuf, 3);
        }
    } /* end if (ziplen > INBUFSIZ) */

/*---------------------------------------------------------------------------
    Searched through whole region where signature should be without finding
    it.  Print informational message and die a horrible death.
  ---------------------------------------------------------------------------*/

fail:
    if (!found) {
#ifdef MSWIN
        MessageBeep(1);
#endif
#ifdef OLD_MSGS
        fprintf(stderr, "  %s:\n\n\
  End-of-central-directory signature not found.  Either this file is not\n\
  a zipfile, or it constitutes one disk of a multi-part archive.  In the\n\
  latter case the central directory and zipfile comment will be found on\n\
  the last disk(s) of this archive.\n", zipfn);
#endif /* OLD_MSGS */
        return PK_ERR;   /* failed */
    }

/*---------------------------------------------------------------------------
    Found the signature, so get the end-central data before returning.  Do
    any necessary machine-type conversions (byte ordering, structure padding
    compensation) by reading data into character array and copying to struct.
  ---------------------------------------------------------------------------*/

    real_ecrec_offset = cur_zipfile_bufstart + (inptr-inbuf);
#ifdef TEST
    printf("\n  found end-of-central-dir signature at offset %ld (%.8lXh)\n",
      real_ecrec_offset, real_ecrec_offset);
    printf("    from beginning of file; offset %d (%.4Xh) within block\n",
      inptr-inbuf, inptr-inbuf);
#endif

    if (readbuf((char *) byterec, ECREC_SIZE+4) <= 0)
        return PK_EOF;

    ecrec.number_this_disk =
        makeword(&byterec[NUMBER_THIS_DISK]);
    ecrec.num_disk_with_start_central_dir =
        makeword(&byterec[NUM_DISK_WITH_START_CENTRAL_DIR]);
    ecrec.num_entries_centrl_dir_ths_disk =
        makeword(&byterec[NUM_ENTRIES_CENTRL_DIR_THS_DISK]);
    ecrec.total_entries_central_dir =
        makeword(&byterec[TOTAL_ENTRIES_CENTRAL_DIR]);
    ecrec.size_central_directory =
        makelong(&byterec[SIZE_CENTRAL_DIRECTORY]);
    ecrec.offset_start_central_directory =
        makelong(&byterec[OFFSET_START_CENTRAL_DIRECTORY]);
    ecrec.zipfile_comment_length =
        makeword(&byterec[ZIPFILE_COMMENT_LENGTH]);

    expect_ecrec_offset = ecrec.offset_start_central_directory +
                          ecrec.size_central_directory;
    return PK_COOL;

} /* end function find_end_central_dir() */





/********************************/
/* Function get_cdir_file_hdr() */
/********************************/

int get_cdir_file_hdr()   /* return PK-type error code */
{
    cdir_byte_hdr byterec;


/*---------------------------------------------------------------------------
    Read the next central directory entry and do any necessary machine-type
    conversions (byte ordering, structure padding compensation--do so by
    copying the data from the array into which it was read (byterec) to the
    usable struct (crec)).
  ---------------------------------------------------------------------------*/

    if (readbuf((char *)byterec, CREC_SIZE) <= 0)
        return PK_EOF;

    crec.version_made_by[0] = byterec[C_VERSION_MADE_BY_0];
    crec.version_made_by[1] = byterec[C_VERSION_MADE_BY_1];
    crec.version_needed_to_extract[0] = byterec[C_VERSION_NEEDED_TO_EXTRACT_0];
    crec.version_needed_to_extract[1] = byterec[C_VERSION_NEEDED_TO_EXTRACT_1];

    crec.general_purpose_bit_flag =
        makeword(&byterec[C_GENERAL_PURPOSE_BIT_FLAG]);
    crec.compression_method =
        makeword(&byterec[C_COMPRESSION_METHOD]);
    crec.last_mod_file_time =
        makeword(&byterec[C_LAST_MOD_FILE_TIME]);
    crec.last_mod_file_date =
        makeword(&byterec[C_LAST_MOD_FILE_DATE]);
    crec.crc32 =
        makelong(&byterec[C_CRC32]);
    crec.csize =
        makelong(&byterec[C_COMPRESSED_SIZE]);
    crec.ucsize =
        makelong(&byterec[C_UNCOMPRESSED_SIZE]);
    crec.filename_length =
        makeword(&byterec[C_FILENAME_LENGTH]);
    crec.extra_field_length =
        makeword(&byterec[C_EXTRA_FIELD_LENGTH]);
    crec.file_comment_length =
        makeword(&byterec[C_FILE_COMMENT_LENGTH]);
    crec.disk_number_start =
        makeword(&byterec[C_DISK_NUMBER_START]);
    crec.internal_file_attributes =
        makeword(&byterec[C_INTERNAL_FILE_ATTRIBUTES]);
    crec.external_file_attributes =
        makelong(&byterec[C_EXTERNAL_FILE_ATTRIBUTES]);  /* LONG, not word! */
    crec.relative_offset_local_header =
        makelong(&byterec[C_RELATIVE_OFFSET_LOCAL_HEADER]);

    return PK_COOL;

} /* end function get_cdir_file_hdr() */





/************************/
/* Function do_string() */
/************************/

int do_string(len, option)      /* return PK-type error code */
    unsigned int len;           /* without prototype, ush converted to this */
    int option;
{
    int block_length, error=PK_COOL;
    ush comment_bytes_left, extra_len;


/*---------------------------------------------------------------------------
    This function processes arbitrary-length (well, usually) strings.  Three
    options are allowed:  SKIP, wherein the string is skipped pretty logical,
    eh?); DISPLAY, wherein the string is printed to standard output after un-
    dergoing any necessary or unnecessary character conversions; and FILENAME,
    wherein the string is put into the filename[] array after undergoing ap-
    propriate conversions (including case-conversion, if that is indicated:
    see the global variable pInfo->lcflag).  The latter option should be OK,
    since filename is now dimensioned at 1025, but we check anyway.

    The string, by the way, is assumed to start at the current file-pointer
    position; its length is given by len.  So start off by checking length
    of string:  if zero, we're already set.
  ---------------------------------------------------------------------------*/

    if (!len)
        return PK_COOL;

    switch (option) {

    /*
     * First case:  print string on standard output.  First set loop vari-
     * ables, then loop through the comment in chunks of OUTBUFSIZ bytes,
     * converting formats and printing as we go.  The second half of the
     * loop conditional was added because the file might be truncated, in
     * which case comment_bytes_left will remain at some non-zero value for
     * all time.  outbuf is used as a scratch buffer because it is avail-
     * able (we should be either before or in between any file processing).
     * [The typecast in front of the MIN() macro was added because of the
     * new promotion rules under ANSI C; readbuf() wants an int, but MIN()
     * returns a signed long, if I understand things correctly.  The proto-
     * type should handle it, but just in case...]
     */

    case DISPLAY:
        comment_bytes_left = len;
        block_length = OUTBUFSIZ;    /* for the while statement, first time */
        while (comment_bytes_left > 0 && block_length > 0) {
            if ((block_length = readbuf((char *)outbuf,
                   (int) MIN(OUTBUFSIZ, comment_bytes_left))) <= 0)
                return PK_EOF;
            comment_bytes_left -= block_length;
            NUKE_CRs(outbuf, block_length);     /* (modifies block_length) */

            /*  this is why we allocated an extra byte for outbuf: */
            outbuf[block_length] = '\0';        /* terminate w/zero:  ASCIIZ */

            A_TO_N(outbuf);     /* translate string to native */

#ifdef MSWIN
            /* ran out of local mem -- had to cheat */
            WriteStringToMsgWin(outbuf, bRealTimeMsgUpdate);
#else /* !MSWIN */
# ifdef OLD_MSGS
            printf("%s", outbuf);
# endif /* OLD_MSGS */
#endif /* ?MSWIN */
        }
 #ifdef OLD_MSGS
        printf("\n");   /* assume no newline at end */
 #endif /* OLD_MSGS */
        break;

    /*
     * Second case:  read string into filename[] array.  The filename should
     * never ever be longer than FILNAMSIZ-1 (1024), but for now we'll check,
     * just to be sure.
     */

    case FILENAME:
        extra_len = 0;
        if (len >= FILNAMSIZ) {
            ZIP_DiagFile( ZIP_ERR_TRUNC_FILENAME, filename );
#ifdef OLD_MSGS
            fprintf(stderr, "warning:  filename too long--truncating.\n");
#endif /* OLD_MSGS */
            error = PK_WARN;
            extra_len = len - FILNAMSIZ + 1;
            len = FILNAMSIZ - 1;
        }
        if (readbuf(filename, len) <= 0)
            return PK_EOF;
        filename[len] = '\0';   /* terminate w/zero:  ASCIIZ */

        A_TO_N(filename);       /* translate string to native */

        if (pInfo->lcflag)      /* replace with lowercase filename */
            TOLOWER(filename, filename);

        if (!extra_len)         /* we're done here */
            break;

        /*
         * We truncated the filename, so print what's left and then fall
         * through to the SKIP routine.
         */
#ifdef OLD_MSGS
        fprintf(stderr, "[ %s ]\n", filename);
#endif /* OLD_MSGS */
        len = extra_len;
        /*  FALL THROUGH...  */

    /*
     * Third case:  skip string, adjusting readbuf's internal variables
     * as necessary (and possibly skipping to and reading a new block of
     * data).
     */

    case SKIP:
        LSEEK(cur_zipfile_bufstart + (inptr-inbuf) + len)
        break;

    /*
     * Fourth case:  assume we're at the start of an "extra field"; malloc
     * storage for it and read data into the allocated space.
     */

    case EXTRA_FIELD:
        if (extra_field != (uch *)NULL)
            free(extra_field);
        if ((extra_field = (uch *)malloc(len)) == (uch *)NULL) {
#ifdef OLD_MSGS
            fprintf(stderr,
              "warning:  extra field too long (%d).  Ignoring...\n", len);
#endif /* OLD_MSGS */
            LSEEK(cur_zipfile_bufstart + (inptr-inbuf) + len)
        } else
            if (readbuf((char *)extra_field, len) <= 0)
                return PK_EOF;
        break;

    } /* end switch (option) */
    return error;

} /* end function do_string() */





#ifdef EBCDIC

/*
 * This is the MTS ASCII->EBCDIC translation table. It provides a 1-1
 * translation from ISO 8859/1 8-bit ASCII to IBM Code Page 37 EBCDIC.
 */

unsigned char ebcdic[] =
{
    0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f,
    0x16, 0x05, 0x25, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26,
    0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f,
    0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d,
    0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f,
    0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6,
    0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
    0xe7, 0xe8, 0xe9, 0xba, 0xe0, 0xbb, 0xb0, 0x6d,
    0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6,
    0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0xa1, 0x07,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x15, 0x06, 0x17,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x1b,
    0x30, 0x31, 0x1a, 0x33, 0x34, 0x35, 0x36, 0x08,
    0x38, 0x39, 0x3a, 0x3b, 0x04, 0x14, 0x3e, 0xff,
    0x41, 0xaa, 0x4a, 0xb1, 0x9f, 0xb2, 0x6a, 0xb5,
    0xbd, 0xb4, 0x9a, 0x8a, 0x5f, 0xca, 0xaf, 0xbc,
    0x90, 0x8f, 0xea, 0xfa, 0xbe, 0xa0, 0xb6, 0xb3,
    0x9d, 0xda, 0x9b, 0x8b, 0xb7, 0xb8, 0xb9, 0xab,
    0x64, 0x65, 0x62, 0x66, 0x63, 0x67, 0x9e, 0x68,
    0x74, 0x71, 0x72, 0x73, 0x78, 0x75, 0x76, 0x77,
    0xac, 0x69, 0xed, 0xee, 0xeb, 0xef, 0xec, 0xbf,
    0x80, 0xfd, 0xfe, 0xfb, 0xfc, 0xad, 0xae, 0x59,
    0x44, 0x45, 0x42, 0x46, 0x43, 0x47, 0x9c, 0x48,
    0x54, 0x51, 0x52, 0x53, 0x58, 0x55, 0x56, 0x57,
    0x8c, 0x49, 0xcd, 0xce, 0xcb, 0xcf, 0xcc, 0xe1,
    0x70, 0xdd, 0xde, 0xdb, 0xdc, 0x8d, 0x8e, 0xdf
}; /* end ebcdic[] */

#endif                          /* EBCDIC */





/***********************/
/* Function makeword() */
/***********************/

ush makeword(b)
    uch *b;
{
    /*
     * Convert Intel style 'short' integer to non-Intel non-16-bit
     * host format.  This routine also takes care of byte-ordering.
     */
    return ((b[1] << 8) | b[0]);
}





/***********************/
/* Function makelong() */
/***********************/

ulg makelong(sig)
    uch *sig;
{
    /*
     * Convert intel style 'long' variable to non-Intel non-16-bit
     * host format.  This routine also takes care of byte-ordering.
     */
    return (((ulg)sig[3]) << 24)
        + (((ulg)sig[2]) << 16)
        + (((ulg)sig[1]) << 8)
        + ((ulg)sig[0]);
}





#ifdef ZMEM   /* memset, memcpy for systems without them */

/*********************/
/* Function memset() */
/*********************/

char *memset(buf, init, len)
    register char *buf, init;   /* buffer loc and initializer */
    register unsigned int len;  /* length of the buffer */
{
    char *start;

    start = buf;
    while (len--)
        *(buf++) = init;
    return start;
}





/*********************/
/* Function memcpy() */
/*********************/

char *memcpy(dst, src, len)
    register char *dst, *src;
    register unsigned int len;
{
    char *start;

    start = dst;
    while (len-- > 0)
        *dst++ = *src++;
    return start;
}

#endif /* ZMEM */
