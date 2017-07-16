Princed Resources V1.0
(c) Copyright 2003, 2004, 2005, 2006 - Princed Development Team
http://www.princed.org
This program is open source under the GNU General Public License terms

1) Unix-based OS compiling:

GCC will work this way:

-shell-2.05b$ make
Compiling import module...
Compiling export module...
Compiling resource manager module...
Compiling classification module...
Compiling disk access library...
Compiling DAT editing library...
Compiling bitmap files support (bmp)...
Compiling MIDI audio files support (mid)...
Compiling JASC palette support (pal)...
Compiling digital wave audio support (wav)...
Compiling prince level files support (plv)...
Compiling memory manager...
Compiling main library primitives for both modes...
Compiling XML parsing module...
Compiling XML search features...
Compiling compression module...
Compiling LZG compression module...
Compiling LZG uncompression module...
Compiling RLE compression module...
Compiling RLE uncompression module...
Compiling command parsing module for standard mode...
Compiling directory and recursive file reading module for standard mode...
Linking files...
Program successfully compiled

Please read readme.txt for syntax information

-shell-2.05b$ cd bin
-shell-2.05b$ ./pr
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

-shell-2.05b$

2) Win32 Console compiling

You can use the VC workspace file: pr.dsw and build the project (selecting
Build/Set active configuration...), setting up Dll or Release compilation
modes and pressing F7.

If you are using LCC, use Makefile.lcc to compile:
 make -f Makefile.lcc build
Make sure you have added the lcc path in the system. For further
information read the shell information in the program documentation
at www.princed.org

3) CVS download

Anonymous CVS access to SourceForge.net will work. Just type:

 cvs -d:pserver:anonymous@cvs.fp.princed.org:/cvsroot/freeprince login
Hit enter when prompted for password
 cvs -d:pserver:anonymous@cvs.fp.princed.org:/cvsroot/freeprince co PR

Sample:

-shell-2.05b$ cvs -d:pserver:anonymous@cvs.fp.princed.org:/cvsroot/freeprince login
Logging in to :pserver:anonymous@cvs.fp.princed.org:2401/cvsroot/freeprince
CVS password:
-shell-2.05b$ cvs -d:pserver:anonymous@cvs.fp.princed.org:/cvsroot/freeprince co PR
cvs server: Updating PR
cvs server: Updating PR/doc
U PR/doc/changelog.txt
U PR/doc/editionfaq.htm
(..)
U PR/winbins/pr.ico
U PR/winbins/pr.lib
-shell-2.05b$

4) Need assistance?
 Just ask here
 http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598846

5) Bug reporting?
 http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598845

6) Feature requests?
 http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598848

7) SourceForge project home page
 http://project.princed.org

8) UNIX source build HowTo sample

-shell-2.05b$ ls -l
total 64
-rw-r--r--    1 ecalot   users       57667 dec  9 10:00 pr.09-dev3.tar.bz2
-shell-2.05b$ bunzip2 pr.09-dev3.tar.bz2
-shell-2.05b$ ls
pr.09-dev3.tar
-shell-2.05b$ tar -xjvf pr.10.tar.bz2
src/
src/extract.c
src/include/
src/include/compile.h
src/include/compress.h
src/include/extract.h
src/include/memory.h
src/include/pr.h
src/include/resources.h
src/include/tasks.h
src/include/bmp.h
(...)
src/formats/pal.c
src/formats/wav.c
src/formats/plv.c
src/resources.c
src/tasks.c
src/xml.c
src/xmlsearch.c
src/pr.dsp
src/pr.c
-shell-2.05b$ cd src
-shell-2.05b$ make
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
Compiling main module in library mode...
Linking files...
Making dynamic library...
Program successfully compiled

Please read readme.txt for syntax information

-shell-2.05b$ cd bin
-shell-2.05b$ ls -l
total 76
-rwxr-xr-x    1 ecalot   users       32656 dic  9 10:02 pr
-rwxr-xr-x    1 ecalot   users       42941 dic  9 10:02 pr.so
-shell-2.05b$ ./pr
Princed resources (PR) v1.0 GNU/Linux
(c) Copyright 2003 - 2005 Princed Development Team
http://www.princed.org

Usage:
  pr [OPTIONS] [-x[EXPORTDIR]|-m[IMPORTDIR]|-c] [DATFILEPATH[@resource1[,resource2...]]]...
  pr --help
  pr --version

  Mandatory arguments to long options are mandatory for short options too.

   -i, -m, --import[=DIRNAME] imports from DIRNAME into given dat file
   -c, --classify             returns the dat file type
   -e, -x, --export[=DIRNAME] exports given dat file into DIRNAME

   -a, --setauthor=NAME       sets your name in extracted plv files
   -b, --backup[=EXTENSION]   backup your files
   -f, --force                rewrites your files without prompting
   -g, --cgi                  run as CGI and output mime headers
   -h, -?, --help             display this help and exit
   -s, --resource=RESFILE     uses an user-specific resource xml file
   -R, --recursive            searches for all dat files (only if DATFILEPATH
                              is not a dat file)
   -t, --datfile=DATFILE      specifies a dat file to read resources
                              different than the original file
   -z, --compression-level=N  a number from 1 to 7 specifying how hard has PR
                              to try to reduce the generated dat files.
                              1 is high speed, low compression, 7 is high
                              compression but low speed.
       --unknown              generate the unknown file without performing
                              any extraction
   -v, --verbose              explain what is being done
       --version              output version information and exit

-shell-2.05b$

9) Make rules

To clean the object files type
 make clean

To build all the sources
 make build

To make only the touched sources
 make

To make the libraries
 make lib

To make the python module
 make pylib

