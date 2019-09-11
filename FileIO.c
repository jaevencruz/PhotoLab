#include "FileIO.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

IMAGE *LoadImage(const char *fname) {
    FILE           *File;
    char            Type[SLEN];
    int             Width, Height,MaxValue;
    int             x, y;
    char            ftype[] = ".ppm";
    char            fname_tmp[SLEN];      
    strcpy(fname_tmp, fname);
    strcat(fname_tmp, ftype);
    File = fopen(fname_tmp, "r");
    if (!File) {
        printf("Cannot open file \"%s\" for reading!\n", fname);
        return NULL;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
        printf("\nUnsupported file format!\n");
        return NULL;
    }
 
    fscanf(File, "%d", &Width);
    /*if (Width != ImageWidth(image)) {
        printf("\nUnsupported image width %d!\n", Width);
        return NULL;
    }*/
    fscanf(File, "%d", &Height);
    /*if (Height != ImageHeight(image)) {
        printf("\nUnsupported image height %d!\n", ImageHeight(image));
        return NULL;
    }*/
    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255) {
        printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return NULL;
    }
    if ('\n' != fgetc(File)) {
        printf("\nCarriage return expected!\n");
        return NULL;
    }
    IMAGE *image = NULL;
    image = CreateImage(Width,Height);

    for (y = 0; y < ImageHeight(image); y++){
        for (x = 0; x < ImageWidth(image); x++) {
            SetPixelR(image,x,y,fgetc(File));
	    SetPixelG(image,x,y,fgetc(File));
            SetPixelB(image,x,y,fgetc(File));
             //Make to set pixel
        }
    }
    if (ferror(File)) {
        printf("\nFile error while reading from file!\n");
        return NULL;
    }
    printf("%s was read.\n", fname_tmp);
    fclose(File); 
    return image;
}

int SaveImage(const char *fname, const IMAGE *image) {
    FILE	*File;
    int	x, y;
    char	SysCmd[SLEN * 5];

    char	ftype[] = ".ppm";
    char	fname_tmp[SLEN];  /*avoid to modify on the original pointer, 11/10/10, X.Han*/
    char	fname_tmp2[SLEN];

    strcpy(fname_tmp, fname);
    strcpy(fname_tmp2, fname);
    strcat(fname_tmp2, ftype);

    File = fopen(fname_tmp2, "w");
    if (!File) {
        printf("Cannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", ImageWidth(image), ImageHeight(image));
    fprintf(File, "255\n");

    for (y = 0; y < ImageHeight(image); y++){
        for (x = 0; x < ImageWidth(image); x++) {
            fputc(GetPixelR(image,x,y),File);
            fputc(GetPixelG(image,x,y),File);
            fputc(GetPixelB(image,x,y),File);
        }
    }

    if (ferror(File)) {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved. \n", fname_tmp2);

    /*
     * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
     * and make it world readable
     */
    sprintf(SysCmd, "/users/grad2/doemer/eecs22/bin/pnmtojpeg_hw3.tcsh %s", fname_tmp2);
    if (system(SysCmd) != 0) {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored.\n", fname_tmp);
    return 0;
}
/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
