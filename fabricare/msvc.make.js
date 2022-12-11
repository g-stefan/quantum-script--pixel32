// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

runInPath("source/XYO/QuantumScript.Extension/Pixel32",function(){
	exitIf(fileToCS("--touch=Library.cpp","--file-in=Library.js","--file-out=Library.Source.cpp","--is-string","--name=librarySource"));
});

if(!Fabricare.include("msvc.make."+Project.make)){
	messageError("Don't know how to make '"+Project.make+"'!");
	exit(1);
};
