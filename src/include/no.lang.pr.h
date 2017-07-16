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

#define PR_ABOUT                  "Princed resources (PR) "PR_VERSION""OS""PR_COPY""PR_URL""

#define PR_CGI_TEXT1              "Innholds-Type:text/html\n\nKjører som cgi\n"
#define PR_CGI_TEXT2              "Resultat: %02d type\n"

#define PR_TEXT_RESULT            "Resultat: %s (%d)\n"
#define PR_TEXT_RESULT_ERR        "Resultat: %d filer med feil\n"

#define PR_TEXT_FILE_NOT_FOUND    "Result: Filen eller katalogen '%s' ble ikke funnet\n"

#define PR_TEXT_SCANNING_CURRENT  "Sjekker dat filene i katalogen\n"
#define PR_TEXT_SCANNING_GIVEN    "Sjekker dat filene i den valgte katalogen\n"

#define PR_TEXT_IMPORTING_CURRENT "Importerer alle dat filer fra den valgte katalogen"
#define PR_TEXT_IMPORTING_GIVEN   "Importerer alle filene fra den valgte katalogen\n"

#define PR_TEXT_TASK_COMPILE      "Importerer '%s' fra '%s'\n"
#define PR_TEXT_TASK_CLASSIFY     "Klassifiserer '%s'\n"
#define PR_TEXT_TASK_EXTRACT      "Eksporterer '%s' til '%s'\n"

#define PR_TEXT_DISK_PROCESSING   "Behandler '%s'...\n"
#define PR_TEXT_DISK_REPLACE      "Filen %s finnes allerede. Overskrive?  [Ja/nei/alt]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'A'

#define PR_TEXT_IMPORT_NOT_OPEN   "'%s' ikke åpen, hoppet over\n"
#define PR_TEXT_IMPORT_SUCCESS    "'%s' impotering vellykket"
#define PR_TEXT_IMPORT_ERRORS     "'%s' har feil, hoppet over\n"
#define PR_TEXT_IMPORT_DONE       "Importering ferdig, %d filene importert ok, %d filene med feil\n"

#define PR_TEXT_EXPORT_WORKING    "'%s' eksportering vellykket"
#define PR_TEXT_EXPORT_ERROR      "'%s' har feil, avbryter...\n"
#define PR_TEXT_EXPORT_BMP_WARN   "Advarsel: Utpakket fil kan være skadet\n"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"Ok",\
"Feil med tilgang til fil",\
"Minne feil i utpakking",\
"Feil DAT fil",\
"XML Behandlings feil",\
"Minne feil ved behandling",\
"XML Attributter ikke gjenkjent",\
"XML Filen ble ikke funnet"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Minne feil",\
"Filen ikke funnet eller ingen tilgang",\
"Ikke en godkjent DAT fil",\
"Brett fil",\
"Grafikk DAT fil uten mønster",\
"Wav/Digital lyd fil",\
"Midi fil",\
"Valgt DAT fil med ukjent innhold",\
"Grafikk DAT fil",\
"Intern PC Høytaler dat fil",\
"","","",\
"Pop2 dat fil"}

#define PR_TEXT_IMPORT_ARRAY {\
"Importering av fil vellykket",\
"DAT fil ikke funnet eller ukjent innhold",\
"DAT Filen kunne ikke åpnes",\
"XML Behandlings feil",\
"Ingen minne",\
"XML Attributer ikke gjenkjent",\
"XML Filen ikke funnet"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_RAW "   -r, --raw                  bruker raw formatet\n"
#else
#define PARSING_RAW ""
#endif
#define PARSING_HELP_BEGIN "Bruk: \n\n  pr [-x[EXPORTDIR]|-i[IMPORTDIR]|-d] [DATFILEPATH]\n\n  pr [OPTIONS] [DATFILEPATH]\n\n\n\n  Mandatory arguments to long options are mandatory for short options too.\n\n\n"
#define PARSING_HELP_PART1 "   -i, -c, --import[=DIRNAME] importerer fra KATALOGNAVN til valgt dat fil\n\n   -d, --classify             returnere til dat filtype\n\n   -x, -e, --export[=DIRNAME] eksportere valgt dat fil til KATALOGNAVN\n\n\n"
#define PARSING_HELP_PART2 "   -a, --setauthor=NAME       setter navnet ditt i utpakkede plv filer\n\n   -b, --backup[=EXTENSION]   tar sikkerhetskopi av dine filer\n\n   -f, --force                omskriver filene dine uten å spørre\n\n   -g, --cgi                  kjører som CGI og lagrer i ny data\n\n   -h, -?, --help             viser denne hjelpen og avslutter\n\n   -s, --resource=RESFILE     bruker en bruker-spesifikk resurs xml file\n\n"PARSING_RAW

#define PARSING_HELP_PART3 "   -R, --recursive            leter etter alle dat filer (kun hvis DATFILEPATH\n\n                              ikke er en dat fil)\n\n   -t, --datfile=DATFILE      spesifiserer en dat fil som skal lese resurser\n\n                              fra annet en den orginal filen\n\n       --unknown              genererer den ukjente filen uten\n\n                              å utpakking\n\n   -v, --verbose              forklar hva som blir gjort\n\n       --version              fremstill versjons informasjon og avslutt\n\n\n"

#define PR_TXT_AUTHORS           "Forfattere"
#define PR_TXT_CODER             "Koding & hovedrutiner"
#define PR_TXT_COD_ASSIST        "Kodings assistent"
#define PR_TXT_GFX_COMP          "Grafikk kompresjons utførelser"
#define PR_TXT_GFX_DEV           "Grafikk format utvikling"
#define PR_TXT_MID               "MID Lyd format utvikling"
#define PR_TXT_XML               "Resurser.xml versjon"
#define PR_TXT_TRANSLATION       "Oversetting"
#define PR_TXT_ABOUT_TRANSLATOR  "Thelootmaster"

