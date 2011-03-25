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
 *  ERROR.H
 *  Error handling.
 *  
 *****************************************************************************/

#pragma once

typedef enum
{
   ERR_OK = 0,
   ERR_NOT_IMPLEMENTED,
   
   /* Webserver errors */
   ERR_SOCKET_OPEN,
   ERR_SOCKET_BIND,
   ERR_SOCKET_LISTEN,
   ERR_SOCKET_CONNECT,
   ERR_SOCKET_ACCEPT,
   ERR_SOCKET_RECV,
   ERR_SOCKET_SEND,
   ERR_REQUEST_NOT_GET,
   ERR_REQUEST_NO_PATH,
   ERR_REQUEST_INVALID_PATH,
   
   /* Chatty errors */
   ERR_CHATTY_DOWNLOAD,
   ERR_CHATTY_PARSE,
   ERR_THREAD_PARSE,
   ERR_STACK_EMPTY,
   ERR_INVALID_ARGUMENT,
   ERR_THREAD_CREATE,
   
   /* Fine-grained error messages for thread_parse(). */
   ERR_THREAD_PARSE_POST_ID,
   ERR_THREAD_PARSE_CATEGORY1,
   ERR_THREAD_PARSE_CATEGORY2,
   ERR_THREAD_PARSE_CATEGORY3,
   ERR_THREAD_PARSE_CATEGORY4,
   ERR_THREAD_PARSE_PREVIEW1,
   ERR_THREAD_PARSE_PREVIEW2,
   ERR_THREAD_PARSE_PREVIEW3,
   ERR_THREAD_PARSE_AUTHOR1,
   ERR_THREAD_PARSE_AUTHOR2,
   ERR_THREAD_PARSE_AUTHOR3,
   ERR_THREAD_PARSE_AUTHOR4,
   ERR_THREAD_PARSE_MISSING_END_UL,
   ERR_THREAD_PARSE_MALFORMED_TREE,
   ERR_THREAD_PARSE_CONTENT_ID,
   ERR_THREAD_PARSE_MISMATCHED_DATA,
   ERR_THREAD_PARSE_POST_BODY1,
   ERR_THREAD_PARSE_POST_BODY2,
   ERR_THREAD_PARSE_POST_BODY3,
   ERR_THREAD_PARSE_POST_DATE1,
   ERR_THREAD_PARSE_POST_DATE2,
   ERR_THREAD_PARSE_POST_DATE3,
} chattyerror;

const char* get_error_string (chattyerror error);

