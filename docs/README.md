# Quantum Script Extension Pixel32 — Documentation

`quantum-script--pixel32` is the **image extension of Quantum Script**.
Loading it with `Script.requireExtension("Pixel32")` gives scripts the global
object `Pixel32`: 32-bit RGBA images in memory, PNG load / save, resizing,
cropping, pasting with or without alpha, rectangles, 3×3 filters, noise and
tileable Perlin noise textures.

```javascript
Script.requireExtension("Pixel32");

var image = Pixel32.pngLoad("logo.png");          // undefined if it fails
if (Script.isUndefined(image)) {
	throw new Error("cannot load logo.png");
};

var icon = image.resize(48, 48);                  // new image, any size
icon.drawRectangle(0, 0, 48, 48, new Pixel32.Pixel(0, 0, 0, 255));
icon.kernel3X3(Pixel32.Kernel3X3.sharpen).pngSave("logo-48.png");
```

It is a thin script layer over the C++ library `xyo-pixel32`
(`XYO::Pixel32`): every image method calls one `XYO::Pixel32::Process`
function, so the scripts get the same pixel format, alpha handling and
clipping rules.

- **Three value types.** `Pixel32.Pixel` (one RGBA color, printed as
  `"RRGGBBAA"`), `Pixel32.Image` (width × height pixels, `(0, 0)` is the top
  left corner) and `Pixel32.Kernel3X3` (a 3×3 convolution matrix).
- **New image or in place.** Methods that change the size or filter
  (`resize`, `scale*`, `cut`, `wrap`, `wrapBox`, `kernel3X3`) return a **new**
  image and leave the original alone; the drawing methods (`clear`,
  `setPixel`, `copy`, `blend`, `draw*`, `average`, `colorRescale`, `noise*`)
  change the image they are called on.
- **Failures are `undefined`, not exceptions.** A bad size, a missing file
  or an empty clip gives `undefined` (or `false` for `pngSave`). Only a method
  called on the wrong kind of object, or `kernel3X3` with something that is
  not a kernel, throws.
- **Alpha done right.** Colors are *straight* (not premultiplied) RGBA;
  scaling, filters and blending weight colors by alpha, so transparent areas
  never bleed dark fringes.
- **Clipping everywhere.** Rectangles for `cut`, `copy`, `blend` and the
  `draw*` methods can be partly or fully outside the images, also at negative
  positions.

```
scripts: quantum-script .js, fabricare build scripts, magnet, icon font builders, ...
quantum-script--pixel32   <-- this extension: Pixel32.Pixel / Image / Kernel3X3, pngLoad, Perlin noise
quantum-script--random    (Random: the generator used by noise / Perlin noise)
quantum-script            (Executive, Variable, Context)
xyo-pixel32               (Image, Process::*, Kernel3X3, libpng + zlib)
xyo-system, xyo-cryptography, xyo-encoding, xyo-data-structures, xyo-managed-memory, xyo-platform
```

## Why it exists

The Quantum Script core has no image type. The XYO build tools need to
process images from scripts: make icon and favicon sizes from a PNG, clean
up icon font glyphs before conversion, generate textures and test pictures.
`Pixel32` gives scripts exactly the image model of `xyo-pixel32`:

