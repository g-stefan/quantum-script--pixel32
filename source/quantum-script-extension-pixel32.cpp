//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef XYO_OS_TYPE_WIN
#ifdef XYO_MEMORY_LEAK_DETECTOR
#include "vld.h"
#endif
#endif

#include "quantum-script-extension-pixel32-license.hpp"
#include "quantum-script-extension-pixel32.hpp"
#include "quantum-script-extension-pixel32-variablepixel.hpp"
#include "quantum-script-extension-pixel32-variableimage.hpp"
#include "quantum-script-extension-pixel32-variablekernel3x3.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_NO_VERSION
#include "quantum-script-extension-pixel32-version.hpp"
#endif

#include "quantum-script-variablenull.hpp"
#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

#include "quantum-script-extension-random-variablerandom.hpp"

#include "quantum-script-extension-pixel32.src"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {

				using namespace XYO;
				using namespace Quantum::Script;
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

				static TPointer<Variable> functionPixel(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-pixel\n");
#endif
					TPointerX<Variable> &rx = arguments->index(0);
					if(TIsTypeExact<VariableUndefined>(rx)) {
						return VariablePixel::newVariable(0);
					};
					if(TIsType<VariableNull>(rx)) {
						return VariablePixel::newVariable(0);
					};
					if(TIsType<VariableString>(rx)) {
						uint32_t r_;
						uint32_t g_;
						uint32_t b_;
						uint32_t a_;
						if(sscanf((char *)(((VariableString *)(rx.value()))->value.value()), "%02X%02X%02X%02X", &r_, &g_, &b_, &a_) != 4){
							r_ = 0;
							g_ = 0;
							b_ = 0;
							a_ = 0;
						};
						return VariablePixel::newVariable(XYO_PIXEL32_PIXEL(r_, g_, b_, a_));
					};

					Number r_ = rx->toNumber();
					Number g_ = (arguments->index(1))->toNumber();
					Number b_ = (arguments->index(2))->toNumber();
					Number a_ = (arguments->index(3))->toNumber();

					Integer r;
					Integer g;
					Integer b;
					Integer a;

					if(isnan(r_) || isinf(r_) || signbit(r_)) {
						r = 0;
					} else {
						r = (Integer)r_;
					};

					if(isnan(g_) || isinf(g_) || signbit(g_)) {
						g = 0;
					} else {
						g = (Integer)g_;
					};

					if(isnan(b_) || isinf(b_) || signbit(b_)) {
						b = 0;
					} else {
						b = (Integer)b_;
					};

					if(isnan(a_) || isinf(a_) || signbit(a_)) {
						a = 0;
					} else {
						a = (Integer)a_;
					};

					return VariablePixel::newVariable(XYO_PIXEL32_PIXEL(r, g, b, a));
				};

				static TPointer<Variable> functionImage(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image\n");
#endif

					Number w_ = (arguments->index(0))->toNumber();
					Number h_ = (arguments->index(1))->toNumber();

					if(!(isnan(w_) || isinf(w_) || signbit(w_))) {
						if(!(isnan(h_) || isinf(h_) || signbit(h_))) {
							Integer w = w_;
							Integer h = h_;
							TPointer<Image> image = Pixel32Process::create(w, h);
							if(image) {
								return VariableImage::newVariable(image.value());
							};
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
				};

				static void newContext(Executive *executive, void *extensionId) {
					VariableFunction *defaultPrototypeFunction;
					Symbol symbolPixel32 = Context::getSymbol("Pixel32");

					Pixel32Context *pixel32Context = getContext();
					executive->setExtensionDeleteContext(extensionId, deleteContext);

					// Pixel32={};
					(Context::getGlobalObject())->setPropertyBySymbol(symbolPixel32,VariableObject::newVariable());

					// Pixel32.Pixel=function(){};
					pixel32Context->symbolFunctionPixel = Context::getSymbol("Pixel");
					pixel32Context->prototypePixel.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(NULL, NULL, NULL, functionPixel, NULL, NULL);
					((Context::getGlobalObject())->getPropertyBySymbol(symbolPixel32))->setPropertyBySymbol(pixel32Context->symbolFunctionPixel,defaultPrototypeFunction);
					pixel32Context->prototypePixel = defaultPrototypeFunction->prototype;

					// Pixel32.Image=function(){};
					pixel32Context->symbolFunctionImage = Context::getSymbol("Image");
					pixel32Context->prototypeImage.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(NULL, NULL, NULL, functionImage, NULL, NULL);
					((Context::getGlobalObject())->getPropertyBySymbol(symbolPixel32))->setPropertyBySymbol(pixel32Context->symbolFunctionImage,defaultPrototypeFunction);
					pixel32Context->prototypeImage = defaultPrototypeFunction->prototype;

					// Pixel32.Kernel3X3=function(){};
					pixel32Context->symbolFunctionKernel3X3 = Context::getSymbol("Kernel3X3");
					pixel32Context->prototypeKernel3X3.newMemory();

					defaultPrototypeFunction = (VariableFunction *)VariableFunction::newVariable(NULL, NULL, NULL, functionKernel3X3, NULL, NULL);
					((Context::getGlobalObject())->getPropertyBySymbol(symbolPixel32))->setPropertyBySymbol(pixel32Context->symbolFunctionKernel3X3,defaultPrototypeFunction);
					pixel32Context->prototypeKernel3X3 = defaultPrototypeFunction->prototype;
				};

				static TPointer<Variable> pixelGetR(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-pixel-get-r\n");
#endif

					if(!TIsType<VariablePixel>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(XYO_PIXEL32_R(((VariablePixel *)(this_))->pixel));
				};

				static TPointer<Variable> pixelGetG(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-pixel-get-g\n");
#endif

					if(!TIsType<VariablePixel>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(XYO_PIXEL32_G(((VariablePixel *)(this_))->pixel));
				};

				static TPointer<Variable> pixelGetB(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-pixel-get-b\n");
#endif
					if(!TIsType<VariablePixel>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(XYO_PIXEL32_B(((VariablePixel *)(this_))->pixel));
				};

				static TPointer<Variable> pixelGetA(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-pixel-get-a\n");
#endif

					if(!TIsType<VariablePixel>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(XYO_PIXEL32_A(((VariablePixel *)(this_))->pixel));
				};

				static TPointer<Variable> pixelSetR(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-pixel-set-r\n");
#endif

					if(!TIsType<VariablePixel>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number tmp = (arguments->index(0))->toNumber();
					if(!(isnan(tmp) || isinf(tmp) || signbit(tmp))) {
						XYO_PIXEL32_CHANGE_R(((VariablePixel *)(this_))->pixel, ((Integer)tmp));
					};

					return this_;
				};

				static TPointer<Variable> pixelSetG(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-pixel-set-g\n");
#endif

					if(!TIsType<VariablePixel>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number tmp = (arguments->index(0))->toNumber();
					if(!(isnan(tmp) || isinf(tmp) || signbit(tmp))) {
						XYO_PIXEL32_CHANGE_G(((VariablePixel *)(this_))->pixel, ((Integer)tmp));
					};

					return this_;
				};

				static TPointer<Variable> pixelSetB(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-pixel-set-b\n");
#endif

					if(!TIsType<VariablePixel>(this_)) {
						throw(Error("invalid parameter"));
					};


					Number tmp = (arguments->index(0))->toNumber();
					if(!(isnan(tmp) || isinf(tmp) || signbit(tmp))) {
						XYO_PIXEL32_CHANGE_B(((VariablePixel *)(this_))->pixel, ((Integer)tmp));
					};

					return this_;
				};

				static TPointer<Variable> pixelSetA(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-pixel-set-a\n");
#endif


					if(!TIsType<VariablePixel>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number tmp = (arguments->index(0))->toNumber();
					if(!(isnan(tmp) || isinf(tmp) || signbit(tmp))) {
						XYO_PIXEL32_CHANGE_A(((VariablePixel *)(this_))->pixel, ((Integer)tmp));
					};

					return this_;
				};

				static TPointer<Variable> pngLoad(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-png-load\n");
#endif

					TPointer<Image> image = Pixel32Process::pngLoad((char *)(((arguments->index(0))->toString()).value()));
					if(image) {
						return VariableImage::newVariable(image.value());
					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> imagePngSave(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-png-save\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableBoolean::newVariable(Pixel32Process::pngSave(((VariableImage *)(this_))->image, (char *)(((arguments->index(0))->toString()).value())));
				};

				static TPointer<Variable> imageGetWidth(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-get-width\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableImage *)(this_))->image->width);
				};

				static TPointer<Variable> imageGetHeight(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-get-height\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableImage *)(this_))->image->height);
				};

				static TPointer<Variable> imageGetPixel(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-get-pixel\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number x = (arguments->index(0))->toNumber();
					Number y = (arguments->index(1))->toNumber();
					if((!(isnan(x) || isinf(x) || signbit(x))) &&
						(!(isnan(y) || isinf(y) || signbit(y)))) {

						return VariablePixel::newVariable(
								Pixel32Process::getPixel(((VariableImage *)(this_))->image,
									(Integer)x,
									(Integer)y
								));

					};

					return VariablePixel::newVariable(0);
				};

				static TPointer<Variable> imageGetPixelX(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-get-pixel-x\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number x = (arguments->index(0))->toNumber();
					Number y = (arguments->index(1))->toNumber();
					if((!(isnan(x) || isinf(x) || signbit(x))) &&
						(!(isnan(y) || isinf(y) || signbit(y)))) {

						return VariablePixel::newVariable(
								Pixel32Process::getPixelX(((VariableImage *)(this_))->image,
									(Integer)x,
									(Integer)y
								));

					};

					return VariablePixel::newVariable(0);
				};

				static TPointer<Variable> imageSetPixel(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-set-pixel\n");
#endif


					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};


					Number x = (arguments->index(0))->toNumber();
					Number y = (arguments->index(1))->toNumber();
					if((!(isnan(x) || isinf(x) || signbit(x))) &&
						(!(isnan(y) || isinf(y) || signbit(y)))) {


						TPointerX<Variable> &pixel = arguments->index(2);
						if(TIsType<VariablePixel>(pixel)) {
							Pixel32Process::setPixel(
								((VariableImage *)(this_))->image,
								(Integer)x,
								(Integer)y,
								((VariablePixel *)(pixel.value()))->pixel
							);
						};
					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageScaleUpBicubic(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-scale-up-bicubic\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number x = (arguments->index(0))->toNumber();
					Number y = (arguments->index(1))->toNumber();
					if((!(isnan(x) || isinf(x) || signbit(x))) &&
						(!(isnan(y) || isinf(y) || signbit(y)))) {

						TPointer<Image> image = Pixel32Process::scaleUpBicubic(((VariableImage *)(this_))->image, (Integer)x, (Integer)y);
						if(image) {
							return VariableImage::newVariable(image.value());
						};

					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> imageScaleUpBilinear(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-scale-up-bilinear\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};


					Number x = (arguments->index(0))->toNumber();
					Number y = (arguments->index(1))->toNumber();
					if((!(isnan(x) || isinf(x) || signbit(x))) &&
						(!(isnan(y) || isinf(y) || signbit(y)))) {

						TPointer<Image> image = Pixel32Process::scaleUpBilinear(((VariableImage *)(this_))->image, (Integer)x, (Integer)y);
						if(image) {
							return VariableImage::newVariable(image.value());
						};


					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> imageScaleUpNearestNeighbor(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-scale-up-nearest-neighbor\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};


					Number x = (arguments->index(0))->toNumber();
					Number y = (arguments->index(1))->toNumber();
					if((!(isnan(x) || isinf(x) || signbit(x))) &&
						(!(isnan(y) || isinf(y) || signbit(y)))) {

						TPointer<Image> image = Pixel32Process::scaleUpNearestNeighbor(((VariableImage *)(this_))->image, (Integer)x, (Integer)y);
						if(image) {
							return VariableImage::newVariable(image.value());
						};


					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> imageScaleDownX2(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-scale-down-x-2\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointer<Image> image = Pixel32Process::scaleDownX2(((VariableImage *)(this_))->image);
					if(image) {
						return VariableImage::newVariable(image.value());
					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> imageScaleDownX2OnX(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-scale-down-x-2-on-x\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointer<Image> image = Pixel32Process::scaleDownX2OnX(((VariableImage *)(this_))->image);
					if(image) {
						return VariableImage::newVariable(image.value());
					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> imageScaleDownX2OnY(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-scale-down-x-2-on-y\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointer<Image> image = Pixel32Process::scaleDownX2OnY(((VariableImage *)(this_))->image);
					if(image) {
						return VariableImage::newVariable(image.value());
					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageScaleDown(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-scale-down\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};


					Number x = (arguments->index(0))->toNumber();
					Number y = (arguments->index(1))->toNumber();
					if((!(isnan(x) || isinf(x) || signbit(x))) &&
						(!(isnan(y) || isinf(y) || signbit(y)))) {

						TPointer<Image> image = Pixel32Process::scaleDown(((VariableImage *)(this_))->image, (Integer)x, (Integer)y);
						if(image) {
							return VariableImage::newVariable(image.value());
						};


					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> imageScaleUp(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-scale-up\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number x = (arguments->index(0))->toNumber();
					Number y = (arguments->index(1))->toNumber();
					if((!(isnan(x) || isinf(x) || signbit(x))) &&
						(!(isnan(y) || isinf(y) || signbit(y)))) {


						TPointer<Image> image = Pixel32Process::scaleUp(((VariableImage *)(this_))->image, (Integer)x, (Integer)y);
						if(image) {
							return VariableImage::newVariable(image.value());
						};


					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> imageResize(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-resize\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number x = (arguments->index(0))->toNumber();
					Number y = (arguments->index(1))->toNumber();
					if((!(isnan(x) || isinf(x) || signbit(x))) &&
						(!(isnan(y) || isinf(y) || signbit(y)))) {

						TPointer<Image> image = Pixel32Process::resize(((VariableImage *)(this_))->image, (Integer)x, (Integer)y);
						if(image) {
							return VariableImage::newVariable(image.value());
						};

					};

					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageCut(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-cut\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};


					Number sx = (arguments->index(0))->toNumber();
					Number sy = (arguments->index(1))->toNumber();
					Number lx = (arguments->index(2))->toNumber();
					Number ly = (arguments->index(3))->toNumber();

					if((!(isnan(sx) || isinf(sx) || signbit(sx))) &&
						(!(isnan(sy) || isinf(sy) || signbit(sy))) &&
						(!(isnan(lx) || isinf(lx) || signbit(lx))) &&
						(!(isnan(ly) || isinf(ly) || signbit(ly)))) {


						TPointer<Image> image = Pixel32Process::cut(((VariableImage *)(this_))->image,
								(Integer)sx,
								(Integer)sy,
								(Integer)lx,
								(Integer)ly
							);
						if(image) {
							return VariableImage::newVariable(image.value());
						};


					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageClear(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-clear\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &pixel = arguments->index(0);
					if(TIsType<VariablePixel>(pixel)) {

						Pixel32Process::clear(((VariableImage *)(this_))->image,
							((VariablePixel *)(pixel.value()))->pixel
						);

					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageCopy(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-copy\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number dx = (arguments->index(1))->toNumber();
					Number dy = (arguments->index(2))->toNumber();
					Number sx = (arguments->index(3))->toNumber();
					Number sy = (arguments->index(4))->toNumber();
					Number lx = (arguments->index(5))->toNumber();
					Number ly = (arguments->index(6))->toNumber();

					if((!(isnan(dx) || isinf(dx) || signbit(dx))) &&
						(!(isnan(dy) || isinf(dy) || signbit(dy))) &&
						(!(isnan(sx) || isinf(sx) || signbit(sx))) &&
						(!(isnan(sy) || isinf(sy) || signbit(sy))) &&
						(!(isnan(lx) || isinf(lx) || signbit(lx))) &&
						(!(isnan(ly) || isinf(ly) || signbit(ly)))) {


						TPointerX<Variable> &imageIn2 = arguments->index(0);
						if(TIsType<VariableImage>(imageIn2)) {

							Pixel32Process::copy(((VariableImage *)(this_))->image,
								((VariableImage *)(imageIn2.value()))->image,
								(Integer)dx,
								(Integer)dy,
								(Integer)sx,
								(Integer)sy,
								(Integer)lx,
								(Integer)ly
							);

						};

					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageWrap(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-wrap\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};


					Number lx = (arguments->index(0))->toNumber();
					Number ly = (arguments->index(1))->toNumber();
					if((!(isnan(lx) || isinf(lx) || signbit(lx))) &&
						(!(isnan(ly) || isinf(ly) || signbit(ly)))) {


						TPointer<Image> image = Pixel32Process::wrap(((VariableImage *)(this_))->image,
								(Integer)lx,
								(Integer)ly
							);
						if(image) {
							return VariableImage::newVariable(image.value());
						};

					};

					return Context::getValueUndefined();
				};

				static TPointer<Variable> imageWrapBox(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-wrap-box\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};


					Number lx = (arguments->index(0))->toNumber();
					Number ly = (arguments->index(1))->toNumber();
					if((!(isnan(lx) || isinf(lx) || signbit(lx))) &&
						(!(isnan(ly) || isinf(ly) || signbit(ly)))) {


						TPointer<Image> image = Pixel32Process::wrapBox(((VariableImage *)(this_))->image,
								(Integer)lx,
								(Integer)ly
							);
						if(image) {
							return VariableImage::newVariable(image.value());
						};

					};

					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageBlend(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-blend\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};


					Number dx = (arguments->index(1))->toNumber();
					Number dy = (arguments->index(2))->toNumber();
					Number sx = (arguments->index(3))->toNumber();
					Number sy = (arguments->index(4))->toNumber();
					Number lx = (arguments->index(5))->toNumber();
					Number ly = (arguments->index(6))->toNumber();

					if((!(isnan(dx) || isinf(dx) || signbit(dx))) &&
						(!(isnan(dy) || isinf(dy) || signbit(dy))) &&
						(!(isnan(sx) || isinf(sx) || signbit(sx))) &&
						(!(isnan(sy) || isinf(sy) || signbit(sy))) &&
						(!(isnan(lx) || isinf(lx) || signbit(lx))) &&
						(!(isnan(ly) || isinf(ly) || signbit(ly)))) {

						TPointerX<Variable> &imageIn2 = arguments->index(0);
						if(TIsType<VariableImage>(imageIn2)) {

							Pixel32Process::blend(((VariableImage *)(this_))->image,
								((VariableImage *)(imageIn2.value()))->image,
								(Integer)dx,
								(Integer)dy,
								(Integer)sx,
								(Integer)sy,
								(Integer)lx,
								(Integer)ly
							);

						};


					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageNoise(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-noise\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &random = arguments->index(0);
					if(TIsType<VariableRandom>(random)) {

						Pixel32Process::noise(((VariableImage *)(this_))->image,
							((VariableRandom *)(random.value()))->value
						);

					};
					return Context::getValueUndefined();
				};

				static TPointer<Variable> imageNoise2Bit(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-noise-2-bit\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &random = arguments->index(0);
					if(TIsType<VariableRandom>(random)) {


						Pixel32Process::noise2Bit(((VariableImage *)(this_))->image,
							((VariableRandom *)(random.value()))->value
						);

					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageDrawRectangle(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-draw-rectangle\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number sx = (arguments->index(0))->toNumber();
					Number sy = (arguments->index(1))->toNumber();
					Number lx = (arguments->index(2))->toNumber();
					Number ly = (arguments->index(3))->toNumber();

					if((!(isnan(sx) || isinf(sx))) &&
						(!(isnan(sy) || isinf(sy))) &&
						(!(isnan(lx) || isinf(lx))) &&
						(!(isnan(ly) || isinf(ly)))) {


						TPointerX<Variable> &pixel = arguments->index(4);
						if(TIsType<VariablePixel>(pixel)) {
							Pixel32Process::drawRectangle(((VariableImage *)(this_))->image,
								(Integer)sx,
								(Integer)sy,
								(Integer)lx,
								(Integer)ly,
								((VariablePixel *)(pixel.value()))->pixel
							);
						};


					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageDrawFilledRectangle(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-draw-filled-rectangle\n");
#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number sx = (arguments->index(0))->toNumber();
					Number sy = (arguments->index(1))->toNumber();
					Number lx = (arguments->index(2))->toNumber();
					Number ly = (arguments->index(3))->toNumber();

					if((!(isnan(sx) || isinf(sx))) &&
						(!(isnan(sy) || isinf(sy))) &&
						(!(isnan(lx) || isinf(lx))) &&
						(!(isnan(ly) || isinf(ly)))) {

						TPointerX<Variable> &pixel = arguments->index(4);
						if(TIsType<VariablePixel>(pixel)) {
							Pixel32Process::drawFilledRectangle(((VariableImage *)(this_))->image,
								(Integer)sx,
								(Integer)sy,
								(Integer)lx,
								(Integer)ly,
								((VariablePixel *)(pixel.value()))->pixel
							);
						};

					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageColorRescale(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-color-rescale\n");
#endif
					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Pixel32Process::colorRescale(((VariableImage *)(this_))->image);

					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageAverage(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-average\n");

#endif

					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					Number level1 = (arguments->index(1))->toNumber();
					Number level2 = (arguments->index(2))->toNumber();
					Number delta = (arguments->index(3))->toNumber();

					if((!(isnan(level1) || isinf(level1) || signbit(level1))) &&
						(!(isnan(level2) || isinf(level2) || signbit(level2))) &&
						(!(isnan(delta) || isinf(delta) || signbit(delta)))) {

						TPointerX<Variable> &imageIn2 = arguments->index(0);
						if(TIsType<VariableImage>(imageIn2)) {

							Pixel32Process::average(((VariableImage *)(this_))->image,
								((VariableImage *)(imageIn2.value()))->image,
								(Integer)level1,
								(Integer)level2,
								(Integer)delta
							);

							return this_;
						};

					};
					return Context::getValueUndefined();
				};


				static TPointer<Variable> imageKernel3X3(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-image-kernel3x3\n");

#endif
					if(!TIsType<VariableImage>(this_)) {
						throw(Error("invalid parameter"));
					};

					TPointerX<Variable> &kernel = arguments->index(0);
					if(!TIsType<VariableKernel3X3>(kernel)) {
						throw(Error("invalid parameter"));
					};

					return VariableImage::newVariable(Pixel32Process::kernel3X3(((VariableImage *)(this_))->image,
								*(((VariableKernel3X3 *)(kernel.value()))->kernel)));

				};

				static TPointer<Variable> kernel3X3GetNormalA(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-kernel3x3-get-normalA\n");
#endif
					if(!TIsType<VariableKernel3X3>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableKernel3X3 *)(this_))->kernel->normalA);
				};

				static TPointer<Variable> kernel3X3SetNormalA(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-kernel3x3-set-normalA\n");
#endif
					if(!TIsType<VariableKernel3X3>(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableKernel3X3 *)(this_))->kernel->normalA = (arguments->index(0))->toNumber();

					return this_;
				};

				static TPointer<Variable> kernel3X3GetNormalB(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-kernel3x3-get-normalB\n");
#endif
					if(!TIsType<VariableKernel3X3>(this_)) {
						throw(Error("invalid parameter"));
					};

					return VariableNumber::newVariable(((VariableKernel3X3 *)(this_))->kernel->normalB);
				};

				static TPointer<Variable> kernel3X3SetNormalB(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-kernel3x3-set-normalB\n");
#endif
					if(!TIsType<VariableKernel3X3>(this_)) {
						throw(Error("invalid parameter"));
					};

					((VariableKernel3X3 *)(this_))->kernel->normalB = (arguments->index(0))->toNumber();

					return this_;
				};

				static TPointer<Variable> kernel3X3GetMatrixV(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-kernel3x3-get-matrix-v\n");
#endif
					if(!TIsType<VariableKernel3X3>(this_)) {
						throw(Error("invalid parameter"));
					};

					size_t x = (arguments->index(0))->toIndex();
					size_t y = (arguments->index(1))->toIndex();

					if(x > 2 || y > 2) {
						return Context::getValueUndefined();
					};

					return VariableNumber::newVariable(((VariableKernel3X3 *)(this_))->kernel->v[x][y]);
				};

				static TPointer<Variable> kernel3X3SetMatrixV(VariableFunction *function, Variable *this_, VariableArray *arguments) {
#ifdef QUANTUM_SCRIPT_VM_DEBUG_RUNTIME
					printf("- pixel32-kernel3x3-set-matrix-v\n");
#endif
					if(!TIsType<VariableKernel3X3>(this_)) {
						throw(Error("invalid parameter"));
					};

					size_t x = (arguments->index(0))->toIndex();
					size_t y = (arguments->index(1))->toIndex();

					if(x > 2 || y > 2) {
						return Context::getValueUndefined();
					};

					((VariableKernel3X3 *)(this_))->kernel->v[x][y] = (arguments->index(2))->toNumber();

					return this_;
				};

				static void setKernel3X3(const char *name, Kernel3X3 &src) {
					TPointer<Kernel3X3> kernel;
					kernel.newMemory();
					kernel->copy(src);
					(((Context::getGlobalObject())->getPropertyBySymbol(Context::getSymbol("Pixel32")))->
							getPropertyBySymbol(Context::getSymbol("Kernel3X3")))->
						setPropertyBySymbol(Context::getSymbol(name),VariableKernel3X3::newVariable(kernel));
				};

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("Pixel32", initExecutive);
				};

				void initExecutive(Executive *executive, void *extensionId) {
					TMemory<VariablePixel>::initMemory();
					TMemory<VariableImage>::initMemory();

					String info = "Pixel32\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "Pixel32");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::Pixel32::Version::versionWithBuild());
#endif
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
					executive->setFunction2("Pixel32.Image.prototype.wrap(img,dx,dy)", imageWrap);
					executive->setFunction2("Pixel32.Image.prototype.wrapBox(img,dx,dy)", imageWrapBox);
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
					executive->compileStringX(extensionPixel32Source);
				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::Pixel32::initExecutive(executive, extensionId);
};
#endif

