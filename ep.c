#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define EP_INPUT_PATH	"/dev/input/"
#define EP_INPUT_NAME_SIZE	256

static int ep_select(const struct dirent *ep)
{
	if(ep->d_type != DT_CHR)
		return 0;

	if(!strstr(ep->d_name, "event"))
		return 0;

	return 1;
}

int main(int argc, char *argv[])
{
	struct dirent **eps;
	int i;
	int n;
	int ret = 0;

	n = scandir(EP_INPUT_PATH, &eps, ep_select, alphasort);
	if (!n) {
		perror ("couldn't open the directory");
		return -1;
	}

	for (i = 0; i < n; i++) {
		char path[64];
		int fd;
		char dev_name[EP_INPUT_NAME_SIZE];

		sprintf(path, EP_INPUT_PATH "%s", eps[i]->d_name);

		fd = open(path, O_RDONLY);
		if (fd < 0) {
			perror("cannot open file");
			return -1;
		}

		ret = ioctl(fd, EVIOCGNAME(EP_INPUT_NAME_SIZE), dev_name);
		if (ret < 0) {
			perror("unable to get the name");
		} else {
			printf("%s is %s\n", eps[i]->d_name, dev_name);
		}

		close(fd);
	}

	return 0;
}
