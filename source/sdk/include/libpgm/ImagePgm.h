#ifndef _PGM_KYOSHO_2015_06_30_
#define _PGM_KYOSHO_2015_06_30_

#include <stdio.h>
#include <string>
#include <stdlib.h>

#define ROWS(img)  ((img)->rows)
#define COLS(img)  ((img)->cols)
#define NAME(img)   ((img)->name)

typedef struct 
{
  char *name;
  int rows, cols;
  int *data;
} IMAGE;


class ImagePgm
{
public:
	ImagePgm();
	~ImagePgm();

	static IMAGE* img_open(std::string filename);
	static IMAGE* img_creat(char *name, int nr, int nc);
	static int img_write(IMAGE *img,std::string filename);
	static int img_save(IMAGE *img,std::string filename);
	static void img_free(IMAGE* img);
	

	static int img_getpixel(IMAGE *img, int r, int c);
	static void img_setpixel(IMAGE *img,int r, int c, int val);
	
	
protected:
private:
	static IMAGE* img_alloc();
	static char* img_basename(std::string filename);
	
};


#endif//_PGM_KYOSHO_2015_06_30_