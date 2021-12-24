# Build Princed Resources

## Unix like systems

GCC will work this way:

       $ make
       Compiling import module...
       ...
       Compiling directory and recursive file reading module for standard mode...
       Linking files...
       Program successfully compiled

       Please read the project README.md for usage information

       $ cd bin
       $ ./pr
       Princed resources (PR) V0.9 Linux
       (c) Copyright 2003 - Princed Development Team
       http://www.princed.org

       Usage: 
       pr [-x[EXTRACTDIR]|-c[COMPILEDIR]|-d] [DATFILEPATH]
       pr [OPTIONS] [DATFILEPATH]

       Mandatory arguments to long options are mandatory for short options too.

       -c, --import[=DIRNAME]     imports from DIRNAME into given dat file
       (...)
              --version              output version information and exit

## Win32 Console compiling

You can use the VC workspace file: pr.dsw and build the project (selecting
Build/Set active configuration...), setting up Dll or Release compilation
modes and pressing F7.

If you are using LCC, use Makefile.lcc to compile:
make -f Makefile.lcc build
Make sure you have added the lcc path in the system. For further
information read the shell information in the program documentation
at www.princed.org

## UNIX source build HowTo sample

       $ ls -l
       total 64
       -rw-r--r--    1 ecalot   users       57667 dec  9 10:00 pr.09-dev3.tar.bz2
       $ bunzip2 pr.09-dev3.tar.bz2
       $ tar -xjvf pr.10.tar.bz2
       src/
       src/extract.c
       ...
       src/pr.c

       $ cd src
       $ make
       Compiling import module...
       ...
       Compiling main module in library mode...
       Linking files...
       Making dynamic library...
       Program successfully compiled

       Please read the project README.md for usage information

       $ cd bin
       $ ls -l
       total 76
       -rwxr-xr-x    1 ecalot   users       32656 dic  9 10:02 pr
       -rwxr-xr-x    1 ecalot   users       42941 dic  9 10:02 pr.so

       $ ./pr
       Usage:
       pr [OPTIONS] [-x[EXPORTDIR]|-m[IMPORTDIR]|-c] [DATFILEPATH[@resource1[,resource2...]]]...
       pr --help
       pr --version
       ...

## Available make targets

       # To clean the object files type
       make clean

       # To build all the sources
       make build

       # To make the libraries
       make lib

       # To make the python module
       make pylib

## License

Princed Resources V1.0
(c) Copyright 2003, 2004, 2005, 2006 - Princed Development Team
http://www.princed.org
