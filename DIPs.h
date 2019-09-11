#ifndef DIPS_H_INCLUDED_
#define DIPS_H_INCLUDED_

#include "Constants.h"
#include "Image.h"
/* change color image to black & white */
IMAGE *BlackNWhite(IMAGE *image);

/* edge detection */
IMAGE  *Edge(IMAGE *image);

/* Shuffle the image */
IMAGE *Shuffle(IMAGE *image);

#endif /* DIPS_H_INCLUDED_ */

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
