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
rlec_uncompress.c: Princed Resources : Image Compression Library :
¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯  Run length encoding with checksum decompressor

 Copyright 2003, 2004, 2005, 2006 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)
  Version: 2.00 (2004-Mar-07)

 Note:
  DO NOT remove this copyright notice
*/

#include <stdio.h>
#include <stdlib.h>
#include "compress.h"

/* Expands RLE algorithm */
int expandRleC(const unsigned char* input, int inputSize,
               unsigned char* output, int *outputSize) {
	register unsigned char rep=0;
	int oCursor=0;
	int iCursor=0;
	/*int done=0;*/

	/* main loop */
	while (iCursor<inputSize) {
		rep=(input[iCursor++]);
		/*done=0;*/
		if (rep&0x80) { /* repeat */
			rep&=(~0x80);
			rep++;
			while (rep--) (output)[oCursor++]=input[iCursor];
			iCursor++;
		} else { /* jump */
			rep++;
			while ((rep--)&&(iCursor<inputSize)) {
				(output)[oCursor++]=input[iCursor++];
			}
		}
	}
	
	*outputSize=oCursor;
	return (rep==255)?COMPRESS_RESULT_SUCCESS:COMPRESS_RESULT_WARNING;
}

