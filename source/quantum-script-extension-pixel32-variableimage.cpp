//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-extension-pixel32.hpp"
#include "quantum-script-extension-pixel32-variableimage.hpp"

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {

				using namespace XYO;
				using namespace XYO::Pixel32;

				XYO_DYNAMIC_TYPE_IMPLEMENT(VariableImage, "{E08EA60C-268F-4D99-B5E1-F449ACDEA6A7}");
				const char *VariableImage::strTypeImage = "Pixel32.Image";

				VariableImage::VariableImage() {
					XYO_DYNAMIC_TYPE_PUSH(VariableImage);
				};

				String VariableImage::getVariableType() {
					return strTypeImage;
				};

				Boolean VariableImage::toBoolean() {
					return true;
				};

				String VariableImage::toString() {
					return strTypeImage;
				};

				bool VariableImage::isEqual(Variable *variable) {
					if (TIsType<VariableImage>(variable)) {
						return (image.value() == ((VariableImage *)variable)->image.value());
					};
					return false;
				};

				Variable *VariableImage::newVariable(Image *image_) {
					VariableImage *retV = TMemory<VariableImage>::newMemory();
					retV->image = image_;
					return (Variable *)retV;
				};

				Variable *VariableImage::instancePrototype() {
					return (Extension::Pixel32::getContext())->prototypeImage->prototype;
				};

				void VariableImage::initMemory() {
					Variable::initMemory();
					TPointer<Image>::initMemory();
				};

			};
		};
	};
};
