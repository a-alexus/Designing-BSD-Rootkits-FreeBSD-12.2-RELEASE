/* Reads in a string from stdin and writes it to a character device. The same
 * string is then read from the character device and then printed to stdout.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CDEV "cd_echo_example"
#define BUFSIZE 4

static char buf[BUFSIZE + 1];

int
main(int argc, char *argv[])
{
	int kernel_fd;
	int len;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((len = strlen(argv[1])) > BUFSIZE) {
		fprintf(stderr, "ERROR: String too long\n");
		exit(EXIT_FAILURE);
	}

	if ((kernel_fd = open("/dev/" CDEV, O_RDWR)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	if (write(kernel_fd, argv[1], len + 1) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}

	if (lseek(kernel_fd, 0, SEEK_SET) == -1) {
		perror("lseek");
		exit(EXIT_FAILURE);
	}

	if (read(kernel_fd, buf, len + 1) == -1) {
		perror("read");
		exit(EXIT_FAILURE);
	}

	printf("Read '%s' from device\n", buf);

	if (close(kernel_fd) == -1) {
		perror("close");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}
