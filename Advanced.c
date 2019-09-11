#include "Advanced.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

/* Adjust the brightness of an image */
IMAGE *Brightness(IMAGE *image,int brightness){
	assert(image);
	assert(image->R);
        assert(image->G);
        assert(image->B);
	for (int i=0; i<ImageWidth(image); i++){
		for (int j=0; j<ImageHeight(image); j++){
			if (image->R[i+j*ImageWidth(image)] + brightness > 255){
				image->R[i+j*ImageWidth(image)] = 255;
			}else if(image->R[i+j*ImageWidth(image)] + brightness < 0){
				image->R[i+j*ImageWidth(image)] = 0;
			}else{
				image->R[i+j*ImageWidth(image)] += brightness;
			}

			if (image->G[i+j*ImageWidth(image)] + brightness > 255){
				image->G[i+j*ImageWidth(image)] = 255;
			}else if(image->G[i+j*ImageWidth(image)] + brightness < 0){
				image->G[i+j*ImageWidth(image)] = 0;
			}else{
				image->G[i+j*ImageWidth(image)] += brightness;
			}

			if (image->B[i+j*ImageWidth(image)] + brightness > 255){
				image->B[i+j*ImageWidth(image)] = 255;
			}else if(image->B[i+j*ImageWidth(image)] + brightness < 0){
				image->B[i+j*ImageWidth(image)] = 0;
			}else{
				image->B[i+j*ImageWidth(image)] += brightness;
			}
		}
	}
	return image;
}

/* Mirror an image horizontally */
IMAGE *HMirror(IMAGE *image) {
	assert(image);
	assert(image->R);
        assert(image->G);
        assert(image->B);
	int x, y;
    for (y = 0; y < ImageHeight(image); y++) {
        for (x = 0; x < ImageWidth(image) / 2; x++) {
					image->R[ImageWidth(image)-1-x+y*ImageWidth(image)] = image->R[x+y*ImageWidth(image)];
					image->G[ImageWidth(image)-1-x+y*ImageWidth(image)] = image->G[x+y*ImageWidth(image)];
					image->B[ImageWidth(image)-1-x+y*ImageWidth(image)] = image->B[x+y*ImageWidth(image)];
		}
	}
	return image;
}

void matrix_3_3multiplyVector_3(float vector[3], const float matrix[3][3])
{
	/* Memory for result */
	float result[3] = {0, 0, 0};

	/* Perform multiplication */
	result[0] = matrix[0][0]*vector[0] + matrix[0][1]*vector[1] + matrix[0][2]*vector[2];
	result[1] = matrix[1][0]*vector[0] + matrix[1][1]*vector[1] + matrix[1][2]*vector[2];
	result[2] = matrix[2][0]*vector[0] + matrix[2][1]*vector[1] + matrix[2][2]*vector[2];

	/* Copy result into other stack's memory */
	vector[0] = result[0];
	vector[1] = result[1];
	vector[2] = result[2];
}

IMAGE *HueRotate(IMAGE *image, float angle)
{
	assert(image);
	assert(image->R);
	assert(image->G);
	assert(image->B);
	/* Matrix to transform normalized
	// RGB color vector from the RGB
	// color space to the YIQ color space */
	static const float RGBtoYIQMatrix[3][3] = {{0.299,  0.587,  0.114},
								  			   {0.596, -0.274, -0.321},
								  			   {0.211, -0.523,  0.311}};

	/* Matrix to transform normalized
	// YIQ color vector from the YIQ
	// color space to the RGB color space */
	static const float YIQtoRGBMatrix[3][3] = {{1.000,  0.956,  0.621},
								  			   {1.000, -0.272, -0.647},
								  			   {1.000, -1.107,  1.705}};

	const float cs = cos(angle), ss = sin(angle);

	/* Matrix to perform a hue
	// rotation on a normalized
	// YIQ color vector*/
	const float XYPlaneVectorRotateMatrix[3][3] = {{1.000, 0.000, 0.000},
												   {0.000, cs, -ss},
												   {0.000, ss, cs}};

	/* Image variables */
	float temp[3];

	for(unsigned int i = 0; i < ImageWidth(image); i++)
	{
		for(unsigned int j = 0; j < ImageHeight(image); j++)
		{
			/* Create a normalized RGB color
			// vector fro the current pixel
			// in the image */
			temp[0] = (float)image->R[i+j*ImageWidth(image)]/255;
			temp[1] = (float)image->G[i+j*ImageWidth(image)]/255;
			temp[2] = (float)image->B[i+j*ImageWidth(image)]/255;

			/* Transform the RGB vector
			// to a YIQ vector */
			matrix_3_3multiplyVector_3(temp, RGBtoYIQMatrix);

			/* Perform the DIP */
			matrix_3_3multiplyVector_3(temp, XYPlaneVectorRotateMatrix);

			/* Transform back to RGB */
			matrix_3_3multiplyVector_3(temp, YIQtoRGBMatrix);

			/* Denormalize and store back into the image */
			temp[0] = temp[0] * 255;
			temp[0] = temp[0] < 0 ? 0 : temp[0];
			temp[0] = temp[0] > 255 ? 255 : temp[0];
			temp[1] = temp[1] * 255;
			temp[1] = temp[1] < 0 ? 0 : temp[1];
			temp[1] = temp[1] > 255 ? 255 : temp[1];
			temp[2] = temp[2] * 255;
			temp[2] = temp[2] < 0 ? 0 : temp[2];
			temp[2] = temp[2] > 255 ? 255 : temp[2];

			image->R[i+j*ImageWidth(image)] = (unsigned char)temp[0];
			image->G[i+j*ImageWidth(image)] = (unsigned char)temp[1];
			image->B[i+j*ImageWidth(image)] = (unsigned char)temp[2];
		}
	}
	return image;
}

