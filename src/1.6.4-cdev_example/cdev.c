/* An example character that reads and writes from a buffer. */
#include <sys/types.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/stat.h>
#include <sys/uio.h>

#define BUFSIZE 4

d_open_t open;
d_close_t close;
d_read_t read;
d_write_t write;

static struct cdevsw echo_cdevsw = { .d_version = D_VERSION,
	.d_open = open,
	.d_close = close,
	.d_read = read,
	.d_write = write,
	.d_name = "cd_echo_example" };

static char buf[BUFSIZE + 1];
static int len; /* len does not include null terminator */

int
open(struct cdev *dev, int flag, int otyp, struct thread *td)
{
	uprintf("Opened device 'cd_echo_example' successfully.\n");
	return 0;
}

int
close(struct cdev *dev, int flag, int otyp, struct thread *td)
{
	uprintf("Closing device 'cd_echo_example'.\n");
	return 0;
}

int
read(struct cdev *dev, struct uio *uio, int ioflag)
{
	int error;
	size_t amt;

	/*
	 * Calculate amount of data left to be read from the device.
	 */
	int data_remaining = uio->uio_offset >= len + 1 ?
		  0 :
		  len + 1 - uio->uio_offset;

	amt = MIN(uio->uio_resid, data_remaining);

	if ((error = uiomove(buf, amt, uio)) != 0)
		uprintf("uiomove failed!\n");

	return (error);
}

/* takes in a character string and saves it to buf for later processing. */
int
write(struct cdev *dev, struct uio *uio, int ioflag)
{
	int error;
	size_t amt;

	/* Only allow writing from the beginning or appending. */
	if (uio->uio_offset != 0 && (uio->uio_offset != len))
		return (EINVAL);

	/* Reset the length for a new message. */
	if (uio->uio_offset == 0)
		len = 0;

	amt = MIN(uio->uio_resid, (BUFSIZE - len));

	error = uiomove(buf + uio->uio_offset, amt, uio);
	len = uio->uio_offset;
	buf[len] = '\0';
	if (error)
		uprintf("write failed\n");

	return error;
}

/* Reference to the device in DEVFS */
static struct cdev *echo_dev;

static int
load(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd) {
	case MOD_LOAD: {
		struct make_dev_args args;
		make_dev_args_init(&args);

		args.mda_devsw = &echo_cdevsw;
		args.mda_uid = UID_ROOT;
		args.mda_gid = GID_WHEEL;
		args.mda_unit = 0;
		args.mda_mode = S_IRUSR | S_IWUSR;

		error = make_dev_s(&args, &echo_dev, "cd_echo_example");

		if (error)
			destroy_dev(echo_dev);
		else
			uprintf("Character device loaded.\n");
		break;
	}
	case MOD_UNLOAD:
		destroy_dev(echo_dev);
		uprintf("Character device unloaded.\n");
		break;

	default:
		error = EOPNOTSUPP;
		break;
	}

	return error;
}

DEV_MODULE(cd_echo_example, load, NULL);
