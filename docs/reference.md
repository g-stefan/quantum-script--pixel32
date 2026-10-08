# API reference

## Script

Load with `Script.requireExtension("Pixel32")` (also loads `Random`).
Number arguments: coordinates are truncated and may be negative; sizes must
not be negative; channels are clamped to `0..255`; `NaN` / `Infinity` make
the call do nothing (see [Script API — Numbers](script-api.md#numbers)).

### Pixel32.Pixel

| Member | Returns | Notes |
|--------|---------|-------|
| `Pixel32.Pixel()` | Pixel | `00000000` |
| `Pixel32.Pixel(r, g, b, a)` | Pixel | missing / `NaN` channel = `0` (alpha too) |
| `Pixel32.Pixel("RRGGBBAA")`, `Pixel32.Pixel("RRGGBB")` | Pixel | hex; 6 digits = opaque; other text = `00000000` |
| `Pixel32.Pixel(pixel)` | Pixel | copy |
| `getR()`, `getG()`, `getB()`, `getA()` | Number | `0..255` |
| `setR(v)`, `setG(v)`, `setB(v)`, `setA(v)` | this | clamped; `NaN` ignored |
| `toString()` | String | `"RRGGBBAA"` |

### Pixel32.Image

| Member | Returns | Changes `this` | Notes |
|--------|---------|----------------|-------|
| `Pixel32.Image(w, h)` | Image / `undefined` | | transparent black; `undefined` for a bad size (with `new`: an empty object) |
| `Pixel32.pngLoad(fileName)` | Image / `undefined` | | any PNG → RGBA |
| `pngSave(fileName)` | Boolean | | RGBA PNG |
| `getWidth()`, `getHeight()` | Number | | |
| `getPixel(x, y)` | Pixel | | `00000000` outside |
| `getPixelX(x, y)` | Pixel | | clamped to the nearest edge pixel |
| `setPixel(x, y, pixel)` | `undefined` | yes | replaces; ignored outside |
| `clear(pixel)` | `undefined` | yes | replaces every pixel |
| `resize(w, h)` | Image / `undefined` | | any size `> 0` |
| `scaleUp(w, h)` | Image / `undefined` | | `w >= width`, `h >= height` |
| `scaleDown(w, h)` | Image / `undefined` | | smaller sizes |
| `scaleUpBicubic(w, h)` | Image / `undefined` | | `w >= width`, `h >= height` |
| `scaleUpBilinear(w, h)` | Image / `undefined` | | `w >= width`, `h >= height` |
| `scaleUpNearestNeighbor(w, h)` | Image / `undefined` | | `w >= width`, `h >= height` |
| `scaleDownX2()` | Image / `undefined` | | half size, needs `>= 2` |
| `scaleDownX2OnX()` | Image / `undefined` | | half width |
| `scaleDownX2OnY()` | Image / `undefined` | | half height |
| `cut(x, y, w, h)` | Image / `undefined` | | clipped |
| `wrap(dx, dy)` | Image | | scroll: result `(x, y)` = this `((x + dx) mod w, (y + dy) mod h)` |
| `wrapBox(dx, dy)` | Image / `undefined` | | adds a tiled border of `dx`, `dy` (not negative) |
| `wrapBox1Resize(lx, ly, fz, k)` | Image | | tileable enlarge of an `lx / fz × ly / fz` tile (Perlin helper; `k` unused) |
| `copy(img, dx, dy, sx, sy, w, h)` | `undefined` | yes | replace, clipped, overlap safe |
| `blend(img, dx, dy, sx, sy, w, h)` | `undefined` | yes | alpha over, clipped |
| `drawRectangle(x, y, w, h, pixel)` | `undefined` | yes | 1 pixel outline, blended, clipped |
| `drawFilledRectangle(x, y, w, h, pixel)` | `undefined` | yes | blended, clipped |
| `average(img, level1, level2, delta)` | this / `undefined` | yes | `(this·level1 + img·level2) / delta` per channel |
| `colorRescale()` | `undefined` | yes | stretch each channel to `0..255` |
| `noise(rnd)` | `undefined` | yes | random gray, opaque; `rnd` is a `Random` |
| `noise2Bit(rnd)` | `undefined` | yes | random black / white, opaque |
| `kernel3X3(kernel)` | Image | | 3×3 filter; throws if `kernel` is not a `Kernel3X3` |

### Pixel32.Kernel3X3

| Member | Returns | Notes |
|--------|---------|-------|
| `Pixel32.Kernel3X3()` | Kernel3X3 | identity, `normalA = normalB = 1` |
| `getMatrixV(x, y)` | Number / `undefined` | `x` column, `y` row, `0..2` |
| `setMatrixV(x, y, value)` | this / `undefined` | |
| `getNormalA()`, `getNormalB()` | Number | |
| `setNormalA(value)`, `setNormalB(value)` | this | result × `normalA / normalB` |
| `Pixel32.Kernel3X3.blur` | Kernel3X3 | all `1`, `1 / 9` |
| `Pixel32.Kernel3X3.gaussian` | Kernel3X3 | `1 2 1 / 2 4 2 / 1 2 1`, `1 / 16` |
| `Pixel32.Kernel3X3.filter8X1` | Kernel3X3 | `1 1 1 / 1 8 1 / 1 1 1`, `1 / 16` |
| `Pixel32.Kernel3X3.sharpen` | Kernel3X3 | `0 -1 0 / -1 5 -1 / 0 -1 0` |
| `Pixel32.Kernel3X3.edgeDetection` | Kernel3X3 | `0 -1 0 / -1 4 -1 / 0 -1 0` |
| `Pixel32.Kernel3X3.edgeDetectionH` | Kernel3X3 | `0 0 0 / -1 2 -1 / 0 0 0` |
| `Pixel32.Kernel3X3.edgeDetectionV` | Kernel3X3 | `0 -1 0 / 0 2 0 / 0 -1 0` |
| `Pixel32.Kernel3X3.gradientDetectionH` | Kernel3X3 | `-1 -1 -1 / 0 0 0 / 1 1 1` |
| `Pixel32.Kernel3X3.gradientDetectionV` | Kernel3X3 | `-1 0 1 / -1 0 1 / -1 0 1` |

### Functions

| Function | Returns | Notes |
|----------|---------|-------|
| `Pixel32.pngLoad(fileName)` | Image / `undefined` | |
| `Pixel32.perlinNoiseWrapBox(lx, ly, octaves, rnd)` | Image / `undefined` | `octaves` = `[[scale, weight1, weight2], ...]`; gray, opaque, tileable |
| `Pixel32.perlinNoise2BitWrapBox(lx, ly, octaves, rnd)` | Image / `undefined` | same, from black / white noise |

### Types

| Value | `typeof(x)` | `"" + x` |
|-------|-------------|----------|
| Pixel | `"Pixel32.Pixel"` | `"RRGGBBAA"` |
| Image | `"Pixel32.Image"` | `"Pixel32.Image"` |
| Kernel3X3 | `"Pixel32.Kernel3X3"` | `"Pixel32.Kernel3X3"` |

## C++

Namespace `XYO::QuantumScript::Extension::Pixel32`, header
`<XYO/QuantumScript.Extension/Pixel32.hpp>`.

| Symbol | Header | Notes |
|--------|--------|-------|
| `void registerInternalExtension(Executive *)` | `Pixel32/Library.hpp` | registers `"Pixel32"` as internal |
| `void initExecutive(Executive *, void *extensionId)` | `Pixel32/Library.hpp` | extension init |
| `extern "C" void quantumScriptExtension(Executive *, void *)` | `Pixel32/Library.cpp` | DLL entry point (dynamic builds only) |
| `Pixel32Context`, `getContext()` | `Pixel32/Context.hpp` | per-thread symbols and prototypes |
| `VariablePixel` | `Pixel32/VariablePixel.hpp` | `Pixel pixel`; `newVariable(Pixel)`; type `"Pixel32.Pixel"` |
| `VariableImage` | `Pixel32/VariableImage.hpp` | `TPointer<Image> image`; `newVariable(Image *)`; type `"Pixel32.Image"`; `clone` deep copies |
| `VariableKernel3X3` | `Pixel32/VariableKernel3X3.hpp` | `TPointer<Kernel3X3> kernel`; `newVariable(Kernel3X3 *)`; type `"Pixel32.Kernel3X3"` |
| `Copyright`, `License`, `Version` | `Pixel32/Copyright.hpp`, ... | extension metadata (`Version::versionWithBuild()`) |
| `XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT` | `Pixel32/Dependency.hpp` | export / import macro; empty with `XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_LIBRARY` |

## fabricare

| Item | Value |
|------|-------|
| Project | `quantum-script--pixel32`, `make: dll-or-lib` |
| Dependencies | `quantum-script`, `quantum-script--console`, `quantum-script--random`, `xyo-pixel32` |
| Library file | `quantum-script--pixel32.dll` / `libquantum-script--pixel32.so` |
| Test | `test.0003` (C++ host + `test/test.0003.js`), then `test/test.0001.js`, `test/test.0002.js` |
