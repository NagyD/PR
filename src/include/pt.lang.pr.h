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
����
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

#define PR_CGI_TEXT1              "Conte�do-Tipo:texto/html\n\nA executar como cgi\n"
#define PR_CGI_TEXT2              "Resultado: tipo %02d \n"

#define PR_TEXT_RESULT            "Resultado: %s (%d)\n"
#define PR_TEXT_RESULT_ERR        "Resultado: %d ficheiros com erros\n"

#define PR_TEXT_FILE_NOT_FOUND    "Resultado: Ficheiro ou directoria '%s' n�o encontrado\n"

#define PR_TEXT_SCANNING_CURRENT  "A procurar ficheiros dat na directoria actual\n"
#define PR_TEXT_SCANNING_GIVEN    "A procurar ficheiros dat na directoria especificada\nA procurar ficheiros dat na directoria actual\nA procurar ficheiros dat na directoria especificada"

#define PR_TEXT_IMPORTING_CURRENT "A importar todos os ficheiros dat v�lidos da directoria actual\n"
#define PR_TEXT_IMPORTING_GIVEN   "A importar todos os ficheiros v�lidos da directoria especificada"

#define PR_TEXT_TASK_COMPILE      "A importar '%s' de '%s'\n"
#define PR_TEXT_TASK_CLASSIFY     "A classificar '%s'\n"
#define PR_TEXT_TASK_EXTRACT      "A exportar '%s' para '%s'\n"

#define PR_TEXT_DISK_PROCESSING   "A processar '%s'...\n"
#define PR_TEXT_DISK_REPLACE      "O ficheiro %s j� existe. Substituir? [Sim/n�o/todos]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'A'

#define PR_TEXT_IMPORT_NOT_OPEN   "'%s' n�o aberto, ignorado\n"
#define PR_TEXT_IMPORT_SUCCESS    "'%s' importado com sucesso"
#define PR_TEXT_IMPORT_ERRORS     "'%s' tem erros, ignorado"
#define PR_TEXT_IMPORT_DONE       "Importa��o completa, %d ficheiros importados sem problemas, %d ficheiros com erros\n"

#define PR_TEXT_EXPORT_WORKING    "'%s' exportado com sucesso"
#define PR_TEXT_EXPORT_ERROR      "'%s' tem erros, a abortar...\n"
#define PR_TEXT_EXPORT_BMP_WARN   "Aviso: Ficheiro extra�do pode estar corrompido\n"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"Ok",\
"Erro ao aceder ao ficheiro",\
"Erro de mem�ria na extrac��o",\
"Ficheiro DAT inv�lido",\
"Erro na an�lise de XML",\
"Erro de mem�ria na an�lise",\
"Atributo XML n�o reconhecido",\
"Ficheiro XML n�o encontrado"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Erro de mem�ria",\
"Ficheiro n�o encontrado ou erro de acesso",\
"N�o � um ficheiro DAT v�lido",\
"Ficheiro dos n�veis",\
"Ficheiro DAT gr�fico sem palete",\
"Ficheiro de som Wave/Digital",\
"Ficheiro MIDI",\
"Ficheiro DAT v�lido com conte�do indefinido",\
"Ficheiro DAT gr�fico",\
"Ficheiro dat da Coluna Interna do PC (PC Internal Speaker)",\
"","","",\
"Ficheiro dat de Pop2"}

#define PR_TEXT_IMPORT_ARRAY {\
"Ficheiro importado com sucesso",\
"Ficheiro DAT n�o encontrado ou inv�lido na importa��o parcial",\
"O ficheiro DAT n�o p�de ser aberto",\
"Erro na an�lise XML",\
"Sem mem�ria",\
"Atributo XML n�o reconhecido",\
"Ficheiro XML n�o encontrado"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_RAW "   -r, --raw                  usa formato raw\n"
#else
#define PARSING_RAW ""
#endif
#define PARSING_HELP_BEGIN "Utiliza��o: \n\n  pr [-x[EXPORTDIR]|-i[IMPORTDIR]|-d] [DATFILEPATH]\n\n  pr [OPTIONS] [DATFILEPATH]\n\n\n\n  Os argumentos mandat�rios para as op��es longas s�o tamb�m mandat�rios para as op��es curtas.\n\n\n"
#define PARSING_HELP_PART1 "   -i, -c, --import[=DIRNAME] importa de DIRNAME (nome da directoria) para o ficheiro dat\n                              especificado\n\n   -d, --classify             volta ao tipo de ficheiro dat\n\n   -x, -e, --export[=DIRNAME] Exporta o ficheiro dat especificado para DIRNAME\n\n\n"
#define PARSING_HELP_PART2 "   -a, --setauthor=NAME       atribui o teu nome nos ficheiros plv extra�dos\n\n   -b, --backup[=EXTENSION]   faz c�pia de seguran�a dos teus ficheiros\n\n   -f, --force                reescreve nos teus ficheiros sem perguntar\n\n   -g, --cgi                  corre como CGI e "output mime"\n   -h, -?, --help             mostra esta ajuda e sai\n\n   -s, --resource=RESFILE     usa um ficheiro xml especificado pelo utilizador\n\n"PARSING_RAW

#define PARSING_HELP_PART3 "   -R, --recursive            procura por todos os ficheiros dat(s� se DATFILEPATH\n\n                              n�o � um ficheiro dat)\n\n   -t, --datfile=DATFILE      especifica um ficheiro dat para ler recursos\n\n                              diferente do ficheiro original\n\n       --unknown              gera o ficheiro desconhecido sem executar\n\n                              nenhuma extrac��o\n\n   -v, --verbose              explica o que est� a ser feito\n\n       --version              informa��o da vers�o output e sai\n\n\n"

#define PR_TXT_AUTHORS           "Autores"
#define PR_TXT_CODER             "Codifica��o & pr�ticas principais"
#define PR_TXT_COD_ASSIST        "Assistente de codifica��o"
#define PR_TXT_GFX_COMP          "Algoritmos de compress�o gr�fica"
#define PR_TXT_GFX_DEV           "Desenvolvimento do formato gr�fico"
#define PR_TXT_MID               " Desenvolvimento do formato de som MID"
#define PR_TXT_XML               "Edi��o de Resources.xml "
#define PR_TXT_TRANSLATION       "Tradu��o"
#define PR_TXT_ABOUT_TRANSLATOR  "Undo (Nuno Raimundo)"

