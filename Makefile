CFLAGS=-Wall -I/usr/include -I/usr/include/lua5.1 -O3 -fomit-frame-pointer
LDFLAGS=-L/usr/lib -llua5.1 -g

all: libamaca.a example

libamaca.a: src/amaca.o
	$(AR) rcs libamaca.a src/amaca.o

example: eg/example.o libamaca.a
	$(CC) -o example eg/example.o libamaca.a $(CFLAGS) $(LDFLAGS)

src/amaca.o: src/amaca.c
	$(CC) -c -o src/amaca.o src/amaca.c $(CFLAGS)

eg/example.o: eg/example.c src/amaca.h
	$(CC) -c -o eg/example.o eg/example.c $(CFLAGS)

clean:
	$(RM) -rf example eg/*.o src/*.o *.a
