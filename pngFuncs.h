/* thanks to https://www.nayuki.io/page/tiny-png-output 
 * for help with the png section of this project
 */
#ifndef PNGFUNCS_H
#define PNGFUNCS_H

#include "main.h"


struct png {

	// config
	uint32_t width;						// measured in pixels
	uint32_t height;					// measured in pixels
	uint32_t line_size;				    // measured in bytes; equivilent to width * 3 + 1

	// running state
	FILE *output;
	uint32_t x_pos; 					// next byte index in current line
	uint32_t y_pos;						// line index of next byte
	uint32_t remaining_uncompressed;	// number of uncompressed bytes remaining
	uint16_t deflate_filled;			// bytes filled in the current block
	uint32_t crc;						// for IDAT chunk
	uint32_t adler;						// for DEFLATE data in IDAT

};


enum pngStatus {
	PNG_OK,
	PNG_IO_ERROR,
};


/*
 * png writer with given width and height, and byte output stream
 */
enum pngStatus pngInit(struct png this[static 1], uint32_t w, uint32_t h, FILE out[static 1]);


/* 
 * writes pixels from given array to the output stream, in 3 byte chunks
 * pixels are presented from top to bottom, left to right, with subpixels in
 * RGB order
 */
enum pngStatus pngWrite(struct png this[static 1], const uint8_t pixels[], size_t count);

// returns whether the data write was sucessful
static bool write(struct png this[static 1], const uint8_t data[], size_t len);

// updates the CRC checksum based on new data 
static void crc32(struct png this[static 1], const uint8_t data[], size_t len);

// updates the adler32 checksum based on new data
static void  adler32(struct png this[static 1], const uint8_t data[], size_t len);

static void putBigUint32(uint32_t val, uint8_t array[static 4]);


#endif