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
  Created: 4 Apr 2004

  Author: Tammo Jan Dijkema

  Language: Dutch

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                           Text Defines                        |
\***************************************************************/

#define PR_ABOUT                  "Princed resources (PR) "PR_VERSION" "OS"\n"PR_COPY"\n"PR_URL"\n\n"

#define PR_CGI_TEXT1              "Content-Type:text/html\n\nGestart in cgi-modus\n"
#define PR_CGI_TEXT2              "Resultaat: type %02d\n"

#define PR_TEXT_RESULT            "Resultaat: %s (%d)\n"
#define PR_TEXT_RESULT_ERR        "Resultaat: %d bestanden met fouten\n"

#define PR_TEXT_FILE_NOT_FOUND    "Resultaat: Bestand of directory '%s' niet gevonden\n"

#define PR_TEXT_SCANNING_CURRENT  "Zoeken dat-bestanden in huidige directory\n"
#define PR_TEXT_SCANNING_GIVEN    "Zoeken dat-bestanden in de opgegeven directory\n"

#define PR_TEXT_IMPORTING_CURRENT "Importeren alle geldige bestanden uit de huidige directory\n"
#define PR_TEXT_IMPORTING_GIVEN   "Importeren alle geldige bestanden uit de opgegeven directory\n"

#define PR_TEXT_TASK_COMPILE      "Importeren '%s' vanuit '%s'\n"
#define PR_TEXT_TASK_CLASSIFY     "Bepalen type van '%s'\n"
#define PR_TEXT_TASK_EXTRACT      "Exporteren '%s' naar '%s'\n"

#define PR_TEXT_DISK_PROCESSING   "Verwerken '%s'...\n"
#define PR_TEXT_DISK_REPLACE      "Bestand %s bestaat al. Overschrijven? [Yes/no/all]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'A'

#define PR_TEXT_IMPORT_NOT_OPEN   "'%s' is niet open en wordt overgeslagen\n"
#define PR_TEXT_IMPORT_SUCCESS    "'%s' met succes geïmporteerd\n"
#define PR_TEXT_IMPORT_ERRORS     "'%s' bevat fouten en wordt overgeslagen\n"
#define PR_TEXT_IMPORT_DONE       "Importeren voltooid, %d bestanden in orde, %d bestanden met fouten\n"

#define PR_TEXT_EXPORT_WORKING    "'%s' succesvol geëxporteerd\n"
#define PR_TEXT_EXPORT_ERROR      "'%s' bevat fouten, afbreken...\n"
#define PR_TEXT_EXPORT_BMP_WARN   "Let op: Uitgepakt bestand kan fouten bevatten\n"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"Ok",\
"Fout bij bestandstoegang",\
"Geheugenfout bij het uitpakken",\
"Ongeldig DAT-bestand",\
"XML parsefout",\
"Geheugenfout bij het parsen",\
"XML-eigenschap niet herkend",\
"XML-bestand niet gevonden"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Geheugenfout",\
"Bestand niet gevonden of fout met toegangsrechten",\
"Geen geldig DAT-bestand",\
"Levels-bestand",\
"DAT-bestand met plaatjes zonder palet",\
"Wave-bestand (geluid)",\
"Midi-bestand (muziek)",\
"Geldig DAT-bestand met ongedefinieerde inhoud",\
"DAT-bestand met plaatje",\
"DAT-bestand met PC-speaker geluid",\
"","","",\
"DAT-file voor POP 2"}

#define PR_TEXT_IMPORT_ARRAY {\
"Bestand met succes geïmporteerd",\
"DAT-bestand niet gevonden of ongeldig na het deels importeren",\
"DAT-bestand kan niet worden geopend",\
"XML parsefout",\
"Geen geheugen",\
"XML-eigenschap niet herkend",\
"XML-bestand niet gevonden"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_RAW "   -r, --raw                  gebruikt raw inhoud\n"
#else
#define PARSING_RAW ""
#endif
#define PARSING_HELP_BEGIN "Usage: \n  pr [-x[EXPORTDIR]|-i[IMPORTDIR]|-d] [DATFILEPATH]\n  pr [OPTIONS] [DATFILEPATH]\n\n  Verplichte argumenten voor lange opties zijn dat voor korte opties ook.\n\n"
#define PARSING_HELP_PART1 "   -i, -c, --import[=DIRNAME] importeren vanuit DIRNAME naar opgegeven dat-bestand\n   -d, --classify             geeft het type van het dat-bestand\n   -x, -e, --export[=DIRNAME] exporteert opgegeven dat-bestand naar DIRNAME\n\n"
#define PARSING_HELP_PART2 "   -a, --setauthor=NAME       stel jouw naam in in uitgepakte plv-bestanden\n   -b, --backup[=EXTENSION]   maak een backup van je bestanden\n   -f, --force                overschrijft je bestanden zonder te vragen\n   -g, --cgi                  draai als CGI-programma en print MIME-uitvoer\n   -h, -?, --help             geef deze help-informatie weer en stop\n   -s, --resource=RESFILE     gebruik een door de gebruiker gedefinieerd XML-bestand\n"PARSING_RAW

#define PARSING_HELP_PART3 "   -R, --recursive            zoek naar alle dat-bestanden (alleen als DATFILEPATH\n                              geen bestand is)\n   -t, --datfile=DATFILE      lees de resource-informatie uit een ander bestand dan\n                              het origineel\n       --unknown              pak het bestand uit zonder iets bijzonders te doen\n   -v, --verbose              geef uitleg over wat er gebeurt\n       --version              output version information and exit\n\n"

#define PR_TXT_AUTHORS           "Auteurs"
#define PR_TXT_CODER             "Programmeren en hoofdroutines"
#define PR_TXT_COD_ASSIST        "Hulp bij programmeren"
#define PR_TXT_GFX_COMP          "Grafische compressie-algoritmen"
#define PR_TXT_GFX_DEV           "Ontwikkeling grafisch formaat"
#define PR_TXT_MID               "Ontwikkeling MID-formaat"
#define PR_TXT_XML               "Resources.xml editor"
#define PR_TXT_TRANSLATION       "Vertaling"
#define PR_TXT_ABOUT_TRANSLATOR  "Tammo Jan Dijkema\n"

