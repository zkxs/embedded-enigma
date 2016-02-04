#include "util.h"
#include <stdio.h>
#include <assert.h>

int isAllowed(const char *c)
{
	if (*c >= 'a' && *c <= 'z') {
		return 1;
	} else if (*c >= 'A' && *c <= 'Z') {
		return 1;
	} else if (*c >= '0' && *c <= '9') {
		return 1;
	} else {
		switch (*c) {
			case ',':
			case '.':
			case '?':
			case '!':
			case ' ':
				return 1;
			default:
				return 0;
		}
	}
}


char charToNumber(const char *c)
{
	if (*c >= 'a' && *c <= 'z') {
		return *c - 'a';
	} else if (*c >= 'A' && *c <= 'Z') {
		return *c - 'A' + 26;
	} else if (*c >= '0' && *c <= '9') {
		return *c - '0' + 52;
	} else {
		switch (*c) {
			case ',':
				return 62;
			case '.':
				return 63;
			case '?':
				return 64;
			case '!':
				return 65;
			case ' ':
				return 66;
			default:
				assert(0); // invalid character
				return -1;
		}
	}
}


char numberToChar(const char *c)
{
	if (*c >= 0 && *c < 26) {
		return *c + 'a';
	} else if (*c >= 26 && *c < 52) {
		return *c - 26 + 'A';
	} else if (*c >= 52 && *c < 62) {
		return *c - 52 + '0';
	} else {
		switch (*c) {
			case 62:
				return ',';
			case 63:
				return '.';
			case 64:
				return '?';
			case 65:
				return '!';
			case 66:
				return ' ';
			default:
				printf("%d is not valid\n", *c);
				assert(0); // invalid character
				return -1;
		}
	}
}


/**
 * returns length of string that became that int.
 * maximal supported length of intstring is 9 characters
 */
int parseInt(const char *string, int *integer)
{
	const char *c;
	c = string;
	
	while (1) {
		if (*c >= '0' && *c <= '9') {
		// got a numeric char
			c++;
		} else {
			// c is currently 1 after the last digit
			int len = c - string; // calculate length
			
			if (len > 9) {
				// too long
				return -1;
			} else if (len > 0) {
				// looks ok
				
				*integer = 0;
				int place = 1; // decimal place of the next digit
				
				// iterate backwards over numeric characters
				for (c -= 1; c >= string; c--) {
					*integer += (*c - '0') * place;
					place *= 10;
				}
			}
			return len;
		}
	}
}


/**
 * Read a comma and space separator
 * returns it's length, or -1 if hit EOL or -2 if
 * invalid characters
 */
int readSeparator(const char *string)
{
	int commaSeen = 0;
	const char *c = string;
	
	while (1) {
		if (*c == '\0') {
			return -1;
		} else if (*c >= '0' && *c <= '9') {
			return c - string;
		}
		else if (*c == ' ') {
			c++;
		} else if (*c == ',') {
			if (commaSeen) {
				// extra comma
				return -2;
			} else {
				c++;
			}
		} else {
			// invalid character
			return -2;
		}
	}
}


char intToValidChar(const int *i)
{
	if (*i > 127 || *i < 0) {
		// clearly not ASCII
		return '~';
	} else {
		char c = (char) *i;
		
		if (isAllowed(&c)) {
			return c;
		} else {
			return '~';
		}
	}
}
