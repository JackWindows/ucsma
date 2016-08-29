#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "gpio_api.h"

#define GPIO_OUTPUT 20

int main(int argc, char *argv[]) {
	int ret;
	int gpio_output_pin = GPIO_OUTPUT;

	if (argc > 1)
		gpio_output_pin = atoi(argv[1]);
	else {
		fprintf(stderr, "Usage: %s <gpio output pin>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	ret = gpioSetup();
	if (ret == -1) {
		printf("open /dev/mem failed\n");
		exit(EXIT_FAILURE);
	}
	else if (ret == -2) {
		printf("MAP_FAILED\n");
		exit(EXIT_FAILURE);
	}

	gpioDirection(gpio_output_pin, 1);

	while (1) {
		gpioSet(gpio_output_pin, 1);
		usleep(100);
		gpioSet(gpio_output_pin, 0);
		usleep(1000000);
	}

	return 0;
}
