#include "libeasybmp.h"
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>

int main(void) {
	struct BMP bmp;
	if (bmp_init(&bmp, 256, 256) != STATUS_OK) {
		fprintf(stderr, "Failed to initialize BMP memory.\n"); 
		free(bmp.pixel_data);
		return 1;
	}
	
	uint8_t *scanline = bmp_get_scanline(&bmp, 1); 
	if (scanline == NULL)
		fprintf(stderr, "Scanline was NULL.\n");
	else 
		printf("Pixel data address: %p\nScanline 1 address: %p\n", bmp.pixel_data, scanline);

	/*  Write 2x2 small BMP
	// Place a red pixel in the first byte of the top row
	scanline[0] = 0; 
	scanline[1] = 0;
	scanline[2] = 255; 

	// Place a blue pixel in the second byte of the top row
	scanline[3] = 255; 
	scanline[4] = 0;
	scanline[5] = 0; 

	// Add the padding (it should already be there, but whatever)
	scanline[6] = 0;
	scanline[7] = 0; 
	*/  
	// Write big 256x256 BMP (all red) 
	for (uint32_t i = 0; i < 256; i++) { 		// scanline iterator
		for (uint32_t j = 0; j < 256; j++) {    // column iterator  
			// My function uses RGB order for parameters, but adheres 
			// to BGR order for BMP
			bmp_put_pixel_at_coordinate(&bmp, i, j, 255, 0, 0); 	
		}
	}
	
	// Write to file
	FILE *fd = fopen("big_example.bmp", "wb");
	if (fd == NULL) {
		fprintf(stderr, "Failed to obtain file descriptor for \"big_example.bmp\".\n");
		free(bmp.pixel_data);
		return 1;
	}
	printf("[DEBUG] File properties...\n- file size: %u\n- image size: %u\n- padding per scanline: %hu\n- width: %u\n- height: %u\n- size (of infoheader): %u\n- planes: %hu\n- bits per pixel: %hu\n- compression: %u\n- x pixels per meter: %u\n- y pixels per meter: %u\n- colors used: %u\n- important colors: %u\n- Pixel data (address): %p\n", bmp.file_size, bmp.image_size, bmp.padding_per_scan_line_in_bytes, bmp.width, bmp.height, bmp.size, bmp.planes, bmp.bits_per_pixel, bmp.compression, bmp.x_pixels_per_meter, bmp.y_pixels_per_meter, bmp.colors_used, bmp.important_colors, bmp.pixel_data);
	
	// Write file 
	fwrite(&(bmp.signature),           1, 2,              fd);	     
	fwrite(&(bmp.file_size),           4, 1,              fd);
	fwrite(&(bmp.reserved),            4, 1,              fd);
	fwrite(&(bmp.data_offset),         4, 1,              fd);
	fwrite(&(bmp.size),                4, 1,              fd);
	fwrite(&(bmp.width), 	           4, 1,              fd);
	fwrite(&(bmp.height),              4, 1,              fd);
	fwrite(&(bmp.planes),              2, 1,              fd);
	fwrite(&(bmp.bits_per_pixel),      2, 1,              fd);
	fwrite(&(bmp.compression),         4, 1,              fd);
	fwrite(&(bmp.image_size),          4, 1,              fd);
	fwrite(&(bmp.x_pixels_per_meter),  4, 1,              fd);
	fwrite(&(bmp.y_pixels_per_meter),  4, 1,              fd);
	fwrite(&(bmp.colors_used),         4, 1,              fd);
	fwrite(&(bmp.important_colors),    4, 1,              fd);
	fwrite(bmp.pixel_data,             1, bmp.image_size, fd);
	fclose(fd);
	free(bmp.pixel_data);
	return 0;
}
