#pragma once
#include <vector>

class Screen {
private:
	std::vector<char> bytes;
	int width;
	int height;

	struct bitRange {
		int startByte;
		int endByte;
		int startBit;
		int endBit;
	};

	bitRange getBitRange(int x1, int x2, int firstByte);
	void setBitsInDeffirentBytes(bitRange range, int startBitMask, int endBitMask);

public:
	Screen();
	Screen(int width, int height);
	void drawLine(int x1, int x2, int y);
	void printScreen();
};
