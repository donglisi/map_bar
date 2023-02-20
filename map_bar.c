#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/anon_inodes.h>
#include <linux/file.h>

#include <asm/io.h>

#include "ioctl.h"

static void __iomem *bar;
static int major;

static ssize_t bar_fops_read(struct file *filep, char __user *buf,
				     size_t count, loff_t *ppos)
{
	int ret;

	ret = copy_to_user(buf, bar + *ppos, count);
	return count;
}

static ssize_t bar_fops_write(struct file *filep, const char __user *buf,
				      size_t count, loff_t *ppos)
{
	int ret;

	ret = copy_from_user(bar + *ppos, buf, count);
	return count;
}

static const struct file_operations bar_fops = {
	.owner		= THIS_MODULE,
	.read		= bar_fops_read,
	.write		= bar_fops_write,
};

static int ioctl_get_bar_fd(void)
{
	struct file *filep;
	int fdno;

	fdno = get_unused_fd_flags(O_CLOEXEC);
	filep = anon_inode_getfile("[map-bar]", &bar_fops, NULL, O_RDWR);
	filep->f_mode |= (FMODE_PREAD | FMODE_PWRITE);
	fd_install(fdno, filep);

	return fdno;
}

static long bar_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
		case IOCTL_GET_BAR_FD:
			return ioctl_get_bar_fd();
		default:
			return 0;
	}
}

static const struct file_operations ioctl_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= bar_ioctl
};

int init_module(void)
{
	major = register_chrdev(0, "map_bar", &ioctl_fops);
	bar = ioremap(0xfce00000, 1024 * 16);

	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(major, "map_bar");
	iounmap(bar);
}

MODULE_LICENSE("GPL");
