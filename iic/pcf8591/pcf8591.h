#ifndef __PCF8591_H__
#define __PCF8591_H__

#define PCF_CHANNEL_0 0x40
#define PCF_CHANNEL_1 0x41
#define PCF_CHANNEL_2 0x42
#define PCF_CHANNEL_3 0x43

#define PCF8591_ADDR 0x48
#define PCF8591_REF_VOL 3.3

extern int pcf8591_start(void);
extern void pcf8591_send_cmd(char channel);
extern unsigned char pcf8591_read_byte(void);
extern float pcf8591_value(char channel);
extern void pcf8591_stop(void);

#endif
