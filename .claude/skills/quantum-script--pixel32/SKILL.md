---
name: quantum-script--pixel32
description: >-
  How to use the Quantum Script Pixel32 extension (quantum-script--pixel32),
  32-bit RGBA images for scripts loaded with
  Script.requireExtension("Pixel32") (also loads Random), a binding of the
  xyo-pixel32 C++ library: Pixel32.Pixel(r, g, b, a) / Pixel("RRGGBBAA") /
  Pixel("RRGGBB") with getR..getA, setR..setA (clamped, chainable),
  toString "RRGGBBAA"; Pixel32.Image(w, h) (top-left origin, transparent
  black) with getWidth / getHeight, getPixel / getPixelX / setPixel, clear,
  pngSave, Pixel32.pngLoad, resize, scaleUp / scaleDown, scaleUpBicubic /
  Bilinear / NearestNeighbor, scaleDownX2 / X2OnX / X2OnY, cut, wrap,
  wrapBox, copy, blend, drawRectangle / drawFilledRectangle, average,
  colorRescale, noise / noise2Bit (Random), kernel3X3; Pixel32.Kernel3X3
  (getMatrixV / setMatrixV with x = column, y = row, normalA / normalB,
  predefined blur, gaussian, filter8X1, sharpen, edgeDetection[H/V],
  gradientDetection[H/V]); Pixel32.perlinNoiseWrapBox /
  perlinNoise2BitWrapBox tileable textures and wrapBox1Resize. Covers new
  image vs in place, undefined on failure (and the new-with-undefined
  trap), clipping with negative positions, alpha rules, pixel reference
  semantics, == comparing identity, threads (clone copies images), the C++
  side (registerInternalExtension, VariablePixel / VariableImage /
  VariableKernel3X3) and the repository (Library.js -> Library.Source.cpp,
  test.0003 host). Use when writing or reviewing Quantum Script code or
  fabricare build scripts that process images with Pixel32, C++ code that
  includes <XYO/QuantumScript.Extension/Pixel32.hpp>, a fabricare.json
  depending on "quantum-script--pixel32", or when working inside the
  quantum-script--pixel32 repository.
---

# quantum-script--pixel32

`Pixel32` extension of Quantum Script (see the `quantum-script` skill for the
language and its differences from JavaScript, and the `xyo-pixel32` skill for
the C++ library underneath; their rules apply). Purpose: **let scripts load,
resize, compose, filter and save RGBA images** — icon sizes, glyph cleanup,
textures, test pictures — with the exact pixel model of `xyo-pixel32`.

