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
parse.c: Princed Resources : XML parsing functions
¯¯¯¯¯¯¯
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

/* Includes */
#include "parse.h"
#include "disk.h"
#include "memory.h"
#include "common.h"
#include <string.h>
#include <stdio.h> /* Included only for XML specific attribute code */
#include <stdlib.h>

#define XML_TAG_CLOSE 1
#define XML_TAG_OPEN 2
#define XML_WAS_CDATA 3
#define XML_WAS_CLOSER 4
#define XML_WAS_EOD 5
#define XML_WASNT_TEXT 6
#define XML_WAS_TAG 7

/****************************************************************\
|                   Tag Tree Handling Functions                  |
\****************************************************************/

const char* getExtDesc(int type) {
	static const char* extarraydesc[]=RES_FILE_TYPES;
	return extarraydesc[type];
}

/* private defines */
#define parse_IsSpace(c) ((c==' ')||(c==9))||(c=='\n')||(c=='\r')
#define parse_IsChar(c) ((('a'<=c)&&(c<='z'))||(('A'<=c)&&(c<='Z'))||(c=='_')||(c=='-')||(c=='?'))

#define parse_Separate while (parse_IsSpace(*i)) i++
#define parse_NextWord(i) while (parse_IsChar(*(i))) (i)++

#define parse_FillAttr(a,b) if (equalsIgnoreCase(attr,b)) { freeAllocation(a); (a)=(val); return 0;}

#define parse_TotalInheritance(attribute) \
		if ((tag->attribute==NULL)&&(father->attribute!=NULL)) \
			tag->attribute=strallocandcopy(father->attribute);

#define parse_Error return PR_RESULT_ERR_XML_PARSING

tTag* getTagStructure() {
	/* initializes */
	tTag* t;
	int i;

	t=(tTag*)malloc(sizeof(tTag));
	if (t==NULL) return NULL;
	for (i=0;i<sizeof(tTag);i++) ((char*)(t))[i]=0; /* I don't trust memset */

/*	t->child=NULL;
	t->next=NULL;
	t->tag=NULL;
	t->desc=NULL;
	t->name=NULL;
	t->path=NULL;
	t->file=NULL;
	t->type=NULL;
	t->palette=NULL;
	t->value=NULL;
	t->index=NULL;
	t->order=NULL;
	t->paletteindex=NULL;
	t->paletteorder=NULL;
	t->paletteoffset=NULL;
	t->version=NULL;
	t->number=NULL;
	t->flags=NULL;*/

	return t;
}

void freeTagStructure(tTag* t) {
	if (t==NULL) return;
	freeTagStructure(t->child);
	freeTagStructure(t->next);
	freeAllocation(t->tag);
	freeAllocation(t->desc);
	freeAllocation(t->path);
	freeAllocation(t->file);
	freeAllocation(t->type);
	freeAllocation(t->name);
	freeAllocation(t->palette);
	freeAllocation(t->value);
	freeAllocation(t->index);
	freeAllocation(t->order);
	freeAllocation(t->paletteindex);
	freeAllocation(t->paletteorder);
	freeAllocation(t->paletteoffset);
	freeAllocation(t->version);
	freeAllocation(t->number);
	freeAllocation(t->flags);
	freeAllocation(t->colors);
	free(t);
}

int parse_attribFill(char* attr,char* val, tTag* t) {
	/*
	 * PR_RESULT_ERR_XML_ATTR  Attribute mismatch
	 * PR_RESULT_SUCCESS       Ok
	 */

	if (equalsIgnoreCase(attr,"?")) {
		free(val);
		return PR_RESULT_SUCCESS;
	}

	parse_FillAttr(t->desc,"desc");
	parse_FillAttr(t->path,"external"); /* external is a path alias for old compatibility */
	parse_FillAttr(t->path,"path");
	parse_FillAttr(t->file,"file");
	parse_FillAttr(t->type,"type");
	parse_FillAttr(t->type,"itemtype"); /* deprecated alias for type */
	parse_FillAttr(t->name,"name");
	parse_FillAttr(t->name,"title"); /* title is a name alias */
	parse_FillAttr(t->palette,"palette");
	parse_FillAttr(t->value,"value");
	parse_FillAttr(t->index,"index");
	parse_FillAttr(t->order,"order");
	parse_FillAttr(t->paletteindex,"paletteindex");
	parse_FillAttr(t->paletteorder,"paletteorder");
	parse_FillAttr(t->paletteoffset,"paletteoffset");
	parse_FillAttr(t->version,"version");
	parse_FillAttr(t->number,"levelnumber"); /* levelnumber is a number alias */
	parse_FillAttr(t->number,"number");
	parse_FillAttr(t->flags,"flags");
	parse_FillAttr(t->colors,"colors");

	return PR_RESULT_ERR_XML_ATTR;
}

