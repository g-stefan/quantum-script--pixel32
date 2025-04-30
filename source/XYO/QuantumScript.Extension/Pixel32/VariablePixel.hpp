// Quantum Script Extension Pixel32
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_VARIABLEPIXEL_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_VARIABLEPIXEL_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Pixel32/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Pixel32 {

	class VariablePixel;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::Pixel32::VariablePixel> : public TMemoryPoolActive<XYO::QuantumScript::Extension::Pixel32::VariablePixel> {};
};

namespace XYO::QuantumScript::Extension::Pixel32 {

	using namespace XYO::Pixel32;

	class VariablePixel : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariablePixel);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT, VariablePixel);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT static const char *strTypePixel;

		public:
			Pixel pixel;

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT VariablePixel();

			inline void memoryPoolActiveConstructor() {
				pixel = 0x00000000;
			};

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT static Variable *newVariable(Pixel pixel_);

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT Boolean toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT String toString();
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT bool isEqual(Variable *variable);

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT Variable *instancePrototype();
	};

};

#endif
