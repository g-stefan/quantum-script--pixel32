//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include "quantum-script-extension-pixel32-version.hpp"

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Pixel32 {
				namespace Version {

					static const char *version_ = "1.2.0";
					static const char *build_ = "3";
					static const char *versionWithBuild_ = "1.2.0.3";
					static const char *datetime_ = "2020-08-17 01:11:49";

					const char *version() {
						return version_;
					};
					const char *build() {
						return build_;
					};
					const char *versionWithBuild() {
						return versionWithBuild_;
					};
					const char *datetime() {
						return datetime_;
					};

				};
			};
		};
	};
};



