/******************************************************************************
 *
 *  WinChatty Server
 *  Copyright (C) 2009, Brian Luft.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 ******************************************************************************
 *
 *  UTILITY.C
 *  General utility functions.
 *  
 *****************************************************************************/
#include "stdafx.h"
#include <memory.h>
#include <string.h>
#include "platform.h"
#include "utility.h"

/* Private functions. */
static void xml_entities (char** string);
static void json_quote   (char** string);

/** append ********************************************************************
 *  PARAMETERS: string   - [in/out] Buffer to mutate by appending 'appendix'.
 *              size     - [in/out] The initial size of the buffer, which is
 *                         incremented by the length of 'appendix'.
 *              appendix - [in] The string to append to 'buffer'.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Appends 'appendix' onto 'buffer', and updates 'size'.
 *****************************************************************************/
void append(char** string, size_t* size, const char* appendix)
{
   size_t appendix_length = strlen(appendix);
   *string = (char*)realloc(*string, *size + appendix_length + 1);
   strcpy_s(*string + *size, appendix_length + 1,appendix);
   *size += appendix_length;
}

/** trim **********************************************************************
 *  PARAMETERS: string - [in] The string to trim         
 *  RETURNS:    Nothing.
 *  PURPOSE:    Strips whitespace from the start and end of a string, in-place.
 *****************************************************************************/
void trim(char* string)
{
   char*  cursor;
   char*  start;
   char*  end;
   bool blank = true;
   
   if (strlen(string) == 0)
      return;

   /* Make sure the string isn't entirely spaces. */
   cursor = string;
   while (*cursor != 0)
   {
      if (*cursor != ' ')
      {
         blank = false;
         break;
      }
      else
         cursor++;
   }

   if (blank == true)
   {
      string[0] = 0;
      return;
   }

   /* Find the start. */
   cursor = string;
   while (*cursor == ' ' || *cursor == '\t' || *cursor == '\n' || *cursor == '\r')
      cursor++;
   start = cursor;

   /* Find the end. */
   cursor = string + strlen(string) - 1;
   while (*cursor == ' ' || *cursor == '\t' || *cursor == '\n' || *cursor == '\r')
      cursor--;
   end = cursor;

   /* Copy into place at the beginning of the string. */
   memmove(string, start, end - start + 1);
   string[end - start + 1] = 0;
}

/** strip_html ****************************************************************
 *  PARAMETERS: string - [in] The string to strip.       
 *  RETURNS:    Nothing.
 *  PURPOSE:    Strips all HTML tags from the string, in-place.
 *****************************************************************************/
void strip_html(char* string)
{
   size_t length       = strlen(string);
   bool inside_tag     = false;
   char*  read_cursor  = string;
   char*  write_cursor = string;

   while (*read_cursor != 0)
   {
      if (*read_cursor == '<')
         inside_tag = true;
      else if (*read_cursor == '>')
         inside_tag = FALSE;
      else if (inside_tag == false && *read_cursor != '\n' && *read_cursor != '\r')
         *(write_cursor++) = *read_cursor;
      else if (inside_tag == false && *read_cursor == '\n')
         *(write_cursor++) = ' ';

      read_cursor++;
   }

   *write_cursor = 0;
}

/** str_make_safe *************************************************************
 *  PARAMETERS: string - [in/out] The original string.
 *              format - [in] The output format to make this string safe for.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Makes a string safe for embedding in XML or JSON.
 *****************************************************************************/
void str_make_safe(char** string, response_format format)
{
   if (format == XML)
      xml_entities(string);
   else if (format == JSON)
      json_quote(string);
}

/** xml_entities **************************************************************
 *  PARAMETERS: string - [in/out] The original string.      
 *  RETURNS:    Nothing.
 *  PURPOSE:    Makes a string safe for embedding in XML.
 *****************************************************************************/
void xml_entities(char** string)
{
   static const char* needles = 
      "\"<>\r\n&";
   static const char* replacements[] = {
      "&quot;", "&lt;", "&gt;", "&#13;", "", "&amp;" };

   str_replace(string, needles, replacements);
}

/** json_quote ****************************************************************
 *  PARAMETERS: string - [in/out] The original string.      
 *  RETURNS:    Nothing.
 *  PURPOSE:    Makes a string safe for embedding in a JSON string.
 *****************************************************************************/
void json_quote(char** string)
{
   static const char* needles = 
      "&<>\"";
   static const char* replacements[] = {
      "\\u0026", "\\u003C", "\\u003E", "\\\"" };

   xml_entities(string);
   str_replace(string, needles, replacements);
}

/** str_replace ***************************************************************
 *  PARAMETERS: string       - [in/out] The original string.      
 *              needles      - [in] Characters to search for.
 *              replacements - [in] Strings to replace each character with.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Replaces all instances of characters from "needles" in "string"
 *              with the corresponding string in "replacements."
 *****************************************************************************/
void str_replace(char** string, const char* needles, const char** replacements)
{
   char*        new_string;
   const char*  read_cursor = *string;
   char*        write_cursor;
   size_t       size = 1;

   while (*read_cursor != 0)
   {
      char* needle = (char*)strchr(needles, *read_cursor);
      if (needle == NULL)
         size++;
      else
         size += strlen(replacements[needle - needles]);

      read_cursor++;
   }

   new_string   = (char*)malloc(size);
   read_cursor  = *string;
   write_cursor = new_string;

   while (*read_cursor != 0)
   {
      char* needle = (char*)strchr(needles, *read_cursor);
      if (needle == NULL)
      {
         *write_cursor = *read_cursor;
         write_cursor++;
      }
      else
      {
         strcpy_s(write_cursor, size, replacements[needle - needles]);
         write_cursor += strlen(replacements[needle - needles]);
      }

      read_cursor++;
   }

   *write_cursor = 0;

   free(*string);
   *string = new_string;
}

/** fix_spoilers **************************************************************
 *  PARAMETERS: string       - [in/out] The original string.      
 *  RETURNS:    Nothing.
 *  PURPOSE:    Replaces the onClick handler of spoiler text to function when
 *              the Shacknews .JS file is not present.
 *****************************************************************************/
void fix_spoilers(char* string)
{
   const char* search      = "return doSpoiler( event );";
   const char* replacement = "this.className = '';      ";
   size_t      length      = strlen(search);
   char*       ptr         = NULL;

   /* Replace the spoiler onClick handler with one that doesn't require
      the Shacknews .js file. */
   while (ptr = strstr(string, search))
   {
      /* Overwrite the onClick handler. */
      memcpy(ptr, replacement, length);
   }
}
