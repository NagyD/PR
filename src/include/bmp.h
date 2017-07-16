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
bmp.h: Princed Resources : BMP file support headers
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)
  Version: 1.10 (2004-Feb-17)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_BMP_H_
#define _PR_BMP_H_

#include "reslist.h" /* tResource */
#include "image.h" /* tImage */

#define FORMATS_BMP_PALETTE_BW "\x00\x00\x00\0\xFF\xFF\xFF"

int mFormatExportBmp(const unsigned char* data, const char *vFileext,unsigned long int size,tImage image,int optionflag,const char* backupExtension);
int mFormatImportBmp(tResource *res);
int mWriteBitMap(tImage img,const char* vFile,int optionflag,const char* backupExtension);
int mReadBitMap(tImage* img,unsigned char* data, int size); /* Note: this will ignore the palette */

int mWriteBmp(const char* file,const unsigned char* data, int w, int h, int bits, int colors, tColor* colorArray, int lineWidth, int optionflag, const char* backupExtension);
int readBmp(const char* file, unsigned char** data, int *ph, int *pw,  int *pbits, int *pcolors, tColor** colorArray, int *plineWidth);

#endif
