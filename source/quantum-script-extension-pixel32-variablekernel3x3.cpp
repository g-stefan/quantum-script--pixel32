//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-extension-pixel32.hpp"
#include "quantum-script-extension-pixel32-variablekernel3x3.hpp"

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {

				using namespace XYO;
				using namespace XYO::Pixel32;

				const char *VariableKernel3X3::typeKernel3X3Key = "{AE109A67-0402-4A3F-B0D9-2E30C0E0A4F9}";
				const void *VariableKernel3X3::typeKernel3X3;
				const char *VariableKernel3X3::strTypeKernel3X3 = "Pixel32.Kernel3X3";

				String VariableKernel3X3::getType() {
					return strTypeKernel3X3;
				};

				Boolean VariableKernel3X3::toBoolean() {
					return true;
				};

				String VariableKernel3X3::toString() {
					return strTypeKernel3X3;
				};

				bool VariableKernel3X3::isEqual(Variable *variable) {
					if (variable->variableType == VariableKernel3X3::typeKernel3X3) {
						return (kernel.value() == ((VariableKernel3X3 *)variable)->kernel.value());
					};
					return false;
				};

				Variable *VariableKernel3X3::newVariable(Kernel3X3 *kernel_) {
					VariableKernel3X3 *retV = TMemory<VariableKernel3X3>::newMemory();
					retV->kernel = kernel_;
					return (Variable *) retV;
				};

				Variable &VariableKernel3X3::operatorReference(Symbol symbolId) {
					return operatorReferenceX(symbolId, (Extension::Pixel32::getContext())->prototypeKernel3X3->prototype);
				};

				Variable *VariableKernel3X3::instancePrototype() {
					return (Extension::Pixel32::getContext())->prototypeKernel3X3->prototype;
				};

				void VariableKernel3X3::initMemory() {
					TPointer<Kernel3X3>::initMemory();
				};

			};
		};
	};
};

