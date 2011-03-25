/******************************************************************************
 *
 *  Latest Chatty Command Line Client
 *  By Electroly.
 *
 *  HTML.H
 *
 *  Decodes posts containing HTML entities, and encodes HTTP POST data into
 *  URL-safe form.
 *  
 *****************************************************************************/

#pragma once
#include "constant.h"

char* html_entity_decode(char* string);
char* url_encode        (const char* string);

