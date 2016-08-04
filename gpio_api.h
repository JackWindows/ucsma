#ifndef GPIO_API_
#define GPIO_API_

void _gpioExport(int);
void _gpioUnexport(int);
void _gpioDirection(int, int);
void _gpioSet(int, int);
int _gpioGet(int);


extern volatile unsigned long *gpio_address;
int gpioSetup();
void gpioDirection(int, int);
void gpioSet(int, int);
void gpioSet2(int, int);
int gpioGet(int);

#endif
