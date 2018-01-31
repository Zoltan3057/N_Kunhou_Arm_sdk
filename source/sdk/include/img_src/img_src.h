#ifndef _IMG_SRC_WANGHONGTAO_20160803_
#define _IMG_SRC_WANGHONGTAO_20160803_

#include <string>
#include <vector>
#include <fstream>

#include <linux/videodev2.h>

typedef unsigned char  BYTE;
typedef unsigned short	WORD;
typedef unsigned long  DWORD;


typedef struct tagBITMAPFILEHEADER{
     WORD	bfType;                // the flag of bmp, value is "BM"
     DWORD    bfSize;                // size BMP file ,unit is bytes
     DWORD    bfReserved;            // 0
     DWORD    bfOffBits;             // must be 54

}BITMAPFILEHEADER;


typedef struct tagBITMAPINFOHEADER{
     DWORD    biSize;                // must be 0x28
     DWORD    biWidth;           //
     DWORD    biHeight;          //
     WORD		biPlanes;          // must be 1
     WORD		biBitCount;            //
     DWORD    biCompression;         //
     DWORD    biSizeImage;       //
     DWORD    biXPelsPerMeter;   //
     DWORD    biYPelsPerMeter;   //
     DWORD    biClrUsed;             //
     DWORD    biClrImportant;        //
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD{
     BYTE	rgbBlue;
     BYTE	rgbGreen;
     BYTE	rgbRed;
     BYTE	rgbReserved;
}RGBQUAD;


#define  IMAGEWIDTH    640
#define  IMAGEHEIGHT   480

class img_src{
public:
	img_src();
	~img_src();




	//if successful return 1,or else return 0
	bool openVideo();
	bool closeVideo();

	//data 用来存储数据的空间, size空间大小
	bool getVideoData(unsigned char *data, int& size);

private:
	bool read_frame(unsigned char *data, int& size);
	bool v4l2_grab(void);

	void yuyv_to_rgb(unsigned char* yuv,unsigned char* rgb);
	int yuyv_2_rgb888(char* yuv,unsigned char* rgb);
private:
	int    fd;
	struct v4l2_capability   cap;
	struct v4l2_fmtdesc fmtdesc;
	struct v4l2_format fmt,fmtack;
	struct v4l2_streamparm setfps;
	struct v4l2_requestbuffers req;
	struct v4l2_buffer buf;
	enum v4l2_buf_type type;
	unsigned char frame_buffer[IMAGEWIDTH*IMAGEHEIGHT*3];

	struct buffer
	{
		void * start;
		unsigned int length;
	} * buffers;
};


#endif//_IMG_SRC_WANGHONGTAO_20160803_
