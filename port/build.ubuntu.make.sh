#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

ACTION=$1
if [ "$ACTION" = "" ]; then
	ACTION=make
fi

echo "-> $ACTION quantum-script-extension-pixel32"

cmdX(){
	if ! "$@" ; then
		echo "Error: $ACTION"
		exit 1
	fi
}

cmdX file-to-cs --touch=source/quantum-script-extension-pixel32.cpp --file-in=source/quantum-script-extension-pixel32.js --file-out=source/quantum-script-extension-pixel32.src --is-string --name=extensionPixel32Source
cmdX xyo-cc --mode=$ACTION @util/quantum-script-extension-pixel32.static.compile
cmdX xyo-cc --mode=$ACTION @util/quantum-script-extension-pixel32.dynamic.compile
