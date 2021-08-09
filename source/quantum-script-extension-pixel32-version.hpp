//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32_VERSION_HPP
#define QUANTUM_SCRIPT_EXTENSION_PIXEL32_VERSION_HPP

#define QUANTUM_SCRIPT_EXTENSION_PIXEL32_VERSION_ABCD                 2,1,0,20
#define QUANTUM_SCRIPT_EXTENSION_PIXEL32_VERSION_STR                 "2.1.0"
#define QUANTUM_SCRIPT_EXTENSION_PIXEL32_VERSION_STR_BUILD           "20"
#define QUANTUM_SCRIPT_EXTENSION_PIXEL32_VERSION_STR_DATETIME        "2021-08-09 17:38:06"

#ifndef XYO_RC

#ifndef QUANTUM_SCRIPT_EXTENSION_PIXEL32__EXPORT_HPP
#include "quantum-script-extension-pixel32--export.hpp"
#endif

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {
				namespace Version {
					QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT const char *version();
					QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT const char *build();
					QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT const char *versionWithBuild();
					QUANTUM_SCRIPT_EXTENSION_PIXEL32_EXPORT const char *datetime();
				};
			};
		};
	};
};

#endif
#endif

