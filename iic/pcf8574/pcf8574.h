#ifndef __PCF8574_H__
#define __PCF8574_H__

extern int pcf8574_start(void);
extern void pcf8574_write_byte(char data);
extern void pcf8574_stop(void);

#endif
