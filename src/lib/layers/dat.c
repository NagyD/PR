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
dat.c: Princed Resources : DAT format library implementation
¯¯¯¯¯
 Copyright 2004,2005 Princed Development Team
  Created: 15 Mar 2004

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2004-Mar-15)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "dat.h"
#include "disk.h"

#include "reslist.h"

/***************************************************************\
|                    Private layer structures                   |
\***************************************************************/

typedef struct {
	tPopVersion         popVersion;
	unsigned char*      highData;
	int                 highDataSize;
	int                 masterItems;
	int                 slaveItems;
	int                 currentMasterItem;
	int                 currentSlaveItem;
	unsigned char*      currentRecord;
	int*                order;
	int*                orderInit;
	char                slaveIndexName[5];
} tIndexCursor;

typedef struct {
	int value;
	int* pointer;
} tValuePointer;

static tBinary        readDatFile;
static tIndexCursor   readIndexCursor;
static const char*    textPop1="pop1";

/***************************************************************\
|                       Checksum handling                       |
\***************************************************************/

int checkSum(const unsigned char* data,int size) {
	unsigned char checksum = 1;

	/* validates the checksum */
	while (size--) checksum+=*(data++);
	return !checksum;
}

/***************************************************************\
|                         Private layer                         |
\***************************************************************/

#define toLower(a) (((unsigned char)'A'<=(unsigned char)(a)&&(unsigned char)(a)<=(unsigned char)'Z')?(a)|0x20:(a))
#define toUpper(a) (((unsigned char)'a'<=(unsigned char)(a)&&(unsigned char)(a)<=(unsigned char)'z')?(a)&0xDF:(a))

/* the cursor get functions */

#define dat_readCursorGetIndexName(r) (r.slaveIndexName)
#define dat_readCursorGetOrder(r)     (r.popVersion==pop2?(*(r.order)):0)
#define dat_readCursorGetId(r)        (array2short(r.currentRecord))
#define dat_readCursorGetOffset(r)    (array2long(r.currentRecord+2))
#define dat_readCursorGetSize(r)      (array2short(r.currentRecord+6))
#define dat_readCursorGetFlags(r)     ((r.popVersion==pop1)?(1<<24):(r.currentRecord[8]<<16|r.currentRecord[9]<<8|r.currentRecord[10]))
#define dat_readCursorGetVersion(r)   (r.popVersion)

/* TODO: send some functions to memory.c/h */

#define shift(a,b,c,d) a=b;b=c;c=d;d=0

/* TODO: use common factor here under the pre: from is [4] and to is [5] */
void dat_datIndexToHuman(char* to, const char* from) {
	register int i;

	/* copy inverted */
	to[0]=from[3];
	to[1]=from[2];
	to[2]=from[1];
	to[3]=from[0];
	to[4]=0;

	/* shift */
	if (to[0]||to[1]||to[2]||to[3])
		while (!to[0]) {
			shift(to[0],to[1],to[2],to[3]);
		}

	/* to lower */
	for (i=0;i<4;i++)
		to[i]=toLower(to[i]);

}

void dat_humanToDatIndex(char* to, const char* from2) {
	register int i,nullReached=0;
	char from[4];

	/* first of all: we don't trust in null terminated strings */
	for (i=0;i<4;i++) {
		if (!from2[i]) nullReached=1;
		from[i]=nullReached?0:from2[i];
	}

	/* copy inverted */
	to[0]=from[3];
	to[1]=from[2];
	to[2]=from[1];
	to[3]=from[0];
	to[5]=0;

	/* shift */
	if (to[0]||to[1]||to[2]||to[3])
		while (!to[0]) {
			shift(to[0],to[1],to[2],to[3]);
		}

	/* to lower */
	for (i=0;i<4;i++)
		to[i]=toUpper(to[i]);
}

/* the cursor move functions */

