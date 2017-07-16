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
plv.c: Princed Resources : PLV prince level files support
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 29 Nov 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2003-Nov-29)

 PLV file format:
  Defined: 28 Nov 2003

  Authors:
   Brendon James <roomshaker@princed.org>
   Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2003-Nov-28)

 Modified by: David from Princed

 Note:
  DO NOT remove this copyright notice
*/

/* Includes */
#include "plv.h"
#include "disk.h"
#include "dat.h"
#include <string.h>
#include <time.h>
#include <stdlib.h> /*malloc(),free()*/

/* Private function to get the current date/time */
char* getDate() {
	/* Code taken from RDR 1.4.1a coded by Enrique Calot */

	/* Declare variables */
#ifdef PLV_FULL_FORMAT
	static char weeks   []   = DATE_WEEKDAYS;
	static char months  []   = DATE_MONTHS;
#endif
	static char formated [37];
	time_t      datet;
	struct tm*  date;

	/* get GMT time from the system clock */
	time(&datet);
	date=gmtime(&datet);

#ifdef PLV_FULL_FORMAT
	/* Format: "Tue, 26 Nov 2002 22:16:39" */
	sprintf(formated,"%s, %d %s %.4d %.2d:%.2d:%.2d",
		weeks+4*(date->tm_wday),
		date->tm_mday,
		months+4*(date->tm_mon),
		date->tm_year+1900,
		date->tm_hour,
		date->tm_min,
		date->tm_sec
	);
#else
	/* Format: "2002-11-26 22:16:39" */
	sprintf(formated,"%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
		date->tm_year+1900,
		date->tm_mon+1,
		date->tm_mday,
		date->tm_hour,
		date->tm_min,
		date->tm_sec
	);
#endif
	return formated;
}

int writePlv(const char* file, tBinary content, int popversion, const char* datfile, int level, const char* filename, const char* desc, const char* title, const char* vDatAuthor, int optionflag,const char* backupExtension) {
	/* Plv files are saved as raw except you must ignore the checksum and add the plv constant file header */

	/* Variables */
	FILE* target;
	int ok;
	unsigned char sizeOfNow;
	char* now;
	unsigned char checksum;
	char levelnum[10];
	const char* nullString="";
	static const char* author=PLV_DEFAULT_AUTHOR;
	unsigned long int block2size;
	const unsigned long int numberOfFieldPairs=9;
	unsigned char version=popversion;

	/* Get current time */
	now=getDate();
	sizeOfNow=(unsigned char)(strlen(now)+1);

	/* Ignore checksum */
	/* size--; */

	/* Validate null strings when no description is set */
	if (desc==NULL) desc=nullString;
	if (title==NULL) title=nullString;
	if (vDatAuthor==NULL) vDatAuthor=author;
	sprintf(levelnum,"%d",level);

	/* Writing file */

	/* Safe open for writing mode */
	ok=writeOpen(file,&target,optionflag);

	/* Write headers */
	ok=ok&&fwrite(PLV_HEADER_A,PLV_HEADER_A_SIZE,1,target);
	/*if (size==12025) version=2; else version=1; * TODO: check if the checksum is included */
	ok=ok&&fwritechar(&version,target);
	version=1;
	ok=ok&&fwritechar(&version,target);
	ok=ok&&fwritechar(&level,target);
	ok=ok&&fwritelong(&numberOfFieldPairs,target);
	ok=ok&&fwritelong(&content.size,target);

	/* Write block 1: raw data without ignoring checksum */
	checksum=getChecksum(content);
	ok=ok&&fwritechar(&checksum,target);
	ok=ok&&fwrite(content.data,content.size,1,target);

	/* Write footers */
	block2size=(
		sizeof(PLV_FOOT_EDITOR)+	     strlen(vDatAuthor)+1+
		sizeof(PLV_FOOT_TITLE)+	       strlen(title)+1+
		sizeof(PLV_FOOT_DESC)+	       strlen(desc)+1+
		sizeof(PLV_FOOT_TCREAT)+	     sizeOfNow+
		sizeof(PLV_FOOT_TMODIF)+	     sizeOfNow+
		sizeof(PLV_FOOT_ORIG_FILE)+	   strlen(filename)+1+
		sizeof(PLV_FOOT_LEV_NUM_ORIG)+ strlen(levelnum)+1
	);

	ok=ok&&fwritelong(&block2size,target);

	/* Write block 2 */
	ok=ok&&fwrite(PLV_FOOT_EDITOR,sizeof(PLV_FOOT_EDITOR),1,target);
	ok=ok&&fwrite(vDatAuthor,strlen(vDatAuthor)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_TITLE,sizeof(PLV_FOOT_TITLE),1,target);
	ok=ok&&fwrite(title,strlen(title)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_DESC,sizeof(PLV_FOOT_DESC),1,target);
	ok=ok&&fwrite(desc,strlen(desc)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_ORIG_FILE,sizeof(PLV_FOOT_ORIG_FILE),1,target);
	ok=ok&&fwrite(filename,strlen(filename)+1,1,target);
	ok=ok&&fwrite(PLV_FOOT_TCREAT,sizeof(PLV_FOOT_TCREAT),1,target);
	ok=ok&&fwrite(now,sizeOfNow,1,target);
	ok=ok&&fwrite(PLV_FOOT_TMODIF,sizeof(PLV_FOOT_TMODIF),1,target);
	ok=ok&&fwrite(now,sizeOfNow,1,target);
	ok=ok&&fwrite(PLV_FOOT_LEV_NUM_ORIG,sizeof(PLV_FOOT_LEV_NUM_ORIG),1,target);
	ok=ok&&fwrite(levelnum,strlen(levelnum)+1,1,target);

	/* Close file and return */
	ok=ok&&(!writeCloseOk(target,optionflag,backupExtension));
	return ok?PR_RESULT_SUCCESS:PR_RESULT_ERR_FILE_NOT_WRITE_ACCESS;
}

