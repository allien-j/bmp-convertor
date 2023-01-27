#include "tools.h"

int bmpToMatrix(structHeader *matrix_header, structPixel ***matrix, char *path){

    /** \brief  Open BMP File and transfer the data to a Matrix and its Header. (Automatically allocates memory).
     *
     * \param 1.   Address of the Header of your Matrix.
     * \Warning    (if it's already defined, it will overwrite the previous data !).
     *
     * \param 2.   Address of your Matrix.
     * \param 3.   BMP File path.
     *
     * \return  "3" if the allocation failed.
     * \return  "2" if the file cannot be opened.
     * \return  "1" if the file path is not valid or if the extension is not ".bmp".
     * \return  "0" when everything's OK.
     *
     * \ATTENTION, It's necessary to free the memory with the "freeMatrix()" function to avoid problems !
     *
     */

    unsigned int overflow_number;
    int bmp_check = 0;
    bool alloc_check = false;

    /* Checks if the file extension is ".bmp" or if the path is not incorrect. */

    bmp_check = strlen(path);
    if (bmp_check<4){
        return 1;
    } else {
        if( (path[bmp_check-4] != '.') || (path[bmp_check-3] != 'b') || (path[bmp_check-2] != 'm') || (path[bmp_check-1] != 'p') ){
            return 1;
        }
    }

    /* Opens the file and checks if it exists */

    FILE* image = NULL;
    image = fopen(path,"rb");

    if (image == NULL){
        return 2;
    }

    /* Copies the file information to the matrix header. */

    fseek(image,0,SEEK_SET);
    fread(&(matrix_header->type), 2, 1, image);
    fread(&(matrix_header->size), 4, 1, image);
    fread(&(matrix_header->reserved), 4, 1, image);
    fread(&(matrix_header->offset), 4, 1, image);
    fread(&(matrix_header->header_format_size), 4, 1, image);
    fread(&(matrix_header->width), 4, 1, image);
    fread(&(matrix_header->height), 4, 1, image);
    fread(&(matrix_header->plans), 2, 1, image);
    fread(&(matrix_header->bpp), 2, 1, image);
    fread(&(matrix_header->compression), 4, 1, image);
    fread(&(matrix_header->image_size), 4, 1, image);
    fread(&(matrix_header->x_ppm), 4, 1, image);
    fread(&(matrix_header->y_ppm), 4, 1, image);
    fread(&(matrix_header->number_colors), 4, 1, image);
    fread(&(matrix_header->important_colors), 4, 1, image);

    /* Checks if the number of bytes on a line is a multiple of 4. */

    overflow_number = ((matrix_header->width)*3)%4;

    /* Dynamically allocates space for the Matrix. */
    /* Checks if the dynamic allocation was successful */

    alloc_check += allocMatrix(matrix_header, matrix);

    /* Copies the information from the file to the matrix */
    /* It also defines the Brightness as the average color of the pixel */

    if (alloc_check == false){
        fseek(image,(matrix_header->offset),SEEK_SET);

        for(int y=0; y<(matrix_header->height); ++y){
            for(int x=0; x<(matrix_header->width); ++x){
                fread(&(*matrix)[y][x].Blue, 1, 1, image);
                fread(&(*matrix)[y][x].Green, 1, 1, image);
                fread(&(*matrix)[y][x].Red, 1, 1, image);
                (*matrix)[y][x].Brightness = ((*matrix)[y][x].Blue + (*matrix)[y][x].Green + (*matrix)[y][x].Red) /PIXEL_SIZE;
            }
        /* If the number of bytes on a line is a multiple of 4, it does not copy the extra 0s*/
            if (overflow_number != 0){
                fseek(image, overflow_number, SEEK_CUR);
            }
        }
    }
    fclose(image);

    if (alloc_check == true){
        return 3;
    }

    return 0;
}

