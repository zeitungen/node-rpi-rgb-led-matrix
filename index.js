/*
 *	Copyright 2015, Maxime Journaux <journaux.maxime@gmail.com>
 */

var path = require("path");
var util = require("util");

var ledmatrix = require(path.join(__dirname, "build", "Release", "node-rpi-rgb-led-matrix.node"));
var LedMatrix = ledmatrix.LedMatrix;

module.exports = LedMatrix;