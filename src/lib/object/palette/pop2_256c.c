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
#include "auxiliary.h" /* isA64kPalette */

#include "pal.h"

/*typedef struct { 
	tColor* colorArray;
	int size;
}tGenericPalette;*/

void* objPop2PaletteNColorsCreate(tBinary cont, int *error) {
	tGenericPalette* r;
	int i,j;
	
	*error=PR_RESULT_SUCCESS;
#if 0 /* "Princess of Persia" and "SNES Remnants" have bugged palettes. */
	if (!isA64kPalette(cont)) {
		*error=-14; /* TODO FIX or assign error code */
		return NULL;
	}
#endif
	r=(tGenericPalette*)malloc(sizeof(tGenericPalette));
	r->colorArray=(tColor*)malloc(sizeof(tColor)*((cont.size+2)/3));
	for (i=0,j=0;i<cont.size;i+=3,j++) {
		r->colorArray[j].r=to8bits_B(cont.data[i+0]);
		r->colorArray[j].g=to8bits_B(cont.data[i+1]);
		r->colorArray[j].b=to8bits_B(cont.data[i+2]);
	}
#if 0 /* actually, headerless palettes come in all sizes */
	if (j!=256 && j!=320){ /*TODO: add free */
		puts("palette error"); return NULL;
	}
#endif
	r->size=j;
	
	return (void*)r;
}

tColor* objPalette_pop2_ncolorsGetColors(void* o) {
	tGenericPalette* pal=o;
	return pal->colorArray;
}

int objPop2PaletteNColorsWrite(void* o, const char* file, int optionflag, const char* backupExtension) {
	tGenericPalette* p=o;
	return writePal(file,p->size,p->colorArray,optionflag,backupExtension);
}

void* objPop2PaletteNColorsRead(const char* file,int *result) {
	tGenericPalette *r;
	
	r=(tGenericPalette*)malloc(sizeof(tGenericPalette));
				
	*result=readPal(file,&r->colorArray,&r->size);

#if 0 /* headerless palettes come in all sizes at importing, too */
	if (*result==PR_RESULT_SUCCESS && !(r->size==256 || r->size==320)) {
		*result=RESULT_PAL_UNSUPPORTED;
		free(r->colorArray);
		free(r);
		return NULL;
	}
#endif
	
	return (void*)r;
}

int objPop2PaletteNColorsSet(void* o,tResource* res) {
/*	puts("objPop2PaletteNColorsSet");*/
	tGenericPalette* pal=o;
	int i;

	res->content.size=pal->size*3;
/*printf("PALSIZE=%d DATA=%p\n",pal->size,res->content.data);*/
	res->content.data=getMemory(res->content.size);
/*	res->content.data=pal->raw;*/
	for (i=0;i<pal->size;i++) {
		res->content.data[(i*3)+0]=convert24to18(pal->colorArray[i].r);
		res->content.data[(i*3)+1]=convert24to18(pal->colorArray[i].g);
		res->content.data[(i*3)+2]=convert24to18(pal->colorArray[i].b);
	}
/*	res->content.size=100;
	res->content.data=pal->raw;*/
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}

