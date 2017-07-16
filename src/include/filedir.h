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
filedir.h: Princed Resources : File list layer for console IDE prototypes
¯¯¯¯¯¯¯¯¯
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

#ifndef _PR_FILEDIR_H_
#define _PR_FILEDIR_H_

/* types */
typedef struct tFileDir {
	char* file;
	struct tFileDir* next;
}tFileDir;

typedef struct {
	tFileDir filenames;
	tFileDir options;
}tFileDir2;

/* prototypes */

void  fileDirClearOptions(tFileDir2* list1);
void  fileDirAddOption(tFileDir2* list1, const char* option);
int   fileDirGetFiles(tFileDir2* list1,tFileDir2* files,int hasExportFlag,int notHasRecursiveFlag,const char* resfile,int givenDatFilename);
char* fileDirGetFile(tFileDir2* files,char** datfile);

#endif

