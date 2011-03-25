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
 *  THREAD.C
 *  Parses the HTML for a thread and formats it into XML.
 *  
 *****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "chattyerror.h"
#include "stack.h"
#include "thread.h"

/* Private functions. */
static thread_t*      thread_new         (void);
static thread_post_t* thread_post_new    (void);
static void           thread_post_free   (thread_post_t*    thread_post);
static void           thread_post_format (thread_post_t*    thread_post,
                                          response_format   format,
                                          char**            xml,
                                          size_t*           xml_size);

/** thread_parse **************************************************************
 *  PARAMETERS: tree_html    - [in] Downloaded thread structure HTML.
 *              content_html - [in] Downloaded thread content HTML.
 *              out_thread   - [out] The parsed thread structure.
 *  RETURNS:    Error code.
 *  PURPOSE:    Parses the raw HTML into our thread_t structure.
 *****************************************************************************/
chattyerror thread_parse(char* tree_html, char* content_html, thread_t** out_thread)
{
#define SEEK(cursor_var, needle, err)      \
   {                                       \
      cursor_var = strstr(cursor, needle); \
      if (cursor_var == NULL)              \
      {                                    \
         ret_err = err;                    \
         goto parse_error;                 \
      }                                    \
   }

#define READ(dest, start, end)             \
   {                                       \
      dest = (char*)malloc(end - start + 1);      \
      memcpy(dest, start, end - start);    \
      dest[end - start] = 0;               \
   }
   
   thread_t* thread   = NULL;
   stack_t*  stack    = NULL;
   array_t*  posts    = NULL;
   char*     cursor   = NULL;
   char*     c1       = NULL;
   char*     c2       = NULL;
   char*     c3       = NULL;
   char*     temp_str = NULL;
   chattyerror ret_err = ERR_THREAD_PARSE;

   if (tree_html    == NULL || strlen(tree_html)    == 0 ||
       content_html == NULL || strlen(content_html) == 0)
      return ERR_INVALID_ARGUMENT;

   thread = thread_new();
   stack  = stack_new();
   posts  = array_new();
   
   /* Read the post structure. */
   cursor = tree_html;
   while (1)
   {
      thread_post_t* post = NULL;

      /* Post ID */
      cursor = strstr(cursor, "<li id=\"item_");
      if (cursor == NULL)
         break;

      post = thread_post_new();   
      SEEK(cursor, "_", ERR_THREAD_PARSE_POST_ID);
      cursor++;
      post->id = atoi(cursor);
      
      if (thread->last_reply_id < post->id)
         thread->last_reply_id = post->id;

      /* Add to the master list of posts */
      array_add(posts, post);

      /* Moderation flag */
      SEEK(cursor, "<div class=\"oneline", ERR_THREAD_PARSE_CATEGORY1);
      SEEK(cursor, "olmod_",               ERR_THREAD_PARSE_CATEGORY2);
      SEEK(c1,     "_",                    ERR_THREAD_PARSE_CATEGORY3);
      c1++;
      SEEK(c2,     " olauthor_",           ERR_THREAD_PARSE_CATEGORY4);
      READ(post->category, c1, c2);
      
      /* Preview */
      SEEK(cursor, "<span class=\"oneline_body", ERR_THREAD_PARSE_PREVIEW1);
      SEEK(c1,     ">",                          ERR_THREAD_PARSE_PREVIEW2);
      c1++;
      SEEK(c2,     "</span> : ",                 ERR_THREAD_PARSE_PREVIEW3);
      READ(post->preview, c1, c2);
      trim(post->preview);
      
      /* Author */
      SEEK(cursor, "<a href=\"/profile/",      ERR_THREAD_PARSE_AUTHOR1);
      SEEK(c1,     "class=\"oneline_user \">", ERR_THREAD_PARSE_AUTHOR2);
      SEEK(c1,     ">",                        ERR_THREAD_PARSE_AUTHOR3);
      c1++;
      SEEK(c2,     "</a>",                     ERR_THREAD_PARSE_AUTHOR4);
      READ(post->author, c1, c2);
      trim(post->author);
      
      set_add(thread->participants, post->author);
      
      /* Set the root if this is the first post, or add it to the parent
         post if not. */
      if (thread->root == NULL)
         thread->root = post;
      else
      {
         thread_post_t* parent = (thread_post_t*)stack_top(stack);
         if (parent == NULL)
         {
            thread_post_free(post);
            ret_err = ERR_THREAD_PARSE_MALFORMED_TREE;
            goto parse_error;
         }
         else
            array_add(parent->replies, post);
      }
      
      /* Determine if this post has replies, or if it is a leaf. */
      c1 = strstr(cursor, "<li id=\"item_");
      SEEK(c2, "</ul>", ERR_THREAD_PARSE_MISSING_END_UL);
      c3 = strstr(cursor, "<ul>");
      
      /* If c1 comes first, then this post has replies.
         If c2 comes first, then this post is a leaf.  There can be any
         number of </ul>s at this point if this is a deeply-nested leaf.
         Each one pops a post off of the stack. */
      if (c1 == NULL)
      {
         /* This is the last post in the thread. */
         break;
      }
      else if (c3 != NULL && c3 < c1)
      {
         /* This post has replies. */
         stack_push(stack, post);
      }
      else if (c2 < c1)
      {
         /* This post is a leaf.  Pop posts from the stack until we hit another
           <li> for the next post, or the end of the file. */
         cursor = c2;
         while (cursor < c1)
         {
            stack_pop(stack);
            cursor++;
            SEEK(cursor, "</ul>", ERR_THREAD_PARSE_MISSING_END_UL);
         }
      }

      cursor = c1;
   }
   
   /* Read the content. */
   cursor = content_html;
   while (1)
   {
      unsigned int   id   = 0;
      unsigned int   i    = 0;
      thread_post_t* post = NULL;
   
      /* Locate the next post. */
      cursor = strstr(cursor, "<div id=\"item_");
      if (cursor == NULL)
         break;
         
      SEEK(cursor, "_", ERR_THREAD_PARSE_CONTENT_ID);
      cursor++;
      id = atoi(cursor);
      
      /* Look up this post in the list we made earlier. */
      for (i = 0; i < posts->count; i++)
      {
         thread_post_t* post_i = (thread_post_t*)posts->items[i];
         
         if (post_i->id == id)
            post = post_i;            
      }
      
      if (post == NULL)
      {
         ret_err = ERR_THREAD_PARSE_MISMATCHED_DATA;
         goto parse_error;
      }
      
      /* Post body */
      SEEK(cursor, "<div class=\"postbody\">", ERR_THREAD_PARSE_POST_BODY1);
      SEEK(c1,     ">",                        ERR_THREAD_PARSE_POST_BODY2);
      c1++;
      SEEK(c2,     "</div>",                   ERR_THREAD_PARSE_POST_BODY3);
      READ(post->body, c1, c2);
      trim(post->body);
      fix_spoilers(post->body);

      /* Post date */
      SEEK(cursor, "<div class=\"postdate\">", ERR_THREAD_PARSE_POST_DATE1);
      SEEK(c1,     ">",                        ERR_THREAD_PARSE_POST_DATE2);
      c1++;
      SEEK(c2,     "</div>",                   ERR_THREAD_PARSE_POST_DATE3);
      READ(post->date, c1, c2);
      trim(post->date);
   }
   
   thread->reply_count = posts->count;

   stack_free(stack);
   array_free(posts);
   *out_thread = thread;
   return ERR_OK;

parse_error:
   stack_free(stack);
   array_free(posts);
   thread_free(thread);
   return ret_err;
   
#undef SEEK
#undef READ
}

