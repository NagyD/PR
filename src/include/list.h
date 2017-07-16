/*  Princed V3 - Prince of Persia Level Editor for PC Version
    Copyright (C) 2003 Princed Development Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    The authors of this program may be contacted at http://forum.princed.org
*/

/*
list.h: Princed Resources : Ordered Read-Only list headers
¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: 04 Jun 2005

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2005-Jun-04)

 Note:
  DO NOT remove this copyright notice
*/

/* Defines */
#ifndef _PR_LIST_H_
#define _PR_LIST_H_

#define GT 1
#define EQ 0
#define LT -1

typedef struct node {
	void* data;
	struct node* next;
}tListNode;

typedef struct {
	int size;
	int (*cmp)(const void*,const void*);
	void (*free)(void* a);
	tListNode *cursor,*first;
}tList;

tList list_create(int dataSize,int dataCmp(const void*,const void*),void dataFree(void* a));
void  list_insert(tList *list,const void* data);
void  list_drop(tList *list);
void  list_nextCursor(tList* list);
void  list_firstCursor(tList* list);
int   list_moveCursor(tList* list,const void* data);
void* list_getCursor(tList* list);
void  list_reorder(tList* list,int dataCmp(const void*,const void*));

#endif
