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
image.h: Princed Resources : Image handling headers
¯¯¯¯¯¯¯
 Copyright 2003, 2004, 2005, 2006 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_IMAGE_H_
#define _PR_IMAGE_H_

#include "reslist.h"
#include "binary.h"

typedef struct { 
	unsigned char r;
	unsigned char g;
	unsigned char b;
}tColor;

typedef struct {
	tColor p8[256];
	tColor p4[16];
	tColor p1[2];
	unsigned char* pop1raw;
	int pop1rawSize;
}tPalette;

typedef struct { 
	int width;
	int widthInBytes;
	int height;
	int bits;
	unsigned char* pix;
	tObject pal;
	int paletteoffset;
	unsigned char type; /* XY where X=B if 4 bits or X=0 if 1 bit; Y=algorithm */
}tImage;

/* Prototypes */

/*int mCompressGraphic(tBinary* input, tBinary* output, int ignoreFirstBytes) */

int objImage16Set(void* o,tResource* res);
int objImage16Write(void* img,const char* file,int optionflag,const char* backupExtension);
int objImage256Set(void* o,tResource* res);
int objImage256Write(void* img,const char* file,int optionflag,const char* backupExtension);
int objImage2Set(void* o,tResource* res);
int objImage2Write(void* img,const char* file,int optionflag,const char* backupExtension);
void* objImage16Create(tBinary c, tObject palette, int *error);
void* objImage16Read(const char* file,tObject palette, int *result);
void* objImage256Create(tBinary c, tObject palette, int *error, int paletteoffset);
void* objImage256Read(const char* file,tObject palette, int *result);
void* objImage2Create(tBinary c, int *error);
void* objImage2Read(const char* file,tObject palette, int *result);

#define getCarry(a) ((((a)>>4)&7)+1)
#define getAlgor(a) ((a)&0x0F)

int mExpandGraphic(const unsigned char* data,tImage *image, int dataSizeInBytes);
int mCompressGraphic(tBinary* input, tBinary* output, int ignoreFirstBytes, int w, int h);

#endif
