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
unknown.c: Princed Resources : Unknown resources handler implementation
¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)
  Version: 1.50 (2005-Dec-19)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

/* Includes */
#include "common.h"
#include "disk.h"
#include "memory.h"
#include "parse.h" /* getTagStructure */
#include "stringformat.h"
#include "translate.h"
#include "tree.h"
#include "unknown.h"
#include <stdio.h>
#include <string.h>

/***************************************************************\
|                     Unknown.xml primitives                    |
\***************************************************************/

/* XML generation defines */

#define RES_XML_UNKNOWN_FILES "%t%03n.%e"
#define XML_TAG_RESOURCES     "resources"
#define XML_ATTRV_VERSION     "build-1"

char* unknown_getVersion(const char* oldVersion) {
	char* newVersion;
	int l,i;

	if (!oldVersion||!oldVersion[0]) return strallocandcopy(XML_ATTRV_VERSION);
	l=strlen(oldVersion);
	newVersion=malloc(l+5); /* l+2 is enough */
	for (i=0;i<l&&isNumber(oldVersion[l-i-1]);i++);

	if (l!=i) strncpy(newVersion,oldVersion,l-i);
	if (i)    sprintf(newVersion+(l-i),"%d",atoi(oldVersion+(l-i))+1);
	else newVersion[l]=0;

	return newVersion;
}

tUnknownFile unknownFile;

/***************************************************************\
|                           Logging Layer                       |
\***************************************************************/

int unknownLogStart (const char* file,int optionflag, const char* backupExtension) {
	if (unknownFile.fd) return PR_RESULT_ERR_XML_ALREADY_OPEN; /* File already open */

	/* Use default filename if file is NULL */
	if (!file) file=RES_XML_UNKNOWN_XML;

	/* Remember optionflag and backupExtension */
	unknownFile.optionflag=optionflag;
	unknownFile.backupExtension=strallocandcopy(backupExtension);
	unknownFile.currentDat=NULL;

	/* Read the previous file if exists */
	{
		int error;

		unknownFile.status.folderFirst=NULL;
		unknownFile.status.folderCursor=NULL;
		unknownFile.tree=xmlParseFile(file,&error);
		if (error) {
			unknownFile.tree=getTagStructure();
			unknownFile.tree->version=strallocandcopy(XML_ATTRV_VERSION);
			unknownFile.tree->tag=strallocandcopy(XML_TAG_RESOURCES);
		} else {
			char* version=unknown_getVersion(unknownFile.tree->version);
			freeAllocation(unknownFile.tree->version);
			unknownFile.tree->version=version;
		}
	}

	/* Open the file */
	if (!writeOpen(file,&unknownFile.fd,optionflag)) return PR_RESULT_ERR_XML_FILE; /* file not open */

	return PR_RESULT_SUCCESS; /* Ok */
}

int unknownLogStop () {
	int i;
	tTag* t;

	if (!unknownFile.fd) return PR_RESULT_ERR_XML_NOT_OPEN; /* File not open */

	/* common factor tree reducing function */
	if (unknownFile.tree) xmlOptimizeCommonFactor(unknownFile.tree->child);

	/* it is time to fix the inheritances */
	resourceTreeFixInheritances(&unknownFile.tree);

	/* now we'll add the new generated part of the tree at the end of the second level (resources id the first) */
	if (unknownFile.tree) {
		xmlOptimizeCommonFactor(unknownFile.tree); /* here some common factors are moved up */
		if (unknownFile.tree->child) {
			for (t=unknownFile.tree->child;t->next;t=t->next);
			t->next=unknownFile.status.folderFirst; /* the first folder of the new tree */
		} else {
			unknownFile.tree->child=unknownFile.status.folderFirst; /* the first folder of the new tree */
		}
	}

	/* generate the XML file */
	treeXmlGenerate(0,unknownFile.tree,unknownFile.fd);

	/* it's time to free the tree */
	freeParsedStructure (&unknownFile.tree);

	/* and close the file */
	writeCloseOk(unknownFile.fd,unknownFile.optionflag,unknownFile.backupExtension);

	/* Free structures */
	freeAllocation(unknownFile.currentDat);
	freeAllocation(unknownFile.backupExtension);
	unknownFile.backupExtension=NULL;
	unknownFile.fd=NULL;
	for (i=0;i<RES_FILE_TYPES_COUNT;i++) unknownFile.typeCount[i]=0; /* re-initialize in 0 for next file processing */

	return PR_RESULT_SUCCESS; /* Ok */
}

int unknownLogAppend(const char* vFiledat,tResourceId id,const char* ext,tResourceType type,const char* vDirExt,tResourceId pal,const char* vFiledatWithPath,int optionflag,int count, unsigned long int flags,const char* filename) {
	if (!unknownFile.fd) return PR_RESULT_ERR_XML_NOT_OPEN; /* File not open, logging is off, just a warning */

	if (!unknownFile.currentDat) { /* this is the beginning of the file */
		treeStatusFolder(vFiledatWithPath,vFiledat,pal.value,translateInt2Ext(strToLower(pal.index)),&unknownFile.status);
		unknownFile.currentDat=strallocandcopy(vFiledat);
		/* it could be moved here the read-parsing-loading and write-opening,
		 * but it is necessary to know the parsing status before the extractions are started */
		treeDeleteFile(vFiledat,unknownFile.tree);
	} else if (!equalsIgnoreCase(unknownFile.currentDat,vFiledat)) {
		int i;
		treeStatusFolder(vFiledatWithPath,vFiledat,pal.value,translateInt2Ext(strToLower(pal.index)),&unknownFile.status);
		freeAllocation(unknownFile.currentDat);
		unknownFile.currentDat=strallocandcopy(vFiledat);
		treeDeleteFile(vFiledat,unknownFile.tree);
		for (i=0;i<RES_FILE_TYPES_COUNT;i++) unknownFile.typeCount[i]=0; /* re-initialize in 0 for next file processing */
	}

	treeStatusItem(id.value,translateInt2Ext(strToLower(id.index)),filename,getExtDesc(type),flags,getExtDesc(type),count,id.order,&unknownFile.status);

	return PR_RESULT_SUCCESS;
}

/***************************************************************\
|                     Middle layer function                     |
\***************************************************************/

void getFileName(char* vFileext,const char* vDirExt,const tResource* r,const char* vFiledat, const char* vDatFileName,int optionflag, const char* backupExtension,const char* format) {
	static const char* extarray[]=RES_FILE_EXTENSIONS;
	const char* filename;
	int pos;

	if (r->path==NULL) {
		pos=((r->type<RES_FILE_TYPES_COUNT)&&(r->type>=0))?r->type:eResTypeBinary;
		unknownFile.typeCount[pos]++;

		/* set filename */
		if (!format) format=RES_XML_UNKNOWN_FILES;
		filename=parseformat(format,r->id.value,r->id.index,getExtDesc(pos),extarray[pos],unknownFile.typeCount[pos],r->id.order,r->desc,r->name);

		sprintf(vFileext,"%s/"RES_XML_UNKNOWN_PATH"/%s/%s",vDirExt,vDatFileName,filename);
		unknownLogAppend(vDatFileName,r->id,extarray[pos],r->type,vDirExt,r->paletteId,vFiledat,optionflag,unknownFile.typeCount[pos],r->flags,filename);
	} else {
		/* set filename */
		sprintf(vFileext,"%s/%s",vDirExt,r->path);
	}
}

