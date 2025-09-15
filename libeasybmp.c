#include "libeasybmp.h"
#include <stdio.h>
#include <stdlib.h>

enum EasyBMPStatus bmp_init(struct BMP *bmp, uint32_t width, uint32_t height) { 
	if (bmp == NULL) { 
		fprintf(stderr, "[Error][bmp_init] BMP struct pointer cannot be NULL.\n");
		return STATUS_FAILED;
	}

	if (width == 0) {
		fprintf(stderr, "[Error][bmp_init] Width of the picture cannot be 0.\n");
		return STATUS_FAILED;
	} 
	if (height == 0) {
		fprintf(stderr, "[Error][bmp_init] Height of the picture cannot be 0.\n");
		return STATUS_FAILED;
	}
	
	// Determine file size   
	// (1) Determine how much padding is required
	bmp->width_in_bytes_without_padding = 3 * width; 
	bmp->padding_per_scan_line_in_bytes = 0;
	uint8_t four_byte_alignment_overage = (bmp->width_in_bytes_without_padding & 0x3);
	if (four_byte_alignment_overage)
		bmp->padding_per_scan_line_in_bytes = 4 - four_byte_alignment_overage;
	bmp->width_in_bytes_with_padding    = bmp->width_in_bytes_without_padding + bmp->padding_per_scan_line_in_bytes;

	// (2) Add the 54 bytes for the header and the info header with the 
	//     product of the height and the width
	bmp->file_size = 54 + (bmp->width_in_bytes_with_padding * height);
		
	// Initialize the header
	bmp->signature[0] = 'B';
	bmp->signature[1] = 'M';
	bmp->reserved = 0;
	bmp->data_offset = 54;

	// Initialize the infoheader
	bmp->size = 40;
	bmp->width = width;
	bmp->height = height;
	bmp->planes = 1;
	bmp->bits_per_pixel = 24;
	bmp->compression = 0;
	bmp->image_size = bmp->file_size - 54;
	bmp->x_pixels_per_meter = 2835;
	bmp->y_pixels_per_meter = 2835;
	bmp->colors_used = 0; // previously 0x00ffffff;	// 16M for 24-bit RGB
	bmp->important_colors = 0;
	
	// Allocate space for pixels
	bmp->pixel_data = malloc(bmp->file_size - 54);
	if (bmp->pixel_data == NULL) { 
		fprintf(stderr, "[Error][bmp_init] Failed to allocate %u bytes for the BMP's pixel data.\n", bmp->file_size - 54);
		return STATUS_FAILED;
	} 
	// Make canvas black by default
	for (size_t i = 0; i < bmp->file_size - 54; i++)
		bmp->pixel_data[i] = 0;

	return STATUS_OK;
}

size_t bmp_put_pixel(struct BMP *bmp, size_t offset, uint8_t red, uint8_t green, uint8_t blue) {
	return 0;
}

// scanline 0 is the bottom of the image, not the top 
// From the UALBERTA source: "scan lines are stored bottom to top instead of top to bottom"
// Returns a pointer to where the scanline begins in the BMP's uint8_t *pixel_data buffer.
uint8_t* bmp_get_scanline(struct BMP *bmp, uint32_t scanline) { 
	if (bmp == NULL) {
		fprintf(stderr, "[Error][bmp_get_scanline] BMP pointer is NULL.\n");
		return NULL;
	}

	if (scanline >= bmp->height) {
		fprintf(stderr, "[Error][bmp_get_scanline] Scanline (%u) cannot be greater than or equal to the image's height (%u). Scanlines begin counting from 0, not 1.\n", scanline, bmp->height);
		return NULL;
	}

	uint8_t *scanline_ptr = bmp->pixel_data + (scanline * bmp->width_in_bytes_with_padding);
	return scanline_ptr;
}

// scanline 0 is the bottom of the image, not the top 
// From the UALBERTA source: "scan lines are stored bottom to top instead of top to bottom"
// Returns the pointer to the next pixel after where you wrote. Goes to the next scanline if the next byte is a padding byte
// (thus putting you at the next pixel)
uint8_t* bmp_put_pixel_at_coordinate(struct BMP *bmp, uint32_t scanline, uint32_t column, uint8_t red, uint8_t green, uint8_t blue) { 
	if (bmp == NULL) {
		fprintf(stderr, "[Error][bmp_put_pixel_at_coordinate] Cannot BMP struct pointer is a NULL pointer.\n");
		return NULL;
	}
	
	if (scanline >= bmp->height) { 
		fprintf(stderr, "[Error][bmp_put_pixel_at_coordinate] Scanline %u cannot be equal to or greater than the image's height (%u). Counting begins at 0.\n", scanline, bmp->height);
		return NULL;
	}

	if (column >= bmp->width) { 
		fprintf(stderr, "[Error][bmp_put_pixel_at_coordinate] Column (%u) within requested scanline (%u) cannot be greater than or equal to the scanline's width in pixels (%u). Tip: Scanline pixel counting starts at 0.\n", column, scanline, bmp->width);
		return NULL;
	}

	uint8_t *scanline_ptr = bmp->pixel_data + (scanline * bmp->width_in_bytes_with_padding);	
	size_t column_offset = column * 3;
	scanline_ptr[column_offset] = blue;
	scanline_ptr[column_offset + 1] = green;
	scanline_ptr[column_offset + 2] = red;
	
	uint8_t *next_pixel = scanline_ptr + column_offset + 3; 
	// Go to next pixel if subsequent byte is a padding byte.
	if (next_pixel >= scanline_ptr + bmp->width_in_bytes_without_padding) {
		next_pixel = bmp_get_scanline(bmp, scanline + 1);
		if (next_pixel == NULL)
			fprintf(stderr, "[Warning][bmp_get_scanline] Pointer to next pixel landing in the padding area (padding = %hu bytes). The pointer was set to return the start of the next scanline, but it seems there are no more scanlines and so the return pointer will be a NULL pointer.\n", bmp->padding_per_scan_line_in_bytes);
	}

	return next_pixel;
}

size_t bmp_draw_line(struct BMP *bmp, enum Direction direction, uint32_t row, uint32_t column, uint8_t red, uint8_t green, uint8_t blue) { 
	return 0;
}


size_t bmp_write_to_file(struct BMP *bmp, FILE *fd) { 
	return 0;
}
