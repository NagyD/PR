# Supported file formats

The extractor supports only a few file formats, it uses the most commonly
supported versions of the formats to allow maximum compatibility. 
As we haven't got enough time to code the program we decided to support
only those kinds of formats, and let the user make the necessary
conversions. See the table above and specifications for more information.

Supported resource file types

| Name      | Extension | Description |
|-----------|-----------|-------------|
| binary    | bin       | Binary files |
| image     | bmp       | Graphic in windows bitmap format |
| level     | plv       | Prince of Persia level |
| midi      | mid       | Game music in midi format |
| palette   | pal       | Image palette in the JASC format |
| pcspeaker | pcs       | Internal PC Speaker Sounds |
| text      | txt       | Text file |
| wave      | wav       | Wave digital audio sounds |

## level

 We decided to give Level files the extension ".plv".  These files
 can be edited in RoomShaker from version 1.66 and Princed V3.
 PV3 can also edit the whole levels.dat file.
 Format supported by: RoomShaker.

## image

We use Windows Bitmaps (BMP) to extract the files.
The file specifications are:

- 16 colour-palette in paletted images and a 2 colour-palette in B/W images
- No compression.

Format supported by: The Gimp, ImageMagick (command line convertor),
Paint Shop Pro, any other image editor.

## palette

For the bitmap palettes we are using the JASC Pal format (Not Microsoft .pal format)
Format supported by: Paint Shop Pro.

## wave

For digital audio we are using the standard .wav format.
The file specifications are:

- Size of Format: 16
- Format: PCM
- Attributes: 8 bit, mono, unsigned
- Channels: 1
- Sample rate: 11025
- Bytes/Sec: 11025
- Block Align: 1

Format supported by: SoX (command line convertor), GoldWave, CoolEdit (editors).

## midi

For musical instrument digital interface we are using the standard midi
format (mid) and type 0 midi format.
The file specifications are:

- Unknown (see standard MIDI specifications)

Format supported by: Power Tracks.

## pcspeaker

For the internal PC Speaker sound we are using binary data saved in pcs
format.
The format is saved this way:

1 unique byte as header
2 bytes per note (2 for frequency and 1 for duration)

Format supported by: not supported yet, use xvi32 or any other hex editor.

## binary

Binary garbage or untested stuff.
Format supported by: xvi32 or any other hex editor.
