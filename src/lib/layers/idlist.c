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
idlist.c: Princed Resources : Matching Id's list routines
¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

/* Includes */
#include "disk.h"
#include "idlist.h"
#include "memory.h"
#include "resourcematch.h"
#include <stdio.h>
#include <string.h> /* strcat strlen */

/* Id list for partial manipulation. Private type */
typedef struct {
	int             count;
	tResourceMatch* list;
}tResIdList;

static tResIdList itemMatchingList;

int itemMatchingListActive() {
	return itemMatchingList.count;
}

/***************************************************************\
|                Partial Resource List Functions                |
\***************************************************************/

int parseGivenPath(char* path) {
	/*
	 * format: \([^@]*\)/(@\([0-9]\+\)?\(:[a-z ]*\)?\(#[a-z0-9]\+\)\)?
	 * where: \1 is the DAT filename, \2 is the partial list whole match if exists,
	 *        \3 is the resource id, \4 is the index name and \5 is the order
	 * examples:                         rID Ind  Ord  Restricted to
	 *  datfile.dat@111:shape#first  --> 111 shap 0    *
	 *  datfile.dat@111:shape        --> 111 shap *    *
	 *  datfile.dat@111#first        --> 111 *    0    *
	 *  datfile.dat@#first           --> *   *    0    *
	 *  datfile.dat@:shape           --> *   shap *    *
	 *  datfile.dat@111              --> 111 *    *    *
	 *  datfile.dat@#last            --> *   *    last *
	 *  datfile.dat@#second          --> *   *    1    *
	 *  datfile.dat@:shap            --> *   shap *    *
	 *  datfile.dat@:sh*             --> *   sh*  *    *
	 *  datfile.dat@:tga palette     --> *   palt *    *
	 *  datfile.dat@#785             --> *   *    785  *
	 *  datfile.dat@/a.bmp           --> *   *    *    a.bmp
	 *  datfile.dat@/img*.bmp        --> *   *    *    img*.bmp
	 *  datfile.dat@!11              --> ¬11 *    *    *
	 *  datfile.dat@#!first          --> *   *    >=1  *
	 *  datfile.dat@!/img*.bmp       --> *   *    *    not img*.bmp
	 *  datfile.dat@:!sh*            --> *   ¬sh* *    *
	 *  datfile.dat@0,1,2,3          --> <=3 *    *    *
	 *
	 * PRE:
	 *  itemMatchingList.list was not allocated
	 * POST:
	 *  itemMatchingList.count=0 and itemMatchingList.list=NULL if all resources
	 *  path was trimmed in the "@"
	 */

	int i;
	int separator=0;
	int j=0;
	int size;
	int error=0;

	/* Check if the variable wasn't initialized before */
	if (itemMatchingList.count!=0) return PR_RESULT_ERR_WRONG_PRIMITIVE_CALL;
	itemMatchingList.list=NULL;

	/* Validates the NULL path */
	if (path==NULL) return PR_RESULT_SUCCESS;

	/* Erase the last "/" if exists. */
	if (path) {
		size=strlen(path);
		if (size>0) {
			size--;
			if (isDirSep(path,size)) path[size]=0;
		}
	}

	/* Locate the string separation */
	while (path[separator]&&path[separator]!='@') separator++;

	/* If no separation */
	if (!path[separator]) return PR_RESULT_SUCCESS; /* There was no separator */

	/* Count values, separate them with '\0' and allocate memory */
	itemMatchingList.count=1;
	path[separator]=0; /* Trim the path to the separator */
	i=++separator;
	while(path[i]) {
		if (path[i]==',') {
			itemMatchingList.count++;
			path[i]=0;
		}
		i++;
	}
	itemMatchingList.list=(tResourceMatch*)malloc(sizeof(tResourceMatch)*itemMatchingList.count);

	/* Parse values and save them in the list */
	for(i=separator;(!error)&&(j!=itemMatchingList.count);i++) {
		error=initRM(path+i,itemMatchingList.list+j);
		while (path[i]) i++;
		j++;
	}
	if (error) {
		for (i=0;i<j-1;i++) freeRM(itemMatchingList.list+i);
		return PR_RESULT_ERR_COMMAND_LINE_SYNTAX;
	}
	return PR_RESULT_SUCCESS;
}

int isInTheItemMatchingList(const char* vFile, tResourceId id) {
	/*
	 * Cases:
	 * "path/path@"                     all files are false
	 * "path/path"                      all files are true
	 * "path/path@12file/jjj.bmp,777"   only file "12file/jjj.bmp" and id 777 are true
	 * "path/path@1,2,3"                only ids 1, 2 and 3 are true
	 * "path/path@12file/?mage1*.bmp"   each file matching "12file/?mage1*.bmp" is true
	 */
	int i;

	if (!itemMatchingList.count) return 1; /* true */

	for (i=0;i<itemMatchingList.count;i++)
		if (runRM(itemMatchingList.list+i,repairFolders(vFile?vFile:""),&id)) return 1; /* true */

	return 0; /* false */
}

void freeItemMatchingList() {
	void* aux=itemMatchingList.list;
	if (itemMatchingList.list) {
		while (itemMatchingList.count--) freeRM(itemMatchingList.list++);
		free(aux);
	}
	itemMatchingList.list=NULL;
}