/** thread_format *************************************************************
 *  PARAMETERS: thread  - [in] The parsed thread structure.
 *              format  - [in] XML or JSON.
 *              out     - [out] The generated XML/JSON to return to the client.
 *  RETURNS:    Error code.
 *  PURPOSE:    Converts the thread structure into the XML/JSON format expected
 *              by the remote HTTP client.
 *****************************************************************************/
chattyerror thread_format(thread_t* thread, response_format format, 
                          char** out)
{
   const char*  header_template;
   const char*  comment1_header_template;
   const char*  participants_template;
   const char*  participant_template;
   const char*  participants_end_template;
   const char*  comments_template;
   const char*  comments_end_template;
   char*        xml      = NULL;
   size_t       xml_size = 0;
   char*        buffer   = NULL;
   unsigned int i;
   
   if (format == XML)
   {
      header_template =
         "<comments"
         " page=\"\""
         " story_id=\"\""
         " story_name=\"\""
         " last_page=\"\""
         ">\n";
      comment1_header_template =
         "<comment"
         " preview=\"%s\""
         " category=\"%s\""
         " author=\"%s\""
         " date=\"%s\""
         " reply_count=\"%u\""
         " id=\"%u\""
         " last_reply_id=\"%u\""
         ">\n"
         "<body>%s</body>\n";
      participants_template = 
         "<participants>\n";
      participant_template = 
         "<participant posts=\"%u\">%s</participant>\n";
      participants_end_template =
         "</participants>\n";
      comments_template =
         "<comments>\n";
      comments_end_template =
         "</comments>\n"
         "</comment>\n"
         "</comments>\n";
   }
   else if (format == JSON)
   {
      return ERR_NOT_IMPLEMENTED;
   }
   
   /* Comments header */
   append(&xml, &xml_size, header_template);
   
   /* Root comment header */
   strip_html(thread->root->preview);
   str_make_safe(&thread->root->preview, format);
   str_make_safe(&thread->root->category, format);
   str_make_safe(&thread->root->author, format);
   str_make_safe(&thread->root->date, format);
   str_make_safe(&thread->root->body, format);
   
   size_t allocsize = strlen(comment1_header_template)
                          + strlen(thread->root->preview)
                          + strlen(thread->root->category)
                          + strlen(thread->root->author)
                          + strlen(thread->root->date)
                          + strlen(thread->root->body)
                          + 64;

   buffer = (char*)malloc(allocsize);
   sprintf_s(buffer, allocsize, comment1_header_template,
      thread->root->preview, thread->root->category, thread->root->author, 
      thread->root->date, thread->reply_count, thread->root->id, 
      thread->last_reply_id, thread->root->body);
   append(&xml, &xml_size, buffer);
   free(buffer);
   
   /* Participants */
   append(&xml, &xml_size, participants_template);
   
   for (i = 0; i < thread->participants->count; i++)
   {
      const char* participant = thread->participants->items[i];
   
      size_t allocsize = strlen(participant_template)
                             + strlen(participant)
                             + 16;

      buffer = (char*)malloc(allocsize);
      sprintf_s(buffer, allocsize, participant_template, 1, participant);
      append(&xml, &xml_size, buffer);
      free(buffer);
   }
   
   append(&xml, &xml_size, participants_end_template);
   
   /* Comments */
   append(&xml, &xml_size, comments_template);
   
   for (i = 0; i < thread->root->replies->count; i++)
   {
      thread_post_format((thread_post_t*)thread->root->replies->items[i], 
                         format, &xml, &xml_size);
   }
   
   append(&xml, &xml_size, comments_end_template);

   *out = xml;
   return ERR_OK;
}

