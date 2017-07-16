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
text.c: Princed Resources : Text resource handling
¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 7 Dec 2014

  Author: David from Princed

 Note:
  DO NOT remove this copyright notice
*/

#include "text.h"
#include "common.h"
#include "memory.h"
#include "bitfield.h" /*BitReader*/
#include "reslist.h" /*tResource*/
#include "dat.h" /*mWriteFileInDatFile*/

/* The table needed for decompression. */
const unsigned char char_table[128] = { /* Taken from PRINCE.EXE of PoP2. */
0x00,0x20,0x61,0x65,0x74,0x6F,0x6E,0x69,0x73,0x72,0x64,0x6C,0x68,0x75,0x67,0x66,
0x63,0x77,0x79,0x70,0x62,0x6D,0x6B,0x2C,0x76,0x53,0x41,0x2E,0x54,0x27,0x50,0x4D,
0x78,0x42,0x43,0x49,0x52,0x47,0x44,0x57,0x48,0x71,0x45,0x2D,0x7A,0x4E,0x46,0x4B,
0x4C,0x30,0x6A,0x3A,0x35,0x31,0x59,0x4A,0x38,0x5C,0x55,0x3F,0x37,0x33,0x51,0x3B,
0x32,0x21,0x34,0x36,0x39,0x0D,0x0A,0x09,0x4F,0x56,0x58,0x5A,0x28,0x29,0x2A,0x2B,
0x22,0x23,0x24,0x25,0x26,0x3C,0x3D,0x3E,0x2F,0x40,0x5B,0x5D,0x5E,0x5F,0x60,0x07,
0x01,0x02,0x03,0x04,0x05,0x06,0x08,0x0B,0x0C,0x0E,0x0F,0x10,0x11,0x12,0x13,0x14,
0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,0x7B,0x7C,0x7D,0x7E,0x7F,
};

/* The table needed for compression. */
/*const*/ unsigned char inv_char_table[128];
int inv_char_table_inited = 0;
void init_inv_char_table() {
	int i;
	if (inv_char_table_inited) return;
	for (i = 0; i < 128; ++i) {
		inv_char_table[char_table[i]] = i;
	}
	inv_char_table_inited = 1;
}

tBinary decompressTxt4(tBinary compressed) {
	tBinary result;
	unsigned char* data = compressed.data;

	/* The first two bytes store the expanded size. */
	result.size = (data[1] << 8) | data[0];
	result.data = malloc(result.size);
	/* Question: Who will free result.data? */

	unsigned char* out = result.data;
	unsigned char* end = result.data + result.size;

	BitReader br;
	initBitReader(&br, data+2);
	/* The first two bits determine the size of the "long" characters. */
	int long_size = readBitReader(&br, 2) + 5;
	while (out < end) {
		/* Read the next character. */
		unsigned char chr = readBitReader(&br, 4);
		if (chr == 0) {
			/* This is a long character. */
			chr = readBitReader(&br, long_size);
		}
		if (long_size < 8) {
			chr = char_table[chr];
		}
		*(out++) = chr;
	}

	return result;
}

tBinary compressTxt4(tBinary text) {
	tBinary result;
	int i;
	int long_size = 5;
	init_inv_char_table();
	for (i = 0; i < text.size; ++i) {
		unsigned char chr = text.data[i];
		if (chr >= 0x80) {
			long_size = 8; break;
		}
		/*printf("chr='%c'=0x%02X",chr,chr);*/
		chr = inv_char_table[chr];
		/*printf("=table[0x%02X] ",chr);*/
		int this_size = 4;
		if (chr >= 0x10) this_size = 5;
		if (chr >= 0x20) this_size = 6;
		if (chr >= 0x40) this_size = 7;
		/*printf("this_size=%d\n",this_size);*/
		if (this_size > long_size) long_size = this_size;
	}

	/* Worst case size: 2 byte header + 2 bit + (4+long_size) bits for each character. */
	int max_size = 2 + (2 + (4+long_size)*text.size + 7) / 8;

	result.size = max_size;
	result.data = malloc(result.size);
	result.data[0] = text.size & 0xFF;
	result.data[1] = text.size >> 8;
	
	BitWriter bw;
	initBitWriter(&bw, result.data+2);
	writeBitWriter(&bw, 2, long_size-5);
	for (i = 0; i < text.size; ++i) {
		/*printf("i=%d, text.size=%d, bw.pos = %p\n",i,text.size,bw.pos);*/
		unsigned char chr = text.data[i];
		if (long_size < 8) {
			chr = inv_char_table[chr];
		}
		if (chr == 0 || chr >= 0x10) {
			/* Long character. */
			writeBitWriter(&bw, 4, 0);
			writeBitWriter(&bw, long_size, chr);
		} else {
			/* Short character. */
			writeBitWriter(&bw, 4, chr);
		}
	}
	unsigned char* end_pos = endBitWriter(&bw);
	result.size = end_pos - result.data;
	if (result.size > max_size) {
		printf("ERROR! TXT4 became too big! result.size=%ld, max_size=%d\n",result.size,max_size);
	}
	
	return result;
}

void* objTxt4Create(tBinary cont, int *error) {
	tBinary* result;
	*error = PR_RESULT_SUCCESS;
	result = malloc(sizeof(tBinary));
	*result = decompressTxt4(cont);
	return (void*)result;
}

/* Just use Binary instead of these. */
/*
int objTxt4Write(void* o, const char* file, int optionflag, const char* backupExtension) { ??? }
void* objTxt4Read(const char* file,int *result) { ??? }
*/

int objTxt4Set(void* o,tResource* res) {
	tBinary* text = (tBinary*)o;
	res->content = compressTxt4(*text);
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}
