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
bitfield.h: Princed Resources : Bitfield handling headers
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 7 Dec 2014

  Author: David from Princed

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_BITFIELD_H_
#define _PR_BITFIELD_H_

typedef struct BitReader {
	unsigned int data; /* The bits in the queue. */
	int level; /* The number of bits in the queue. */
	unsigned char* pos; /* The position in the input stream. */
} BitReader;

void initBitReader(BitReader* br, unsigned char* pos);
int readBitReader(BitReader* br, int n_bits);

typedef struct BitWriter {
	unsigned int data; /* The bits in the queue. */
	int level; /* The number of bits in the queue. */
	unsigned char* pos; /* The position in the output stream. */
} BitWriter;

void initBitWriter(BitWriter* bw, unsigned char* pos);
void writeBitWriter(BitWriter* bw, int n_bits, int value);
unsigned char* endBitWriter(BitWriter* bw);

#endif
