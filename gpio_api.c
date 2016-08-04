#include "gpio_api.h"

void _gpioExport(int gpio) {
	int fd;
	char buf[255];
	fd = open("/sys/class/gpio/export", O_WRONLY);
	sprintf(buf, "%d", gpio);
	write(fd, buf, strlen(buf));
	close(fd);
}

void _gpioUnexport(int gpio) {
	int fd;
	char buf[255];
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	sprintf(buf, "%d", gpio);
	write(fd, buf, strlen(buf));
	close(fd);
}

void _gpioDirection(int gpio, int direction) {	// 1 for output, 0 for input
	int fd;
	char buf[255];

	sprintf(buf, "/sys/class/gpio/gpio%d/direction", gpio);
	fd = open(buf, O_WRONLY);

	if (direction) {
		write(fd, "out", 3);
	}
	else {
		write(fd, "in", 2);
	}
	close(fd);
}

void _gpioSet(int gpio, int value) {
	int fd;
	char buf[255];

	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(buf, O_WRONLY);
	sprintf(buf, "%d", value);
	write(fd, buf, 1);
	close(fd);
}

int _gpioGet(int gpio) {
	int fd;
	char buf[255];
	char value;

	sprintf(buf, "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(buf, O_RDONLY);
	read(fd, &value, 1);
	close(fd);
	if (value == '0') {
		return 0;
	}
	else {
		return 1;
	}
}

