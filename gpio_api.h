#ifndef GPIO_API_
#define GPIO_API_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void _gpioExport(int);
void _gpioUnexport(int);
void _gpioDirection(int, int);
void _gpioSet(int, int);
int _gpioGet(int);

#endif
