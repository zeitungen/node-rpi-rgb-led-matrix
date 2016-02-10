/*
 *	Copyright 2016, Maxime Journaux <journaux.maxime@gmail.com>
 * 	This work is free. You can redistribute it and/or modify it under the
 *	terms of the Do What The Fuck You Want To Public License, Version 2,
 *	as published by Sam Hocevar. 
 *	See http://www.wtfpl.net for more details.
 */

#include <nan.h>

#include <ledmatrix.h>

void init(v8::Local<v8::Object> exports) {
  LedMatrix::Init(exports);
}

NODE_MODULE(node_rpi_rgb_led_matrix, init);