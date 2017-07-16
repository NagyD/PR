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
bmp.c: Princed Resources : BMP file support
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)
  Version: 1.10 (2004-Feb-17)

 Modified by: David from Princed

 Note:
  DO NOT remove this copyright notice
*/

#include "bmp.h"
#include "common.h"
#include "compress.h"
#include "dat.h"
#include "disk.h"
#include "memory.h"
#include "palette.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mWriteBmp(const char* file,const unsigned char* data, int w, int h, int bits, int colors_pal, tColor* colorArray, int lineWidth, int optionflag, const char* backupExtension) {

	/* declare variables */
	int a;
	unsigned short int planes=1;
	unsigned long int colours_bmp=1<<bits;
	unsigned long int extra=1000;
	unsigned long int filesize;
	unsigned long int headerSize;
	unsigned long int height=h;
	unsigned long int offset;
	unsigned char color;
	unsigned long int width=w;
	const unsigned long int zero=0;
	char lineSerialization;
	FILE* bitmap;

	/* open file */
	if (!writeOpen(file,&bitmap,optionflag)) return PR_RESULT_ERR_FILE_NOT_WRITE_ACCESS;

	/* initialize variables */
/*	width=img.width;
	height=img.height;
	bits=getCarry(img.type);
	colours_bmp=1<<bits;*/
	headerSize=40;
	offset=54+(colours_bmp<<2);
	lineSerialization=(-lineWidth)&3;
	filesize=offset+(lineWidth+lineSerialization)*height;

	/* Write header */
	fwrite     ("BM",2,1   ,bitmap);    /* Magic identifier            */
	fwritelong (&filesize  ,bitmap);    /* File size in bytes          */
	fwritelong (&zero      ,bitmap);    /* reserved                    */
	fwritelong (&offset    ,bitmap);    /* Offset to image data, bytes */
	fwritelong (&headerSize,bitmap);    /* Header size in bytes        */
	fwritelong (&width     ,bitmap);    /* Width of image              */
	fwritelong (&height    ,bitmap);    /* Height of image             */
	fwriteshort(&planes    ,bitmap);    /* Number of colour planes     */
	fwriteshort(&bits      ,bitmap);    /* Bits per pixel              */
	fwritelong (&zero      ,bitmap);    /* Compression type (0=none)   */
	fwritelong (&zero      ,bitmap);    /* Image size in bytes (may be 0 if no compression) */
	fwritelong (&extra     ,bitmap);    /* Pixels per meter x          */
	fwritelong (&extra     ,bitmap);    /* Pixels per meter y          */
	fwritelong (&colours_bmp   ,bitmap);    /* Number of colours           */
	fwritelong (&zero      ,bitmap);    /* Important colours           */

	/* Write ColorTable */
	for (a=0;a<colors_pal && a<colours_bmp;a++) {
		color=colorArray[a].b;
		fwritechar(&color,bitmap); /* Blue  */
		color=colorArray[a].g;
		fwritechar(&color,bitmap); /* Green */
		color=colorArray[a].r;
		fwritechar(&color,bitmap); /* Red   */
		fwritechar(&zero ,bitmap); /* alpha */
	}
	/* Fill unused palette entries with zeroes (makes sense for 256-color images only) */
	for(;a<colours_bmp;a++) fwrite(&zero,4,1,bitmap); /* by David */

	/* Write data */
	while (h--) {
		fwrite(data+h*lineWidth,lineWidth,1,bitmap);
		fwrite(&zero,lineSerialization,1,bitmap);
	}

	fclose(bitmap);
	/*writeCloseOk(bitmap,optionflag,backupExtension);*/
	return PR_RESULT_SUCCESS; 
}

