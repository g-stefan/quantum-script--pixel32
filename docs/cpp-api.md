# C++ API

The C++ side is small: two functions to load the extension into an engine,
the DLL entry point, and three `Variable` types that other extensions or
hosts can use to exchange images with scripts.

```cpp
#include <XYO/QuantumScript.Extension/Pixel32.hpp>                     // initExecutive, registerInternalExtension
#include <XYO/QuantumScript.Extension/Pixel32/VariablePixel.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/VariableImage.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/VariableKernel3X3.hpp>

using namespace XYO::QuantumScript;
namespace QSPixel32 = XYO::QuantumScript::Extension::Pixel32;
```

The headers include `<XYO/QuantumScript.hpp>` and `<XYO/Pixel32.hpp>`.
Everything is in `XYO::QuantumScript::Extension::Pixel32`, which also does
`using namespace XYO::Pixel32` (so `Image`, `Pixel`, `Kernel3X3` are the
`xyo-pixel32` types). Inside that namespace a bare `Pixel32` names the
extension namespace, not the library: write `XYO::Pixel32::Process::...` in
full, or use an alias as the extension does
(`namespace Pixel32Process = XYO::Pixel32::Process;`).

## Loading the extension

```cpp
namespace XYO::QuantumScript::Extension::Pixel32 {
	void registerInternalExtension(Executive *executive);
	void initExecutive(Executive *executive, void *extensionId);
};
```

- `registerInternalExtension(executive)` registers `initExecutive` under the
  name `"Pixel32"`. Call it from the host init callback; scripts then load
  it with `Script.requireExtension("Pixel32")`. Register `Random` too
  (`Extension::Random::registerInternalExtension`): the script part of
  `Pixel32` requires it.
- `initExecutive(executive, extensionId)` is what the engine calls when the
  extension is loaded: it sets name / info / version, creates the `Pixel32`
  object and the three constructors, registers every method with
  `setFunction2("Pixel32.Image.prototype.resize(x,y)", imageResize)`,
  creates the predefined kernels and compiles `Library.js`
  (`librarySource`, generated into `Library.Source.cpp`).
- The DLL exports
  `extern "C" void quantumScriptExtension(Executive *, void *extensionId)`,
  which calls `initExecutive`. It exists only in a dynamic library build
  (`XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY`) and not when
  `XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_LIBRARY` is defined.

On unload (`setExtensionDeleteContext`) the per-thread `Pixel32Context`
drops the three prototypes.

## Script values

All three derive from `XYO::QuantumScript::Variable`, are allocated from an
active memory pool (`TMemory<...>`), have a dynamic type
(`TIsType<VariableImage>(v)`) and implement `clone` (copy into another
thread, see below).

### VariablePixel

```cpp
class VariablePixel : public Variable {
	public:
		Pixel pixel;                                  // uint32_t, R | G << 8 | B << 16 | A << 24
		static Variable *newVariable(Pixel pixel_);
		String getVariableType();                     // "Pixel32.Pixel"
		String toString();                            // "RRGGBBAA"
		Boolean toBoolean();                          // true
		Variable *clone(SymbolList &inSymbolList);    // same color
};
```

### VariableImage

```cpp
class VariableImage : public Variable {
	public:
		TPointer<Image> image;                        // never nullptr in a value made by the extension
		static Variable *newVariable(Image *image_);  // takes a reference, does not copy
		String getVariableType();                     // "Pixel32.Image"
		String toString();                            // "Pixel32.Image"
		Boolean toBoolean();                          // true
		Variable *clone(SymbolList &inSymbolList);    // deep copy of the pixels
};
```

### VariableKernel3X3

```cpp
class VariableKernel3X3 : public Variable {
	public:
		TPointer<Kernel3X3> kernel;
		static Variable *newVariable(Kernel3X3 *kernel_);
		String getVariableType();                     // "Pixel32.Kernel3X3"
		String toString();                            // "Pixel32.Kernel3X3"
		Variable *clone(SymbolList &inSymbolList);    // copy of the matrix and factors
};
```

