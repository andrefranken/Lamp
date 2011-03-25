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
 *  CHATTY.H
 *  Parses the HTML for a chatty and formats it into XML.
 *  
 *****************************************************************************/

#pragma once

#include "chattyerror.h"
#include "utility.h"
#include "set.h"
#include "stack.h"

typedef struct root_post root_post_t;
typedef struct chatty    chatty_t;

struct root_post
{
   unsigned int id;
   unsigned int last_reply_id;
   char*        author;
   char*        date;
   char*        preview;
   char*        body;
   unsigned int reply_count;
   char*        category;
   set_t*       participants;
};

struct chatty
{
   unsigned int  story_id;
   char*         story_name;
   unsigned int  last_page;
   unsigned int  page;
   array_t*      posts;
};

chattyerror chatty_parse  (char*        html, 
                       unsigned int id, 
                       chatty_t**   out_chatty);
                       
chattyerror chatty_format (chatty_t*    chatty, 
                          response_format format, 
                          char** out);
                       
void    chatty_free   (chatty_t* chatty);

