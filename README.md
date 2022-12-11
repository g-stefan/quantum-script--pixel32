# Quantum Script Extension Pixel32

```javascript
Pixel32;
Pixel32.Pixel(r,g,b,a)
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
Pixel32.Image.prototype.wrap(img,dx,dy);
Pixel32.Image.prototype.wrapBox(img,dx,dy);
Pixel32.Image.prototype.blend(img,dx,dy,sx,sy,lx,ly);
Pixel32.Image.prototype.noise(random);
Pixel32.Image.prototype.noise2Bit(random);
Pixel32.Image.prototype.drawRectangle(sx,sy,lx,ly,pixel);
Pixel32.Image.prototype.drawFilledRectangle(sx,sy,lx,ly,pixel);
Pixel32.Image.prototype.colorRescale();
Pixel32.Image.prototype.average(img,level1,level2,delta);
Pixel32.Image.prototype.wrapBox1Resize(lx,ly,fz,k);
Pixel32.pngLoad(fileName);
Pixel32.perlinNoiseWrapBox(lx,ly,freqAndSum,rnd);
Pixel32.perlinNoise2BitWrapBox(lx,ly,freqAndSum,rnd);
```

## License

Copyright (c) 2022 Grigore Stefan
Licensed under the [MIT](LICENSE) license.
