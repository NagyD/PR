Attribute VB_Name = "pr"
'   Princed V3 - Prince of Persia Level Editor for PC Version
'   Copyright (C) 2003 Princed Development Team
'
'   This program is free software; you can redistribute it and/or modify
'   it under the terms of the GNU General Public License as published by
'   the Free Software Foundation; either version 2 of the License, or
'   (at your option) any later version.
'
'   This program is distributed in the hope that it will be useful,
'   but WITHOUT ANY WARRANTY; without even the implied warranty of
'   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'   GNU General Public License for more details.
'
'   You should have received a copy of the GNU General Public License
'   along with this program; if not, write to the Free Software
'   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
'
'   The authors of this program may be contacted at http://forum.princed.org


'
'pr.bas: Princed Resources : pr.dll declaration for Visual Basic
'¯¯¯¯¯¯
' Copyright 2003, 2004 Princed Development Team
'  Created: 24 Apr 2003
'
'  Author: Enrique Calot <ecalot.cod@princed.org>
'  Version: 1.00 (2003-Apr-24) for PR v0.6
'  Version: 2.00 (2004-Mar-03) for PR v1.0
'  Version: 2.01 (2004-Mar-23) for PR v1.0
'
' Note:
'  DO NOT remove this copyright notice
'
' Check for the latest version at http://www.princed.org


'Enumerations

'Resource type
Enum prResourceType
 prResMemoryError = -2
 prResFileNotFoundError = -1
 prResInvalidDatFile = 0
 prResLevelFile = 1
 prResGraphicExtFile = 2
 prResWaveFile = 3
 prResMidiFile = 4
 prResValidUnknownFile = 5
 prResGraphicFile = 6
 prResPCSpeaker = 7
 prResPop2Dat = 11
End Enum

Enum prFlags
 prFlagImport = 1
 prFlagClassify = 2
 prFlagExport = 4
 prFlagBackup = 8
 prFlagForce = 16
 prFlagCgi = 32
 prFlagHelp = 64
 prFlagRaw = 128
 prFlagRecursive = 256
 prFlagVerbose = 512
 prFlagVersion = 1024
 prFlagUnknown = 2048
End Enum


'Export
Enum prExportResult
 prExpXmlFileNotFound = -7
 prExpXmlAttributeNotRecognized = -6
 prExpMemoryErrorInParsinng = -5
 prExpXmlParseError = -4
 prExpInvalidDatFile = -3
 prExpMemoryError = -2
 prExpAccessDenied = -1
 prExpOk = 0
End Enum

'Import
Enum prImportResult
 prImpXmlFileNotFound = -6
 prImpXmlAttributeNotRecognized = -5
 prImpNoMemory = -4
 prImpXmlParseError = -3
 prImpInvalidInPartial = -2
 prImpAccessDenied = -1
 prImpOk = 0
 'Note: If the file was parcially compiled a positive number of files missing will be returned
End Enum

'This function will extract vDatFile into vDirName adding the extracted files in vResFilename
Public Declare Function prExportDat Lib "pr.dll" Alias "_prExportDat" ( _
 ByVal vDatFile As String, _
 ByVal vDirName As String, _
 ByVal vResFilename As String) As Integer

'This function will import the files located in vDirName into vDatFile acording to the information read in vResFilename
Public Declare Function prImportDat Lib "pr.dll" Alias "_prImportDat" (ByVal vDatFile As String, ByVal vDirName As String, ByVal vResFilename As String) As Integer
'This function will verify if the vDatFile exists, id it is a valid POP1 DAT file, and what kind of DAT file it is
Public Declare Function prVerifyDatType Lib "pr.dll" Alias "_prVerifyDatType" (ByVal vDatFile As String) As Integer

'  verifyDatType return values may be:
'    -2 PR internal Memory error (only if RAM is too low)
'    -1 File not found
'
'    00 Not a valid POP1 DAT file
'
'    01 Levels file
'    02 Graphic file with an image in the first valid entry (not common)
'    03 Waves/Digital sound file
'    04 Midis file
'    05 Valid DAT file with Undefined content (example ibmsnd for the moment)
'    06 Graphic file with a palette in the first valid entry (common)
'    07 Internal PC Speaker dat file
'    11 Pop2 dat file

' extended functions

Public Declare Function prVerifyDatTypeRes Lib "pr.dll" Alias "_prVerifyDatType" (ByVal vDatFile As String) As prResourceType

