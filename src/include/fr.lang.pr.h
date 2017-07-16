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

#define PR_ABOUT                  "Ressource de Princed (PR) "PR_VERSION""OS""PR_COPY""PR_URL""

#define PR_CGI_TEXT1              "Content-Type:text/html\n\nDémarrage en tant que cgi\n"
#define PR_CGI_TEXT2              "Resultat: Type %02d"

#define PR_TEXT_RESULT            "Resultat: %s (%d)\n"
#define PR_TEXT_RESULT_ERR        "Resultat: %d fichiers avec erreurs"

#define PR_TEXT_FILE_NOT_FOUND    "Result: Fichier ou répertoire '%s' non trouvé"

#define PR_TEXT_SCANNING_CURRENT  "Scan des fichiers dat du répertoire courant"
#define PR_TEXT_SCANNING_GIVEN    "Scan des fichiers dat du répertoire spécifié\n"

#define PR_TEXT_IMPORTING_CURRENT "Importation des fichiers dat valides du répertoire courant\n"
#define PR_TEXT_IMPORTING_GIVEN   "Importation des fichiers dat valides du répertoire spécifié\n"

#define PR_TEXT_TASK_COMPILE      "Importation de '%s' à partir de '%s'\n"
#define PR_TEXT_TASK_CLASSIFY     "Classification de '%s'\n"
#define PR_TEXT_TASK_EXTRACT      "Exportation de '%s' vers '%s'\n"

#define PR_TEXT_DISK_PROCESSING   "Traitement de '%s'...\n"
#define PR_TEXT_DISK_REPLACE      "Le fichier %s existe déjà. Le remplacer? [Oui/Non/Tous]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'A'

#define PR_TEXT_IMPORT_NOT_OPEN   "'%s' non ouvert, ignoré\n"
#define PR_TEXT_IMPORT_SUCCESS    "'%s' importé avec succès\n"
#define PR_TEXT_IMPORT_ERRORS     "'%s' a des erreurs, ignoré\n"
#define PR_TEXT_IMPORT_DONE       "Importation achevée, %d fichiers importé avec succès, %d fichiers importés avec erreurs\n"

#define PR_TEXT_EXPORT_WORKING    "'%s' exporté avec succès\n"
#define PR_TEXT_EXPORT_ERROR      "'%s' a des erreurs, annulation...\n"
#define PR_TEXT_EXPORT_BMP_WARN   "Attention: Le fichier extrait pourrait être corrompu\n"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"Ok",\
"Erreur durant l'accès à un fichier",\
"Erreur de mémoire dans l'extraction",\
"Fichier DAT invalide",\
"Erreur d'analyse XML",\
"Erreur de mémoire dans l'analyse",\
"Attribut XML non reconnu",\
"Fichier XML non trouvé"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Erreur de mémoire",\
"Fichier non trouvé ou fichier déjà en cours d'utilisation",\
"Pas un fichier DAT valide",\
"Fichier comprenant les niveaux",\
"Fichier DAT comprenant les graphiques sans palette",\
"Fichier comprenant les sons",\
"Fichier comprenant les midis",\
"DAT valide avec contenu indéfini",\
"Fichier DAT graphique",\
"Fichier DAT pour les sons du haut-parleur interne",\
"","","",\
"Fichier DAT de Pop2"}

#define PR_TEXT_IMPORT_ARRAY {\
"Fichier importé avec succès",\
"Fichier DAT non trouvé ou invalide dans l'importation partielle",\
"Fichier DAT File n'a pas pu être ouvert",\
"Erreur d'analyse XML",\
"Pas assez de mémoire",\
"Attribut XML non reconnu",\
"Fichier XML non trouvé"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_RAW "   -r, --raw                  utilise le format raw\n"
#else
#define PARSING_RAW ""
#endif
#define PARSING_HELP_BEGIN "Utilisation: \n\n  pr [-x[DOSSIER D'EXPORT]|-i[DOSSIER D'IMPORT]|-d] [CHEMIN DU FICHIER DAT]\n\n  pr [OPTIONS] [CHEMIN DU FICHIER DAT]\n\n\n\n  Les arguments obligatoires pour les options longues le sont aussi pour les options courtes.\n\n\n"
#define PARSING_HELP_PART1 "   -i, -c, --import[=DOSSIER] import de DOSSIER dans le fichier DAT spécifié\n\n   -d, --classify             renvoie le type de fichier DAT\n\n   -x, -e, --export[=DOSSIER] export du fichier DAT spécifié dans le répertoire DOSSIER\n\n\n"
#define PARSING_HELP_PART2 "   -a, --setauthor=NOM        définit votre nom dans les fichiers plv extraits\n\n   -b, --backup[=EXTENSION]   sauvegarde de vos fichiers\n\n   -f, --force                écrase vos fichiers sans confirmation\n\n   -g, --cgi                  démarre en tant que CGI et sort les en-têtes mime\n\n   -h, -?, --help             affiche cette aide et quitte\n\n   -s, --resource=RESFILE     utilise un fichier ressource XML specifique\n\n"PARSING_RAW

#define PARSING_HELP_PART3 "   -R, --recursive            cherche tous les fichiers DAT (seulement si FICHIERDAT\n\n                              est un fichier DAT valide)\n\n   -t, --datfile=FICHIERDAT   définit à un fichier DAT de lire les ressources\n\n                              différement que l'original\n\n       --unknown              génère un fichier 'unknown' sans réaliser\n\n                              une extraction\n\n   -v, --verbose              explique ce qui est en train d'être exécuté\n\n       --version              affiche les informations de version et quitte\n\n\n"

#define PR_TXT_AUTHORS           "Auteurs"
#define PR_TXT_CODER             "Cadage & routines principales"
#define PR_TXT_COD_ASSIST        "Assistant au codage"
#define PR_TXT_GFX_COMP          "Algorithmes de compression graphique"
#define PR_TXT_GFX_DEV           "Developpement du format graphique"
#define PR_TXT_MID               "Developpement du format son MID"
#define PR_TXT_XML               "Edition de Resources.xml"
#define PR_TXT_TRANSLATION       "Traduction"
#define PR_TXT_ABOUT_TRANSLATOR  "DEMBSKI Julien\n"

