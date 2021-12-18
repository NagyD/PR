# Princed Resources: Frequent Asked Questions

- What extraction formats are supported?

    [Check the supported data formats](Dataformats.md)

- I'm importing an image but it got corrupted in the game, why?

    Perhaps this is a bug, we aren't perfect. Anyway, be careful to keep the format options in the extracted file (don't change the number of colour sets, format compression, etc). We invite you to contribute and report the bug, in that case please check that you are having the latest version and fill in the bug report form from [http://project.princed.org](http://project.princed.org).

- I'm importing an image but PR doesn't want to import it?

    First use the -v or --verbose option to see what image is that. Then check if the image exists and, if you are in unix-like systems, check case of the file, remember some operating systems are case sensitive. If the image exists, check that this image has a palette of 16 colours and the format is correct. Some images are in a two-color format, make sure that images aren't one of them (check them in resources.xml if palette is set to monochrome).

- I don't understand the palettes!

    First of all, remember we made the editor, not the dat format, so there are some things we would change if we where the game coders, for example this one. The DAT format supports only one palette for a group of images. This means that if you want to change the palette for one image you have to change all other images' palettes. Read the questions below to understand it.

- So, how can I edit a palette of a group of images?

    You have to edit the palette of one image and export it to the PAL file that represent this group. For example for all images in vdungeon.dat the palette is called dungeon.pal. If you want to know what palette belongs to what image, just take a look at resources.xml. Note that the palettes in each BMP files are just for viewing purposes, changing them won't change the original palette; the only way to change the original palette is changing the PAL file. The import tool will always ignore the palettes for all BMPs files, and use the PAL file. Make sure to use the JASC PAL format (supported by Paint Shot Pro) because other formats like MS PAL aren't supported.

- Ok, I changed the palette file, now what?

    If you want all the BMP files you are editing to be updated with the new palette, just import the file and re export it again. The new palette will be propagated to all the files. Now you can edit the files using the palette that will be displayed in the game.

- I replaced the PAL file and now all the images have the colours swapped. Any suggestions?

    Remember that the palette has an index order, you must keep this order if you want the images to be displayed correctly in the game. To prevent/solve this problem you have to fix the palette order, the easy way is applying the new palette using the nearest colour method to the affected images before the palette changing. A palette will be exported rightfully only if the colour indexes are in the same order as the PAL file, to check this you can apply the palette using the palette index number method.

- Is there any definition loss?

    Yes JASC palettes has 24 bit RGB values (16,7 millions of colours), DAT palettes has 18 bit RGB values (262144 colours). Colours are approached to the nearest 18-bit colour  equivalence. For example the RGB colour (123,122,121) will became (124,124,120) using multiples of 4. This is the only definition loss. After this approaching is done the first time, you can export and import a DAT file all the times you want and the definition will be the same.

- I'm not sure where to find the correct PAL file for an image

    This is the hardest way, but if you insist... Open resources.xml with a simple text editor (vim, notepad, TextPad, etc). Look for the file name of the BMP image. This will appear in an item tag like this:

        <item value="478" path="chopped.bmp"> Prince Chopped</item>
        
        This item belongs to a bigger item, in that case 

            <folder path="deaths">
        
        This means that the file is in the deaths folder. This item is in a bigger item section like this:
        
            <folder name="Our prince and mouse images" file="kid.dat" path="kid" palette="400" itemtype="image">
            
        Now we know that the folder that contained the image is in directory "kid" and its palette value is 400. Then just look at a resource where value="400" inside the folder of the same file, in this case file="kid.dat", so we'll find
        
            <item value="400" path="kid.pal" itemtype="palette">Resource number 400</item>
        
        This means that a palette value 400 is correctly a palette and the path name is kid.pal.

- How can I edit resources.xml?

    Open it, read instructions and comments written there and happy editing.

- How can I edit PCS files?

    We will release our little piano program some day! Anyway, readme.txt explains the format.

- How can I edit the other guards?

    All guards are sharing the BMP files, and have different palettes, so you can only edit the palettes. If you really want to edit them, there is a file in prince.dat called guard palettes.ext. Using a [hex editor](http://www.chmaas.handshake.de/delphi/freeware/xvi32/xvi32.htm) you may edit it this way: There are 16 colours per guard, each colour has 3 bytes. So a guard has 48 bytes (the file has 7 guards and 336 bytes). The colour index numbers are in order. So if you want to locate the colour 4 of the guard type 3 it is:

        3 * colourNumber + 48 * guardNumber = 3 * 4 + 48 * 3 = 156

    Remember the enumeration starts from 0. A colour field of 3 bytes has this format: one byte for the Red saturation value, one for the green and one for the blue following that order. The values are unsigned and from 0 to 64. So you have to multiply this values by 4 to get the palette saturation value. For example the 3 bytes that starts in 156 are 1E 17 0E in hex and 30, 23, 14 in decimal. 30*4=120; 23*4=92; 14*4=56; so the RGB value of the fifth colour (indexed 4) of the palette of the fourth guard (3 if we count the first guard as 0) is (120,92,56). Note they are all multiples of 4. Maybe some day we will include a support of this file in PR.

- I improved a bit resources.xml in order to make easier my editing

    What are you waiting? Send it! We may include it in the next release!

- I edited resources.xml but now my compiled files are broken

    Use the provided resources.xml's, we have tested them a lot. In case you invested too much time in that and you don't want it to be wasted, send it, we have developed some tools to check, detect errors and fix broken resources.xml. Remember: Don't delete items you don't know what are them for, if one item is missing or has another identification value, all imported DAT files using this item may be broken. Even ext files are needed.

- PR doesn't work in my game set?

    Current resources.xml file only supports POP 1.4. You may want to make your own resources.xml for your version, in that case, empty the current resources.xml you have, only leave the resources and ?xml version tags and export the dat files. Exported files will be auto detected and saved in the unknown folder with the unknown.xml file. There it is the information of your dat file, you can add it to the resources.xml you made and edit the descriptions, etc..

- Ok, I've finished a graphic or level set, can I publish it?

    Yes, of course, it's your work and you can publish. We ask you to publish it under the GPL or any compatible license. We have a site where we centralized all the level, graphic ans sound sets made for Prince of Persia, we recommend you to publish it there because it is there where the people will look first. The site is [http://www.princed.org/artworks](http://www.princed.org/artworks).

- I Hex-edited a DAT file and now prince of persia asks me for the disks. How did you do to avoid this problem?

    Work, work and more work. Each resource has a checksum, there is an index for each dat file, the format is very complicated. If something is wrong the game will ask for the installation disks. Hex-editing a dat file without any experience or reading any documentation could become a bit frustrating and damage your dat files. We spent a lot of time getting this error until we discovered how to calculate the checksums, how the index works, how to locate it, etc. We are working in a DAT file format specification for those people who wanna know how he format is. Anyways we highly recommend coders to use our libraries. We don't want to make people work in something that was already done, so we opened the source and you are able to look into and edit it under the terms of the GPL.

- How to contact us?

    Here: [http://forum.princed.org](http://forum.princed.org)

- What features has PR got?

    PR was designed for three basic functions:

    - Extracting DAT files into directories
    - Importing directories into DAT files
    - Getting information on DAT files

- What exactly is extracting a DAT file?

    When you have a file (e.g. vdungeon.dat) and you want the resources that are saved inside to be saved in directories in known formats (e.g. bmp)

- What is resources.xml?

    Is a file that saves easy names and descriptions for the content of the DAT files. When the extracted resources are re-imported the filenames are read from here.

- What is to import a directory?

    When you are reading a resource directory (e.g. a directory with the bmp or wav you need to make the DAT file) to make a DAT file, you are importing this directory. You need to have all resources there, in any other case the DAT file won't work fine in the game.

- When you said the third basic function you said "Getting information on DAT files", what does exactly that mean?

    Getting the DAT file type. e.g. vdungeon is image, digisnd is wave.

- I want the program to make backups of the edited files. Is it possible?

    Yes, run PR with the -b or --backup option and specify an extension (e.g. bak) and all files will be backuped in files with this name. (e.g. compiling a new vdungeon.dat will save the old file as vdungeon.dat.bak)

- I don't want the program to ask if I want to replace a file, what can I do?

    Use the -f or --force option.

- I'm a web coder and I want to classify a file that is in the server

    Run the program in the --cgi mode and capture the output in order to parse it. Take a look at the Princed Uploading System (that is under the GPL) at the Princed home page; this system uses PR to classify uploaded DAT files.

- I'm extracting a level set (levels.dat) and I want to set the author of the extracted PLV levels to me

    Use PR with the -a or --setauthor option and add a name to save in the author field of the PLV information. If you don't do that, the default author name "PR user" will be saved instead.

- Can I use another resources.xml file?

    If there is another newer version of the resources xml file, it is recommended to upgrade PR. But if you want to, you can rename your file to resources.xml or use the -m or --resource directive.

- What is the raw option?

    This is for format developers. But if you are interested, DAT files are archives, like tar and zip files. The files are stored indexed and compressed. The raw option exports or imports the files from the archive without performing any type of conversion or compression/decompression, so the extracted file is in the format the game uses (not compatible in most cases).

- The program seems to work, but the result is an 8 bytes-length file.

    Upgrade PR

- The program seems to work, but the result is a file that doesn't work.

    Try the -v or --verbose option, and check if all files are Ok. The verbose option has lots of extra information.

- Is it possible to extract all dat files that are in all subdirectories of the specified directory?

    Use PR -R or --recursive (take care with the upper case). This will recourse into directories and check out all dat files located in this directory and subdirectories.

- What is the --unknown option for?

    Hey, if you want to be a resources.xml writer, just contact us!

- I'm working on a file called dungeonbyjohn.dat, and I don't want to rename it to vdungeon.dat each time I do an extraction. How can I do?

    Use the -t or --datfile option. That is what it was made for. For example, if you want dungeonbyjohn.dat to be treated like vdungeon.dat, the valid syntax should be:

        pr -tvdungeon.dat -xdirectory dungeonbyjohn.dat

## License

(c) 2003 - Copyleft Princed Development Team
http://www.princed.org

Last update: 2005-06-08 23:11 UTC-3
