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
mid.c: Princed Resources : MIDI files support
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

/* Includes */
#include "common.h"
#include "dat.h"
#include "disk.h"
#include "mid.h"
#include <stdlib.h>
#include <string.h>

int writeMid(const char* file, tBinary* snd, int optionflag, const char* backupExtension) {
	/* Mid files are saved as raw */
	return writeData(snd->data,0,file,snd->size,optionflag,backupExtension)?PR_RESULT_SUCCESS:PR_RESULT_ERR_FILE_NOT_WRITE_ACCESS;
}

int mFormatImportMid(tResource *res) {
	unsigned char* file;

	file=malloc(res->content.size+1);
	file[0]=(unsigned char)((res->type==4)?2:0); /* Now should be 0x02: First character must be a 0x01 (wav type in DAT) */
	memcpy(file+1,res->content.data,res->content.size);
	res->content.size++;
	free(res->content.data);
	res->content.data=file;
	mWriteFileInDatFile(res);
	return 1; /* true */
}

