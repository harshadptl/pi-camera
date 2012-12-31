all:
	gcc -Wall camera.c -o camera

debug:
	gcc -Wall -DDEBUG camera.c -o camera

clean:
	rm -f camera

.PHONY: all debug clean
