#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "gpio_api.h"

#define GPIO_OUTPUT 18
#define GPIO_INPUT 21

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
	gpioDirection(GPIO_INPUT, 0);

	int count = 0;
	gpioSet(GPIO_OUTPUT, 1);
	while (1) {
		if (gpioGet(GPIO_INPUT)) {
			printf("get postive signal after %d poll\n", count);
			break;
		}
		else {
			count++;
		}
	}

	gpioSet(GPIO_OUTPUT, 0);
	gpioDirection(GPIO_OUTPUT, 0);
	gpioDirection(GPIO_INPUT, 0);

	return 0;
}