int dat_cursorNextIndex(tIndexCursor* r) {
	if (r->popVersion==pop1) {
		/* POP1 */
		return 0; /* false */
	} else {
		/* POP2 */
		if (r->currentMasterItem+1==r->masterItems) {
			return 0; /* false: its over */
		}
		r->currentMasterItem++;

		/* remember the new slave index name */
		dat_datIndexToHuman(r->slaveIndexName,(char*)(r->highData+2+6*r->currentMasterItem));

		/* remember the new slave index size */
		r->slaveItems=array2short(r->highData+array2short(r->highData+6+6*r->currentMasterItem));

		/* jump to next index */
		r->currentSlaveItem=0;
		r->currentRecord=r->highData+array2short(r->highData+6+6*r->currentMasterItem)+2;
	}
	if (r->order) r->order++;
	return 2; /* true, but with index change */
}

int dat_cursorNext(tIndexCursor* r) {
	if (r->popVersion==pop1) {
		/* POP1 */
		r->currentSlaveItem++;
		r->currentRecord+=8;
		if (r->currentSlaveItem==r->slaveItems)
			return 0; /* false */
	} else {
		/* POP2 */
		r->currentSlaveItem++;
		r->currentRecord+=11;
		if (r->currentSlaveItem==r->slaveItems)
			return dat_cursorNextIndex(r);
		if (r->order) r->order++;
	}
	return 1; /* true */
}

void dat_cursorFirst(tIndexCursor* r) {
	if (r->popVersion==pop2) {
		/* remember the first slave index name */
		dat_datIndexToHuman(r->slaveIndexName,(char*)(r->highData+2));
		r->currentRecord=r->highData+array2short(r->highData+6)+2;
	} else {
		r->currentRecord=r->highData+2;
	}

	/* jump to the first index */
	r->currentMasterItem=0;
	r->currentSlaveItem=0;
	r->order=r->orderInit;
}

int dat_cursorMoveId(tIndexCursor* r, tResourceId id) {
	dat_cursorFirst(r);
	/* first try to find the index name */

	do {
		if (!strcmp(r->slaveIndexName,id.index)) {
			/* the same index */
			do {
				if (strcmp(r->slaveIndexName,id.index)) return 0; /* false: in case we are passed */
				if (array2short(r->currentRecord)==id.value) return 1; /* true: found! */
/*printf("AT: IDX=%s ID=%d SEARCHING: IDX=%s ID=%d\n",r->slaveIndexName,array2short(r->currentRecord),id.index,id.value);*/
			} while (dat_cursorNext(r));
			return 0; /* false: id not found */
		}
	} while (dat_cursorNextIndex(r));
	return 0; /* false: index name not found */
}

int dat_cursorMove(tIndexCursor* r,int pos) {
	if (r->popVersion==pop1) {
		/* POP1 */
		if (r->slaveItems<=pos) return 0; /* false */
		r->currentSlaveItem=pos;
		r->currentRecord=r->highData+8*pos+2;
		return 1; /* true */
	} else {
		/* POP2 */
		int i,oldPos=pos;
		for (i=0;i<r->masterItems;i++) {
			/* read how many items are in the slave index number i */
			int itemCount=array2short(r->highData+array2short(r->highData+6+6*i));
			if (pos<itemCount) {
				/* Great! we found it */

				/* remember the new slave index name */
				dat_datIndexToHuman(r->slaveIndexName,(char*)(r->highData+2+6*i));

				/* remember the new slave index size */
				r->slaveItems=itemCount;
				r->order=r->orderInit+oldPos;

				/* jump to next index */
				r->currentMasterItem=i;
				r->currentSlaveItem=pos;
				r->currentRecord=r->highData+array2short(r->highData+6+6*r->currentMasterItem)+2+pos*11;
				return 1; /* true */
			}
			pos-=itemCount;
		}
		return 0; /* false: we have read all the master index and we didn't read the "pos" value */
	}
}

/* the detect function */

