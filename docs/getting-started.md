# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it), `quantum-script--console`, `quantum-script--random` and
`xyo-pixel32` must be installed to the SDK first. From the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run the tests (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

`make` first runs `fabricare/make.prepare.js`, which converts the script part
of the extension, `Library.js`, into `Library.Source.cpp` (a C string
compiled into the library). Edit `Library.js`, never the generated file.

`fabricare.json` declares two projects:

| Project | Kind | Purpose |
|---------|------|---------|
| `quantum-script--pixel32` | `dll-or-lib`: shared library in a dynamic build, static library in a static build | the extension |
| `test.0003` | executable, category `test` | runs `test/test.0003.js` with the extension just built |

After `fabricare install`, `quantum-script--pixel32.dll` (Windows) /
`libquantum-script--pixel32.so` (Linux) sits in the SDK `bin` folder next to
`quantum-script.exe`, which is where `Script.requireExtension("Pixel32")`
finds it.

## 2. Use it from a script

```javascript
Script.requireExtension("Console");
Script.requireExtension("Pixel32");

var image = new Pixel32.Image(64, 32);              // transparent black
image.clear(new Pixel32.Pixel(255, 255, 255, 255)); // opaque white
image.drawFilledRectangle(8, 8, 48, 16, new Pixel32.Pixel("2060C0FF"));
image.drawRectangle(0, 0, 64, 32, new Pixel32.Pixel(0, 0, 0, 255));

Console.writeLn(image.getPixel(10, 10));            // 2060C0FF
Console.writeLn(image.pngSave("hello.png"));        // true

var thumb = Pixel32.pngLoad("hello.png").resize(16, 8);
Console.writeLn(thumb.getWidth() + "x" + thumb.getHeight());   // 16x8
```

Run it with:

```bash
quantum-script hello-pixel32.js
```

`Script.requireExtension("Pixel32")` looks for an external
`quantum-script--pixel32` library first (the file as named, then every
include path folder: next to the interpreter, next to the script), then for
an internal extension registered by the host. Loading twice does nothing. A
missing extension throws `Unable to open "Pixel32"`.

Loading `Pixel32` also loads `Random` (its script part calls
`Script.requireExtension("Random")`), so `new Random()` is available for
`noise`, `noise2Bit` and the Perlin noise functions without another line.

Until the extension is loaded `Pixel32` does not exist: `typeof(Pixel32)` is
`"undefined"`.

## 3. Hosts that already have it

- `magnet` (through `quantum-script--magnet`) registers `Pixel32` as an
  internal extension: its scripts call `Script.requireExtension("Pixel32")`
  and need no DLL.
- `quantum-script` and `fabricare` do **not** preload it; a script or build
  script calls `Script.requireExtension("Pixel32")`, which loads the
  installed DLL. The icon font projects (`Web/*-icons-font`) use it this way
  in their `fabricare/source/process.qs.js` to clean up glyph images.

## 4. Register it in a C++ host

A host that embeds Quantum Script makes `Pixel32` available as an internal
extension by registering it in the init callback. Register `Random` too: the
script part of `Pixel32` requires it.

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/Random.hpp>
#include <XYO/QuantumScript.Extension/Pixel32.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::Random::registerInternalExtension(executive);
	Extension::Pixel32::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeString(
		        "Script.requireExtension(\"Console\");"
		        "Script.requireExtension(\"Pixel32\");"
		        "var image = new Pixel32.Image(4, 4);"
		        "Console.writeLn(image.getWidth());")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes the extension *available*: scripts still call
`Script.requireExtension("Pixel32")`. With the DLL build of the engine an
external `quantum-script--pixel32.dll` found on the include path wins over
the internal one; use `Script.requireInternalExtension("Pixel32")` to force
the internal one.

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--pixel32"
	]
}
```

`quantum-script--pixel32` depends on `quantum-script`,
`quantum-script--console`, `quantum-script--random` and `xyo-pixel32`;
fabricare resolves them transitively.

## 5. Static builds

There is no separate `.static` project. On a static platform (for example
`win64-msvc-2026.static`) the `dll-or-lib` project is built as a static
library, the export macro is empty and the `quantumScriptExtension` DLL
entry point is left out (define `XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_LIBRARY`
when you compile the sources into your own program). A static host must
register the extension with `registerInternalExtension` (section 4).

## 6. Tests

`fabricare test` (after `fabricare make`) runs:

1. `test.0003`: a C++ host built in `output/test` that registers `Console`,
   `Random`, `Thread` and `Pixel32` as internal extensions and runs
   `test/test.0003.js` — about 100 checks of the whole API, against the
   extension **just built** in `output/bin`, not the installed one. The PNG
   check writes `test.0003.png` in `output/test`.
2. `test/test.0001.js` and `test/test.0002.js` with the installed
   `quantum-script`: they generate Perlin noise textures in
   `test/output.0001.png` and `test/output.0002.png` (64 blur passes). Open
   `test/output.0001.html` in a browser to see `output.0001.png` tiled: no
   seams should be visible. These two use the **installed** extension (the
   interpreter loads the DLL next to itself).

`test/test.0003.js` also runs directly, with the installed extension:

```bash
quantum-script test/test.0003.js
```

A failed check prints `* failed: <name>: "<value>", expected "<expected>"`
and the script ends with an exception (exit code 1).

To try a script with the extension just built, without installing it, put
the script in `output/bin` and run it with its **full path**: the
interpreter looks for the DLL in the script's folder before its own folder.
With a bare file name (`quantum-script my.js` from inside `output/bin`) the
script folder is empty and the installed DLL is loaded.

## 7. Threads

Each thread that runs scripts has its own engine, so every thread loads the
extension itself with `Script.requireExtension("Pixel32")`.

`Pixel`, `Image` and `Kernel3X3` values passed to another thread (as `this`,
arguments or return value, for example with the `Thread` extension) are
**copied**: an image is copied pixel by pixel, so changing the copy never
changes the original. Load `Pixel32` in the thread before calling methods on
the copies:

```javascript
Script.requireExtension("Thread");
Script.requireExtension("Pixel32");

var thread = Thread.newThread(function(image) {
	Script.requireExtension("Pixel32");
	return image.kernel3X3(Pixel32.Kernel3X3.gaussian);   // copied back
}, undefined, [Pixel32.pngLoad("photo.png")]);
thread.join();
var soft = thread.getReturnedValue();
```

An image object itself has no lock: use it from the thread that owns it.
