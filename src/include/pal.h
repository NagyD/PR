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
pal.h: Princed Resources : JASC PAL files support headers
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_PAL_H_
#define _PR_PAL_H_

#include "image.h"
#include "palette.h"

/*
#define PAL_SAMPLE {0x97, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x03, 0x08, 0x0F, 0x07, 0x0C, 0x13, 0x0C, 0x11, 0x16, 0x11, 0x17, 0x1C, 0x1E, 0x21, 0x25, 0x2F, 0x31, 0x33, 0x3F, 0x3F, 0x3F, 0x2A, 0x2A, 0x2A, 0x15, 0x15, 0x15, 0x17, 0x1C, 0x20, 0x07, 0x0C, 0x13, 0x0C, 0x12, 0x17, 0x15, 0x1A, 0x1F, 0x00, 0x1D, 0x13, 0x00, 0x29, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
*/

/*int mFormatExportPal(const tPalette* p, int bits, char *vFileext, int optionflag, const char* backupExtension);*/
int mFormatImportPal(tPalette* p,int* bits, const char* vFile);

/*
#define mLoadPalette(array,image,popversion)\
	if (popversion==pop1)\
		memcpy(((image).pal),((array)+5),(16*3));\
	else\
		memcpy(((image).pal),((array)+1),(16*3))
*/

int writePal(const char* file, int colors, const tColor* colorArray, int optionflag, const char* backupExtension);
int readPal(const char* file,tColor** colorArray,int *colors);

#endif
