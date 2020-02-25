//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-extension-pixel32.hpp"
#include "quantum-script-extension-pixel32-variablepixel.hpp"

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {

				using namespace XYO;
				using namespace XYO::Pixel32;

				const char *VariablePixel::typePixelKey = "{AF0B4B0F-4D52-4340-A99B-4DA3D7EB76DD}";
				const void *VariablePixel::typePixel;
				const char *VariablePixel::strTypePixel = "Pixel32.Pixel";

				String VariablePixel::getType() {
					return strTypePixel;
				};

				Boolean VariablePixel::toBoolean() {
					return true;
				};

				String VariablePixel::toString() {
					char buf[32];
					sprintf(buf, "%02X%02X%02X%02X", XYO_PIXEL32_R(pixel), XYO_PIXEL32_G(pixel), XYO_PIXEL32_B(pixel), XYO_PIXEL32_A(pixel));
					return buf;
				};

				bool VariablePixel::isEqual(Variable *variable) {
					if (variable->variableType == VariablePixel::typePixel) {
						return pixel == ((VariablePixel *)variable)->pixel;
					};
					return false;
				};

				Variable *VariablePixel::newVariable(Pixel pixel_) {
					VariablePixel *retV = TMemory<VariablePixel>::newMemory();
					retV->pixel = pixel_;
					return (Variable *) retV;
				};

				Variable &VariablePixel::operatorReference(Symbol symbolId) {
					return operatorReferenceX(symbolId, (Extension::Pixel32::getContext())->prototypePixel->prototype);
				};

				Variable *VariablePixel::instancePrototype() {
					return (Extension::Pixel32::getContext())->prototypePixel->prototype;
				};


			};
		};
	};
};

