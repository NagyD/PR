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
common.h: Princed Resources : Defines and prototypes common to all PR code
¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 24 Aug 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2003-Aug-24)

  Modified by: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.10 (2003-Dec-03)

  Language: Abstract

 Note:
  DO NOT remove this copyright notice
*/

#ifndef _PR_COMMON_H_
#define _PR_COMMON_H_

/***************************************************************\
|                Princed Resource Library Functions             |
\***************************************************************/

#include <stdio.h>

/* User configuration defines */
#ifndef WIN32
#define UNIX
#endif

#ifdef WIN32
#ifndef OS
#define OS "Win32"
#endif
#endif

/***************************************************************\
|             M A I N   P R O G R A M   D E F I N E S           |
\***************************************************************/

/***************************************************************\
|                     PR Specific Defines                       |
\***************************************************************/

#define PR_URL                    "http://www.princed.org"
#define PR_VERSION                "v1.3.1"
#define PR_COPY                   "(c) Copyright 2003-2012 Princed Development Team"

/***************************************************************\
|                         Other defines                         |
\***************************************************************/

/* Default Files */
#define RES_XML_UNKNOWN_XML      "unknown.xml"
#define RES_XML_RESOURC_XML      "resources.xml"
#define RES_XML_UNKNOWN_PATH     "unknown"

#define DEFAULT_BACKUP_EXTENSION "bak"

/* File extensions */
#define RES_FILE_EXTENSIONS   {"raw","bin","bmp","bmp","bmp","plv","mid","pcs","pal","pal","pal","pal","pal","txt","wav"}
#define RES_FILE_TYPES        {"autodetect","binary","image","image-mono","image-full","level","midi","pcspeaker","palette-pop1-4bits","palette-pop1-guards","palette-mono","palette-pop2-320cols","palette-pop2-4bits","text","wave","txt4"}
#define RES_FILE_TYPES_COUNT  16

typedef enum {
	eResTypeNone=-1,
	eResTypeRaw=0,
	eResTypeBinary=1,
	eResTypeImage16=2,
	eResTypeImage2=3,
	eResTypeImage256=4,
	eResTypeLevel=5,
	eResTypeMidi=6,
	eResTypePcspeaker=7,
	eResTypePop1Palette4bits=8,
	eResTypePop1PaletteGuards=9,
	eResTypePop1PaletteMono=10,
	eResTypePop2PaletteNColors=11,
	eResTypePop2Palette4bits=12,
	eResTypeText=13,
	eResTypeWave=14,
	eResTypeTxt4=15,
} tResourceType;

/***************************************************************\
|                        L A N G U A G E                        |
\***************************************************************/

#include "en.lang.pr.h"

/* Credits */
#define PARSING_ABOUT PR_TXT_AUTHORS": \n\
   "PR_TXT_CODER"\n\
    Enrique Calot\n\
\n\
   "PR_TXT_COD_ASSIST"\n\
    Santiago Benito Zamora\n\
\n\
   "PR_TXT_GFX_COMP"\n\
    Tammo Jan Dijkema\n\
    Enrique Calot\n\
    Diego Essaya\n\
\n\
   "PR_TXT_GFX_DEV"\n\
    Tammo Jan Dijkema\n\
    Anke Balderer\n\
\n\
   "PR_TXT_MID"\n\
    Christian Lundheim\n\
\n\
   "PR_TXT_XML"\n\
    Steven Fayers\n\
\n\
   "PR_TXT_TRANSLATION"\n\
    "PR_TXT_ABOUT_TRANSLATOR"\n\
\n"

/***************************************************************\
|                           Prototypes                          |
\***************************************************************/

#include "pr.h"

/***************************************************************\
|                   Command Line specific options               |
\***************************************************************/

