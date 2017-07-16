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
tree.c: Princed Resources : Specific XML tree handling routines
¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: 28 Dec 2005

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2003-Oct-23)
  Version: 1.50 (2005-Dec-19)

 Modified by: David from Princed

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

/* Includes */
#include "common.h"
#include "list.h" /* list primitives needed by the Common Factor routines */
#include "memory.h"
#include "parse.h" /* getTagStructure */
#include "unknown.h" /* typedef tUnknownFile */
#include <stdio.h>
#include <string.h>

/***************************************************************\
|                     XML generation defines                    |
\***************************************************************/

#define XML_HEADER \
	"<!DOCTYPE resources SYSTEM \"http://www.princed.org/standards/xml/resources/std1.dtd\">\n"\
	"<?xml version=\"1.0\" ?>\n"

/***************************************************************\
|              Common factor tree reducing routines             |
\***************************************************************/

/*
 * Affected attributes: only fully inheritable
 * PRE: inheritances are shown in the tree, so if the parent has
 * file="a", then the son comes with file="a" except that other
 * file was explicitly specified for the child. NULL is never shown
 * after a non-NULL parent.
 *
 * POST: if the folder has n children and there are n*2/3 equal attributes
 * then those attributes comes to the parent.
 *
 * if the folder has n children and there are at most 6/n different attributes
 * we can say that there is a ratio of 6 items per attribute or more.
 * If that happens for at least one attribute, the attribute with the highest
 * ratio will be partitioned that way:
 *   if an attribute value is present in 3 or more items, all items go
 *   together under a new folder with this item set.
 */

#define TREE_PART_ALLOWED_CHILDREN_DEN   3
#define TREE_PART_ALLOWED_CHILDREN_NUM   2
#define TREE_PART_ALLOWED_DIFFERENT_ATTR 4
#define TREE_PART_ALLOWED_MIN_ATTR       2
#define TREE_PART_ALLOWED_RATIO          6

/* TODO
 * Fix sigfaults
 - Check memory releasing
 - Calculate the optimum values for the constants
 * transform numbers into defines
 - handle the NULL attribute numbers
 - invert the insertion order orphan
 */

/* private type */
typedef struct {
	const char* attr;
	int count;
	tTag* relatedTag;
}tAttrCount;

/* private functions */
int tree_attrcmp(const void* x,const void* y) {
	register const tAttrCount* a=x;
	register const tAttrCount* b=y;
	/* the index has the priority */
	int c=strcmp(a->attr,b->attr);
	if (c>0) return GT;
	if (c<0) return LT;
	return EQ;
}

void tree_increaseList(const char* attr,tList* l) {
	tAttrCount a;
	tAttrCount* aux;

	a.attr=attr;
	a.count=1; /* if it appeared for the first time */
	if (list_moveCursor(l,&a)) {
		aux=list_getCursor(l);
		aux->count++;
	} else {
		list_insert(l,&a);
	}
}

#define attributeCount 8
/* this is the most ugly thing I've ever made... nevertheless it was the only way to make it abstract */
#define tree_bindAttr(name,i) attrInfo[i].offset=(long)(&(parent->name))-(long)(parent)
#define tree_getAttr(tag) (*( (char**)((long)(tag)+attrInfo[i].offset) ))

