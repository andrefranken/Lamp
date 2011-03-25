/*
 * jutils.c
 *
 * Copyright (C) 1991-1996, Thomas G. Lane.
 * This file is part of the Independent JPEG Group's software.
 * For conditions of distribution and use, see the accompanying README file.
 *
 * This file contains tables and miscellaneous utility routines needed
 * for both compression and decompression.
 * Note we prefix all global names with "j" to minimize conflicts with
 * a surrounding application.
 */

#pragma once
#include "jinclude.h"
#include "jpeglib.h"

size_t JFREAD(FILE* file, void* buf, size_t sizeofbuf)
{
   return fread(buf, 1, sizeofbuf, file);
}

size_t JFWRITE(FILE* file, void* buf, size_t sizeofbuf)
{
   return fwrite((const void *)buf, 1, sizeofbuf, file);
}

int JFFLUSH(FILE* file)
{  
   return fflush(file);
}

int JFERROR(FILE* file)
{
   return ferror(file);
}

int JFSEEK(FILE* file, long offset, int origin)
{
   return fseek(file, offset, origin);
}

int JFCLOSE(FILE* file)
{
   return fclose(file);
}

int JGETC(FILE* file)
{
   return getc(file);
}

int JPUTC(int ch, FILE* file)
{
   return putc(ch, file);
}
