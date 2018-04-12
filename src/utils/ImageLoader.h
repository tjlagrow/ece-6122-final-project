#ifndef ECE_6122_FINAL_PROJECT_IMAGELOADER_H
#define ECE_6122_FINAL_PROJECT_IMAGELOADER_H


class ImageLoader
{
public:

	static unsigned char *load(const char *filepath, int *width, int *height);
	static void free(unsigned char *bits);

private:
};


#endif //ECE_6122_FINAL_PROJECT_IMAGELOADER_H