int maxtrixToBmp(structHeader *matrix_header, structPixel ***matrix, char *path){

    /** \brief  Transfer the data of your Matrix and its Header to a BMP File. (Does not free the memory).
     *
     * \param 1.   Address of the Header of your Matrix.
     * \param 2.   Address of your Matrix.
     * \param 3.   BMP File save path.
     *
     * \return  "3" if the pointer is NULL.
     * \return  "2" if the file cannot be written.
     * \return  "1" if the file path is not valid or if the extension is not ".bmp".
     * \return  "0" when everything's OK.
     *
     * \ATTENTION, It's necessary to free the memory with the "freeMatrix()" function to avoid problems !
     *             This function does not do it for you !
     *
     * \Warning    The Matrix and its Header must be created with the "bmpToMatrix()" function
     *             otherwise it can cause problems if they are not well defined.
     *
     */

    unsigned int overflow_number;
    int repertory_check = 0;

    /* Checks if the pointer isn't NULL */

    if( (*matrix) == NULL){
        return 3;
    }

    /* Checks if the file extension is ".bmp" or if the path is not incorrect. */

    repertory_check = strlen(path);
    if (repertory_check < 4){
        return 1;
    } else {
        if( (path[repertory_check-4] != '.') || (path[repertory_check-3] != 'b') || (path[repertory_check-2] != 'm') || (path[repertory_check-1] != 'p') ){
            return 1;
        }
    }

    /* Opens the file and check if it can be written. */

    FILE* image = NULL;
    image = fopen(path,"wb");

    if (image == NULL){
        return 2;
    }

    /* Writes the matrix header information to the file. */

    fseek(image,0,SEEK_SET);
    fwrite(&(matrix_header->type), 2, 1, image);
    fwrite(&(matrix_header->size), 4, 1, image);
    fwrite(&(matrix_header->reserved), 4, 1, image);
    fwrite(&(matrix_header->offset), 4, 1, image);
    fwrite(&(matrix_header->header_format_size), 4, 1, image);
    fwrite(&(matrix_header->width), 4, 1, image);
    fwrite(&(matrix_header->height), 4, 1, image);
    fwrite(&(matrix_header->plans), 2, 1, image);
    fwrite(&(matrix_header->bpp), 2, 1, image);
    fwrite(&(matrix_header->compression), 4, 1, image);
    fwrite(&(matrix_header->image_size), 4, 1, image);
    fwrite(&(matrix_header->x_ppm), 4, 1, image);
    fwrite(&(matrix_header->y_ppm), 4, 1, image);
    fwrite(&(matrix_header->number_colors), 4, 1, image);
    fwrite(&(matrix_header->important_colors), 4, 1, image);

    /* Checks if the number of bytes on a line is a multiple of 4. */

    overflow_number = ((matrix_header->width)*PIXEL_SIZE)%4;

    /* Copies the information from the matrix to the file */

    fseek(image,(matrix_header->offset),SEEK_SET);

    for(int y=0; y<(matrix_header->height); ++y){
        for(int x=0; x<(matrix_header->width); ++x){
            fwrite(&(*matrix)[y][x].Blue, 1, 1, image);
            fwrite(&(*matrix)[y][x].Green, 1, 1, image);
            fwrite(&(*matrix)[y][x].Red, 1, 1, image);
        }
    /* If the number of bytes on a line is a multiple of 4, it will add extra 0s to make it multiple of 4. */
        if (overflow_number != 0){
            for(int o=1; o <= overflow_number; o++){
                fputc(0,image);
            }
        }
    }

    fclose(image);

    return 0;
}

