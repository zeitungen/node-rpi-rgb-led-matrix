# node-rpi-rgb-led-matrix
Pilot your rgb led matrix with Nodejs on Raspberry Pi ! Nodejs binding of rpi-rgb-led-matrix library https://github.com/hzeller/rpi-rgb-led-matrix

## Compatibility
NOTE: node-rpi-rgb-led-matrix should work across Node.js versions 0.8, 0.10, 0.12, 4 and 5 as it uses [Native Abstractions](https://github.com/nodejs/nan). *HOWEVER* there is a problem with the Node.js version 0.10.29 packaged with Raspbian Jessie 2015-11-21 which breaks Native Abstractions. The easiest fix is to make a minor edit to your v8.js file; this and other solutions outlined on [this Raspberry Pi Forum post](https://www.raspberrypi.org/forums/viewtopic.php?f=66&t=127939). 

### Installation with npm
```
$ npm i node-rpi-rgb-led-matrix
```

### Installation with git

```
$ git clone --recursive https://github.com/zeitungen/node-rpi-rgb-led-matrix.git
$ cd node-rpi-rgb-led-matrix
$ npm i
```

## Get some Fun

```js
var LedMatrix = require("node-rpi-rgb-led-matrix");

var matrix = new LedMatrix(16);
matrix.fill(255, 50, 100);
matrix.setPixel(0, 0, 0, 50, 255);
```

## LedMatrix API

* **Constructor: LedMatrix(rows, chainedDisplays, parallelDisplays)**
  default values are `rows=32`, `chainedDisplays=1`, `parallelDisplay=1`
* **getWidth()** return current led matrix width
* **getHeight()** return current led matrix height
* **fill(red, green, blue)** fill the led matrix with color specified
  by `red`, `green` and `blue`. `red`, `green` and `blue` must be between `0` and `255` 
* **setPixel(x, y, red, green, blue)** set the color pixel at the position `x`,`y` 
  with color specified by `red`, `green` and `blue`. `red`, `green` and `blue` must be
  between `0` and `255`. `x` must be between `0` and `getWidth()`. `y` must be between
  `0`and `getGeight()`
* **clear()** reset the led matrix display (shut down all leds)


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

**Don't forget to recompile**:
```
$ cd /path/to/node-rpi-rgb-led-matrix
$ node-gyp build
```
or run `npm i`.
