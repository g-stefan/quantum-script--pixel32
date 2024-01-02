// Quantum Script Extension Pixel32
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_VARIABLEIMAGE_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_VARIABLEIMAGE_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Pixel32/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Pixel32 {

	class VariableImage;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::Extension::Pixel32::VariableImage> : public TMemoryPoolActive<XYO::QuantumScript::Extension::Pixel32::VariableImage> {};
};

namespace XYO::QuantumScript::Extension::Pixel32 {

	using namespace XYO::Pixel32;

	class VariableImage : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableImage);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT, VariableImage);

		protected:
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT static const char *strTypeImage;

		public:
			TPointer<Image> image;

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT VariableImage();

			inline void memoryPoolActiveDestructor() {
				image.deleteMemory();
			};

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT static Variable *newVariable(Image *image_);

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT Boolean toBoolean();
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT String toString();
			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT bool isEqual(Variable *variable);

			XYO_QUANTUMSCRIPT_EXTENSION_PIXEL32_EXPORT Variable *instancePrototype();

			static void initMemory();
	};

};

#endif
