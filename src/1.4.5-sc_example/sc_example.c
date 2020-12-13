/*
 * An example syscall for freeBSD. Takes a string and outputs it to the system
 * console and logging facility via printf.
 */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/proc.h>
#include <sys/sysent.h>
#include <sys/sysproto.h>

/* The syscall contains a single str argument.
 */
struct sc_example_args {
	char *str;
};

/* The system call implementation.
 * The function prototype for syscalls is defined in sys/sysent.h as:
 * 	typedef int sy_call_t(struct thread *, void *);
 */
static int
sc_example(struct thread *td, void *syscall_args)
{
	int error = 0;
	struct sc_example_args *uap;

	/* cast void* to syscall args struct pointer */
	uap = (struct sc_example_args *)syscall_args;

	char buf[4096];
	error = copyinstr(uap->str, buf, sizeof(buf), NULL);
	if (error) {
		return error;
	}

	printf("%s\n", buf);

	return error;
}

static struct sysent sc_example_sysent = {
	1,	   /* number of arguments */
	sc_example /* implementing function */
};

/* the offset in sysent[] where the system call is to be allocated */
static int offset = NO_SYSCALL;

/* event handler for the syscall */
static int
load(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd) {
	case MOD_LOAD:
		uprintf("System call loaded at offest %d.\n", offset);
		break;

	case MOD_UNLOAD:
		uprintf("System call unloaded from offset %d.\n", offset);
		break;
	default:
		error = EOPNOTSUPP;
		break;
	}

	return (error);
}

/* SYSCALL_MODULE is a wrapper around the DECLARE_MODULE macro */
SYSCALL_MODULE(sc_example, &offset, &sc_example_sysent, load, NULL);
