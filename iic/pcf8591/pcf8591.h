#ifndef __PCF8591_H__
#define __PCF8591_H__

#define CHANNEL_0 0x40
#define CHANNEL_1 0x41
#define CHANNEL_2 0x42
#define CHANNEL_3 0x43

extern int pcf8591_start(void);
extern void pcf8591_send_cmd(char channel);
extern unsigned char pcf8591_read_byte(void);
extern float pcf8591_value(char channel);
extern void pcf8591_stop(void);

#endif