tPopVersion detectPopVersion(unsigned char* highArea,int highAreaSize,unsigned short int *numberOfItems) {
	const unsigned char* cursor;
	unsigned short numberOfRecords;

	/* create cursor */
	cursor=highArea;

	/* read number of records */
	numberOfRecords=array2short(cursor);cursor+=2;

	/* check pop1: if there are numberOfRecords records sized 8 and 2 bytes for the short numberOfRecords */
	if ((numberOfRecords*8+2)==highAreaSize) {
		*numberOfItems=numberOfRecords;
		return pop1;
	}

	/* check pop2: if there are numberOfRecords records sized 6 and 2 bytes for the short numberOfRecords */
	*numberOfItems=0;
	if ((numberOfRecords*6+2)>=highAreaSize) return none;
	for (;numberOfRecords;numberOfRecords--,cursor+=6) {
		int startOfSection;
		int endOfSection;
		int sizeOfSection;

		/* calculate section size and offset */
		if (numberOfRecords==1) { /* the last section size is calculated using the highAreaSize */
			endOfSection=highAreaSize;
		} else {
			endOfSection=array2short(cursor+10);
		}
		startOfSection=array2short(cursor+4);
		sizeOfSection=endOfSection-startOfSection;

		/* check section integrity */
		if (sizeOfSection<0) return none;

		if ((array2short(highArea+startOfSection)*11+2)!=sizeOfSection) return none;
		*numberOfItems+=array2short(highArea+startOfSection);
	}

	return pop2;
}

/* the cursor create function */

tIndexCursor dat_createCursor(unsigned char* highData,int highDataSize,unsigned short int* numberOfItems) {
	tIndexCursor r;

	/* read the high data to detect pop version and set up the number of items */
	r.popVersion=detectPopVersion(highData,highDataSize,numberOfItems);

	switch (r.popVersion) {
	case pop1:
		r.highData=highData;
		r.highDataSize=highDataSize;
		r.slaveItems=array2short(highData);

		/* remember the first slave index name */
		strcpy(r.slaveIndexName,textPop1);

		/* jump to the first index */
		r.currentSlaveItem=0;
		r.currentRecord=r.highData+2;

		return r;
	case pop2:
		r.highData=highData;
		r.highDataSize=highDataSize;
		r.masterItems=array2short(highData);

		/* remember the first slave index name */
		dat_datIndexToHuman(r.slaveIndexName,(char*)(highData+2));

		/* remember the first slave index size */
		r.slaveItems=array2short(highData+array2short(highData+6));

		/* the first time will ignore the order */
		r.orderInit=NULL;

		/* jump to the first index */
		dat_cursorFirst(&r);

		/* order array initialization  */
		{
		int *list, *listI;
		tValuePointer *vp, *vpI;
		register tValuePointer aux;
		register int i,j;
		int total=(highDataSize-8*r.masterItems)/11;

		vp=vpI=malloc(total*sizeof(tValuePointer));
		list=listI=malloc(total*sizeof(int));

		do {
			vpI->value=dat_readCursorGetOffset(r);
			(vpI++)->pointer=listI++;
		} while (dat_cursorNext(&r));

		/* bubble sort for the moment */
		i=total;
		while (i--)
			for (j=0;j<i;j++)
				if (vp[j].value>vp[j+1].value) {
					aux=vp[j];
					vp[j]=vp[j+1];
					vp[j+1]=aux;
				}

		/* initilize list */
		for (i=0;i<total;i++)
			*(vp[i].pointer)=i;

		/* bind the list to the cursor and release auxiliary memory */
		free(vp);
		r.orderInit=list;
		dat_cursorFirst(&r);

		}

		return r;
	default:
		return r;
	}
}

/* the cursor read function */

