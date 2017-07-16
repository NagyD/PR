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
tree.h: Princed Resources : Special XML tree prototypes
¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_TREE_H_
#define _PR_TREE_H_

/* Includes */
#include "reslist.h"
#include <stdio.h> /* FILE* */

typedef struct {
	tTag*        folderCursor;
	tTag*        folderFirst;
	tTag*        itemCursor;
} tTreeStatus;

void treeStatusItem  (int value,const char* index,const char* path,const char* type,unsigned long int flags,const char* typedesc,int count, int order, tTreeStatus* status);
void treeStatusFolder(const char* path, const char* file, int palette, const char* paletteindex, tTreeStatus* status);
void treeDeleteFile  (const char* file, tTag* tree);
void treeXmlGenerate (int n,const tTag* t,FILE* outputStream);

#endif
