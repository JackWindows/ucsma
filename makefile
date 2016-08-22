gpio_tool: gpio_sysfs.c
	mips-openwrt-linux-gcc gpio_sysfs.c -o gpio_tool

gpio_poll: gpio_poll.c gpio_api.c gpio_api.h
	mips-openwrt-linux-gcc gpio_poll.c gpio_api.c -o gpio_poll -std=c99

gpio_pulse: gpio_pulse.c gpio_api.c gpio_api.h
	mips-openwrt-linux-gcc gpio_pulse.c gpio_api.c -o gpio_pulse -std=gnu99

gpio_delay: gpio_delay.c gpio_api.c gpio_api.h
	mips-openwrt-linux-gcc gpio_delay.c gpio_api.c -o gpio_delay -std=gnu99
