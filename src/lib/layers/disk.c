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
disk.c: Princed Resources : Disk Access & File handling functions
¯¯¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 29 Oct 2003

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00 (2003-Oct-29)

  Modified by: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.10 (2003-Dec-03)
  Modified by: Santiago Zamora <drkzight@users.sourceforge.net>
  Version: 1.20 (2004-Jan-06)

 Note:
  DO NOT remove this copyright notice
*/

/* Defines */
#include "memory.h" /* strallocandcopy */
#include <string.h>
#include "disk.h"
#ifndef WIN32
#define UNIX
#endif

#include <sys/types.h>
#include <sys/stat.h>

#ifndef DISK_ALLWAYS_FORCE
#include "common.h" /* language texts */
#endif

#ifndef WIN32
#define defmkdir(a) mkdir (a,(mode_t)0755)
	/* Recursive directory scanning needs <dirent> for POSIX or "direntwin" for windows */
#ifdef DISK_DIR_SCANING
#include <dirent.h>
#endif
	/* Terminal manipulation for UNIX (to avoid the enter after selecting an option) */
#ifdef DISK_TERM_MANIPULATION
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#define osIndepGetCharacter() getchar()
#else
#include <direct.h> /* mkdir */
#define defmkdir(a) mkdir (a)
#ifdef DISK_DIR_SCANING
//#include "direntwin.h"
#include <dirent.h>
#endif
#include <conio.h> /* getche */
#define osIndepGetCharacter() getche()
#endif

#ifndef DISK_ALLWAYS_FORCE
extern FILE* outputStream;
#endif

typedef struct tOpenFiles {
	struct tOpenFiles* next;
	FILE* file;
	char* name;
	tBinary content;
/*	unsigned char* content;
	long int size;*/
}tOpenFiles;

/***************************************************************\
|              Disk Access & File handling functions            |
\***************************************************************/

/* Repair folders */
const char *repairFolders(const char* a) {
	int i,k;
	static char result[MAX_FILENAME_SIZE];

	for (i=0,k=0;(k<MAX_FILENAME_SIZE)&&a[i];) {
		if (isDirSep(a,i)) {
			result[k]=DIR_SEPARATOR;
			i++;
			while (isDirSep(a,i)) i++;
		} else {
			result[k]=a[i];
			i++;
		}
		k++;
	}
	result[k]=0;
	return result;
}

/* Create base directory of a file */
int makebase(const char* p) {
	/*
		Creates the base directory of the given file "p"
		Returns 0 if created
		Returns -1 if an error occurred

		Note: if the base directory already exists it will return -1!
	*/

	/* Declare variables */
	static char old[MAX_FILENAME_SIZE];
	int i,a=0,equal=1;
	int size;
	char* aux;

	/* Initialize variables */
	size=(strlen(p)+1);
	aux=(char*)malloc(size);

	/* Make directories */
	for (i=0;i<size;i++) {
		aux[i]=0;
		equal=equal&&(old[i]==p[i]);
		if ((!equal)&&(p[i]==DIR_SEPARATOR)) a=defmkdir(aux);
		old[i]=p[i];
		aux[i]=p[i];
	}

	free(aux);
	return a;
}

#ifndef DISK_ALLWAYS_FORCE
static tOpenFiles* openFilesList=NULL;

void addFileToOpenFilesList(const char* fileName,int hasBackup) {
	/*
		Add a recently safe open file to the file pointer dynamic table
		using the LIFO philosophy.
	*/

	tOpenFiles* newNode;

	/* Create the new node and fill in the fields */
	newNode=(tOpenFiles*)malloc(sizeof(tOpenFiles));
	newNode->next=openFilesList;
	newNode->name=strallocandcopy(fileName);

	if (hasBackup) {
		newNode->content=mLoadFileArray(fileName);
		if (newNode->content.size<0) newNode->content.size=0;
	} else {
		newNode->content.size=0;
	}
	openFilesList=newNode;
}

#define addPointerToOpenFilesList(fp) openFilesList->file=fp;

