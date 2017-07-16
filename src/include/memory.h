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
memory.h: Princed Resources : Memory handling & comparison prototypes
¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 20 Dec 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2004-Feb-13)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_MEMORY_H_
#define _PR_MEMORY_H_

#include <stdlib.h>

#define getMemory(size) ((unsigned char*)malloc(size))
char* strallocandcopy(const char* text);
unsigned char* binaryallocandcopy(const unsigned char* bin,int size);
#define freeAllocation(m) if ((m)!=NULL) free(m)

#define ptoi(p) ((p!=NULL)?atoi(p):0)

/* equalsIgnoreCase is built in unless we are in the BSD standards that is taken from string.h */
#ifdef __USE_BSD
#include <string.h>
#define IGNORE_EQUALS
#define equalsIgnoreCase(a,b) (!strcasecmp(a,b))
#else
#define equalsIgnoreCase(a,b) equalsIgnoreCase2(a,b)
int equalsIgnoreCase2(const char s1[],const char s2[]);
#endif

char* strToLower(const char* txt);
void str5lowercpy (char* dst,const char* src);
int matchesIn(const char *s, const char *p);
int getOrder(const char* order);

#include "binary.h"
tBinary tbinaryallocandcopy(tBinary c);

#endif
