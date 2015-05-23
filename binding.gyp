{
	"targets": [
		{
	        "target_name": "node-rpi-rgb-led-matrix",
	        "sources": [ "src/base.cc", "src/ledmatrix.cc" ],
	        "dependencies": [ "./binding.gyp:rpi-rgb-led-matrix" ],
	        "include_dirs": [ "./external/rpi-rgb-led-matrix/include", "./include/" ]
	    },
		{
			"target_name": "rpi-rgb-led-matrix",
			"type": "static_library",
			"sources": ["external/rpi-rgb-led-matrix/lib/bdf-font.cc",
				"external/rpi-rgb-led-matrix/lib/framebuffer.cc", "external/rpi-rgb-led-matrix/lib/gpio.cc",
				"external/rpi-rgb-led-matrix/lib/graphics.cc", "external/rpi-rgb-led-matrix/lib/led-matrix.cc",
				"external/rpi-rgb-led-matrix/lib/thread.cc"],
			"libraries": ["-lrt", "-lm", "-lpthread"],
			"include_dirs": [ "external/rpi-rgb-led-matrix/include" ],
	        "direct_dependent_settings": {
	            "include_dirs": [ "external/rpi-rgb-led-matrix/" ]
	        }
		}
	]
}