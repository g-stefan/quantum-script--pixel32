# Script API

Everything lives in the global object `Pixel32`, created by
`Script.requireExtension("Pixel32")`:

| Name | Kind |
|------|------|
| `Pixel32.Pixel` | constructor of colors |
| `Pixel32.Image` | constructor of images |
| `Pixel32.Kernel3X3` | constructor of 3×3 filters; also holds the predefined filters |
| `Pixel32.pngLoad(fileName)` | function, loads a PNG file |
| `Pixel32.perlinNoiseWrapBox(lx, ly, octaves, rnd)` | function, gray tileable noise texture |
| `Pixel32.perlinNoise2BitWrapBox(lx, ly, octaves, rnd)` | function, same from black / white noise |

All three constructors work with or without `new`. `typeof(x)` gives
`"Pixel32.Pixel"`, `"Pixel32.Image"` and `"Pixel32.Kernel3X3"`; `"" + x`
gives the color text for a pixel and the type name for the other two.

## Numbers

Every number argument goes through `Convert.toNumber`, so `"12"` works as
`12`. Then:

| Argument | Rule |
|----------|------|
| coordinate (`x`, `y`, `dx`, `sx`, ...) | truncated toward zero (`2.9` → `2`, `-0.5` → `0`); negative is allowed; `NaN` / `Infinity` make the call do nothing |
| size (`w`, `h`, `lx`, `ly` of `cut`, `copy`, `blend`, scaling) | truncated; negative, `NaN` or `Infinity` make the call do nothing |
| color channel (`r`, `g`, `b`, `a`) | truncated and clamped to `0..255` (`300` → `255`, `-5` → `0`) |
| weight (`average`) | truncated, negative / `NaN` make the call do nothing |

"Do nothing" means: methods that return an image return `undefined`, the
in-place methods leave the image unchanged.

## Pixel32.Pixel

One RGBA color: red, green, blue, alpha, each `0..255`. Alpha is
*straight*: `255` is opaque, `0` fully transparent, and the color channels
are not multiplied by alpha.

```javascript
var red = new Pixel32.Pixel(255, 0, 0, 255);
var glass = new Pixel32.Pixel(255, 255, 255, 64);
var orange = new Pixel32.Pixel("FF8000");        // "RRGGBB": opaque
var shadow = new Pixel32.Pixel("00000080");      // "RRGGBBAA"
var copy = new Pixel32.Pixel(red);               // independent copy
```

