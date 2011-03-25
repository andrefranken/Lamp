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
 *  CHATTY.C
 *  Parses the HTML for a chatty and formats it into XML.
 *  
 *****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "platform.h"
#include "utility.h"
#include "chatty.h"

/* Private functions. */
static chatty_t*    chatty_new     (void);
static root_post_t* root_post_new  (void);
static void         root_post_free (root_post_t* root_post);

/** chatty_parse **************************************************************
 *  PARAMETERS: html       - [in] Downloaded chatty HTML.
 *              id         - [in] Story ID.
 *              out_chatty - [out] The parsed chatty structure.
 *  RETURNS:    Error code.
 *  PURPOSE:    Parses the raw HTML into our chatty_t structure.
 *****************************************************************************/
chattyerror chatty_parse(char* html, unsigned int id, chatty_t** out_chatty)
{
#define SEEK(cursor_var, needle)           \
   {                                       \
      cursor_var = strstr(cursor, needle); \
      if (cursor_var == NULL)              \
      {                                    \
         chatty_free(chatty);              \
         return ERR_CHATTY_PARSE;          \
      }                                    \
   }

#define READ(dest, start, end)             \
   {                                       \
      dest = (char*)malloc(end - start + 1);      \
      memcpy(dest, start, end - start);    \
      dest[end - start] = 0;               \
   }

   chatty_t* chatty   = chatty_new();
   set_t*    posters  = set_new();
   char*     cursor   = html;
   char*     c1       = NULL;
   char*     c2       = NULL;
   char*     temp_str = NULL;

   chatty->story_id = id;

   /* Locate the "story" div. */
   SEEK(cursor, "<div class=\"story\">");
   SEEK(cursor, "/onearticle.x/");
   SEEK(cursor, ">");
   SEEK(c1, "<");
   cursor++;
   READ(chatty->story_name, cursor, c1);

   /* Locate the "pagenavigation" div. */
   SEEK(cursor, "<div class=\"pagenavigation");
   SEEK(cursor, ">");

   /* Iterate through the page buttons to compute 'last_page' and 'page'. */
   while (1)
   {
      c1 = strstr(cursor, "<a ");
      c2 = strstr(cursor, "</div>");

      if (c1 < c2)
      {
         bool is_selected = false;
         
         /* There is another page button. */
         cursor = c1 + 1;
         chatty->last_page++;

         c1 = strstr(cursor, "class=");
         c2 = strstr(cursor, "href=");

         if (c1 < c2)
            is_selected = TRUE;

         cursor = c2;
         SEEK(cursor, "&page=");
         cursor += 6;

         if ((unsigned int)atoi(cursor) > chatty->last_page)
            chatty->last_page = atoi(cursor);

         if (is_selected == TRUE)
            chatty->page = chatty->last_page;
      }
      else
         break;
   }

   /* Iterate through the posts. */
   while (cursor != NULL)
   {
      root_post_t* post;

      cursor = strstr(cursor, "<div class=\"fullpost");
      if (cursor == NULL)
         break;

      /* Create the new root post. */
      post = root_post_new();
      array_add(chatty->posts, post);

      /* Parse the post moderation flag. */
      SEEK(cursor, "fpmod_");
      SEEK(cursor, "_");
      SEEK(c1, " ");
      cursor++;
      READ(post->category, cursor, c1);

      /* Parse the post ID. */
      SEEK(cursor, "<div class=\"postnumber\">");
      SEEK(cursor, "id=");
      cursor += 3;
      post->id = atoi(cursor);

      /* Parse the post's author. */
      SEEK(cursor, "/profile/");
      SEEK(cursor, ">");
      SEEK(c1, "<");
      cursor++;
      READ(post->author, cursor, c1);
      trim(post->author);

      /* Add the root post's author as the first thread participant. */
      set_add(post->participants, post->author);

      /* Parse the post body. */
      SEEK(cursor, "<div class=\"postbody\">");
      SEEK(cursor, ">");
      SEEK(c1, "</div>");
      cursor++;
      READ(post->body, cursor, c1);
      trim(post->body);
      fix_spoilers(post->body);

      /* Parse the post date. */
      SEEK(cursor, "<div class=\"postdate\">");
      SEEK(cursor, ">");
      SEEK(c1, "</div>");
      cursor++;
      READ(post->date, cursor, c1);

      /* Parse the preview. */
      SEEK(cursor, "<span class=\"oneline_body\">");
      SEEK(cursor, ">");
      SEEK(c1, "</span> :");
      cursor++;
      READ(post->preview, cursor, c1);
      strip_html(post->preview);
      trim(post->preview);

      /* Look through all the replies to get the names of all 
         thread participants. */
      while (1)
      {
         c1 = strstr(cursor, "<div class=\"oneline");
         c2 = strstr(cursor, "<div class=\"fullpost");
         if (c1 != NULL && (c2 == NULL || c1 < c2))
         {
            bool   found = false;

            post->reply_count++;

            /* If there are no replies, there will be no "oneline" underneath
               the capnote. */
            cursor = c1;
            c1 = strstr(cursor, "oneline0");
            c2 = strstr(cursor, "\">");
            if (c1 != NULL && c2 != NULL && c1 < c2)
            {
               /* This is the most recent reply.  Grab the ID. */
               SEEK(cursor, "laryn.x?id=");
               SEEK(cursor, "=");
               SEEK(c1, "\"");
               cursor++;
               READ(temp_str, cursor, c1);
               trim(temp_str);
               post->last_reply_id = atoi(temp_str);
               free(temp_str);
            }

            /* Parse the author of this thread reply. */
            SEEK(cursor, "class=\"oneline_user");
            SEEK(cursor, ">");
            SEEK(c1, "</a>");
            cursor++;
            READ(temp_str, cursor, c1);
            trim(temp_str);

            set_add(post->participants, temp_str);
         }
         else
         {
            break;
         }
      }
   }

   *out_chatty = chatty;
   return ERR_OK;

#undef SEEK
#undef READ
}

