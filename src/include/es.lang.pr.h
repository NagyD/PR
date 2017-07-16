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

#define PR_ABOUT                  "Princed Resources"PR_VERSION""OS""PR_COPY""PR_URL""

#define PR_CGI_TEXT1              "Content-Type:text/html\n\nCorriendo como CGI"
#define PR_CGI_TEXT2              "Resultado: %02d type"

#define PR_TEXT_RESULT            "Resultado: %s (%d)"
#define PR_TEXT_RESULT_ERR        "Resultados: %d archivos con errores"

#define PR_TEXT_FILE_NOT_FOUND    "Resultado: File or directory '%s' not found"

#define PR_TEXT_SCANNING_CURRENT  "Buscando archivos dat en el directorio actual"
#define PR_TEXT_SCANNING_GIVEN    "Buscando archivos dat en el directorio dado"

#define PR_TEXT_IMPORTING_CURRENT "Importando todos los archivos dat válidos del directorio actual"
#define PR_TEXT_IMPORTING_GIVEN   "Importando todos los archivos dat válidos del directorio dado"

#define PR_TEXT_TASK_COMPILE      "Importando '%s' desde '%s'"
#define PR_TEXT_TASK_CLASSIFY     "Clasificando '%s'"
#define PR_TEXT_TASK_EXTRACT      "Exportando '%s' a '%s'"

#define PR_TEXT_DISK_PROCESSING   "Procesando '%s'..."
#define PR_TEXT_DISK_REPLACE      "El archivo %s ya existía. ¿Reemplazar? [Si/no/todos]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'A'

#define PR_TEXT_IMPORT_NOT_OPEN   "'%s' no abierto, ignorado"
#define PR_TEXT_IMPORT_SUCCESS    "'%s' importado correctamente"
#define PR_TEXT_IMPORT_ERRORS     "'%s' tiene errores, ignorado"
#define PR_TEXT_IMPORT_DONE       "Importación terminada, %d archivos correctos, %d archivos con errores"

#define PR_TEXT_EXPORT_WORKING    "'%s' exportado correctamente"
#define PR_TEXT_EXPORT_ERROR      "'%s' tiene errores, abortando..."
#define PR_TEXT_EXPORT_BMP_WARN   "Advertencia: El archivo extraido puede estar corrompido"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"Ok",\
"Error accediendo al archivo",\
"Error de memoria en la extracción",\
"Archivo DAT inválido",\
"Error de codificación XML",\
"Error de memoria en la decodificación",\
"Atributo XML no reconocido",\
"El archivo XML no fue encontrado"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Error de memoria",\
"Error por archivo no encontrado o acceso denegado",\
"Archivo DAT no válido",\
"Archivo de Niveles",\
"Archivo DAT de imágenes sin paleta",\
"Archivo de sonido de Ondas/Digital",\
"Archivo de sonido MIDI",\
"Archivo DAT válido con contenido indefinido",\
"Archivo DAT de imágenes",\
"Archivo DAT de sonido del parlante interno",\
"","","",\
"Archivo DAT del Pop2"}

#define PR_TEXT_IMPORT_ARRAY {\
"Archivo importado correctamente",\
"Archivo DAT no encontrado o inválido en importación parcial",\
"El archivo DAT no pudo ser abierto",\
"Error de decodificación XML",\
"Sin memoria",\
"Atributo XML no reconocido",\
"Archivo XML no encontrado"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_RAW "   -r, --raw                  utiliza formato crudo"
#else
#define PARSING_RAW ""
#endif
#define PARSING_HELP_BEGIN "Modo de uso: \n  pr [-x[EXPORTDIR]|-i[IMPORTDIR]|-d] [DATFILEPATH]\n  pr [OPTIONS] [DATFILEPATH]\n\n  Los argumentos mandatorios en las opciones largas lo son tambien para las cortas.\n"
#define PARSING_HELP_PART1 "   -i, -c, --import[=DIRNAME] importa desde DIRNAME al archivo dat dado\n   -d, --classify             retorna el tipo del archivo dat\n   -x, -e, --export[=DIRNAME] exportas el archivo dat dado a DIRNAME\n"
#define PARSING_HELP_PART2 "   -a, --setauthor=NAME       utiliza NAME en los archivos plv extraidos\n   -b, --backup[=EXTENSION]   hace copias de seguridad\n   -f, --force                reescribe los archivos sin preguntar\n   -g, --cgi                  corre como CGI e imprime encabezados mime\n   -h, -?, --help             muestra esta ayuda y sale\n   -s, --resource=RESFILE     usa el archivo XML de recursosuses especificado\n"PARSING_RAW

#define PARSING_HELP_PART3 "   -R, --recursive            busca todos los archivos dat en el directorio\n                              (solo si DATFILEPATH no es un archivo dat)\n   -t, --datfile=DATFILE      especifica un archivo DAT para leer recursos\n                              diferente del original\n       --unknown              genera el archivo de recursos desconocidos sin\n                              realizar extracción alguna\n   -v, --verbose              explica que se está haciendo\n       --version              imprime infirmación sobre la versión y sale\n"

#define PR_TXT_AUTHORS           "Autores"
#define PR_TXT_CODER             "Programación y rutinas principales"
#define PR_TXT_COD_ASSIST        "Asistente de programación"
#define PR_TXT_GFX_COMP          "Algoritmos de compresión de imagenes"
#define PR_TXT_GFX_DEV           "Desarrollo del formato de imagenes"
#define PR_TXT_MID               "Desarrollo del Formato MID"
#define PR_TXT_XML               "Edición de Resources.xml"
#define PR_TXT_TRANSLATION       "Traducción"
#define PR_TXT_ABOUT_TRANSLATOR  "Enrique Calot"

