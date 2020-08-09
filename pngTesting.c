#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "pngFuncs.h"

int main(void) {


	// sample data

	const int WIDTH = 3;
	const int HEIGHT = 2;

	const uint8_t MAX = 0xFF;
	const uint8_t PIXEL_ARRAY[] = {
		MAX,MAX,MAX, MAX,MAX,MAX, MAX,MAX,MAX,
		MAX,MAX,MAX, MAX,MAX,MAX, MAX,MAX,MAX,  
	
	};


	// open output file

	FILE *fout = fopen("png-test.png", "wb");
	if (fout == NULL) {
		printf("Error: Failed to open file\n");
		return 0;
	}


	// init output

	struct png pngout;
	enum pngStatus status = pngInit(&pngout, (uint32_t) WIDTH, (uint32_t) HEIGHT, fout);
	if(status != PNG_OK) {
		printf("Error: Bad status\n");
	}


	// write image data
	status = pngWrite(&pngout, PIXEL_ARRAY, (size_t)(WIDTH * HEIGHT));
	if (status != PNG_OK) {
		printf("Error: Bad status\n");
		return 0;
	}


	// close output file
	if (fclose(fout) != 0) {
		printf("Error: Failed to close file\n");
		return 0;
	}

	return 0;
}