/** chatty_format *************************************************************
 *  PARAMETERS: chatty  - [in] The parsed chatty structure.
 *              format  - [in] XML or JSON.
 *              out     - [out] The generated XML/JSON to return to the client.
 *  RETURNS:    Error code.
 *  PURPOSE:    Converts the chatty structure into the XML/JSON format 
 *              expected by the remote HTTP client.
 *****************************************************************************/
chattyerror chatty_format(chatty_t* chatty, response_format format, char** out)
{
   const char*  header_template;
   const char*  comment_header_template;
   const char*  participants_header;
   const char*  participant_template;
   const char*  participants_footer;
   const char*  comment_footer;
   const char*  footer;
   char*        xml      = NULL;
   size_t       xml_size = 0;
   char*        buffer   = NULL;
   unsigned int i, j;

   /* Set up the sprintf templates based on the desired response format. */
   if (format == XML)
   {
      header_template =         "<comments story_id=\"%u\" story_name=\"%s\" " 
                                "last_page=\"%u\" page=\"%u\">\n\n";
      comment_header_template = "<comment last_reply_id=\"%u\" author=\"%s\" "
                                "date=\"%s\" preview=\"%s\" reply_count=\"%d\" "
                                "id=\"%u\" category=\"%s\">\n"
                                "<body>%s</body>\n<comments></comments>\n";
      participants_header =     "<participants>\n";
      participant_template =    "<participant>%s</participant>\n";
      participants_footer =     "</participants>\n";
      comment_footer =          "</comment>\n\n";
      footer =                  "</comments>\n\n";
   }
   else if (format == JSON)
   {
      header_template =         "{\"story_id\": \"%u\", \"story_name\": \"%s\", "
                                "\"last_page\": %u, \"page\": %u, \"comments\": [\n\n";
      comment_header_template = "{\"last_reply_id\": %u, \"author\": \"%s\", "
                                "\"date\": \"%s\", \"preview\": \"%s\", "
                                "\"reply_count\": %u, \"id\": %u, \"category\": \"%s\", "
                                "\"body\": \"%s\", \"comments\": [], \"participants\": [\n";
      participants_header =     "";
      participant_template =    "\"%s\", ";
      participants_footer =     "";
      comment_footer =          "]},\n\n";
      footer =                  "]}\n\n";
   }

   /* Header */
   str_make_safe(&chatty->story_name, format);

   size_t allocsize = strlen(header_template) + strlen(chatty->story_name) + 64;
   buffer = (char*)malloc(allocsize);
   sprintf_s(buffer, allocsize, header_template, chatty->story_id, chatty->story_name,
             chatty->last_page, chatty->page);
   append(&xml, &xml_size, buffer);
   free(buffer);

   /* Root posts */
   for (i = 0; i < chatty->posts->count; i++)
   {
      root_post_t* post = (root_post_t*)chatty->posts->items[i];

      str_make_safe(&post->preview, format);
      str_make_safe(&post->body, format);
      str_make_safe(&post->author, format);

      buffer = (char*)malloc(strlen(comment_header_template) + strlen(post->author) +
                             strlen(post->date) + strlen(post->preview) + 
                             strlen(post->body) + strlen(post->category) + 64);
      sprintf_s(buffer, allocsize, comment_header_template, post->last_reply_id, post->author,
              post->date, post->preview, post->reply_count, post->id, 
              post->category, post->body);
      append(&xml, &xml_size, buffer);
      free(buffer);

      append(&xml, &xml_size, participants_header);

      for (j = 0; j < post->participants->count; j++)
      {
         str_make_safe(&post->participants->items[j], format);

         buffer = (char*)malloc(strlen(participant_template) + 
                                strlen(post->participants->items[j]) + 1);
         sprintf_s(buffer, allocsize, participant_template, post->participants->items[j]);
         append(&xml, &xml_size, buffer);
         free(buffer);
      }

      append(&xml, &xml_size, participants_footer);
      append(&xml, &xml_size, comment_footer);
   }

   /* Footer */
   append(&xml, &xml_size, footer);

   *out = xml;
   return ERR_OK;
}

