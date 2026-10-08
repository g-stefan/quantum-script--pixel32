// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Builds and runs test.0003 (test/test.0003.cpp + test/test.0003.js),
// with the extension from output/bin
Script.include("fabricare://test.js");

// ---

for(var k=1;k<=2;++k){
	exitIf(Shell.execute("quantum-script  --execution-time test/test.000"+k+".js"));
};

