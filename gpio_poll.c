#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "gpio_api.h"

#define GPIO_INPUT 19
int main(int argc, char *argv[]) {
	int value, last_value, ret, step = 0, count = 0; 
	int gpio_input_pin = GPIO_INPUT;

	if (argc > 1)
		gpio_input_pin = atoi(argv[1]);
	else {
		fprintf(stderr, "Usage: %s <gpio input pin>\n", argv[0]);
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

	gpioDirection(gpio_input_pin, 0);
	clock_t end, begin = clock();
	last_value = gpioGet(gpio_input_pin);
	while (1) {
		value = gpioGet(gpio_input_pin);
		if (value != last_value) {
			if (value == 0 && count > 300) {
				end = clock();
				printf("value changed from %d to %d after %4d count at %f, step %6d\n", last_value, value, count, (double)(end - begin) / CLOCKS_PER_SEC, step);
			}
			last_value = value;
			count = 0;
		}
		else {
			count++;
		}
		step++;
	}

	return 0;
}
