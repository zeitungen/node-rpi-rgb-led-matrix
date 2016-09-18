/*
 *	Copyright 2016, Maxime Journaux <journaux.maxime@gmail.com>
 * 	This work is free. You can redistribute it and/or modify it under the
 *	terms of the Do What The Fuck You Want To Public License, Version 2,
 *	as published by Sam Hocevar. 
 *	See http://www.wtfpl.net for more details.
 */

#include <v8.h>
#include <node.h>

#include <stdlib.h>
#include <unistd.h>

#include <led-matrix.h>
#include <ledmatrix.h>

using namespace v8;
using namespace node;
using namespace rgb_matrix;
using rgb_matrix::GPIO;

Nan::Persistent<v8::Function> LedMatrix::constructor;

LedMatrix::LedMatrix(int rows, int chained_displays, int parallel_displays) {
	assert(io.Init());
	matrix = new RGBMatrix(&io, rows, chained_displays, parallel_displays);	
	matrix->set_luminance_correct(true);
	image = NULL;
}

LedMatrix::~LedMatrix() {
	delete image;
	delete matrix;
}

void LedMatrix::Init(v8::Local<v8::Object> exports) {

	Nan::HandleScope scope;
	
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	
	tpl->SetClassName(Nan::New("LedMatrix").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Nan::SetPrototypeMethod(tpl, "getWidth", GetWidth);
	Nan::SetPrototypeMethod(tpl, "getHeight", GetHeight);
	Nan::SetPrototypeMethod(tpl, "setPixel", SetPixel);
	Nan::SetPrototypeMethod(tpl, "clear", Clear);
	Nan::SetPrototypeMethod(tpl, "fill", Fill);
	Nan::SetPrototypeMethod(tpl, "setImageBuffer", SetImageBuffer);
	Nan::SetPrototypeMethod(tpl, "draw", Draw);
	Nan::SetPrototypeMethod(tpl, "scroll", Scroll);
	
	constructor.Reset(tpl->GetFunction());

	exports->Set(Nan::New("LedMatrix").ToLocalChecked(), tpl->GetFunction());
}

int LedMatrix::GetWidth() {
	return matrix->width();
}

int LedMatrix::GetHeight() {
	return matrix->height();
}

void LedMatrix::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
	matrix->SetPixel(x, y, r, g, b);
}

void LedMatrix::Clear() {
	matrix->Clear();
}

void LedMatrix::Clear(int x, int y, int w, int h) {
	for(int j=y; j<y+h; j++) {
		for(int i=x; i<x+w; i++) {
			matrix->SetPixel(i, j, 0, 0, 0);
		}
	}
}

void LedMatrix::Fill(uint8_t r, uint8_t g, uint8_t b) {
	matrix->Fill(r, g, b);
}

void LedMatrix::SetImage(Image* img) {
	if(image && image->IsValid()) {
		delete image;
		image = NULL;
	}
	image = new Image();
	image->Reset();
	image->SetWidth(img->GetWidth());
	image->SetHeight(img->GetHeight());
	Pixel* p = (Pixel*)malloc(sizeof(Pixel)*image->GetWidth()*image->GetHeight());
	memcpy(p, img->GetPixels(), sizeof(Pixel)*image->GetWidth()*image->GetHeight());
	image->SetPixels(p);
}

void LedMatrix::Draw(int screenx, int screeny, int width, int height, int imgx, int imgy, bool looph, bool loopv) {
	int w = width;
	if(image->GetWidth() - imgx < w && !looph)
		w = image->GetWidth() - imgx;

	int h = height;
	if(image->GetHeight() - imgy < h && !loopv)
		h = image->GetHeight() - imgy;

	for(int i = 0; i < w; i++) {
		int px = (i + imgx) % image->GetWidth();
		int sx = i + screenx;
		for(int j = 0; j < h; j++) {
			int py = (j + imgy) % image->GetHeight();
			int sy = j + screeny;
			Pixel p = image->GetPixel(px, py);
			matrix->SetPixel(sx, sy, p.R(), p.G(), p.B());
		}
	}
}	

void LedMatrix::New(const Nan::FunctionCallbackInfo<Value>& args) {
	// throw an error if it's not a constructor 
	if (!args.IsConstructCall()) {
		Nan::ThrowError("LedMatrix::must be called as a constructor with 'new' keyword");
	}

	// grab parameters
	int rows = 32;
	int chained = 1;
	int parallel = 1;

	if(args.Length() > 0 && args[0]->IsNumber()) {
		rows = args[0]->ToInteger()->Value();
	}
	if(args.Length() > 1 && args[1]->IsNumber()) {
		chained = args[1]->ToInteger()->Value();
	}
	if(args.Length() > 2 && args[2]->IsNumber()) {
		parallel = args[2]->ToInteger()->Value();
	}

	// make the matrix
	LedMatrix* matrix = new LedMatrix(rows, chained, parallel);
	matrix->Wrap(args.This());

	// return this object
	args.GetReturnValue().Set(args.This());
}