/****************************************************************\
|                       XML Parsing Functions                    |
\****************************************************************/

/* Parse text functions */
int parse_parseNext(char** pString, tTag* tag) {
	/*
	 * PR_RESULT_ERR_XML_ATTR    Attribute not recognized
	 * PR_RESULT_ERR_MEMORY      No memory
	 * PR_RESULT_ERR_XML_PARSING Parse error
	 * PR_RESULT_SUCCESS         If continue
	 * XML_TAG_CLOSE             If tag end
	 * XML_TAG_OPEN              If end
	 */

	char* start;
	char* attribute;
	char* value;
	char aux[MAX_VALUE_SIZE];
	int size;
	char* i=*pString;

	parse_Separate;

	if (*i=='>') {
		*pString=i+1;
		return XML_TAG_OPEN;
	}
	if (*i=='/') {
		i++;
		parse_Separate;
		if (*i=='>') {
			*pString=i+1;
			return XML_TAG_CLOSE;
		} else {
			parse_Error;
		}
	}

	start=i;
	parse_NextWord(i);
	if (start==i) parse_Error;
	if (*i==0)    parse_Error;

	if (!(parse_IsSpace(*i)||(*i=='=')||(*i=='>'))) parse_Error;

	size=(long int)i-(long int)start; /* Note: casted to long for portability with 64 bits architectures */
	attribute=(char*)malloc(1+size);
	if (attribute==NULL) return PR_RESULT_ERR_MEMORY;
	memcpy(attribute,start,size);
	attribute[size]=0;

	if (*i=='=') {
		int k=0;
		/* It's a text attribute */
		i++;
		if (*i!='"') parse_Error;
		i++;
		/* Parse until the next " */
		for(start=i; (k<MAX_VALUE_SIZE)&&(*i!='"')&&(*i!=0) ;i++) {
			aux[k]=*i;
			if (aux[k]=='\\') {
				i++;
				switch (*i) {
					case 'n':
						aux[k]='\n';
						break;
					case 'r':
						aux[k]='\r';
						break;
					case 't':
						aux[k]='\t';
						break;
					default:
						aux[k]=*i;
						break;
				}
			}
			k++;
		}
		if ((*i)!='"') {
			free(attribute);
			parse_Error;
		}
		i++;
		value=(char*)malloc(k+1);
		if (value==NULL) {
			free(attribute);
			return PR_RESULT_ERR_MEMORY;
		}
		memcpy(value,aux,k);
		value[k]=0;
	} else {
		/* It's a boolean attribute, I'll define it */
		value=(char*)malloc(1);
		if (value==NULL) {
			free(attribute);
			return PR_RESULT_ERR_MEMORY;
		}
		value[0]=0;
	}

	if (parse_attribFill(attribute,value,tag)) {
		free(attribute);
		free(value);
		return PR_RESULT_ERR_XML_ATTR;
	}
	free(attribute);
	*pString=i;
	return PR_RESULT_SUCCESS;
}

int getNextTag(char** pString, char** value) {
	/*
	 * PR_RESULT_ERR_MEMORY       No memory
	 * PR_RESULT_ERR_XML_PARSING  Parse error
	 * XML_WAS_TAG     if next item is a tag (value allocated)
	 * XML_WAS_CDATA   if it was a text (value allocated)
	 * XML_WAS_CLOSER  if next item closes a tag (value allocated)
	 * XML_WAS_EOD     end of document (value not allocated)
	 * XML_WASNT_TEXT  if there was no text (value not allocated)
	 */

	char* i=*pString;
	int   result;
	char* start;
	int   size;

	parse_Separate;

	if (*i=='<') {
		/* it is a tag */
		i++;
		parse_Separate;
		if (*i=='/') {
			// closing tag
			result=XML_WAS_CLOSER;
			i++;
		} else {
			if ((*i=='!')||(*i=='?')) {
				// It is a comment or processing instruction: skip it.
				while ((*i)&&((*i)!='>')) i++;
				if (!(*i)) parse_Error;
				i++;
				if (!(*i)) return XML_WAS_EOD;
				result=getNextTag(&i,value);
				*pString=i;
				return result;
			} else {
				// opening tag
				result=XML_WAS_TAG;
			}
		}
		start=i;
		parse_NextWord(i);
		if (start==i) parse_Error;
		if (*i==0)    parse_Error;
		// Don't skip over next char, it might be '>'.
		//i++;

		// Copy tag name.
		// size is one bigger than the tag name.
		size=(int)((long int)i-(long int)start)+1; /* Note: casted to long for portability with 64 bits architectures */
		*value=(char*)malloc(size);
		if (*value==NULL) return PR_RESULT_ERR_MEMORY;
		memcpy(*value,start,size-1);
		(*value)[size-1]=0;
		*pString=i-(!result);
		return result;
	}
	// It is not a tag but plain text:
	start=i;
	while ((*i)&&((*i)!='<')) i++;
	if (!(*i)) return XML_WAS_EOD;
	if (start==i) return XML_WASNT_TEXT;
	size=(int)((long int)i-(long int)start); /* Note: casted to long for portability with 64 bits architectures */
	*value=(char*)malloc(1+size);
	if (*value==NULL) return PR_RESULT_ERR_MEMORY;
	memcpy(*value,start,size);
	(*value)[size]=0;
	*pString=i;
	return XML_WAS_CDATA;
}

