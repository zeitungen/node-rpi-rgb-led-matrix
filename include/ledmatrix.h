/*
 *	Copyright 2015, Maxime Journaux <journaux.maxime@gmail.com>
 */

#ifndef NODE_LED_MATRIX_H
#define NODE_LED_MATRIX_H

#include <node.h>
#include <node_object_wrap.h>

#include <led-matrix.h> 

using namespace rgb_matrix;
using rgb_matrix::GPIO;

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

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
		static v8::Persistent<v8::Function> constructor;

		/**
	 	 * Function: Initialize
	 	 *   Used to intialize the EventEmitter from Node.js
		 *
		 * Parameters:
		 *   target - v8::Object the Node.js global module object
		 */
		static void Initialize(v8::Local<v8::Object> target);

		int GetWidth();
		int GetHeight();
		void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
		void Clear();
		void Fill(uint8_t r, uint8_t g, uint8_t b);

	protected:
		LedMatrix(int rows = 16, int chained_displays = 1, int parallel_displays = 1);

		virtual ~LedMatrix();

		static void New(const FunctionCallbackInfo<Value>& args);

		static void GetWidth(const FunctionCallbackInfo<v8::Value>& args);
		static void GetHeight(const FunctionCallbackInfo<v8::Value>& args);
		static void SetPixel(const FunctionCallbackInfo<v8::Value>& args);
		static void Clear(const FunctionCallbackInfo<v8::Value>& args);
		static void Fill(const FunctionCallbackInfo<v8::Value>& args);

	private:
		v8::Handle<v8::Object> self;

		GPIO io;
		RGBMatrix* matrix;
};

#endif