#define ESC '\x1B'
#define CSI '['
#define DEL '\x7F'

// struct to store the width and height of a terminal
typedef struct {
	int columns;
	int rows;
} TermSize;

/**
 * Determines terminal dimensions. Currently suports windows and linux.
 * Be sure to free the memory used by the TermSize struct!
 */
TermSize* getTermSize(void);

/**
 * Clear the screen, and move the cursor back to 0,0
 */
void clearScreen(void);

/**
 * Set cursor position (1-indexed)
 */
void setCursorPosition(unsigned int row, unsigned int col);



void echoOn(void);
void echoOff(void);
void saveCursorPosition(void);
void restoreCursorPosition(void);
void clearLine(void);
char getChar(void);