/* Parse Tree functions */
tTag* parse_makeTree(char** p,char* name, int* error,tTag* father) {
	/* *error
	 * PR_RESULT_ERR_XML_ATTR    Attribute not recognized
	 * PR_RESULT_ERR_MEMORY      No memory
	 * PR_RESULT_ERR_XML_PARSING Parse error
	 * PR_RESULT_SUCCESS         If the tag was parsed successfully
	 */

	tTag* tag;
	tTag* children=NULL;
	char* value;
	int result;

	tag=getTagStructure();
	tag->tag=name;

	while (!((*error)=parse_parseNext(p, tag)));

	if ((*error)<0) {freeTagStructure(tag);return NULL;} /* Fatal error */
	/* (*error) is
	 * XML_TAG_CLOSE  If tag is closed in the same opening
	 * XML_TAG_OPEN   If tag remains open
	 */

	/* In case there are some empty attributes, they may be inherited */
	/* BEGIN specific XML tag inheritance */
	parse_TotalInheritance(palette);
	parse_TotalInheritance(paletteindex);
	parse_TotalInheritance(paletteorder);
	parse_TotalInheritance(paletteoffset);
	parse_TotalInheritance(type);
	parse_TotalInheritance(file);
	parse_TotalInheritance(index);
	parse_TotalInheritance(order);
	parse_TotalInheritance(flags);
	parse_TotalInheritance(colors);
	/* parse_PartialConcatInheritance(tag->path,father->path,tag->value); */
	if ((tag->value==NULL /*is folder */)||(tag->path!=NULL)) {
		char* str;

		/* Make sure paths do exist */
		if (father->path==NULL) {father->path=(char*)malloc(1);*(father->path)=0;}
		if (tag->path==NULL) {tag->path=(char*)malloc(1);*(tag->path)=0;}

		/* Create new variable */
		str=(char*)malloc(strlen(father->path)+strlen(tag->path)+2);
		if (str==NULL) {*error=PR_RESULT_ERR_MEMORY;return NULL;}

		/* Set variable and destroy old variables */
		sprintf(str,"%s/%s",father->path,tag->path);
		free(tag->path);
		if ((*(father->path))==0) {free(father->path);father->path=NULL;}
		tag->path=str;
	}
	/* END specific XML tag inheritance */

	if ((*error)==XML_TAG_CLOSE) {
		*error=PR_RESULT_SUCCESS; /* No errors, end of the tag in the same tag <tag /> */
		return tag;
	}

	/* Parse Child tags */
	while (1) {
		(*error)=getNextTag(p, &value);
		if ((*error)<0) return NULL; /* Fatal error */
		/* (*error)
		 * XML_WAS_CDATA   If it was a text
		 * XML_WAS_CLOSER  If next item closes a tag
		 * XML_WAS_EOD     End of document
		 * XML_WASNT_TEXT  If there was no text
		 * XML_WAS_TAG     If next item is a tag
		 */
		result=(*error);
		switch (result) {
			case XML_WAS_TAG:
				if (children==NULL) {
					tag->child=parse_makeTree(p,value,error,tag);
					children=tag->child;
				} else {
					children->next=parse_makeTree(p,value,error,tag);
					children=children->next;
				}
				if (*error) {freeTagStructure(tag); return NULL;}
				break;
			case XML_WAS_CDATA:
				freeAllocation(tag->name);
				tag->name=value;
				break;
			case XML_WAS_CLOSER:
				/* "no errors" or "a wrong tag is closed" */
				*error=(equalsIgnoreCase(value,tag->tag))?0:PR_RESULT_ERR_XML_PARSING;
				free(value);
				return tag;
			case XML_WAS_EOD:
				*error=PR_RESULT_ERR_XML_PARSING; /* this tag wasn't closed */
				return tag;
				break;
		}
	}
	return NULL;
}

