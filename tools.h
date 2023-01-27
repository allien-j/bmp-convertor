#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define PIXEL_SIZE 3
#define MAX_PATH 256

typedef struct Pixel{
    unsigned char Blue;
    unsigned char Green;
    unsigned char Red;
    unsigned char Brightness;
} structPixel;

typedef struct Header {
    uint16_t type;
    uint32_t size;
    uint32_t reserved;
    uint32_t offset;
    uint32_t header_format_size;
    uint32_t width;
    uint32_t height;
    uint16_t plans;
    uint16_t bpp;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_ppm;
    uint32_t y_ppm;
    uint32_t number_colors;
    uint32_t important_colors;
} structHeader;

int bmpToMatrix(structHeader *matrix_header, structPixel ***matrix, char *path);
int maxtrixToBmp(structHeader *matrix_header, structPixel ***matrix, char *path);

int allocMatrix(structHeader *matrix_header, structPixel ***matrix);
int freeMatrix(structHeader *matrix_header, structPixel ***matrix);

int outline(structHeader *matrix_header, structPixel ***matrix);
int reverse(structHeader *matrix_header, structPixel ***matrix);
int monochrome(structHeader *matrix_header, structPixel ***matrix);
int greylevel(structHeader *matrix_header, structPixel ***matrix);
int fusion(structHeader *matrix_header, structPixel ***matrix, structHeader *matrix_header2, structPixel ***matrix2);

#endif // TOOLS_H_INCLUDED
