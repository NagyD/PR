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
rle_compress.c: Princed Resources : Image Compression Library
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003, 2004 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)
  Version: 2.00 (2004-Mar-07)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>

/* Compress using the Run Length Encoding algorithm */
void compressRle(const unsigned char* input, int inputSize,
                 unsigned char* output, int *outputSize) {
	/* Declare pointers */
	unsigned char* cursorData        = output;
	char*          counter;
	const unsigned char* cursorPix   = input;
	const unsigned char* imgEnd      = input+inputSize;

	while (cursorPix<imgEnd) {
		/* Step 1: Create counter */
		counter=(char*)(cursorData++);
		*counter=-1;

		/* Step 2: Look and copy the string until more than two repeated bytes are found */
		while (
			(cursorPix+1<imgEnd)&&  /* reads one more */
			(
				(*cursorPix!=*(cursorPix+1))||
				(
					((cursorPix+2)<imgEnd)&& /* reads one more */
					(*cursorPix!=*(cursorPix+2))
				)
			)&&
			((*counter)!=127)
		) {
			*(cursorData)=*(cursorPix);
			(*counter)++;
			cursorPix++;
			cursorData++;
		}

		/* Step 3: If there was a repeated string, let's ignore it and add the cursor with the repetitions */
		if (*counter==-1) {
			while ((cursorPix+1<imgEnd)&&(*cursorPix==(*(cursorPix+1)))&&((*counter)!=-128)) {
				cursorPix++;
				(*counter)--;
			}

			*(cursorData)=*(cursorPix); /* Print repeated char */
			cursorPix++;
			cursorData++;
		}
	}
	/* Write the last char
	if ((*counter)>0) {
		*(cursorData++)=0;
		*(cursorData)= ||||||| 0xff |||||  *(cursorPix); |||||||
	} else {
		(*counter)--;
	}*/
	*outputSize=(int)((long int)cursorData-(long int)output); /* Note: casted to long for portability with 64 bits architectures */
}

