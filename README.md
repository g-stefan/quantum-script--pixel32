# Quantum Script Extension Pixel32

Quantum Script extension
- 32-bit RGBA images for Quantum Script: `Pixel32.Pixel` (a color),
`Pixel32.Image` (width x height pixels, top left origin) and
`Pixel32.Kernel3X3` (a 3x3 filter), on top of the `xyo-pixel32` library.
- Load any PNG and save RGBA PNG, resize (bicubic / bilinear / nearest /
box), crop, copy and alpha blend with clipping, rectangles, blur / sharpen /
edge filters, color stretch, noise and tileable Perlin noise textures.
- Used by build scripts to make icon sizes, clean up icon font glyphs and
generate textures.

```javascript
Script.requireExtension("Pixel32");

Pixel32.Pixel(r,g,b,a);
Pixel32.Pixel("RRGGBBAA");
Pixel32.Pixel.prototype.getR();
Pixel32.Pixel.prototype.getG();
Pixel32.Pixel.prototype.getB();
Pixel32.Pixel.prototype.getA();
Pixel32.Pixel.prototype.setR(v);
Pixel32.Pixel.prototype.setG(v);
Pixel32.Pixel.prototype.setB(v);
Pixel32.Pixel.prototype.setA(v);
Pixel32.Image(w,h);
Pixel32.Image.prototype.pngSave(fileName);
Pixel32.Image.prototype.getWidth();
Pixel32.Image.prototype.getHeight();
Pixel32.Image.prototype.getPixel(x,y);
Pixel32.Image.prototype.getPixelX(x,y);
Pixel32.Image.prototype.setPixel(x,y,pixel);
Pixel32.Image.prototype.scaleUpBicubic(x,y);
Pixel32.Image.prototype.scaleUpBilinear(x,y);
Pixel32.Image.prototype.scaleUpNearestNeighbor(x,y);
Pixel32.Image.prototype.scaleDownX2();
Pixel32.Image.prototype.scaleDownX2OnX();
Pixel32.Image.prototype.scaleDownX2OnY();
Pixel32.Image.prototype.scaleDown(x,y);
Pixel32.Image.prototype.scaleUp(x,y);
Pixel32.Image.prototype.resize(x,y);
Pixel32.Image.prototype.cut(sx,sy,lx,ly);
Pixel32.Image.prototype.clear(pixel);
Pixel32.Image.prototype.copy(img,dx,dy,sx,sy,lx,ly);
Pixel32.Image.prototype.wrap(dx,dy);
Pixel32.Image.prototype.wrapBox(dx,dy);
Pixel32.Image.prototype.blend(img,dx,dy,sx,sy,lx,ly);
Pixel32.Image.prototype.noise(random);
Pixel32.Image.prototype.noise2Bit(random);
Pixel32.Image.prototype.drawRectangle(sx,sy,lx,ly,pixel);
Pixel32.Image.prototype.drawFilledRectangle(sx,sy,lx,ly,pixel);
Pixel32.Image.prototype.colorRescale();
Pixel32.Image.prototype.average(img,level1,level2,delta);
Pixel32.Image.prototype.kernel3X3(kernel3X3);
Pixel32.Image.prototype.wrapBox1Resize(lx,ly,fz,k);
Pixel32.Kernel3X3();
Pixel32.Kernel3X3.prototype.getNormalA();
Pixel32.Kernel3X3.prototype.setNormalA(value);
Pixel32.Kernel3X3.prototype.getNormalB();
Pixel32.Kernel3X3.prototype.setNormalB(value);
Pixel32.Kernel3X3.prototype.getMatrixV(x,y);
Pixel32.Kernel3X3.prototype.setMatrixV(x,y,value);
Pixel32.Kernel3X3.filter8X1;
Pixel32.Kernel3X3.gaussian;
Pixel32.Kernel3X3.blur;
Pixel32.Kernel3X3.sharpen;
Pixel32.Kernel3X3.edgeDetection;
Pixel32.Kernel3X3.edgeDetectionH;
Pixel32.Kernel3X3.edgeDetectionV;
Pixel32.Kernel3X3.gradientDetectionH;
Pixel32.Kernel3X3.gradientDetectionV;
Pixel32.pngLoad(fileName);
Pixel32.perlinNoiseWrapBox(lx,ly,freqAndSum,rnd);
Pixel32.perlinNoise2BitWrapBox(lx,ly,freqAndSum,rnd);
```

Built on `quantum-script`, `quantum-script--random` and `xyo-pixel32`, part of the XYO C++ SDK.

## Documentation

- [Overview](docs/README.md) - purpose, concepts, source map
- [Getting started](docs/getting-started.md) - build, load from a script, hosts, register in a C++ host, tests, threads
- [Script API](docs/script-api.md) - `Pixel`, `Image`, `Kernel3X3`, PNG, noise, Perlin noise, recipes
- [C++ API](docs/cpp-api.md) - registration, DLL entry point, `VariablePixel` / `VariableImage` / `VariableKernel3X3`
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--pixel32](.claude/skills/quantum-script--pixel32/SKILL.md).

## License

Copyright (c) 2016-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
