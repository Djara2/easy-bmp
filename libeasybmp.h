// Source: https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
#include <stdint.h> 
#include <stdlib.h>
#include <stdio.h>
#ifndef LIBEASYBMP_H 
#define LIBEASYBMP_H
enum Direction {
	DIRECTION_HORIZONTAL,
	DIRECTION_VERTICAL	
};

enum EasyBMPStatus { 
	STATUS_OK,
	STATUS_FAILED
};

struct BMP {
	// FIELD                			COMMENTS			Bytes
	// Header
	uint8_t  signature[2];				// "BM"				2 	2
	uint16_t padding_per_scan_line_in_bytes;	// 2	4
	uint32_t file_size;				// Overall file size in bytes	4	8
	uint32_t reserved; 				// unused = 0			4 	12
	uint32_t data_offset;  				//				4  	16
				
	// InfoHeader	
	uint32_t size;			//				4	4
	uint32_t width;			//				4	8
	uint32_t height;		//				4 	12
	uint16_t planes;		//				2 	14
	uint16_t bits_per_pixel;	//				2	16
	uint32_t compression;		//				4	20
	uint32_t image_size;		// 				4	24
	uint32_t x_pixels_per_meter;	//				4 	28
	uint32_t y_pixels_per_meter;	//				4 	32
	uint32_t colors_used;       	//				4 	36
	uint32_t important_colors;  	//				4 	40
	
	// Pixel data
	uint8_t *pixel_data;		//				8	8
	
	// Stuff to help me
	size_t width_in_bytes_without_padding;	//			8 	8
	size_t width_in_bytes_with_padding;	//			8	16
};

enum EasyBMPStatus bmp_init(struct BMP *bmp, uint32_t width, uint32_t height);

uint8_t* bmp_get_scanline(struct BMP *bmp, uint32_t scanline);

size_t bmp_put_pixel(struct BMP *bmp, size_t offset, uint8_t red, uint8_t green, uint8_t blue);

uint8_t* bmp_put_pixel_at_coordinate(struct BMP *bmp, uint32_t scanline, uint32_t column, uint8_t red, uint8_t green, uint8_t blue);

size_t bmp_draw_line(struct BMP *bmp, enum Direction direction, uint32_t row, uint32_t column, uint8_t red, uint8_t green, uint8_t blue);

size_t bmp_write_to_file(struct BMP *bmp, FILE *fd);
#endif
