#include "algorithms.h"
#include <stdio.h>
#include "util.h"

static char davidMap(char *c) {
	switch (*c) {
		case 'a': return 'z';
		case 'A': return 'Z';
		case '0': return '9';
		case '.': return ',';
		case '?': return '.';
		case '!': return '?';
		case ' ': return '!';
		case ',': return ' ';
		default: return *c - 1;
	}
}

void decodeDavid(char *c)
{
	putchar(davidMap(c));
}


void decodeSubstitution(char *c)
{
	char number = (charToNumber(c) - 30) % 67;
	if (number < 0) number += 67;
	putchar(numberToChar(&number));
}

int asciiValue = 0;
char tmpChar;
void decodeAscii(char *c)
{
	if (*c == ',') {
		tmpChar = (char) asciiValue;
		if (isAllowed(&tmpChar)) {
			putchar(asciiValue);
		}
		asciiValue = 0;
	} else if (*c >= '0' && *c <= '9') {
		asciiValue *= 10;
		asciiValue += *c - '0';
	}
}


