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
 *  UTILITY.H
 *  General utility functions.
 *  
 *****************************************************************************/

#pragma once

typedef enum 
{
   XML,
   JSON
}response_format;


void append        (char**            string, 
                    size_t*           size, 
                    const char*       appendix);
                    
void trim          (char*             string);

void strip_html    (char*             string);

void str_make_safe (char**            string, 
                    response_format  format);
                    
void str_replace   (char**            string, 
                    const char*       needles, 
                    const char**      replacements);

void fix_spoilers  (char*             string);

void program_exit  (void);