int allocMatrix(structHeader *matrix_header, structPixel ***matrix){

    /** \brief  Dynamically allocates space for the Matrix.
     *
     * \param 1.   Address of the Header of your Matrix.
     * \param 2.   Address of your Matrix.
     *
     * \return  "2" if there is an error with the allocation.
     * \return  "1" if the height and width of the Matrix Header are empty.
     * \return  "0" when everything's OK.
     *
     * \ATTENTION, It's necessary to free the memory with the "freeMatrix()" function to avoid problems !
     *
     * \Warning    You must first define your Matrix Header.
     *
     */

    /* Checks if the matrix height and width are not empty */

    if((matrix_header->height) == 0 || matrix_header->width == 0){
        return 1;
    }

    /* Dynamically allocates space for the Matrix according to the length and width of the image. */

    (*matrix) = calloc(matrix_header->height, sizeof(unsigned char*)); // y (height)

    /* Checks if the dynamic allocation was successful */

    if( (*matrix) == NULL){
        return 2;
    }

    for(int y=0; y<matrix_header->height; ++y){
        (*matrix)[y] = calloc(matrix_header->width, sizeof(struct Pixel)); // x (width)

        /* Checks if the dynamic allocation was successful */
        /* else it frees the memory that has been allocated */

        if( (*matrix)[y] == NULL){
            freeMatrix(matrix_header, matrix);
            return 2;
        }
    }

    return 0;
}

int freeMatrix(structHeader *matrix_header, structPixel ***matrix){

    /** \brief  Dynamically frees space of Matrix
     *
     * \param 1.   Address of the Header of your Matrix.
     * \param 2.   Address of your Matrix.
     *
     * \return  "2" if the pointer is NULL.
     * \return  "1" if the height and width of the Matrix Header are empty.
     * \return  "0" when everything's OK.
     *
     * \ATTENTION, It's necessary to free only the memory that has been allocated with the "allocMatrix()" function to avoid problems!
     *
     * \Warning    You must use the Matrix Header with which you made your dynamic allocation.
     *
     */

    /* Checks if the matrix height and width are not empty */

    if((matrix_header->height) == 0 || matrix_header->width == 0){
        return 1;
    }

    /* Checks if the pointer isn't NULL */

    if( (*matrix) == NULL){
        return 2;
    }

    /* Dynamically frees space for the Matrix according to the length and width of the image. */
    /* Checks if the pointer isn't NULL (used if the allocation failed) */

    for(int y=0; y<matrix_header->height; ++y){
        if ((*matrix)[y] == NULL){
            break;
        }
        free((*matrix)[y]); // x (width)
    }
    free(*matrix); // y (height)

    return 0;
}

int greylevel(structHeader *matrix_header, structPixel ***matrix){

    /** \brief   Applies a greylevel filter to your Matrix.
     *
     * \param 1.   Address of the Header of your Matrix.
     * \param 2.   Address of your Matrix.
     *
     * \return  "2" if the pointer is NULL.
     * \return  "1" if the matrix size is empty (=0).
     * \return  "0" when everything's OK.
     *
     */

    /* Checks if the pointer isn't NULL */

    if( (*matrix) == NULL){
        return 2;
    }

    /* Checks if the matrix size is not 0. */

    if((matrix_header->size) == 0){
        return 1;
    }

    /* Replaces the colors of each pixel by the brightness (average of the colors). */

    for(int y=0; y<matrix_header->height; ++y){
        for(int x=0; x<matrix_header->width; ++x){
            (*matrix)[y][x].Blue  = (*matrix)[y][x].Brightness;
            (*matrix)[y][x].Green  = (*matrix)[y][x].Brightness;
            (*matrix)[y][x].Red = (*matrix)[y][x].Brightness;
        }
    }

    return 0;
}