The script methods `getMatrixV(x, y)` / `setMatrixV(x, y, v)` use `x` as the
column and `y` as the row: they read and write `kernel->v[y][x]`
(`xyo-pixel32` stores `v[row][column]`).

## Exchanging images with scripts

Give a script an image made in C++ (the value shares the `Image`; nothing is
copied):

```cpp
static TPointer<Variable> hostScreenshot(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointer<XYO::Pixel32::Image> image = XYO::Pixel32::Process::create(320, 200);
	if (!image) {
		return Context::getValueUndefined();
	};
	// ... fill image->pixel[y][x] ...
	return QSPixel32::VariableImage::newVariable(image);
};

static void initExecutive(Executive *executive) {
	Extension::Random::registerInternalExtension(executive);
	Extension::Pixel32::registerInternalExtension(executive);
	executive->compileStringX("var Host={};");
	executive->setFunction2("Host.screenshot()", hostScreenshot);
};
```

The script must still call `Script.requireExtension("Pixel32")` before it
uses the methods of the returned value: the prototypes are created when the
extension is loaded.

Take an image from a script argument:

```cpp
static TPointer<Variable> hostShow(VariableFunction *function, Variable *this_, VariableArray *arguments) {
	TPointerX<Variable> &argument = arguments->index(0);
	if (!TIsType<QSPixel32::VariableImage>(argument)) {
		throw Error("invalid parameter");
	};
	XYO::Pixel32::Image *image = ((QSPixel32::VariableImage *)(argument.value()))->image;
	// read image->width, image->height, image->pixel[y][x]
	return Context::getValueUndefined();
};
```

Changes made in C++ to `image` are visible to the script and the other way
around, because both hold the same `Image`.

## Threads

Each thread that runs scripts has its own `Pixel32Context` (a thread
singleton): symbols and prototypes belong to the thread. When a value
crosses threads (`Thread` / `Job` extensions, `Executive::cloneVariable`)
the engine calls `clone`:

| Type | `clone` |
|------|---------|
| `VariablePixel` | a new value with the same color |
| `VariableImage` | a new value with a **copy** of the image (`Process::cut` of the whole image); `undefined` if the copy cannot be allocated |
| `VariableKernel3X3` | a new value with a copy of the kernel |

## Notes for maintainers

- Methods live in `Pixel32/Library.cpp` as
  `static TPointer<Variable> name(VariableFunction *, Variable *this_, VariableArray *arguments)`.
  They check `TIsType<VariableImage>(this_)` (or `VariablePixel` /
  `VariableKernel3X3`) and throw `Error("invalid parameter")` otherwise.
- Convert number arguments with the helpers at the top of `Library.cpp`,
  not with casts: `toCoordinate` (finite, can be negative, clamped to the
  32-bit `long int` range — `long int` is 32-bit on Windows), `toSize`
  (finite, not negative), `toChannel` (clamped `0..255`), `toWeight`. They
  return `false` for values the call must ignore.
- Keep the library semantics: clipping and negative positions are handled by
  `xyo-pixel32`; do not reject negative coordinates in the binding. Return
  `undefined` when a `Process::` function returns `nullptr`.
- The script part is `Library.js`; `fabricare make` regenerates
  `Library.Source.cpp`. `wrapBox1Resize` cuts `lx + fz` and resizes back on
  purpose: `resize` maps corner to corner, and this keeps the texture
  tileable (checked by `test/test.0003.js`, "perlin tiles").
- `Variable::isEqual` is not virtual, so the `isEqual` members of the three
  types are never called by `==`.
- New or changed methods: update `README.md`, `docs/script-api.md`,
  `docs/reference.md`, `test/test.0003.js` and the skill in
  `.claude/skills/quantum-script--pixel32/`.
- Code style: tabs (`.clang-format`), CRLF, statements and blocks end with
  `};`, camelCase. SPDX: MIT for `source/` and `docs/`, Unlicense for
  `test/`, `fabricare/` and `.claude/` (see `.reuse/dep5`).
