//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_VARIABLEKERNEL3X3_HPP
#define QUANTUM_SCRIPT_EXTENSION_PIXEL32_VARIABLEKERNEL3X3_HPP

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_HPP
#include "quantum-script-extension-pixel32.hpp"
#endif

#ifndef XYO_PIXEL32_HPP
#include "xyo-pixel32.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {

				class VariableKernel3X3;

			};
		};
	};
};

namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::Pixel32::VariableKernel3X3>:
			public TMemoryPoolActive<Quantum::Script::Extension::Pixel32::VariableKernel3X3> {};
	};
};


namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {

				using namespace XYO;
				using namespace XYO::Pixel32;

				class VariableKernel3X3 :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableKernel3X3);
					protected:
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static const char *strTypeKernel3X3;
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static const char *typeKernel3X3Key;
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static const void *typeKernel3X3;
					public:

						TPointer<Kernel3X3> kernel;

						inline VariableKernel3X3() {
							variableType = registerType(typeKernel3X3, typeKernel3X3Key);
						};

						inline void memoryPoolActiveDestructor() {
							kernel.deleteMemory();
						};

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static Variable *newVariable(Kernel3X3 *kernel_);

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT String getType();

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Boolean toBoolean();
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT String toString();
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT bool isEqual(Variable *variable);

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Variable &operatorReference(Symbol symbolId);
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Variable *instancePrototype();

						static void initMemory();

						//
						inline static bool isVariableKernel3X3(const Variable *value) {
							if(typeKernel3X3 == nullptr) {
								typeKernel3X3 = registerType(typeKernel3X3, typeKernel3X3Key);
							};
							return (value->variableType == typeKernel3X3);
						};
						//

				};

			};
		};
	};
};

#endif

