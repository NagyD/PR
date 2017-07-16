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
main.c: PR console program parsing and IDE
¯¯¯¯¯¯
 Copyright 2004 Princed Development Team
  Created: See CVS

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00

 Note:
  DO NOT remove this copyright notice
*/

#ifdef UNIX
 #include <unistd.h>
 #ifndef LINUX
  #include "getopt.h"
 #endif
#else
 #include "getopt.h"
#endif

#include "common.h"
#include <string.h>
#include "memory.h"
#include "filedir.h"
#include "disk.h" /* MAX_FILENAME_SIZE */
#include "compress.h" /* setCompressionLevel */
#include "parse.h" /* free parsed cache */

FILE* outputStream;

void main_syntax() {
	fprintf(outputStream,PARSING_HELP_BEGIN);
	fprintf(outputStream,PARSING_HELP_PART1);
	fprintf(outputStream,PARSING_HELP_PART2);
	fprintf(outputStream,PARSING_HELP_PART3);
	fprintf(outputStream,PARSING_HELP_PART4);
	fprintf(outputStream,PARSING_HELP_PART5);
}

/***************************************************************\
|      Standard executable command line parsing function        |
\***************************************************************/

int main (int argc, char **argv) {
	/* declare variables */
	char* dirName          =NULL;
	char* dirNameDef            ;
	char* datAuthor        =NULL;
	char* datFileName      =NULL;
	char* datfile          =NULL;
	char* extension        =NULL;
	char* format           =NULL;
	char* resFile          =NULL;
	char* optimizeXmlFile  =NULL;
	char* file;
	char* errors[]         =PR_TEXT_ERRORS;
	char* types[]          =PR_TEXT_TYPES;
	int   c,result=0;
	int   optionflag=0;

	/* Parse command line options */
	do {
		static struct option long_options[] = PARSING_OPTIONS;
		int junk = 0;

		c = getopt_long(argc,argv,PARSING_CHARS,long_options,&junk);
		switch (c) {
				case 'm':
				case 'i':
					if (hasFlag(classify_flag|export_flag)) setFlag(help_flag);
					setFlag(import_flag);
					dirName=strallocandcopy(optarg);
					break;
				case 'g':
					setFlag(cgi_flag); /* if cgi, a classify must be performed */
				case 'c':
					if (hasFlag(import_flag|export_flag)) setFlag(help_flag);
					setFlag(classify_flag);
					break;
				case 2:
					setFlag(unknown_flag);
				case 'x':
				case 'e':
					if (hasFlag(classify_flag|import_flag)) setFlag(help_flag);
					setFlag(export_flag);
					dirName=strallocandcopy(optarg);
					break;
				case 'b':
					setFlag(backup_flag);
					freeAllocation(extension);
					extension=strallocandcopy(optarg);
					break;
				case 'F':
					freeAllocation(format);
					format=strallocandcopy(optarg);
					break;
				case 'f':
					setFlag(force_flag);
					break;
				case 's':
					freeAllocation(resFile);
					resFile=strallocandcopy(optarg);
					break;
				case 'z':
					if (optarg) setCompressionLevel(atoi(optarg));
					break;
#ifndef PR_IGNORE_RAW_OPTION
				case 'w':
					setFlag(raw_flag);
					break;
#endif
				case 'r':
				case 'R':
					setFlag(recursive_flag);
					break;
				case 't':
					freeAllocation(datFileName);
					datFileName=strallocandcopy(optarg);
					break;
				case 'a':
					freeAllocation(datAuthor);
					datAuthor=strallocandcopy(optarg);
					break;
				case 'v':
					setFlag(verbose_flag);
				case -1:
					break;
				case 3:
					freeAllocation(optimizeXmlFile);
					optimizeXmlFile=strallocandcopy(optarg);
					if (!optimizeXmlFile) setFlag(help_flag);
					break;
				case 1:
					setFlag(version_flag);
				default:
					setFlag(help_flag);
		}
	} while (c!=-1);

	outputStream=stdout;

	/* At least one of these options must be selected, if not, the user needs help! */
	if (!(optimizeXmlFile||hasFlag(import_flag|export_flag|classify_flag))) setFlag(help_flag);

	/* optimizeXmlFile is incompatible with the 3 main options, perform the check  */
	if (optimizeXmlFile&&(hasFlag(import_flag|export_flag|classify_flag))) setFlag(help_flag);

	/* Show cgi and about stuff */
	if (hasFlag(cgi_flag)) fprintf(outputStream,PR_CGI_TEXT1);
	fprintf(outputStream,PR_ABOUT);

	/* Show version screen if requested */
	if (hasFlag(version_flag)) {
		fprintf(outputStream,PARSING_ABOUT);
		return PR_RESULT_HELP_SHOWN;
	}

	if (hasFlag(help_flag)) {
		main_syntax();
	} else if (optimizeXmlFile) { /* special case optimize */
		tTag* tree;
		tree=xmlParseFile(optimizeXmlFile,&result);
		if (result==PR_RESULT_SUCCESS) {
			if (tree) xmlOptimizeCommonFactor(tree->child);
			resourceTreeFixInheritances(&tree);
			result=xmlGenerateFile(optimizeXmlFile,tree);
			freeTagStructure(tree);
		}
		fprintf(outputStream,PR_TEXT_RESULT,errors[-result],result);
	} else {
		tFileDir2 files;
		tFileDir2 input;

		fileDirClearOptions(&input);
		dirNameDef=dirName?dirName:".";
		if (optind < argc) {
			while (optind < argc)
				fileDirAddOption(&input,argv[optind++]);
		} else {
			fileDirAddOption(&input,".");
		}
		result=fileDirGetFiles(&input,&files,!hasFlag(import_flag),!hasFlag(recursive_flag),resFile,datFileName!=NULL);

		if (result==PR_RESULT_SUCCESS) {
			char  unknownFile[MAX_FILENAME_SIZE];
			if (hasFlag(export_flag)) {
				sprintf(unknownFile,"%s/" RES_XML_UNKNOWN_PATH "/" RES_XML_UNKNOWN_XML,dirNameDef);
				unknownLogStart(unknownFile,optionflag,extension);
			}
			while ((file=fileDirGetFile(&files,&datfile))) {
				const char* dat;
				if (datFileName)
					dat=datFileName;
				else
					dat=datfile;
				/* Call PR */
				if (hasFlag(import_flag)) {
					/* import */
					fprintf(outputStream,PR_TEXT_TASK_COMPILE,file,dirNameDef);
					result=prImportDatOpt(file,dirNameDef,resFile,optionflag,dat,extension);
					if (result>0) {
						fprintf(outputStream,PR_TEXT_RESULT_ERR,result,result);
					} else {
						fprintf(outputStream,PR_TEXT_RESULT,errors[-result],result);
					}
				} else if (hasFlag(export_flag)) {
					/* export */
					fprintf(outputStream,PR_TEXT_TASK_EXTRACT,file,dirNameDef);
					result=prExportDatOpt(file,dirNameDef,resFile,optionflag,dat,datAuthor,extension,format);
					if (result>0) {
						fprintf(outputStream,PR_TEXT_EXPORT_SUCCESS,result,result);
					} else {
						fprintf(outputStream,PR_TEXT_RESULT,errors[-result],result);
					}
				} else {
					/* classify */
					fprintf(outputStream,PR_TEXT_TASK_CLASSIFY,file);
					result=prClassifyDat(file);
					if (result>0) {
						fprintf(outputStream,PR_TEXT_RESULT_DETECTED,types[result],result);
					} else {
						fprintf(outputStream,PR_TEXT_RESULT,errors[-result],result);
					}
				}
				free(file);
				free(datfile);
			}
			fprintf(outputStream,"\n");
			if (hasFlag(export_flag)) unknownLogStop();
		} else {
			fprintf(outputStream,PR_TEXT_RESULT,errors[-result],result);
		}
	}

	freeXmlCache();
	freeAllocation(datAuthor);
	freeAllocation(datFileName);
	freeAllocation(extension);
	freeAllocation(optimizeXmlFile);
	freeAllocation(resFile);
	freeAllocation(dirName);

	return result;
}

