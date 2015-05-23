/*
 *	Copyright 2015, Maxime Journaux <journaux.maxime@gmail.com>
 */

var path = require("path");
var util = require("util");
var events = require("events");

var ledmatrix = require(path.join(__dirname, "build", "Release", "node-rpi-rgb-led-matrix.node"));
var LedMatrix = ledmatrix.LedMatrix;

// var led = new LedMatrix(16, 1, 1);
// console.log(led);
// for(var i in led) console.log(i);

inherits(LedMatrix, events.EventEmitter);

exports.LedMatrix = LedMatrix;

// extend prototype
function inherits(target, source) {
  for (var k in source.prototype)
    target.prototype[k] = source.prototype[k];
}


/*
// javascript shim that lets our object inherit from EventEmitter
var nodewii = require(__dirname + '/build/Release/nodewii.node');
var WiiMote = nodewii.WiiMote;
var events = require('events');

inherits(WiiMote, events.EventEmitter);
copy(exports,nodewii);
exports.WiiMote = WiiMote;

// extend prototype
function inherits(target, source) {
  for (var k in source.prototype)
    target.prototype[k] = source.prototype[k];
}

// copy object
function copy ( target, source ) {
	for ( var k in source )
		target[k] = source[k];
}
//*/