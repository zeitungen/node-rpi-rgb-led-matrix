/*
 *	Copyright 2016, Maxime Journaux <journaux.maxime@gmail.com>
 * 	This work is free. You can redistribute it and/or modify it under the
 *	terms of the Do What The Fuck You Want To Public License, Version 2,
 *	as published by Sam Hocevar. 
 *	See http://www.wtfpl.net for more details.
 */
 
#ifndef NODE_LEDMATRIX_SCROLLER_IMAGE_H
#define NODE_LEDMATRIX_SCROLLER_IMAGE_H

#include <stdint.h>

class Pixel {
	public:
		// Pixel() : red(0), green(0), blue(0) {}
		Pixel();

		uint8_t R() const;
		uint8_t G() const;
		uint8_t B() const;

		void SetR(uint8_t r);
		void SetG(uint8_t g);
		void SetB(uint8_t b);

	private:
		uint8_t red;
		uint8_t green;
		uint8_t blue;
};

class Image {
	public:
		// Image() : width(0), height(0), pixels(NULL) {}
		Image();
		~Image();

		bool IsValid();
		Pixel& GetPixel(int x, int y);
		Pixel* GetPixels();
		int GetWidth();
		int GetHeight();

		void SetPixels(Pixel* pxls);
		void SetWidth(int w);
		void SetHeight(int h);

		void Reset();
		void Remove();

	private:
		int width;
		int height;
		Pixel* pixels;
};

#endif
