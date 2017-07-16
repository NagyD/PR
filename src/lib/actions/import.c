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
import.c: Princed Resources : DAT Importer
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

#include <string.h>
#include "common.h"
#include "import.h"

#include "autodetect.h"
#include "dat.h"
#include "disk.h"
#include "idlist.h"
#include "memory.h"
#include "unknown.h"
#include "object.h"

#include "bmp.h"
#include "mid.h"
#include "wav.h"
#include "pal.h"
#include "plv.h"

#include "sound.h" /* wavetype */

extern FILE* outputStream;

char* errors[]         =PR_TEXT_ERRORS;

/***************************************************************\
|                   M A I N   F U N C T I O N                   |
\***************************************************************/

int import_full(const char* vFiledat, const char* vDirExt, tResourceList* r, int optionflag, const char* vDatFileName,const char* backupExtension) {
	/*
		Return values:
			-1 File couldn't be open for writing
			00 File successfully compiled
			positive number: number of missing files
	*/

	char vFileext[MAX_FILENAME_SIZE];
	int error=0;
	int ok=0;
	const tResource* res;
	tResource newRes;

	if (mWriteBeginDatFile(vFiledat,optionflag)) return PR_RESULT_ERR_FILE_DAT_NOT_WRITE_ACCESS; /* File couldn't be open */

	{ /* Read wavetype from file. */
		wavetype = 0;
		sprintf(vFileext,"%s/%s",vDirExt,"wavetype.txt");
		FILE* fp = fopen(vFileext,"r");
		if (fp != NULL) {
			fscanf(fp,"%d",&wavetype);
			fclose(fp);
		}
	}

	list_firstCursor(r);
	while ((res=list_getCursor(r))) {
		int result;
		tObject o;
		/* remember only id and type */
		newRes.id=res->id;
		newRes.type=res->type;
		newRes.flags=res->flags;

		if (hasFlag(raw_flag)) newRes.type=0; /* compile from raw */
		getFileName(vFileext,vDirExt,res,vFiledat,vDatFileName,optionflag,backupExtension,NULL);
		/* TODO: if image read the palette */
		/* the file is in the archive, so I'll add it to the main DAT body */
/*		newRes.content=mLoadFileArray(vFileext);*/
/*		if (newRes.content.size>0) {*/
			/* TODO: let each format handle the files */
		o=readObject(vFileext,&newRes,&result);
/*if(o.obj==NULL) printf("o.obj==NULL in import_full\n");*/
/*			if (!fatal(result)) */
				if (!result)
			if(o.obj!=NULL) setObject(o,&result,&newRes);
			if (result != PR_RESULT_SUCCESS) {
				fprintf(outputStream,PR_TEXT_IMPORT_ERRORS,getFileNameFromPath(vFileext));
				fprintf(outputStream,PR_TEXT_RESULT, errors[-result], result);
			}
/*			if (!fatal(ok)) {
				if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_ERRORS,getFileNameFromPath(vFileext));
				error++;
			} else {
				if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_SUCCESS,getFileNameFromPath(vFileext));
				ok++;
			}*/
			/*free(newRes.content.data);*/
/*		} else {
			if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_NOT_OPEN,getFileNameFromPath(vFileext));
			error++;
		}*/

		list_nextCursor(r);
	}

	/* Close file. If empty, don't save */
	mWriteCloseDatFile(ok,optionflag,backupExtension);

	if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_DONE,ok,error);

	return error;
}

int import_partial(const char* vFiledat, const char* vDirExt, tResourceList* r, int optionflag, const char* vDatFileName,const char* backupExtension) {
	/*
		Return values:
			-2 Previous DAT file was invalid
			-1 File couldn't be open for writing
			00 File successfully compiled
			positive number: number of missing files
	*/

	int                error,errors=0,ok=0;
	int                indexNumber;
	char vFileext[MAX_FILENAME_SIZE];
	unsigned short int numberOfItems;
	tResource          res;
	tResource          newRes;

	/* Initialize abstract variables to read this new DAT file */
	if ((error=mRWBeginDatFile(vFiledat,&numberOfItems,optionflag))) return error;

	{ /* Read wavetype from file. */
		wavetype = 0;
		sprintf(vFileext,"%s/%s",vDirExt,"wavetype.txt");
		FILE* fp = fopen(vFileext,"r");
		if (fp != NULL) {
			fscanf(fp,"%d",&wavetype);
			fclose(fp);
		}
	}

	/* main loop */
	for (indexNumber=0;(indexNumber<numberOfItems);indexNumber++) {
		int readResult;
		readResult=mReadFileInDatFile(&res,indexNumber);
		if (readResult==PR_RESULT_INDEX_NOT_FOUND) return PR_RESULT_ERR_INVALID_DAT; /* Read error */
		if (readResult==PR_RESULT_CHECKSUM_ERROR) fprintf(outputStream,"Warning: Ignoring checksum error\n"); /* Warning */

		/*if (res.id.value==0xFFFF) continue;*/ /* Tammo Jan Bug fix */

		/* add to res more information from the resource list */
		resourceListAddInfo(r,&res);

		if (isInTheItemMatchingList(res.path,res.id)) { /* If the resource was specified */
			int result;
			tObject o;
			if ((!res.type)&&(!hasFlag(raw_flag))) res.type=verifyHeader(res.content);
			if (hasFlag(raw_flag)) res.type=0; /* If "extract as raw" is set, type is 0 */

			/* get save file name (if unknown document is in the XML) */
			getFileName(vFileext,vDirExt,&res,vFiledat,vDatFileName,optionflag,backupExtension,NULL);

			newRes = res;
			o=readObject(repairFolders(vFileext),&newRes,&result);
/*			if (!fatal(ok)) */
			if (!ok)
				setObject(o,&result,&newRes);

			/* the file is in the partial matching list, so I'll import */
/*			newRes.content=mLoadFileArray(vFileext);
			if (newRes.content.size>0) {
				newRes.id=res.id;
				newRes.type=res.type;
				if (!mAddCompiledFileToDatFile(&newRes,vFileext)) {
					if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_ERRORS,getFileNameFromPath(vFileext));
					errors++;
				} else {
					if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_SUCCESS,getFileNameFromPath(vFileext));
					ok++;
				}
				free(newRes.content.data);
			} else {
				if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_NOT_OPEN,getFileNameFromPath(vFileext));
				errors++;
			}
			*/
		} else {
			/* the file wasn't in the partial matching list, so I'll re-copy it from the open DAT file */
			mWriteFileInDatFile/*IgnoreChecksum*/(&res);
		}
	}

	/* Close DAT file */
	mRWCloseDatFile(0);

	if (hasFlag(verbose_flag)) fprintf(outputStream,PR_TEXT_IMPORT_DONE,ok,errors);
	return errors;
}

int compile(const char* vFiledat, const char* vDirExt, tResourceList* r, int optionflag, const char* vDatFileName,const char* backupExtension) {
	if (itemMatchingListActive()) {
		return import_partial(vFiledat,vDirExt,r,optionflag,vDatFileName,backupExtension);
	} else {
		return import_full(vFiledat,vDirExt,r,optionflag,vDatFileName,backupExtension);
	}
}

