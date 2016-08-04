#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "gpio_api.h"

#define GPIO_INPUT 19
#define MAX_STEP 10000000
int main() {
	int value, ret, count = 0, max_count = 0;

	ret = gpioSetup();
	if (ret == -1) {
		printf("open /dev/mem failed\n");
		exit(EXIT_FAILURE);
	}
	else if (ret == -2) {
		printf("MAP_FAILED\n");
		exit(EXIT_FAILURE);
	}

	clock_t begin = clock();
	for (int i = 0; i < MAX_STEP; i++) {
		value = gpioGet(GPIO_INPUT);
		if (value == 1) {
			count++;
		}
		else {
			if (count > 0) {
				printf("%d consecutive high voltage detected\n", count);
				if (count > max_count) {
					max_count = count;
				}
				count = 0;
			}
		}
	}
	clock_t end = clock();

	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\n");
	printf("longest consectuive high voltage: %d\n", max_count);
	printf("total running time for %d rounds of polling: %fs\n", MAX_STEP, time_spent);
	return 0;
}
