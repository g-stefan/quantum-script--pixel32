// Quantum Script Extension ProcessInteractive
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_CONTEXT_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_CONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Pixel32/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Pixel32 {

	class Pixel32Context : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Pixel32Context);

		public:
			Symbol symbolFunctionPixel;
			Symbol symbolFunctionImage;
			Symbol symbolFunctionKernel3X3;
			TPointerX<Prototype> prototypePixel;
			TPointerX<Prototype> prototypeImage;
			TPointerX<Prototype> prototypeKernel3X3;

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT Pixel32Context();
	};

	XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT Pixel32Context *getContext();
};

#endif