IMAGE *Saturate(IMAGE *image, float percent){
	assert(image);
	assert(image->R);
	assert(image->G);
	assert(image->B);
	float Rt, Gt, Bt; 
	int x,y;
	for(x = 0; x < ImageWidth(image); x++){
		for(y=0;y<ImageHeight(image); y++){
			Rt = (image->R[x+y*(ImageWidth(image))]+image->G[x+y*(ImageWidth(image))]+image->B[x+y*(ImageWidth(image))])/3;
			Gt = Rt;
			Bt = Rt;
			/*Difference of Original and Temporary R value*/
			Rt = image->R[x+y*(ImageWidth(image))]-Rt;
			Gt = image->G[x+y*(ImageWidth(image))]-Gt;
			Bt = image->B[x+y*(ImageWidth(image))]-Bt;
	
			/*Add the difference multiplied by percentage with conditional checks to keep pixel values in range of 0-255*/
			Rt = image->R[x+y*(ImageWidth(image))] + (Rt * percent)/100.00;
			if(Rt<0){
                                Rt = 0;
                        }
                        else if(Rt>255){
                                Rt = 255;
                        }
			Gt  = image->G[x+y*(ImageWidth(image))] + (Gt * percent)/100.00;
			if(Gt<0){
                                Gt = 0;
                        }
                        else if(Gt>255){
                                Gt = 255;
                        }
			Bt = image->B[x+y*(ImageWidth(image))] + (Bt * percent)/100.0;
			if(Bt<0){
                                Bt = 0;
                        }
                        else if(Bt>255){
                                Bt = 255;
                        }
			/*Pixel values cast back into unsigned char*/
			image->R[x+y*ImageWidth(image)] = (unsigned char)Rt;
			image->G[x+y*ImageWidth(image)] = (unsigned char)Gt;
			image->B[x+y*ImageWidth(image)] = (unsigned char)Bt;

		}
	}
	return image;
}

