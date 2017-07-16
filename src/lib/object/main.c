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
main.c: Princed Resources : Main item class implementation
¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 16 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.org>

 Modified by: David from Princed

 Note:
  DO NOT remove this copyright notice
*/

#include "common.h"
#include "object.h"
#include "reslist.h"

#include "image.h"
#include "other.h"
#include "palette.h"
#include "sound.h"
#include "text.h"

/* Object polimorphism support layer */

tObject getObject(tResource* r, int* error) {
	tObject o;
	if (!r) {
		*error=PR_RESULT_SUCCESS;
		o.type=eResTypeNone;
		o.obj=NULL;
		return o;
	}

	o.type=r->type;
	switch (o.type) {
	case eResTypeLevel:
		o.obj=objLevelCreate(r->content,r->number,r->datfile,r->name,r->desc,r->datAuthor,error); 
		break;
	case eResTypeBinary: /* Binary files */
	case eResTypeText: /* Text files */
	case eResTypeRaw: /* Raw files */
		o.obj=objBinaryCreate(r->content,error); 
		break;
	case eResTypePop1Palette4bits: /* save and remember palette file */
		o.obj=objPalette_pop1_4bitsCreate(r->content,error);
		break;
	case eResTypePop2Palette4bits: /* save and remember palette file */
		o.obj=objPalette_pop2_4bitsCreate(r->content,error);
		break;
	case eResTypePop2PaletteNColors: /* save and remember palette file */
		o.obj=objPop2PaletteNColorsCreate(r->content,error);
		break;
	case eResTypePcspeaker: /* save pcs file */
		o.obj=objPcspeakerCreate(r->content,error);
		break;
	case eResTypeMidi:	/* save midi file */
		o.obj=objMidiCreate(r->content,error);
		break;
	case eResTypeWave: /* save wav file */
		o.obj=objWaveCreate(r->content,error);
		break;
	case eResTypeImage2: /* save image */
		o.obj=objImage2Create(r->content,error);
		break;
	case eResTypeImage16: /* save image */
		o.obj=objImage16Create(r->content,r->palette,error);
		break;
	case eResTypeImage256: /* save image */
		o.obj=objImage256Create(r->content,r->palette,error,r->paletteoffset);
		break;
	case eResTypeTxt4: /* compressed text */
		o.obj=objTxt4Create(r->content,error);
		break;
	default:
		printf("Warning, resource type %d cannot be exported yet!\n",o.type);
		*error=/*PR_RESULT_SUCCESS*/1; /* NOTE: change to 1 to detect unhooked types */
		break;
	}
	
	return o;
}