void tree_TagCommonFactor(tTag* parent) {
	tTag* child;
	tAttrCount* a;
	int totalItems, totalAttributes, partitionate, maxCount, i;
	float maxRatio;
	const char* result;
	struct attributeInfo {
		int   countAll;
		int   countNull;
		tList valueList;
		long  offset;
	} attrInfo[attributeCount];
	if (!parent->child) return; /* avoid a full cycle for foils */

	tree_bindAttr(palette,0);
	tree_bindAttr(paletteindex,1);
	tree_bindAttr(paletteorder,2);
	tree_bindAttr(type,3);
	tree_bindAttr(file,4);
	tree_bindAttr(index,5);
	tree_bindAttr(order,6);
	tree_bindAttr(flags,7);

	/* initialize counting list */
	for (i=0;i<attributeCount;i++) {
		attrInfo[i].valueList=list_create(sizeof(tAttrCount),tree_attrcmp,NULL);
		attrInfo[i].countAll=0;
		attrInfo[i].countNull=0;
	}

	/* count the attributes */
	for (child=parent->child;child;child=child->next) {
		for (i=0;i<attributeCount;i++) {
			if (tree_getAttr(child)) { /* if not attribute, do nothing */
				tree_increaseList(tree_getAttr(child),&(attrInfo[i].valueList));
			} else {
				attrInfo[i].countNull++;
			}
			attrInfo[i].countAll++;
		}
	}

	maxRatio=0;
	partitionate=-1;
	for (i=0;i<attributeCount;i++) {
		totalItems=0;
		totalAttributes=0;
		maxCount=0;
		result=NULL;

		/* for each possible attribute value check if it is possible to move it upper */
		list_firstCursor(&(attrInfo[i].valueList));
		while ((a=list_getCursor(&(attrInfo[i].valueList)))) {
			totalItems+=a->count;
			totalAttributes++;
			if (
					a->count*TREE_PART_ALLOWED_CHILDREN_DEN >
					attrInfo[i].countAll*TREE_PART_ALLOWED_CHILDREN_NUM
				&&
					a->count>maxCount
			) {
				maxCount=a->count;
				result=a->attr;
			}
			list_nextCursor(&(attrInfo[i].valueList));
		}

		if (result) { /* it is possible to move one attribute upper (result) */
			if (tree_getAttr(parent)!=result) { /* it is possible, and is the most probable case, that the parent was already the best choice. In that case, do nothing */
				freeAllocation(tree_getAttr(parent));
				tree_getAttr(parent)=strallocandcopy(result); /* result is copied to avoid checking each time a string is released below */
			}
			for (child=parent->child;child;child=child->next) {
				if (tree_getAttr(child)&&!strcmp(tree_getAttr(child),tree_getAttr(parent))) {
					freeAllocation(tree_getAttr(child)); /* the first time this is ran, result is released! */
					tree_getAttr(child)=NULL;
				}
			}
		} else { /* attribute couldn't be moved up, so let's try to move it down partitionating the folder */
			if (totalAttributes) {
				register float ratio=totalItems/totalAttributes;
				if (ratio>TREE_PART_ALLOWED_RATIO && ratio>maxRatio && totalAttributes>=TREE_PART_ALLOWED_MIN_ATTR) {
					maxRatio=ratio;
					partitionate=i;
/*printf("result=NO but partition is possible: maxRatio=%f totalItems=%d totalAttributes=%d i=%d\n",maxRatio,totalItems,totalAttributes,i);*/
				}
			}
/*else printf("result=NO maxRatio=%f totalItems=%d totalAttributes=%d i=%d\n",maxRatio,totalItems,totalAttributes,i);*/
		}
	}

	/* in case we need to make a partition by one attribute (the one with the max ratio) */
	if ((i=partitionate)!=-1) {
		/* separate the parent and the children */
		tTag* orphans=parent->child;
		parent->child=NULL;

/*printf("here it comes p=%d\n",partitionate);*/
		/* initialize all relatedTags and creates folders if necessary */
		list_firstCursor(&(attrInfo[i].valueList));
		while ((a=list_getCursor(&(attrInfo[i].valueList)))) {
			if (a->count>=TREE_PART_ALLOWED_DIFFERENT_ATTR) {
				int l;
/*printf("partitioned tag=%s %p\n",a->attr,a);*/
				/* create a new folder */
				a->relatedTag=getTagStructure();
				a->relatedTag->tag=strallocandcopy("folder");
				l=strlen(parent->path);
				a->relatedTag->path=malloc(l+2);
				strncpy(a->relatedTag->path,parent->path,l);
				a->relatedTag->path[l]='/';
				a->relatedTag->path[l+1]=0;
				tree_getAttr(a->relatedTag)=(char*)a->attr; /* it's just a type problem, I won't edit it */

				/* intercalate as the first of the child folders */
				a->relatedTag->next=parent->child;
				parent->child=a->relatedTag;

			} else {
				/* the original parent will be kept as the parent folder */
				a->relatedTag=parent;
/*printf("not partitioned tag=%s\n",a->attr);*/
			}
			list_nextCursor(&(attrInfo[i].valueList));
		}
/*printf("done, now checking orphans (orphan=%p)\n",orphans);*/

		/* for each orphan */
		while (orphans) {
			tTag* aux;
			tAttrCount info;
/*printf("orphan={%s}\n",tree_getAttr(orphans));*/

			/* search the related tag to this attribute */
			if (tree_getAttr(orphans)) {
				info.attr=tree_getAttr(orphans);
				list_moveCursor(&(attrInfo[i].valueList),&info);
				a=list_getCursor(&(attrInfo[i].valueList));
			} else {
				/* the NULL case is handled here */
				info.attr=NULL;
				info.count=1;
				info.relatedTag=parent;
				a=&info;
			}

			/* free the attribute in process */
/*printf("orphan for a={%s,%d}\n",a->attr,a->count);*/
			if (a->count>=TREE_PART_ALLOWED_DIFFERENT_ATTR) { /* of course only if moved down */
				char* aux2;

				/* free repeated attribute (only if not on the list) */
				if (a->attr!=tree_getAttr(orphans)) freeAllocation(tree_getAttr(orphans)); /* as I said, I won't edit it */
				tree_getAttr(orphans)=NULL;

				/* add a / at the beginning of the path */
				if ((aux2=orphans->path)) {
					int ik;
					ik=strlen(aux2);
					orphans->path=malloc(ik+2);
					while ((--ik)&&aux2[ik]!='/');
					if (ik) strncpy(orphans->path,aux2,ik);
					        strcpy (orphans->path+ik+1,aux2+ik);
					orphans->path[ik]='/';
					free(aux2);
				}
			}

			/* insert the orphan into the tree in the first place */
			aux=orphans->next;
			orphans->next=a->relatedTag->child;
			a->relatedTag->child=orphans;
/*printf("partitioned tag=%s a=%p rt=%p aux=%p of=%p\n",a->attr,a,a->relatedTag,aux,orphans);*/

			/* increase iterator */
			orphans= /* old orphans->next */ aux;
		}
	}

	/* finally, free all lists */
	for (i=0;i<attributeCount;i++) {
		list_drop(&(attrInfo[i].valueList));
	}
}