int dat_readRes(tResource* res, tIndexCursor indexCursor) {
	/* for each archived file the index is read */
	res->id.value=        dat_readCursorGetId        (indexCursor);
	strncpy(res->id.index,dat_readCursorGetIndexName (indexCursor),5);
	res->id.order=        dat_readCursorGetOrder     (indexCursor);
	res->offset=          dat_readCursorGetOffset    (indexCursor);
	res->content.size=    dat_readCursorGetSize      (indexCursor);
	res->flags=           dat_readCursorGetFlags     (indexCursor);

	if (res->offset + res->content.size + 1 > readDatFile.size) {
		/* The resource ends after the end of the dat file */
		return PR_RESULT_ERR_INVALID_DAT;
	}

	/*res->size++; * add the checksum */

	/*res->content.data=readDatFile.data+res->offset+1;*/ /* ignore the checksum */
	res->content.data=malloc(res->content.size);
	/* Make a copy, otherwise freeing would result in an error, because the pointer would point into the middle of an allocated block. */
	memcpy(res->content.data,readDatFile.data+res->offset+1,res->content.size);

/*printf("reading resource: %d:%4s at %d order=%d\n",res->id.value,res->id.index,res->offset,res->id.order);*/
	return checkSum(readDatFile.data+res->offset,res->content.size+1);
}

void dat_releaseCursor(tIndexCursor* indexCursor) {
	if (0&&indexCursor->popVersion==pop2) free(indexCursor->order); /* TODO: check*/
}

/***************************************************************\
|                          Public Layer                         |
\***************************************************************/

/***************************************************************\
|                     DAT reading primitives                    |
\***************************************************************/

#ifdef PR_DAT_INCLUDE_DATREAD

tPopVersion mReadGetVersion() {
	return readIndexCursor.popVersion;
}

void mReadCloseDatFile() {
	dat_releaseCursor(&readIndexCursor);
	free(readDatFile.data);
}

int mReadBeginDatFile(unsigned short int *numberOfItems,const char* vFiledat){
	/*
		Return Values:
			-1 Wrong Format
			-2 File not found or empty
			0 Ok
	*/
	unsigned long  int indexOffset;
	unsigned short int indexSize;

	/* Open file */
	readDatFile=mLoadFileArray(vFiledat);
	if (readDatFile.size<=0) {
		switch (readDatFile.size) {
		case PR_RESULT_ERR_FILE_NOT_READ_ACCESS:
			return PR_RESULT_ERR_FILE_DAT_NOT_READ_ACCESS;
		case PR_RESULT_ERR_FILE_NOT_OPEN_WASDIR:
			return PR_RESULT_ERR_FILE_DAT_NOT_OPEN_WASDIR;
		case PR_RESULT_ERR_FILE_NOT_OPEN_NOTFOUND:
			return PR_RESULT_ERR_FILE_DAT_NOT_OPEN_NOTFOUND;
		case PR_RESULT_SUCCESS:
			return PR_RESULT_ERR_INVALID_DAT;
		default:
			return readDatFile.size;
		}
	}
	if (readDatFile.size<=6) {
		free(readDatFile.data);
		return PR_RESULT_ERR_INVALID_DAT;
	}

	/* read header  */
	indexOffset=array2long(readDatFile.data);
	indexSize=array2short(readDatFile.data+4);

	/* verify DAT format: the index offset belongs to the file and the file size is the index size plus the index offset */
	if ((indexOffset>readDatFile.size)&&((indexOffset+indexSize)!=readDatFile.size)) {
		free(readDatFile.data);
		return PR_RESULT_ERR_INVALID_DAT; /* this is not a valid prince DAT file */
	}

	/* create cursor */
	readIndexCursor=dat_createCursor(readDatFile.data+indexOffset,indexSize,numberOfItems);

	/* pop version check */
	if (!dat_readCursorGetVersion(readIndexCursor)) return PR_RESULT_ERR_INVALID_DAT;

	return PR_RESULT_SUCCESS;
}

int mReadFileInDatFileId(tResource* res) {
	if (!dat_cursorMoveId(&readIndexCursor,res->id)) return PR_RESULT_INDEX_NOT_FOUND; /* false means index not found */
	return dat_readRes(res,readIndexCursor)?PR_RESULT_SUCCESS:PR_RESULT_CHECKSUM_ERROR; /* depending on the checksum */
}