| Call | Result |
|------|--------|
| `Pixel32.Pixel()` | `00000000` (transparent black) |
| `Pixel32.Pixel(r, g, b, a)` | channels converted as in [Numbers](#numbers); a missing or `NaN` channel is `0` — **`Pixel(255, 0, 0)` is transparent**, pass `255` for alpha |
| `Pixel32.Pixel("RRGGBBAA")` | 8 hex digits, upper or lower case |
| `Pixel32.Pixel("RRGGBB")` | 6 hex digits, alpha `FF` |
| `Pixel32.Pixel(otherString)` | `00000000`; any string first argument is read as hex text, `Pixel("12", 0, 0, 255)` is `00000000` |
| `Pixel32.Pixel(pixel)` | a copy |

Methods:

| Method | Returns | Notes |
|--------|---------|-------|
| `getR()`, `getG()`, `getB()`, `getA()` | Number `0..255` | |
| `setR(v)`, `setG(v)`, `setB(v)`, `setA(v)` | the pixel (chainable) | `v` clamped to `0..255`; `NaN` leaves the channel unchanged |
| `toString()` / `"" + pixel` | `"RRGGBBAA"` | upper case hex, the same text the constructor accepts |

```javascript
var p = new Pixel32.Pixel(10, 20, 30, 255);
p.setR(200).setA(128);
Console.writeLn(p);                 // C8141E80
```

Value model:

- A `Pixel` is an object: `q = p; q.setR(0);` changes `p` too. Copy with
  `new Pixel32.Pixel(p)`.
- `image.getPixel(x, y)` returns a **new** pixel each time; changing it does
  not change the image until you call `image.setPixel(x, y, pixel)`.
- `==` and `===` compare identity, not color: two pixels with the same
  color are not `==`. Compare the text: `"" + a == "" + b`.

## Pixel32.Image

A `width × height` array of pixels. `(0, 0)` is the **top left** corner, `x`
grows to the right, `y` grows down.

```javascript
var image = new Pixel32.Image(320, 200);         // all 00000000
var loaded = Pixel32.pngLoad("input.png");       // or undefined
```

`Pixel32.Image(w, h)` with a width or height of `0`, negative, `NaN` or too
large for memory returns `undefined`. With `new` the engine replaces an
`undefined` result by an empty object (no image methods), so when the size
comes from input either check it first or call without `new`:

```javascript
var image = Pixel32.Image(w, h);
if (Script.isUndefined(image)) {
	throw new Error("bad image size " + w + " x " + h);
};
```

### Size, pixels, files

| Method | Returns | Notes |
|--------|---------|-------|
| `getWidth()`, `getHeight()` | Number | |
| `getPixel(x, y)` | new `Pixel` | `00000000` outside the image |
| `getPixelX(x, y)` | new `Pixel` | outside the image: the nearest edge pixel (clamped) |
| `setPixel(x, y, pixel)` | `undefined` | replaces the pixel, alpha included; ignored outside the image or when `pixel` is not a `Pixel` |
| `clear(pixel)` | `undefined` | every pixel becomes `pixel` (replaced, not blended) |
| `pngSave(fileName)` | `true` / `false` | always 8-bit RGBA PNG |
| `Pixel32.pngLoad(fileName)` | `Image` / `undefined` | any PNG: gray, palette, transparency chunk, 16-bit, interlaced → RGBA |

A pixel loop (this is the slow path: two native calls per pixel; prefer the
image methods when one fits):

```javascript
var x, y, p, gray;
for (y = 0; y < image.getHeight(); ++y) {
	for (x = 0; x < image.getWidth(); ++x) {
		p = image.getPixel(x, y);
		gray = (p.getR() * 77 + p.getG() * 150 + p.getB() * 29) / 256;
		image.setPixel(x, y, p.setR(gray).setG(gray).setB(gray));
	};
};
```

### New images: resize, scale, cut, wrap

These return a **new** image (or `undefined`) and never change `this`.

| Method | Size rule | Method used |
|--------|-----------|-------------|
| `resize(w, h)` | any `w, h > 0` | per axis `scaleUp` or `scaleDown`; same size = exact copy. **Use this one.** |
| `scaleUp(w, h)` | `w >= width`, `h >= height` | doubles with bicubic + smoothing, then bicubic to the exact size |
| `scaleDown(w, h)` | `w, h > 0`, meant for smaller sizes | halves with 2×2 averages, then reaches the exact size |
| `scaleUpBicubic(w, h)` | `w >= width`, `h >= height` | one bicubic pass |
| `scaleUpBilinear(w, h)` | `w >= width`, `h >= height` | one bilinear pass |
| `scaleUpNearestNeighbor(w, h)` | `w >= width`, `h >= height` | nearest pixel, no new colors (pixel art) |
| `scaleDownX2()` | width and height `>= 2` | `width / 2 × height / 2`, 2×2 average; odd last row / column dropped |
| `scaleDownX2OnX()` | width `>= 2` | `width / 2 × height` |
| `scaleDownX2OnY()` | height `>= 2` | `width × height / 2` |
| `cut(x, y, w, h)` | rectangle clipped to the image | the clipped part; `undefined` if nothing is left |
| `wrap(dx, dy)` | — | same size, scrolled: result `(x, y)` = this `((x + dx) mod width, (y + dy) mod height)`; negative offsets allowed |
| `wrapBox(dx, dy)` | `dx, dy >= 0` | `(width + 2·dx) × (height + 2·dy)`: the image with a border filled as if it were tiled |
| `kernel3X3(kernel)` | — | filtered copy, see [Kernel3X3](#pixel32kernel3x3) |

- Bicubic, bilinear and nearest neighbor map **corner to corner**: pixel
  `(0, 0)` stays at `(0, 0)` and `(width - 1, height - 1)` goes to
  `(w - 1, h - 1)`.
- Colors are mixed weighted by alpha; a result pixel with alpha `0` gets
  color `0`.
- `cut(0, 0, image.getWidth(), image.getHeight())` is the way to duplicate
  an image.

```javascript
var thumb = photo.resize(128, 128 * photo.getHeight() / photo.getWidth());   // sizes are truncated
var sprite = sheet.cut(32 * column, 32 * row, 32, 32);
var big = sprite.scaleUpNearestNeighbor(128, 128);
```

### In place: copy, blend, draw

These change `this` and return `undefined`. All rectangles are
`x, y, width, height`, clipped on both images; positions can be negative or
past the edge.

| Method | Effect |
|--------|--------|
| `copy(img, dx, dy, sx, sy, w, h)` | the `w × h` area of `img` at `(sx, sy)` **replaces** the area of `this` at `(dx, dy)`, alpha included; safe when `img` is `this` and the areas overlap |
| `blend(img, dx, dy, sx, sy, w, h)` | same area, drawn **over** `this` with alpha (Porter-Duff "over"); not safe for overlapping areas of the same image |
| `drawRectangle(x, y, w, h, pixel)` | 1 pixel outline of the rectangle, blended over |
| `drawFilledRectangle(x, y, w, h, pixel)` | filled rectangle, blended over (a transparent `pixel` does nothing; to erase use `copy` from a transparent image) |

`img` must be an `Image`; anything else is ignored.

```javascript
// watermark in the bottom right corner, 8 pixels from the edges
photo.blend(logo, photo.getWidth() - logo.getWidth() - 8, photo.getHeight() - logo.getHeight() - 8,
            0, 0, logo.getWidth(), logo.getHeight());

// sprite sheet: 4 frames side by side
var sheet = new Pixel32.Image(4 * 32, 32);
for (var k = 0; k < 4; ++k) {
	sheet.copy(frame[k], 32 * k, 0, 0, 0, 32, 32);
};

// 50% black shadow under a box
image.drawFilledRectangle(12, 12, 100, 40, new Pixel32.Pixel(0, 0, 0, 128));
image.drawFilledRectangle(8, 8, 100, 40, new Pixel32.Pixel(255, 255, 255, 255));
```

### Color and noise

| Method | Returns | Effect |
|--------|---------|--------|
| `average(img, level1, level2, delta)` | `this` / `undefined` | every channel, alpha included, of the area common to both images: `this = (this·level1 + img·level2) / delta`, clamped to `255`. `delta` `0` changes nothing. `undefined` when `img` is not an image or a weight is invalid |
| `colorRescale()` | `undefined` | stretches **each channel separately** (R, G, B and A) so its minimum becomes `0` and its maximum `255`; a constant channel is left alone (so an opaque image stays opaque) |
| `noise(rnd)` | `undefined` | every pixel a random gray, opaque; one `rnd.next()` per pixel |
| `noise2Bit(rnd)` | `undefined` | every pixel black or white, opaque |

`rnd` must be a `Random` object (the `Random` extension, loaded by
`Pixel32`); anything else is silently ignored. The same seed gives the same
image:

```javascript
var rnd = new Random();
rnd.seed(42);
var texture = new Pixel32.Image(64, 64);
texture.noise(rnd);
```

Mixing two images 75% / 25%: `a.average(b, 3, 1, 4)`.

## Pixel32.Kernel3X3

A 3×3 convolution matrix `v` with two scale factors `normalA` and `normalB`
(the result is multiplied by `normalA / normalB`).

```javascript
var kernel = new Pixel32.Kernel3X3();   // identity: center 1, normalA = normalB = 1
```

| Method | Returns | Notes |
|--------|---------|-------|
| `getMatrixV(x, y)` | Number / `undefined` | `x` = column, `y` = row, each `0..2`; `(1, 1)` is the center, row `0` is above the pixel |
| `setMatrixV(x, y, value)` | the kernel / `undefined` | `undefined` (and no change) when `x` or `y` is out of range |
| `getNormalA()`, `getNormalB()` | Number | |
| `setNormalA(value)`, `setNormalB(value)` | the kernel | do not set `normalB` to `0` |

`image.kernel3X3(kernel)` returns the filtered image, same size; pixels
outside the image repeat the edge. It throws `invalid parameter` when the
argument is not a `Kernel3X3`. How the result is computed depends on the
**sum of the matrix**:

- **Positive sum** (blur, smoothing, sharpen): colors are averaged weighted
  by `v × alpha`, so the color does not depend on `normalA / normalB`; the
  new alpha is `normalA × Σ(v × alpha) / normalB`. Use
  `normalA / normalB = 1 / Σv` to keep opaque pixels opaque.
- **Zero or negative sum** (edges, gradients): each color channel is
  `normalA × Σ(v × channel) / normalB`; the alpha of the center pixel is
  kept. Negative results become `0`.

Predefined kernels (properties of `Pixel32.Kernel3X3`):

| Name | Rows (top to bottom) | `normalA / normalB` | Effect |
|------|----------------------|---------------------|--------|
| `blur` | `1 1 1 / 1 1 1 / 1 1 1` | `1 / 9` | box blur |
| `gaussian` | `1 2 1 / 2 4 2 / 1 2 1` | `1 / 16` | soft blur |
| `filter8X1` | `1 1 1 / 1 8 1 / 1 1 1` | `1 / 16` | light smoothing |
| `sharpen` | `0 -1 0 / -1 5 -1 / 0 -1 0` | `1 / 1` | sharpen |
| `edgeDetection` | `0 -1 0 / -1 4 -1 / 0 -1 0` | `1 / 1` | edges (Laplacian) |
| `edgeDetectionH` | `0 0 0 / -1 2 -1 / 0 0 0` | `1 / 1` | changes along a row |
| `edgeDetectionV` | `0 -1 0 / 0 2 0 / 0 -1 0` | `1 / 1` | changes along a column |
| `gradientDetectionH` | `-1 -1 -1 / 0 0 0 / 1 1 1` | `1 / 1` | gradient top → bottom |
| `gradientDetectionV` | `-1 0 1 / -1 0 1 / -1 0 1` | `1 / 1` | gradient left → right |

Each predefined kernel is a copy made when the extension is loaded: changing
it with `setMatrixV` changes it for the rest of the script (in this thread),
not for other scripts or the C++ library. Build your own instead:

```javascript
// emboss, sum = 1
var emboss = new Pixel32.Kernel3X3();
var rows = [[-2, -1, 0], [-1, 1, 1], [0, 1, 2]];
for (var y = 0; y < 3; ++y) {
	for (var x = 0; x < 3; ++x) {
		emboss.setMatrixV(x, y, rows[y][x]);
	};
};
var embossed = image.kernel3X3(emboss);

// stronger blur: apply several times
var soft = image;
for (var k = 0; k < 4; ++k) {
	soft = soft.kernel3X3(Pixel32.Kernel3X3.gaussian);
};
```

## Perlin noise

```javascript
var rnd = new Random();
rnd.seed(1);
var texture = Pixel32.perlinNoiseWrapBox(256, 256, [
	[1, 1, 0],
	[2, 1, 1],
	[4, 2, 1],
	[8, 4, 1],
	[16, 8, 1],
	[32, 16, 1],
	[64, 32, 1]
], rnd);
texture.pngSave("clouds.png");
```

`Pixel32.perlinNoiseWrapBox(lx, ly, octaves, rnd)` returns an `lx × ly`
gray, opaque image that **tiles** (its right edge continues on its left edge,
its bottom on its top): clouds, marble, terrain height maps.
`Pixel32.perlinNoise2BitWrapBox` is the same built from black / white noise
(more contrast). Both are written in script (`Library.js`).

`octaves` is an array of `[scale, weight1, weight2]`:

1. For every entry, a noise image of `lx / scale × ly / scale` pixels is made
   (`noise` / `noise2Bit`, then `colorRescale`) and enlarged to `lx × ly`
   with `wrapBox1Resize`, which keeps it tileable. An entry whose size is
   below 1 pixel is skipped.
2. The images are merged starting from the **last** entry (the largest
   scale, the coarse shapes). Step `k = 1, 2, ...` merges the image of entry
   `m - 1 - k` (`m` = number of entries) using the **weights of entry `k`**:
   `result = (result × weight1 + image × weight2) / (weight1 + weight2)`. The
   weights of entry 0 are not used. Skipped entries leave out their step.

With the table above each finer octave gets a smaller share (`1/2`, `1/3`,
`1/5`, ...): the classic "fractal" noise. Returns `undefined` when every
entry is skipped (`lx` or `ly` smaller than every scale). The image is
exactly tileable when `lx` and `ly` are multiples of the largest scale.

`image.wrapBox1Resize(lx, ly, fz, k)` is the helper: it enlarges a tile of
`lx / fz × ly / fz` pixels to `lx × ly`, adding a wrapped 1 pixel border
before resizing so the edges stay continuous. `k` is not used.

## Errors

| Situation | Result |
|-----------|--------|
| a method called on another kind of object (`o.f = Pixel32.Image.prototype.getWidth; o.f()`) | throws `invalid parameter` |
| `kernel3X3(x)` with `x` not a `Kernel3X3` | throws `invalid parameter` |
| bad size, empty clip, scale in the wrong direction, `pngLoad` failure | `undefined` |
| `pngSave` failure (folder missing, no rights) | `false` |
| `setPixel` / `clear` / `draw*` with a non-`Pixel` color, `copy` / `blend` / `average` with a non-image, `noise` without a `Random` | nothing happens |

## Recipes

Icon sizes from one PNG:

```javascript
Script.requireExtension("Pixel32");

var source = Pixel32.pngLoad("icon-512.png");
if (Script.isUndefined(source)) {
	throw new Error("icon-512.png not found");
};
var sizes = [16, 24, 32, 48, 64, 128, 256];
for (var k = 0; k < sizes.length; ++k) {
	if (!source.resize(sizes[k], sizes[k]).pngSave("icon-" + sizes[k] + ".png")) {
		throw new Error("cannot write icon-" + sizes[k] + ".png");
	};
};
```

Square canvas with padding (keeps the aspect ratio):

```javascript
function fitSquare(image, size) {
	var w = image.getWidth();
	var h = image.getHeight();
	var nw = size;
	var nh = size;
	if (w > h) {
		nh = size * h / w;
	} else {
		nw = size * w / h;
	};
	if (nw < 1) {
		nw = 1;
	};
	if (nh < 1) {
		nh = 1;
	};
	var scaled = image.resize(nw, nh);              // sizes are truncated
	var canvas = new Pixel32.Image(size, size);     // transparent
	nw = scaled.getWidth();
	nh = scaled.getHeight();
	canvas.copy(scaled, (size - nw) / 2, (size - nh) / 2, 0, 0, nw, nh);
	return canvas;
};
```

Sizes and coordinates are truncated by the extension, so `size * h / w` and
`(size - nw) / 2` need no rounding.

Recolor a monochrome glyph (keep its alpha, change its color):

```javascript
var x, y, p;
for (y = 0; y < glyph.getHeight(); ++y) {
	for (x = 0; x < glyph.getWidth(); ++x) {
		p = glyph.getPixel(x, y);
		glyph.setPixel(x, y, p.setR(0x20).setG(0x60).setB(0xC0));
	};
};
```

Checkerboard background behind a transparent image:

```javascript
function checkerboard(w, h, cell) {
	var board = new Pixel32.Image(w, h);
	var light = new Pixel32.Pixel("FFFFFF");
	var dark = new Pixel32.Pixel("CCCCCC");
	board.clear(light);
	for (var y = 0; y < h; y += cell) {
		for (var x = ((y / cell) % 2) * cell; x < w; x += 2 * cell) {
			board.drawFilledRectangle(x, y, cell, cell, dark);
		};
	};
	return board;
};
var preview = checkerboard(image.getWidth(), image.getHeight(), 8);
preview.blend(image, 0, 0, 0, 0, image.getWidth(), image.getHeight());
```

Seamless filter on a tiling texture (filter with a wrapped border, then cut
the border away, so the edges are filtered with their tiled neighbors):

```javascript
var box = texture.wrapBox(1, 1);
var soft = box.kernel3X3(Pixel32.Kernel3X3.gaussian).cut(1, 1, texture.getWidth(), texture.getHeight());
```
