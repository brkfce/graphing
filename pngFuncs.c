/* thanks to https://www.nayuki.io/page/tiny-png-output 
 * for help with the PNG section of this project
 */
#include "main.h"
#include "pngFuncs.h"

/*
 * convert data to pixel array
 */



/*
 * create png from pixel array
 */
static const uint16_t DEFLATE_MAX_BLOCK_SIZE = 65535;	// max size of block for DEFLATE compression

enum pngStatus pngInit(struct png this[static 1], uint32_t w, uint32_t h, FILE out[static 1]) {

	// no checks required for size values, as PNG size will not be changable by the user
	this->width = w;
	this->height = h;

	// compute data sizes
	uint64_t line_sz = (uint64_t) this->width * 3 + 1;
	this->line_size = line_sz;

	uint64_t rm_uncomp = this->line_size * this->height;
	this->remaining_uncompressed = (uint32_t) rm_uncomp;

	uint32_t block_count = this->remaining_uncompressed / DEFLATE_MAX_BLOCK_SIZE;
	if (this->remaining_uncompressed % DEFLATE_MAX_BLOCK_SIZE != 0) {
		block_count++;	// round up
	}
	// 5 bytes per DEFLATE uncompressed block header, 2 bytes for zlib header, 4 bytes for zlib Adler-32 footer
	uint64_t idat_size = (uint64_t) block_count * 5 + 6;
	idat_size += this->remaining_uncompressed;

	// write header
	uint8_t header[] = { // 43 bytes
		// PNG header
		0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
		// IHDR chunk
		0x00, 0x00, 0x00, 0x0D,
		0x49, 0x48, 0x44, 0x52,
		0, 0, 0, 0,  // 'width' placeholder
		0, 0, 0, 0,  // 'height' placeholder
		0x08, 0x02, 0x00, 0x00, 0x00,
		0, 0, 0, 0,  // IHDR CRC-32 placeholder
		// IDAT chunk
		0, 0, 0, 0,  // 'idat_size' placeholder
		0x49, 0x44, 0x41, 0x54,
		// DEFLATE data
		0x08, 0x1D,
	};
	putBigUint32(this->width, &header[16]);
	putBigUint32(this->height, &header[20]);
	putBigUint32(idat_size, &header[33]);
	this->crc = 0;
	crc32(this, &header[12], 17);
	putBigUint32(this->crc, &header[29]);
	this->output = out;
	if (!write(this, header, sizeof(header) / sizeof(header[0]))) {
		return PNG_IO_ERROR;
	}
	this->crc = 0;
	crc32(this, &header[37], 6);
	this->adler = 1;

	this->x_pos = 0;
	this->y_pos = 0;
	this->deflate_filled = 0;
	return PNG_OK;
}

enum pngStatus pngWrite(struct png this[static 1], const uint8_t pixels[], size_t count) {

	count *= 3;	// convert pixel count to byte count
	while (count > 0) {
		if (this->deflate_filled == 0) { // start DEFLATE block
			uint16_t size = DEFLATE_MAX_BLOCK_SIZE;
			if (this->remaining_uncompressed < size) {
				size = (uint16_t) this->remaining_uncompressed;
			}
			const uint8_t header[] = { // 5 bytes long
				(uint8_t)(this->remaining_uncompressed <= DEFLATE_MAX_BLOCK_SIZE ? 1: 0),
				(uint8_t)(size >> 0),
				(uint8_t)(size >> 8),
				(uint8_t)((size >> 0) ^ 0xFF),
				(uint8_t)((size >> 8) ^ 0xFF),
			};
			if (!write(this, header, sizeof(header) / sizeof(header[0]))) {
				return PNG_IO_ERROR;
			}
			crc32(this, header, sizeof(header) / sizeof(header[0]));
		}
		// missing assert here, might be necessary

		if (this->x_pos == 0) { // beginning of line, write filter method byte
			uint8_t b[] = {0};
			if (!write(this, b, sizeof(b) / sizeof(b[0]))) {
				return PNG_IO_ERROR;
			}
			crc32(this, b, 1);
			adler32(this, b, 1);
			this->x_pos++;
			this->remaining_uncompressed--;
			this->deflate_filled++;
		}
		else { // write some pixel bytes for the current line
			uint16_t n = DEFLATE_MAX_BLOCK_SIZE - this->deflate_filled;
			if (this->line_size - this->x_pos < n) {
				n = (uint16_t)(this->line_size - this->x_pos);
			}
			if (count < n) {
				n = (uint16_t) count;
			}
			// missing assert here, might be necessary
			if(!write(this, pixels, n)) {
				return PNG_IO_ERROR;
			}

			// update checksums
			crc32(this, pixels, n);
			adler32(this, pixels, n);

			// increment positions
			count -= n;
			pixels += n;
			this->x_pos += n;
			this->remaining_uncompressed -= n;
			this->deflate_filled += n;
		}

		if (this->deflate_filled >= DEFLATE_MAX_BLOCK_SIZE) {
			this->deflate_filled = 0;	// end current block
		}

		if (this->x_pos == this-> line_size) { // increment line
			this->x_pos = 0;
			this->y_pos ++;
			if (this->y_pos == this->height) { // reached end of pixels
				uint8_t footer[] = { // 20 bytes
					0, 0, 0, 0, 	 // DEFLATE Adler-32 placeholder
					0, 0, 0, 0,		 // IDAT CRC-32 placeholder
					// IEND Chunk
					0x00, 0x00, 0x00, 0x00,
					0x49, 0x45, 0x4E, 0x44,
					0xAE, 0x42, 0x60, 0x82,
				};
				putBigUint32(this->adler, &footer[0]);
				crc32(this, &footer[0], 4);
				putBigUint32(this->crc, &footer[4]);
				if (!write(this, footer, sizeof(footer) / sizeof(footer[0]))) {
					return PNG_IO_ERROR;
				}
			}
		}
	}
	return PNG_OK;
}

// returns whether the write was sucessful
static bool write(struct png this[static 1], const uint8_t data[], size_t len) {
	return fwrite(data, sizeof(data[0]), len, this->output) == len;
}

// updates the CRC checksum based on new data 
static void crc32(struct png this[static 1], const uint8_t data[], size_t len) {
	this->crc = ~this->crc;
	for (size_t i = 0; i < len; i++) {
		for (int j = 0; j < 8; j++) {
			uint32_t bit = (this->crc ^ (data[i] >> j)) & 1;
			this->crc = (this->crc >> 1) ^ ((-bit) & UINT32_C(0xEDB88320));
		}
	}
	this->crc = ~this->crc;
}

// updates the adler32 checksum based on new data
static void  adler32(struct png this[static 1], const uint8_t data[], size_t len) {
	uint32_t s1 = this->adler & 0xFFFF;
	uint32_t s2 = this->adler >> 16;
	for (size_t i = 0; i < len; i++) {
		s1 = (s1 + data[i]) % 65521;
		s2 = (s2 + s1) % 65521;
	}
	this->adler = s2 << 16 | s1;
}

static void putBigUint32(uint32_t val, uint8_t array[static 4]) {
	for (int i = 0; i < 4; i++) {
		array[i] = (uint8_t)(val >> ((3-i) * 8));
	}
}
