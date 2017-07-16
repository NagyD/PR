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
search.c: Princed Resources : Specific XML handling functions
¯¯¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 23 Oct 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-25)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/
#include <stdio.h> /* only on debugging purposes */

/* Includes */
#include "common.h"
#include "memory.h"
#include "parse.h"
#include "search.h"
#include "translate.h" /* to translate indexes */
#include <string.h>

/***************************************************************\
|            Filtering XML structure to tResourceList           |
\***************************************************************/

/* parse file */
void search_workTag(const tTag* t,void* pass);

int xmlParseFileForResource(const char* vFile, const char* datFile, tResourceList *rlist) {
	/* Declare error variable */
	int error;
	tPassWork pass;
	tTag* structure;

	/* Generate XML structure if doesn't exist */
	if ((error=xmlParseStructure(vFile,&structure))) return error;

	/* Use the XML structure to Generate the resource structure of the file */
	pass.datFile=datFile;
	pass.rlist=rlist;

	xmlRunOverTree(structure,&pass,search_workTag);

	/* All done */
	return PR_RESULT_SUCCESS;
}

/****************************************************************\
|                   Tag Tree Searching Functions                 |
\****************************************************************/

#define search_keepStringAttribute(attribute) res.attribute=strallocandcopy(t->attribute)
#define search_keepIntAttribute(attribute,type) res.attribute=(type)ptoi(t->attribute);
#define search_keepIdAttributes(attribute,idnum,idindex) \
	res.attribute.value=(unsigned short int)ptoi(t->idnum);\
	if (t->idindex) str5lowercpy(res.attribute.index,translateExt2Int(t->idindex));\
	else res.attribute.index[0]=0

#define search_keepIdAttributesElse(attribute,idnum,idindex,idelse) \
	if (t->idnum && !strcmp(t->idnum,"monochrome")) res.attribute.value=-1;\
	else res.attribute.value=(unsigned short int)ptoi(t->idnum);\
	if (t->idindex) str5lowercpy(res.attribute.index,translateExt2Int(t->idindex));\
	else str5lowercpy(res.attribute.index,t->idelse)

void search_workTag(const tTag* t,void* pass) {
	/*
	 * If the tag matches, it is converted to resource and added to the array
	*/

	/* Declare variables */
	const char* datFile=((tPassWork*)pass)->datFile;
	tResourceList* rlist=((tPassWork*)pass)->rlist;
	tResource res;
	char* end;

/*printf("comienzan las preguntas (t->file,datFile)=(%s,%s)\n",t->file,datFile);
printf("tv=%s ti=%s tag=%s\n",t->value,t->index,t->tag);*/
	/* Skipping conditions */
	if (!equalsIgnoreCase(t->file,datFile))   return; /* If it doesn't belong to the given DAT file */
	if (!t->value) return;                            /* If there was not number id */
	if (!t->index) return;                            /* If there was not index id */
	if (!equalsIgnoreCase(t->tag,"item"))     return; /* If the tag isn't an item */
/*printf("terminan las preguntas\n");*/

	/* Process tag and copy values to resource: */

	/* Get string type and convert into the typeId */
	res.type=0;
#ifndef IGNORERESOURCEFUNCTIONS
	if (t->type!=NULL) { /* NULL tells the extractor that the type should be auto detected */
		int i=RES_FILE_TYPES_COUNT;
		while((!res.type)&&(i--)){
			if (equalsIgnoreCase(t->type,getExtDesc(i)))
				res.type=i;
		}
		/* If error it returns 0 and the verifyHeader will try to detect the type */
		if(res.type==0) printf("Unknown resource type in XML: \"%s\"\n",t->type);
	}
	if (res.type==eResTypeImage16) {
		switch (ptoi(t->colors)) {
		case 2:
			res.type=eResTypeImage2;
			break;										
		case 256:
			res.type=eResTypeImage256;
			break;										
		case 16:
		default:
			res.type=eResTypeImage16;
			break;	
		}
	}
#endif

	/* Get the order */
	res.id.order=getOrder(t->order);
	res.paletteId.order=getOrder(t->paletteorder);

	/* Copy id and palette id */
	search_keepIdAttributes(id,value,index);
	search_keepIdAttributesElse(paletteId,palette,paletteindex,index);
	res.paletteoffset = ptoi(t->paletteoffset);

	/* Copy number, title, desc and path */
	search_keepIntAttribute(number,unsigned char); /* Transforms the char* levelnumer/number attribute into a char value, if error, demo level is used */
	if (t->flags) {
		res.flags=strtol(t->flags,&end,0);
		if (*end) return;
	} else {
		res.flags=0;
	}
	search_keepStringAttribute(desc);  /* only for plv */
	search_keepStringAttribute(name);  /* only for plv */
	search_keepStringAttribute(path);

	resourceListAdd(rlist,&res);
}