#ifdef IGNOREVERIFYDATTYPES

extern FILE* outputStream;

void showTag(int n,tTag* t) {
	int a;
	tTag* children;

	for(a=0;a<n;a++) fprintf (outputStream,"  ");
	if (t!=NULL) {
		fprintf(outputStream,"%s (%s@%s:%s#%s)\n",t->tag,t->file,t->value,t->index,t->order);
		children=t->child;

		while (children!=NULL) {
			showTag(n+1,children);
			children=children->next;
		}
	} else {
		fprintf(outputStream,"None\n");
	}
}

#endif

tTag* xmlParseFile(const char* vFile,int* error) {
	/* error may take the following values:
	 * PR_RESULT_ERR_XML_PARSING Parse error
	 * PR_RESULT_ERR_MEMORY      No memory
	 * PR_RESULT_ERR_XML_ATTR    Attribute not recognized
	 * PR_RESULT_ERR_XML_FILE    File not found
	 * PR_RESULT_SUCCESS         No errors
	 */

	char* p;
	/*char* l;*/
	char* value=NULL;
	tTag* tag;
	tTag* father;
	/*int size;*/
	tBinary bin;

	bin=mLoadFileArray(vFile);

	if (bin.size<=0) {
		*error=PR_RESULT_ERR_XML_FILE; /* File not open */
		return NULL;
	}
	/* bug fix moved to loadArray */ bin.data[bin.size-1]=0; /* The last character must be an end of line (the > is forbidden) */
	p=(char*)bin.data;

	*error=getNextTag(&p, &value);
	if ((*error)<0) {
		free(bin.data);
		return NULL; /* Fatal error will stop the execution of the parsing */
	}

	father=getTagStructure();
	tag=parse_makeTree(&p,value,error,father);

	if ((*error)<0) {
		freeTagStructure(tag);
		free(bin.data);
		free(father);
		return NULL;
	}

	*error=getNextTag(&p, &value);

	free(bin.data);
	if (*error<0) {
		freeTagStructure(tag);
		free(father);
		return NULL; /* Fatal error will stop the execution of the parsing */
	}
	if (*error==XML_WAS_EOD) { /* XML_WAS_EOD means end of document */
		*error=PR_RESULT_SUCCESS;
		free(father);
		return tag;
	} else {
		freeTagStructure(tag);
		free(father);
		*error=PR_RESULT_ERR_XML_PARSING;
		return NULL;
	}
}

static tTag* xmlStructure=NULL; /* Keeping the parsed file structure in memory will save a lot of time */
static char lastFile[256]="";
static int xmlStructureError=0;

/* cache parsed structure. If null is passed the default name will be used */
int xmlParseStructure(const char* vFile,tTag** structure) {
	/* Resources input XML tree. Private+abstract variable */
	static const char defaultXmlFile[]=RES_XML_RESOURC_XML;

	if (vFile==NULL) vFile=defaultXmlFile;

	/* Generate XML structure if doesn't exist */
	if (strcmp(lastFile,vFile)) {
		/* if the file is different than the cached file */
		freeParsedStructure(&xmlStructure);
		xmlStructure=xmlParseFile(vFile,&xmlStructureError);
		strncpy(lastFile,vFile,256); /* remember the new cached filename */
	}

	if (!xmlStructureError) {
		*structure=xmlStructure;
	} else {
		*structure=NULL;
	}

	return xmlStructureError;
}

void freeXmlCache() {
	freeParsedStructure(&xmlStructure);
	lastFile[0]=0;
}

void freeParsedStructure(tTag** structure) {
	/* Free it only if exists */
	if (*structure!=NULL) freeTagStructure(*structure);
	/* Reinitializes the variable */
	*structure=NULL;
}

/***************************************************************\
|                Resource tree browsing for DLL                 |
\***************************************************************/

tTag* resourceTreeGetNext (tTag* whereAmI) {
	return whereAmI->next;
}

tTag* resourceTreeGetChild(tTag* whereAmI) {
	return whereAmI->child;
}

int   resourceTreeGetInfo (tTag* whereAmI, char** tag, char** desc, char** path, char** file, char** type, char** name, char** palette, char** value, char** version, char** number,char** colors) {
	if (whereAmI==NULL) return 0; /* false */
	*tag=whereAmI->tag;
	*desc=whereAmI->desc;
	*path=whereAmI->path;
	*file=whereAmI->file;
	*type=whereAmI->type;
	*name=whereAmI->name;
	*palette=whereAmI->palette;
	*value=whereAmI->value;
	*version=whereAmI->version;
	*number=whereAmI->number;
	*colors=whereAmI->colors;
	return 1; /* true */
}