int getFromOpenFilesList(FILE* fp, char** fileName, tBinary *content) {
	tOpenFiles* currentNode;
	tOpenFiles* priorNode=NULL;

	/* Search using FILE* file as key */
	if (openFilesList==NULL) return 0; /* Empty list */
	currentNode=openFilesList;
	while ((currentNode->file!=fp)&&(currentNode->next!=NULL)) {
		priorNode=currentNode;
		currentNode=currentNode->next;
	}
	if (currentNode->file!=fp) return 0; /* Not found */

	/* Return results */
	*fileName=currentNode->name;
	*content=currentNode->content;
/*	*size=currentNode->size;*/

	/* free node and set prior pointer to the next */
	if (priorNode==NULL) {
		openFilesList=currentNode->next;
	} else {
		priorNode->next=currentNode->next;
	}
	free(currentNode);

	return 1;
}

int writeClose(FILE* fp,int dontSave,int optionflag,const char* backupExtension) {
	tBinary content;
	char* fileName;

	if (getFromOpenFilesList(fp,&fileName,&content)) {
		if (dontSave) {
			fclose(fp);
			if (content.size) {
				fp=fopen(fileName,"wb");
				if (fp==NULL) return -1;
				fwrite(content.data,1,content.size,fp);
				fclose(fp);
			} else {
				remove(fileName);
			}
		} else {
			/* File Existed before and we need to back it up */
			if (hasFlag(backup_flag)) {
				char aux[MAX_FILENAME_SIZE];
				static const char defaultbackupExtension[]=DEFAULT_BACKUP_EXTENSION;
				/* Set default values if there isn't */
				if (backupExtension==NULL) backupExtension=defaultbackupExtension;
				/* generate the file name */
				sprintf(aux,"%s.%s",fileName, backupExtension);
				fclose(fp);
				fp=fopen(aux,"wb");
				if (fp==NULL) return -2;
				fwrite(content.data,1,content.size,fp);
				fclose(fp);
			} else {
				fclose(fp);
			}
		}

		free(fileName);
		if (content.size) free(content.data);
	}

	return 0;
}

int writeOpen(const char* vFileext, FILE* *fp, int optionflag) {
	/*
		Opens vFileext for write access
		 if the path doesn't exist it is created
		 if the file doesn't exist it is created
		 if the file does exist it is overwritten

		Sets the file pointer and returns 1 if Ok or 0 if error

		Returns
		 0 if error
		 1 if ok
	*/
	const char* file;
	whatIs fileType;
	static int all=0;
	int result;

#ifdef UNIX
#ifdef DISK_TERM_MANIPULATION
	/* This will eliminate the enter after the input */
	struct termios term;
	struct termios termOld;

	tcgetattr (STDIN_FILENO, &term);
	tcgetattr (STDIN_FILENO, &termOld); /* save original proprieties */
	term.c_lflag &= ~(ICANON);
	tcsetattr (STDIN_FILENO, TCSANOW, &term);
#endif
#endif

	/* Create base directory and save file */
	file=repairFolders(vFileext);

	/* Verify if file already exists. */
	fileType=isDir(vFileext);
	if (fileType==eDirectory) return 0;

	if (fileType==eFile) {
		/* File exists. We need to ask */
		if ((!(hasFlag(force_flag)))&&(!all)) {
			char answer;
			printf(PR_TEXT_DISK_REPLACE,getFileNameFromPath(file));
			answer=osIndepGetCharacter();
			printf("\n");
			if (charToUpper(answer)==PR_DISK_REPLACE_NO)  return 0;
			if (charToUpper(answer)==PR_DISK_REPLACE_ALL) all=1;
		}
	} else {
		makebase(file);
	}

#ifdef UNIX
#ifdef DISK_TERM_MANIPULATION
	/* restoring previous terminal options */
	term=termOld;
	tcsetattr (STDIN_FILENO, TCSANOW, &termOld);
#endif
#endif
	/*
		If the file exists, we need to remember the old content in memory
		if not, we need to know the name in case we need to delete it
	*/

	addFileToOpenFilesList(file,hasFlag(backup_flag));

	if ((result=((*fp=fopen(file,"wb"))!=NULL))) addPointerToOpenFilesList(*fp);

	return result;
}
#else
int writeClose(FILE* fp,int dontSave,int optionflag,const char* backupExtension) {
	unsigned long int size=0;

		if (dontSave) {
			fclose(fp);
			if (size) {
				fp=fopen(/*fileName*/"/dev/null","wb");
				if (fp==NULL) return -1;
			}
	}

	return fclose(fp);
}

