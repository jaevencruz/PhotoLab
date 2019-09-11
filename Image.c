#include "Image.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/* Return the pointer to the image, or NULL in case of error */

unsigned char GetPixelR(const IMAGE *image, unsigned int x,  unsigned int y){
	assert(image);
	assert(image->R);
	return image->R[x+(y*ImageWidth(image))];
}

unsigned char GetPixelG(const IMAGE *image, unsigned int x,  unsigned int y){
        assert(image);
	assert(image->G);
        return image->G[x+(y*ImageWidth(image))];
}

unsigned char GetPixelB(const IMAGE *image, unsigned int x,  unsigned int y){
        assert(image);
	assert(image->B);
        return image->B[x+(y*ImageWidth(image))];
}


void SetPixelR(IMAGE *image, unsigned int x,  unsigned int y, unsigned char r){
	assert(image);
	assert(image->R);
	image->R[x+(y*ImageWidth(image))] = r;
		
}

void SetPixelG(IMAGE *image, unsigned int x,  unsigned int y, unsigned char g){
        assert(image);
	assert(image->G);
        image->G[x+(y*ImageWidth(image))] = g;

}

void SetPixelB(IMAGE *image, unsigned int x,  unsigned int y, unsigned char b){
        assert(image);
	assert(image->B);
        image->B[x+(y*ImageWidth(image))] = b;

}

IMAGE *CreateImage(unsigned int Width, unsigned int Height){
        IMAGE *image = NULL;
        image = malloc(sizeof(IMAGE));
        if (!image){
                exit(10);
        }
	image->W=Width;
        image->H=Height;
	
        image->R=malloc(Width*Height*sizeof(unsigned char));
        image->G=malloc(Width*Height*sizeof(unsigned char));
        image->B=malloc(Width*Height*sizeof(unsigned char));

        return image;
}

void DeleteImage(IMAGE *image){
        assert(image->B);
        free(image->B);
        image->B = NULL;

        assert(image->G);
        free(image->G);
        image->G = NULL;

        assert(image->R);
        free(image->R);
        image->R = NULL;

        assert(image);
        free(image);
        image = NULL;

}


/* Return the image's width in pixels */
unsigned int ImageWidth(const IMAGE *image){
	assert(image);
	return image->W;
}

/* Return the image's height in pixels */
unsigned int ImageHeight(const IMAGE *image){
	assert(image);
        return image->H;
}
