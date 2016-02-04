typedef void (*ApplyCipher)(char input[], char output[], int size);

void encodeRailFence(char input[], char output[], int size);
void decodeRailFence(char input[], char output[], int size);
void encodeSubstitution(char input[], char output[], int size);
void decodeSubstitution(char input[], char output[], int size);
void encodeAscii(char input[], char output[], int size);
void decodeAscii(char input[], char output[], int size);
void encodeDual(char input[], char output[], int size);
void decodeDual(char input[], char output[], int size);
