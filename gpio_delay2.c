#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "gpio_api.h"

int main(int argc, char *argv[]) {
	int value, last_value, ret;
	unsigned int step = 0, count = 0;
	int gpio_input_pin1, gpio_input_pin2;

	if (argc > 2) {
		gpio_input_pin1 = atoi(argv[1]);
		gpio_input_pin2 = atoi(argv[2]);
	}
	else {
		fprintf(stderr, "Usage: %s <gpio input pin1> <gpio input pin2>\n", argv[0]);
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

	gpioDirection(gpio_input_pin1, 0);
	gpioDirection(gpio_input_pin2, 0);
	gpioDirection(20, 1);
	clock_t end, begin = clock();
	last_value = gpioGet(gpio_input_pin1) & gpioGet(gpio_input_pin2);
	while (1) {
		if (step == 1000000) {
			gpioSet(20, 1);
		}
		value = gpioGet(gpio_input_pin1) & gpioGet(gpio_input_pin2);
		if (value != last_value) {
			if (value == 0 && count > 300) {
				end = clock();
				printf("value changed from %d to %d after %4d count at %f, step %6d\n", last_value, value, count, (double)(end - begin) / CLOCKS_PER_SEC, step);
				break;
			}
			last_value = value;
			count = 0;
		}
		else {
			count++;
			if (last_value == 1 && count > 300) {
				end = clock();
				printf("value changed from %d to %d after %4d count at %f, step %6d\n", last_value, value, count, (double)(end - begin) / CLOCKS_PER_SEC, step);
				break;
			}
		}
		step++;
	}

	gpioSet(20, 0);
	return 0;
}
