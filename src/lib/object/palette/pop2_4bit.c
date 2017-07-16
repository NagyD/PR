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
palette.c: Princed Resources : The palette object implementation
¯¯¯¯¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 09 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2006-Feb-09)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "palette.h"
#include "memory.h"
#include "dat.h"
#include "disk.h" /* writeData */

#include "pal.h"

void addPop1Raw(tPalette* p,unsigned char* data, int dataSize);

/***************************************************************\
|                         Palette Object                        |
\***************************************************************/

/*static tColor sample_pal4[]={{0,0,0},{4,4,4}};*/ /*SAMPLE_PAL16;*/

#if 0
int setPalette(tPalette* p, int bits, tColor* palette) {
	switch(bits) {
	case 1:
		memcpy(p->p1,palette,2*sizeof(tColor));
		break;
	case 4:
		memcpy(p->p4,palette,16*sizeof(tColor));
		break;
	case 8:
		memcpy(p->p8,palette,256*sizeof(tColor));
		break;
	default:
		return -1; /* unsupported bit rate */
	}
	return 0;
}

int getPalette(const tPalette* p, int bits, const tColor** apalette) {
	switch(bits) {
	case 1:
		*apalette=p->p1;
		break;
	case 4:
		*apalette=p->p4;
		break;
	case 8:
		*apalette=p->p8;
		break;
	default:
		return -1; /* unsupported bit rate */
	}
	return 0;
}

tPalette createPalette() {
	tPalette r;
	int i;

	/* Palette 1 bit */
	r.p1[0].r=0;
	r.p1[0].g=0;
	r.p1[0].b=0;
	r.p1[1].r=255;
	r.p1[1].g=255;
	r.p1[1].b=255;

	/* Palette 4 bits */
	memcpy(r.p4, sample_pal4, 16);

	/* Palette 8 bits */
	for (i=0;i<256;i++) {
		r.p8[i].r=i;
		r.p8[i].g=i;
		r.p8[i].b=i;
	}

	/* initialize the rest */
	r.pop1raw=NULL;
	return r;
}
#endif

/* middle layer */
#define to8bits_A(a) (((a)<<2)|((a)>>4))
#define to8bits_B(a) (((a)<<2)         )
#define to8bits_C(a) (((a)<<2)+2       )

#if 0
/* reads the information in data and tries to remember it in the palette */
int readPalette(tPalette* p, unsigned char* data, int dataSize) {
	tColor c[256];
	int i,bits=0;
	*p=createPalette();
	printf("reading a palette from data (%d)\n",dataSize);
	/* TODO: validate checksum */

	switch (dataSize) {
	case 101:
		for (i=0;i<16;i++) {
			c[i].r=data[(i*3)+5]<<2;
			c[i].g=data[(i*3)+6]<<2;
			c[i].b=data[(i*3)+7]<<2;
		}
		bits=4;
		/* this palette needs to be remembered as binary */
		/*addPop1Raw(p,data+1,dataSize-1);*/
		break;
	case 3*256+1:
	case 3*320+1:
		for (i=0;i<256;i++) {
			c[i].r=data[(i*3)+1]<<2;
			c[i].g=data[(i*3)+2]<<2;
			c[i].b=data[(i*3)+3]<<2;
		}
		bits=8;
		break;
	}

	if (bits) setPalette(p,bits,c);
	return bits;
}
#endif
/*
int applyPalette(tPalette* p,tImage *i) {
	i->pal=*p;
	return 0;
}

void addPop1Raw(tPalette* p,unsigned char* data, int dataSize) {
	freeAllocation(p->pop1raw);
	p->pop1raw=binaryallocandcopy(data,dataSize);
	p->pop1rawSize=dataSize;
}
*/

typedef struct {
	tColor c[16];
	unsigned char raw[55];
}tPop2_4bitsPalette;

void* objPalette_pop2_4bitsCreate(tBinary cont, int *error) {
	int i;
	tPop2_4bitsPalette* pal;
	// Don't check size, PoP2 palettes can have various sizes.
	/*
	if (cont.size!=55) {
		*error=PR_RESULT_XML_AND_DAT_FORMAT_DO_NOT_MATCH;
		return NULL;
	}
	*/

	pal=(tPop2_4bitsPalette*)malloc(sizeof(tPop2_4bitsPalette));

	for (i=0;i<16;i++) {
		pal->c[i].r=cont.data[(i*3)+7]<<2;
		pal->c[i].g=cont.data[(i*3)+8]<<2;
		pal->c[i].b=cont.data[(i*3)+9]<<2;
	}

	memcpy(pal->raw,cont.data,55);

	*error=PR_RESULT_SUCCESS;

	return (void*)pal;
}

int objPalette_pop2_4bitsWrite(void* o, const char* file, int optionflag, const char* backupExtension) {
	tPop2_4bitsPalette* pal=o;
	char aux[260];

	/* Export extra palette information */
	sprintf(aux,"%s.more",file);
	writeData(pal->raw,0,aux,55,optionflag,backupExtension);

	return writePal(file,16,pal->c,optionflag,backupExtension);
}

tColor* objPalette_pop2_4bitsGetColors(void* o) {
	tPop2_4bitsPalette* pal=o;
	return pal->c;
}

#if 0
tColor* paletteGetColorArrayForColors(int colors) {
	static tColor p1[2];
	static tColor p4[2];
	static tColor p8[2];
	int i;

	switch(colors) {
	case 2:
		/* Palette 1 bit */
		p1[0].r=0;
		p1[0].g=0;
		p1[0].b=0;
		p1[1].r=255;
		p1[1].g=255;
		p1[1].b=255;
		return p1;
	case 16:
		/* Palette 4 bits */
		memcpy(p4, sample_pal4, 16);
		return p4;
	case 256:
		/* Palette 8 bits */
		for (i=0;i<256;i++) {
			p8[i].r=i;
			p8[i].g=i;
			p8[i].b=i;
		}
		return p8;
	default:
		return NULL; /* unsupported bit rate */
	}
}
#endif

void* objPop2Palette4bitsRead(const char* file,int *result) {
	tPop2_4bitsPalette* pal=(tPop2_4bitsPalette*)malloc(sizeof(tPop2_4bitsPalette));
	tColor* colorArray;
	int colors;
	char aux[260];
	tBinary raw;

	/* Import extra palette information */
	sprintf(aux,"%s.more",file);
	raw=mLoadFileArray(aux);
	if (raw.size!=55) return NULL; /* TODO; free memory */
	memcpy(pal->raw,raw.data,55);
	free(raw.data);

	*result=readPal(file,&colorArray,&colors);

	if (*result==PR_RESULT_SUCCESS && colors!=16) {
		*result=/*PR_WRONG_PALETTE_COLOR_COUNT*/RESULT_PAL_UNSUPPORTED;
		free(colorArray);
		return NULL;
	}
	memcpy(pal->c,colorArray,sizeof(tColor)*16);
	free(colorArray);

	return (void*)pal;
}

/*#define convert24to18(x) (unsigned char)((x+2)>>2);*/

int objPop2Palette4bitsSet(void* o,tResource* res) {
	tPop2_4bitsPalette* pal=o;
	int i;

	res->content.size=55;
	res->content.data=pal->raw;
	for (i=0;i<16;i++) {
		res->content.data[(i*3)+7]=convert24to18(pal->c[i].r);
		res->content.data[(i*3)+8]=convert24to18(pal->c[i].g);
		res->content.data[(i*3)+9]=convert24to18(pal->c[i].b);
	}
	res->content.size=55;
	res->content.data=pal->raw;
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}

