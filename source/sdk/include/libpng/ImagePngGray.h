/***************************************************************************************************
* Copyright (C), 2015-2016 Suzhou Kunhou Automation Co.,Ltd
* File name:    ImagePngGray.h
* Description:
* Version:      0.0.1
* History: 
* (1) Author:   taibaidl
*     Date:     2016-07-14
*     Operate:  Create
*     Version:  0.0.1
* Others: 
***************************************************************************************************/
#ifndef _IMAGE_PNG_GRAY_H_
#define _IMAGE_PNG_GRAY_H_

#include <iostream>
#include "KHTypes.h"
#include "libpng/png.h"

class ImagePngGray
{
public:
	ImagePngGray();
	~ImagePngGray();

public:
	bool imread(STRING file_path);

	//
	bool imwrite(U8     *pImgBuf,
		         S32     img_w,
		         S32     img_h,
		         STRING  file_name);

private:
	void releaseImage();

public:
	U8    *data_;
	S32    width_;
	S32    height_;

private:
	S32    color_type_;
};


#endif//_IMAGE_PNG_GRAY_H_