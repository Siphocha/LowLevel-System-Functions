CC=gcc
CFLAGS=-Wall -Wextra -fPIC
LDFLAGS=-ldl

# Targets
all: libdiary.so diary_manager

libdiary.so: diary.o
	$(CC) -shared -o libdiary.so diary.o

diary.o: diary.c diary.h
	$(CC) $(CFLAGS) -c diary.c

diary_manager: main.c libdiary.so
	$(CC) $(CFLAGS) -o diary_manager main.c -L. -ldiary

clean:
	rm -f *.o *.so diary_manager

install:
	cp libdiary.so /usr/local/lib/
	cp diary.h /usr/local/include/

test: diary_manager
	LD_LIBRARY_PATH=. ./diary_manager

.PHONY: all clean install test