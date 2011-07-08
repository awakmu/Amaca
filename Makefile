# Amaca Makefile
# Copyright (C) 2011 Alessandro Ghedini <al3xbio@gmail.com>
# This file is released under the BSD license, see the COPYING file

LUA_PACKAGE?=lua5.1
LUA_CFLAGS?=`pkg-config --cflags $(LUA_PACKAGE)`
LUA_LDFLAGS?=`pkg-config --libs $(LUA_PACKAGE)`

OPTI?=-O3 -fomit-frame-pointer
CFLAGS?=-Wall -pedantic $(OPTI) $(LUA_CFLAGS) -fPIC
LDFLAGS?=-L. -Wl,-rpath,. $(LUA_LDFLAGS)

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
