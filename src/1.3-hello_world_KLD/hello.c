#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/module.h>

static int
hello_world(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd) {
	case MOD_LOAD:
		uprintf("Hello, world!\n");
		break;
	case MOD_UNLOAD:
		uprintf("Good-bye, cruel world!\n");
		break;
	default:
		error = EOPNOTSUPP;
		break;
	}

	return (error);
}

static moduledata_t hello_mod = {
	"hello",     /* module name */
	hello_world, /* event hander */
	NULL	     /* extra data */
};

DECLARE_MODULE(hello, hello_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);
