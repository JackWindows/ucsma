#include <stdio.h>
#include <time.h>
#include "gpio_api.h"

#define GPIO_INPUT 19
#define MAX_STEP 100000
int main() {
	int values[MAX_STEP];
	int value;

	clock_t begin = clock();
	for (int i = 0; i < MAX_STEP; i++) {
		value = _gpioGet(GPIO_INPUT);
		values[i] = value;
	}
	clock_t end = clock();

	int cur_state = 0, count = 0;
	for (int i = 0; i < MAX_STEP; i++) {
		if (values[i] == 0) {
			if (cur_state == 1) {
				printf("%d consecutive high voltage detected\n", count);
				cur_state = 0, count = 0;
			}
		}
		else {
			if (cur_state == 0) {
				cur_state = 1;
				count++;
			}
			else {
				count++;
			}
		}
	}
	printf("\n");
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("total running time for %d rounds of polling: %fs\n", MAX_STEP, time_spent);
	return 0;
}
