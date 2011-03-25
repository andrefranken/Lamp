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
 *  ERROR.C
 *  Error handling.
 *  
 *****************************************************************************/
#include "stdafx.h"
#include "chattyerror.h"

/** get_error_string **********************************************************
 *  PARAMETERS: error - [in] Error code.
 *  RETURNS:    Human-readable text for the specified error.  This string
 *              should not be free()'d by the caller.
 *  PURPOSE:    Converts an error code into a string for display.
 *****************************************************************************/
const char* get_error_string(chattyerror error)
{
   static const char* strings[] = {
      "OK",
      "Not implemented.",
      
      /* Webserver errors */
      "Error opening the web server's listening socket.",
      "Error binding the socket to the port.",
      "Error listening for incoming connection requests.",
      "Error connecting to the remote server.",
      "Error accepting new connection requests.",
      "Error receiving data from the client.",
      "Error sending data to the client.",
      "The request is not a GET.",
      "The request does not contain a path.",
      "The requested path is invalid.",
      
      /* Chatty errors */
      "Error downloading the chatty HTML.",
      "Unable to parse the chatty HTML.",
      "Unable to parse the thread HTML.",
      "Attempted to pop from an empty stack.",
      "Invalid argument.",
      "Unable to spawn a new thread.",
      
      /* Fine-grained error messages for thread_parse(). */
      "Unable to parse the thread HTML. (Post ID)",
      "Unable to parse the thread HTML. (Category; Step 1)",
      "Unable to parse the thread HTML. (Category; Step 2)",
      "Unable to parse the thread HTML. (Category; Step 3)",
      "Unable to parse the thread HTML. (Category; Step 4)",
      "Unable to parse the thread HTML. (Preview; Step 1)",
      "Unable to parse the thread HTML. (Preview; Step 2)",
      "Unable to parse the thread HTML. (Preview; Step 3)",
      "Unable to parse the thread HTML. (Author; Step 1)",
      "Unable to parse the thread HTML. (Author; Step 2)",
      "Unable to parse the thread HTML. (Author; Step 3)",
      "Unable to parse the thread HTML. (Author; Step 4)",
      "Unable to parse the thread HTML. (Missing </ul>)",
      "Unable to parse the thread HTML. (Malformed tree)",
      "Unable to parse the thread HTML. (Content Post ID)",
      "The structure and content thread data does not match.",
      "Unable to parse the thread HTML. (Post Body; Step 1)",
      "Unable to parse the thread HTML. (Post Body; Step 2)",
      "Unable to parse the thread HTML. (Post Body; Step 3)",
      "Unable to parse the thread HTML. (Post Date; Step 1)",
      "Unable to parse the thread HTML. (Post Date; Step 2)",
      "Unable to parse the thread HTML. (Post Date; Step 3)",
   };

   return strings[error];
}
