// Quantum Script Extension Pixel32
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_LIBRARY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_LIBRARY_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Pixel32/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Pixel32 {

	XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT void initExecutive(Executive *executive, void *extensionId);
	XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT void registerInternalExtension(Executive *executive);

};

#endif