extern FILE* outputStream;

	/* fixed by David */
	/* Erases PLV header/footer from res->content in place. */
int readPlv(tResource *res) {
	/* declare variables */
	unsigned char* pos;
/*	unsigned char* posAux;*/
	unsigned long int oldSize=res->content.size;
	unsigned long int newSize;
	unsigned char* leveldata;

	/* integrity check 1 */
	if (oldSize<=PLV_HEADER_A_SIZE+1+PLV_HEADER_B_SIZE){ puts("plv import: oldsize too small 1"); return 0; /* false */ }
	if (memcmp(res->content.data,PLV_HEADER_A,PLV_HEADER_A_SIZE)){ puts("plv import: header wrong"); return 0; /* false */ }

	/* jump to size */
	pos=res->content.data+PLV_HEADER_A_SIZE+7; /* TODO: check this */

	/* read size and jump to data */
	newSize=array2long(pos);pos+=4;

	/* integrity check 2 */
	if (oldSize<=PLV_HEADER_A_SIZE+1+PLV_HEADER_B_SIZE+newSize){ puts("plv import: oldsize too small 2"); return 0; /* false */ }

	/* validate checksum */
	if (!checkSum(pos,newSize+1))
		fprintf(outputStream,PR_TEXT_IMPORT_PLV_WARN);

	pos++; /* skip checksum */

	/* save data */
	/*posAux=res->content.data;*/
	/* Make a copy, otherwise freeing would result in an error, because the pointer would point into the middle of an allocated block. */
	leveldata=malloc(newSize);
	memcpy(leveldata,pos,newSize);
	free(res->content.data);
	res->content.data=leveldata;
	res->content.size=newSize;
	/*mWriteFileInDatFileIgnoreChecksum(res);*/
	/*res->content.data=posAux;*/

	return 1; /* true */
}
