#include "DIPs.h"
#include "Image.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
/* Example of DIP */
/* make the picture color black & white */
IMAGE  *BlackNWhite(IMAGE *image)
{
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);
    int x, y, tmp;
   
    for (y = 0; y < ImageHeight(image); y++)
    {
        for (x = 0; x < ImageWidth(image); x++)
        {
            tmp = (image->R[x+y*ImageWidth(image)] + image->G[x+y*ImageWidth(image)] + image->B[x+y*ImageWidth(image)]) / 3;
            image->R[x+y*ImageWidth(image)] = image->G[x+y*ImageWidth(image)] = image->B[x+y*ImageWidth(image)]  = tmp;
        }
    }
	return image;
}

/* Add edge to the image*/
IMAGE *Edge(IMAGE *image) {
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);
    int             x, y, m, n, a, b;
    IMAGE *tempImage = NULL;
    tempImage = CreateImage(ImageWidth(image),ImageHeight(image));
    for (y = 0; y < ImageHeight(image); y++){
        for (x = 0; x < ImageWidth(image); x++) {
            tempImage->R[x+y*ImageWidth(image)] = image->R[x+y*ImageWidth(image)];
            tempImage->G[x+y*ImageWidth(image)] = image->G[x+y*ImageWidth(image)];
            tempImage->B[x+y*ImageWidth(image)] = image->B[x+y*ImageWidth(image)];
        }
    }
    int sumR = 0;   /* sum of the intensity differences with neighbors */
    int sumG = 0;
    int sumB = 0;
    for (y = 1; y < ImageHeight(image) - 1; y++){
        for (x = 1; x < ImageWidth(image) - 1; x++){
            for (n = -1; n <= 1; n++){
                for (m = -1; m <= 1; m++) {
                    a = (x + m >= ImageWidth(image)) ? ImageWidth(image) - 1 : (x + m < 0) ? 0 : x + m;
                    b = (y + n >= ImageHeight(image)) ? ImageHeight(image) - 1 : (y + n < 0) ? 0 : y + n;
                    sumR += (tempImage->R[x+y*ImageWidth(image)] - tempImage->R[a+b*ImageWidth(image)]);
                    sumG += (tempImage->G[x+y*ImageWidth(image)] - tempImage->G[a+b*ImageWidth(image)]);
                    sumB += (tempImage->B[x+y*ImageWidth(image)] - tempImage->B[a+b*ImageWidth(image)]);
                }
            }
            image->R[x+y*ImageWidth(image)] = (sumR > MAX_PIXEL) ? MAX_PIXEL: (sumR < 0) ? 0: sumR;
            image->G[x+y*ImageWidth(image)] = (sumG > MAX_PIXEL) ? MAX_PIXEL: (sumG < 0) ? 0: sumG;
            image->B[x+y*ImageWidth(image)] = (sumB > MAX_PIXEL) ? MAX_PIXEL: (sumB < 0) ? 0: sumB;
            sumR = sumG = sumB = 0;
        }
    }
    /* set all four borders to 0 */
    for (y = 0; y < ImageHeight(image); y++) {
        image->R[0+y*ImageWidth(image)] = 0;
        image->G[0+y*ImageWidth(image)] = 0;
        image->B[0+y*ImageWidth(image)] = 0;
        image->R[(ImageWidth(image) - 1)+y*ImageWidth(image)] = 0;
        image->G[(ImageWidth(image) - 1)+y*ImageWidth(image)] = 0;
        image->B[(ImageWidth(image) - 1)+y*ImageWidth(image)] = 0;
    }
    for (x = 0; x < ImageWidth(image); x++) {
        image->R[x] = 0;
        image->G[x] = 0;
        image->B[x] = 0;
        image->R[x+(ImageHeight(image) - 1)*ImageWidth(image)] = 0;
        image->G[x+(ImageHeight(image) - 1)*ImageWidth(image)] = 0;
        image->B[x+(ImageHeight(image) - 1)*ImageWidth(image)] = 0;
    }
	DeleteImage(tempImage);
	return image;
}

/* Shuffle the image */
IMAGE *Shuffle(IMAGE *image)
{
    assert(image);
    assert(image->R);
    assert(image->G);
    assert(image->B);
    int block_cnt = SHUFF_HEIGHT_DIV * SHUFF_WIDTH_DIV;
    int block_width = ImageWidth(image)/SHUFF_WIDTH_DIV;
    int block_height = ImageHeight(image)/SHUFF_HEIGHT_DIV;
    int block[SHUFF_WIDTH_DIV+SHUFF_HEIGHT_DIV*ImageWidth(image)];
    int i, j;


    srand(time(NULL));
    /* Initialize block markers to not done (-1) */
    for (i = 0; i < SHUFF_WIDTH_DIV; i++) {
        for (j = 0; j< SHUFF_HEIGHT_DIV; j++) {
            block[i+j*ImageWidth(image)] = -1;
        }
    }

    while (block_cnt > 0) {
        /* Generate a random pair of blocks */
        int dest_height = rand() % SHUFF_HEIGHT_DIV;
        int dest_width = rand() % SHUFF_WIDTH_DIV;
        int src_height = rand() % SHUFF_HEIGHT_DIV;
        int src_width = rand() % SHUFF_WIDTH_DIV;

        /* Check if these blocks are already swaped, if not swap blocks */
        if ((block[dest_width+(dest_height)*ImageWidth(image)] == -1) && (block[src_width+(src_height)*ImageWidth(image)] == -1)) {
            /* Swap blocks */
            for (i = 0; i < block_height; i++) {
                /* Init src and dest height offset */
                int h_dest = ((dest_height * block_height) + i) % ImageWidth(image);
                int h_src  = ((src_height * block_height) + i) % ImageHeight(image);
                for (j = 0; j < block_width; j++) {
                    int temp;
                    /* Init src and dest width offset */
                    int w_src  = ((src_width * block_width) + j) % ImageWidth(image);
                    int w_dest = ((dest_width * block_width) + j) % ImageWidth(image);

                    temp = image->R[w_dest+(h_dest)*ImageWidth(image)];
                    image->R[w_dest+h_dest*ImageWidth(image)] = image->R[w_src+h_src*ImageWidth(image)];
                    image->R[w_src+h_src*ImageWidth(image)] = temp;

                    temp = image->G[w_dest+(h_dest)*ImageWidth(image)];
                    image->G[w_dest+h_dest*ImageWidth(image)] = image->G[w_src+h_src*ImageWidth(image)];
                    image->G[w_src+h_src*ImageWidth(image)] = temp;

                    temp = image->B[w_dest+(h_dest)*ImageWidth(image)];
                    image->B[w_dest+h_dest*ImageWidth(image)] = image->B[w_src+h_src*ImageWidth(image)];
                    image->B[w_src+h_src*ImageWidth(image)] = temp;
                }
            }
            /* Set marker as done */
            block[dest_width+dest_height*ImageWidth(image)] = 1;
            block[src_width+src_height*ImageWidth(image)] = 1;
            /* Decrease block count */
            block_cnt -= 2; /* Two blocks are swapped */
        }

    }
	return image;
}

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
