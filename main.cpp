#include <iostream>
#include <bitset>
#include <unordered_set>
#include "screen.h";

#pragma region PROTOTYPES


void printBitSet(int x);
bool getBit(int num, int position);
int setBit(int num, int position);
int flipBit(int num, int position);
int clearBit(int num, int position);
int updateBit(int num, int position, bool isFlagged);

std::bitset<32> insertion(int bigger, int smaller, int end, int start);
bool checkSizes(int toCheck, int space);
std::bitset<32> insertSmaller(int smaller, int bigger, int start, int end);

void binaryToString(double num);
std::vector<bool> getBinaryRepresentation(double num);
std::vector<bool> getNextBinaryValue(double & outNum, std::vector<bool> binary);
bool checkBinaryOverflow(std::vector<bool> binaryRepresentation);
void printBinaryRepresentation(std::vector<bool> binaryRepresentation);

int flipBitToWin(int num);
int getFirstSetBit(int num);
void addNextSequence(int & num, std::vector<int> & sequences);
int getBitSequenceLength(int num, bool isSet);
int getBestSequenceExceptEnd(std::vector<int> sequences);
void checkLastFlaggedSequence(std::vector<int> bitSequences, int & bestSequenceLength);

void nextNumber(int num);
int getNextNumWithSameOneCount(int num);
int getFirstSetBitBeforeZero(int num);
void AssignIfLarger(int & currentBest, int toCheck);
int getNumberOfTrailingOnes(int num, int trailingOnePosition);
int getPrevNumWithSameOneCount(int num);
bool checkIfPrevExists(int num);
int getFirstTrailingZero(int num);
int getLeftOnesMask(int position, int TrailingOnes);

int conversion(int first, int second);

int pairwiseSwap(int num);

#pragma endregion

void main() {

}


void printBitSet(int x) {
	std::bitset<32> bits(x);
	std::cout << bits << std::endl;
}


int getFirstSetBit(int num) {
	int position = 1;
	while (!(num & 1)) {
		num >>= 1;
		++position;
	}
	return position;
}


#pragma region GET/SET/FLIP/CLEAR/UPDATE


bool getBit(int num, int position) {
	return (num & (1 << position)) != 0;
}


int setBit(int num, int position) {
	return num | (1 << position);
}


int flipBit(int num, int position) {
	bool isFlagged = (num & (1 << position));
	return 0;
}


int clearBit(int num, int position) {
	return 0;
}


int updateBit(int num, int position, bool isFlagged) {
	int mask = ~(1 << position);
	int bit = isFlagged ? 1 : 0;
	(num &= mask) |= (bit << position);
	return num;
}


#pragma endregion
#pragma region INSERTION


std::bitset<32> insertion(int bigger, int smaller, int end, int start) {
	int givenSpace = start - end;
	std::bitset<32> result(0);
	if (!checkSizes(smaller, givenSpace)) {
		return result;
	}
	result = insertSmaller(smaller, bigger, start, end);
	return result;
}


bool checkSizes(int smaller, int space) {
	int neededSpaceCounter = 32;
	while (smaller <<= 1) {
		neededSpaceCounter--;
	}
	if (neededSpaceCounter >= space) {
		return true;
	}
	return false;
}


std::bitset<32> insertSmaller(int smaller, int bigger, int start, int end) {
	int onesFromStart = pow(2, start) - 1;
	int onesFromEnd = pow(2, end) - 1;
	int mask = ~(onesFromStart - onesFromEnd);
	bigger &= mask;
	bigger |= (smaller << end);
	return bigger;
}


#pragma endregion
#pragma region BINARY TO STRING


void binaryToString(double num) {
	if (num <= 0 || num >= 1) {
		std::cout << "ERROR" << std::endl;
	}
	std::vector<bool> binaryRepresentation = getBinaryRepresentation(num);
	if (binaryRepresentation.size() == 0) {
		std::cout << "ERROR" << std::endl;
	}
	else {
		printBinaryRepresentation(binaryRepresentation);
	}
}


std::vector<bool> getBinaryRepresentation(double num) {
	std::vector<bool> binaryRepresentation;
	bool binaryOverflow = false;
	while (!binaryOverflow && num != 0) {
		binaryRepresentation = getNextBinaryValue(num, binaryRepresentation);
		binaryOverflow = checkBinaryOverflow(binaryRepresentation);
	}
	std::vector<bool> error = {};
	return binaryOverflow ? error : binaryRepresentation;
}


 std::vector<bool> getNextBinaryValue(double & outNum, std::vector<bool> binary) {
	 outNum *= 2;
	 if (outNum >= 1) {
		 outNum -= 1;
		 binary.push_back(1);
	 }
	 else {
		 binary.push_back(0);
	 }
	 return binary;
}


bool checkBinaryOverflow(std::vector<bool> binaryRepresentation) {
	if (binaryRepresentation.size() > 32) {
		return true;
	}
	return false;
}


void printBinaryRepresentation(std::vector<bool> binaryRepresentation) {
	std::cout << 0 << ".";
	for (auto bin : binaryRepresentation) {
		bin ? std::cout << 1 : std::cout << 0;
	}
	std::cout << std::endl;
}


