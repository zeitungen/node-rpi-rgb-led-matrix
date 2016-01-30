/*
 *	Copyright 2015, Maxime Journaux <journaux.maxime@gmail.com>
 */

#ifndef NODE_LED_MATRIX_H
#define NODE_LED_MATRIX_H

#include <node.h>
#include <node_object_wrap.h>
#include <nan.h>

#include <led-matrix.h> 

using namespace rgb_matrix;
using rgb_matrix::GPIO;


/**
 * Class: LedMatrix
 *	Wrapper for rpi-rgb-led-matrix.
 */
class LedMatrix : public node::ObjectWrap {
	public:
		/**
		 * Variable: constructor
		 *	Used to create nodejs constructor.
		 */
		static Nan::Persistent<v8::Function> constructor;

		/**
	 	 * Function: Initialize
	 	 *   Used to intialize the EventEmitter from Node.js
		 *
		 * Parameters:
		 *   target - v8::Object the Node.js global module object
		 */
		static void Init(v8::Local<v8::Object> exports);

		int GetWidth();
		int GetHeight();
		void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
		void Clear();
		void Fill(uint8_t r, uint8_t g, uint8_t b);

	protected:
		LedMatrix(int rows = 16, int chained_displays = 1, int parallel_displays = 1);

		virtual ~LedMatrix();

		static void New(const Nan::FunctionCallbackInfo<v8::Value>& args);

		static void GetWidth(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void GetHeight(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void SetPixel(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void Clear(const Nan::FunctionCallbackInfo<v8::Value>& args);
		static void Fill(const Nan::FunctionCallbackInfo<v8::Value>& args);

	private:

		GPIO io;
		RGBMatrix* matrix;
};

#endif