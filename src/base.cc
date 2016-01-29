/*
 *	Copyright 2015, Maxime Journaux <journaux.maxime@gmail.com>
 */

#include <nan.h>

#include <ledmatrix.h>

void init(v8::Local<v8::Object> exports) {
  LedMatrix::Init(exports);
}

NODE_MODULE(node_rpi_rgb_led_matrix, init);