int mReadFileInDatFile(tResource* res, int k) {
	if (!dat_cursorMove(&readIndexCursor,k)) return PR_RESULT_INDEX_NOT_FOUND; /* false means out of range */
	return dat_readRes(res,readIndexCursor)?PR_RESULT_SUCCESS:PR_RESULT_CHECKSUM_ERROR; /* depending on the checksum */
}

#endif

/***************************************************************\
|                     DAT Writing primitives                    |
\***************************************************************/

#ifdef PR_DAT_INCLUDE_DATWRITE
FILE* writeDatFile;
tResourceList resIndex;

int mWriteBeginDatFile(const char* vFile, int optionflag) {
	/*
	 * Opens safely a DAT file for writing mode and
	 * reserves space for the headers
	 *
	 * Return Values:
	 *  PR_RESULT_SUCCESS                       Ok
	 *  PR_RESULT_ERR_FILE_DAT_NOT_WRITE_ACCESS File couldn't be open
	 *
	 */
	if (writeOpen(vFile,&writeDatFile,optionflag|backup_flag)) {
		short fill=0;
		resIndex=resourceListCreate(1);
		fwriteshort(&fill,writeDatFile); /* Fill the file with 6 starting null bytes */
		fwriteshort(&fill,writeDatFile); /* Fill the file with 6 starting null bytes */
		fwriteshort(&fill,writeDatFile); /* Fill the file with 6 starting null bytes */
		return PR_RESULT_SUCCESS;
	} else {
		return PR_RESULT_ERR_FILE_DAT_NOT_WRITE_ACCESS;
	}
}

void dat_write(const tResource* res,unsigned char checksum) {
	tResource insert;

	/* remember only indexed values */
	insert.id=res->id;
	insert.flags=res->flags;
	insert.content.size=res->content.size;
	insert.offset=((unsigned long)ftell(writeDatFile));

	/* writes the checksum */
	fwritechar(&checksum,writeDatFile);

	/* do the magic */
	fwrite(res->content.data,res->content.size,1,writeDatFile);

	resourceListAdd(&resIndex,&insert);
}

void mWriteFileInDatFileIgnoreChecksum(const tResource* res) {
	tResource aux;

	/* set up virtual resource auxiliary */
	aux=*res;

	/* drop the checksum */
	aux.content.data++;
	aux.content.size--;

	/* write it */
	dat_write(&aux,res->content.data[0]);
}

unsigned char getChecksum(tBinary c) {
	unsigned char  checksum = 0;

	/* calculates the checksum */
	while (c.size--) checksum+=*(c.data++);
	return ~checksum;
}

void mWriteFileInDatFile(const tResource* res) {
	/*
	 * Adds a data resource to a DAT file keeping
	 * abstractly the checksum verifications
	 */

	/* write the resource contents */
	dat_write(res,getChecksum(res->content));
}

