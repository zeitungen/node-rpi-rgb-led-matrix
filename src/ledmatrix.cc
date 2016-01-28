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

Persistent<Function> LedMatrix::constructor;

LedMatrix::LedMatrix(int rows, int chained_displays, int parallel_displays) {
	assert(io.Init());
	matrix = new RGBMatrix(&io, rows, chained_displays, parallel_displays);	
	matrix->set_luminance_correct(true);
}

LedMatrix::~LedMatrix() {
	delete matrix;
}

void LedMatrix::Initialize(Handle<v8::Object> target) {

	Isolate* isolate = target->GetIsolate(); 
	HandleScope handle(isolate);
	
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, LedMatrix::New);

	tpl->SetClassName(String::NewFromUtf8(isolate, "LedMatrix"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tpl, "getWidth", GetWidth);
	NODE_SET_PROTOTYPE_METHOD(tpl, "getHeight", GetHeight);
	NODE_SET_PROTOTYPE_METHOD(tpl, "setPixel", SetPixel);
	NODE_SET_PROTOTYPE_METHOD(tpl, "clear", Clear);
	NODE_SET_PROTOTYPE_METHOD(tpl, "fill", Fill);
	
	constructor.Reset(isolate, tpl->GetFunction());

	target->Set(String::NewFromUtf8(isolate, "LedMatrix"), tpl->GetFunction());
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

void LedMatrix::New(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate(); 
	HandleScope scope(isolate);

	// throw an error if it's not a constructor 
	if (!args.IsConstructCall()) {
		isolate->ThrowException(Exception::TypeError(
			String::NewFromUtf8(isolate, "LedMatrix::must be called as a constructor with 'new' keyword")));
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

void LedMatrix::GetWidth(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate(); 
	HandleScope scope(isolate);

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());
	
	args.GetReturnValue().Set(Number::New(isolate, matrix->GetWidth()));
}

void LedMatrix::GetHeight(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate(); 
	HandleScope scope(isolate);

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	args.GetReturnValue().Set(Number::New(isolate, matrix->GetHeight()));
}

void LedMatrix::SetPixel(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate(); 
	HandleScope scope(isolate);

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	if(!args.Length() == 5 || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()
	|| !args[3]->IsNumber() || !args[4]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Parameters error")));
  	}

  	int x = args[0]->ToInteger()->Value();
  	int y = args[1]->ToInteger()->Value();
  	int r = args[2]->ToInteger()->Value();
  	int g = args[3]->ToInteger()->Value();
  	int b = args[4]->ToInteger()->Value();

  	matrix->SetPixel(x, y, r, g, b);

}

void LedMatrix::Clear(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate(); 
	HandleScope scope(isolate);

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	matrix->Clear();
}

void LedMatrix::Fill(const FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate(); 
	HandleScope scope(isolate);

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	if(!args.Length() == 3 || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Parameters error")));
	}

	int r = args[0]->ToInteger()->Value();
  	int g = args[1]->ToInteger()->Value();
  	int b = args[2]->ToInteger()->Value();
  	matrix->Fill(r, g, b);

}

