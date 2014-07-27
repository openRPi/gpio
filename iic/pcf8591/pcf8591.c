#include "bcm2835.h"
#include "pcf8591.h"

#define PCF8591_ADDR 0x48
#define REF_VOL 3.3

int pcf8591_start(void)
{
	if(!bcm2835_init())
		return 0;
	bcm2835_i2c_begin();
	bcm2835_i2c_setSlaveAddress(PCF8591_ADDR);
	return 1;
}

void pcf8591_send_cmd(char channel)
{
	char buf[1] = { channel  };
  	bcm2835_i2c_write(buf, 1);
}

unsigned char pcf8591_read_byte(void)
{
	char buf[1] = {0};

	bcm2835_i2c_read(buf,1);

	return buf[0];
}

float pcf8591_value(char channel)
{
	unsigned int adnum;
	float value;
	pcf8591_send_cmd(channel);
	adnum = pcf8591_read_byte()*2;
	value = adnum/2 * (REF_VOL / 256);
	return value;
}

void pcf8591_stop(void)
{
	bcm2835_i2c_end();
	bcm2835_close();
}
 