int writeOpen(const char* vFileext, FILE* *fp, int optionflag) {
	/*
		Opens vFileext for write access
		 if the path doesn't exist it is created
		 if the file doesn't exist it is created
		 if the file does exist it is overwritten

		Sets the file pointer and returns 1 if Ok or 0 if error

		Returns
		 0 if error
		 1 if ok
	*/
	const char* file;
	whatIs fileType;
	int result;

	/* Create base directory and save file */
	file=repairFolders(vFileext);

	/* Verify if file already exists. */
	fileType=isDir(vFileext);
	if (fileType==eDirectory) return 0;

	if (fileType==eFile) {
		/* File exists. We need to ask */
	} else {
		makebase(file);
	}

	/*
		If the file exists, we need to remember the old content in memory
		if not, we need to know the name in case we need to delete it
	*/

/*	addFileToOpenFilesList(file,hasFlag(backup_flag));*/
	result=((*fp=fopen(file,"wb"))!=NULL);/* addPointerToOpenFilesList(*fp);*/
	return result;
}
#endif

int writeData(const unsigned char* data, int ignoreChars, const char* vFileext, int size, int optionflag,const char* backupExtension) { /* TODO: use tBinary */
	/*
		Creates vFileext and saves data in it. In case the directory doesn't
		exist it will be created.

		Data is read from ignoreChars to size.
		Example:
			if data="123456789", ignoreChars=3, size=8
			saved file will contain "45678"

		Returns
		 0 if error
		 1 if ok
	*/

	/* Declare variables */
	FILE* target;
	char  ok;

	/* Verify parameters */
	size-=ignoreChars;
	if (size<0) return 0;
	/*if (size==0) return 1; * Wrote 0 bytes */

	/* Save file */
	ok=writeOpen(vFileext,&target,optionflag);
	ok=ok&&((!size)||fwrite(data+ignoreChars,size,1,target));
	ok=ok&&(!writeCloseOk(target,optionflag,backupExtension));
	return ok;
}

tBinary mLoadFileArray(const char* vFile) {
	/*
		Using the string in vFile, it opens the file and returns the
		number of bytes	in it and the content of the file in array.
		In case the file couldn't be open or memory allocated returns 0.
	*/

	/* declare variables */
	FILE *fp;
	int  aux; /* TODO: replace by ret.size */
	const char* file=repairFolders(vFile);
	whatIs f;
	tBinary ret;

	/* check type */
	f=isDir(file);
	if (f==eDirectory) {
		ret.size=PR_RESULT_ERR_FILE_NOT_OPEN_WASDIR;
		ret.data=NULL;
		return ret;
	}
	if (f==eNotFound) {
		ret.size=PR_RESULT_ERR_FILE_NOT_OPEN_NOTFOUND;
		ret.data=NULL;
		return ret;
	}

	/* Open the file */
	if ((fp=fopen(file,"rb"))==NULL) {
		ret.size=PR_RESULT_ERR_FILE_NOT_READ_ACCESS;
		ret.data=NULL;
		return ret;
	} else {
		/* get file size */
		fseek(fp,0,SEEK_END);
		aux=ftell(fp);
		if (!aux) {
			fclose(fp);
			ret.size=0;
			ret.data=NULL;
			return ret;
		}
		if ((ret.data=(unsigned char*)malloc(aux+1))==NULL) {
			fclose(fp);
			ret.size=PR_RESULT_ERR_MEMORY;
			ret.data=NULL;
			return ret; /* this could happen in big files! */
		} else {
			/* if the file was successfully open */
			fseek(fp,0,SEEK_SET);
			aux=fread (ret.data,1,aux,fp);
			ret.data[aux]=0;
			fclose(fp);
			ret.size=aux;
			return ret;
		}
	}
}

