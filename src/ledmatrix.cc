/*
 *	Copyright 2015, Maxime Journaux <journaux.maxime@gmail.com>
 */

#include <v8.h>
#include <node.h>

#include <stdlib.h>

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
}

LedMatrix::~LedMatrix() {
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

void LedMatrix::Fill(uint8_t r, uint8_t g, uint8_t b) {
	matrix->Fill(r, g, b);
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

	if(!args.Length() == 5 || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()
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

	matrix->Clear();
}

void LedMatrix::Fill(const Nan::FunctionCallbackInfo<v8::Value>& args) {

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.Holder());

	if(!args.Length() == 3 || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		Nan::ThrowTypeError("Wrong parameters! Expects 3 numbers");
	}

	int r = args[0]->ToInteger()->Value();
  	int g = args[1]->ToInteger()->Value();
  	int b = args[2]->ToInteger()->Value();
  	matrix->Fill(r, g, b);

}

