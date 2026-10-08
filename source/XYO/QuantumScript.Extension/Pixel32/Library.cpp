// Quantum Script Extension Pixel32
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/Pixel32/Library.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/Copyright.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/License.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/Version.hpp>

#include <XYO/QuantumScript.Extension/Pixel32/Context.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/VariablePixel.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/VariableImage.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/VariableKernel3X3.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/Library.Source.cpp>

#include <XYO/QuantumScript.Extension/Random/VariableRandom.hpp>

namespace XYO::QuantumScript::Extension::Pixel32 {

	using namespace XYO::Pixel32;
	typedef Extension::Random::VariableRandom VariableRandom;
	namespace Pixel32Process = XYO::Pixel32::Process;

	Pixel32Context::Pixel32Context() {
		symbolFunctionPixel = 0;
		prototypePixel.pointerLink(this);
		symbolFunctionImage = 0;
		prototypeImage.pointerLink(this);
		symbolFunctionKernel3X3 = 0;
		prototypeKernel3X3.pointerLink(this);
	};

	Pixel32Context *getContext() {
		return TSingleton<Pixel32Context>::getValue();
	};

	// Script number to a coordinate, false for NaN / Infinity,
	// truncated toward zero, clamped to the long int range of every platform
	static bool toCoordinate(Variable *value, long int &out) {
		Number x = value->toNumber();
		if (isnan(x) || isinf(x)) {
			return false;
		};
		if (x > 2147483647.0) {
			x = 2147483647.0;
		};
		if (x < -2147483647.0) {
			x = -2147483647.0;
		};
		out = (long int)x;
		return true;
	};

	// Script number to a size or a length, false for NaN / Infinity / negative
	static bool toSize(Variable *value, long int &out) {
		Number x = value->toNumber();
		if (isnan(x) || isinf(x) || signbit(x)) {
			return false;
		};
		if (x > 2147483647.0) {
			x = 2147483647.0;
		};
		out = (long int)x;
		return true;
	};

	// Script number to a color channel, false for NaN, clamped to [0, 255]
	static bool toChannel(Variable *value, uint32_t &out) {
		Number x = value->toNumber();
		if (isnan(x)) {
			return false;
		};
		if (x < 0) {
			x = 0;
		};
		if (x > 255) {
			x = 255;
		};
		out = (uint32_t)x;
		return true;
	};

	static bool hexDigit(char c, uint32_t &out) {
		if (c >= '0' && c <= '9') {
			out = (uint32_t)(c - '0');
			return true;
		};
		if (c >= 'A' && c <= 'F') {
			out = (uint32_t)(c - 'A' + 10);
			return true;
		};
		if (c >= 'a' && c <= 'f') {
			out = (uint32_t)(c - 'a' + 10);
			return true;
		};
		return false;
	};

	// "RRGGBBAA" or "RRGGBB" (opaque), false for anything else
	static bool pixelFromString(const String &text, Pixel &out) {
		uint32_t channel[4];
		uint32_t high;
		uint32_t low;
		int length = (int)text.length();
		int k;
		if (length != 8 && length != 6) {
			return false;
		};
		channel[3] = 0xFF;
		for (k = 0; k < length / 2; ++k) {
			if (!hexDigit(text[k * 2], high)) {
				return false;
			};
			if (!hexDigit(text[k * 2 + 1], low)) {
				return false;
			};
			channel[k] = (high << 4) | low;
		};
		out = XYO_PIXEL32_PIXEL(channel[0], channel[1], channel[2], channel[3]);
		return true;
	};

	static TPointer<Variable> functionPixel(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-pixel\n");
#endif
		TPointerX<Variable> &rx = arguments->index(0);
		if (TIsTypeExact<VariableUndefined>(rx)) {
			return VariablePixel::newVariable(0);
		};
		if (TIsType<VariableNull>(rx)) {
			return VariablePixel::newVariable(0);
		};
		if (TIsType<VariablePixel>(rx)) {
			return VariablePixel::newVariable(((VariablePixel *)(rx.value()))->pixel);
		};
		if (TIsType<VariableString>(rx)) {
			Pixel pixel;
			if (!pixelFromString(((VariableString *)(rx.value()))->value, pixel)) {
				pixel = 0;
			};
			return VariablePixel::newVariable(pixel);
		};

		uint32_t r;
		uint32_t g;
		uint32_t b;
		uint32_t a;

		if (!toChannel(rx, r)) {
			r = 0;
		};
		if (!toChannel(arguments->index(1), g)) {
			g = 0;
		};
		if (!toChannel(arguments->index(2), b)) {
			b = 0;
		};
		if (!toChannel(arguments->index(3), a)) {
			a = 0;
		};

		return VariablePixel::newVariable(XYO_PIXEL32_PIXEL(r, g, b, a));
	};