void mWriteCloseDatFile(int dontSave,int optionflag, const char* backupExtension) {
	/*
	 * Closes a DAT file filling the index and other structures
	 */
	unsigned short int totalItems=0;
	unsigned short int size2=2;
	unsigned long  int size1=ftell(writeDatFile);
	const tResource*   res;
	char               index[5];

	/* Write index */
	fwriteshort(&totalItems,writeDatFile); /* Junk total items count to reserve 2 bytes */

	resourceListStartIteration(&resIndex);
	res=resourceListGetElement(&resIndex);
	if (res) {
		if (!strncmp(res->id.index,textPop1,4)) { /* POP1 */
			do {
				totalItems++;
				fwriteshort(&(res->id.value),writeDatFile);
				fwritelong(&(res->offset),writeDatFile);
				fwriteshort(&(res->content.size),writeDatFile);
			} while ((res=resourceListGetElement(&resIndex)));
			size2+=totalItems<<3;

		} else { /* POP2 */
			/*unsigned long int flags=res->flags;*/
			int numberOfSlaveItems=0;
			int slaveCountPos=size1; /* first value is a junked place */
			int c;
			unsigned char v;
			char aux[4];

			/* initial step: arrange the index */
			resourceListRebuildForIndex(&resIndex);
			res=resourceListGetElement(&resIndex);

			/* first step: read the list to create the master index */
			strcpy(index,"X");
			do {
				if (strncmp(res->id.index,index,4)) {
					dat_humanToDatIndex(aux,res->id.index);
					for (c=0;c<4;c++)
						fwritechar((unsigned char*)(aux+c),writeDatFile);
					fwriteshort(&totalItems,writeDatFile); /* Junk (I) */

					strncpy(index,res->id.index,5);
					/*flags=res->flags;*/
				}
			} while ((res=resourceListGetElement(&resIndex)));

			/* second step: read the list to create the slave indexes */
			resourceListStartIteration(&resIndex);
			res=resourceListGetElement(&resIndex);
			strcpy(index,"a");
			do {
				if (strncmp(res->id.index,index,4)) {
					int relativePos=ftell(writeDatFile)-size1;

					/* go to the master index to write the beginning of this new index */
					fseek(writeDatFile,size1+6*(++totalItems),SEEK_SET);
					fwriteshort(&relativePos,writeDatFile); /* overwrite junk (I) */

					/* go to the last junk (II) and write the right value */
					fseek(writeDatFile,slaveCountPos,SEEK_SET);
					fwriteshort(&numberOfSlaveItems,writeDatFile); /* overwrite junk (II) */
					numberOfSlaveItems=0;

					/* return to the end of the file to keep writing */
					fseek(writeDatFile,0,SEEK_END);
					slaveCountPos=ftell(writeDatFile); /* remember the junk (II) place */
					fwriteshort(&numberOfSlaveItems,writeDatFile); /* junk (II) */

					strncpy(index,res->id.index,5);
				}
				/* write slave index content */
				fwriteshort(&(res->id.value),writeDatFile);
/*printf("writing resource: %d:%4s at %d f=%08x\n",res->id.value,res->id.index,res->offset,res->flags);*/
				fwritelong(&(res->offset),writeDatFile);
				fwriteshort(&(res->content.size),writeDatFile);

				/* this is the flag written in endian-safe */
				v=res->flags>>16;
				fwritechar(&v,writeDatFile);
				v=res->flags>>8;
				fwritechar(&v,writeDatFile);
				v=res->flags;
				fwritechar(&v,writeDatFile);

				numberOfSlaveItems++;
			} while ((res=resourceListGetElement(&resIndex)));
			size2=ftell(writeDatFile)-size1;
			fseek(writeDatFile,slaveCountPos,SEEK_SET);
			fwriteshort(&numberOfSlaveItems,writeDatFile); /* overwrite junk (II) */
		}
	} else {
		dontSave=1;
	}

	/* Write totalItems */
	fseek(writeDatFile,size1,SEEK_SET);
	fwriteshort(&totalItems,writeDatFile); /* Definitive total items count */

	/* Write first 6 bytes header */
	fseek(writeDatFile,0,SEEK_SET);
	fwritelong(&size1,writeDatFile);
	fwriteshort(&size2,writeDatFile);

	/* Closes the file and flushes the buffer */
	writeClose(writeDatFile,dontSave,optionflag,backupExtension);

	/* Drops the index list */
	resourceListDrop(&resIndex);
}
#endif

/***************************************************************\
|                       DAT R/W primitives                      |
\***************************************************************/

#ifdef PR_DAT_INCLUDE_DATREAD
#ifdef PR_DAT_INCLUDE_DATWRITE
int mRWBeginDatFile(const char* vFile, unsigned short int *numberOfItems, int optionflag) {
	int error;
	if ((error=mReadBeginDatFile(numberOfItems,vFile))) return error;
	if ((error=mWriteBeginDatFile(vFile,optionflag))) {
		mReadCloseDatFile();
		return error;
	}
	return PR_RESULT_SUCCESS;
}
#endif
#endif
