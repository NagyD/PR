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
translate.c: Princed Resources : Array translating resources
¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: 16 Dec 2005

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2005-Dec-16)

 Note:
  DO NOT remove this copyright notice
*/

#include "translate.h"
#include <string.h> /* strcmp */

const char* _indexInt[]=TRANS_ARRAY_INDEX_INT;
const char* _indexExt[]=TRANS_ARRAY_INDEX_EXT;

const char* translate (const char* input, const char** s1, const char** s2,int sizeOfArray) {
	while (sizeOfArray--) {
		if (!strcmp(input,s1[sizeOfArray])) {
			return s2[sizeOfArray];
		}
	}
	return input;
}

