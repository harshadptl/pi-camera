CC := clang

all:
	${CC} -Wall camera.c -o camera -lm

clean:
	rm -f camera

.PHONY: all debug clean
