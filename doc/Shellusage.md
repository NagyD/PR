# PR Compilation, shell commands and running samples

## Shell Example of LCC Compilation under Win32

    C:\princed\src> set path=%path%;c:\lcc\bin

    C:\princed\src> path
    PATH=C:\WINDOWS;C:\WINDOWS\COMMAND;**C:\LCC\BIN**

    C:\princed\src> ren makefile makefile.gcc

    C:\princed\src> ren makefile.lcc makefile

    C:\princed\src> make
    Compiling import module...
    Compiling compression module...
    Compiling export module...
    Compiling main module in standard mode...
    Compiling resource manager module...
    Compiling extra tasks module...
    Compiling disk access functions...
    Compiling xml parsing module...
    Compiling xml search features...
    Compiling bitmap files support (bmp)...
    Compiling midi audio files support (mid)...
    Compiling JASC palette support (pal)...
    Compiling digital wave audio support (wav)...
    Compiling prince level files support (plv)...
    Compiling memory manager...
    Porting the dirent structure...
    Porting the Unix-like getopt function (first part)...
    Porting the Unix-like getopt function (second part)...
    Linking files...
    **Program successfully compiled**

    Please read the project README.md for usage information

    Time: 9.89 seconds

## Shell Example of a normal GCC Compilation under several unix-like OS

    $ make
    Compiling import module...
    Compiling compression module...
    Compiling export module...
    Compiling resource manager module...
    Compiling extra tasks module...
    Compiling disk access functions...
    Compiling xml parsing module...
    Compiling xml search features...
    Compiling bitmap files support (bmp)...
    Compiling midi audio files support (mid)...
    Compiling JASC palette support (pal)...
    Compiling digital wave audio support (wav)...
    Compiling prince level files support (plv)...
    Compiling memory manager...
    Compiling main module in standard mode...
    Uncompressing resources...
    Linking files...
    Program successfully compiled

    Please read the project README.md for usage information

## Clearing object files

    $ make clean
    Erasing temporary object files...

## Build Example

    $ make build
    Erasing temporary object files...
    Compiling import module...
    Compiling compression module...
    Compiling export module...
    Compiling resource manager module...
    Compiling extra tasks module...
    Compiling disk access functions...
    Compiling xml parsing module...
    Compiling xml search features...
    Compiling bitmap files support (bmp)...
    Compiling midi audio files support (mid)...
    Compiling JASC palette support (pal)...
    Compiling digital wave audio support (wav)...
    Compiling prince level files support (plv)...
    Compiling memory manager...
    Compiling main module in standard mode...
    Linking files...
    Program successfully compiled

    Please read the project README.md for usage information

## Problems with old Make versions. Very common in BSD systems

    $ make
    "Makefile", line 9: Missing dependency operator
    "Makefile", line 13: Need an operator
    "Makefile", line 16: Need an operator
    make: fatal errors encountered -- cannot continue

    $ mv Makefile Makefile.new
    $ mv Makefile.old Makefile
    $ make
    Compiling import module...
    Compiling compression module...
    Compiling export module...
    Compiling resource manager module...
    Compiling extra tasks module...
    Compiling disk access functions...
    Compiling xml parsing module...
    Compiling xml search features...
    Compiling bitmap files support (bmp)...
    Compiling midi audio files support (mid)...
    Compiling JASC palette support (pal)...
    Compiling digital wave audio support (wav)...
    Compiling prince level files support (plv)...
    Compiling memory manager...
    Porting the Unix-like getopt function (first part)...
    Porting the Unix-like getopt function (second part)...
    Compiling main module in standard mode...
    Linking files...
    Program successfully compiled

    Please read the project README.md for usage information

    Compiling main module in library mode...
    Making dynamic library...
    Library successfully compiled

    Please read readme.coders.txt and pr.h for interfaces

## PR Running Example

    $ cd bin
    $ ./pr
    Princed resources (PR) V0.9-R2 FreeBSD
    (c) Copyright 2003 - Princed Development Team
    http://www.princed.org

    Usage:
      pr [-x[EXTRACTDIR]|-c[COMPILEDIR]|-d] [DATFILEPATH]
      pr [OPTIONS] [DATFILEPATH]

      Mandatory arguments to long options are mandatory for short options too.

      -c, --import[=DIRNAME]     imports from DIRNAME into given dat file
      -d, --classify             returns the DAT file type
      -x, -e, --export[=DIRNAME] extracts given dat file into DIRNAME

      -a, --setauthor=NAME       sets your name in extracted PLV files
      -b, --backup[=EXTENSION]   backup your files
      -f, --force                default option, you cannot disable it,
                                  so please make a backup of your files
      -g, --cgi                  run as CGI and output mime headers
      -h, -?, --help             display this help and exit
      -m, --resource=RESFILE     uses an user-specific resource xml file
      -r, --raw                  uses raw format
      -R, --recursive            searches for all dat files (only if DATFILEPATH
                                  is not a dat file)
      -t, --datfile=DATFILE      specifies a dat file to read resources
                                  different than the original file
          --unknown              generate the unknown file without performing
                                  any extraction
      -v, --verbose              explain what is being done
          --version              output version information and exit

