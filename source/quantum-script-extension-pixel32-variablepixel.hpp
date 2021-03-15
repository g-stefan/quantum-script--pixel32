//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_VARIABLEPIXEL_HPP
#define QUANTUM_SCRIPT_EXTENSION_PIXEL32_VARIABLEPIXEL_HPP

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

				class VariablePixel32;

			};
		};
	};
};

namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::Pixel32::VariablePixel32>:
			public TMemoryPoolActive<Quantum::Script::Extension::Pixel32::VariablePixel32> {};
	};
};


namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {

				using namespace XYO;

				using namespace XYO::Pixel32;

				class VariablePixel :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariablePixel);
					protected:
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static const char *strTypePixel;
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static const char *typePixelKey;
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static const void *typePixel;
					public:


						Pixel pixel;

						inline VariablePixel() {
							variableType = registerType(typePixel, typePixelKey);
							pixel = 0x00000000;
						};

						inline void memoryPoolActiveConstructor() {
							pixel = 0x00000000;
						};

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static Variable *newVariable(Pixel pixel_);

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT String getType();

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Boolean toBoolean();
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT String toString();
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT bool isEqual(Variable *variable);

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Variable &operatorReference(Symbol symbolId);
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Variable *instancePrototype();

						//
						inline static bool isVariablePixel(const Variable *value) {
							if(typePixel == nullptr) {
								typePixel = registerType(typePixel, typePixelKey);
							};
							return (value->variableType == typePixel);
						};
						//

				};



			};
		};
	};
};

#endif

