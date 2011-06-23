# Amaca Makefile
# Copyright (C) 2011 Alessandro Ghedini <al3xbio@gmail.com>
# This file is released under the BSD license, see the COPYING file

OPTI?=-O3 -fomit-frame-pointer
CFLAGS?=-Wall -pedantic -I/usr/include/lua5.1 $(OPTI) -fPIC
LDFLAGS?=-L. -Wl,-rpath,. -llua5.1

DYLIB?=libamaca.so

all: example check $(DYLIB)

$(DYLIB): src/amaca.o
	$(CC) -shared -fPIC -o $(DYLIB) src/amaca.o $(CFLAGS) $(LDFLAGS)

example: eg/example.o $(DYLIB)
	$(CC) -o example eg/example.o $(CFLAGS) $(LDFLAGS) -lamaca

test: test.o $(DYLIB)
	$(CC) -o test test.o $(CFLAGS) $(LDFLAGS) -lamaca

check: test
	./test

src/amaca.o: src/amaca.c
eg/example.o: eg/example.c eg/../src/amaca.h
test.o: test.c src/amaca.h

clean:
	$(RM) -rf example test eg/*.o src/*.o *.*o
