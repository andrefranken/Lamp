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
 *  STACK.C
 *  A basic LIFO stack that holds pointers.
 *  
 *****************************************************************************/
#include "stdafx.h"
#include <memory.h>
#include "platform.h"
#include "stack.h"

/** stack_new *****************************************************************
 *  PARAMETERS: None.
 *  RETURNS:    New stack_t object.
 *  PURPOSE:    Creates a new stack_t.
 *****************************************************************************/
stack_t* stack_new(void)
{
   stack_t* stack = (stack_t*)malloc(sizeof(stack_t));
   memset(stack, 0, sizeof(stack_t));
   return stack;
}

/** stack_push ****************************************************************
 *  PARAMETERS: stack - [in] The stack to push onto.
 *              item  - [in] The item to push.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Pushes an item onto the stack.
 *****************************************************************************/
void stack_push(stack_t* stack, void* item)
{
   stack->count++;
   stack->items = (void**)realloc(stack->items, stack->count * sizeof(void*));
   stack->items[stack->count - 1] = item;
}

/** stack_top *****************************************************************
 *  PARAMETERS: stack - [in] The stack to inspect.
 *  RETURNS:    Item pointer, or NULL.
 *  PURPOSE:    Gets the item at the top of the stack.
 *****************************************************************************/
void* stack_top(stack_t* stack)
{
   if (stack->count == 0)
      return NULL;
   
   return stack->items[stack->count - 1];
}

/** stack_pop *****************************************************************
 *  PARAMETERS: stack - [in] The stack to pop from.
 *  RETURNS:    Error code.
 *  PURPOSE:    Tops the item at the top of the stack.
 *****************************************************************************/
chattyerror stack_pop(stack_t* stack)
{
   if (stack->count == 0)
      return ERR_STACK_EMPTY;
   
   stack->count--;
   stack->items[stack->count] = NULL;
   return ERR_OK;
}

/** stack_free ****************************************************************
 *  PARAMETERS: stack - [in] The stack to free.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Frees the memory associated with "stack".  Does not free the
 *              individual items in the stack.
 *****************************************************************************/
void stack_free(stack_t* stack)
{
   free(stack->items);
   free(stack);
}
