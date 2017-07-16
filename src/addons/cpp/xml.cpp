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

/* Headers */
#include "pr.h"
#include "xml.h"

Xml::Xml(const char* file) {
	int error;
	tTag* t=xmlParseFile(file,&error);

	if (error) throw error;

	this->structure=(void*)t;
}

void Xml::write(const char* file) { /* Bug: saving will destroy the tree */
	tTag* tree=this->structure;
	resourceTreeFixInheritances(&tree);
	int r=xmlGenerateFile(file, tree);
	if (r) throw r;
}

void Xml::optimize(TagReference t) {
	tTag* tree=t;
	if (tree) xmlOptimizeCommonFactor(tree->child);
}

void Xml::optimize() {
	this->optimize(this->structure);
}

TagReference Xml::getRoot() {
	return this->structure;
}

TagReference Xml::getChild(TagReference t) {
	return resourceTreeGetChild((tTag*)t);
}

TagReference Xml::getNext(TagReference t) {
	return resourceTreeGetNext((tTag*)t);
}