#pragma endregion
#pragma region FLIP BIT TO WIN


int flipBitToWin(int num) {
	std::vector<int> bitSequences;
	int position = getFirstSetBit(num);
	num >>= position - 1;
	while(num > 0) {
		addNextSequence(num, bitSequences);
	}
	if (bitSequences.size() < 3) {
		return bitSequences[0];
	}
	int bestSequenceLength = getBestSequenceExceptEnd(bitSequences);
	checkLastFlaggedSequence(bitSequences, bestSequenceLength);
	return bestSequenceLength;
}


void addNextSequence(int & num, std::vector<int> & bitSequences) {
	bool bitIsSet = num & 1;
	int sequenceLength = getBitSequenceLength(num, bitIsSet);
	num >>= sequenceLength;
	bitSequences.push_back(sequenceLength);
}


int getBitSequenceLength(int num, bool isSet) {
	int sameValueCount = 0;
	while (getBit(num, 0) == isSet) {
		sameValueCount++;
		num >>= 1;
	}
	return sameValueCount;
}


int getBestSequenceExceptEnd(std::vector<int> bitSequences) {
	int bestSequenceLength = 0;
	for (int i = 0; i < bitSequences.size() - 2; i = i + 2) {
		if (bitSequences[i + 1] == 1) {
			int possibleFlipLength = (bitSequences[i] + bitSequences[i + 2]) + 1;
			AssignIfLarger(bestSequenceLength, possibleFlipLength);
		}
		else {
			int standAloneSequencePlusFlip = bitSequences[i] + 1;
			AssignIfLarger(bestSequenceLength, standAloneSequencePlusFlip);
		}
	}
	return bestSequenceLength;
}


void checkLastFlaggedSequence(std::vector<int> bitSequences, int & bestSequenceLength) {
	if (bitSequences.size() % 2 == 0) {
		int secondToLast = bitSequences[bitSequences.size() - 2];
		AssignIfLarger(bestSequenceLength, secondToLast);
	}
	else {
		int last = bitSequences[bitSequences.size() - 1];
		AssignIfLarger(bestSequenceLength, last);
	}
}


void AssignIfLarger(int & currentBest, int toCheck) {
	if (toCheck > currentBest) {
		currentBest = toCheck;
	}
}


#pragma endregion
#pragma region NEXT NUMBER


void nextNumber(int num) {
	int nextNum = getNextNumWithSameOneCount(num);
	std::cout << "Next number: " << nextNum << std::endl;
	int prevNum = getPrevNumWithSameOneCount(num);
	std::cout << "Previous number: " << prevNum << std::endl;
}


int getFirstSetBitBeforeZero(int num) {
	int position = getFirstSetBit(num);
	num >>= position - 1;
	while (num >> 1 & 1) {
		num >>= 1;
		++position;
	}
	return position;
}


int getNumberOfTrailingOnes(int num, int trailingOnePosition) {
	int numberOfOnes = 0;
	for (int i = 1; i != trailingOnePosition; ++i) {
		if (num & 1) {
			++numberOfOnes;
		}
		num >>= 1;
	}
	return numberOfOnes;
}


int getPrevNumWithSameOneCount(int num) {
	if (!checkIfPrevExists(num)) {
		return 0;
	}
	int firstTrailingZero = getFirstTrailingZero(num);
	int numberOfTrailingOnes = getNumberOfTrailingOnes(num, firstTrailingZero);
	int flipOneAfterTrailingZeroMask = ~(1 << firstTrailingZero);
	int clearRightZeroes = ~0 - pow(2, firstTrailingZero) - 1;
	int pushRemainingOnesLeftMask = getLeftOnesMask(firstTrailingZero, numberOfTrailingOnes);

	num &= flipOneAfterTrailingZeroMask;
	num &= clearRightZeroes;
	num |= pushRemainingOnesLeftMask;
	return num;
}


bool checkIfPrevExists(int num) {
	while (num & 1) {
		num >>= 1;
	}
	return num != 0;
}


int getFirstTrailingZero(int num) {
	int position = 1;
	while (num & 1) {
		num >>= 1;
		++position;
	}
	while (!(num >> 1 & 1)) {
		num >>= 1;
		++position;
	}
	return position;
}


int getLeftOnesMask(int position, int TrailingOnes) {
	int num = 0;
	for (int i = 0; i < TrailingOnes + 1; ++i) {
		num <<= 1;
		num += 1;
	}
	num <<= position - TrailingOnes - 1;
	return num;
};


#pragma endregion
#pragma region CONVERSION


int conversion(int first, int second) {
	int xor = first ^ second;
	int bitsDifferent = 0;
	while (xor != 0) {
		if (xor & 1) {
			++bitsDifferent;
		}
		xor >>= 1;
	}
	return bitsDifferent;
}


#pragma endregion
#pragma region PAIRWISE SWAP


int pairwiseSwap(int num) {
	return (0xAAAAAAAA & num) >> 1 | (0x55555555 & num) << 1;
}


#pragma endregion