/*
 * LZG compression
 *
 * ----------------------------------------------------------------------------
 *
 * Authors:
 *   Enrique Calot <ecalot.cod@princed.org>
 *   Diego Essaya <dessaya@fi.uba.ar>
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

/*
 * Modus operandi of the compression algorithm
 * -------------------------------------------
 *
 * For each input byte we take a window containing the 1023 previous bytes.
 * If the window goes out of bounds (ie, when the current input byte is
 * before position 1024), we consider it filled with zeros.
 *
 *     00000000000000000000********************************
 *                         ^                  ^
 *                    input start   current input byte
 *           |--------------------------------|
 *                    window size=1023
 *
 * The algorithm works as follows:
 *
 * While there is unread input data:
 *     Create a maskbyte.
 *     For each bit in the maskbyte (and there is still unread input data):
 *         Compare the following input bytes with the bytes in the window,
 *         and search the longest pattern that is equal to the next bytes.
 *         If we found a pattern of length n > 2:
 *             Assign 0 to the current bit of the maskbyte.
 *             In the next 2 bytes of the output, specify the relative
 *             position and length of the pattern.
 *             Advance output pointer by 2.
 *             Advance input pointer by n.
 * 	       Else:
 *             Assign 1 to the current bit of the maskbyte.
 * 	           Copy the current input byte in the next output byte.
 *             Advance output pointer by 1.
 *             Advance input pointer by 1.
 */

#include <stdio.h>
#include <stdlib.h>
#include "compress.h"

/*#define LZG_REVERSE*/
extern int compressionHigher;

#ifdef LZG_REVERSE
void *memrchr2(unsigned char *s, int c, size_t n) {
	while (n--) {
		if (s[n]==c) return s+n;
	}
	return NULL;
}
#define memsearch(a,b,c) memrchr2(a,b,c)
#else
#include <string.h>  /* memchr() */
#define memsearch(a,b,c) memchr(a,b,c)
#endif

/* Window size */
#define WIN_SIZE 1024

/* A repetition pattern must have a length of at least MIN_PATTERN_SIZE
 * to be accepted (ie, to be worth for compressing) */
#define MIN_PATTERN_SIZE 3
#define MAX_PATTERN_SIZE 66

/* search the longest pattern in the window that matches the first bytes
 * of the input */
void search_best_pattern(unsigned char *input, int inputSize,
                         unsigned char **best_pattern, int *best_pattern_len)
{
	unsigned char *pattern;
	int pattern_len;
	unsigned char *window = input - (WIN_SIZE /*- 1*/);
	int window_len = WIN_SIZE /*- 1*/;

	*best_pattern_len = 0;

	while ((pattern = (unsigned char *)memsearch(window, *input, window_len)))
	{
		unsigned char *ic = input + 1;
		unsigned char *wc = pattern + 1;
		pattern_len = 1;

		while (
			(ic < (input + inputSize)) &&
			(*ic == *wc) &&
			pattern_len < MAX_PATTERN_SIZE
		)	{
			ic++; wc++; pattern_len++; /* increase until the pattern is different */
		}

		if (pattern_len > *best_pattern_len) /* if it is the maximum, save it */
		{
			/*window_len+= *best_pattern_len - pattern_len; * optimization *
			if (window_len <= 0) break;*/
			*best_pattern_len = pattern_len;
			*best_pattern = pattern;
		}

		if (pattern_len == MAX_PATTERN_SIZE) break;

		/* if cLevel is 6 or 7 (cHigh set) compression rate will be
		 * 5% better and compression time will be 400% slower */ /* but I fixed it */
		cHigh {
			/* If window..pattern-1 doesn't contain *input, */
			/* then window+1..pattern-1 will also not contain it. */
			/* So the next search would find the same pattern. */
			/*window_len--;
			window++;*/ /* fix by David */
			/* So we should search from pattern+1. */
			++pattern; window_len-=pattern-window; window=pattern;
			if (window_len <= 0) break;
		} else {
			window_len -= wc - window;
			if (window_len <= 0) break;
			window = wc;
		}
	}
}

/* Insert the specified bit in the current maskByte. If the maskByte is full,
 * start a new one. */
static int maskBit=8;
void pushBit(int b, unsigned char *output, int *oCursor)
{
	static unsigned char* maskByte;
	if ( maskBit == 8 ) /* first time or maskBit is full */
	{
		/* start a new maskByte */
		maskByte = output + *oCursor;
		(*oCursor)++;
		*maskByte = 0;
		maskBit = 0;
	}

	*maskByte |= b<<maskBit;
	maskBit++;
}

/* Insert the two bytes describing the pattern repetition to the output. */
void addPattern(unsigned char *input, int iCursor,
                unsigned char *output, int oCursor,
                unsigned char *pattern, int pattern_len)
{
	int loc = (pattern - input) + WIN_SIZE - MAX_PATTERN_SIZE;
	output[oCursor] =
		(((pattern_len - MIN_PATTERN_SIZE) << 2) & 0xfc) + ((loc & 0x0300) >> 8);
	output[oCursor + 1] = (loc & 0x00FF);
}

/* Compress using the LZG algorithm */
/* Assume output has been allocated and the size is very big */
void compressLzg(const unsigned char* input2, int inputSize,
                 unsigned char* output, int *outputSize)
{
	int iCursor = 0, oCursor = 0;
	unsigned char* input=(unsigned char*)malloc(inputSize+WIN_SIZE);

	/* Create ghost window filled with zeros before input data: */
	memset(input, 0, WIN_SIZE);
	input += WIN_SIZE;
	memcpy(input,input2,inputSize);

	while (iCursor < inputSize)
	{
		unsigned char *best_pattern;
		int best_pattern_len;

		search_best_pattern(input + iCursor, inputSize - iCursor,
		                    &best_pattern, &best_pattern_len);

		if (best_pattern_len < MIN_PATTERN_SIZE)
		{
			/* No suitable pattern found. Just copy the current byte. */
			pushBit(1, output, &oCursor);
			output[oCursor] = input[iCursor];
			iCursor++;
			oCursor++;
		}
		else
		{
			/* Can compress. Repeat the best pattern. */
			pushBit(0, output, &oCursor);
			addPattern(input, iCursor, output, oCursor,
			           best_pattern, best_pattern_len);
			iCursor += best_pattern_len;
			oCursor += 2;
		}
	}

	maskBit=8; /* reinitialize maskBit*/

	free(input-WIN_SIZE);
	*outputSize = oCursor;
}

