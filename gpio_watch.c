#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "gpio_api.h"

int main(int argc, char *argv[]) {
	int ret, gpio_port;
	
	ret = gpioSetup();
	if (ret == -1) {
		printf("open /dev/mem failed\n");
		exit(EXIT_FAILURE);
	}
	else if (ret == -2) {
		printf("MAP_FAILED\n");
		exit(EXIT_FAILURE);
	}

	if (argc - 1 != 1) {
		fprintf(stderr, "Usage: %s gpio_port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	gpio_port = atoi(argv[1]);

	int count = 0;
	struct timeval tv;
	struct timezone tz;

	while (1) {
		if (gpioGet(gpio_port)) {
			if (count > 0) {
				gettimeofday(&tv, &tz);
				printf("[%ld.%06ld]get postive signal(1) after %d count\n", tv.tv_sec, tv.tv_usec, count);
				count = 0;
			}
		}
		else {
			count++;
		}
	}

	return 0;
}
