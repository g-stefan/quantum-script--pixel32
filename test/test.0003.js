// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression tests of the Pixel32 extension.
// Run by test/test.0003.cpp (fabricare test), or directly:
//     quantum-script test/test.0003.js

Script.requireExtension("Console");
Script.requireExtension("Random");
Script.requireExtension("Pixel32");

var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("* failed: " + name + ": \"" + value + "\", expected \"" + expected + "\"");
	++failed;
};

function checkThrows(name, fn) {
	var thrown = false;
	try {
		fn();
	} catch (e) {
		thrown = true;
	};
	check(name + " throws", thrown, true);
};

function color(r, g, b, a) {
	return new Pixel32.Pixel(r, g, b, a);
};

var red = color(255, 0, 0, 255);
var blue = color(0, 0, 255, 255);

// --- Pixel

check("pixel toString", "" + color(1, 2, 3, 4), "01020304");
check("pixel typeof", typeof(red), "Pixel32.Pixel");
check("pixel without new", "" + Pixel32.Pixel(16, 32, 48, 64), "10203040");
check("pixel no arguments", "" + new Pixel32.Pixel(), "00000000");
check("pixel missing alpha is 0", "" + color(255, 0, 0), "FF000000");
check("pixel clamp high", "" + color(300, 256, 1000, 255), "FFFFFFFF");
check("pixel clamp low", "" + color(-5, -1, 7.9, 255), "000007FF");
check("pixel NaN", "" + color(0 / 0, 1, 1, 1), "00010101");
check("pixel string is hex", "" + color("x", 1, 1, 1), "00000000");
check("pixel string RRGGBBAA", "" + new Pixel32.Pixel("FF8000C0"), "FF8000C0");
check("pixel string lower case", "" + new Pixel32.Pixel("ff8000c0"), "FF8000C0");
check("pixel string RRGGBB", "" + new Pixel32.Pixel("FF8000"), "FF8000FF");
check("pixel string bad", "" + new Pixel32.Pixel("GG8000FF"), "00000000");
check("pixel string bad length", "" + new Pixel32.Pixel("FF80"), "00000000");
check("pixel copy", "" + new Pixel32.Pixel(color(9, 8, 7, 6)), "09080706");

var p = color(10, 20, 30, 40);
check("getR", p.getR(), 10);
check("getG", p.getG(), 20);
check("getB", p.getB(), 30);
check("getA", p.getA(), 40);
p.setR(300).setG(-1).setB(5.5).setA(128);
check("setters clamp and chain", "" + p, "FF0005" + "80");
p.setR("x");
check("setter NaN ignored", p.getR(), 255);

// --- Image

var image = new Pixel32.Image(4, 3);
check("image size", image.getWidth() + "x" + image.getHeight(), "4x3");
check("image typeof", typeof(image), "Pixel32.Image");
check("image starts transparent", "" + image.getPixel(0, 0), "00000000");
check("image bad size", Pixel32.Image(0, 5), undefined);
check("image negative size", Pixel32.Image(-1, 5), undefined);
check("image NaN size", Pixel32.Image("x", 5), undefined);

image.setPixel(1, 2, red);
check("setPixel / getPixel", "" + image.getPixel(1, 2), "FF0000FF");
check("getPixel outside", "" + image.getPixel(10, 10), "00000000");
check("getPixel negative", "" + image.getPixel(-1, 0), "00000000");
image.setPixel(-1, 0, red);
image.setPixel(99, 0, red);
check("setPixel outside ignored", "" + image.getPixel(0, 0), "00000000");
image.setPixel(0, 2, blue);
check("getPixelX clamps below", "" + image.getPixelX(-5, 7), "0000FFFF");
check("getPixelX clamps above", "" + image.getPixelX(1, 99), "FF0000FF");

image.clear(blue);
check("clear", "" + image.getPixel(3, 2), "0000FFFF");

// --- Cut, copy, blend with clipping

var big = new Pixel32.Image(8, 8);
big.setPixel(0, 0, red);
var part = big.cut(-2, -2, 4, 4);
check("cut clipped size", part.getWidth() + "x" + part.getHeight(), "2x2");
check("cut clipped pixel", "" + part.getPixel(0, 0), "FF0000FF");
check("cut outside", big.cut(20, 20, 4, 4), undefined);
check("cut full is a copy", "" + big.cut(0, 0, 8, 8).getPixel(0, 0), "FF0000FF");

var source = new Pixel32.Image(4, 4);
source.clear(red);
var target = new Pixel32.Image(4, 4);
target.copy(source, -2, -2, 0, 0, 4, 4);
check("copy negative position", "" + target.getPixel(1, 1), "FF0000FF");
check("copy negative position, outside part", "" + target.getPixel(2, 2), "00000000");

target = new Pixel32.Image(4, 4);
target.blend(source, -2, -2, 0, 0, 4, 4);
check("blend negative position", "" + target.getPixel(1, 1), "FF0000FF");
check("blend negative position, outside part", "" + target.getPixel(2, 2), "00000000");