void LedMatrix::GetWidth(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.Holder());
	args.GetReturnValue().Set(Nan::New<v8::Number>(matrix->GetWidth()));
}

void LedMatrix::GetHeight(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.Holder());
	args.GetReturnValue().Set(Nan::New<v8::Number>(matrix->GetHeight()));
}

void LedMatrix::SetPixel(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.Holder());

	if(args.Length() != 5 || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()
	|| !args[3]->IsNumber() || !args[4]->IsNumber()) {
		Nan::ThrowTypeError("Wrong parameters! Expects 5 numbers");
  	}

  	int x = args[0]->ToInteger()->Value();
  	int y = args[1]->ToInteger()->Value();
  	int r = args[2]->ToInteger()->Value();
  	int g = args[3]->ToInteger()->Value();
  	int b = args[4]->ToInteger()->Value();

  	matrix->SetPixel(x, y, r, g, b);
}

void LedMatrix::Clear(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.Holder());

	if(args.Length() == 4 && args[0]->IsNumber() && args[1]->IsNumber() && args[2]->IsNumber() && args[3]->IsNumber()) {
		int x = args[0]->ToInteger()->Value();
	  	int y = args[1]->ToInteger()->Value();
	  	int w = args[2]->ToInteger()->Value();
	  	int h = args[3]->ToInteger()->Value();
	  	matrix->Clear(x, y, w, h);
	} else {
		matrix->Clear();
	}
}

void LedMatrix::Fill(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.Holder());

	if(args.Length() != 3 || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		Nan::ThrowTypeError("Wrong parameters! Expects 3 numbers");
	}

	int r = args[0]->ToInteger()->Value();
  	int g = args[1]->ToInteger()->Value();
  	int b = args[2]->ToInteger()->Value();
  	matrix->Fill(r, g, b);
}

void LedMatrix::SetImageBuffer(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	if(args.Length() < 3 || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		return Nan::ThrowTypeError("Parameters error: SetImageBuffer(buffer, width, height)");
	}

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.Holder());

	char* buf = Buffer::Data(args[0]->ToObject());
	size_t bufl = Buffer::Length(args[0]->ToObject());
	int width = args[1]->ToInteger()->Value();
	int height = args[2]->ToInteger()->Value();

	assert((int)bufl == width*height*3);

	Image* img = new Image();
	Pixel* pixels = (Pixel*) malloc(sizeof(Pixel)*width*height);
	for(int i=0; i < width*height; i++) {
		int j = i*3;
		Pixel p;
		p.SetR(buf[j]);
		p.SetG(buf[j+1]);
		p.SetB(buf[j+2]);
		pixels[i] = p;
	}

	img->Reset();
	img->SetPixels(pixels);
	img->SetWidth(width);
	img->SetHeight(height);

	matrix->SetImage(img);
}

void LedMatrix::Draw(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	int startx = 0;
	int starty = 0;
	if(args.Length() > 0 && args[0]->IsNumber()) startx = args[0]->ToInteger()->Value();
	if(args.Length() > 1 && args[1]->IsNumber()) starty = args[1]->ToInteger()->Value();

	int width = matrix->GetWidth() - startx;
	int height = matrix->GetHeight() - starty;
	int imgx = 0;
	int imgy = 0;
	bool looph = false;
	bool loopv = false;

	if(args.Length() > 2 && args[2]->IsNumber()) width = args[2]->ToInteger()->Value();
	if(args.Length() > 3 && args[3]->IsNumber()) height = args[3]->ToInteger()->Value();
	if(args.Length() > 4 && args[4]->IsNumber()) imgx = args[4]->ToInteger()->Value();
	if(args.Length() > 5 && args[5]->IsNumber()) imgy = args[5]->ToInteger()->Value();
	if(args.Length() > 6 && args[6]->IsBoolean()) looph = args[6]->ToBoolean()->Value();
	if(args.Length() > 7 && args[7]->IsBoolean()) loopv = args[7]->ToBoolean()->Value();


    matrix->Draw(startx, starty, width, height, imgx, imgy, looph, loopv);
}

void LedMatrix::Scroll(const Nan::FunctionCallbackInfo<v8::Value>& args) {
	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	if(args.Length() == 0 || !args[0]->IsFunction()) {
    	return Nan::ThrowTypeError("Callback is required and must be a Function");
  	}

	int startx = 0;
	int starty = 0;
	if(args.Length() > 1 && args[1]->IsNumber()) startx = args[1]->ToInteger()->Value();
	if(args.Length() > 2 && args[2]->IsNumber()) starty = args[2]->ToInteger()->Value();

	int width = matrix->GetWidth() - startx;
	int height = matrix->GetHeight() - starty;
	int scroll = SCROLL_TO_LEFT;
	int speed = 1;
	int loop = 0;

	if(args.Length() > 3 && args[3]->IsNumber()) width = 	args[3]->ToInteger()->Value();
	if(args.Length() > 4 && args[4]->IsNumber()) height = 	args[4]->ToInteger()->Value();
	if(args.Length() > 5 && args[5]->IsNumber()) scroll = 	args[5]->ToInteger()->Value();
	if(args.Length() > 6 && args[6]->IsNumber()) speed = 	args[6]->ToInteger()->Value();
	if(args.Length() > 7 && args[7]->IsNumber()) loop = 	args[7]->ToInteger()->Value();

	// convert to ms
	speed = speed * 1000;

	uvscroll* uv = new uvscroll();
	uv->matrix = matrix;	uv->callback = new Nan::Callback(Local<Function>::Cast(args[0]));
	uv->startx = startx;	uv->starty = starty;
	uv->width = width;		uv->height = height;
	uv->scroll = scroll;	uv->loop = loop;
	uv->speed = speed;

	matrix->Ref();

	uv_work_t* w = new uv_work_t;
	w->data = uv;
	int r = uv_queue_work(uv_default_loop(), w, UV_Scroll, UV_AfterScroll);
	if(r != 0) {
		delete uv;
		delete w;

		matrix->Unref();

		return Nan::ThrowTypeError("Internal error: Failed to queue scroll work");
	}
}