/* public function */
void xmlOptimizeCommonFactor(tTag* tag) {
	if (tag) {
		xmlOptimizeCommonFactor(tag->next);
		xmlOptimizeCommonFactor(tag->child);
		tree_TagCommonFactor(tag); /* post order */
	}
}

#ifdef DEBUG_CF
void tree_test() {
	tTag tr[100];
	int i;
	for (i=0;i<6800;i++) ((char*)tr)[i]=0;

	tr[0].countAllhild=&(tr[1]);

	tr[1].countNullext =&(tr[2]);
	tr[1].countAllhild=&(tr[6]);
	tr[2].countNullext =&(tr[3]);
	tr[3].countNullext =&(tr[4]);
	tr[4].countNullext =&(tr[5]);
	tr[5].countNullext =NULL;

	tr[6].countNullext =&(tr[7]);
	tr[7].countNullext =&(tr[8]);
	tr[8].countNullext =&(tr[9]);
	tr[8].countAllhild=&(tr[11]);
	tr[9].countNullext =&(tr[10]);
	tr[10].countNullext =NULL;

	tr[11].countNullext =&(tr[12]);
	tr[12].countNullext =&(tr[13]);
	tr[13].countNullext =&(tr[14]);
	tr[14].countAllhild=&(tr[15]);
	tr[15].countNullext =&(tr[16]);
	tr[16].countNullext =NULL;

	tr[0].palette=strallocandcopy("joj0");
	tr[1].palette=strallocandcopy("joj1");
	tr[2].palette=strallocandcopy("joj0");
	tr[3].palette=strallocandcopy("joj1");
	tr[4].palette=strallocandcopy("joj0");
	tr[5].palette=strallocandcopy("joj1");
	tr[6].palette=strallocandcopy("joj1");
	tr[7].palette=strallocandcopy("joj0");
	tr[8].palette=strallocandcopy("joj0");
	tr[9].palette=strallocandcopy("joj1");
	tr[10].palette=strallocandcopy("joj11");
	tr[11].palette=strallocandcopy("joj11");
	tr[12].palette=strallocandcopy("joj10");
	tr[13].palette=strallocandcopy("joj10");
	tr[14].palette=strallocandcopy("joj11");
	tr[15].palette=strallocandcopy("joj11");
	tr[16].palette=strallocandcopy("joj11");

	treeXmlGenerate(0,tr,stdout);

	xmlOptimizeCommonFactor(tr);

	treeXmlGenerate(0,tr,stdout);

}
#endif

