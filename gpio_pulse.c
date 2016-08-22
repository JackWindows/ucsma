#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "gpio_api.h"

#define GPIO_OUTPUT 20

int main() {
	int ret;
	
	ret = gpioSetup();
	if (ret == -1) {
		printf("open /dev/mem failed\n");
		exit(EXIT_FAILURE);
	}
	else if (ret == -2) {
		printf("MAP_FAILED\n");
		exit(EXIT_FAILURE);
	}

	gpioDirection(GPIO_OUTPUT, 1);

	while (1) {
		gpioSet(GPIO_OUTPUT, 1);
		usleep(100);
		gpioSet(GPIO_OUTPUT, 0);
		usleep(1000000);
	}

	return 0;
}
