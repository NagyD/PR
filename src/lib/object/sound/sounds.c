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
sounds.c: Princed Resources : Sound resource handling
¯¯¯¯¯¯¯¯
 Copyright 2006 Princed Development Team
  Created: 19 Feb 2006

  Author: Enrique Calot <ecalot.cod@princed.org>
  Version: 1.01 (2006-Feb-09)

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                  I M P L E M E N T A T I O N                  |
\***************************************************************/

#include "binary.h" /* tBinary */
#include "common.h"
#include "wav.h" 
#include <stdlib.h>
#include <string.h> /* memcpy */
#include "dat.h" 
#include "reslist.h" 
#include "sound.h"
#include "bitfield.h" /*BitReader*/

/***************************************************************\
|                         Binary Object                         |
\***************************************************************/

/* Maybe this should be in lib/compression ? */
tBinary decompressWave(tBinary compressed) {
	tBinary result;
	unsigned char* data = compressed.data;

	/* The first two bytes store the expanded size. */
	result.size = (data[1] << 8) | data[0];
	result.data = malloc(result.size);
	/* Question: Who will free result.data? */

	unsigned char* out = result.data;
	unsigned char* end = result.data + result.size;

	/* The first sample is stored as a byte. */
	unsigned char sample = data[2];
	*(out++) = sample;
	/* All the next samples are stored in <=8 bits, as differences from the previous. */
	BitReader br;
	initBitReader(&br, data+3);
	while (out < end) {
		/* The size of the following samples in bits. */
		int n_bits = readBitReader(&br, 3) + 2;
		if (n_bits == 9) {
			/* A special case: delta = 0x80 */
			sample ^= 0x80; /* +=0x80 or -=0x80, it's the same for bytes. */
			*(out++) = sample;
		} else {
			/* The smallest negative value, with only the sign bit=1. */
			int sign = 1 << (n_bits - 1);
			int minus = 1 << n_bits;
			while (out < end) {
				int delta = readBitReader(&br, n_bits);
				/* The smallest negative value means that the samples' size will change. */
				if (delta == sign) break;
				/* This is a two's complement signed value. */
				if (delta & sign) delta -= minus;
				sample += delta;
				*(out++) = sample;
			}
		}
	}

	return result;
}

int wavetype = 0;

void* objWaveCreate(tBinary cont, int *error) { /* use get like main.c */
	tWave* r;
	int type = 0;
	int bits=0, samplerate=0, n_samples=0, header_size = 0;

	*error=PR_RESULT_SUCCESS;

	if (cont.data[7] == 8 || cont.data[7] == 255) {
		type = 1; /* PoP 1.0 */
		bits = cont.data[7];
		samplerate = (cont.data[2] << 8) | cont.data[1];
		n_samples = (cont.data[4] << 8) | cont.data[3];
		header_size = 8;
	} else if (cont.data[3] == 8 || cont.data[3] == 255) {
		type = 2; /* PoP 1.3, 1.4, 2 */
		bits = cont.data[3];
		samplerate = (cont.data[2] << 8) | cont.data[1];
		n_samples = (cont.data[5] << 8) | cont.data[4];
		header_size = 10;
	} else {
		/* TODO: find a better error code */
		*error = PR_RESULT_ERR_EXTRACTION;
		printf("Can't determine wave version!\n");
		return NULL;
	}

	/* wavetype is written into a file in extract() */
	wavetype = type;

	void *data = cont.data + header_size;
	int   size = cont.size - header_size;

	r = (tWave*)malloc(sizeof(tWave));
	r->samples.data = data;
	r->samples.size = size;

	if (bits == 255) {
		/* compressed: PoP1 demo, PoP2 NISDIGI.DAT */
		/* TODO: decompression */
		//*error = PR_RESULT_ERR_EXTRACTION;
		//return NULL;
		r->samples = decompressWave(r->samples);
	}

	/* TODO: warn if (r->samples.size != n_samples) */
	r->samplerate = samplerate;
	return (void*)r;
}

void* objMidiCreate(tBinary cont, int *error) {
	tBinary* r;
	*error=PR_RESULT_SUCCESS;
	
	r=(tBinary*)malloc(sizeof(tBinary));
	r->data=cont.data+1;
	r->size=cont.size-1;
	return (void*)r;
}

/* Should we cut the first byte? */
void* objPcspeakerCreate(tBinary cont, int *error) {
	tBinary* r;
	*error=PR_RESULT_SUCCESS;
	
	r=(tBinary*)malloc(sizeof(tBinary));
	r->data=cont.data+1;
	r->size=cont.size-1;
	return (void*)r;
}

