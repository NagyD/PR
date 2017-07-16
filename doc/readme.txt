Princed Resources v1.1
(c) Copyright 2003 - 2006 - Princed Development Team
http://www.princed.org
This program is open source under the GNU General Public License terms

Please read this readme file thoroughly before starting to use Princed
Resource Editor.

1) Supported file formats:

 The extractor supports only a few file formats, it uses the most commonly
 supported versions of the formats to allow maximum compatibility. 
 As we haven't got enough time to code the program we decided to support
 only those kinds of formats, and let the user make the necessary
 conversions. See the table above and specifications for more information.

Supported resource file types:
 Name       Extension  Description
 ~~~~       ~~~~~~~~~  ~~~~~~~~~~~
 binary     bin        Binary files
 image      bmp        Graphic in windows bitmap format
 level      plv        Prince of Persia level
 midi       mid        Game music in midi format
 palette    pal        Image palette in the JASC format
 pcspeaker  pcs        Internal PC Speaker Sounds
 text       txt        Text file
 wave       wav        Wave digital audio sounds

Levels:
 * Type "level":
 We decided to give Level files the extension ".plv".  These files
 can be edited in RoomShaker from version 1.66 and Princed V3.
 PV3 can also edit the whole levels.dat file.
 Format supported by: RoomShaker.

Images:
 * Type "image":
 We use Windows Bitmaps (BMP) to extract the files.
 The file specifications are:
  - 16 colour-palette in paletted images and a 2 colour-palette in B/W images
  - No compression.
 Format supported by: The Gimp, ImageMagick (command line convertor),
                      Paint Shop Pro, any other image editor.

 * Type "palette":
 For the bitmap palettes we are using the JASC Pal format (Not Microsoft
  .pal format)
 Format supported by: Paint Shop Pro.

Sound:
 * Type "wave":
 For digital audio we are using the standard .wav format.
 The file specifications are:
  - Size of Format: 16
  - Format: PCM
  - Attributes: 8 bit, mono, unsigned
  - Channels: 1
  - Sample rate: 11025
  - Bytes/Sec: 11025
  - Block Align: 1
 Format supported by: SoX (command line convertor), GoldWave,
                      CoolEdit (editors).

 * Type "midi":
 For musical instrument digital interface we are using the standard midi
 format (mid) and type 0 midi format.
 The file specifications are:
  - Unknown (see standard MIDI specifications)
 Format supported by: Power Tracks.

 * Type "pcspeaker":
 For the internal PC Speaker sound we are using binary data saved in pcs
 format.
 The format is saved this way:
  1 unique byte as header
  3 bytes per note (2 for frequency and 1 for duration)
 Format supported by: not supported yet, use xvi32 or any other hex editor.

Binary:
 * Type "binary":
 Binary garbage or untested stuff.
 Format supported by: xvi32 or any other hex editor.

2) Bugs:
 All known bugs were solved. If you find a bug (program crashes, corrupted
 resources, typos, etc), it's very probable that we haven't found it. So
 please report it. To report bugs just post them in
 http://sourceforge.net/tracker/?func=add&group_id=91915&atid=598845

