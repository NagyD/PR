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
auxiliary.c: Princed Resources : Auxiliary common routines to help the object layer
¯¯¯¯¯¯¯¯¯¯¯                      They DO know all the format inside each binary resource,
                                 but they don't know the .dat format itself.

 Copyright 2006 Princed Development Team
  Created: 7 Mar 2006

  Author: Enrique Calot <ecalot.cod@princed.org>

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include "auxiliary.h"
#include "disk.h" /* array2short */

/* Palette helper */

int isA64kPalette(tBinary c) {
	while (c.size--) if (c.data[c.size]>>6) return 0; /* false */
	return 1; /* true */
}

/* Image helper */

/*
 * 	Header info:
 * 	 2 bytes        - height                 little endian short int
 * 	 2 bytes        - width                  little endian short int
 * 	 1 byte         - doubleLayered          boolean char (0 or 1)
 * 	 1 byte         - mask byte composed by:
 * 	   bit 0        - paletted               boolean bit
 *     bits 1,2,3   - color bits-1           octal number, add 1 to get the value
 *     bits 4,5,6,7 - compression type       hex number (from 0 to 4)
 */

void getHeader(const unsigned char* header, int *w, int *h, int *paletted, int *doubleLayered, int *bits, int *algorithm) {
	*w=array2short(header+0);
	*h=array2short(header+2);
	*doubleLayered=header[4];
	*paletted=header[5]>>7;
	*bits=(header[5]>>4)&7;
	*algorithm=header[5]&15;
	(*bits)++;
}

void setHeader(unsigned char* header, int w, int h, int paletted, int doubleLayered, int bits, int algorithm) {
	bits--;
	header[0]=w&0xff;
	header[1]=(w>>8)&0xff;
	header[2]=h&0xff;
	header[3]=(h>>8)&0xff;
	header[4]=doubleLayered;
	header[5]= ((paletted&1)<<7) | ((bits&7)<<4) | ((algorithm&15)<<0);
}