Full documentation: `docs/` in the quantum-script--pixel32 repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--pixel32\docs` on this machine):
README (purpose, concepts, source map), getting-started (build, load, hosts,
C++ registration, tests, threads), **script-api** (every method, number
rules, value model, kernels, Perlin noise, recipes), cpp-api (Variable
types, exchanging images with C++, maintainers), reference. When in doubt
read `source/XYO/QuantumScript.Extension/Pixel32/Library.cpp` (every native
method) and `Library.js` (Perlin noise); `test/test.0003.js` exercises the
whole API.

## Pick a tool

| Need | Use |
|------|-----|
| Load | `Script.requireExtension("Pixel32");` (Random comes with it) |
| Color | `new Pixel32.Pixel(r, g, b, 255)`, `new Pixel32.Pixel("FF8000")` (opaque), `"RRGGBBAA"` |
| New image | `var img = Pixel32.Image(w, h); if (Script.isUndefined(img)) ...` |
| Load / save PNG | `Pixel32.pngLoad(file)` (`undefined` on error) / `img.pngSave(file)` (`true` / `false`) |
| Any resize | `img.resize(w, h)` (new image) |
| Pixel art upscale | `img.scaleUpNearestNeighbor(w, h)` |
| Crop / duplicate | `img.cut(x, y, w, h)` / `img.cut(0, 0, img.getWidth(), img.getHeight())` |
| Paste replacing / with alpha | `dst.copy(src, dx, dy, sx, sy, w, h)` / `dst.blend(...)` |
| Fill / rectangles | `img.clear(p)` (replace) / `drawFilledRectangle`, `drawRectangle` (blended) |
| Blur / sharpen / edges | `img.kernel3X3(Pixel32.Kernel3X3.gaussian)` (new image) |
| Mix two images | `a.average(b, 3, 1, 4)` (75 / 25, in place) |
| Auto levels | `img.colorRescale()` |
| Texture | `rnd = new Random(); rnd.seed(n); Pixel32.perlinNoiseWrapBox(w, h, octaves, rnd)` |
| Per pixel | `p = img.getPixel(x, y); ...; img.setPixel(x, y, p)` (slow: prefer methods) |

## Hard rules

1. **Load it first**, in every script and every thread:
   `Script.requireExtension("Pixel32")`. Not preloaded by `quantum-script`
   or fabricare; magnet registers it internally.
2. **New image vs in place.** `resize`, `scale*`, `cut`, `wrap`, `wrapBox`,
   `kernel3X3`, `wrapBox1Resize` return a **new** image and never change
   `this` — assign the result. `clear`, `setPixel`, `copy`, `blend`,
   `draw*`, `average`, `colorRescale`, `noise*` change `this` and return
   `undefined` (`average` returns `this`).
3. **Failures return `undefined`** (`pngSave`: `false`), they do not throw:
   check results from files and computed sizes. Only a method on a wrong
   `this`, or `kernel3X3(nonKernel)`, throws `invalid parameter`. Wrong
   argument types (a string instead of a Pixel, a non-image to `copy`, no
   `Random` to `noise`) are **silently ignored**.
4. **`new Pixel32.Image(0, 5)` is not `undefined`**: `new` turns the
   `undefined` result into an empty object without methods. When a size may
   be bad, call `Pixel32.Image(w, h)` without `new` and test
   `Script.isUndefined`.
5. **Alpha is required**: `Pixel(255, 0, 0)` has alpha `0` (transparent).
   Write `Pixel(255, 0, 0, 255)` or `Pixel("FF0000")`. Channels are
   truncated and clamped to `0..255`; `NaN` → `0` (setters ignore `NaN`).
   Any **string** first argument is parsed as hex (`"RRGGBBAA"` /
   `"RRGGBB"`, else `00000000`).
6. **Coordinates**: `(0, 0)` top left, `y` down. Numbers are truncated
   toward zero (no rounding needed for `w / 2`). Rectangles are
   `x, y, width, height` and clipped on both images; negative positions are
   fine. `getPixel` outside → `"00000000"`, `getPixelX` clamps to the edge,
   `setPixel` outside is ignored.
7. **Pixels are objects**: `q = p; q.setR(0)` changes `p`; copy with
   `new Pixel32.Pixel(p)`. `getPixel` returns a fresh copy — call
   `setPixel` to write back. `==` / `===` compare identity: compare colors
   with `"" + a == "" + b`.
8. **Scaling rules**: `resize` takes any size `> 0` (prefer it);
   `scaleUp*` need `w >= width && h >= height`; `scaleDownX2*` need `>= 2`
   and drop an odd last row / column. Bicubic / bilinear / nearest map corner
   to corner. Mixing is alpha weighted; alpha `0` results get color `0`.
9. **Compositing**: `copy` / `clear` / `setPixel` replace (alpha included),
   `copy` is overlap safe; `blend` / `draw*` are alpha "over" (a
   transparent color draws nothing), not safe for overlapping areas of the
   same image.
10. **Kernel3X3**: `getMatrixV(x, y)` / `setMatrixV(x, y, v)` — `x` column,
    `y` row, `0..2`, `(1, 1)` center, row 0 above. Positive matrix sum →
    alpha weighted average, alpha × `normalA / normalB` (use `1 / Σv`);
    zero / negative sum → per channel `normalA·Σ(v·c)/normalB`, center alpha
    kept, negatives → 0. The predefined `Pixel32.Kernel3X3.*` are per-script
    copies; build your own instead of modifying them. Never `setNormalB(0)`.
11. **Noise** needs a `Random` (`rnd.seed(n)` for reproducible output; one
    `next()` per pixel). Output is opaque gray (`noise`) or black / white
    (`noise2Bit`).
12. **Perlin**: `octaves = [[scale, w1, w2], ...]`; each entry makes a
    `lx/scale × ly/scale` noise image, enlarged tileably; merged from the
    **last** entry, step `k` merges entry `m-1-k` with the weights of entry
    `k`: `(acc·w1 + img·w2) / (w1 + w2)`. Entries below 1 pixel are skipped;
    all skipped → `undefined`. Use sizes divisible by the largest scale. The
    standard table: `[1,1,0],[2,1,1],[4,2,1],[8,4,1],[16,8,1],[32,16,1],[64,32,1]`.
13. **Threads**: values passed to another thread are copied (`clone`:
    images pixel by pixel); load `Pixel32` inside the thread before using
    them. An image object has no lock.
14. Large per-pixel loops are slow (two native calls per pixel): use
    `clear`, `drawFilledRectangle`, `copy`, `blend`, `kernel3X3`, `average`
    where they fit.

## Recipes

```javascript
Script.requireExtension("Pixel32");

