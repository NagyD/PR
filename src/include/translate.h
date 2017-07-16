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
translate.h: Princed Resources : Array translating resources prototypes
¯¯¯¯¯¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: 16 Dec 2005

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2005-Dec-16)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_TRANSLATE_H_
#define _PR_TRANSLATE_H_

#define TRANS_ARRAY_INDEX_INT {\
	"pop1", "cust", "fram", "palc", "pals", "palt", "piec", "scr", "shap", "shpl", "strl", "snd", "seqs", "txt4"}

#define TRANS_ARRAY_INDEX_EXT {\
	"pop 1","custom", "frame", "cga palette", "svga palette", "tga palette", "piece", "screen", "shape", "shape palette", "string", "sound", "sequence", "text4"}

#define TRANS_ARRAY_INDEX_SIZ 13

extern const char* _indexInt[];
extern const char* _indexExt[];

const char* translate (const char* input, const char** s1, const char** s2,int sizeOfArray);

#define translateInt2Ext(a) translate (a, _indexInt, _indexExt, TRANS_ARRAY_INDEX_SIZ)
#define translateExt2Int(a) translate (a, _indexExt, _indexInt, TRANS_ARRAY_INDEX_SIZ)

#endif