target = new Pixel32.Image(4, 4);
target.clear(blue);
source.clear(color(255, 0, 0, 0));
target.blend(source, 0, 0, 0, 0, 4, 4);
check("blend transparent keeps destination", "" + target.getPixel(0, 0), "0000FFFF");

target = new Pixel32.Image(4, 4);
target.drawFilledRectangle(-2, -2, 4, 4, red);
check("drawFilledRectangle clipped", "" + target.getPixel(1, 1), "FF0000FF");
check("drawFilledRectangle clipped, outside part", "" + target.getPixel(2, 2), "00000000");

target = new Pixel32.Image(5, 5);
target.drawRectangle(0, 0, 5, 5, red);
check("drawRectangle border", "" + target.getPixel(4, 2), "FF0000FF");
check("drawRectangle inside", "" + target.getPixel(2, 2), "00000000");

// --- Wrap

var line = new Pixel32.Image(4, 1);
line.setPixel(0, 0, red);
check("wrap positive", "" + line.wrap(1, 0).getPixel(3, 0), "FF0000FF");
check("wrap negative", "" + line.wrap(-1, 0).getPixel(1, 0), "FF0000FF");
var box = line.wrapBox(1, 1);
check("wrapBox size", box.getWidth() + "x" + box.getHeight(), "6x3");
check("wrapBox border", "" + box.getPixel(5, 0), "FF0000FF");

// --- Scale

var small = new Pixel32.Image(4, 4);
small.clear(red);
check("resize up", small.resize(10, 6).getWidth() + "x" + small.resize(10, 6).getHeight(), "10x6");
check("resize down", "" + small.resize(2, 3).getHeight(), "3");
check("resize keeps color", "" + small.resize(9, 9).getPixel(4, 4), "FF0000FF");
check("resize bad size", small.resize(0, 4), undefined);
check("scaleUp smaller fails", small.scaleUp(2, 2), undefined);
check("scaleUpBicubic", "" + small.scaleUpBicubic(8, 8).getWidth(), "8");
check("scaleUpBilinear", "" + small.scaleUpBilinear(8, 8).getWidth(), "8");
check("scaleUpNearestNeighbor", "" + small.scaleUpNearestNeighbor(8, 8).getPixel(7, 7), "FF0000FF");
check("scaleDown", "" + small.scaleDown(3, 3).getWidth(), "3");
check("scaleDownX2", "" + small.scaleDownX2().getWidth(), "2");
check("scaleDownX2OnX", small.scaleDownX2OnX().getWidth() + "x" + small.scaleDownX2OnX().getHeight(), "2x4");
check("scaleDownX2OnY", small.scaleDownX2OnY().getWidth() + "x" + small.scaleDownX2OnY().getHeight(), "4x2");
var onePixel = new Pixel32.Image(1, 1);
check("scaleDownX2 of 1 pixel fails", onePixel.scaleDownX2(), undefined);

// --- Average, colorRescale

var mixA = new Pixel32.Image(1, 1);
var mixB = new Pixel32.Image(1, 1);
mixA.clear(color(200, 0, 0, 255));
mixB.clear(color(0, 0, 100, 255));
check("average returns this", mixA.average(mixB, 1, 1, 2) === mixA, true);
check("average", "" + mixA.getPixel(0, 0), "640032FF");
check("average delta 0 changes nothing", "" + mixA.average(mixB, 1, 1, 0).getPixel(0, 0), "640032FF");
check("average bad image", mixA.average("x", 1, 1, 2), undefined);

var levels = new Pixel32.Image(2, 1);
levels.setPixel(0, 0, color(100, 100, 100, 255));
levels.setPixel(1, 0, color(150, 150, 150, 255));
levels.colorRescale();
check("colorRescale low", "" + levels.getPixel(0, 0), "000000FF");
check("colorRescale high", "" + levels.getPixel(1, 0), "FFFFFFFF");

// --- Kernel3X3

var identity = new Pixel32.Kernel3X3();
check("kernel typeof", typeof(identity), "Pixel32.Kernel3X3");
check("kernel default center", identity.getMatrixV(1, 1), 1);
check("kernel default corner", identity.getMatrixV(0, 0), 0);
check("kernel default normal", identity.getNormalA() + "/" + identity.getNormalB(), "1/1");
check("kernel out of range", identity.getMatrixV(3, 0), undefined);
check("predefined blur", Pixel32.Kernel3X3.blur.getNormalB(), 9);

// getMatrixV(x, y) / setMatrixV(x, y, v): x is the column, y is the row
var shift = new Pixel32.Kernel3X3();
shift.setMatrixV(1, 1, 0).setMatrixV(2, 0, 1);
check("kernel setMatrixV x, y", shift.getMatrixV(2, 0) + "," + shift.getMatrixV(0, 2), "1,0");
var dot = new Pixel32.Image(3, 3);
dot.setPixel(1, 1, red);
// result (x, y) = input (x + 1, y - 1)
var shifted = dot.kernel3X3(shift);
check("kernel column / row order", "" + shifted.getPixel(0, 2), "FF0000FF");
check("kernel column / row order, empty", "" + shifted.getPixel(2, 0), "00000000");