void LedMatrix::UV_Scroll(uv_work_t* work) {
	uvscroll* uv = static_cast<uvscroll*>(work->data);

	assert(uv->matrix != NULL);

	if(uv->scroll == SCROLL_TO_LEFT) {
		if(uv->loop > 0) {
			for(int l = 0; l < uv->loop; l++) {
				for(int i = 0; i < uv->matrix->image->GetWidth(); i++) {
					uv->matrix->Clear(uv->startx, uv->starty, uv->width, uv->height);
					uv->matrix->Draw(uv->startx, uv->starty, uv->width, uv->height, i, 0, true, false);
					usleep(uv->speed);
				}
				uv->matrix->Clear(uv->startx, uv->starty, uv->width, uv->height);
				uv->matrix->Draw(uv->startx, uv->starty, uv->width, uv->height, 0, 0, true, false);
			}
		} else {
			for(int i = 0; i < uv->matrix->image->GetWidth(); i++) {
				uv->matrix->Clear(uv->startx, uv->starty, uv->width, uv->height);
				uv->matrix->Draw(uv->startx, uv->starty, uv->width, uv->height, i, 0, false, false);
				usleep(uv->speed);
			}
		}
	} else if(uv->scroll == SCROLL_TO_RIGHT) {
		if(uv->loop < 1) {
			for(int i = uv->startx; i < uv->width; i++) {
				uv->matrix->Clear(uv->startx, uv->starty, uv->width, uv->height);
				uv->matrix->Draw(i, uv->starty, uv->width-i, uv->height, 0, 0, false, false);
				usleep(uv->speed);
			}
		} else {
			for(int l = 0; l < uv->loop; l++) {
				for(int i = uv->matrix->image->GetWidth()-1; i >= 0 ; i--) {
					uv->matrix->Clear(uv->startx, uv->starty, uv->width, uv->height);
					uv->matrix->Draw(uv->startx, uv->starty, uv->width, uv->height, i, 0, true, false);
					usleep(uv->speed);
				}
			}
		}
	} else if(uv->scroll == SCROLL_TO_TOP) {
		if(uv->loop > 0) {
			for(int l = 0; l < uv->loop; l++) {
				for(int i = 0; i < uv->matrix->image->GetHeight(); i++) {
					uv->matrix->Clear(uv->startx, uv->starty, uv->width, uv->height);
					uv->matrix->Draw(uv->startx, uv->starty, uv->width, uv->height, 0, i, false, true);
					usleep(uv->speed);
				}
				uv->matrix->Clear(uv->startx, uv->starty, uv->width, uv->height);
				uv->matrix->Draw(uv->startx, uv->starty, uv->width, uv->height, 0, 0, false, true);	
			}
		} else {
			for(int i = 0; i < uv->matrix->image->GetHeight(); i++) {
				uv->matrix->Draw(uv->startx, uv->starty, uv->width, uv->height, 0, i, false, false);
				usleep(uv->speed);
			}
		}
	} else if(uv->scroll == SCROLL_TO_BOTTOM) {
		if(uv->loop < 1) {
			for(int i = uv->starty; i < uv->height; i++) {
				uv->matrix->Clear(uv->startx, uv->starty, uv->width, uv->height);
				uv->matrix->Draw(uv->startx, i, uv->width, uv->height-1, 0, 0, false, false);
				usleep(uv->speed);
			}
		} else {
			for(int l = 0; l < uv->loop; l++) {
				for(int i = uv->matrix->image->GetHeight()-1; i >= 0 ; i--) {
					uv->matrix->Clear(uv->startx, uv->starty, uv->width, uv->height);
					uv->matrix->Draw(uv->startx, uv->starty, uv->width, uv->height, 0, i, false, true);
					usleep(uv->speed);
				}
			}
		}
	}
}

void LedMatrix::UV_AfterScroll(uv_work_t* work, int status) {
	uvscroll* uv = static_cast<uvscroll*>(work->data);
	uv->matrix->Unref();
	uv->callback->Call(0, 0);
	delete uv;
	delete work;
}
