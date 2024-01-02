// Quantum Script Extension Pixel32
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript.Extension/Pixel32/Context.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/VariablePixel.hpp>

namespace XYO::QuantumScript::Extension::Pixel32 {

	using namespace XYO::Pixel32;

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariablePixel, "{AF0B4B0F-4D52-4340-A99B-4DA3D7EB76DD}");
	const char *VariablePixel::strTypePixel = "Pixel32.Pixel";

	VariablePixel::VariablePixel() {
		XYO_DYNAMIC_TYPE_PUSH(VariablePixel);
		pixel = 0x00000000;
	};

	String VariablePixel::getVariableType() {
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
		if (TIsType<VariablePixel>(variable)) {
			return pixel == ((VariablePixel *)variable)->pixel;
		};
		return false;
	};

	Variable *VariablePixel::newVariable(Pixel pixel_) {
		VariablePixel *retV = TMemory<VariablePixel>::newMemory();
		retV->pixel = pixel_;
		return (Variable *)retV;
	};

	Variable *VariablePixel::instancePrototype() {
		return (Extension::Pixel32::getContext())->prototypePixel->prototype;
	};

};