/***************************************************************\
|                     Tag generation routines                   |
\***************************************************************/

/*
 * Simple routines to create new tags from the given attributes. They are
 * also linked to a tree using a tTreeStatus abstract type.
 */

void treeStatusFolder(const char* path, const char* file, int palette, const char* paletteindex, tTreeStatus* status) {
	char number[10];
	tTag* folder=getTagStructure();

	sprintf(number,"%d",palette);
	folder->tag=strallocandcopy("folder");
	folder->name=strallocandcopy("unknown");
	folder->path=strallocandcopy(path);
	folder->file=strallocandcopy(file);
	folder->palette=strallocandcopy(number);
	folder->paletteindex=strallocandcopy(paletteindex);

	if (!status->folderFirst) {
		status->folderFirst=folder;
	} else {
		status->folderCursor->next=folder;
	}
	status->folderCursor=folder;
	status->itemCursor=NULL;
}

void treeStatusItem(int value,const char* index,const char* path,const char* type,unsigned long int flags,const char* typedesc,int count, int order, tTreeStatus* status) {
	char aux[100];
	tTag* item=getTagStructure();

	item->tag=strallocandcopy("item");
	sprintf(aux,"%d",value);
	item->value=strallocandcopy(aux);
	sprintf(aux,"%d",order);
	item->order=strallocandcopy(aux);
	item->index=strallocandcopy(index);
	item->path=strallocandcopy(path);
	item->type=strallocandcopy(type);
	sprintf(aux,"0x%lx",flags);
	item->flags=strallocandcopy(aux);
	sprintf(aux,"Unknown %s %d",typedesc, count);
	item->desc=strallocandcopy(aux);

	if (!status->itemCursor) {
		status->folderCursor->child=item;
	} else {
		status->itemCursor->next=item;
	}
	status->itemCursor=item;
}

/***************************************************************\
|                       Memory tree --> XML                     |
\***************************************************************/

/*
 * Given a tTag* with a tree, it will be dumped into an XML file.
 * Empty tags are collapsed and ends with /
 * Each non-empty tags that only has text inside are put in one line,
 * the rest of the non-empty tags are expanded using a tab indentation
 * and the text is dumped in the "name" attribute.
 * A doctype is added at the beginning of the file.
 */

void treeXmlGenerate(int n,const tTag* t,FILE* outputStream) {
	int a;
	tTag* children;

	if (!n) fprintf(outputStream,XML_HEADER);
	for(a=0;a<n;a++) fprintf (outputStream,"\t");
	if (t!=NULL) {
		fprintf(outputStream,"<%s",t->tag);

#define tree_FillAttr(a,b) if (a) fprintf(outputStream," %s=\"%s\"",b,a)

	if (t->child) tree_FillAttr(t->desc,"name");
	tree_FillAttr(t->path,"path");
	tree_FillAttr(t->file,"file");
	tree_FillAttr(t->type,"type");
	tree_FillAttr(t->desc,"desc");
	tree_FillAttr(t->palette,"palette");
	tree_FillAttr(t->value,"value");
	tree_FillAttr(t->index,"index");
	tree_FillAttr(t->order,"order");
	tree_FillAttr(t->paletteindex,"paletteindex");
	tree_FillAttr(t->paletteorder,"paletteorder");
	tree_FillAttr(t->version,"version");
	tree_FillAttr(t->number,"number");
	tree_FillAttr(t->flags,"flags");

#undef FillAttr

		if ((children=t->child)) {
			fprintf(outputStream,">\n");
			while (children!=NULL) {
				treeXmlGenerate(n+1,children,outputStream);
				children=children->next;
			}
			for(a=0;a<n;a++) fprintf (outputStream,"\t");
			fprintf(outputStream,"</%s>\n\n",t->tag);
		} else {
			if (t->name) {
				fprintf(outputStream,">%s</%s>\n",t->name,t->tag);
			} else {
				fprintf(outputStream," />\n");
			}
		}
	}
}