check("kernel3X3 identity", "" + dot.kernel3X3(identity).getPixel(1, 1), "FF0000FF");
check("kernel3X3 blur size", "" + small.kernel3X3(Pixel32.Kernel3X3.blur).getWidth(), "4");
check("kernel3X3 blur flat", "" + small.kernel3X3(Pixel32.Kernel3X3.gaussian).getPixel(1, 1), "FF0000FF");
checkThrows("kernel3X3 not a kernel", function() {
	dot.kernel3X3(5);
});

// --- Methods on a wrong this

checkThrows("Image method on other object", function() {
	var o = {};
	o.getWidth = Pixel32.Image.prototype.getWidth;
	o.getWidth();
});
checkThrows("Pixel method on other object", function() {
	var o = {};
	o.getR = Pixel32.Pixel.prototype.getR;
	o.getR();
});

// --- Noise

var rnd = new Random();
rnd.seed(12345);
var noiseA = new Pixel32.Image(8, 8);
noiseA.noise(rnd);
rnd.seed(12345);
var noiseB = new Pixel32.Image(8, 8);
noiseB.noise(rnd);
check("noise is reproducible", "" + noiseA.getPixel(5, 3), "" + noiseB.getPixel(5, 3));
check("noise is opaque", noiseA.getPixel(5, 3).getA(), 255);
noiseB.noise2Bit(rnd);
check("noise2Bit is opaque", noiseB.getPixel(2, 2).getA(), 255);

// --- Perlin noise

var octaves = [
	[1, 1, 0],
	[2, 1, 1],
	[4, 2, 1],
	[8, 4, 1],
	[16, 8, 1],
	[32, 16, 1],
	[64, 32, 1]
];

rnd.seed(1);
var perlin = Pixel32.perlinNoiseWrapBox(64, 48, octaves, rnd);
check("perlinNoiseWrapBox size", perlin.getWidth() + "x" + perlin.getHeight(), "64x48");
// 64 / 64 = 1 pixel, 48 / 64 < 1 pixel: the last octave is skipped
rnd.seed(1);
var perlinSmall = Pixel32.perlinNoise2BitWrapBox(32, 32, octaves, rnd);
check("perlin with skipped octaves", perlinSmall.getWidth() + "x" + perlinSmall.getHeight(), "32x32");
check("perlin all octaves skipped", Pixel32.perlinNoiseWrapBox(1, 1, [[4, 1, 1]], rnd), undefined);

// the texture tiles: the step from the last column to the first one is
// not larger than the step between neighbor columns
function columnStep(img, a, b) {
	var sum = 0;
	var y;
	var d;
	for (y = 0; y < img.getHeight(); ++y) {
		d = img.getPixel(a, y).getR() - img.getPixel(b, y).getR();
		sum += (d < 0) ? -d : d;
	};
	return sum / img.getHeight();
};
rnd.seed(7);
var seamless = Pixel32.perlinNoiseWrapBox(128, 128, octaves, rnd);
var stepInside = 0;
for (var x = 0; x < 127; ++x) {
	stepInside += columnStep(seamless, x, x + 1);
};
stepInside /= 127;
check("perlin tiles", columnStep(seamless, 127, 0) < stepInside * 2, true);

var tile = new Pixel32.Image(4, 4);
tile.noise(rnd);
var tileResized = tile.wrapBox1Resize(32, 32, 8, 0);
check("wrapBox1Resize size", tileResized.getWidth() + "x" + tileResized.getHeight(), "32x32");

// --- PNG, written to the current folder (output/test when run by fabricare test)

var pngFile = "test.0003.png";
var png = new Pixel32.Image(3, 2);
png.setPixel(0, 0, red);
png.setPixel(2, 1, color(1, 2, 3, 4));
check("pngSave", png.pngSave(pngFile), true);
var loaded = Pixel32.pngLoad(pngFile);
check("pngLoad size", loaded.getWidth() + "x" + loaded.getHeight(), "3x2");
check("pngLoad pixel", "" + loaded.getPixel(0, 0), "FF0000FF");
check("pngLoad alpha", "" + loaded.getPixel(2, 1), "01020304");
check("pngLoad missing", Pixel32.pngLoad("test.0003.missing.png"), undefined);

// --- Values copied to a thread (Variable::clone)

Script.requireExtension("Thread");

var thread = Thread.newThread(function(pixel, img, kernel) {
	Script.requireExtension("Pixel32");
	img.setPixel(0, 0, pixel);
	return [img, "" + img.getPixel(0, 0), kernel.getNormalB(), pixel];
}, undefined, [red, new Pixel32.Image(2, 2), Pixel32.Kernel3X3.blur]);
thread.join();
var result = thread.getReturnedValue();
check("thread pixel", result[1], "FF0000FF");
check("thread kernel", result[2], 9);
check("thread image back", "" + result[0].getPixel(0, 0), "FF0000FF");
check("thread pixel back", "" + result[3], "FF0000FF");

if (failed > 0) {
	throw "test 0003 failed (" + failed + ")";
};

Console.writeLn("-> test 0003 ok");
