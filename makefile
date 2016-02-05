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
TARGETS=decode decode.exe

# libraries required during linking
LIBS=libterm.o util.o algorithms.o
WINLIBS=libterm.lib util.lib algorithms.lib

all: $(TARGETS)


# build the executables
decode: enigma.c $(LIBS)
	$(CC) $(CFLAGS) $(LIBS) $< -o $@
decode.exe: enigma.c $(WINLIBS)
	$(WINCC) $(CFLAGS) $(WINLIBS) $< -o $@


# cross compile static libraries (the native compile is implicit)
%.lib: %.c
	$(WINCC) $(CFLAGS) -c $< -o $@


clean: 
	rm -f $(LIBS) $(WINLIBS) $(TARGETS)
