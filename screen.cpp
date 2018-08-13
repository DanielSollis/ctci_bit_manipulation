#include "screen.h"
#include <iostream>
#include <bitset>


Screen::Screen() {
	bytes = { 0, 0, 0, 0,
			  0, 0, 0, 0};
	width = 2;
	height = 4;
}


Screen::Screen(int width, int height) {
	this->width = width;
	this->height = height;
	for (int i = 0; i < width * height; ++i) {
		bytes.push_back(0);
	}
}


void Screen::drawLine(int x1, int x2, int y) {
	int bitsInByte = 8;
	int firstByteAtY = (y - 1) * width;
	bitRange ranges = getBitRange(x1, x2, firstByteAtY);
	int startBitMask = pow(2, bitsInByte - ranges.startBit + 1) - 1;
	int endBitMask = pow(2, bitsInByte - ranges.endBit) - 1;
	endBitMask = ~endBitMask;

	if (ranges.startByte == ranges.endByte) {
		int mask = endBitMask & startBitMask;
		bytes[ranges.startByte] |= mask;
	}
	else {
		setBitsInDeffirentBytes(ranges, startBitMask, endBitMask);
	}
}


Screen::bitRange Screen::getBitRange(int x1, int x2, int firstByte) {
	int startByte = x1 / 8 + firstByte;
	int endByte = x2 / 8 + firstByte;
	int startBit = x1 % 8;
	int endBit = x2 % 8;
	bitRange result = { startByte, endByte, startBit, endBit };
	return result;
}


void Screen::setBitsInDeffirentBytes(bitRange range, int startBitMask, int endBitMask) {
	bytes[range.startByte] |= startBitMask;
	bytes[range.endByte] |= endBitMask;
	for (int i = range.startByte + 1; i < range.endByte; ++i) {
		bytes[i] = ~0;
	}
}


void Screen::printScreen() {
	int iterator = 0;
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			std::bitset<8> currentByte(bytes[iterator]);
			std::cout << currentByte;
			++iterator;
		}
		std::cout << std::endl;
	}
}