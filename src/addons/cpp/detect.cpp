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
pr.c: Main source file for Princed Resources library
¯¯¯¯
	Princed Resources editor
	(c) Copyright 2003-2006 Princed Development Team

	Authors
	 Coding & main routines
	  Enrique Calot
	  Santiago Zamora

	 Graphic compression algorithms
	  Tammo Jan Dijkema
	  Enrique Calot

	 Graphic format development
	  Tammo Jan Dijkema
	  Anke Balderer

	 MID Sound format development
	  Christian Lundheim

	 Resources.xml edition
	  Steven Fayers

 Note:
  DO NOT remove this copyright notice
*/

/* Headers */
#include <stdio.h>
#include <string.h>

#include "common.h"

#include "classify.h"
#include "export.h"
#include "import.h"

#include "disk.h"      /* getFileNameFromPath */
#include "idlist.h"
#include "memory.h"    /* getMemory, free */
#include "reslist.h"

#include "search.h"    /* parse */
#include "unknown.h"

/***************************************************************\
|                      Main working functions                   |
\***************************************************************/
#if 0
FILE* outputStream=NULL;

void prSetOutput(FILE* output) {
	outputStream=output;
}

int prExportDat(const char* vDatFile, const char* vDirName, const char* vResFile) {
	outputStream=stdout;
	return prExportDatOpt(vDatFile,vDirName,vResFile,export_flag,NULL,NULL,NULL,NULL);
}

int prExportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char* vDatFileName,const char* datAuthor, const char* backupExtension,const char* format) {
	/*
	 * Arguments:
	 * 	char* vDatFile        - full Path to the DAT file
	 * 	char* vDirName        - full Path to the extracting folder
	 * 	                        (doesn't need to exist)
	 * 	char* vResFile        - full Path to the resources XML file
	 * 	                        NULL is the default file resources.xml
	 * 	char opt              - program options, see below
	 * 	char* vDatFileName    - name of the file to be extracted
	 * 	                        NULL means predict it from vDatFile
	 * 	const char* datAuthor - Author's name when extracting PLV's,
	 * 	                        NULL is default
	 * 	const char* backupExtension
	 * 	                      - If backup_flag is set, the string to attach
	 * 	                        to the backup files
	 * 	const char* format
	 * 	                      - Sets the format the unknown files will be created.
	 * 	                        See stringformat.c documentation.
	 *
	 * Options:
	 * 	unknown_flag   - generate the unknown file without performing
	 *                   any extraction
	 * 	raw_flag       - uses raw format
	 * 	verbose_flag   - explain what is being done
	 * 	recursive_flag - searches for all DAT files (only if vDatFile
	 * 	                 is not a DAT file and vDatFileName is NULL)
	 * 	force_flag     - default option, you cannot disable it,
	 * 	                 so please make a backup of your files
	 * 	backup_flag    - backup your files
	 *
	 * Return values:
	 * 	00 Ok
	 * 	-1 Error accessing the file DAT
	 * 	-2 Memory error in extraction
	 * 	-3 Invalid DAT file
	 * 	-4 XML Parse error
	 * 	-5 Memory error in parsing
	 * 	-6 XML Attribute not recognized
	 * 	-7 XML File not found
	 */

	/* Declare variables */
	tResourceList r;
	int a;
	const char* aux;
	char* currentDatFileName;
	char* currentDatFile;

	r=resourceListCreate(0);

	currentDatFile=strallocandcopy(vDatFile);

	if (parseGivenPath(currentDatFile)) return PR_RESULT_ERR_COMMAND_LINE_SYNTAX;

	if (vDatFileName==NULL) { /* if no special DAT file was specified, a path parsed will be used */
		aux=getFileNameFromPath(currentDatFile);
	} else {
		aux=vDatFileName;
	}
	currentDatFileName=strallocandcopy(aux);

	/* Parse XML and export the file */
	a=xmlParseFileForResource(vResFile,currentDatFileName,&r);
	if (a==PR_RESULT_SUCCESS) {
		/* exporting errors/no errors */
		a=extract(currentDatFile,vDirName,&r,opt,currentDatFileName,datAuthor,backupExtension,format);
	}
	resourceListDrop(&r);
	free(currentDatFileName);
	free(currentDatFile);
	freeItemMatchingList();

	return a;
}

int prImportDat(const char* vDatFile, const char* vDirName, const char* vResFile) {
	outputStream=stdout;
	return prImportDatOpt(vDatFile,vDirName,vResFile,0,NULL,NULL);
}

int prImportDatOpt(const char* vDatFile, const char* vDirName, const char* vResFile,int opt,const char* vDatFileName, const char* backupExtension) {
	/*
	 * Arguments:
	 * 	char* vDatFile        - full Path to the DAT file;
	 * 	                        if file use it, if directory, perform it for
	 * 	                        all files
	 * 	char* vDirName        - full Path to the extracting folder
	 * 	                        (doesn't need to exist)
	 * 	char* vResFile        - full Path to the resources XML file
	 * 	                        (resources.xml by default)
	 * 	char opt              - program options, see below
	 * 	char *vDatFileName    - name of the file to be extracted
	 * 	                        NULL means predict it from vDatFile
	 * 	const char* backupExtension
	 * 	                      - If backup_flag is set, the string to attach
	 * 	                        to the backup files
	 *
	 * Options:
	 * 	raw_flag       - uses raw format
	 * 	verbose_flag   - explain what is being done
	 * 	recursive_flag - searches for all DAT files (only if vDatFile
	 * 	                 is not a DAT file and vDatFileName is NULL)
	 * 	force_flag     - If not set and the file exists it will prompt
	 * 	                 for action
	 * 	backup_flag    - backup your files
	 *
	 * Return values:
	 * 	-1 DAT File couldn't be open for writing
	 * 	-2 DAT file not found or invalid in partial importation
	 * 	-3 XML Parse error
	 * 	-4 No memory
	 * 	-5 XML Attribute not recognized
	 * 	-6 XML File not found
	 * 	00 File successfully imported
	 * 	positive number: number of missing files
	 */

	/* Declare variables */
	tResourceList r;
	int a;
	const char* aux;
	char* currentDatFileName;
	char* currentDatFile;

	r=resourceListCreate(0);

	currentDatFile=strallocandcopy(vDatFile);

	if (parseGivenPath(currentDatFile)) return PR_RESULT_ERR_COMMAND_LINE_SYNTAX;

	if (vDatFileName==NULL) { /* if no special DAT file was specified, a path parsed will be used */
		aux=getFileNameFromPath(currentDatFile);
	} else {
		aux=vDatFileName;
	}
	currentDatFileName=strallocandcopy(aux);

	/* Parse XML and import files */
	a=xmlParseFileForResource(vResFile,currentDatFileName,&r);
	if (a==PR_RESULT_SUCCESS) {
		/* importing errors/no errors */
		a=compile (currentDatFile, vDirName,&r,opt,currentDatFileName,backupExtension);
	}
	resourceListDrop(&r);
	free(currentDatFileName);
	free(currentDatFile);
	freeItemMatchingList();
	return a;
}

/* When compiling in UNIX SO libraries */
#ifndef WIN32
void start() {}
#endif
#endif
