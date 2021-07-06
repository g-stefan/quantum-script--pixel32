//
// Quantum Script Extension Pixel32
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
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

					static const char *version_ = "1.7.0";
					static const char *build_ = "7";
					static const char *versionWithBuild_ = "1.7.0.7";
					static const char *datetime_ = "2021-07-06 15:11:50";

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



