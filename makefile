# cross compilers:
# i686-w64-mingw32-gcc
# x86_64-w64-mingw32-gcc

WINCC=x86_64-w64-mingw32-gcc

# other nifty flags: -Werror -Wno-unused-function

# for debugging
#CFLAGS=-g -std=c99 -Wall -Wmissing-prototypes

# for production
CFLAGS=-O3 -std=c99 -Wall -Wmissing-prototypes

# desired build targets
TARGETS=encode decode encode.exe decode.exe

# libraries required during linking
LIBS=libterm.o util.o algorithms.o
WINLIBS=libterm.lib util.lib algorithms.lib

all: $(TARGETS)


# build the executables
encode: enigma.c $(LIBS)
	$(CC) $(CFLAGS) -D ENCODE $(LIBS) $< -o $@
encode.exe: enigma.c $(WINLIBS)
	$(WINCC) $(CFLAGS) -D ENCODE $(WINLIBS) $< -o $@
decode: enigma.c $(LIBS)
	$(CC) $(CFLAGS) -D DECODE $(LIBS) $< -o $@
decode.exe: enigma.c $(WINLIBS)
	$(WINCC) $(CFLAGS) -D DECODE $(WINLIBS) $< -o $@


# cross compile static libraries (the native compile is implicit)
%.lib: %.c
	$(WINCC) $(CFLAGS) -c $< -o $@


clean: 
	rm -f $(LIBS) $(WINLIBS) $(TARGETS)
