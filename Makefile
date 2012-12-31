all:
	gcc -Wall camera.c -o camera -lm

debug:
	gcc -Wall -DDEBUG camera.c -o camera -lm

clean:
	rm -f camera

.PHONY: all debug clean
