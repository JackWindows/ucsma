#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "gpio_api.h"

int main(int argc, char *argv[]) {
	char action[255];
	int gpio_port;
	int ret;

	if (argc - 1 < 2) {
		fprintf(stderr, "Usage: %s action gpio_port\naction: set_direction, get, set\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	strcpy(action, argv[1]);
	gpio_port = atoi(argv[2]);

	ret = gpioSetup();
	if (ret == -1) {
		printf("open /dev/mem failed\n");
		exit(EXIT_FAILURE);
	}
	else if (ret == -2) {
		printf("MAP_FAILED\n");
		exit(EXIT_FAILURE);
	}

	if (strcmp(action, "set_direction") == 0) {
		if (argc - 1 < 3) {
			fprintf(stderr, "Usage: %s set_direction gpio_port in|out\n", argv[0]);
			exit(EXIT_FAILURE);
		}
		char direction[255];
		strcpy(direction, argv[3]);
		if (strcmp(direction, "out") == 0) {
			gpioDirection(gpio_port, 1);
			printf("gpio %d direction set to out\n", gpio_port);
		}
		else if (strcmp(direction, "in") == 0) {
			gpioDirection(gpio_port, 0);
			printf("gpio %d direction set to in\n", gpio_port);
		}
		else {
			fprintf(stderr, "unknown direction encountered, direction can only be in or out\n");
			exit(EXIT_FAILURE);
		}
	}
	else if (strcmp(action, "set") == 0) {
		if (argc - 1 < 3) {
			fprintf(stderr, "Usage: %s set gpio_port 0|1\n", argv[0]);
			exit(EXIT_FAILURE);
		}
		int value;
		value = atoi(argv[3]);
		if (value != 0 && value != 1) {
			fprintf(stderr, "value can only be 0 or 1\n");
			exit(EXIT_FAILURE);
		}
		gpioSet(gpio_port, value);
		printf("gpio %d value set to %d\n", gpio_port, value);
	}
	else if (strcmp(action, "get") == 0) {
		int value;
		value = gpioGet(gpio_port);
		printf("gpio %d current value is %d\n", gpio_port, value);
	}
	else {
		fprintf(stderr, "unknown action encountered\n");
		exit(EXIT_FAILURE);
	}
	return 0;
}