int reverse(structHeader *matrix_header, structPixel ***matrix){

    /** \brief   Applies a reverse filter to your Matrix.
     *
     * \param 1.   Address of the Header of your Matrix.
     * \param 2.   Address of your Matrix.
     *
     * \return  "2" if the pointer is NULL.
     * \return  "1" if the matrix size is empty (=0).
     * \return  "0" when everything's OK.
     *
     */

    /* Checks if the pointer isn't NULL */

    if( (*matrix) == NULL){
        return 2;
    }

    /* Checks if the matrix size is not 0. */

    if((matrix_header->size) == 0){
        return 1;
    }

    /* Replaces the colors of each pixel by their reverse over a range of 255 (1 byte). */

    for(int y=0; y<matrix_header->height; ++y){
        for(int x=0; x<matrix_header->width; ++x){
            (*matrix)[y][x].Blue  = 255 - (*matrix)[y][x].Blue;
            (*matrix)[y][x].Green  = 255 - (*matrix)[y][x].Green;
            (*matrix)[y][x].Red = 255 - (*matrix)[y][x].Red;
        }
    }

    return 0;
}

int monochrome(structHeader *matrix_header, structPixel ***matrix){

    /** \brief   Applies a monochrome filter to your Matrix.
     *
     * \param 1.   Address of the Header of your Matrix.
     * \param 2.   Address of your Matrix.
     *
     * \return  "2" if the pointer is NULL
     * \return  "1" if the matrix size is empty (=0).
     * \return  "0" when everything's OK.
     *
     */

    /* Checks if the pointer isn't NULL */

    if( (*matrix) == NULL){
        return 2;
    }

    /* Checks if the matrix size is not 0. */

    if((matrix_header->size) == 0){
        return 1;
    }

    /* Replaces the colors of each pixel by 0 (black) if the brightness is close the 0 (black) otherwise it's by white. */

    for(int y=0; y<matrix_header->height; ++y){
        for(int x=0; x<matrix_header->width; ++x){
            if((*matrix)[y][x].Brightness <= 128){
                (*matrix)[y][x].Blue  = 0;
                (*matrix)[y][x].Green  = 0;
                (*matrix)[y][x].Red = 0;
            } else {
                (*matrix)[y][x].Blue  = 255;
                (*matrix)[y][x].Green  = 255;
                (*matrix)[y][x].Red = 255;
            }
        }
    }

    return 0;
}

