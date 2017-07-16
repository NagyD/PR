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
classify.h: Princed Resources : DAT file classify routine headers
¯¯¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_CLASSIFY_H_
#define _PR_CLASSIFY_H_

/*
 * Return values may be:
 *  -2 Memory error
 *  -1 File not found or no access error
 *
 *  00 Not a valid POP1 DAT file
 *
 *  01 Levels file
 *  02 Graphic file with an image in the first valid entry (not common)
 *  03 Waves/Digital sound file
 *  04 Midis file
 *  05 Valid DAT file with Undefined content
 *  06 Graphic file with a palette in the first valid entry (common)
 *  07 PC Speaker DAT file
 *
 *  11 Pop2 DAT files
 */

int prVerifyDatType(const char* vFiledat);

#endif
