#include "libterm.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __linux__ 
	#include <sys/ioctl.h> // required for getTermSize()
	#include <unistd.h> // required for isatty()
	#include <termios.h> // required for disabling echo
	//#include <ctype.h>
#elif _WIN32
	#include <windows.h> // required for getTermSize()
	#include <conio.h> // required for reading characters
#else
	#error Platform not supported
#endif

#define MAX_COLS 1024   // maximal terminal columns that is treated as valid
#define MAX_ROWS 10000  // maximal terminal rows that is treated as valid
#define DEFAULT_COLS 80 // default number of terminal columns
#define DEFAULT_ROWS 25 // default number of terminal rows


#ifdef __linux__ 
	// Used to remember original terminal attributes
	struct termios saved_attributes;
#elif _WIN32
	// used to remember cursor position
	COORD cursorPosition = { .X = 0, .Y = 0 };
#endif
	
char getChar() {
	#ifdef __linux__ 
		return getchar();
	#elif _WIN32
		return getch();
	#endif
}

void echoOn()
{
	#ifdef __linux__ 
		tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
	#elif _WIN32
		// Not planning on implementing this
	#endif
}

void echoOff()
{
	#ifdef __linux__ 
		struct termios tattr;

		// Make sure stdin is a terminal.
		if (!isatty(STDIN_FILENO)) {
			fprintf(stderr, "Not a terminal.\n");
			exit(EXIT_FAILURE);
		}

		// Save the terminal attributes so we can restore them later.
		tcgetattr(STDIN_FILENO, &saved_attributes);
		atexit(echoOn);

		// Set the funny terminal modes.
		tcgetattr(STDIN_FILENO, &tattr);
		tattr.c_lflag &= ~(ICANON | ECHO); // Clear ICANON and ECHO.
		tattr.c_cc[VMIN] = 1;
		tattr.c_cc[VTIME] = 0;
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
	#elif _WIN32
		// Not planning on implementing this
	#endif
}

void clearLine()
{
	#ifdef __linux__ 
		putchar(ESC);
		putchar(CSI);
		fputs("2K", stdout);
	#elif _WIN32
		assert(0); // clearLine() not yet implemented on Windows
	#endif
}

void saveCursorPosition()
{
	#ifdef __linux__ 
		putchar(ESC);
		putchar(CSI);
		putchar('s');
	#elif _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		cursorPosition = csbi.dwCursorPosition;
	#endif
}

void restoreCursorPosition()
{
	#ifdef __linux__ 
		putchar(ESC);
		putchar(CSI);
		putchar('u');
	#elif _WIN32
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
	#endif
}

void clearScreen()
{
	#ifdef __linux__ 
		putchar(ESC);
		putchar(CSI);
		fputs("2J", stdout);
	#elif _WIN32
		system("cls"); // kludge
	#endif
		
	// reset cursor position, as some systems do not do this
	setCursorPosition(1, 1);
}

void setCursorPosition(unsigned int row, unsigned int col)
{
	#ifdef __linux__ 
		putchar(ESC);
		putchar(CSI);
		printf("%u;%uH", row, col);
	#elif _WIN32
		COORD coord;
		coord.X = col - 1;
		coord.Y = row - 1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	#endif
}

TermSize* getTermSize()
{
	
	TermSize *ts = malloc(sizeof(TermSize));
	assert(ts != NULL);
	
	#ifdef __linux__ 
		struct winsize w;
		ioctl(0, TIOCGWINSZ, &w);
		ts->columns = w.ws_col;
		ts->rows = w.ws_row;
	#elif _WIN32
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		ts->columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		ts->rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
		
		// so if the terminal is something weird, windows will return garbage
		
		// check for garbage, and correct it if necessary
		if (ts->columns <= 0 || ts->rows <= 0 || ts->columns > MAX_COLS || ts->rows > MAX_ROWS ) {
			ts->columns = DEFAULT_COLS;
			ts->rows = DEFAULT_ROWS;
		}
	#endif
	
	return ts;
}