int writeObject(tObject o, const char* file, int optionflag, const char* backupExtension) {
	int error;
	switch (o.type) {
	case eResTypeLevel:
		error=objLevelWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeBinary: /* Binary files */
	case eResTypeText: /* Text files */
	case eResTypeRaw: /* Raw/autodetect files */
	case eResTypeTxt4: /* compressed text */
		error=objBinaryWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypePop1Palette4bits: /* save and remember palette file */
		error=objPalette_pop1_4bitsWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypePop2Palette4bits: /* save and remember palette file */
		error=objPalette_pop2_4bitsWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypePcspeaker: /* save pcs file */
		error=objPcspeakerWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeMidi:	/* save midi file */
		error=objMidiWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeWave: /* save wav file */
		error=objWaveWrite(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeImage2: /* save image */
		error=objImage2Write(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeImage16: /* save image */
		error=objImage16Write(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypeImage256: /* save image */
		error=objImage256Write(o.obj,file,optionflag,backupExtension);
		break;
	case eResTypePop2PaletteNColors: /* save and remember palette file */
		error=objPop2PaletteNColorsWrite(o.obj,file,optionflag,backupExtension);
		break;
	default:
		printf("Warning, resource type %d cannot be written yet!\n",o.type);
		break;
	}

	return error;
}	

/* Palette class methods */

int paletteGetBits(tObject pal) {
	int i=paletteGetColors(pal),b=0;
	while(b<8 && 1<<b<i) b++;
	return b;
	switch (pal.type) {
	case eResTypePop2PaletteNColors:
		return 8;
	case eResTypePop1Palette4bits: 
	case eResTypePop2Palette4bits: 
		return 4;
	case eResTypePop1PaletteMono: 
		return 1;
	default:
		return 0;
	}
}
			
int paletteGetColors(tObject pal) {
	switch (pal.type) {
	case eResTypePop2PaletteNColors:
		return ((tGenericPalette*)pal.obj)->size;
		return 256;
	case eResTypePop1Palette4bits: 
	case eResTypePop2Palette4bits: 
		return 16;
	case eResTypePop1PaletteMono: 
		return 2;
	default:
		return 0;
	}
}

tColor* paletteGetColorArray(tObject pal) {
	switch (pal.type) {
	case eResTypePop1Palette4bits: /* save and remember palette file */
		return objPalette_pop1_4bitsGetColors(pal.obj);
	case eResTypePop2Palette4bits: /* save and remember palette file */
		return objPalette_pop2_4bitsGetColors(pal.obj);
	case eResTypePop2PaletteNColors:
		return objPalette_pop2_ncolorsGetColors(pal.obj);
	default:
		printf("paletteGetColorArray: unknown palette type = %d\n", pal.type);
		return NULL;
	}
}

/***************************************************************\
|                    Dat compiling primitive                    |
\***************************************************************/

void setObject(tObject o,int *result,tResource* res) {

/*if(o.obj==NULL) printf("o.obj==NULL\n");*/
if(o.obj==NULL){ *result=PR_RESULT_ERR_FILE_NOT_OPEN_NOTFOUND; return; }

	switch (o.type) {
		case eResTypeLevel:
			/*o.obj=objLevelRead(file,res.content,result);*/
			*result=objLevelSet(o.obj,res);
			break;
		case eResTypeImage16:
			*result=objImage16Set(o.obj,res);
			break;
		case eResTypeWave:
			*result=objWaveSet(o.obj,res);
			break;
		case eResTypeMidi:
			*result=objMidiSet(o.obj,res);
			break;
		case eResTypePcspeaker:
			*result=objPcspeakerSet(o.obj,res);
			break;
		case eResTypePop1Palette4bits:
			*result=objPop1Palette4bitsSet(o.obj,res);
			break;
		case eResTypeImage2: /* fix by David */
			*result=objImage2Set(o.obj,res);
			break;
		case eResTypePop2PaletteNColors:
			*result=objPop2PaletteNColorsSet(o.obj,res);
			break;
		case eResTypeImage256:
			*result=objImage256Set(o.obj,res);
			break;
		case eResTypeTxt4: /* compressed text */
			*result=objTxt4Set(o.obj,res);
			break;
		default:
			printf("Warning, resource type %d cannot be imported yet!\n",o.type);
		case eResTypeBinary:
			*result=objBinarySet(o.obj,res);
			break;
	}
	
}

/* Format detection function (private function, not in header file) */
tObject readObject(const char* file,tResource* res,int *result) {
	/* return true if ok, false if error */
	tObject o;
	switch (res->type) {
		case eResTypeLevel:
			/*o.obj=objLevelRead(file,res.content,result);*/
			o.obj=objLevelRead(file,result);
			break;
		case eResTypeImage16:
			o.obj=objImage16Read(file,res->palette,result);
			break;
		case eResTypeWave:
			o.obj=objWaveRead(file,result);
			break;
		case eResTypeMidi:
			o.obj=objMidiRead(file,result);
			break;
		case eResTypePcspeaker:
			o.obj=objPcspeakerRead(file,result);
			break;
		case eResTypePop1Palette4bits:
			o.obj=objPop1Palette4bitsRead(file,result);
			break;
		case eResTypePop2Palette4bits:
			o.obj=objPop2Palette4bitsRead(file,result);
			break;
		case eResTypeImage2: /* fix by David */
			o.obj=objImage2Read(file,res->palette,result);
			break;
		case eResTypeImage256:
			o.obj=objImage256Read(file,res->palette,result);
			break;
		case eResTypePop2PaletteNColors:
			o.obj=objPop2PaletteNColorsRead(file,result);
			break;
		default:
			printf("Warning, resource type %d cannot be read yet!\n",res->type);
		case eResTypeBinary:
		case eResTypeTxt4: /* compressed text */
			o.obj=objBinaryRead(file,result);
			break;
	}
	o.type=res->type;

	if(o.obj==NULL){ *result=PR_RESULT_ERR_FILE_NOT_OPEN_NOTFOUND; return o; }
/*if(o.obj==NULL) printf("o.obj==NULL in readObject, res->type=%d\n",res->type);*/

	return o;
}

