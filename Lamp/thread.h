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
 *  THREAD.H
 *  Parses the HTML for a thread and formats it into XML.
 *  
 *****************************************************************************/

#pragma once

#include "chattyerror.h"
#include "stack.h"
#include "set.h"
#include "utility.h"

typedef struct thread_post thread_post_t;
typedef struct thread      thread_t;

struct thread_post
{
   unsigned int id;
   char*        author;
   char*        date;
   char*        preview;
   char*        body;
   char*        category;
   array_t*     replies;
};

struct thread
{
   thread_post_t* root;
   unsigned int   last_reply_id;
   unsigned int   reply_count;
   set_t*         participants;
};

chattyerror thread_parse  (char*             tree_html, 
                           char*             content_html, 
                           thread_t**        out_thread);
                       
chattyerror thread_format (thread_t*         thread, 
                           response_format   format, 
                           char**            out);
                       
void    thread_free   (thread_t*         thread);