3) Usage:
 Run the program without parameters to get the usage help.
 
 Syntax samples:
 
 C:\PRINCED\PR> pr -eext DAT\vdungeon.dat
 
 The above win32 example will export the bitmaps and palettes stored in 
 C:\princed\pr\DAT\vdungeon.dat into the c:\princed\pr\ext folder.
 
 C:\PRINCE> pr -e vdungeon.dat
 
 This will export all the bitmaps and palettes stored in
 c:\prince\vdungeon.dat into the folder c:\prince.
 
 C:\PRINCED\PR> pr -iext compiled\vdungeon.dat
 
 This will import the bitmaps and palettes that are in c:\princed\pr\ext
 and associated to vdungeon.dat in the resource file and generate the file
 c:\princed\pr\compiled\vdungeon.dat.

 C:\PRINCED\PR> pr -eext compiled\vpalace.dat@161
 
 This will export the resource number 161 to the directory according to
 resources.xml (c:\princed\pr\ext\prince\sword\in the floor\bright.bmp).

 $ ./pr -eext FP/prince.dat@162:pop1 -f -v

 This UNIX example will export the resource 162 from the index "pop1"
 forcing overwrite and being verbose.

 $ ./pr -eimages 'FP/prince.dat@prince/sword/*.bmp'

 This is to export all bitmaps from prince/sword directory to the
 images/prince/sword directory.
 Note: the quotation marks '' are only to avoid wildcard expansions and
  won't be needed in win32 consoles.

 $ ./pr -efpdats FP

 Exports all DAT files in FP to fpdats directory.

 $ ./pr -efpdats FP -r

 The same, but looking into sub-directories.

 $ ./pr -e FP/v*.DAT

 Exports to the current folder all DAT files in FP whose name starts with v.
 Note: Only works in shells with wildcard expansion support.

 $ ./pr -eimg FP/prince.dat FP/pv.dat

 Exports to img the content of both files prince.dat and pv.dat located at the folder FP.

 $ ./pr -iimg FP/prince.dat@161

 Imports back to (updates) FP/prince.dat only the resource 161 located in the directory img.
 
 $ ./pr -z7 -iimg 'FP/prince.dat@/prince/sword/*.bmp'

 Updates the current bmp resources in img/prince/sword into the DAT file FP/prince.dat 
 using compression level 7 (slow but very effective).

 $ ./pr -ximages 'FP/vdungeon.dat@/vdungeon/chopper/frame*.bmp,/vdungeon/spikes/frame01.bmp,1310'

 Exports to images folder the resources matching with vdungeon/chopper/frame*.bmp,
 the resource vdungeon/spikes/frame01.bmp and the resource number 1310.
 
 $ ./pr -xpop2 pop2/bird.dat@/bird/long_jump/frame06.bmp,751:shape -v -f

 Exports the file bird/long_jump/frame06.bmp and the resource 751 indexed in "shap" from
 pop2/bird.dat to the pop2 folder.

 $ ./pr --xml-optimize=test/resources.xml

 Optimizes the file test/resources.xml performing the following tasks:
  - add doctype declaration
  - indent tags and arrange line separation
  - reduce file size deleting inheritable attributes
  - remove comments

 $ ./pr --unknown prince2
 
 Reads all DAT information from the prince2 directory and then creates
 the file unknown/unknown.xml and fills it with the items that were not
 inside the resources.xml file.
 
4) Requirements
  
  Approximately 25Mb in RAM are required for a normal full directory task
  
  Recommended:
   AMD K6-II at 350Mhz or higher
   32 Mb RAM
	
  Benchmark results:
   Performed on an AMD Athlon(tm) XP 2000+ with 1667.022 MHz
   The file tested was the original vdungeon.dat, sized 14220 bytes

  Normal benchmark results:
   Compression level 1. Size 38031 bytes. Time 0.028137 seconds.
   Compression level 2. Size 23756 bytes. Time 0.031273 seconds.
   Compression level 3. Size 15912 bytes. Time 0.031836 seconds.
   Compression level 4. Size 12519 bytes. Time 0.066246 seconds.
   Compression level 5. Size 11347 bytes. Time 0.087975 seconds.
   Compression level 6. Size 11218 bytes. Time 1.506183 seconds.
   Compression level 7. Size 10932 bytes. Time 2.916612 seconds.

5) Credits

 Coding & main routines
  Enrique Calot
  
 Coding Assistant
  Santiago Benito-Zamora

 Graphic compression algorithms
  Tammo Jan Dijkema
  Enrique Calot
  Diego Essaya

 Graphic format development
  Tammo Jan Dijkema
  Anke Balderer

 MID Sound format development
  Christian Lundheim

 Resources.xml edition
  Steven Fayers
  Piotr Kochanek

Type pr --version to see translation credits for an specific language 
Pr is also using some GPL compatible and GPL components made by
«Kevlin Henney» (dirent structure port for windows) and «the Free Software
Foundation, Inc.» (getopt parsing functions ports).
Special thanks to:
 - the authors of Valgrind and GDB for the great debugging features.
 - the authors of GCC and LCC for making excellent free compilers.
 - SourceForge.net for the home page hosting and the development
   environment provided.
 - Lugfi for all the support. 

Please enjoy using Princed Resource Editor!

