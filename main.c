#include "tools.h"

int main(int argc, char ** argv)
{
    structHeader headerImage = {0};
    structPixel **matrixImage;

    structHeader headerImage2 = {0};
    structPixel **matrixImage2;

    char repertory[MAX_PATH] = {0};
    char repertory1[MAX_PATH] = {0};
    char repertory2[MAX_PATH] = {0};
    char repertory_result[MAX_PATH] = {0};
    int size_slash_check = 0;

    char file_name[MAX_PATH] = {0};
    char file_name2[MAX_PATH] = {0};

    char processing_string[MAX_PATH] = {0};
    int processing = 0;

    bool processing_error = false;
    bool open_error = false;
    bool write_error = false;

    /* Checks if the program has been opened with arguments. */

    /* Displays the command prompt interface to get the necessary informations. */
    if(argc == 1){

        printf("-------------------------------------------\n");
        printf("|             IMAGE CONVERTOR             |\n");
        printf("-------------------------------------------\n");
        printf("\n      Saisir le r%cpertoire de travail :\n",130);
        printf("\n> ");

        gets(repertory);

        printf("\n           Saisir un traitement :\n\n");
        printf("1- Niveaux de gris\n");
        printf("2- Inverse\n");
        printf("3- Monochrome\n");
        printf("4- Contours \n");
        printf("5- Superposition\n");
        printf("\n> ");

        scanf("%d", &processing);
        fflush(stdin);

        printf("\n             Saisir une image :\n");
        printf("\n> ");

        gets(file_name);

        if(processing == 5){
            printf("\n           Saisir une 2%cme image :\n",138);
            printf("\n> ");

            gets(file_name2);
        }

    }

    /* Obtains the necessary information from the arguments. */
    if(argc >= 4){
        strcpy(repertory, argv[1]);
        strcpy(processing_string, argv[2]);
        processing = strtol(processing_string, NULL, 10);
        strcpy(file_name, argv[3]);
        if(processing == 5 && argc >= 5){
            strcpy(file_name2, argv[4]);
        }
    }

    /* Check if the directory name has a '/' or '\' at the end, otherwise we add one */

    size_slash_check = strlen(repertory);
    if(size_slash_check != 0){
        if(repertory[size_slash_check-1] != '\\' || repertory1[size_slash_check-1] != '/'){
            repertory[size_slash_check] = '\\';
        }
    }

    /* Copies and Assembles all the paths to the files to be processed. */

    strcpy(repertory1, repertory);
    strcpy(repertory2, repertory);
    strcpy(repertory_result, repertory);

    strcat(repertory1, file_name);
    strcat(repertory2, file_name2);
    strcat(repertory_result, "result.bmp");

    /* Copies the file into the matrix. */

    open_error += bmpToMatrix(&headerImage, &matrixImage, repertory1);

    /* Applies the processing chosen by the user. */

    switch (processing){
        case 1:
            processing_error += greylevel(&headerImage, &matrixImage);
            break;
        case 2:
            processing_error += reverse(&headerImage, &matrixImage);
            break;
        case 3:
            processing_error += monochrome(&headerImage, &matrixImage);
            break;
        case 4:
            processing_error += outline(&headerImage, &matrixImage);
            break;
        case 5:
        /* In this case, it creates a new matrix for the 2nd image and free it right after. */
            open_error += bmpToMatrix(&headerImage2, &matrixImage2, repertory2);
            processing_error += fusion(&headerImage, &matrixImage, &headerImage2, &matrixImage2);
            freeMatrix(&headerImage2,&matrixImage2);
            break;
        default:
            processing_error += true;
    }

    /* Writes the result only if a treatment has been applied. */

    if (processing_error == false){
        write_error += maxtrixToBmp(&headerImage, &matrixImage, repertory_result);
    }

    /* Dynamically frees space for the Matrix */

    freeMatrix(&headerImage, &matrixImage);


    /* Checks all the errors and applies the returns according to them. */

    if (open_error == true){
        return -2;
    }

    if (processing_error == true){
        return -1;
    }

    if (write_error == true){
        return -3;
    }

    return 0;
}
