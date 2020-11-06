# CAFF Previewer

C program that extracts a single frame out off a CAFF file. And saves it in TARGA format.

## Build

In order to build CAFF Previewer all you need is cmake, make and a C compiler. No libraries required other than the standard C libraries.

In order to build the software, first clone it from git, cd into it and then use the following commands:
```shell script
cmake .
make -j4
```
After this the `caff_previewer` binary should appear in the working directory.

**Note**: Adjust a job count that suits your platform better.

## Usage

There are a few things that simpler than using CAFF Previewer. All you have to do is provide the input and the output file name and path as command line arguments. That's all.

```shell script
./caff_previewer test.caff test.tga
```

The successfulness of the procedure is indicated by the exit code of the program.

## Exit codes
If something goes wrong, or the file to be parsed is invalid, CAFF previewer returns an exit code that tells the exact problem. Read more about error codes in the Contributing section. Here is a list of the currently effective exit codes:

`0x00` File read failures:
 - `0x01` FILE_READ_COULD_NOT_OPEN
 - `0x02` FILE_READ_TOO_BIG
   - The maximum file size this program can parse is currently limited to 512MB. This error indicates an input file larger than this limit.
 - `0x03` FILE_READ_COULD_NOT_ALLOCATE
   - Memory allocation failed for reading the input file.
 - `0x04` FILE_SEEK_ERROR
   - Error while seeking the file. (File not seekable?)
 - `0x05` FILE_READ_ERROR
   - The amount of bytes read is not equal to the file size .
 
 `0x10` CAFF format violations
 
 - `0x11` CAFF_PARSE_LENGTH_ERROR
   - Something is not right with the length of variable fields in the file.
 - `0x12` CAFF_PARSE_BAD_FRAME
   - Invalid frame id found
 - `0x13` CAFF_PARSE_HEADER_PLACE_ERROR
   - The first frame is not a header type, or a header type found in another frame.
 - `0x14` CAFF_PARSE_ANIMATION_COUNT_ERROR
   - The number of animation frames declared in the header and the actual number of animation frames does not match up.
 - `0x15` CAFF_PARSE_NO_CREDITS_ERROR
   - The file does not contain a credits type frame.
 - `0x16` CAFF_PARSE_HEADER_BAD_MAGIC
   - The header frame have bad magic.
 - `0x17` CAFF_PARSE_CREDITS_BAD_DATE
   - The date provided in the credits frame is invalid (e.g.: 14th month or 61st minute).
 - `0x18` CAFF_PARSE_ANIMATION_NO_DURATION
   - An animation frame have zero duration.
 - `0x19` CAFF_PARSE_UNKNOWN_ERROR
   - The CAFF file seemed valid, yet an animation frame could not be read.
 
 `0x20` CIFF format violations
 
 - `0x21` CIFF_PARSE_LENGTH_ERROR
   - Something is not right with the length of variable fields in the file.
 - `0x22` CIFF_PARSE_HEADER_DIMENSIONS_INCORRECT
   - The image have 0 as width or height (or both)
 - `0x23` CIFF_PARSE_HEADER_BAD_MAGIC
   - The header of the CIFF image have bad magic.
 - `0x24` CIFF_PARSE_UNKNOWN_ERROR
   - A variable field have invalid contents.
 
 `0x30` Image flipping failure

 - `0x31` IMAGE_FLIP_LENGTH_ERROR
   - The image size precheck is failed. (`w*h*3 != size`)
 - `0x32` IMAGE_FLIP_COULD_NOT_ALLOCATE
   - Couldn't allocate memory for the flipped image.
 
 `0x40` LibTarga errors
 
 - `0x41` TGA_ERR_BAD_HEADER
 - `0x42` TGA_ERR_OPEN_FAILS
 - `0x43` TGA_ERR_BAD_FORMAT
 - `0x44` TGA_ERR_UNEXPECTED_EOF
 - `0x45` TGA_ERR_NODATA_IMAGE
 - `0x46` TGA_ERR_COLORMAP_FOR_GRAY
 - `0x47` TGA_ERR_BAD_COLORMAP_ENTRY_SIZE
 - `0x48` TGA_ERR_BAD_COLORMAP
 - `0x49` TGA_ERR_READ_FAILS
 - `0x4a` TGA_ERR_BAD_IMAGE_TYPE
 - `0x4b` TGA_ERR_BAD_DIMENSIONS
   - image has size 0 width or height (or both)
 

`0x50` Usage errors

 - `0x51` USAGE_ERROR_WRONG_PARAMETERS
   - Wrong amount of command line arguments provided. See Usage section.
 

## Contributing

CAFF Previewer uses several code conventions in order to keep the code simple and maintainable. The most notable of those conventions are described in the followings.

### Error codes
Functions that perform a complex task that can fail in multiple ways are always have to return with a status code which have `uint8_t` type. This status code have to be `0x00` on success. Any output of the function must be returned using pointers in the parameters. The status code is almost always used as a exit code for the program, so the users can easily identify what went wrong.

In case of a code indicates failure the code should be unique among the whole program. In order to aid achiving this, The error code is devidede to two 4 bit part. The first 4 bit refers to a certain module of the software, the last 4 bit identifies the exact problem.

### MagicMemory
Each memory allocation call must be performed trough the MagicMemory library. All context must be freed up before the program exits.

If a function allocates memory and returns with it, then it should take a `mm_ctx` type MagicMemory context as it's first parameter and perform the allocation inside that context.

### Parser and validator
There two kind of functions that performs data validation. Validator and parser. A validator only validates data according to an assumption of it's format, the parser does the same plus extracting some fields from the data after validation. 

It's not unusual for a parser function to call a validator function to validate it's whole input before extracting the fields. 

### stdint
In the whole program (where possible) `stdint` types have to be used in order to be clear and precise about each integer's size and signedness. 

### Endianness
Currently this program is designed to run on Big-Endian platforms only (such as x86) therefore no extra code should be written to achieve supporting Little-endian platforms. 

## Credits

CAFF Previewer use the LibTarga library by Alex Mohr: https://research.cs.wisc.edu/graphics/Gallery/LibTarga/