#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "gpio_api.h"

int main(int argc, char *argv[]) {
	int ret;
	int gpio_output_pin;
	int sleep_delay = 1000;

	if (argc > 1) {
		gpio_output_pin = atoi(argv[1]);
		if (argc > 2)
			sleep_delay = atoi(argv[2]);
	}
	else {
		fprintf(stderr, "Usage: %s <gpio output pin> [sleep_delay]\n", argv[0]);
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
		usleep(1);
		gpioSet(gpio_output_pin, 0);
		usleep(sleep_delay);
	}

	return 0;
}