int xmlGenerateFile(const char* vFile,const tTag* t) {
	FILE* fd;
	if (!(fd=fopen(vFile,"wb"))) return PR_RESULT_ERR_XML_NOT_OPEN;
	treeXmlGenerate(0,t,fd);
	fclose(fd); /* by David */
	return PR_RESULT_SUCCESS;
}

/***************************************************************\
|                File attribute deletion routines               |
\***************************************************************/

/* Given a tTag* tree, all items and folders containing a specific file
 * are erased from the tree. In case some items depends on affected folders
 * they are moved up until this affection is gone.
 * This process is recursive and checks all the tree.
 */

void tree_rec(tTag* *t,const char* file) {
	tTag* aux=*t;
	tTag* aux2;

	if (*t && (*t)->file) {
		if (equalsIgnoreCase((aux->file),file)) {
			if (aux->child) {
				*t=aux->child; /* the children are now replacing his parent */
				if ((*t)->next) {
					for(aux2=*t;aux2->next;aux2=aux2->next); /* go to the last child */
					aux2->next=aux->next; /* and set the next sibling as the parent next sibling */
				}
				aux->child=NULL;
			} else {
				*t=aux->next;
			}
			aux->next=NULL;
			if (*t) tree_rec(t,file);
		} else {
			if ((*t) && (*t)->next) tree_rec(&((*t)->next),file);
		}
	}

	if ((*t) && (*t)->child) tree_rec(&((*t)->child),file);
}

void treeDeleteFile(const char* file, tTag* tree) {
	if (tree)
		tree_rec(&(tree->child),file);
}

/***************************************************************\
|                  Inheritance fixing routines                  |
\***************************************************************/

/*
 * Given a tTag* tree, all inheritable attributes that are bellow are dropped
 * to let the inheritance act and save memory.
 * The path attribute is re-contracted to let the inheritance act again.
 * This is very useful before dumping to a file because the file parse will
 * redo this inheritances.
 */

#define tree_TotalInheritance(a) if (parent->a&&child->a&&equalsIgnoreCase(parent->a,child->a)) {freeAllocation(child->a);child->a=NULL;}

void tree_rec_fix(tTag* parent,tTag* child) {
	if (child->next) tree_rec_fix(parent,child->next);
	if (child->child) tree_rec_fix(child,child->child);

	if (parent) {
		tree_TotalInheritance(palette);
		tree_TotalInheritance(paletteindex);
		tree_TotalInheritance(paletteorder);
		tree_TotalInheritance(type);
		tree_TotalInheritance(file);
		tree_TotalInheritance(index);
		tree_TotalInheritance(order);
		tree_TotalInheritance(flags);

		/* partial */
		if ((parent->path!=NULL)&&(child->path!=NULL)) {
			char *p,*c,*aux;

			for (p=parent->path,c=child->path;*p&&*c&&*p==*c;p++,c++);
			if (*c=='/') c++;
			aux=strallocandcopy(c);
			free(child->path);
			child->path=aux;
			if (child->path&&!child->path[0]) { /* drop empty paths */
				free(child->path);
				child->path=NULL;
			}
		}
	}
}

void resourceTreeFixInheritances(tTag* *tree) {
	if (*tree) {
		tree_rec_fix(NULL,(*tree));

		freeAllocation((*tree)->path);
		(*tree)->path=NULL;
	}
}
