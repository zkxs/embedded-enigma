#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "libterm.h"
#include "util.h"
#include "algorithms.h"

static void printBranch(char c, char *branch)
{
	printf("You have selected the %s branch.\n", branch);
}

int main(int argc, char *argv[])
{
	// disable terminal echo under linux
	echoOff();
	
	// declare variables
	char c;
	int invalidBranch = 1;
	ApplyCipher applyCipher; // pointer to the function to code text
	
	puts("Welcome to the team Michael Force decoder");
	puts("Please enter the number corresponding to your branch of the military:");
	puts("1: Army");
	puts("2: Navy");
	puts("3: Air Force");
	
	do {
		c = getChar();
		switch (c) {
			case '1':
				printBranch(c, "Army");
				applyCipher = decodeDavid;
				invalidBranch = 0;
				break;
			case '2':
				printBranch(c, "Navy");
				applyCipher = decodeSubstitution;
				invalidBranch = 0;
				break;
			case '3':
				printBranch(c, "Air Force");
				applyCipher = decodeAscii;
				invalidBranch = 0;
				break;
		}
	} while (invalidBranch);

	// read characters from user
	while (((c = getChar()) != EOF) && c != '\n' && c != '\r')
	{
		if (isAllowed(&c)) {
			applyCipher(&c);
		}
	}
	
	printf("\n");


	#ifdef _WIN32
		printf("Done. Press any key to close this window.\n");
		getChar();
	#endif
	
	
	return EXIT_SUCCESS;
}