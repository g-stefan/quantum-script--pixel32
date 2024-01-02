// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_DEPENDENCY_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

#ifndef XYO_PIXEL32_HPP
#	include <XYO/Pixel32.hpp>
#endif

// -- Export

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_INTERNAL
#	ifdef QUANTUM_SCRIPT__PIXEL32_INTERNAL
#		define XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_INTERNAL
#	endif
#endif

#ifdef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_INTERNAL
#	define XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT XYO_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT XYO_LIBRARY_IMPORT
#endif

#endif
