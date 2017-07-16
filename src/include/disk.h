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
disk.h: Princed Resources : Disk Access & File handling prototypes
¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 29 Oct 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2003-Oct-29)

  Modified by: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.10 (2003-Dec-03)
  Modified by: Santiago Zamora <drkzight@users.sourceforge.net>
  Version: 1.20 (2004-Jan-06)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_DISK_H_
#define _PR_DISK_H_

#include <stdio.h>

#define DISK_DIR_SCANING
#define DISK_TERM_MANIPULATION
/* #define DISK_ALLWAYS_FORCE   */

#define MAX_FILENAME_SIZE        260

/* Path defines */
#ifndef WIN32
#define DIR_SEPARATOR           '/'
#else
	#define DIR_SEPARATOR           '\\'
#endif

#include "binary.h"

typedef enum {eFile,eDirectory,eNotFound}whatIs;

#define charToUpper(a) ((a)&0xDF)
#define isDirSep(a,i) ((a[i]=='\\')||(a[i]=='/'))

#define writeCloseOk(a,b,c) writeClose(a,0,b,c)
int  writeData     (const unsigned char* data, int ignoreChars, const char* vFileext, int size, int optionflag,const char* backupExtension);
int  writeOpen     (const char* vFileext, FILE* *fp, int optionflag);
int  writeClose    (FILE* fp,int dontSave,int optionflag,const char* backupExtension);
tBinary  mLoadFileArray(const char* vFile);
int  makebase      (const char* p);
const char* repairFolders(const char* a);
const char* getFileNameFromPath(const char* path);
whatIs isDir(const char *nombre);

#ifdef DISK_DIR_SCANING
int recurseDirectory(const char* path,int recursive, void* pass, void (*function)(const char*,void*));
#endif

/* array2vars*/

#define array2short(a) (((*(a)))|((*((a)+1))<<8))
#define array2long(a)  (((*(a)))|((*((a)+1))<<8)|((*((a)+2))<<16)|((*((a)+3))<<24))

#ifdef MACOS
#define freadshort(var,file)  macfreads ((var),file)
#define freadlong(var,file)   macfreadl ((var),file)
#define fwriteshort(var,file) macfwrites((var),file)
#define fwritelong(var,file)  macfwritel((var),file)
#define fwritechar(var,file)  fwrite((var),1,1,file)
#define freadchar(var,file)   fread ((var),1,1,file)
int macfreads (void* bigEndian,FILE* file);
int macfreadl (void* var,FILE* file);
int macfwrites(const void* var,FILE* file);
int macfwritel(const void* var,FILE* file);
#else
#define freadshort(var,file)  fread ((var),2,1,file)
#define freadlong(var,file)   fread ((var),4,1,file)
#define fwriteshort(var,file) fwrite((var),2,1,file)
#define fwritelong(var,file)  fwrite((var),4,1,file)
#define fwritechar(var,file)  fwrite((var),1,1,file)
#define freadchar(var,file)   fread ((var),1,1,file)
#endif

#endif

