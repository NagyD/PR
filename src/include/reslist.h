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
reslist.h: Princed Resources : Resource list prototypes
¯¯¯¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: 05 Jun 2005

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2005-Jun-05)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_RESLIST_H_
#define _PR_RESLIST_H_

#include "list.h" /* tList */
typedef tList tResourceList;

#include "binary.h"

#include "common.h" /* Item Types */

/* types */
typedef struct { 
	tResourceType type;
	void* obj;
}tObject;

typedef struct {
	signed short       value;
	char               index[5];
	unsigned int       order;
}tResourceId;

typedef struct {
	tResourceId        id;
	tResourceId        paletteId;
	tObject            palette;
	int                paletteoffset;
	/*long int           size;
	unsigned char*     data;*/
	tBinary            content;
	unsigned long int  offset; /* Used internally in dat.c to remember the offset */
	unsigned char      number; /* Used for level number */
	tResourceType      type;
	char*              desc;
	char*              name;
	char*              path;
	const char*        datfile;
	const char*        datAuthor;
	unsigned long      flags;
}tResource;

#ifdef DEBUG_RESPRINT
void printr(const tResource* record); /* for debug purposes */
void resourceListDebugPrint(tResourceList* r);
#endif

#define resourceListStartIteration(l) list_firstCursor(l)
#define resourceListDrop(r)	list_drop(r)

int resCmp(const void* a,const void* b);
int resourceListCompareId(tResourceId a,tResourceId b);
const tResource* resourceListGetElement(tResourceList* r);
tResourceList resourceListCreate(int isCopy);
void resourceListAddInfo(tResourceList* r,tResource* res);
void resourceListAdd(tResourceList* r,tResource* res); /* only increases order */
void resourceListDebugPrint(tResourceList* r);
void resourceListRebuildForIndex(tResourceList* r);

#endif
