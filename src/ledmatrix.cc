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

LedMatrix::LedMatrix(int rows, int chained_displays, int parallel_displays) {
	assert(io.Init());
	matrix = new RGBMatrix(&io, rows, chained_displays, parallel_displays);	
	matrix->set_luminance_correct(true);
}

LedMatrix::~LedMatrix() {
	delete matrix;
}

void LedMatrix::Initialize(Handle<v8::Object> target) {
	HandleScope handle;

	Local<FunctionTemplate> t = FunctionTemplate::New(LedMatrix::New);

	constructor_template = Persistent<FunctionTemplate>::New(t);
	constructor_template->InstanceTemplate()->SetInternalFieldCount(1);
	constructor_template->SetClassName(String::NewSymbol("LedMatrix"));

	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getWidth", GetWidth);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "getHeight", GetHeight);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "setPixel", SetPixel);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "clear", Clear);
	NODE_SET_PROTOTYPE_METHOD(constructor_template, "fill", Fill);

	target->Set(String::NewSymbol("LedMatrix"), constructor_template->GetFunction());
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

Handle<Value> LedMatrix::New(const Arguments& args) {
	HandleScope scope;

	assert(args.IsConstructCall());

	if(args.Length() == 0 || !args[0]->IsNumber()) {
    	return ThrowException(Exception::Error(String::New("Arguments error")));
    }

    if(args.Length() == 1 || !args[1]->IsNumber()) {
    	return ThrowException(Exception::Error(String::New("Arguments error")));
    }

    if(args.Length() == 2 || !args[2]->IsNumber()) {
    	return ThrowException(Exception::Error(String::New("Arguments error")));
    }

    int rows = args[0]->ToInteger()->Value();
    int chained = args[1]->ToInteger()->Value();
    int parallel = args[2]->ToInteger()->Value();

	LedMatrix* matrix = new LedMatrix(rows, chained, parallel);
	matrix->Wrap(args.This());

	matrix->self = Persistent<Object>::New(args.This());

	return scope.Close(args.This());
}

Handle<Value> LedMatrix::GetWidth(const v8::Arguments& args) {
	HandleScope scope;

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	return Integer::New(matrix->GetWidth());
}

Handle<Value> LedMatrix::GetHeight(const Arguments& args) {
	HandleScope scope;

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	return Integer::New(matrix->GetHeight()); 
}

Handle<Value> LedMatrix::SetPixel(const Arguments& args) {
	HandleScope scope;

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	if(!args.Length() == 5 || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()
	|| !args[3]->IsNumber() || !args[4]->IsNumber()) {
    	return ThrowException(Exception::Error(String::New("Parameters error")));
  	}

  	int x = args[0]->ToInteger()->Value();
  	int y = args[1]->ToInteger()->Value();
  	int r = args[2]->ToInteger()->Value();
  	int g = args[3]->ToInteger()->Value();
  	int b = args[4]->ToInteger()->Value();

  	matrix->SetPixel(x, y, r, g, b);

  	return Undefined();
}

Handle<Value> LedMatrix::Clear(const Arguments& args) {
	HandleScope scope;

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	matrix->Clear();

	return Undefined();
}

Handle<Value> LedMatrix::Fill(const Arguments& args) {
	HandleScope scope;

	LedMatrix* matrix = ObjectWrap::Unwrap<LedMatrix>(args.This());

	if(!args.Length() == 3 || !args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
    	return ThrowException(Exception::Error(String::New("Parameters error")));
  	}

	int r = args[0]->ToInteger()->Value();
  	int g = args[1]->ToInteger()->Value();
  	int b = args[2]->ToInteger()->Value();
  	matrix->Fill(r, g, b);

  	return Undefined();
}



Persistent<FunctionTemplate> LedMatrix::constructor_template;