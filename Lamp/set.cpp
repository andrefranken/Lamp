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
 *  SET.C
 *  A basic set that holds strings.
 *  
 *****************************************************************************/
#include "stdafx.h"
#include "platform.h"
#include "set.h"

/** set_new *******************************************************************
 *  PARAMETERS: None.
 *  RETURNS:    New set instance.
 *  PURPOSE:    Allocates a new set.
 *****************************************************************************/
set_t* set_new(void)
{
   set_t* set = (set_t*)malloc(sizeof(set_t));
   memset(set, 0, sizeof(set_t));
   return set;
}

/** set_add *******************************************************************
 *  PARAMETERS: set  - [in] Set to modify.
 *              item - [in] Item to add to the set.
 *  RETURNS:    Whether the item was added.
 *  PURPOSE:    Adds an item to the set if it is not already present.
 *****************************************************************************/
bool set_add(set_t* set, const char* item)
{
   if (set_contains(set, item))
      return false;
   else
   {
      set->count++;
      set->items = (char**)realloc(set->items, set->count * sizeof(void*));
      set->items[set->count - 1] = _strdup(item);
      return true;
   }
}

/** set_contains **************************************************************
 *  PARAMETERS: set  - [in] Set to check.
 *              item - [in] Item to look for.
 *  RETURNS:    Whether the item exists in the set.
 *  PURPOSE:    Check if the specified item exists in the set.
 *****************************************************************************/
bool set_contains(set_t* set, const char* item)
{
   unsigned int i;
   
   for (i = 0; i < set->count; i++)
   {
      if (strcmp(set->items[i], item) == 0)
         return true;
   }
   
   return false;
}

/** set_free ******************************************************************
 *  PARAMETERS: set - [in] Set to free.
 *  RETURNS:    Nothing.
 *  PURPOSE:    Deallocates the set.
 *****************************************************************************/
void set_free(set_t* set)
{
   unsigned int i;
   
   for (i = 0; i < set->count; i++)
      free(set->items[i]);

   free(set->items);
   free(set);
}
