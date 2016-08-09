#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "gpio_api.h"

#define GPIO_INPUT 19
int main() {
	int value, last_value, ret, step = 0, count = 0; 

	ret = gpioSetup();
	if (ret == -1) {
		printf("open /dev/mem failed\n");
		exit(EXIT_FAILURE);
	}
	else if (ret == -2) {
		printf("MAP_FAILED\n");
		exit(EXIT_FAILURE);
	}

	clock_t end, begin = clock();
	last_value = gpioGet(GPIO_INPUT);
	while (1) {
		value = gpioGet(GPIO_INPUT);
		if (value != last_value) {
			if (value == 0) {
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