/* Is this used at all?
int objSoundWrite(void* o, const char* file, int write(const char* file,tBinary* data,int optionflag, const char* backupExtension), int optionflag, const char* backupExtension) {
	return write(file,(tBinary*)o,optionflag,backupExtension);
}
*/

int objWaveWrite(void* o, const char* file, int optionflag, const char* backupExtension) {
	return writeWav(file,(tWave*)o,optionflag,backupExtension);
}

/*void* objSoundRead(const char* file, int read(const char* file, tBinary* c, int *pchannels, long *psamplerate, long *pbps), int *result, int *pchannels, long *psamplerate, long *pbps) {
	tBinary* o=(tBinary*)malloc(sizeof(tBinary));
	*result=read(file,o);
	return (void*)o;
}*/

void* objWaveRead(const char* file, int *result) {
	int  channels;
	long samplerate;
 	long bps;
	tWave* o=(tWave*)malloc(sizeof(tWave));
	o->samples.data = NULL;
	
	*result=readWav(file,&(o->samples),&channels,&samplerate,&bps);

	if (*result==PR_RESULT_SUCCESS) {
		if (bps!=8)            *result=PR_RESULT_WAV_UNSUPPORTED_BITRATE;
		if (samplerate>=65536||samplerate<0) *result=PR_RESULT_WAV_UNSUPPORTED_SAMPLERATE;
		if (channels!=1)     *result=PR_RESULT_WAV_UNSUPPORTED_STEREO;
		o->samplerate = samplerate;
	}
	if (*result!=PR_RESULT_SUCCESS) {
		if (o->samples.data) free(o->samples.data);
		free(o);
		return NULL;
	}
	
	return (void*)o;
}

/* TODO: factorize */
int objWaveSet(void* o,tResource* res) {
	tWave* wave=o;

	int type=0;

	/* wavetype is set by import_full() */
		/* The default wavetype is the PoP version. */
		if (wavetype == 0) {
			/*wavetype = mReadGetVersion();*/ /* = 0 WHY??? */
			static const char*    textPop1="pop1"; /* from dat.c */
			/* Annoying: tResource has no "popversion" field, so I have to check index=="pop1". */
			if (!strncmp(res->id.index,textPop1,4)) {
				wavetype = 1;
			} else {
				wavetype = 2;
			}
			printf("wavetype = %d\n", wavetype);
		}
	type = wavetype;

	int header_size=0;
	if (type == 1) {
		header_size = 8;
	} else if (type == 2) {
		header_size = 10;
	} else {
		/* TODO: find a better error code */
		return PR_RESULT_COMPRESS_RESULT_FATAL;
	}

	res->content.size=wave->samples.size+header_size;
	res->content.data=malloc(wave->samples.size+header_size);
	/* TODO: set loop flag if needed */
	res->content.data[0]=0x01; /* TODO: use WAVE_MAGIC */
	res->content.data[1] = wave->samplerate & 0xFF;
	res->content.data[2] = (wave->samplerate >> 8) & 0xFF;
	if (type == 1) {
		res->content.data[3] = wave->samples.size & 0xFF;
		res->content.data[4] = (wave->samples.size >> 8) & 0xFF;
		res->content.data[5] = 0;
		res->content.data[6] = 0;
		res->content.data[7] = 8;
	} else if (type == 2) {
		res->content.data[3] = 8;
		res->content.data[4] = wave->samples.size & 0xFF;
		res->content.data[5] = (wave->samples.size >> 8) & 0xFF;
		res->content.data[6] = 0;
		res->content.data[7] = 0;
		res->content.data[8] = 0;
		res->content.data[9] = 0;
	} else {
		/* TODO: find a better error code */
		return PR_RESULT_COMPRESS_RESULT_FATAL;
	}

	memcpy(res->content.data+header_size,wave->samples.data,wave->samples.size);
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}

int objMidiSet(void* o,tResource* res) {
	tBinary* midi=o;
	res->content.size=midi->size+1;
	res->content.data=malloc(midi->size+1);
	res->content.data[0]=0x02; /* TODO: use MIDI_MAGIC */
	memcpy(res->content.data+1,midi->data,midi->size);
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}

int objPcspeakerSet(void* o,tResource* res) {
	tBinary* pcspeaker=o;
	res->content.size=pcspeaker->size+1;
	res->content.data=malloc(pcspeaker->size+1);
	res->content.data[0]=0x00; /* TODO: use PCSPEAKER_MAGIC */
	memcpy(res->content.data+1,pcspeaker->data,pcspeaker->size);
	mWriteFileInDatFile(res);
	return PR_RESULT_SUCCESS;
}

