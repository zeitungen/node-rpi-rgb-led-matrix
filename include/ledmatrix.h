/*
 *	Copyright 2016, Maxime Journaux <journaux.maxime@gmail.com>
 * 	This work is free. You can redistribute it and/or modify it under the
 *	terms of the Do What The Fuck You Want To Public License, Version 2,
 *	as published by Sam Hocevar. 
 *	See http://www.wtfpl.net for more details.
 */

#ifndef NODE_LED_MATRIX_H
#define NODE_LED_MATRIX_H

#include <node.h>
#include <node_object_wrap.h>
#include <nan.h>
#include <led-matrix.h>
#include <image.h>

using namespace rgb_matrix;
using rgb_matrix::GPIO;

#define SCROLL_TO_LEFT 		0x01
#define SCROLL_TO_RIGHT 	0x02
#define SCROLL_TO_BOTTOM 	0x03
#define SCROLL_TO_TOP 		0x04

/**
 * Class: LedMatrix
 *	Wrapper for rpi-rgb-led-matrix.
 */
class LedMatrix : public node::ObjectWrap {
	public:
		static Nan::Persistent<v8::Function> constructor;
		static void Init(v8::Local<v8::Object> exports);

		int GetWidth();
		int GetHeight();
		void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
		void Clear();
		void Clear(int x, int y, int w, int h);
		void Fill(uint8_t r, uint8_t g, uint8_t b);
		void SetImage(Image* img);
		void Draw(int screenx, int screeny, int width, int height, int imgx, int imgy,
					bool looph, bool loopv);

	protected:
		LedMatrix(int rows = 16, int chained_displays = 1, int parallel_displays = 1);

		virtual ~LedMatrix();

		static void New(const Nan::FunctionCallbackInfo<v8::Value>& args);

		static void GetWidth(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void GetHeight(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void SetPixel(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void Clear(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void Fill(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void SetImageBuffer(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void Draw(const Nan::FunctionCallbackInfo<v8::Value>& args);

		static void Scroll(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void UV_Scroll(uv_work_t* work);
		static void UV_AfterScroll(uv_work_t* work, int status);

	private:

		GPIO io;
		RGBMatrix* matrix;

		Image* image;

		struct uvscroll {
			LedMatrix* matrix;
			Nan::Callback* callback;
			int startx;	int starty;
			int width;	int height;
			int scroll;	int loop;
			int speed;
		};
};

#endif