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
other.h: Princed Resources : 
¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 17 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.org>

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_OTHER_H_
#define _PR_OTHER_H_

#include "binary.h"

void* objBinaryCreate(tBinary c, int *error);
int objBinaryWrite(void* o, const char* file, int optionflag, const char* backupExtension);
void* objBinaryRead(const char* file,int *result);
int objBinarySet(void* o,tResource* res);

void* objLevelCreate(tBinary content,int number,const char* datfile,const char* name,const char* desc,const char* datAuthor,int *error); /* TODO: move to level.h */ 
int objLevelWrite(void* o, const char* file, int optionflag, const char* backupExtension);

void* objLevelRead(const char* file,int *result);
int objLevelSet(void* o,tResource* res);

#endif