| Need | How `Pixel32` does it |
|------|-----------------------|
| Load any PNG (palette, gray, 16-bit, interlaced, transparency) | `Pixel32.pngLoad(file)` → 8-bit RGBA image |
| Make sizes for icons | `image.resize(w, h)`: best method per axis, any size |
| Read / change single pixels | `image.getPixel(x, y)` → `Pixel`, `image.setPixel(x, y, pixel)` |
| Compose: watermark, sprite sheet, padding | `copy` (replace) and `blend` (alpha over), clipped |
| Blur, sharpen, edge detection | `image.kernel3X3(Pixel32.Kernel3X3.gaussian)` or a custom kernel |
| Reproducible textures | `noise(rnd)` with a seeded `Random`, `Pixel32.perlinNoiseWrapBox` (tileable) |
| Save the result | `image.pngSave(file)` → `true` / `false` |

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the extension | `Script.requireExtension("Pixel32");` | also loads `Random` |
| A color | `new Pixel32.Pixel(r, g, b, a)` or `new Pixel32.Pixel("FF8000FF")` | channels `0..255`; **alpha missing = 0** (transparent) |
| Print a color | `"" + pixel` | `"RRGGBBAA"` |
| New image | `new Pixel32.Image(w, h)` | transparent black; `Pixel32.Image(w, h)` (no `new`) is `undefined` for a bad size |
| Load / save PNG | `Pixel32.pngLoad(file)` / `image.pngSave(file)` | `undefined` / `false` on error |
| Size | `image.getWidth()`, `image.getHeight()` | |
| Read / write a pixel | `image.getPixel(x, y)` / `image.setPixel(x, y, pixel)` | outside: `"00000000"` / ignored |
| Resize | `image.resize(w, h)` | new image |
| Crop | `image.cut(x, y, w, h)` | new image, clipped |
| Paste | `dst.copy(src, dx, dy, sx, sy, w, h)` / `dst.blend(...)` | in place, clipped |
| Rectangles | `image.drawRectangle(x, y, w, h, pixel)` / `drawFilledRectangle` | blended |
| Filter | `image.kernel3X3(Pixel32.Kernel3X3.blur)` | new image |
| Texture | `Pixel32.perlinNoiseWrapBox(w, h, octaves, rnd)` | gray, opaque, tileable |

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, load the extension, a first script, hosts that have it, register it in a C++ host, tests, threads |
| [Script API](script-api.md) | `Pixel`, `Image`, `Kernel3X3`, PNG, noise and Perlin noise; value model, clipping, failures; recipes |
| [C++ API](cpp-api.md) | `registerInternalExtension`, `initExecutive`, the DLL entry point, `VariablePixel` / `VariableImage` / `VariableKernel3X3`, notes for maintainers |
| [API reference](reference.md) | Every script and C++ symbol on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`. The image functions are documented in more depth in the
`xyo-pixel32` repository, `docs/` (`images.md`, `processing.md`,
`files.md`), and the generator in the `quantum-script--random` repository.

## Source map

```
source/XYO/QuantumScript.Extension/Pixel32.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/Pixel32.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/Pixel32/
    Dependency.hpp                                        <XYO/QuantumScript.hpp>, <XYO/Pixel32.hpp>, export macro
    Library[.hpp/.cpp]                                    initExecutive, registerInternalExtension, every native method
    Library.js                                            script part: wrapBox1Resize, perlinNoiseWrapBox, perlinNoise2BitWrapBox
    Library.Source.cpp                                    generated from Library.js (fabricare/make.prepare.js), do not edit
    Context.hpp                                           Pixel32Context: symbols and prototypes of Pixel, Image, Kernel3X3
    VariablePixel[.hpp/.cpp]                              script value holding one Pixel
    VariableImage[.hpp/.cpp]                              script value holding a TPointer<Image>
    VariableKernel3X3[.hpp/.cpp]                          script value holding a TPointer<Kernel3X3>
    Atomic.hpp                                            unused, kept for compatibility
    Copyright / License / Version                         extension metadata
    Library.rc, *.rh                                      Windows version resource
fabricare/make.prepare.js                                 Library.js -> Library.Source.cpp
fabricare/test.js                                         fabricare test: test.0003 host, then test.0001 / test.0002
test/test.0001.js, test/test.0002.js                      Perlin noise textures (written to test/output.000N.png)
test/output.0001.html                                     shows test/output.0001.png tiled in a browser
test/test.0003.cpp, test/test.0003.js                     regression tests of the whole API, run against output/bin
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--pixel32/`](../.claude/skills/quantum-script--pixel32/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use
`Pixel32` (Quantum Script tools, fabricare build scripts, magnet scripts).
