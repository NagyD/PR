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
compress.h: Princed Resources : Compression headers
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_COMPRESS_H_
#define _PR_COMPRESS_H_

#include "common.h"

#define COMPRESS_RAW    0x00
#define COMPRESS_RLE_LR 0x01
#define COMPRESS_RLE_UD 0x02
#define COMPRESS_LZG_LR 0x03
#define COMPRESS_LZG_UD 0x04

#define COMPRESS_WORKING_ALGORITHMS 5

#define COMPRESS_RESULT_FATAL   PR_RESULT_COMPRESS_RESULT_FATAL
#define COMPRESS_RESULT_WARNING PR_RESULT_COMPRESS_RESULT_WARNING
#define COMPRESS_RESULT_SUCCESS PR_RESULT_SUCCESS 

/* Prototypes */

extern int compressionLevel;
extern int compressionHigher;

#define cLevel(a) if (compressionLevel>=a)
#define cHigh     if (compressionHigher)
#define setHigh   compressionHigher=1
#define unsetHigh compressionHigher=0

/* private declarations */

/***************************************************************\
|                Internal compression prototypes                |
\***************************************************************/

/* compress and sets the bytes */
void compressLzg(const unsigned char* input, int inputSize,
                 unsigned char* output, int *outputSize);
void compressRle(const unsigned char* input, int inputSize,
                 unsigned char* output, int *outputSize);

void compressRleC(const unsigned char* input, int inputSize,
                 unsigned char* output, int *outputSize);

/* decompress and allocates output */
int expandLzg(const unsigned char* input, int inputSize,
               unsigned char** output, int *outputSize);
int expandRle(const unsigned char* input, int inputSize,
               unsigned char** output, int *outputSize);
int expandRleC(const unsigned char* input, int inputSize,
               unsigned char* output, int *outputSize);

#endif
