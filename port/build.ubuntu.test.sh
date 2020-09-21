#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

echo "-> test quantum-script-extension-pixel32"

cmdX(){
	if ! "$@" ; then
		echo "Error: test"
		exit 1
	fi
}

cmdX quantum-script --execution-time test/test.0001.js
cmdX quantum-script --execution-time test/test.0002.js