int readBmp(const char* file, unsigned char** data, int *ph, int *pw,  int *pbits, int *pcolors, tColor** colorArray, int *plineWidth) {
/*int mReadBitMap(tImage* image,unsigned char* data, int size) {*/
	FILE* bitmap;
	char lineSerialization;
	int ok;
	char magic[2];
	int a;
	unsigned long int colours=0;
	unsigned long int filesize=0;
	unsigned long int offset=0;
	unsigned long int aux=0;
	unsigned long int headersize=0;
	unsigned short int bits=0;
	unsigned long int height=0;
	unsigned long int width=0;

	bitmap=fopen(file,"rb");
	if (!bitmap){
/*printf("readBmp() returning error because can't open file %s\n",file);*/
		return PR_RESULT_ERR_FILE_NOT_READ_ACCESS; 
	}

	/* Read headers */
		/* file header */
	ok=fread(magic,2,1         ,bitmap);
	ok=ok&&!strncmp(magic,"BM",2);
	ok=ok&&freadlong(&filesize ,bitmap);
	ok=ok&&freadlong(&aux      ,bitmap);
	ok=ok&&(!aux);
	ok=ok&&freadlong(&offset   ,bitmap);
	
		/* info header */
	ok=ok&&freadlong(&aux      ,bitmap);
/* http://en.wikipedia.org/wiki/BMP_file_format#DIB_header_.28bitmap_information_header.29 */
/* GIMP saves 108 byte headers by default */
	ok=ok&&(aux>=40);
        headersize=aux;
	ok=ok&&freadlong(&width    ,bitmap);
	ok=ok&&freadlong(&height   ,bitmap);
	ok=ok&&freadshort(&aux     ,bitmap);
	ok=ok&&(aux==1);
	ok=ok&&freadshort(&bits    ,bitmap);
	ok=ok&&freadlong(&aux      ,bitmap);    /* Compression type (0=none)   */
	if (ok&&aux!=0) { fclose(bitmap); return -1; /* PR_NO_COMPRESS_SUPPORT */ }
	ok=ok&&freadlong(&aux      ,bitmap);    /* Image size in bytes (junk)  */
	ok=ok&&freadlong(&aux      ,bitmap);    /* Pixels per meter x (junk)   */
	ok=ok&&freadlong(&aux      ,bitmap);    /* Pixels per meter y (junk)   */
	ok=ok&&freadlong(&colours  ,bitmap);    /* Number of colours (junk)    */
	ok=ok&&(colours<1000);
	ok=ok&&freadlong(&aux      ,bitmap);    /* Important colours (junk)    */
	colours=1<<bits; /* fix by David */
	/* Verify */
	*plineWidth=(width*bits+7)/8; /* Note: only works in bits=1,2,4,8 */
	lineSerialization=(-*plineWidth)&3;
/*	offset=54+(colors<<2);
	lineSerialization=(-lineWidth)&3;
	filesize=offset+(lineWidth+lineSerialization)*height;*/

	/* Read ColorTable */
	fseek(bitmap,headersize+0x0E,SEEK_SET);
	aux=0;
	*colorArray=malloc(sizeof(tColor)*colours);
	for (a=0;a<colours;a++) {
		ok=ok&&freadchar(&((*colorArray)[a].b),bitmap); /* Blue  */
		ok=ok&&freadchar(&((*colorArray)[a].g),bitmap); /* Green */
		ok=ok&&freadchar(&((*colorArray)[a].r),bitmap); /* Red   */
		ok=ok&&freadchar(&aux,bitmap); /* alpha */
	}

	/* Write data */
	fseek(bitmap,offset,SEEK_SET);
	*ph=height;
	*pw=width;
	*data=malloc((*plineWidth+lineSerialization)*height);
	while (height--) {
		ok=ok&&fread(*data+height**plineWidth,*plineWidth,1,bitmap);
		if (lineSerialization) ok=ok&&fread(&aux,lineSerialization,1,bitmap);
	}
	
#if 0
	/* Validate if there is header and if it starts in BM */
	ok    = size>50;
	ok=ok&& data[0]=='B' && data[1]=='M';
	/* Read sizes from header */
	width=(unsigned short)array2short(data+18);
	height=(unsigned short)array2short(data+22);
	bits=(unsigned short)(data[28]);

	/* Save sizes into image */
	image->width=width;            /* width in pixels */
	image->height=height;

	/* Calculate serialized widths */
	carry=(bits==1)?0:2;
	image->type=(bits==1)?0:0xB0;
	colours=1<<bits;
	offset=54+(colours<<2);
	width=(width+1)>>1;
	image->widthInBytes=(width+(0x07>>(1+carry)))>>(2-carry); /* this is an optimization of /(8/bits) : image->widthInBytes=imageWidthInPixels*bitsPerPixel/(8 bits/byte) */
	lineSerialization=(-image->widthInBytes)&3;
	serializedWidth=image->widthInBytes+lineSerialization;
	filesize=offset+serializedWidth*height;

	/* Validate image and file size; get memory to allocate the image */
	ok=ok&& (filesize==size);
	ok=ok&& (	(image->pix=malloc(height*image->widthInBytes*2)) != NULL	);
	/* if validations==wrong */
	if (!ok) {
		freeAllocation(image->pix);
		fclose(bitmap); /* by David */
		return 0; /* false: this is not a valid BMP file format or memory too low */
	}

	/* Serialize bitmap-->raw array */
	while (height--)
		memcpy(
			image->pix+(x++)*image->widthInBytes,
			data+offset+height*serializedWidth,
			image->widthInBytes
		);
#endif

	fclose(bitmap); /* by David */

	if (!ok) {
		free(*colorArray);
		free(*data);
/*printf("readBmp() returning error because !ok\n");*/
		return PR_RESULT_ERR_FILE_NOT_READ_ACCESS; /* TODO: use a bad format code */
	}
	
	*pbits        = bits;
	*pcolors      = colours;

	return PR_RESULT_SUCCESS;			
}