	static TPointer<Variable> functionImage(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image\n");
#endif

		long int w;
		long int h;

		if (toSize(arguments->index(0), w) && toSize(arguments->index(1), h)) {
			TPointer<Image> image = Pixel32Process::create(w, h);
			if (image) {
				return VariableImage::newVariable(image.value());
			};
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> functionKernel3X3(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-kernel3x3\n");
#endif
		return VariableKernel3X3::newVariable(TMemory<Pixel32::Kernel3X3>::newMemory());
	};

	static void deleteContext() {
		Pixel32Context *pixel32Context = getContext();
		pixel32Context->prototypePixel.deleteMemory();
		pixel32Context->symbolFunctionPixel = 0;

		pixel32Context->prototypeImage.deleteMemory();
		pixel32Context->symbolFunctionImage = 0;

		pixel32Context->prototypeKernel3X3.deleteMemory();
		pixel32Context->symbolFunctionKernel3X3 = 0;
	};

	static void newContext(Executive *executive, void *extensionId) {
		VariableFunction *defaultPrototypeFunction;
		Symbol symbolPixel32 = Context::getSymbol("Pixel32");

		Pixel32Context *pixel32Context = getContext();
		executive->setExtensionDeleteContext(extensionId, deleteContext);

		// Pixel32={};
		(Context::getGlobalObject())->setPropertyBySymbol(symbolPixel32, VariableObject::newVariable());

		// Pixel32.Pixel=function(){};
		pixel32Context->symbolFunctionPixel = Context::getSymbol("Pixel");
		pixel32Context->prototypePixel.newMemory();

		defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(NULL, NULL, NULL, functionPixel, NULL, NULL);
		((Context::getGlobalObject())->getPropertyBySymbol(symbolPixel32))->setPropertyBySymbol(pixel32Context->symbolFunctionPixel, defaultPrototypeFunction);
		pixel32Context->prototypePixel = defaultPrototypeFunction->prototype;

		// Pixel32.Image=function(){};
		pixel32Context->symbolFunctionImage = Context::getSymbol("Image");
		pixel32Context->prototypeImage.newMemory();

		defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(NULL, NULL, NULL, functionImage, NULL, NULL);
		((Context::getGlobalObject())->getPropertyBySymbol(symbolPixel32))->setPropertyBySymbol(pixel32Context->symbolFunctionImage, defaultPrototypeFunction);
		pixel32Context->prototypeImage = defaultPrototypeFunction->prototype;

		// Pixel32.Kernel3X3=function(){};
		pixel32Context->symbolFunctionKernel3X3 = Context::getSymbol("Kernel3X3");
		pixel32Context->prototypeKernel3X3.newMemory();

		defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(NULL, NULL, NULL, functionKernel3X3, NULL, NULL);
		((Context::getGlobalObject())->getPropertyBySymbol(symbolPixel32))->setPropertyBySymbol(pixel32Context->symbolFunctionKernel3X3, defaultPrototypeFunction);
		pixel32Context->prototypeKernel3X3 = defaultPrototypeFunction->prototype;
	};

	static TPointer<Variable> pixelGetR(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-pixel-get-r\n");
#endif

		if (!TIsType<VariablePixel>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(XYO_PIXEL32_R(((VariablePixel *)(this_))->pixel));
	};

	static TPointer<Variable> pixelGetG(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-pixel-get-g\n");
#endif

		if (!TIsType<VariablePixel>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(XYO_PIXEL32_G(((VariablePixel *)(this_))->pixel));
	};

	static TPointer<Variable> pixelGetB(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-pixel-get-b\n");
#endif
		if (!TIsType<VariablePixel>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(XYO_PIXEL32_B(((VariablePixel *)(this_))->pixel));
	};

	static TPointer<Variable> pixelGetA(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-pixel-get-a\n");
#endif

		if (!TIsType<VariablePixel>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(XYO_PIXEL32_A(((VariablePixel *)(this_))->pixel));
	};

	static TPointer<Variable> pixelSetR(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-pixel-set-r\n");
#endif

		if (!TIsType<VariablePixel>(this_)) {
			throw(Error("invalid parameter"));
		};

		uint32_t value;
		if (toChannel(arguments->index(0), value)) {
			XYO_PIXEL32_CHANGE_R(((VariablePixel *)(this_))->pixel, value);
		};

		return this_;
	};

	static TPointer<Variable> pixelSetG(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-pixel-set-g\n");
#endif

		if (!TIsType<VariablePixel>(this_)) {
			throw(Error("invalid parameter"));
		};

		uint32_t value;
		if (toChannel(arguments->index(0), value)) {
			XYO_PIXEL32_CHANGE_G(((VariablePixel *)(this_))->pixel, value);
		};

		return this_;
	};

	static TPointer<Variable> pixelSetB(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-pixel-set-b\n");
#endif

		if (!TIsType<VariablePixel>(this_)) {
			throw(Error("invalid parameter"));
		};

		uint32_t value;
		if (toChannel(arguments->index(0), value)) {
			XYO_PIXEL32_CHANGE_B(((VariablePixel *)(this_))->pixel, value);
		};

		return this_;
	};

	static TPointer<Variable> pixelSetA(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-pixel-set-a\n");
#endif

		if (!TIsType<VariablePixel>(this_)) {
			throw(Error("invalid parameter"));
		};

		uint32_t value;
		if (toChannel(arguments->index(0), value)) {
			XYO_PIXEL32_CHANGE_A(((VariablePixel *)(this_))->pixel, value);
		};

		return this_;
	};

	static TPointer<Variable> pngLoad(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-png-load\n");
#endif

		TPointer<Image> image = Pixel32Process::pngLoad((char *)(((arguments->index(0))->toString()).value()));
		if (image) {
			return VariableImage::newVariable(image.value());
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> imagePngSave(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-png-save\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableBoolean::newVariable(Pixel32Process::pngSave(((VariableImage *)(this_))->image, (char *)(((arguments->index(0))->toString()).value())));
	};

	static TPointer<Variable> imageGetWidth(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-get-width\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableImage *)(this_))->image->width);
	};

	static TPointer<Variable> imageGetHeight(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-get-height\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableImage *)(this_))->image->height);
	};

	static TPointer<Variable> imageGetPixel(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-get-pixel\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		long int x;
		long int y;
		if (toCoordinate(arguments->index(0), x) && toCoordinate(arguments->index(1), y)) {
			return VariablePixel::newVariable(Pixel32Process::getPixel(((VariableImage *)(this_))->image, x, y));
		};

		return VariablePixel::newVariable(0);
	};

	static TPointer<Variable> imageGetPixelX(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-get-pixel-x\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		long int x;
		long int y;
		if (toCoordinate(arguments->index(0), x) && toCoordinate(arguments->index(1), y)) {
			return VariablePixel::newVariable(Pixel32Process::getPixelX(((VariableImage *)(this_))->image, x, y));
		};

		return VariablePixel::newVariable(0);
	};

	static TPointer<Variable> imageSetPixel(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-set-pixel\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		long int x;
		long int y;
		if (toCoordinate(arguments->index(0), x) && toCoordinate(arguments->index(1), y)) {
			TPointerX<Variable> &pixel = arguments->index(2);
			if (TIsType<VariablePixel>(pixel)) {
				Pixel32Process::setPixel(((VariableImage *)(this_))->image, x, y, ((VariablePixel *)(pixel.value()))->pixel);
			};
		};
		return Context::getValueUndefined();
	};

	typedef TPointer<Image> (*ImageResizeProc)(Image *imgThis, long int nx, long int ny);

	static TPointer<Variable> imageResizeWith(ImageResizeProc resizeProc, Variable *this_, VariableArray *arguments) {
		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		long int x;
		long int y;
		if (toSize(arguments->index(0), x) && toSize(arguments->index(1), y)) {
			TPointer<Image> image = (*resizeProc)(((VariableImage *)(this_))->image, x, y);
			if (image) {
				return VariableImage::newVariable(image.value());
			};
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> imageScaleUpBicubic(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-scale-up-bicubic\n");
#endif
		return imageResizeWith(Pixel32Process::scaleUpBicubic, this_, arguments);
	};

	static TPointer<Variable> imageScaleUpBilinear(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-scale-up-bilinear\n");
#endif
		return imageResizeWith(Pixel32Process::scaleUpBilinear, this_, arguments);
	};

	static TPointer<Variable> imageScaleUpNearestNeighbor(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-scale-up-nearest-neighbor\n");
#endif
		return imageResizeWith(Pixel32Process::scaleUpNearestNeighbor, this_, arguments);
	};

	typedef TPointer<Image> (*ImageHalfProc)(Image *imgThis);

	static TPointer<Variable> imageHalfWith(ImageHalfProc halfProc, Variable *this_) {
		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		TPointer<Image> image = (*halfProc)(((VariableImage *)(this_))->image);
		if (image) {
			return VariableImage::newVariable(image.value());
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> imageScaleDownX2(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-scale-down-x-2\n");
#endif
		return imageHalfWith(Pixel32Process::scaleDownX2, this_);
	};

	static TPointer<Variable> imageScaleDownX2OnX(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-scale-down-x-2-on-x\n");
#endif
		return imageHalfWith(Pixel32Process::scaleDownX2OnX, this_);
	};

	static TPointer<Variable> imageScaleDownX2OnY(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-scale-down-x-2-on-y\n");
#endif
		return imageHalfWith(Pixel32Process::scaleDownX2OnY, this_);
	};

	static TPointer<Variable> imageScaleDown(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-scale-down\n");
#endif
		return imageResizeWith(Pixel32Process::scaleDown, this_, arguments);
	};

	static TPointer<Variable> imageScaleUp(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-scale-up\n");
#endif
		return imageResizeWith(Pixel32Process::scaleUp, this_, arguments);
	};

	static TPointer<Variable> imageResize(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-resize\n");
#endif
		return imageResizeWith(Pixel32Process::resize, this_, arguments);
	};

	static TPointer<Variable> imageCut(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-cut\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		long int sx;
		long int sy;
		long int lx;
		long int ly;

		// the rectangle is clipped to the image
		if (toCoordinate(arguments->index(0), sx) &&
		    toCoordinate(arguments->index(1), sy) &&
		    toSize(arguments->index(2), lx) &&
		    toSize(arguments->index(3), ly)) {

			TPointer<Image> image = Pixel32Process::cut(((VariableImage *)(this_))->image, sx, sy, lx, ly);
			if (image) {
				return VariableImage::newVariable(image.value());
			};
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> imageClear(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-clear\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		TPointerX<Variable> &pixel = arguments->index(0);
		if (TIsType<VariablePixel>(pixel)) {

			Pixel32Process::clear(((VariableImage *)(this_))->image,
			                      ((VariablePixel *)(pixel.value()))->pixel);
		};
		return Context::getValueUndefined();
	};

	typedef void (*ImageCopyProc)(Image *imgThis, Image *imgIn2, long int dx, long int dy, long int sx, long int sy, long int lx, long int ly);

	static TPointer<Variable> imageCopyWith(ImageCopyProc copyProc, Variable *this_, VariableArray *arguments) {
		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		long int dx;
		long int dy;
		long int sx;
		long int sy;
		long int lx;
		long int ly;

		// both rectangles are clipped, positions can be negative
		if (toCoordinate(arguments->index(1), dx) &&
		    toCoordinate(arguments->index(2), dy) &&
		    toCoordinate(arguments->index(3), sx) &&
		    toCoordinate(arguments->index(4), sy) &&
		    toSize(arguments->index(5), lx) &&
		    toSize(arguments->index(6), ly)) {

			TPointerX<Variable> &imageIn2 = arguments->index(0);
			if (TIsType<VariableImage>(imageIn2)) {
				(*copyProc)(((VariableImage *)(this_))->image,
				            ((VariableImage *)(imageIn2.value()))->image,
				            dx, dy, sx, sy, lx, ly);
			};
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> imageCopy(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-copy\n");
#endif
		return imageCopyWith(Pixel32Process::copy, this_, arguments);
	};

	static TPointer<Variable> imageWrap(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-wrap\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		long int dx;
		long int dy;

		// offsets can be negative, they are taken modulo the size
		if (toCoordinate(arguments->index(0), dx) && toCoordinate(arguments->index(1), dy)) {
			TPointer<Image> image = Pixel32Process::wrap(((VariableImage *)(this_))->image, dx, dy);
			if (image) {
				return VariableImage::newVariable(image.value());
			};
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> imageWrapBox(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-wrap-box\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		long int dx;
		long int dy;

		if (toSize(arguments->index(0), dx) && toSize(arguments->index(1), dy)) {
			TPointer<Image> image = Pixel32Process::wrapBox(((VariableImage *)(this_))->image, dx, dy);
			if (image) {
				return VariableImage::newVariable(image.value());
			};
		};

		return Context::getValueUndefined();
	};

	static TPointer<Variable> imageBlend(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-blend\n");
#endif
		return imageCopyWith(Pixel32Process::blend, this_, arguments);
	};

	static TPointer<Variable> imageNoise(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-noise\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		TPointerX<Variable> &random = arguments->index(0);
		if (TIsType<VariableRandom>(random)) {

			Pixel32Process::noise(((VariableImage *)(this_))->image,
			                      ((VariableRandom *)(random.value()))->value);
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> imageNoise2Bit(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-noise-2-bit\n");
#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		TPointerX<Variable> &random = arguments->index(0);
		if (TIsType<VariableRandom>(random)) {

			Pixel32Process::noise2Bit(((VariableImage *)(this_))->image,
			                          ((VariableRandom *)(random.value()))->value);
		};
		return Context::getValueUndefined();
	};

	typedef void (*ImageDrawProc)(Image *imgThis, long int sx, long int sy, long int lx, long int ly, Pixel pixel);

	static TPointer<Variable> imageDrawWith(ImageDrawProc drawProc, Variable *this_, VariableArray *arguments) {
		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		long int sx;
		long int sy;
		long int lx;
		long int ly;

		// the rectangle is clipped to the image
		if (toCoordinate(arguments->index(0), sx) &&
		    toCoordinate(arguments->index(1), sy) &&
		    toCoordinate(arguments->index(2), lx) &&
		    toCoordinate(arguments->index(3), ly)) {

			TPointerX<Variable> &pixel = arguments->index(4);
			if (TIsType<VariablePixel>(pixel)) {
				(*drawProc)(((VariableImage *)(this_))->image, sx, sy, lx, ly, ((VariablePixel *)(pixel.value()))->pixel);
			};
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> imageDrawRectangle(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-draw-rectangle\n");
#endif
		return imageDrawWith(Pixel32Process::drawRectangle, this_, arguments);
	};

	static TPointer<Variable> imageDrawFilledRectangle(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-draw-filled-rectangle\n");
#endif
		return imageDrawWith(Pixel32Process::drawFilledRectangle, this_, arguments);
	};

	static TPointer<Variable> imageColorRescale(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-color-rescale\n");
#endif
		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		Pixel32Process::colorRescale(((VariableImage *)(this_))->image);

		return Context::getValueUndefined();
	};

	// Script number to an average weight, false for NaN / negative, clamped to 32 bit
	static bool toWeight(Variable *value, uint32_t &out) {
		Number x = value->toNumber();
		if (isnan(x) || signbit(x)) {
			return false;
		};
		if (x > 4294967295.0) {
			x = 4294967295.0;
		};
		out = (uint32_t)x;
		return true;
	};

	static TPointer<Variable> imageAverage(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-average\n");

#endif

		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		uint32_t level1;
		uint32_t level2;
		uint32_t delta;

		if (toWeight(arguments->index(1), level1) &&
		    toWeight(arguments->index(2), level2) &&
		    toWeight(arguments->index(3), delta)) {

			TPointerX<Variable> &imageIn2 = arguments->index(0);
			if (TIsType<VariableImage>(imageIn2)) {

				Pixel32Process::average(((VariableImage *)(this_))->image,
				                        ((VariableImage *)(imageIn2.value()))->image,
				                        level1,
				                        level2,
				                        delta);

				return this_;
			};
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> imageKernel3X3(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-image-kernel3x3\n");

#endif
		if (!TIsType<VariableImage>(this_)) {
			throw(Error("invalid parameter"));
		};

		TPointerX<Variable> &kernel = arguments->index(0);
		if (!TIsType<VariableKernel3X3>(kernel)) {
			throw(Error("invalid parameter"));
		};

		TPointer<Image> image = Pixel32Process::kernel3X3(((VariableImage *)(this_))->image,
		                                                  *(((VariableKernel3X3 *)(kernel.value()))->kernel));
		if (image) {
			return VariableImage::newVariable(image.value());
		};
		return Context::getValueUndefined();
	};

	static TPointer<Variable> kernel3X3GetNormalA(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-kernel3x3-get-normalA\n");
#endif
		if (!TIsType<VariableKernel3X3>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableKernel3X3 *)(this_))->kernel->normalA);
	};

	static TPointer<Variable> kernel3X3SetNormalA(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-kernel3x3-set-normalA\n");
#endif
		if (!TIsType<VariableKernel3X3>(this_)) {
			throw(Error("invalid parameter"));
		};

		((VariableKernel3X3 *)(this_))->kernel->normalA = (arguments->index(0))->toNumber();

		return this_;
	};

	static TPointer<Variable> kernel3X3GetNormalB(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-kernel3x3-get-normalB\n");
#endif
		if (!TIsType<VariableKernel3X3>(this_)) {
			throw(Error("invalid parameter"));
		};

		return VariableNumber::newVariable(((VariableKernel3X3 *)(this_))->kernel->normalB);
	};

	static TPointer<Variable> kernel3X3SetNormalB(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-kernel3x3-set-normalB\n");
#endif
		if (!TIsType<VariableKernel3X3>(this_)) {
			throw(Error("invalid parameter"));
		};

		((VariableKernel3X3 *)(this_))->kernel->normalB = (arguments->index(0))->toNumber();

		return this_;
	};

	static TPointer<Variable> kernel3X3GetMatrixV(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-kernel3x3-get-matrix-v\n");
#endif
		if (!TIsType<VariableKernel3X3>(this_)) {
			throw(Error("invalid parameter"));
		};

		size_t x = (arguments->index(0))->toIndex();
		size_t y = (arguments->index(1))->toIndex();

		if (x > 2 || y > 2) {
			return Context::getValueUndefined();
		};

		// v[row][column]
		return VariableNumber::newVariable(((VariableKernel3X3 *)(this_))->kernel->v[y][x]);
	};

	static TPointer<Variable> kernel3X3SetMatrixV(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
		printf("- pixel32-kernel3x3-set-matrix-v\n");
#endif
		if (!TIsType<VariableKernel3X3>(this_)) {
			throw(Error("invalid parameter"));
		};

		size_t x = (arguments->index(0))->toIndex();
		size_t y = (arguments->index(1))->toIndex();

		if (x > 2 || y > 2) {
			return Context::getValueUndefined();
		};

		// v[row][column]
		((VariableKernel3X3 *)(this_))->kernel->v[y][x] = (arguments->index(2))->toNumber();

		return this_;
	};

	static void setKernel3X3(const char *name, Kernel3X3 &src) {
		TPointer<Kernel3X3> kernel;
		kernel.newMemory();
		kernel->copy(src);
		(((Context::getGlobalObject())->getPropertyBySymbol(Context::getSymbol("Pixel32")))->getPropertyBySymbol(Context::getSymbol("Kernel3X3")))->setPropertyBySymbol(Context::getSymbol(name), VariableKernel3X3::newVariable(kernel));
	};

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("Pixel32", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {
		TMemory<VariablePixel>::initMemory();
		TMemory<VariableImage>::initMemory();
		TMemory<VariableKernel3X3>::initMemory();

		String info = "Pixel32\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "Pixel32");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::Pixel32::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		newContext(executive, extensionId);

		executive->setFunction2("Pixel32.Pixel.prototype.getR()", pixelGetR);
		executive->setFunction2("Pixel32.Pixel.prototype.getG()", pixelGetG);
		executive->setFunction2("Pixel32.Pixel.prototype.getB()", pixelGetB);
		executive->setFunction2("Pixel32.Pixel.prototype.getA()", pixelGetA);
		executive->setFunction2("Pixel32.Pixel.prototype.setR(v)", pixelSetR);
		executive->setFunction2("Pixel32.Pixel.prototype.setG(v)", pixelSetG);
		executive->setFunction2("Pixel32.Pixel.prototype.setB(v)", pixelSetB);
		executive->setFunction2("Pixel32.Pixel.prototype.setA(v)", pixelSetA);
		executive->setFunction2("Pixel32.Image.prototype.pngSave(fileName)", imagePngSave);
		executive->setFunction2("Pixel32.Image.prototype.getWidth()", imageGetWidth);
		executive->setFunction2("Pixel32.Image.prototype.getHeight()", imageGetHeight);
		executive->setFunction2("Pixel32.Image.prototype.getPixel(x,y)", imageGetPixel);
		executive->setFunction2("Pixel32.Image.prototype.getPixelX(x,y)", imageGetPixelX);
		executive->setFunction2("Pixel32.Image.prototype.setPixel(x,y,pixel)", imageSetPixel);
		executive->setFunction2("Pixel32.Image.prototype.scaleUpBicubic(x,y)", imageScaleUpBicubic);
		executive->setFunction2("Pixel32.Image.prototype.scaleUpBilinear(x,y)", imageScaleUpBilinear);
		executive->setFunction2("Pixel32.Image.prototype.scaleUpNearestNeighbor(x,y)", imageScaleUpNearestNeighbor);
		executive->setFunction2("Pixel32.Image.prototype.scaleDownX2()", imageScaleDownX2);
		executive->setFunction2("Pixel32.Image.prototype.scaleDownX2OnX()", imageScaleDownX2OnX);
		executive->setFunction2("Pixel32.Image.prototype.scaleDownX2OnY()", imageScaleDownX2OnY);
		executive->setFunction2("Pixel32.Image.prototype.scaleDown(x,y)", imageScaleDown);
		executive->setFunction2("Pixel32.Image.prototype.scaleUp(x,y)", imageScaleUp);
		executive->setFunction2("Pixel32.Image.prototype.resize(x,y)", imageResize);
		executive->setFunction2("Pixel32.Image.prototype.cut(sx,sy,lx,ly)", imageCut);
		executive->setFunction2("Pixel32.Image.prototype.clear(pixel)", imageClear);
		executive->setFunction2("Pixel32.Image.prototype.copy(img,dx,dy,sx,sy,lx,ly)", imageCopy);
		executive->setFunction2("Pixel32.Image.prototype.wrap(dx,dy)", imageWrap);
		executive->setFunction2("Pixel32.Image.prototype.wrapBox(dx,dy)", imageWrapBox);
		executive->setFunction2("Pixel32.Image.prototype.blend(img,dx,dy,sx,sy,lx,ly)", imageBlend);
		executive->setFunction2("Pixel32.Image.prototype.noise(random)", imageNoise);
		executive->setFunction2("Pixel32.Image.prototype.noise2Bit(random)", imageNoise2Bit);
		executive->setFunction2("Pixel32.Image.prototype.drawRectangle(sx,sy,lx,ly,pixel)", imageDrawRectangle);
		executive->setFunction2("Pixel32.Image.prototype.drawFilledRectangle(sx,sy,lx,ly,pixel)", imageDrawFilledRectangle);
		executive->setFunction2("Pixel32.Image.prototype.colorRescale()", imageColorRescale);
		executive->setFunction2("Pixel32.Image.prototype.average(img,level1,level2,delta)", imageAverage);
		executive->setFunction2("Pixel32.Image.prototype.kernel3X3(kernel3X3)", imageKernel3X3);
		executive->setFunction2("Pixel32.Kernel3X3.prototype.getNormalA()", kernel3X3GetNormalA);
		executive->setFunction2("Pixel32.Kernel3X3.prototype.setNormalA(value)", kernel3X3SetNormalA);
		executive->setFunction2("Pixel32.Kernel3X3.prototype.getNormalB()", kernel3X3GetNormalB);
		executive->setFunction2("Pixel32.Kernel3X3.prototype.setNormalB(value)", kernel3X3SetNormalB);
		executive->setFunction2("Pixel32.Kernel3X3.prototype.getMatrixV(x,y)", kernel3X3GetMatrixV);
		executive->setFunction2("Pixel32.Kernel3X3.prototype.setMatrixV(x,y,value)", kernel3X3SetMatrixV);
		//
		executive->setFunction2("Pixel32.pngLoad(fileName)", pngLoad);
		//
		setKernel3X3("filter8X1", Kernel3X3::filter8X1);
		setKernel3X3("gaussian", Kernel3X3::gaussian);
		setKernel3X3("blur", Kernel3X3::blur);
		setKernel3X3("sharpen", Kernel3X3::sharpen);
		setKernel3X3("edgeDetection", Kernel3X3::edgeDetection);
		setKernel3X3("edgeDetectionH", Kernel3X3::edgeDetectionH);
		setKernel3X3("edgeDetectionV", Kernel3X3::edgeDetectionV);
		setKernel3X3("gradientDetectionH", Kernel3X3::gradientDetectionH);
		setKernel3X3("gradientDetectionV", Kernel3X3::gradientDetectionV);
		//

		executive->compileStringX(librarySource);
	};

};

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_LIBRARY
#	ifdef XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::Pixel32::initExecutive(executive, extensionId);
};
#	endif
#endif
