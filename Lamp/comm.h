/******************************************************************************
 *
 *  Latest Chatty Command Line Client
 *  By Electroly.
 *
 *  COMM.H
 *
 *  Network communications.  Allows GET and POST requests to a web server.
 *  
 *****************************************************************************/

#pragma once

#include "constant.h"

error_t comm_init     (const char* host);
error_t comm_download (const char* host, const char* path, 
                       char** buffer, unsigned int* size,
                       const char* post_data);
void    comm_cleanup  (void);

