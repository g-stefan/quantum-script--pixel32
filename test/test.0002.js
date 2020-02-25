// Public domain
// http://unlicense.org/

Script.requireExtension("Console");
Script.requireExtension("Pixel32");

var rnd = new Random();

var x = Pixel32.perlinNoise2BitWrapBox(512, 512,
		[
			[1, 1, 0],
			[2, 1, 1],
			[4, 2, 1],
			[8, 4, 1],
			[16, 8, 1],
			[32, 16, 1],
			[64, 32, 1]
		]
		, rnd);

for(k = 0; k < 64; ++k) {
	x = x.kernel3X3(Pixel32.Kernel3X3.blur);
};

x.pngSave("test/output.0002.png");

Console.writeLn("-> test 0002 ok");
