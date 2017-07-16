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
memory.c: Princed Resources : Memory handling and comparing routines
¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 20 Dec 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2004-Feb-13)

 Note:
  DO NOT remove this copyright notice
*/

#include "memory.h"
#include "binary.h"
#include <string.h>

char* strallocandcopy(const char* text) {
	int size;
	char* aux;
	if (text==NULL) return NULL;
	size=strlen(text)+1;
	aux=(char*)malloc(size);
	if (aux) memcpy(aux,text,size);
	return aux;
}

unsigned char* binaryallocandcopy(const unsigned char* bin,int size) { /* TODO: replace by the next function */
	unsigned char* aux;
	if (bin==NULL) return NULL;
	aux=(unsigned char*)malloc(size);
	if (aux) memcpy(aux,bin,size);
	return aux;
}

tBinary tbinaryallocandcopy(tBinary c) {
	tBinary aux;
	if (c.data==NULL) return c;

	aux.data=(unsigned char*)malloc(c.size);
	aux.size=c.size;
	
	if (aux.data) memcpy(aux.data,c.data,c.size);
	return aux;
}

void str5lowercpy (char* dst,const char* src) {
	int i;
	for (i=0;(i<4)&&(*src);i++) {
		*dst=((*src>='A')&&(*src<='Z'))?*src|0x20:*src;
		src++;
		dst++;
	}
	*dst=0;
}

char* strToLower(const char* txt) {
	static char ret[5];
	char* r=ret;
	while (*txt) {
		*r=(((unsigned char)'A'<=(unsigned char)(*txt)) && ((unsigned char)(*txt)<=(unsigned char)'Z'))?(*txt)|0x20:*txt;
		r++;
		txt++;
	}
	*r=0;
	return ret;
}

int matchesIn(const char *s, const char *p) {
	switch(*p) {
		case 0:   return !(*s);
		case '*': while(*(p+1)=='*') p++; return matchesIn(s,p+1) || ((*s) && matchesIn(s+1,p));
		case '?': return (*s) && matchesIn(s+1,p+1);
		case '\\':
		case '/': return ('/'==(*s)||'\\'==(*s)) && matchesIn(s+1,p+1);
		case '&': p++;
		default:  return ((*p)==(*s)) && matchesIn(s+1,p+1);
	}
}

#ifndef IGNORE_EQUALS
int equalsIgnoreCase2(const char s1[],const char s2[]) {
	int i=0;
	for (;((s1[i]&0xDF)==(s2[i]&0xDF))&&s1[i];i++);
	return !(s1[i]||s2[i]);
}
#endif

int getOrder(const char* order) {
	if (order) {
		if (equalsIgnoreCase(order,"first")) {
			return 0; /* first */
		} else if (equalsIgnoreCase(order,"second")) {
			return 1; /* second */
		} else if (equalsIgnoreCase(order,"last")) {
			return 65535; /* last */
		} else {
			return atoi(order);
		}
	} else {
		return 0; /* else: first */
	}
}

