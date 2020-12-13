/* Calls the syscall sc_example written in ./sc_example.c */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/syscall.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	int syscall_num;
	struct module_stat stat;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int modid;
	/* "sys/" is prepended to the name passed in to the SYSCALL_MODULE macro
	 * in /usr/src/sys/sys/sysent.h
	 */
	if ((modid = modfind("sys/sc_example")) == -1) {
		perror("modfind");
		exit(EXIT_FAILURE);
	}

	stat.version = sizeof(stat);
	if (modstat(modid, &stat) == -1) {
		perror("modstat");
		exit(EXIT_FAILURE);
	}

	int syscall_number = stat.data.intval;

	syscall(syscall_number, argv[1]);
	return EXIT_SUCCESS;
}
