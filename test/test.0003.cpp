// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/Random.hpp>
#include <XYO/QuantumScript.Extension/Thread.hpp>
#include <XYO/QuantumScript.Extension/Pixel32.hpp>

using namespace XYO::QuantumScript;

// Runs test/test.0003.js against the extension just built (output/bin),
// not the one installed in the SDK

void initExecutive(Executive *executive) {
	Extension::Console::registerInternalExtension(executive);
	Extension::Random::registerInternalExtension(executive);
	Extension::Thread::registerInternalExtension(executive);
	Extension::Pixel32::registerInternalExtension(executive);
};

void test(int cmdN, char *cmdS[]) {

	const char *codeFile = "../../test/test.0003.js";

	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		ExecutiveX::includePath(Shell::getFilePath(codeFile));
		if (ExecutiveX::executeFile(codeFile)) {
			int exitCode = ExecutiveX::getExitCode();
			ExecutiveX::endProcessing();
			if (exitCode != 0) {
				throw std::runtime_error("Exit code");
			};
			return;
		};
		printf("%s\n", (ExecutiveX::getError()).value());
		printf("%s", (ExecutiveX::getStackTrace()).value());
		ExecutiveX::endProcessing();

		throw std::runtime_error("Code");
	};
};

int main(int cmdN, char *cmdS[]) {
	try {

		test(cmdN, cmdS);

		return 0;

	} catch (const std::exception &e) {
		printf("* Error: %s\n", e.what());
	} catch (...) {
		printf("* Error: Unknown\n");
	};

	return 1;
};
