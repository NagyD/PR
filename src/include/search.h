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
search.h: Princed Resources : Abstract XML tree searching prototypes
¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 23 Oct 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-25)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#ifndef _PR_SEARCH_H_
#define _PR_SEARCH_H_

/* Includes */
#include "common.h" /* tTag */
#include "reslist.h"

/****************************************************************\
|                   Tag Tree Searching Functions                 |
\****************************************************************/

/* parse XML file */
int xmlParseFileForResource(const char* vFile, const char* datFile, tResourceList *r);

/* Abstract function that runs all the tree and executes "function(tag,pass)" for each tag */
void xmlRunOverTree(const tTag* t,void* pass, void (*function)(const tTag*,void*));

#ifdef COMPILE_WITH_COMPARISION
void xmlCompareFiles(tTag* modified,tTag* original);
#endif

/* File List Functions */
char* getFileFromList();

/* Abstract passing structures */
typedef struct tPassWork {
	const char* datFile;
	tResourceList* rlist;
}tPassWork;

typedef struct tPassCompare {
	const tTag* tag;
}tPassCompare;

#endif

