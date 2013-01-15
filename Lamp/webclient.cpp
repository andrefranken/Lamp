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
 *  WEBCLIENT.C
 *  A minimal web client for downloading the Shack chatty.
 *  
 *****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "mydownlib.h"
#include "platform.h"
#include "webclient.h"

extern CRITICAL_SECTION g_ThreadAccess;

extern bool g_bSingleThreadStyle;

char g_useragent[256];

void SetUserAgent(const char *str)
{
   if(!g_bSingleThreadStyle)
   {
      ::EnterCriticalSection(&g_ThreadAccess);
   }

   strcpy_s(g_useragent, 256, str);

   if(!g_bSingleThreadStyle)
   {
      ::LeaveCriticalSection(&g_ThreadAccess);
   }
}

/** webclient_download *********************************************************
 *  PARAMETERS: host - [in] Hostname.
 *              path - [in] Path starting with /
 *              out_response - [out] The contents of the page http://host/path
 *  RETURNS:    Error code.
 *  PURPOSE:    Downloads a web page.
 *****************************************************************************/
chattyerror webclient_download(const char* host, 
                               const char* path, 
                               const char* username, 
                               const char* password, 
                               char** out_response, 
                               bool secure,
                               int *psize/* = NULL*/)
{
   static const char* url_template = "http://%s%s";
   static const char* url_template_s = "https://%s%s";
   unsigned int   size;
   mydown_options opt;
   char           *url;

   memset(&opt, 0, sizeof(mydown_options));

   opt.user     = (uint8_t*)username;
   opt.pass     = (uint8_t*)password;
   opt.useragent = (uint8_t*)g_useragent;
   opt.from     = 0;//-1;
   opt.tot      = 0;//-1;
   opt.verbose  = 0;//-1;
   opt.filedata = (unsigned char**) out_response;
   opt.get      = (uint8_t*)"GET";

   size_t allocsize;
   if(secure)
   {
      allocsize = strlen(url_template_s) + strlen(host) + strlen(path) + 1;
   }
   else
   {
      allocsize = strlen(url_template) + strlen(host) + strlen(path) + 1;  
   }

   url = (char*)malloc(allocsize);

   if(secure)
   {
      sprintf_s(url, allocsize, url_template_s, host, path);
   }
   else
   {
      sprintf_s(url, allocsize, url_template, host, path);
   }

   size = mydown((uint8_t*)url, NULL, &opt);

   if(size == (unsigned int)-1 &&
      *out_response == NULL)
   {
      size = 0;
   }

   if(psize != NULL)
   {
      *psize = size;
   }

   free(url);

   if (size > 0)
      return ERR_OK;
   else
      return ERR_CHATTY_DOWNLOAD;
}


chattyerror webclient_post(const char* host, 
                           const char* path, 
                           const char* post_data, 
                           const char* username, 
                           const char* password, 
                           char** out_response, 
                           int *psize/* = NULL*/)
{
   static const char* url_template = "http://%s%s";
   unsigned int   size;
   mydown_options opt;
   char           *url;

   memset(&opt, 0, sizeof(mydown_options));

   opt.user     = (uint8_t*)username;
   opt.pass     = (uint8_t*)password;
   opt.useragent = (uint8_t*)g_useragent;
   opt.from     = 0;//-1;
   opt.tot      = 0;//-1;
   opt.verbose  = 0;//-1;
   opt.filedata = (unsigned char**) out_response;
   opt.get      = (uint8_t*)"POST";
   opt.content  = (uint8_t*)post_data;
   opt.contentsize = strlen(post_data);

   size_t allocsize = strlen(url_template) + strlen(host) + strlen(path) + 1;

   url = (char*)malloc(allocsize);
   sprintf_s(url, allocsize, url_template, host, path);

   size = mydown((uint8_t*)url, NULL, &opt);

   if(size == (unsigned int)-1 &&
      *out_response == NULL)
   {
      size = 0;
   }

   if(psize != NULL)
   {
      *psize = size;
   }

   free(url);

   if (size > 0)
      return ERR_OK;
   else
      return ERR_CHATTY_DOWNLOAD;
}
