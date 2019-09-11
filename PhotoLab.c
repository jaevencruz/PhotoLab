/*********************************************************************/
/* PhotoLab.c: homework assignment #3, for EECS 22,  Fall 2017       */
/* Modified: Mihnea Chirila for F18, 10/30/18                                                          */
/* Author: Mina Moghadam                                                */
/* Date:   11/8/2017                                                */
/*                                                                   */
/* Comments: */
/*                                                                   */
/* Please use this template to submit your hw3.                      */
/*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "DIPs.h"
#include "Advanced.h"
#include "FileIO.h"
#include "Constants.h"
#include "Image.h"
#include "Test.h"

/*** function declarations ***/

/* print a menu */
void PrintMenu();

/* Test all functions */

int main()
{
	int rc ;


#ifdef DEBUG
	AutoTest();
	rc = 0 ;

#else
	IMAGE *image = NULL;
	int option=0,percentage;			/* user input option */
	char fname[SLEN];		/* input file name */
	int brightness = -256;
	float angle,percent;
	rc = 1;
	PrintMenu();
    printf("Please make your choice: ");
    scanf("%d", &option);

while (option != EXIT) {
	if (option == 1) {
            printf("Please input the file name to load: ");
            scanf("%s", fname);
	    image = LoadImage(fname);
            rc  =  0;
		}
		/* menu item 2 - 11 requires image is loaded first */
        else if (option >= 2 && option <= 11) {
            if (rc != SUCCESS)	 {
                printf("No image is read.\n");
            }
            /* now image is loaded */
            else {
		switch (option) {
			case 2:
				printf("Please input the file name to save: ");
				scanf("%s", fname);
				SaveImage(fname, image);
				break;
			case 3:
				image = BlackNWhite(image);
				printf("\"Black & White\" operation is done!\n");
				break;
			case 4:
				image =  Edge(image);
				printf("\"Edge\" operation is done!\n");
				break;
			case 5:
				image = Shuffle(image);
				printf("\"Shuffle\" operation is done!\n");
				break;
			case 6:
				while(brightness > 255 || brightness < -255){
					printf("Enter brightness value (between -255 and 255):");
					scanf("%d", &brightness);
				}
				image =  Brightness(image, brightness);
				printf("\"brightness\" operation is done!\n");
				break;
			case 7:
				image = HMirror(image);
				printf("\"Horizontally Mirror\" operation is done!\n");
				break;
			case 8:
				printf("Enter hue rotation angle:");
				scanf("%f", &angle);
				image = HueRotate(image, angle);
				printf("\"HueRotate\" operation is done!\n");
				break;
			case 9:
				while(option != 1 && option != 2 && option != 3 && option != 4){
					printf("Pick your rotation option (1 for horizontal flip, 2 for vertical flip, 3 for right rotation, 4 for left rotation):");
					scanf("%d",&option);
					image = Rotate(image,option);
				}
				printf("\"Rotation\" operation is done!\n");
				break;
			case 10:
                                printf("Please input the resizing percentage (integer between 1-500):");
                                scanf("%d",&percentage);
                                image = Resize(image,percentage);
                                printf("\"Resize\" operation is done!\n");
                                break;
			case 11:
                                printf("Please input saturation value (integer between 1-500):");
                                scanf("%f",&percent);
                                image = Saturate(image,percent);
                                printf("\"Saturation\" operation is done!\n");
                                break;
			case 12:
				AutoTest();
				rc = SUCCESS;	/* set returned code SUCCESS, since image is loaded */
				break;

			default:
				break;
				}
			}
		}

	else if (option == 12) {
            AutoTest();
            rc = SUCCESS;	/* set returned code SUCCESS, since image is loaded */
        }
        else {
            printf("Invalid selection!\n");
        }

	/* Process finished, waiting for another input */
        PrintMenu();
        printf("Please make your choice: ");
        scanf("%d", &option);
	}
	if(image){
		assert(image);
		DeleteImage(image);
	}
    printf("You exit the program.\n");
#endif
	
	return 0;
}


/*******************************************/
/* Function implementations should go here */
/*******************************************/

/* Menu */
void PrintMenu() {
    printf("\n----------------------------\n");
    printf(" 1: Load a PPM image\n");
    printf(" 2: Save an image in PPM and JPEG format\n");
    printf(" 3: Change a color image to Black & White\n");
    printf(" 4: Sketch the edge of an image\n");
    printf(" 5: Shuffle an image\n");
    printf(" 6: Adjust the brightness of an image\n");
    printf(" 7: Mirror an image horizontally\n");
    printf(" 8: Adjust the hue of an image\n");
    printf(" 9: Rotate or flip the image\n");
    printf(" 10: Resize the image\n");
    printf(" 11: Saturate the image\n");
    printf(" 12: Test all functions\n");
    printf(" 13: Exit\n");
}


/* auto test*/

/*int AutoTest(void) {
    char fname[SLEN] = "HSSOE";
		int brightness = 200;
		float angle = -1.5;
	IMAGE *image = LoadImage(fname);
    LoadImage(fname);
    BlackNWhite(image);
    SaveImage("bw", image);
    printf("Black & White tested!\n\n");

    LoadImage(fname);
    Edge(image);
    SaveImage("edge",image);
    printf("Edge Detection tested!\n\n");

		LoadImage(fname);
		Shuffle(image);
		SaveImage("shuffle", image);
		printf("Shuffle tested!\n\n");

		LoadImage(fname);
    Brightness(image, brightness);
    SaveImage("brightness", image);
    printf("Brightness tested!\n\n");


    LoadImage(fname);
    HMirror(image);
    SaveImage("hmirror",image);
    printf("HMirror tested!\n\n");

    LoadImage(fname);
    HueRotate (image, angle);
    SaveImage("hue", image);
    printf("HueRotate tested!\n\n");
    DeleteImage(image);
	return 0;
}*/

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
