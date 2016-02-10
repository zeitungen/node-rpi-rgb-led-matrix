/*
 *	Copyright 2016, Maxime Journaux <journaux.maxime@gmail.com>
 * 	This work is free. You can redistribute it and/or modify it under the
 *	terms of the Do What The Fuck You Want To Public License, Version 2,
 *	as published by Sam Hocevar. 
 *	See http://www.wtfpl.net for more details.
 */


var path = require("path");
var util = require("util");

var ledmatrix = require(path.join(__dirname, "build", "Release", "node-rpi-rgb-led-matrix.node"));
var LedMatrix = ledmatrix.LedMatrix;

module.exports = LedMatrix;