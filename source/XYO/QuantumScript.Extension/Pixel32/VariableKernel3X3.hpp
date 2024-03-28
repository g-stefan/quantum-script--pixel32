// Quantum Script Extension Pixel32
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_VARIABLEKERNEL3X3_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_VARIABLEKERNEL3X3_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Pixel32/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Pixel32 {

	class VariableKernel3X3;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::Pixel32::VariableKernel3X3> : public TMemoryPoolActive<XYO::QuantumScript::Extension::Pixel32::VariableKernel3X3> {};
};

namespace XYO::QuantumScript::Extension::Pixel32 {

	using namespace XYO::Pixel32;

	class VariableKernel3X3 : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableKernel3X3);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT, VariableKernel3X3);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT static const char *strTypeKernel3X3;

		public:
			TPointer<Kernel3X3> kernel;

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT VariableKernel3X3();

			inline void memoryPoolActiveDestructor() {
				kernel.deleteMemory();
			};

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT static Variable *newVariable(Kernel3X3 *kernel_);

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT Boolean toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT String toString();
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT bool isEqual(Variable *variable);

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT Variable &operatorReference(Symbol symbolId);
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT Variable *instancePrototype();

			static void initMemory();
	};

};

#endif