void xmlRunOverTree(const tTag* t,void* pass, void (*function)(const tTag*,void*)) {
	/*
		Runs the given function for each matching tag
	*/
	tTag* children;

	if (t!=NULL) {
		if (t->file!=NULL) (*function)(t,pass);

		children=t->child;

		while (children!=NULL) {
			xmlRunOverTree(children,pass,function);
			children=children->next;
		}
	}
}

/****************************************************************\
|                       Compare two XML files                    |
\****************************************************************/

#ifdef COMPILE_WITH_COMPARISION

static int compareStatisticsOk=0;
static int compareStatisticsTotals=0;
static int compareStatisticsWarnings=0;
extern FILE* outputStream;

const tTag* search_searchTree(const tTag* t,const char* datFile, const char* id) {
	/* tTag*
	 * tag pointer if found
	 * NULL if not found
	 */
	tTag* children;
	const tTag* result;

	if (t!=NULL) {
		if (((t->file)!=NULL)&&((t->value)!=NULL)) {
			if ((equalsIgnoreCase(t->file,datFile))&&(equalsIgnoreCase(t->value,id))) return t;
		}
		children=t->child;

		while (children!=NULL) {
			if (NULL!=(result=search_searchTree(children,datFile,id))) {
				return result;
			}
			children=children->next;
		}
	}
	return NULL;
}

void search_compareForeach(const tTag* tag,void* pass) {
	const tTag* modified;
	const tTag* result;

	modified=((tPassCompare*)pass)->tag;

	if ((tag->file)&&(tag->value)) {
		result=search_searchTree(modified,tag->file,tag->value);
		if (!result) {
			fprintf(outputStream,"Error: Item not found: '%s@%s'\n",tag->value,tag->file);
			fprintf(outputStream,"-> <item value=\"%s\" path=\"%s\" type=\"%s\" palette=\"%s\">%s</item>\n",
				tag->value,
				tag->path,
				tag->type,
				tag->palette,
				tag->desc
			);
		} else {
			fprintf(outputStream,"Item found: '%s@%s' %s\n",tag->value,tag->file,result->file);
			if (!equalsIgnoreCase(tag->type,result->type)) {
				compareStatisticsWarnings++;
				fprintf(outputStream,"Warning: Type mismatch in '%s@%s' (%s!=%s)\n",tag->value,tag->file,tag->type,result->type);
			}
			compareStatisticsOk++;
		}
		compareStatisticsTotals++;
	}
}

void xmlCompareFiles(tTag* modified,tTag* original) {
	tPassCompare pass;
	pass.tag=modified;

	xmlRunOverTree(original,&pass,search_compareForeach);
	fprintf(outputStream,"Statistics:\n Totals: %d\n Working: %d (%5.2f%%)\n Warnings: %d\n Missing: %d (%5.2f%%)\n",
		compareStatisticsTotals,
		compareStatisticsOk,(float)(100*(float)compareStatisticsOk/compareStatisticsTotals),
		compareStatisticsWarnings,
		compareStatisticsTotals-compareStatisticsOk,(float)(100*(float)(compareStatisticsTotals-compareStatisticsOk)/compareStatisticsTotals)
	);
}

#endif