int outline(structHeader *matrix_header, structPixel ***matrix){

    /** \brief   Applies a Sobel filter to your Matrix. (outlines)
     *
     * \param 1.   Address of the Header of your Matrix.
     * \param 2.   Address of your Matrix.
     *
     * \return  "2" if the pointer is NULL.
     * \return  "1" if the matrix size is empty (=0).
     * \return  "0" when everything's OK.
     *
     */

    /* Checks if the matrix size is not 0. */

    if((matrix_header->size) == 0){
        return 1;
    }

    /* Creates and allocates a new Matrix. */

    struct Pixel **matrix_contour_result;
    int mask = 0, mask2 = 0;

    allocMatrix(matrix_header, &matrix_contour_result);

    /* Applies a Gaussian Blur to get better results and save it in the 2nd Matrix */

    for(int y=0; y<matrix_header->height; ++y){
        for(int x=0; x<matrix_header->width; ++x){
            if((y-1>=0 && x-1>=0) && (y+1<matrix_header->height && x+1<matrix_header->width)){

                mask = ((*matrix)[y-1][x-1].Brightness + (*matrix)[y-1][x].Brightness + (*matrix)[y-1][x+1].Brightness +
                       (*matrix) [y]  [x-1].Brightness + (*matrix)[y]  [x].Brightness + (*matrix)[y]  [x+1].Brightness +
                       (*matrix) [y+1][x-1].Brightness + (*matrix)[y+1][x].Brightness + (*matrix)[y+1][x+1].Brightness) / 9;

                matrix_contour_result[y][x].Brightness = mask;

            }
        }
    }

    /* Copies the 2nd Matrix to the 1st. (Brightness only) */

    for(int y=0; y<matrix_header->height; ++y){
        for(int x=0; x<matrix_header->width; ++x){
            if((y-1>=0 && x-1>=0) || (y+1<=matrix_header->height && x+1<=matrix_header->width)){
                (*matrix)[y][x].Brightness = matrix_contour_result[y][x].Brightness;
            }
        }
    }

    /* Applies the Sobel filter and save it in the 2nd Matrix. */

    for(int y=0; y<matrix_header->height; ++y){
        for(int x=0; x<matrix_header->width; ++x){

            if((y-1>=0 && x-1>=0) && (y+1<matrix_header->height && x+1<matrix_header->width)){

                mask = ((*matrix)[y-1][x+1].Brightness + (*matrix)[y+1][x+1].Brightness + ((*matrix)[y][x+1].Brightness)*2) -
                        ((*matrix)[y-1][x-1].Brightness + (*matrix)[y+1][x-1].Brightness + ((*matrix)[y][x-1].Brightness)*2);

                mask2 = ((*matrix)[y+1][x-1].Brightness + (*matrix)[y+1][x].Brightness + ((*matrix)[y+1][x+1].Brightness)*2) -
                        ((*matrix)[y-1][x-1].Brightness + (*matrix)[y-1][x].Brightness + ((*matrix)[y-1][x+1].Brightness)*2);

                matrix_contour_result[y][x].Brightness = sqrt(pow(mask, 2) + pow(mask2, 2));

                if(matrix_contour_result[y][x].Brightness <= 128){
                    matrix_contour_result[y][x].Brightness = 255;
                } else {
                    matrix_contour_result[y][x].Brightness = 0;
                }
            }
        }
    }

    /* Copies the 2nd Matrix (Brightness) to the 1st (every colors). */

    for(int y=0; y<matrix_header->height; ++y){
        for(int x=0; x<matrix_header->width; ++x){

            if((y-1>=0 && x-1>=0) || (y+1<=matrix_header->height && x+1<=matrix_header->width)){
                (*matrix)[y][x].Blue  = matrix_contour_result[y][x].Brightness;
                (*matrix)[y][x].Green  = matrix_contour_result[y][x].Brightness;
                (*matrix)[y][x].Red = matrix_contour_result[y][x].Brightness;
            }
        }
    }

    /* Dynamically frees space for the Matrix */

    freeMatrix(matrix_header, &matrix_contour_result);

    return 0;
}

int fusion(structHeader *matrix_header, structPixel ***matrix, structHeader *matrix_header2, structPixel ***matrix2){

    /** \brief  Made the fusion of 2 images.
     *
     * \param 1.   Address of the Header of your First Matrix.
     * \param 2.   Address of your First Matrix.
     * \param 3.   Address of the Header of your Second Matrix.
     * \param 4.   Address of your Second Matrix.
     *
     * \return  "2" if the pointer is NULL.
     * \return  "1" if the matrices does not have the same height, width or if size is empty (=0).
     * \return  "0" when everything's OK.
     *
     * \Warning  The result will be stored in the 1st matrix.
     * \Warning  The headers and the 2nd matrix will remain unchanged.
     *
     */

    /* Checks if the pointer isn't NULL */

    if( (*matrix) == NULL){
        return 2;
    }

    /* Checks if the matrices does not have the same height, width or if size is not 0. */

    if((matrix_header->size) == 0 || (matrix_header2->size) == 0 || (matrix_header->width != matrix_header2->width) || (matrix_header->height != matrix_header2->height)){
        return 1;
    }

    /* Replaces the colors of each pixel by the average of each color of each pixel of both matrices */

    for(int y=0; y<matrix_header->height; ++y){
        for(int x=0; x<matrix_header->width; ++x){
            (*matrix)[y][x].Blue  = ((*matrix)[y][x].Blue + (*matrix2)[y][x].Blue)/2;
            (*matrix)[y][x].Green  = ((*matrix)[y][x].Green + (*matrix2)[y][x].Green)/2;
            (*matrix)[y][x].Red = ((*matrix)[y][x].Red + (*matrix2)[y][x].Red)/2;
        }
    }

    return 0;
}