/*#define PR_IGNORE_RAW_OPTION*/
#define DEBUG_RESPRINT

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_OPTRAW ,{"raw",         no_argument,       0,'w'},
#define PARSING_CHRRAW "w"
#define PARSING_SHOWRAW(a) a
#else
#define PARSING_OPTRAW ,
#define PARSING_CHRRAW ""
#define PARSING_SHOWRAW(a) ""
#endif
#define PARSING_OPTIONS {\
{"import",      optional_argument, 0,'m'},\
{"classify",    no_argument,       0,'c'},\
{"export",      optional_argument, 0,'x'},\
\
{"setauthor",   required_argument, 0,'a'},\
{"backup",      optional_argument, 0,'b'},\
{"force",       no_argument,       0,'f'},\
{"format",      required_argument, 0,'F'},\
{"cgi",         no_argument,       0,'g'},\
{"help",        no_argument,       0,'?'},\
{"resource",    required_argument, 0,'s'},\
{"compression-level",required_argument, 0,'z'},\
{"recursive",   no_argument,       0,'r'},\
{"datfile",     required_argument, 0,'t'},\
{"unknown",     no_argument,       0,2},\
{"verbose",     no_argument,       0,'v'},\
{"version",     no_argument,       0,1},\
{"xml-optimize",required_argument, 0,3}\
PARSING_OPTRAW\
{0, 0, 0, 0}\
}

#define PARSING_CHARS    "z::i::m::cx::e::b::a::fgs::t::Rrvh?"PARSING_CHRRAW

/* Flags */
#define import_flag      0x0001
#define classify_flag    0x0002
#define export_flag      0x0004
#define backup_flag      0x0008
#define force_flag       0x0010
#define cgi_flag         0x0020
#define help_flag        0x0040
#define raw_flag         0x0080
#define recursive_flag   0x0100
#define verbose_flag     0x0200
#define version_flag     0x0400
#define unknown_flag     0x0800
#define undef1_flag      0x1000
#define undef2_flag      0x2000
#define undef3_flag      0x4000
#define undef4_flag      0x8000

#define hasFlag(a) (optionflag&(a))
#define setFlag(a) optionflag|=(a)

#define PR_RESULT_ERR_COMMAND_LINE_SYNTAX -17
#define PR_RESULT_ERR_EXTRACTION -18
#define PR_RESULT_ERR_FILE_DAT_NOT_OPEN_NOTFOUND -2
#define PR_RESULT_ERR_FILE_DAT_NOT_OPEN_WASDIR -3
#define PR_RESULT_ERR_FILE_DAT_NOT_READ_ACCESS -4
#define PR_RESULT_ERR_FILE_DAT_NOT_WRITE_ACCESS -5
#define PR_RESULT_ERR_FILE_NOT_OPEN_NOTFOUND -6
#define PR_RESULT_ERR_FILE_NOT_OPEN_WASDIR -7
#define PR_RESULT_ERR_FILE_NOT_READ_ACCESS -8
#define PR_RESULT_ERR_FILE_NOT_WRITE_ACCESS -9
#define PR_RESULT_ERR_INVALID_DAT -10
#define PR_RESULT_ERR_MEMORY -11
#define PR_RESULT_ERR_XML_ALREADY_OPEN -15
#define PR_RESULT_ERR_XML_ATTR -12
#define PR_RESULT_ERR_XML_FILE -13
#define PR_RESULT_ERR_XML_NOT_OPEN -16
#define PR_RESULT_ERR_XML_PARSING -14
#define PR_RESULT_FD_IMPORT_FROM_MORE_THAN_ONE_DIR -20
#define PR_RESULT_FD_IMPORT_RECURSIVE -21
#define PR_RESULT_FD_NO_FILES_SELECTED -22
#define PR_RESULT_INDEX_NOT_FOUND -23
#define PR_RESULT_CHECKSUM_ERROR -24
#define PR_RESULT_ERR_WRONG_PRIMITIVE_CALL -19
#define PR_RESULT_XML_AND_DAT_FORMAT_DO_NOT_MATCH -25
#define PR_RESULT_HELP_SHOWN -1
#define PR_RESULT_SUCCESS 0
#define PR_RESULT_COMPRESS_RESULT_FATAL -26 
#define PR_RESULT_COMPRESS_RESULT_WARNING -27
#define PR_RESULT_WAV_UNSUPPORTED_BITRATE -28
#define PR_RESULT_WAV_UNSUPPORTED_SAMPLERATE -29
#define PR_RESULT_WAV_UNSUPPORTED_STEREO -30
#define PR_RESULT_ERR_BMP_BITRATE_DIFFERS -31
#define RESULT_PAL_UNSUPPORTED -32

#endif
