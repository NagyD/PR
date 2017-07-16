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
plv.h: Princed Resources : PLV prince level files support headers
¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 29 Nov 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2003-Nov-29)

 PLV file format:
  Defined: 28 Nov 2003

  Authors:
   Brendon James <roomshaker@princed.org>
   Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2003-Nov-28)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_PLV_H_
#define _PR_PLV_H_

/*
 * PLV 1 SPECS:
 * bytes offset description                     content
 *     7      0 HAS FILE TAG OF 8 LETTERS       "POP_LVL"
 *     1      7 POP VERS                        0x01
 *     1      8 PLV VERS                        0x01
 *     1      9 LEV NUM
 *     4     10 FIELD-PAIR ( NAME / CONTENT ) COUNT
 *     4     14 BLOCK 1: LEVEL SIZE (B1)        2306 (including the checksum)
 *    B1     18 BLOCK 1: LEVEL CODE
 *     4  18+B1 BLOCK 2: USER DATA SIZE VALUE IN BYTES (B2)
 *    B2  22+B1 BLOCK 2: LEVEL CODE NEXT, REST OF FILE
 *
 * Total size of file B1+B2+22.
 * All values are unsigned and in the Intel x86 architecture
 */

#include "reslist.h"
#include "common.h" /* for PR_VERSION */

/*int mFormatImportPlv(tResource *res);
int mFormatExportPlv(const unsigned char* data, const char *vFileext,unsigned long int size,unsigned char level, const char* filename, const char* desc, const char* title, const char* vDatAuthor,int optionflag, const char* backupExtension);*/


int writePlv(const char* file, tBinary content, int popversion, const char* datfile, int level, const char* filename, const char* desc, const char* title, const char* vDatAuthor, int optionflag,const char* backupExtension);
int readPlv(tResource *res);

/* Weekdays and months */
#define DATE_WEEKDAYS "Sun\0Mon\0Tue\0Wed\0Thu\0Fri\0Sat"
#define DATE_MONTHS   "Jan\0Feb\0Mar\0Apr\0May\0Jun\0Jul\0Aug\0Sep\0Oct\0Nov\0Dec"

/* Format stuff */
#define PLV_HEADER_A           "POP_LVL"
#define PLV_HEADER_A_SIZE      7

#define PLV_HEADER_B_SIZE      4

#define PLV_FOOT_EDITOR        "Editor Name\0PR\0Editor Version\0"PR_VERSION"\0Level Author"
#define PLV_FOOT_TITLE         "Level Title"
#define PLV_FOOT_DESC          "Level Description"
#define PLV_FOOT_TCREAT        "Time Created"
#define PLV_FOOT_TMODIF        "Time Last Modified"
#define PLV_FOOT_ORIG_FILE     "Original Filename"
#define PLV_FOOT_LEV_NUM_ORIG  "Original Level Number"

#define PLV_DEFAULT_AUTHOR     "PR user"

#endif
