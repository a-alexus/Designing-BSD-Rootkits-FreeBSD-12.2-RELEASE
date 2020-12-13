#include <sys/param.h>
//#include <sys/proc.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/stat.h>
//#include <sys/uio.h>

static struct cdevsw cd_example_cdevsw = {
	.d_version = D_VERSION,
	.d_name = "cd_example"
};

/* Reference to the device in DEVFS */
static struct cdev *sdev;

static int
load(struct module *module, int cmd, void *arg) {
	int error = 0;

	switch (cmd) {
	case MOD_LOAD: {
		struct make_dev_args args;
		make_dev_args_init(&args);

		args.mda_devsw = &cd_example_cdevsw;
		args.mda_uid = UID_ROOT;
		args.mda_gid = GID_WHEEL;
		args.mda_unit = 0;
		args.mda_mode = S_IRUSR | S_IWUSR;

		error = make_dev_s(&args, &sdev, "cd_example");

		if (error)
			destroy_dev(sdev);
		else
			uprintf("Character device loaded.\n");
		break;

	} case MOD_UNLOAD:
		destroy_dev(sdev);
		uprintf("Character device unloaded.\n");
		break;

	default:
		error = EOPNOTSUPP;
		break;
	}

	return error;
}

DEV_MODULE(cd_example, load, NULL);