IMAGE *Resize(IMAGE *image, int percentage){
	assert(image);
	assert(image->R);
        assert(image->G);
        assert(image->B);
	/*x,x2,y,y2 are used in for loops*/
	/*xp1,xp2,yp1,yp2 are the pixel positions of the original x and y multiplied by percent ratio*/
	int x,y,xp1,yp1,xp2,yp2,x2,y2;
	int Width = ImageWidth(image) * ((float)percentage/100);
	int Height = ImageHeight(image) * ((float)percentage/100);
	float avgR, avgG,avgB;
	int sumR=0,sumG=0,sumB=0; 
	/*No change*/
	if(percentage == 100){	
	}
	/*Enlarge*/	
	else if(percentage > 100){
		IMAGE *tempImage = NULL;
		tempImage = CreateImage(Width,Height);
		//Dividing the index in the original image by the ratio of the larger image  allows a pixel to cover multiple pixels in the new image
		for(x=0;x<Width;x++){
			for(y=0;y<Height;y++){
						tempImage->R[x+y*Width]=image->R[(int)(x/(percentage/100.00))+(int)(y/(percentage/100.00))*ImageWidth(image)];
						tempImage->G[x+y*Width]=image->G[(int)(x/(percentage/100.00))+(int)(y/(percentage/100.00))*ImageWidth(image)];	
						tempImage->B[x+y*Width]=image->B[(int)(x/(percentage/100.00))+(int)(y/(percentage/100.00))*ImageWidth(image)];		
                                        	
			}
		}
		/*Builds new image with width and height of the enlarged/shrunken image*/
		DeleteImage(image);
		image = CreateImage(Width,Height);
		for(x=0;x<Width;x++){
                        for(y=0;y<Height;y++){
                                image->R[x+y*Width]=tempImage->R[(x+y*Width)];
                                image->G[x+y*Width]=tempImage->G[(x+y*Width)];
                                image->B[x+y*Width]=tempImage->B[(x+y*Width)];

                        }
                }
		DeleteImage(tempImage);		
	}
	/*Shrink*/
	else if(percentage<100){
		IMAGE *tempImage = NULL;
		tempImage = CreateImage(Width,Height);
		for(x=0;x<Width;x++){
                        for(y=0;y<Height;y++){
				avgR = avgG = avgB = 0;
				sumR = sumB = sumG = 0;
				xp1 = (x/(percentage/100.00));
                                yp1 = (y/(percentage/100.00));
				xp2 = ((x+1)/(percentage/100.00));
				yp2 = ((y+1)/(percentage/100.00));
				for(x2=xp1;x2<xp2; x2++){
					for(y2=yp1;y2<yp2; y2++){
						sumR = sumR + image->R[x2+y2*ImageWidth(image)];
						sumG = sumG + image->G[x2+y2*ImageWidth(image)];
						sumB = sumB + image->B[x2+y2*ImageWidth(image)];
					}
				}
				
				avgR = sumR/((xp2-xp1)*(yp2-yp1));
				if(avgR<0){
                                                avgR = 0;
                                        }
                                else if(avgR>255){
                                                avgR = 255;
				}
				avgG = sumG/((xp2-xp1)*(yp2-yp1));
                                if(avgG<0){
                                                avgG = 0;
                                        }
                                else if(avgG>255){
                                                avgG = 255;
				}
				avgB = sumB/((xp2-xp1)*(yp2-yp1));
                                if(avgB<0){
                                                avgB = 0;
                                        }
                                else if(avgB>255){
                                                avgB = 255;
					}
				tempImage->R[x+y*Width] = (unsigned char)avgR;
				tempImage->G[x+y*Width] = (unsigned char)avgG;
				tempImage->B[x+y*Width] = (unsigned char)avgB;

				
				}
						
			}
			DeleteImage(image);
			image = CreateImage(Width,Height);
			for(x=0;x<Width;x++){
				for(y=0;y<Height;y++){
					image->R[x+y*Width]=tempImage->R[(x+y*Width)];
					image->G[x+y*Width]=tempImage->G[(x+y*Width)];
					image->B[x+y*Width]=tempImage->B[(x+y*Width)];

				}
			}	
			DeleteImage(tempImage);
		}
	
	return image;
}

