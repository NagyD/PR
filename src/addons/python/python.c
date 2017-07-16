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
python.c: Princed Resources : PR Python interface
¯¯¯¯¯¯¯¯
 Copyright 2005 Princed Development Team
  Created: (check CVS history)

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.00

 Note:
  DO NOT remove this copyright notice
*/

#include <Python.h>
#include "pr.h"

/* Implemented functions:
 *
 * prClassifyDat
 * prExportDat
 * prExportDatOpt
 * prImportDat
 * prImportDatOpt
 * prSetOutput
 * setCompressionLevel
 */

/* setCompressionLevel */
static PyObject* pr_setCompressionLevel(PyObject *self, PyObject *args){
	int cl;

	if (!PyArg_ParseTuple(args, "i", &cl)) return NULL;
	setCompressionLevel(cl);
	return Py_BuildValue("i", 0);
}

/* prClassifyDat */
static PyObject* pr_classifyDat(PyObject *self, PyObject *args){
	char* vFiledat;
	int result;

	if (!PyArg_ParseTuple(args, "s", &vFiledat)) return NULL;
	result=prClassifyDat(vFiledat);
	return Py_BuildValue("i", result);
}

/* prExportDat */
static PyObject* pr_exportDat(PyObject *self, PyObject *args){
	char* vDatFile;
	char* vDirName;
	char* vResFile;
	int result;

	if (!PyArg_ParseTuple(args, "sss", &vDatFile,&vDirName,&vResFile)) return NULL;
	result=prExportDat(vDatFile,vDirName,vResFile);
	return Py_BuildValue("i", result);
}

/* prImportDat */
static PyObject* pr_importDat(PyObject *self, PyObject *args){
	char* vDatFile;
	char* vDirName;
	char* vResFile;
	int result;

	if (!PyArg_ParseTuple(args, "sss", &vDatFile,&vDirName,&vResFile)) return NULL;
	result=prImportDat(vDatFile,vDirName,vResFile);
	return Py_BuildValue("i", result);
}

/* Python initialization */
static PyMethodDef SpamMethods[] = {
	{"setCompressionLevel",  pr_setCompressionLevel, METH_VARARGS,
		"Sets the compression level to be used by the module."},
	{"classifyDat",  pr_classifyDat, METH_VARARGS,
		"Checks if a file is a valid POP DAT file and tries to return the DAT file type."},
	{"exportDat",  pr_exportDat, METH_VARARGS,
		"Exports files from a given DAT archive."},
	{"importDat",  pr_importDat, METH_VARARGS,
		"Imports files to a given DAT archive."},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpr(void) {
	(void) Py_InitModule("pr", SpamMethods);
}

int main(int argc, char *argv[]) {
	/* Pass argv[0] to the Python interpreter */
	Py_SetProgramName(argv[0]);

	/* Initialize the Python interpreter.  Required. */
	Py_Initialize();

	/* Add a static module */
	initpr();
	return 0;
}

