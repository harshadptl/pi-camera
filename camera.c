#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

// Number of digits in the number.
#define FNAME_DIGITS  5

// Hackish bullcrap to get FNAME_DIGITS as a string when necessary.
#define STRING(x) #x
#define MACRO_STRING(x) STRING(x)

// capture and fname_length are set to their initial values in initialize()
int capture = 0;

// When given the number, return the full filaname.
char *number2filename(int number)
{
	// Magic number explanation:
	//   14 = <length of capture_> + <length of .jpeg> + 1 for the null byte

	char *name = (char*)malloc(14 + FNAME_DIGITS);

	snprintf(name, 14 + FNAME_DIGITS, "capture_%0" MACRO_STRING(FNAME_DIGITS) "d.jpeg", number);

	return name;
}

// Iterate until we find a usable file number.
void initialize()
{
//  while (File.exist?(number2filename capture) {
//    capture += 1;
//  }
}

// Capture a still image
void capture_still()
{
	puts("meep");
	printf("ffmpeg -f video4linux2 -i /dev/video0 -v:f 1 \"%s\"", number2filename(capture));
	
	capture += 1;
}

void camera() {
	/* This should probably chdir somewhere safe as well.
	   and set the umask. But this does get the process in
	   the right place in the tree. It could also execve
	   something instead of looping. */

	initialize();

	// Insert magic to call capture_still() on each SIGUSR1
	printf("%s\n%s\n%s\n%s\n", number2filename(0), number2filename(1), number2filename(12), number2filename(12345));

	while (1)
		sleep(UINT_MAX);
}

int main(void)
{
#ifndef DEBUG
	int r;
	r = fork();
	if (r == 0) {
		close(0);
		close(1);
		close(2);
		setsid();
#endif

		camera();

#ifndef DEBUG
	} else if (r == -1) {
		fprintf(stderr, "Could not fork: %s\n", strerror(errno));
		exit(-1);
	} else {
		printf("%d\n", r);
		exit(0);
	}
#endif

	return 0;
}

