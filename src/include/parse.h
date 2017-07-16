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
parse.h: Princed Resources : XML parsing prototypes
¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 23 Oct 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-25)

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_PARSE_H_
#define _PR_PARSE_H_

#include "pr.h"
const char* getExtDesc(int type);
tTag* getTagStructure();

#ifdef IGNOREVERIFYDATTYPES
void showTag(int n,tTag* t);
#endif

/* Constants */
#define MAX_VALUE_SIZE        3000
/*#define RES_TYPECOUNT         9*/

/* for RES_FILE_TYPES */
#include "common.h"

#endif
