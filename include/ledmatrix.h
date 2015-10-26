/*
 *	Copyright 2015, Maxime Journaux <journaux.maxime@gmail.com>
 */

#ifndef NODE_LED_MATRIX_H
#define NODE_LED_MATRIX_H

#include <node.h>

#include <led-matrix.h> 

using namespace v8;
using namespace node;
using namespace rgb_matrix;
using rgb_matrix::GPIO;

/**
 * Class: LedMatrix
 *	Wrapper for rpi-rgb-led-matrix.
 */
class LedMatrix : public ObjectWrap {
	public:
		/**
		 * Variable: constructor_template
		 *	Used to create nodejs constructor.
		 */
		static v8::Persistent<v8::FunctionTemplate> constructor_template;

		/**
	     * Function: Initialize
	     *   Used to intialize the EventEmitter from Node.js
	     *
	     * Parameters:
	     *   target - v8::Object the Node.js global module object
	     */
		static void Initialize(v8::Handle<v8::Object> target);

		int GetWidth();
		int GetHeight();
		void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
		void Clear();
		void Fill(uint8_t r, uint8_t g, uint8_t b);

	protected:
		LedMatrix(int rows = 16, int chained_displays = 1, int parallel_displays = 1);

		virtual ~LedMatrix();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static v8::Handle<v8::Value> GetWidth(const v8::Arguments& args);
		static v8::Handle<v8::Value> GetHeight(const v8::Arguments& args);
		static v8::Handle<v8::Value> SetPixel(const v8::Arguments& args);
		static v8::Handle<v8::Value> Clear(const v8::Arguments& args);
		static v8::Handle<v8::Value> Fill(const v8::Arguments& args);

	private:
		v8::Handle<v8::Object> self;

		GPIO io;
		RGBMatrix* matrix;
};

#endif