const char* getFileNameFromPath(const char* path) {
	/*
		If you give a path you get the filename,
		if you give a filename, you get the same filename
	*/
	int size;
	size=strlen(path);
	while (size) {
		if (isDirSep(path,size)) {
			return path+size+1;
		}
		size--;
	}
	return path;
}

whatIs isDir(const char *path) {
	/*
		eDirectory if path is a directory
		eNotFound if path isn't a directory or doesn't exist
		eFile if it is a file
	*/
	struct stat buf;

	if(stat(path,&buf)==-1) return eNotFound;
	return (S_ISDIR(buf.st_mode))?eDirectory:eFile;
}

#ifdef DISK_DIR_SCANING

int recurseDirectory(const char* path,int recursive, void* pass, void (*function)(const char*,void*)) {
	/*
		Search for all .DAT files in the directory
		if recursive flag is set search over the subdirectories
		if verbose flag is set shows some messages in the screen
		when .DAT files are found it runs prMain form each of them
	*/

	/* Declare variables */
	char*          recursivePath;
	struct dirent* directoryStructure;
	DIR*           dir;

	/* Opens directory */
	if ((dir = opendir(path))==NULL) {
		return -1;
	}

	/* Main loop: while there are still more files left */
	while ((directoryStructure = readdir(dir))!=NULL) {
		if /* Don't look over the system directories */
			(
				strcmp(directoryStructure->d_name,".")&&
				strcmp(directoryStructure->d_name,"..")
		) {
			/* Declare variables */
			int sizeOfPath=strlen(path);
			int sizeOfFile=strlen(directoryStructure->d_name);

			/* Generate recursive path */
			recursivePath=(char*)malloc(sizeOfPath+2+sizeOfFile);
			memcpy(recursivePath,path,sizeOfPath);
			recursivePath[sizeOfPath]=DIR_SEPARATOR;
			memcpy(recursivePath+sizeOfPath+1,directoryStructure->d_name,sizeOfFile+1);

			if (isDir(recursivePath)==eDirectory&&recursive) {
				/* Only recourse if we are in a directory and recursive is true */
				recurseDirectory(recursivePath,recursive,pass,function);
			} else {
				function(recursivePath,pass);
			}
			/* Free all allocated memory */
			free(recursivePath);
		}
	}
	closedir(dir);
	return 0;
}

#endif

#ifdef MACOS
int macfreads (void* bigEndian,FILE* file) {
	unsigned short int littleEndian;
	unsigned char* lit_e=(unsigned char*)&littleEndian;
	unsigned char* big_e=(unsigned char*)bigEndian;
	int result=fread(lit_e,2,1,file);
	big_e[0]=lit_e[1];
	big_e[1]=lit_e[0];
	return result;
}
int macfreadl (void* bigEndian,FILE* file) {
	unsigned long int littleEndian;
	unsigned char* lit_e=(unsigned char*)&littleEndian;
	unsigned char* big_e=(unsigned char*)bigEndian;
	int result=fread(lit_e,4,1,file);
	big_e[0]=lit_e[3];
	big_e[1]=lit_e[2];
	big_e[2]=lit_e[1];
	big_e[3]=lit_e[0];
	return result;
}
int macfwrites(const void* var,FILE* file) {
	unsigned short int littleEndian;
	unsigned short int bigEndian=*(unsigned short int*)(var);
	unsigned char* lit_e=(unsigned char*)&littleEndian;
	unsigned char* big_e=(unsigned char*)&bigEndian;
	lit_e[0]=big_e[1];
	lit_e[1]=big_e[0];
	return fwrite(lit_e,2,1,file);
}

int macfwritel(const void* var,FILE* file) {
	unsigned long int littleEndian;
	long int bigEndian=*(long int*)(var);
	unsigned char* lit_e=(unsigned char*)&littleEndian;
	unsigned char* big_e=(unsigned char*)&bigEndian;
	lit_e[0]=big_e[3];
	lit_e[1]=big_e[2];
	lit_e[2]=big_e[1];
	lit_e[3]=big_e[0];
	return fwrite(lit_e,4,1,file);
}

#endif

