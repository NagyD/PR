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
binary.c: Princed Resources : Binary resource handling
¯¯¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 17 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2006-Feb-09)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include "binary.h" /* tBinary */
#include "common.h"
#include "dat.h" /* WriteDat */
#include "reslist.h" /* tResources */
#include "disk.h" /* we are skiping the format layer here */
#include <stdlib.h>

/***************************************************************\
|                         Binary Object                         |
\***************************************************************/

void* objBinaryCreate(tBinary cont, int *error) { /* use get like main.c */
	tBinary* r;
	*error=PR_RESULT_SUCCESS;
	
	r=(tBinary*)malloc(sizeof(tBinary));
	*r=cont;
	return (void*)r;
}

int objBinaryWrite(void* o, const char* file, int optionflag, const char* backupExtension) {
	tBinary* b=o;
	return writeData(b->data,0,file,b->size,optionflag,backupExtension)?PR_RESULT_SUCCESS:PR_RESULT_ERR_FILE_NOT_WRITE_ACCESS;
}

void* objBinaryRead(const char* file,int *result) {
	tBinary o=mLoadFileArray(file);
	if (o.size<0) {
		*result=o.size;
		return NULL;
	}
	return objBinaryCreate(o,result);
}

int objBinarySet(void* o,tResource* res) {
	tBinary* bin=o;
	res->content=*bin;
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}

