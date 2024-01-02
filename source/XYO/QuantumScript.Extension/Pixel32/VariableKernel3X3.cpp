// Quantum Script Extension Pixel32
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript.Extension/Pixel32/Context.hpp>
#include <XYO/QuantumScript.Extension/Pixel32/VariableKernel3X3.hpp>

namespace XYO::QuantumScript::Extension::Pixel32 {

	using namespace XYO::Pixel32;

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableKernel3X3, "{AE109A67-0402-4A3F-B0D9-2E30C0E0A4F9}");
	const char *VariableKernel3X3::strTypeKernel3X3 = "Pixel32.Kernel3X3";

	VariableKernel3X3::VariableKernel3X3() {
		XYO_DYNAMIC_TYPE_PUSH(VariableKernel3X3);
	};

	String VariableKernel3X3::getVariableType() {
		return strTypeKernel3X3;
	};

	Boolean VariableKernel3X3::toBoolean() {
		return true;
	};

	String VariableKernel3X3::toString() {
		return strTypeKernel3X3;
	};

	bool VariableKernel3X3::isEqual(Variable *variable) {
		if (TIsType<VariableKernel3X3>(variable)) {
			return (kernel.value() == ((VariableKernel3X3 *)variable)->kernel.value());
		};
		return false;
	};

	Variable *VariableKernel3X3::newVariable(Kernel3X3 *kernel_) {
		VariableKernel3X3 *retV = TMemory<VariableKernel3X3>::newMemory();
		retV->kernel = kernel_;
		return (Variable *)retV;
	};

	Variable *VariableKernel3X3::instancePrototype() {
		return (Extension::Pixel32::getContext())->prototypeKernel3X3->prototype;
	};

	void VariableKernel3X3::initMemory() {
		Variable::initMemory();
		TPointer<Kernel3X3>::initMemory();
	};

};
