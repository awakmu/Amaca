CFLAGS=-Wall -I/usr/include -I/usr/include/lua5.1 -O3 -fomit-frame-pointer -fPIC
LDFLAGS=-L/usr/lib -llua5.1 -g

all: example shared

shared: src/amaca.o
	$(CC) -shared -fPIC -o libamaca.so src/amaca.o $(CFLAGS) $(LDFLAGS)

example: eg/example.o src/amaca.o
	$(CC) -o example eg/example.o src/amaca.o $(CFLAGS) $(LDFLAGS)

example-shared:
	$(CC) -o example eg/example.o -L. -lamaca $(CFLAGS) $(LDFLAGS)

src/amaca.o: src/amaca.c
	$(CC) -c -o src/amaca.o src/amaca.c $(CFLAGS)

eg/example.o: eg/example.c src/amaca.h
	$(CC) -c -o eg/example.o eg/example.c $(CFLAGS)

clean:
	$(RM) -rf example eg/*.o src/*.o *.so
