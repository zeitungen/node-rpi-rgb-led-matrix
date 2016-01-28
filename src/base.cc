/*
 *	Copyright 2015, Maxime Journaux <journaux.maxime@gmail.com>
 */

#include <node.h>
#include <v8.h>

#include <ledmatrix.h>

void init(Local<v8::Object> target) {
  LedMatrix::Initialize(target);
}

NODE_MODULE(node_rpi_rgb_led_matrix, init);