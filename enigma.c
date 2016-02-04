#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "libterm.h"
#include "util.h"
#include "algorithms.h"

#ifdef ENCODE
	#define CODE(NAME) encode ## NAME
#elif DECODE
	#define CODE(NAME) decode ## NAME
#else
	#error ENCODE or DECODE must be defined
#endif

static int isAllowedOrSpecial(const char *c)
{
	switch (*c) {
		case DEL: // allow backspace
		case ESC: // allow ANSI escapes
			return 1;
		default:
			return isAllowed(c);
	}
}

static void printBranch(char c, char *branch)
{
	printf("You have selected the %s branch.\n", branch);
	puts("Press any key to continue...");
	getChar();
}

int main(int argc, char *argv[])
{
	// disable terminal echo under linux
	echoOff();
	
	// declare variables
	char input[BUFFER_SIZE];
	char output[BUFFER_SIZE];
	int idx = 0;
	char c;
	int escaping = 0;
	int invalidBranch = 1;
	ApplyCipher applyCipher; // pointer to the function to code text
	
	#ifdef ENCODE
		puts("Welcome to the team Michael Force encoder");
	#elif DECODE
		puts("Welcome to the team Michael Force decoder");
	#endif
	puts("Please enter the number corresponding to your branch of the military:");
	puts("1: Army");
	puts("2: Navy");
	puts("3: Air Force");
	
	do {
		c = getChar();
		switch (c) {
			case '1':
				printBranch(c, "Army");
				applyCipher = CODE(RailFence);
				invalidBranch = 0;
				break;
			case '2':
				printBranch(c, "Navy");
				applyCipher = CODE(Substitution);
				invalidBranch = 0;
				break;
			case '3':
				printBranch(c, "Air Force");
				applyCipher = CODE(Ascii);
				invalidBranch = 0;
				break;
			// I could add some more 'secret' cases here
			case '*':
				printBranch(c, "Secret Service");
				applyCipher = CODE(Dual);
				invalidBranch = 0;
				break;
		}
	} while (invalidBranch);
	
	// clear screen
	clearScreen();
	
	// get terminal size
	TermSize *ts = getTermSize();
	int cols = ts->columns;
	free(ts);

	// initial printing
	printf("Input:\n\nOutput:\n");
	setCursorPosition(2, 1);

	// read characters from user
	while (((c = getChar()) != EOF) && c != '\n' && c != '\r'
		&& idx < BUFFER_SIZE - 2)
	{
		// handle funny things, such as backspaces and ANSI escape sequences
		if (escaping) {
			if (c != CSI && c >= 64 && c <= 126) {
				escaping = 0;
			}
			continue;
		} else if (!isAllowedOrSpecial(&c)) {
			continue;
		} else {
			switch (c) {
				case DEL:
					if (idx > 0) {
						idx--;
					}
					break;
				case ESC:
					escaping = 1;
					continue;
				default:
					input[idx++] = c;
			}
		}
		
		// ensure input string is null-terminated
		input[idx] = '\0';
		
		// perform the encode or decode operation
		applyCipher(input, output, idx);
		
		// if line wrapping must be accounted for
		if (c == DEL || idx % cols == 0) {
			clearScreen();
			puts("Input:");
			puts(input);
			#if !defined(_WIN32) // yay consistency
				if (idx > 0 && idx % cols == 0) {
					putchar('\n');
				}
			#endif
			puts("Output:");
			puts(output);
			// set output cursor position
			setCursorPosition(idx / cols + 2, idx % cols + 1);
		} else {
			putchar(c);
			saveCursorPosition();
			setCursorPosition(idx / cols + 4, 1);
			puts(output);
			restoreCursorPosition();
		}
	}
	
	// set final cursor position
	setCursorPosition((idx / cols) + (strlen(output) / cols) + 5, 1);


	#ifdef _WIN32
		printf("Done. Press any key to close this window.\n");
		getChar();
	#endif
	
	
	return EXIT_SUCCESS;
}