IMAGE *Rotate(IMAGE *image, int option){
	assert(image);
        assert(image->R);
        assert(image->G);
        assert(image->B);
        IMAGE *tempImage = NULL;
	int x,y;

	/*Horizontal flip*/
	if(option == 1){
		tempImage = CreateImage(ImageWidth(image),ImageHeight(image));
                //Flip Image
                for(x=0;x<ImageWidth(image);x++){
                        for(y=0;y<ImageHeight(image);y++){
                                tempImage->R[(ImageWidth(image)-x-1)+(y)*ImageWidth(image)] = image->R[x+y*ImageWidth(image)];
                                tempImage->G[(ImageWidth(image)-x-1)+(y)*ImageWidth(image)] = image->G[x+y*ImageWidth(image)];
                                tempImage->B[(ImageWidth(image)-x-1)+(y)*ImageWidth(image)] = image->B[x+y*ImageWidth(image)];

                        }
                }
                //Set OG image to Flipped Image
                for(x=0;x<ImageWidth(image);x++){
                        for(y=0;y<ImageHeight(image);y++){
                                image->R[x+y*ImageWidth(image)]=tempImage->R[(x+y*ImageWidth(image))];
                                image->G[x+y*ImageWidth(image)]=tempImage->G[(x+y*ImageWidth(image))];
                                image->B[x+y*ImageWidth(image)]=tempImage->B[(x+y*ImageWidth(image))];
                        }
                }
                DeleteImage(tempImage);
        }
	//Vertical Flip
	else if(option == 2){
		tempImage = CreateImage(ImageWidth(image),ImageHeight(image));
		//Flip Image
		for(x=0;x<ImageWidth(image);x++){
			for(y=0;y<ImageHeight(image);y++){
				tempImage->R[(x)+(ImageHeight(image)-y-1)*ImageWidth(image)] = image->R[x+y*ImageWidth(image)];
				tempImage->G[(x)+(ImageHeight(image)-y-1)*ImageWidth(image)] = image->G[x+y*ImageWidth(image)];
				tempImage->B[(x)+(ImageHeight(image)-y-1)*ImageWidth(image)] = image->B[x+y*ImageWidth(image)];
			}
		}
		//Set OG image to Flipped Image
		for(x=0;x<ImageWidth(image);x++){
                        for(y=0;y<ImageHeight(image);y++){
                                image->R[x+y*ImageWidth(image)]=tempImage->R[(x+y*ImageWidth(image))];
                                image->G[x+y*ImageWidth(image)]=tempImage->G[(x+y*ImageWidth(image))];
                                image->B[x+y*ImageWidth(image)]=tempImage->B[(x+y*ImageWidth(image))];
                        }
                }
		DeleteImage(tempImage);
	}
	else if(option == 3){
                tempImage = CreateImage(ImageHeight(image),ImageWidth(image));
                for(x=0;x<ImageWidth(tempImage);x++){
                        for(y=0;y<ImageHeight(tempImage);y++){
				tempImage->R[(x)+y*ImageWidth(tempImage)] = image->R[(y)+(ImageHeight(image)-x-1)*ImageWidth(image)];
                                tempImage->G[(x)+y*ImageWidth(tempImage)] = image->G[(y)+(ImageHeight(image)-x-1)*ImageWidth(image)];
                                tempImage->B[(x)+y*ImageWidth(tempImage)] = image->B[(y)+(ImageHeight(image)-x-1)*ImageWidth(image)];
                        }
                }
                DeleteImage(image);
                image = CreateImage(ImageWidth(tempImage),ImageHeight(tempImage));
                for(x=0;x<ImageWidth(tempImage);x++){
                        for(y=0;y<ImageHeight(tempImage);y++){
                                image->R[x+y*ImageWidth(tempImage)]=tempImage->R[(x+y*ImageWidth(tempImage))];
                                image->G[x+y*ImageWidth(tempImage)]=tempImage->G[(x+y*ImageWidth(tempImage))];
                                image->B[x+y*ImageWidth(tempImage)]=tempImage->B[(x+y*ImageWidth(tempImage))];
                        }
                }
                DeleteImage(tempImage);
        }
	else if(option == 4){
		tempImage = CreateImage(ImageHeight(image),ImageWidth(image));
		for(x=0;x<ImageWidth(tempImage);x++){
			for(y=0;y<ImageHeight(tempImage);y++){
				tempImage->R[(x)+y*ImageWidth(tempImage)] = image->R[(ImageWidth(image)-y-1)+x*ImageWidth(image)];
                                tempImage->G[(x)+y*ImageWidth(tempImage)] = image->G[(ImageWidth(image)-y-1)+x*ImageWidth(image)];
                                tempImage->B[(x)+y*ImageWidth(tempImage)] = image->B[(ImageWidth(image)-y-1)+x*ImageWidth(image)];
			}
		}
		DeleteImage(image);
		image = CreateImage(ImageWidth(tempImage),ImageHeight(tempImage));
		for(x=0;x<ImageWidth(tempImage);x++){
                        for(y=0;y<ImageHeight(tempImage);y++){
                                image->R[x+y*ImageWidth(tempImage)]=tempImage->R[(x+y*ImageWidth(tempImage))];
                                image->G[x+y*ImageWidth(tempImage)]=tempImage->G[(x+y*ImageWidth(tempImage))];
                                image->B[x+y*ImageWidth(tempImage)]=tempImage->B[(x+y*ImageWidth(tempImage))];

                        }
                }
		DeleteImage(tempImage);
	}
		
	return image;

}
/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
