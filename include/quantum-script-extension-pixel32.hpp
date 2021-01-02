//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_HPP
#define QUANTUM_SCRIPT_EXTENSION_PIXEL32_HPP

#ifndef QUANTUM_SCRIPT_HPP
#include "quantum-script.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32__EXPORT_HPP
#include "quantum-script-extension-pixel32--export.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_COPYRIGHT_HPP
#include "quantum-script-extension-pixel32-copyright.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_LICENSE_HPP
#include "quantum-script-extension-pixel32-license.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_VERSION_HPP
#include "quantum-script-extension-pixel32-version.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {

				using namespace Quantum::Script;

				class Pixel32Context:
					public Object {
						XYO_DISALLOW_COPY_ASSIGN_MOVE(Pixel32Context);
					public:

						Symbol symbolFunctionPixel;
						Symbol symbolFunctionImage;
						Symbol symbolFunctionKernel3X3;
						TPointerX<Prototype> prototypePixel;
						TPointerX<Prototype> prototypeImage;
						TPointerX<Prototype> prototypeKernel3X3;

						QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Pixel32Context();
				};

				QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT Pixel32Context *getContext();
				QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT void initExecutive(Executive *executive, void *extensionId);
				QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT void registerInternalExtension(Executive *executive);

			};
		};
	};
};

#endif

