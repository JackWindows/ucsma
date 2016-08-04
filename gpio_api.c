#include "gpio_api.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#define GPIO_ADDR 0x18040000	// base address
#define GPIO_BLOCK 48	// memory block size

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

volatile unsigned long *gpio_address;

int gpioSetup() {
	int m_mfd;
	if ((m_mfd = open("/dev/mem", O_RDWR )) < 0 ) {
		return -1;
	}
	gpio_address = (unsigned long*)mmap(NULL, GPIO_BLOCK, PROT_READ|PROT_WRITE, MAP_SHARED, m_mfd, GPIO_ADDR);
	close(m_mfd);

	if (gpio_address == MAP_FAILED) {
		return -2;
	}

	return 0;
}

void gpioDirection(int gpio, int direction) {
	unsigned long value = *(gpio_address + 0);	//obtain current settings
	if (direction == 1) {
		value |= (1 << gpio);	//set bit to 1
	}
	else {
		value &= ~(1 << gpio);	//clear bit
	}
	*(gpio_address + 0) = value;
}

void gpioSet(int gpio, int target_value) {
	unsigned long value = *(gpio_address + 2);	//obtain current settings
	if (target_value == 1) {
		value |= (1 << gpio);	//set bit to 1
	}
	else {
		value &= ~(1 << gpio);	//clear bit
	}
	*(gpio_address + 2) = value;
}

void gpioSet2(int gpio, int target_value) {	//another way to set gpio value
	if (target_value == 1) {
		*(gpio_address + 3) = (1 << gpio);
	}
	else {
		*(gpio_address + 4) = (1 << gpio);
	}
}

int gpioGet(int gpio) {
	unsigned long value = *(gpio_address + 1);
	return ((value >> gpio) & 1);
}
