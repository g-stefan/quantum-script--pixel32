//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_VARIABLEIMAGE_HPP
#define QUANTUM_SCRIPT_EXTENSION_PIXEL32_VARIABLEIMAGE_HPP

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

				class VariableImage;

			};
		};
	};
};

namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Extension::Pixel32::VariableImage>:
			public TMemoryPoolActive<Quantum::Script::Extension::Pixel32::VariableImage> {};
	};
};


namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {

				using namespace XYO;
				using namespace XYO::Pixel32;

				class VariableImage :
					public Variable {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableImage);
					protected:
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static const char *strTypeImage;
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static const char *typeImageKey;
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static const void *typeImage;
					public:

						TPointer<Image> image;

						inline VariableImage() {
							variableType = registerType(typeImage, typeImageKey);
						};

						inline void memoryPoolActiveDestructor() {
							image.deleteMemory();
						};

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT static Variable *newVariable(Image *image_);

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT String getType();

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Boolean toBoolean();
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT String toString();
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT bool isEqual(Variable *variable);

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Variable &operatorReference(Symbol symbolId);
						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Variable *instancePrototype();

						static void initMemory();

						//
						inline static bool isVariableImage(const Variable *value) {
							if(typeImage == nullptr) {
								typeImage = registerType(typeImage, typeImageKey);
							};
							return (value->variableType == typeImage);
						};
						//

				};

			};
		};
	};
};

#endif

