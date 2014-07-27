#include "bcm2835.h"
#include "pcf8574.h"
#include <stdio.h>

#define PCF8574_ADDR 0x25

int pcf8574_start(void)
{
	if(!bcm2835_init())
		return 0;
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(PCF8574_ADDR);
	return 1;
}

void pcf8574_write_byte(char data)
{
	char buf[1] = {data};
	
	bcm2835_i2c_write(buf, 1);

}

unsigned char pcf8574_read_byte(void)
{
	char data[1];
	return data[0];
}

void pcf8574_stop(void)
{
	bcm2835_i2c_end();
	bcm2835_close();
}

