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
 *  STACK.H
 *  A basic LIFO stack that holds pointers.  Also masquerades as an array.
 *  
 *****************************************************************************/

#pragma once

#include "chattyerror.h"

#define array_new    stack_new
#define array_add    stack_push
#define array_free   stack_free

typedef struct stack stack_t;
typedef struct stack array_t;

struct stack
{
   void**       items;
   unsigned int count;
};

stack_t* stack_new  (void);

void     stack_push (stack_t* stack, 
                     void*    item);
                     
void*    stack_top  (stack_t* stack);

chattyerror  stack_pop  (stack_t* stack);

void     stack_free (stack_t* stack);

