#ifndef FILEIO_H_INCLUDED_
#define FILEIO_H_INCLUDED_

#include "Constants.h"
#include "Image.h"

/* read image from a file */
IMAGE *LoadImage(const char *fname);

/* save a processed image */
int SaveImage(const char *fname, const IMAGE *image);

#endif /* FILEIO_H_INCLUDED_ */

/* vim: set tabstop=8 softtabstop=8 shiftwidth=8 noexpandtab : */
