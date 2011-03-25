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
 *  WEBCLIENT.H
 *  A minimal web client for downloading the Shack chatty.
 *  
 *****************************************************************************/

#pragma once
#include "chattyerror.h"

chattyerror webclient_download(const char* host, 
                               const char* path, 
                               const char* username, 
                               const char* password, 
                               char**      out_response,
                               int*        psize = NULL);

chattyerror webclient_post(const char* host, 
                           const char* path,
                           const char* post_data, 
                           const char* username, 
                           const char* password, 
                           char**      out_response,
                           int*        psize = NULL);

