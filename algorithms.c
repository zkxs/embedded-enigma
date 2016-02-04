#include "algorithms.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "util.h"

char asciiBuffer[16]; // that should be plenty
char tmpBuffer[BUFFER_SIZE]; // useful for chaining algorithms

/**
 * Take plaintext input, and encode it to Rail Fence ciphertext.
 * This algorithm can be easily reversed by swapping the indices of the
 * input and output arrays.
 */
void encodeRailFence(char input[], char output[], int size)
{
	int outIdx = 0;
	for (int i = 0; i < size; i+=4) {
		output[outIdx++] = input[i];
	}
	for (int i = 1; i < size; i+=2) {
		output[outIdx++] = input[i];
	}
	for (int i = 2; i < size; i+=4) {
		output[outIdx++] = input[i];
	}
	output[size] = '\0';
}

void decodeRailFence(char input[], char output[], int size)
{
	int outIdx = 0;
	for (int i = 0; i < size; i+=4) {
		output[i] = input[outIdx++];
	}
	for (int i = 1; i < size; i+=2) {
		output[i] = input[outIdx++];
	}
	for (int i = 2; i < size; i+=4) {
		output[i] = input[outIdx++];
	}
	output[size] = '\0';
}

void encodeSubstitution(char input[], char output[], int size)
{
	for (int i = 0; i < size; i++) {
		char number = (charToNumber(&input[i]) + 30) % 67;
		output[i] = numberToChar(&number);
	}
	output[size] = '\0';
}

void decodeSubstitution(char input[], char output[], int size)
{
	for (int i = 0; i < size; i++) {
		char number = (charToNumber(&input[i]) - 30) % 67;
		if (number < 0) number += 67;
		output[i] = numberToChar(&number);
	}
	output[size] = '\0';
}


char *asciiFormat = ", %d";
char *asciiFormatFirst = "%d";
void encodeAscii(char input[], char output[], int size)
{
	int outIdx = 0;
	int fragmentLength;
	for (int i = 0; i < size; i++) {
		if (i == 0) {
			fragmentLength = sprintf(asciiBuffer, asciiFormatFirst, input[i]);
		} else {
			fragmentLength = sprintf(asciiBuffer, asciiFormat, input[i]);
		}
		
		if (outIdx + fragmentLength >= BUFFER_SIZE) {
			break;
		} else {
			strcpy(&output[outIdx], asciiBuffer);
			outIdx += fragmentLength;
		}
		
	}
	output[outIdx] = '\0';
}

void decodeAscii(char input[], char output[], int size)
{
	int offset;
	int i; // used to store result of parseInt()
	char *inPtr = input;
	char *outPtr = output;
	// output is guaranteed to be equal to or smaller than input
	// therefore, we only need to do bounds checking on inPtr
	// and bounds checking on input is performed elsewhere, so we're good
	
	// TODO: bounds checking
	while (1) {
		offset = readSeparator(inPtr);
		if (offset == -1) {
			// we hit EOL
			*outPtr = '\0';
			return;
		} else if (offset == -2) {
			strcpy(output, "ERROR: Invalid character");
			return;
		}
		
		inPtr += offset;
		
		offset = parseInt(inPtr, &i);
		
		assert(offset); // really ought not to be zero
		
		if (offset == -1) {
			strcpy(output, "ERROR: Number too long to parse");
			return;
		}
		
		inPtr += offset;
		
		// we've got this number i now
		*(outPtr++) = intToValidChar(&i);	
	}
}

void encodeDual(char input[], char output[], int size)
{
	encodeRailFence(input, tmpBuffer, size);
	encodeSubstitution(tmpBuffer, output, size);
}

void decodeDual(char input[], char output[], int size)
{
	decodeSubstitution(input, tmpBuffer, size);
	decodeRailFence(tmpBuffer, output, size);
}
