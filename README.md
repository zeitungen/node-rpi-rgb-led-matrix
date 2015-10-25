# node-rpi-rgb-led-matrix
Pilot your rgb led matrix with Nodejs ! Nodejs binding of rpi-rgb-led-matrix library https://github.com/hzeller/rpi-rgb-led-matrix

## Add specific defines for rpi-rgb-led-matrix

You can add specific defines for the rpi-rgb-led-matrix compilation.

You have to modify the `binding.gyp` file. In the `rpi-rgb-led-matrix` target add `defines`array like this:
```json
...
  {
			"target_name": "rpi-rgb-led-matrix",
			"type": "static_library",
			"sources": ["external/matrix/lib/bdf-font.cc",
				"external/matrix/lib/framebuffer.cc", "external/matrix/lib/gpio.cc",
				"external/matrix/lib/graphics.cc", "external/matrix/lib/led-matrix.cc",
				"external/matrix/lib/thread.cc"],
			"libraries": ["-lrt", "-lm", "-lpthread"],
			"defines": ["RGB_CLASSIC_PINOUT", "ONLY_SINGLE_CHAIN"],
			"include_dirs": [ "external/matrix/include" ],
	        "direct_dependent_settings": {
	            "include_dirs": [ "external/matrix/include" ]
	        }
		}
...
```

You have this defines possibility:
* `INVERSE_RGB_DISPLAY_COLORS`
* `SHOW_REFRESH_RATE`
* `RGB_SLOWDOWN_GPIO=1` or `RGB_SLOWDOWN_GPIO=2`
* `ADAFRUIT_RGBMATRIX_HAT`
* `ADAFRUIT_RGBMATRIX_HAT_PWM`
* `RGB_CLASSIC_PINOUT`
* `ONLY_SINGLE_CHAIN`

Look at [rpi-rgb-led-matrix Makefile](https://github.com/hzeller/rpi-rgb-led-matrix/blob/master/lib/Makefile)
for more details of this defines.
