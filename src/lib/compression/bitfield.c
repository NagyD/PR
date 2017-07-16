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
bitfield.c: Princed Resources : Bitfield handling
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 7 Dec 2014

  Author: David from Princed

 Note:
  DO NOT remove this copyright notice
*/

#include "bitfield.h"

/* BitReader */

void initBitReader(BitReader* br, unsigned char* pos) {
	br->data = br->level = 0;
	br->pos = pos;
}

int readBitReader(BitReader* br, int n_bits) {
	/* If there are not enough bits in the queue, read more. */
	while (br->level < n_bits) {
		br->data = (br->data << 8) | *(br->pos++);
		br->level += 8;
	}
	/* Read the topmost bits. */
	int result = (br->data >> (br->level - n_bits)) & ((1 << n_bits) - 1);
	/* Remove the topmost bits. */
	br->level -= n_bits;
	return result;
}

/* BitWriter */

void initBitWriter(BitWriter* bw, unsigned char* pos) {
	bw->data = bw->level = 0;
	bw->pos = pos;
}

void writeBitWriter(BitWriter* bw, int n_bits, int value) {
	/* Add the new bits to the queue. */
	bw->level += n_bits;
	bw->data = (bw->data << n_bits) | (value & ((1 << n_bits) - 1));
	while (bw->level >= 8) {
		/* Write out the topmost 8 bits as a byte. */
		*(bw->pos++) = bw->data >> (bw->level - 8);
		/* Remove the topmost 8 bits. */
		bw->level -= 8;
	}
}

unsigned char* endBitWriter(BitWriter* bw) {
	/* Fill the remaining bits with zeroes, and force writing the bits that are still in the queue. */
	/* Note: the zeroes are written only until the end of the current byte. */
	/* If the previous write ended on a byte boundary then this does nothing. */
	writeBitWriter(bw,7,0);
	/* Return the current position. (After the last written byte.) */
	unsigned char* pos = bw->pos;
	/* Clear the BitWriter. */
	bw->data = 0;
	bw->level = 0;
	bw->pos = 0;
	return pos;
}
