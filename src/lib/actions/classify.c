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
tasks.c: Princed Resources : File classify routines
¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#include "autodetect.h"
#include "classify.h"
#include "common.h"
#include "dat.h"
#include "disk.h" /* mLoadFileArray */
#include "memory.h"
#include <stdio.h>

/***************************************************************\
|                    Get the type of a DAT file                 |
\***************************************************************/

#define READ_ERROR {mReadCloseDatFile();return PR_RESULT_ERR_INVALID_DAT;}

int prClassifyDat(const char* vFiledat) {
	int                indexNumber;
	tResourceType      type=eResTypeBinary;
	unsigned short int numberOfItems;
	tPopVersion        popVersion;
	tResource          res;
	int                error;

	/* Initialize abstract variables to read this new DAT file */
	if ((error=mReadBeginDatFile(&numberOfItems,vFiledat))) return error;

	popVersion=mReadGetVersion();

	/* main loop */
	for (indexNumber=0;(indexNumber<numberOfItems)&&(type==eResTypeBinary);indexNumber++) {
		int ok;
		ok=mReadFileInDatFile(&res,indexNumber);
		if (ok==PR_RESULT_INDEX_NOT_FOUND) READ_ERROR; /* Read error */
		/*if (ok==PR_RESULT_CHECKSUM_ERROR) fprintf(outputStream,"Warning: Checksum error\n"); Warning TODO: add an output for the checksum warning */
		/*if (res.id.value==0xFFFF) continue;*/ /* Tammo Jan Bug fix */
		type=verifyHeader(res.content);
	}

	mReadCloseDatFile();
	return (popVersion==pop1)?type:(type+10);
}

typedef struct {
	unsigned long int checkSum;
	long size;
}tExeClassification;

int prClassify(const char* fileName) {
	int result;

	/* 1) check if it is a DAT file */
	result=prClassifyDat(fileName);

	if (!result) { /* it's not a DAT file*/
		/*long int fileContent.size;
		unsigned char* fileContent.data;*/
		tBinary fileContent;

		/* let's get it's content and see what it is */
		fileContent=mLoadFileArray(fileName);
		if (fileContent.size<=0) return fileContent.size;

		/* 2) let's compare the size with a .SAV size */
		if (fileContent.size==8) {
			int framesLeft;
			/* check that the frames (seconds/12) are in the range [0*12,60*12) */
			framesLeft=fileContent.data[2]|fileContent.data[3]<<8;
			if (framesLeft<60*12)
				result=30; /* SAV file */
		}

		/* 3) let's compare the size with a .HOF size */
		if (fileContent.size==176) {
			int records;
			/* check that the number of stored records are 6 or less */
			records=fileContent.data[0]|fileContent.data[1]<<8;
			if (records<=6) {
				result=31; /* HOF file */
				while (records) {
					int framesLeft;
					/* wrong seconds left format for this record will invalidate the whole file */
					framesLeft=fileContent.data[29*records-2]|fileContent.data[29*records-1]<<8;
					if (framesLeft>=60*12) result=0;
					records--;
				}
			}
		}

		/* 4) as the last resource, check if it is an EXE file */
		if (!result && fileContent.size>2 && fileContent.data[0]=='M' && fileContent.data[1]=='Z') {
			static tExeClassification x[]={
				/* install.pdm         : 41 */ {717181985,4233},
				/* prince.exe v1.0 THG : 42 */ {622612442,123335},
				{0,0}
			};
			unsigned long checkSum=0;
			int i;
			result=40; /* generic EXE file */
			/* Now I'll try to recognize some known EXE files */
			/* calculate checksum */
			for (i=0;i<fileContent.size;i++) {
				checkSum+=fileContent.data[i]<<((3-(i%4))*8);
			}
#ifdef DEBUG_GETCHECKSUM
			printf("{%lu,%ld},\n",checkSum,fileContent.size);
#endif

			/* compare checksum*/
			for (i=0;x[i].size;i++)
				if ((x[i].checkSum==checkSum) && (x[i].size==fileContent.size)) {
					result=41+i;
					break;
				}
		}

		free(fileContent.data);
	}

	return result;
}

