/*
 * LZG extraction
 *
 * ----------------------------------------------------------------------------
 *
 * Authors:
 *   Enrique Calot <ecalot.cod@princed.org>
 *
 * Research: Tammo Jan Dijkemma, Anke Balderer, Enrique Calot
 *
 * Modified by: David from Princed
 *
 * ----------------------------------------------------------------------------
 *
 * Copyright (C) 2004, 2005 the Princed Team
 *
 * This file is part of the Princed project.
 *
 * Princed is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Princed is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include "compress.h"

/* modulus to be used in the 10 bits of the algorithm */
#define LZG_WINDOW_SIZE    0x400 /* =1024=1<<10 */

/* LZG expansion algorithm sub function */
unsigned char popBit(unsigned char *byte) {
	register unsigned char bit=(*byte)&1;
	(*byte)>>=1;
	return bit;
}

/* Expands LZ Groody algorithm. This is the core of PR */
int expandLzg(const unsigned char* input, int inputSize,
               unsigned char** output2, int *outputSize) {

	int                    oCursor=0, iCursor=0;
	unsigned char          maskbyte=0;
	/* TODO: Don't reserve more than needed. */
	unsigned char          output[65500];
	register int           loc;
	register unsigned char rep, k;

	if (*outputSize) (*outputSize)+=LZG_WINDOW_SIZE; else *outputSize=65500;

	/* initialize the first 1024 bytes of the window with zeros */
	for(oCursor=0;oCursor<LZG_WINDOW_SIZE;output[oCursor++]=0);

	/* main loop */
	while (iCursor<inputSize&&(oCursor)<(*outputSize)) {
		maskbyte=input[iCursor++];
		for (k=8;k&&(iCursor<inputSize)&&(oCursor)<(*outputSize);k--) { /* fix by David */
			if (popBit(&maskbyte)) {
				output[oCursor++]=input[iCursor++]; /* copy input to output */
			} else {
				/*
				 * loc:
				 *  10 bits for the slide iCursorition (S). Add 66 to this number,
				 *  substract the result to the current oCursor and take the last 10 bits.
				 * rep:
				 *  6 bits for the repetition number (R). Add 3 to this number.
				 */
				loc= 66 + ((input[iCursor] & 0x03 /*00000011*/) <<8) + input[iCursor+1];
				rep= 3  + ((input[iCursor] & 0xfc /*11111100*/) >>2);

				iCursor+=2; /* move the cursor 2 bytes ahead */

				loc=(oCursor-loc)&0x3ff; /* this is the real loc number (allways positive!) */
				if(loc==0) loc=0x400; /* by David */

				while (rep--) { /* repeat pattern in output */
					output[oCursor]=output[oCursor-loc];
					oCursor++;
				}
			}
		}
	}

	inputSize-=iCursor;
	/* ignore the first 1024 bytes */
	*outputSize=oCursor-LZG_WINDOW_SIZE;
	*output2=(unsigned char*)malloc(*outputSize);
	for(iCursor=LZG_WINDOW_SIZE;iCursor<oCursor;iCursor++)
		(*output2)[iCursor-LZG_WINDOW_SIZE]=output[iCursor];

	if (oCursor>=(*outputSize)) return inputSize; /* TODO: check if this case never happens !!! */

	return (!maskbyte)-1;
	/*return rep?COMPRESS_RESULT_WARNING:COMPRESS_RESULT_SUCCESS;*/
}

