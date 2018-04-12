#include "ImageLoader.h"
#include <FreeImage.h>

unsigned char* ImageLoader::load(const char *filepath, int *width, int *height)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP *fibmp = nullptr;

	// Try to get the type of image
	fif = FreeImage_GetFileType(filepath, 0);

	// If we can't figure out the type automatically
	// try to get it by the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filepath);

	// If we still don't know the image type, return NULL
	if (fif == FIF_UNKNOWN)
		return (nullptr);

	if (FreeImage_FIFSupportsReading(fif))
		fibmp = FreeImage_Load(fif, filepath);
	if (! fibmp)
		return (nullptr);

	BYTE *bits = FreeImage_GetBits(fibmp);
	*width = FreeImage_GetWidth(fibmp);
	*height = FreeImage_GetHeight(fibmp);
	if (bits == 0 || width == 0 || height == 0)
		return (nullptr);

	return (bits);
}


void ImageLoader::free(unsigned char *bits)
{
	free(bits);
}