/** thread_post_format ********************************************************
 *  PARAMETERS: post     - [in] The post to format.
 *              format   - [in] XML or JSON.
 *              xml      - [ref] The generated XML/JSON to return to the client
 *              xml_size - [ref] Running total of bytes in 'xml'
 *  RETURNS:    Error code.
 *  PURPOSE:    Converts the post into the XML/JSON format expected
 *              by the remote HTTP client.
 *****************************************************************************/
void thread_post_format(thread_post_t* post, response_format format,
                        char** xml, size_t* xml_size)
{
   const char*  comment_header_template;
   const char*  comment_footer_template;
   unsigned int i;
   char*        buffer;

   if (format == XML)
   {
      comment_header_template =
         "<comment"
         " preview=\"%s\""
         " category=\"%s\""
         " author=\"%s\""
         " date=\"%s\""
         " reply_count=\"%u\""
         " id=\"%u\""
         ">\n"
         "<body>%s</body>\n"
         "<comments>\n";
      comment_footer_template =
         "</comments>\n"
         "</comment>\n";
   }
   else if (format == JSON)
   {
   }
   
   if (post == NULL)
      return;
      
   strip_html(post->preview);
   str_make_safe(&post->preview, format);
   str_make_safe(&post->category, format);
   str_make_safe(&post->author, format);
   str_make_safe(&post->date, format);
   str_make_safe(&post->body, format);
   
   size_t allocsize = strlen(comment_header_template) 
                          + strlen(post->preview)
                          + strlen(post->category)
                          + strlen(post->author)
                          + strlen(post->date)
                          + strlen(post->body)
                          + 64;

   buffer = (char*)malloc(allocsize);
   sprintf_s(buffer, allocsize, comment_header_template,
      post->preview, post->category, post->author, post->date, 
      post->replies->count, post->id, post->body);
   append(xml, xml_size, buffer);
   free(buffer);
   
   /* Replies */
   for (i = 0; i < post->replies->count; i++)
   {
      thread_post_t* reply = (thread_post_t*)post->replies->items[i];
      thread_post_format(reply, format, xml, xml_size);
   }
   
   append(xml, xml_size, comment_footer_template);
}

/** thread_new ****************************************************************
 *  PARAMETERS: None.
 *  RETURNS:    New thread_t structure.
 *  PURPOSE:    Allocates and initializes a new thread_t structure.
 *****************************************************************************/
thread_t* thread_new(void)
{
   thread_t* thread = (thread_t*)malloc(sizeof(thread_t));
   memset(thread, 0, sizeof(thread_t));
   thread->participants = set_new();
   return thread;
}

/** thread_free ***************************************************************
 *  PARAMETERS: thread - [in] Chatty to deallocate.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Frees the memory associated with 'thread'.
 *****************************************************************************/
void thread_free(thread_t* thread)
{
   if (thread == NULL)
      return;

   thread_post_free(thread->root);
   set_free(thread->participants);
   free(thread);
}

/** thread_post_new ***********************************************************
 *  PARAMETERS: None.
 *  RETURNS:    New thread_post_t structure.
 *  PURPOSE:    Allocates and initializes a new thread_post_t structure.
 *****************************************************************************/
thread_post_t* thread_post_new(void)
{
   thread_post_t* post = (thread_post_t*)malloc(sizeof(thread_post_t));
   memset(post, 0, sizeof(thread_post_t));
   post->replies = array_new();
   return post;
}

/** thread_post_free **********************************************************
 *  PARAMETERS: root_post - [in] The post structure to deallocate.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Frees the memory associated with 'root_post'.
 *****************************************************************************/
void thread_post_free(thread_post_t* thread_post)
{
   unsigned int i;
   
   if (thread_post == NULL)
      return;

   free(thread_post->author);
   free(thread_post->date);
   free(thread_post->preview);
   free(thread_post->body);
   free(thread_post->category);

   for (i = 0; i < thread_post->replies->count; i++)
      thread_post_free((thread_post_t*)thread_post->replies->items[i]);

   array_free(thread_post->replies);
   free(thread_post);
}