Public Declare Function prExportDatOpt Lib "pr.dll" Alias "_prExportDatOpt" ( _
 ByVal vDatFile As String, _
 ByVal vDirName As String, _
 ByVal vResFilename As String, _
 ByVal vExportOpt As prFlags, _
 ByVal vDatFileName As String, _
 ByVal datAuthor As String, _
 ByVal backupExtension As String) As prExportResult

'Arguments:
'            char* vDatFile        - full Path to the dat file;
'                                    if file use it, if directory, perform it for
'                                    All Files
'            char* vDirName        - full Path to the extracting folder
'                                    (doesn't need to exist)
'            char* vResFile        - full Path to the resources XML file
'                                    NULL is the default file resources.xml
'            char opt              - program options, see below
'            char * vDatFileName   - name of the file to be extracted
'                                    NULL means predict it from vDatFile
'            const char* datAuthor - Author's name when extracting PLV's,
'                                    NULL is default
'            const char* backupExtension
'                                  - If backup_flag is set, the string to attach
'                                    to the backup files
'Options:
'            unknown_flag   - generate the unknown file without performing
'                       any extraction
'            raw_flag       - uses raw format
'            verbose_flag   - explain what is being done
'            recursive_flag - searches for all dat files (only if vDatFile
'                             is not a dat file and vDatFileName is NULL)
'            force_flag     - default option, you cannot disable it,
'                             so please make a backup of your files
'            backup_flag    - backup your files
'        Return values:
'             0 Ok
'            -1 Error accessing the file DAT
'            -2 Memory error in extraction
'            -3 Invalid DAT file
'            -4 XML Parse error
'            -5 Memory error in parsing
'            -6 XML Attribute not recognized
'            -7 XML File not found

Public Declare Function prImportDatOpt Lib "pr.dll" Alias "_prImportDatOpt" ( _
 ByVal vDatFile As String, _
 ByVal vDirName As String, _
 ByVal vResFilename As String, _
 ByVal vImportOpt As prFlags, _
 ByVal vDatFileName As String, _
 ByVal backupExtension As String) As prImportResult

'   Arguments:
'    char* vDatFile        - full Path to the dat file;
'                            if file use it, if directory, perform it for
'                            all files
'    char* vDirName        - full Path to the extracting folder
'                            (doesn't need to exist)
'    char* vResFile        - full Path to the resources XML file
'                            (resources.xml by default)
'    char opt              - program options, see below
'    char * vDatFileName   - name of the file to be extracted
'                            NULL means predict it from vDatFile
'    const char* backupExtension
'                          - If backup_flag is set, the string to attach
'                            to the backup files
'
'   Options:
'    raw_flag       - uses raw format
'    verbose_flag   - explain what is being done
'    recursive_flag - searches for all dat files (only if vDatFile
'                     is not a dat file and vDatFileName is NULL)
'    force_flag     - If not set and the file exists it will prompt
'                     for action
'    backup_flag    - backup your files
'
'   Return values:
'    -1 DAT File couldn't be open for writing
'    -2 XML Parse error
'    -3 No memory
'    -4 XML Attribute not recognized
'    -5 XML File not found
'    00 File successfully imported
'    positive number: number of missing files

'type tResource as record
'type tResources as tResource(2000)

'Public Declare Function parseFile Lib "pr.dll" Alias "_parseFile" (ByVal vFile as string, byval datFile as string, byref resources as tResources) as integer
Public Declare Function parseStructure Lib "pr.dll" Alias "_parseStructure" (ByVal vFile As String) As Integer

Public Declare Function resourceTreeGetChild Lib "pr.dll" Alias "_resourceTreeGetChild" (ByVal whereAmI As Integer) As Integer
Public Declare Function resourceTreeGetNext Lib "pr.dll" Alias "_resourceTreeGetNext" (ByVal whereAmI As Integer) As Integer
Public Declare Function resourceTreeGetRoot Lib "pr.dll" Alias "_resourceTreeGetRoot" () As Integer
Public Declare Function resourceTreeGetInfo Lib "pr.dll" Alias "_resourceTreeGetInfo" (ByVal whereAmI As Integer, ByRef tag As Integer, ByRef desc As String, ByRef path As String, ByRef file As String, ByRef itemtype As String, ByRef name As String, ByRef palette As String, ByRef restype As String, ByRef value As String, ByRef version As String, ByRef number As String) As Integer

Public Declare Sub prSetOutput Lib "pr.dll" Alias "_prSetOutput" (ByVal stream As Any)
Public Declare Sub freeParsedStructure Lib "pr.dll" Alias "_freeParsedStructure" ()