## Version about

    $ ./pr --version
    Princed resources (PR) V0.9-R2 NetBSD
    (c) Copyright 2003 - Princed Development Team
    http://www.princed.org

    Authors:
      Coding & main routines
        Enrique Calot
        Santiago Zamora

      Graphic compression algorithms
        Tammo Jan Dijkema
        Enrique Calot

      Graphic format development
        Tammo Jan Dijkema
        Anke Balderer

      MID Sound format development
        Christian Lundheim

      Resources.xml edition
        Steven Fayers

## Windows running examples

    C:\princed\src\bin> pr -xdir c:\prince

    Princed resources (PR) V0.9-R2 Win32
    (c) Copyright 2003 - Princed Development Team
    http://www.princed.org

    Scanning dat files in given directory
    Extracting 'c:\prince\CDUNGEON.DAT' to 'dir'
    Result: Ok (0)
    Extracting 'c:\prince\CPALACE.DAT' to 'dir'
    Result: Ok (0)
    ...
    Result: Ok (0)

    C:\princed\src\bin> cd dir

    C:\princed\src\bin\dir> dir

    Drive C label is PRTEST
    Serial number id 1380-0FE3
    Directory of C:\princed\src\bin\dir

    .              <DIR>        02-15-04 10:37p .
    ..             <DIR>        02-15-04 10:37p ..
    UNKNOWN        <DIR>        02-15-04 10:37p unknown
    DIGISND1       <DIR>        02-15-04 10:37p digisnd1
    DIGISND2       <DIR>        02-15-04 10:37p digisnd2
    DIGISND3       <DIR>        02-15-04 10:37p digisnd3
    FAT            <DIR>        02-15-04 10:37p fat
    GUARDS         <DIR>        02-15-04 10:37p guards
    IBM_SND1       <DIR>        02-15-04 10:37p ibm_snd1
    IBM_SND2       <DIR>        02-15-04 10:37p ibm_snd2
    KID            <DIR>        02-15-04 10:37p kid
    MIDISND1       <DIR>        02-15-04 10:37p midisnd1
    MIDISND2       <DIR>        02-15-04 10:37p midisnd2
    PRINCE         <DIR>        02-15-04 10:37p prince
    PV             <DIR>        02-15-04 10:37p pv
    SHADOW         <DIR>        02-15-04 10:38p shadow
    SKEL           <DIR>        02-15-04 10:38p skel
    TITLE          <DIR>        02-15-04 10:38p title
    VDUNGEON       <DIR>        02-15-04 10:38p vdungeon
    VIZIER         <DIR>        02-15-04 10:38p vizier
    VPALACE        <DIR>        02-15-04 10:38p vpalace
    LEVELS         <DIR>        02-15-04 10:38p levels
            0 files                 0 bytes
            22 directories     394,055,680 free bytes

**Note**: now you can edit the files in each of those directories

    C:\princed\src\bin\dir> cd ..

    C:\princed\src\bin> pr -cdir c:\prince

    Princed resources (PR) V0.9-R2 Win32
    (c) Copyright 2003 - Princed Development Team
    http://www.princed.org

    Importing all valid files from given directory
    Compiling 'c:\prince\title.dat' from 'dir'
    File title.dat already exists. Replace? [Y/n/a]:y
    Result: File succesfully compiled (0)
    Compiling 'c:\prince\vdungeon.dat' from 'dir'
    File vdungeon.dat already exists. Replace? [Y/n/a]:y
    Result: File succesfully compiled (0)
    Compiling 'c:\prince\vpalace.dat' from 'dir'
    File vpalace.dat already exists. Replace? [Y/n/a]:a
    Result: File succesfully compiled (0)
    Compiling 'c:\prince\pv.dat' from 'dir'
    Result: File succesfully compiled (0)
    ...
    Compiling 'c:\prince\levels.dat' from 'dir'
    Result: File succesfully compiled (0)

## License

(c) 2003 - Copyleft Princed Development Team
           http://www.princed.org

Last update: 2004-02-18 15:06 UTC-3
