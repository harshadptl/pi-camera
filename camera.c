#include "camera.h"

// capture_number is set to their initial values in initialize()
int capture_number = 0;

// When given the number, return the full filaname.
char *number2filename(char *name, int number)
{
	// Magic number explanation:
	//   14 = <length of capture_> + <length of .jpeg> + 1 for the null byte

	//char *name = (char*)malloc(14 + FNAME_DIGITS);

	snprintf(name, 14 + FNAME_DIGITS, "capture_%0" MACRO_STRING(FNAME_DIGITS) "d.jpeg", number);

	return name;
}

// Iterate until we find a usable file number.
void initialize()
{
	struct stat buf;
	int i;
	char *filename;
	char *number = malloc(FNAME_DIGITS + 2); // FNAME_DIGITS + 1 digit + NULL byte
	char *name = malloc(FNAME_LENGTH);

	printf("Finding last file");

	capture_number = -1;

	do {
		capture_number++;

		sprintf(number, "%d", capture_number);

		filename = number2filename(name, capture_number);
		i = stat(filename, &buf);
		if (strlen(number) > FNAME_DIGITS) {
			// We should never get here...
			error(1, 0, "%d is longer than FNAME_DIGITS (%d).", capture_number, FNAME_DIGITS);
		}

		if (i != 0) { // If the does not exist exists...
			break;
		}
		putc('.', stdout);
	} while (1);
	printf(".\nDone: %s\n\n", number2filename(name, capture_number));
	free(name);
}

// Set up signals
void signal_setup()
{
	struct sigaction action;

	action.sa_handler = capture;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	sigaction(SIGUSR1, &action, NULL);
}

// Capture a still image
void capture_still()
{
	char *str  = malloc(255),
	     *name = malloc(FNAME_LENGTH);

	printf("Capturing %s.\n", number2filename(name, capture_number));

	sprintf(str, "ffmpeg -f video4linux2 -i /dev/video0 -v:f 1 %s", number2filename(name, capture_number));
	puts(str);
	system(str);

	free(name);
	free(str);

	capture_number++;
}

// Capture an image, video, etc..
void capture()
{
	capture_still(); // We can add logic if we ever get a capture_video()
}

void camera() {
	/* This should probably chdir somewhere safe as well.
	   and set the umask. But this does get the process in
	   the right place in the tree. It could also execve
	   something instead of looping. */

	signal_setup();

	while (1)
		sleep(UINT_MAX);
}

int main(int argc, const char **argv)
{
	if ((argc > 1) && ((strncmp(argv[1], "-help", 5) == 0) || (strncmp(argv[1], "--help", 6) == 0) || (strncmp(argv[1], "-h", 2) == 0))) {
		puts("Usage: camera [ARG ...]\n"
		     "\n"
		     "-nofork  Don't fork.\n"
		     "-help    This.");
		return 0;
	}

	initialize();

	if ((argc > 1) && (strncmp(argv[1], "-nofork", 7) == 0)) {
		camera();
		return 0;
	}

	int r;
	r = fork();
	if (r == 0) {
		close(0);
		close(1);
		close(2);
		setsid();

		camera();
	} else if (r == -1) {
		fprintf(stderr, "Could not fork: %s\n", strerror(errno));
		exit(-1);
	} else {
		printf("%d\n", r);
		exit(0);
	}

	return 0;
}

