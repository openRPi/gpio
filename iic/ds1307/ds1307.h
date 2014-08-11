#ifndef __ds1307__
#define __ds1307__

#define DS1307_ADDR 0x68

#define DS1307_CH_ON 0
#define DS1307_CH_OFF 1

extern int ds1307_init(void);
extern void ds1307_end(void);

extern int ds1307_get_ch(void);
extern int ds1307_get_sec(void);
extern int ds1307_get_min(void);
extern int ds1307_get_hour(void);
extern int ds1307_get_day(void);
extern int ds1307_get_date(void);
extern int ds1307_get_mon(void);
extern int ds1307_get_year(void);

extern void ds1307_set_ch(int ch);
extern void ds1307_set_sec(int sec);
extern void ds1307_set_min(int min);
extern void ds1307_set_hour(int hour);
extern void ds1307_set_hour(int hour);
extern void ds1307_set_day(int day);
extern void ds1307_set_date(int date);
extern void ds1307_set_mon(int mon);
extern void ds1307_set_year(int year);

#endif