/** chatty_new ****************************************************************
 *  PARAMETERS: None.
 *  RETURNS:    New chatty_t structure.
 *  PURPOSE:    Allocates and initializes a new chatty_t structure.
 *****************************************************************************/
chatty_t* chatty_new(void)
{
   chatty_t* chatty = (chatty_t*)malloc(sizeof(chatty_t));
   memset(chatty, 0, sizeof(chatty_t));
   chatty->posts = array_new();
   return chatty;
}

/** chatty_free ***************************************************************
 *  PARAMETERS: chatty - [in] Chatty to deallocate.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Frees the memory associated with 'chatty'.
 *****************************************************************************/
void chatty_free(chatty_t* chatty)
{
   unsigned int i;

   free(chatty->story_name);

   for (i = 0; i < chatty->posts->count; i++)
      root_post_free((root_post_t*)chatty->posts->items[i]);

   array_free(chatty->posts);
   free(chatty);
}

/** root_post_new *************************************************************
 *  PARAMETERS: None.
 *  RETURNS:    New root_post_t structure.
 *  PURPOSE:    Allocates and initializes a new root_post_t structure.
 *****************************************************************************/
root_post_t* root_post_new(void)
{
   root_post_t* post = (root_post_t*)malloc(sizeof(root_post_t));
   memset(post, 0, sizeof(root_post_t));
   post->participants = set_new();
   return post;
}

/** root_post_free ************************************************************
 *  PARAMETERS: root_post - [in] The post structure to deallocate.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Frees the memory associated with 'root_post'.
 *****************************************************************************/
void root_post_free(root_post_t* root_post)
{
   free(root_post->author);
   free(root_post->date);
   free(root_post->preview);
   free(root_post->body);
   free(root_post->category);
   set_free(root_post->participants);
   free(root_post);
}
