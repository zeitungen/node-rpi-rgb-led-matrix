/*
 *	Copyright 2016, Maxime Journaux <journaux.maxime@gmail.com>
 * 	This work is free. You can redistribute it and/or modify it under the
 *	terms of the Do What The Fuck You Want To Public License, Version 2,
 *	as published by Sam Hocevar. 
 *	See http://www.wtfpl.net for more details.
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>

#include <image.h>

Pixel::Pixel() : red(0), green(0), blue(0) {}

uint8_t Pixel::R() const { return red; }
uint8_t Pixel::G() const { return green; }
uint8_t Pixel::B() const { return blue; }

void Pixel::SetR(uint8_t r) { red = r; }
void Pixel::SetG(uint8_t g) { green = g; }
void Pixel::SetB(uint8_t b) { blue = b; }

Image::Image() : width(-1), height(-1), pixels(NULL) {}

Image::~Image() {
 	Remove();
}

bool Image::IsValid() {
	return GetPixels() && GetHeight() > 0 && GetWidth() > 0;
}

Pixel& Image::GetPixel(int x, int y) {
	return pixels[x + GetWidth() * y];
}

Pixel* Image::GetPixels() { return pixels; }
int Image::GetWidth() { return width; }
int Image::GetHeight() { return height; }

void Image::SetPixels(Pixel* pxls) {
	delete [] pixels;
	pixels = pxls;
}

void Image::SetWidth(int w) { width = w; }
void Image::SetHeight(int h) { height = h; }

void Image::Reset() {
	pixels = NULL;
	width = -1;
	height = -1;
}

void Image::Remove() {
	delete [] pixels;
	Reset();
}