// icon sizes
var src = Pixel32.pngLoad("icon-512.png");
if (Script.isUndefined(src)) {
	throw new Error("icon-512.png not found");
};
var sizes = [16, 32, 48, 256];
for (var k = 0; k < sizes.length; ++k) {
	if (!src.resize(sizes[k], sizes[k]).pngSave("icon-" + sizes[k] + ".png")) {
		throw new Error("cannot write icon-" + sizes[k] + ".png");
	};
};

// watermark, bottom right, 8 px margin
photo.blend(logo, photo.getWidth() - logo.getWidth() - 8, photo.getHeight() - logo.getHeight() - 8,
            0, 0, logo.getWidth(), logo.getHeight());

// center on a transparent square canvas
var scaled = img.resize(64, 64 * img.getHeight() / img.getWidth());   // sizes truncated
var canvas = new Pixel32.Image(64, 64);
canvas.copy(scaled, (64 - scaled.getWidth()) / 2, (64 - scaled.getHeight()) / 2,
            0, 0, scaled.getWidth(), scaled.getHeight());

// recolor a glyph, keep alpha
for (var y = 0; y < glyph.getHeight(); ++y) {
	for (var x = 0; x < glyph.getWidth(); ++x) {
		glyph.setPixel(x, y, glyph.getPixel(x, y).setR(0x20).setG(0x60).setB(0xC0));
	};
};

// seamless filter of a tiling texture
var soft = tex.wrapBox(1, 1).kernel3X3(Pixel32.Kernel3X3.gaussian).cut(1, 1, tex.getWidth(), tex.getHeight());

// custom kernel (emboss)
var emboss = new Pixel32.Kernel3X3();
var rows = [[-2, -1, 0], [-1, 1, 1], [0, 1, 2]];
for (var r = 0; r < 3; ++r) {
	for (var c = 0; c < 3; ++c) {
		emboss.setMatrixV(c, r, rows[r][c]);
	};
};
var embossed = img.kernel3X3(emboss);
```

## C++

```cpp
#include <XYO/QuantumScript.Extension/Random.hpp>
#include <XYO/QuantumScript.Extension/Pixel32.hpp>
using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {                       // host init callback
	Extension::Random::registerInternalExtension(executive);        // Library.js requires Random
	Extension::Pixel32::registerInternalExtension(executive);       // scripts still requireExtension("Pixel32")
};
```

- fabricare.json dependency `"quantum-script--pixel32"` (`dll-or-lib`;
  pulls in `quantum-script`, `quantum-script--console`,
  `quantum-script--random`, `xyo-pixel32`). No `.static` project; static
  hosts register it as internal.
- Values: `VariablePixel` (`Pixel pixel`), `VariableImage`
  (`TPointer<Image> image`, `newVariable(Image *)` shares, does not copy),
  `VariableKernel3X3` (`TPointer<Kernel3X3> kernel`; script `x, y` =
  `v[y][x]`). Test with `TIsType<Extension::Pixel32::VariableImage>(v)`.
  All three implement `clone` (image: deep copy).
- Inside `namespace XYO::QuantumScript::Extension::Pixel32` write
  `XYO::Pixel32::Process::...` in full (the code uses the alias
  `Pixel32Process`).

## Working in this repository

- Build / test: `fabricare make`, then `fabricare test` (see the `fabricare`
  skill; on Windows clear `NoDefaultCurrentDirectoryInExePath` first).
  `test` builds `test.0003` (C++ host in `output/test` registering Console,
  Random, Thread, Pixel32, runs `test/test.0003.js` against `output/bin`),
  then runs `test/test.0001.js` / `test.0002.js` with the **installed**
  interpreter and DLL (Perlin textures in `test/output.000N.png`;
  `test/output.0001.html` shows the tiling).
- Natives in `Pixel32/Library.cpp`: check `TIsType<VariableImage>(this_)`
  (throw `Error("invalid parameter")`), convert numbers with
  `toCoordinate` / `toSize` / `toChannel` / `toWeight` (never reject
  negative coordinates: `xyo-pixel32` clips), return `undefined` for a
  `nullptr` image; register in `initExecutive` with
  `executive->setFunction2("Pixel32.Image.prototype.name(a,b)", name)`.
- Script part in `Library.js`; `make` regenerates `Library.Source.cpp`
  (never edit it). `wrapBox1Resize` cuts `lx + fz` then resizes on purpose
  (corner-to-corner resize; keeps tiling — guarded by the "perlin tiles"
  check).
- New or changed methods: update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, `test/test.0003.js` (`check(name, value, expected)`)
  and this skill.
- Code style: tabs, `.clang-format`, CRLF (edit with the Edit tool or
  binary-mode Python, not `sed -i`), statements and blocks end with `};`.
  SPDX: MIT for `source/` and `docs/`, Unlicense for `test/`, `fabricare/`
  and `.claude/` (`.reuse/dep5`; check with `python -m reuse lint`).
