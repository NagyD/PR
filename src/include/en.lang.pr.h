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
pr.h: Princed Resources : English language strings
¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 18 Feb 2004

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2004-Feb-18)

  Language: English

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                           Text Defines                        |
\***************************************************************/

#define PR_ABOUT                  "Princed resources (PR) " PR_VERSION " " OS "\n" PR_COPY "\n" PR_URL "\n\n"

#define PR_CGI_TEXT1              "Content-Type:text/html\n\nRunning as a cgi\n"
#define PR_CGI_TEXT2              "Result: %02d type\n"

#define PR_TEXT_RESULT            "Result: %s (%d)\n"
#define PR_TEXT_RESULT_ERR        "Result: %d files with errors (%d)\n"
#define PR_TEXT_RESULT_DETECTED   "Result: Detected as %s (%d)\n"

#define PR_TEXT_FILE_NOT_FOUND    "Result: File or directory '%s' not found\n"

#define PR_TEXT_SCANNING_CURRENT  "Scanning DAT files in current directory\n"
#define PR_TEXT_SCANNING_GIVEN    "Scanning DAT files in given directory\n"

#define PR_TEXT_IMPORTING_CURRENT "Importing all valid DAT files from the current directory\n"
#define PR_TEXT_IMPORTING_GIVEN   "Importing all valid files from given directory\n"

#define PR_TEXT_TASK_COMPILE      "Importing '%s' from '%s'\n"
#define PR_TEXT_TASK_CLASSIFY     "Classifying '%s'\n"
#define PR_TEXT_TASK_EXTRACT      "Exporting '%s' to '%s'\n"

#define PR_TEXT_DISK_PROCESSING   "Processing '%s'...\n"
#define PR_TEXT_DISK_REPLACE      "File %s already exists. Replace? [Yes/no/all]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'A'

#define PR_TEXT_IMPORT_NOT_OPEN   "'%s' not open, skipped\n"
#define PR_TEXT_IMPORT_SUCCESS    "'%s' successfully imported\n"
#define PR_TEXT_IMPORT_ERRORS     "'%s' has errors, skipped\n"
#define PR_TEXT_IMPORT_DONE       "Importing done, %d files imported ok, %d files with error\n"
#define PR_TEXT_IMPORT_PLV_WARN   "Warning: PLV file may be corrupt\n"

#define PR_TEXT_EXPORT_WORKING    "'%s' successfully exported\n"
#define PR_TEXT_EXPORT_SUCCESS    "Result: %d files successfully exported (%d)\n"
#define PR_TEXT_EXPORT_ERROR      "'%s' has errors, aborting...\n"
#define PR_TEXT_EXPORT_BMP_WARN   "Warning: Extracted file may be corrupt\n"

#define PR_TEXT_ERROR_ONE_DIR     "You may select one directory to export all DAT files or specify DAT files.\nBoth actions are not allowed.\n"
#define PR_TEXT_ERROR_XML_FILE    "Error, check the XML file\n"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_ERRORS {\
	"Success",\
	NULL, /* help shown */\
	"DAT file not found",\
	"Expected DAT file, but a directory was given",\
	"DAT file reading permission denied",\
	"DAT file writing permission denied",\
	"File not found",\
	"Expected file, but a directory was given",\
	"File reading permission denied",\
	"File writing permission denied",\
	"Invalid DAT file",\
	"Memory error",\
	"XML parsing attribute error",\
	"Cannot open XML file",\
	"XML parsing error",\
	"XML file already open",\
	"XML file cannot be open",\
	"Input line syntax error",\
	"Extraction error",\
	"-19",\
	"You may select one directory to export all DAT files or specify DAT files; both actions are not allowed",\
	"Recursive flag must not be set when you import files",\
	"No files selected", /* should never happen because default is "." */\
	"-23"}


#define PR_TEXT_TYPES {\
	NULL,\
	"Levels file",\
	"Graphic DAT file without palette",\
	"Waves/Digital sound file",\
	"Midis file",\
	"Valid DAT file with undefined content",\
	"Graphic DAT file",\
	"Internal PC Speaker DAT file",\
	NULL,NULL,NULL,\
	"invalid Pop2 DAT file",\
	"Pop2 level file",\
	"Pop2 graphic DAT file",\
	"Pop2 wav DAT file",\
	"Pop2 midi DAT file",\
	"Pop2 valid DAT file",\
	"Pop2 graphic DAT file"}

#define PR_TEXT_IMPORT_ARRAY {\
	"File successfully imported",\
	"DAT file not found or invalid in partial importation",\
	"DAT File couldn't be open",\
	"XML Parsing error",\
	"No memory",\
	"XML Attribute not recognized",\
	"XML File not found"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#define PARSING_RAW "   -w, --raw                  uses raw format\n"

#define PARSING_HELP_BEGIN "Usage: \n\
  pr [OPTIONS] [-x[EXPORTDIR]|-m[IMPORTDIR]|-c] [DATFILEPATH[ ...]]\n\
  pr --help\n\
  pr --version\n\
\n\
   DATFILEPATH syntax is FILE.DAT[@[RESID][:RESINDEX][#ORDER], ...]\n\
\n\
  Mandatory arguments to long options are mandatory for short options too.\n\
\n"
#define PARSING_HELP_PART1 "\
   -i, -m, --import[=DIRNAME] import from DIRNAME into given DAT file\n\
   -c, --classify             return the DAT file type\n\
   -e, -x, --export[=DIRNAME] export given DAT file into DIRNAME\n\
\n"
#define PARSING_HELP_PART2 "\
   -a, --setauthor=NAME       set your name in extracted plv files\n\
   -b, --backup[=EXTENSION]   backup your files\n\
   -f, --force                rewrite your files without prompting\n\
   -g, --cgi                  run as CGI and output mime headers\n\
   -h, -?, --help             display this help and exit\n\
   -s, --resource=RESFILE     use an user-specific resource XML file\n\
"PARSING_SHOWRAW(PARSING_RAW)

#define PARSING_HELP_PART3 "\
   -r, --recursive            searche for all DAT files (only if DATFILEPATH\n\
                              is not a DAT file)\n\
   -t, --datfile=DATFILE      specify a DAT file to read resources different\n\
                              than the original file\n"

#define PARSING_HELP_PART4 "\
   -z, --compression-level=N  a number from 1 to 7 specifying how hard has PR\n\
                              to try to reduce the generated DAT files\n\
                              1 is high speed, low compression, 7 is high\n\
                              compression but low speed\n"

#define PARSING_HELP_PART5 "\
       --format=FORMAT        generate the item files in the unknown file using\n\
                              the given format\n\
       --unknown              generate the unknown file without performing\n\
                              any extraction\n\
   -v, --verbose              explain what is being done\n\
       --version              output version information and exit\n\
       --xml-optimize=XMLFILE optimize a resources XML file\n\
\n"

#define PR_TXT_AUTHORS           "Authors"
#define PR_TXT_CODER             "Coding & main routines"
#define PR_TXT_COD_ASSIST        "Coding assistant"
#define PR_TXT_GFX_COMP          "Graphic compression algorithms"
#define PR_TXT_GFX_DEV           "Graphic format development"
#define PR_TXT_MID               "MID Sound format development"
#define PR_TXT_XML               "Resources.xml edition"
#define PR_TXT_TRANSLATION       ""
#define PR_TXT_ABOUT_TRANSLATOR  ""
