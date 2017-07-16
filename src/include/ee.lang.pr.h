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

#define PR_ABOUT                  "Princed resources (PR)"PR_VERSION""OS""PR_COPY""PR_URL""

#define PR_CGI_TEXT1              "Content-Type:text/html\n\nTöötab CGI-programmina"
#define PR_CGI_TEXT2              "Tulemus: %02d tüüp"

#define PR_TEXT_RESULT            "Tulemus: %s (%d)"
#define PR_TEXT_RESULT_ERR        "Tulemus: %d failis olid vead"

#define PR_TEXT_FILE_NOT_FOUND    "Tulemus: Faili või kausta '%s' ei leitud"

#define PR_TEXT_SCANNING_CURRENT  "Otsib aktiivsest kaustast DAT-faile"
#define PR_TEXT_SCANNING_GIVEN    "Otsib antud kaustast DAT-faile"

#define PR_TEXT_IMPORTING_CURRENT "Impordib aktiivsest kaustast kõiki ilma vigadeta DAT-faile"
#define PR_TEXT_IMPORTING_GIVEN   "Impordib antud kaustast kõiki ilma vigadeta DAT-faile"

#define PR_TEXT_TASK_COMPILE      "Impordib faili '%s' kaustast '%s'"
#define PR_TEXT_TASK_CLASSIFY     "Liigitab faili '%s'"
#define PR_TEXT_TASK_EXTRACT      "Ekspordib faili '%s' kausta '%s'"

#define PR_TEXT_DISK_PROCESSING   "Töötleb faili '%s'..."
#define PR_TEXT_DISK_REPLACE      "Fail %s on juba olemas. Asenda? [Yes/no/all]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'A'

#define PR_TEXT_IMPORT_NOT_OPEN   "Fail '%s' pole lahti, see jäeti vahele"
#define PR_TEXT_IMPORT_SUCCESS    "Fail '%s' edukalt imporditud"
#define PR_TEXT_IMPORT_ERRORS     "Failis '%s' on vigu, see jäeti vahele"
#define PR_TEXT_IMPORT_DONE       "Importimine valmis, %d fail(i) imporditi korralikult, %d failis olid vead"

#define PR_TEXT_EXPORT_WORKING    "Fail '%s' edukalt eksporditud"
#define PR_TEXT_EXPORT_ERROR      "Failis '%s' on vigu, katkestab..."
#define PR_TEXT_EXPORT_BMP_WARN   "Hoiatus: Lahti pakitud fail võib olla vigane"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"OK",\
"Viga juurdepääsus failile",\
"Mälu viga lahtipakkimisel",\
"Vigane DAT-fail",\
"XML'i töötlemise viga",\
"Mälu viga töötlemisel",\
"XML-atribuuti ei tuvastatud",\
"XML-faili ei leitud"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Mälu viga",\
"Faili ei leitud või viga juurdepääsus failile",\
"Vigane DAT-fail või mõni muu fail",\
"Tasemefail",\
"Graafikat sisaldav DAT-fail ilma paletita",\
"Digitaalhelisid sisaldav fail",\
"MIDI-sid sisaldav fail",\
"Korras DAT-fail defineerimata sisuga",\
"Graafikat sisaldav DAT-fail",\
"PC Speaker'i muusikat sisaldav DAT-fail",\
"","","",\
"Pop2 DAT-fail"}

#define PR_TEXT_IMPORT_ARRAY {\
"Fail edukalt imporditud",\
"DAT-faili ei leitud või tunnistati osalisel importimisel vigaseks",\
"DAT-faili ei saa avada",\
"XML-i töötlemise viga",\
"Mälu pole",\
"XML-atribuuti ei tuvastatud",\
"XML-faili ei leitud"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_RAW "   -r, --raw                  kasutab RAW-formaati"
#else
#define PARSING_RAW ""
#endif
#define PARSING_HELP_BEGIN "Usage:\n\n  pr [-x[EXPORTDIR]|-i[IMPORTDIR]|-d] [DATFILEPATH]\n\n  pr [OPTIONS] [DATFILEPATH]\n\n\n\n  Vajalikud argumendid pikkadele suvanditele on vajalikud ka lühikestele suvanditele."
#define PARSING_HELP_PART1 "   -i, -c, --import[=DIRNAME] impordib kataloogist DIRNAME antud DAT-faili\n\n   -d, --classify             annab DAT-faili tüübi\n\n   -x, -e, --export[=DIRNAME] ekspordib antud DAT-faili kataloogi DIRNAME"
#define PARSING_HELP_PART2 "   -a, --setauthor=NAME       paneb sinu nime lahti pakitud PLV-failidesse\n\n   -b, --backup[=EXTENSION]   teeb sinu failidest varukoopiad\n\n   -f, --force                ei küsi sult failide asendamisel\n\n   -g, --cgi                  käivitub CGI-programmina ja väljastab MIME-päised\n\n   -h, -?, --help             näitab seda argumendinimestikku ja väljub\n\n   -s, --resource=RESFILE     kasutab kasutaja poolt määratud ressursside XML-faili"PARSING_RAW

#define PARSING_HELP_PART3 "   -R, --recursive            otsib kõiki DAT-faile (ainult siis, kui DATFILEPATH ei ole DAT-fail)\n\n   -t, --datfile=DATFILE      määrab originaalfailist erineva DAT-faili ressursside lugemiseks\n\n       --unknown              genereerib tundmatu faili ilma lahtipakkimist teostamata\n\n   -v, --verbose              seletab, mida praegu tehakse\n\n       --version              väljastab versiooniinformatsiooni ja väljub"

#define PR_TXT_AUTHORS           "Autorid"
#define PR_TXT_CODER             "Programmeerimine & peaprogramm"
#define PR_TXT_COD_ASSIST        "Programmeerimisel aidanud"
#define PR_TXT_GFX_COMP          "Graafika pakkimise algoritmid"
#define PR_TXT_GFX_DEV           "Graafikaformaadi arendus"
#define PR_TXT_MID               "MID-heliformaadi arendus"
#define PR_TXT_XML               "Resources.xml"
#define PR_TXT_TRANSLATION       "Tõlkinud"
#define PR_TXT_ABOUT_TRANSLATOR  "Eero Ränik"

