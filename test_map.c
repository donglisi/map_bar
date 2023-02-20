#include <linux/vfio.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "ioctl.h"

#define NVME_REG_VS 0x0008

int main()
{
	int map_bar, bar, val, ret;

	map_bar = open("/dev/map_bar", O_RDWR);
	bar = ioctl(map_bar, IOCTL_GET_BAR_FD);
	ret = pread(bar, &val, 4, NVME_REG_VS);
	printf("ret val %d %d\n", ret, val);
	return 0;
}
