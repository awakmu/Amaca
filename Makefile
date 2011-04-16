CFLAGS=-Wall -I/usr/include -I/usr/include/lua5.1 -O3 -fomit-frame-pointer
LDFLAGS=-L/usr/lib -llua5.1 -g

all: example

example: eg/example.o src/amaca.o
	$(CC) -o example eg/example.o src/amaca.o $(CFLAGS) $(LDFLAGS)

src/amaca.o: src/amaca.c
	$(CC) -c -o src/amaca.o src/amaca.c $(CFLAGS)

eg/example.o: eg/example.c src/amaca.h
	$(CC) -c -o eg/example.o eg/example.c $(CFLAGS)

clean:
	rm -rf example eg/*.o src/*.o
