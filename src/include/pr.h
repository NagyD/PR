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
pr.h: Princed Resources : Main library header prototypes and definitions
¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2003-Aug-24)

  Modified by: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.10 (2003-Dec-03)

  Modified by: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.20 (2005-Mar-25)

  Language: Abstract

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_PR_H_
#define _PR_PR_H_

/* types */
typedef struct tTag {
	struct tTag* child;
	struct tTag* next;
	char* tag;
	char* desc;
	char* path;
	char* file;
	char* type;
	char* name;
	char* palette;
	char* paletteindex;
	char* paletteorder;
	char* paletteoffset;
	char* value;
	char* index;
	char* order;
	char* version;
	char* number;
	char* flags;
	char* colors;
}tTag;

/***************************************************************\
|                           Prototypes                          |
\***************************************************************/
/* The library EXPORTS are:
 * freeParsedStructure
 * freeXmlCache
 * freeTagStructure
 * xmlParseStructure
 * xmlParseFile
 * prClassifyDat
 * prExportDat
 * prExportDatOpt
 * prImportDat
 * prImportDatOpt
 * prSetOutput
 * resourceTreeGetChild
 * resourceTreeGetInfo
 * resourceTreeGetNext
 * setCompressionLevel
 */

/* Main functions */
int   prExportDat(const char* vDatFile, const char* vDirName, const char* vResFile);
int   prImportDat(const char* vDatFile, const char* vDirName, const char* vResFile);
int   prClassifyDat(const char* vFiledat);

/* Extra featured functions */
int   prExportDatOpt(const char* vDatFile, const char* vDirName,
      const char* vResFile,int opt,const char * vDatFileName,
      const char* datAuthor, const char* backupExtension, const char* format);
int   prImportDatOpt(const char* vDatFile, const char* vDirName,
      const char* vResFile,int opt,const char* vDatFileName,
      const char* backupExtension);

/* Option functions */
#include <stdio.h> /* FILE* */
void  prSetOutput         (FILE* output);
void  setCompressionLevel (int cl);

/* XML parsing functions */
void  freeParsedStructure (tTag** structure);
void  freeTagStructure    (tTag* structure);
void  freeXmlCache        ();
void  resourceTreeFixInheritances(tTag* *tree);
tTag* resourceTreeGetChild(tTag* whereAmI);
int   resourceTreeGetInfo (tTag* whereAmI, char** tag, char** desc, char** path, char** file, char** type, char** name, char** palette, char** value, char** version, char** number,char** colors);
tTag* resourceTreeGetNext (tTag* whereAmI);
void  xmlOptimizeCommonFactor    (tTag* tag);
int   xmlGenerateFile     (const char* vFile,const tTag* t);
tTag* xmlParseFile        (const char* vFile,int* error);
int   xmlParseStructure   (const char* vFile, tTag** structure);

/* unknown.xml logging */

int   unknownLogStart     (const char* file,int optionflag, const char* backupExtension);
int   unknownLogStop      ();

#endif

