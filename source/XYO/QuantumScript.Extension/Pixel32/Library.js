// Quantum Script Extension Pixel32
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

Script.requireExtension("Random");

Pixel32.Image.prototype.wrapBox1Resize = function(lx, ly, fz, k) {
	var ddx, ddy, imgTmp1, imgTmp2, imgOut;
	imgTmp1 = this.wrapBox(1, 1);
	imgTmp2 = imgTmp1.resize(lx + 2 * fz, ly + 2 * fz);
	return imgTmp2.cut(fz, fz, lx + fz, ly + fz).resize(lx, ly);
};

Pixel32.perlinNoiseWrapBox = function(lx, ly, freqAndSum, rnd) {
	var m, k, nlx, nly;
	var imgTmp;
	var imgList;
	m = freqAndSum.length;
	imgList = [];
	for (k = 0; k < m; ++k) {
		nlx = lx / freqAndSum[k][0];
		nly = ly / freqAndSum[k][0];
		if ((nlx < 1) || (nly < 1)) {
			continue;
		};
		imgTmp = new Pixel32.Image(nlx, nly);
		imgTmp.noise(rnd);
		imgTmp.colorRescale();
		imgList[m - k - 1] = imgTmp.wrapBox1Resize(lx, ly, freqAndSum[k][0], k);
	};
	imgTmp = imgList[0];
	for (k = 1; k < m; ++k) {
		imgTmp.average(imgList[k], freqAndSum[k][1], freqAndSum[k][2], freqAndSum[k][1] + freqAndSum[k][2]);
	};
	return imgTmp;
};

Pixel32.perlinNoise2BitWrapBox = function(lx, ly, freqAndSum, rnd) {
	var m, k, nlx, nly;
	var imgTmp;
	var imgList;
	m = freqAndSum.length;
	imgList = [];
	for (k = 0; k < m; ++k) {
		nlx = lx / freqAndSum[k][0];
		nly = ly / freqAndSum[k][0];
		if ((nlx < 1) || (nly < 1)) {
			continue;
		};
		imgTmp = new Pixel32.Image(nlx, nly);
		imgTmp.noise2Bit(rnd);
		imgTmp.colorRescale();
		imgList[m - k - 1] = imgTmp.wrapBox1Resize(lx, ly, freqAndSum[k][0], k);
	};
	imgTmp = imgList[0];
	for (k = 1; k < m; ++k) {
		imgTmp.average(imgList[k], freqAndSum[k][1], freqAndSum[k][2], freqAndSum[k][1] + freqAndSum[k][2]);
	};
	return imgTmp;
};
