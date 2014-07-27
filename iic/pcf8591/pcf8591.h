#ifndef __PCF8591_H__
#define __PCF8591_H__

extern int pcf8591_start(void);
extern void pcf8591_send_cmd(char channel);
extern unsigned char pcf8591_read_byte(void);
extern float pcf8591_value(char channel);
extern void pcf8591_stop(void);

#endif
