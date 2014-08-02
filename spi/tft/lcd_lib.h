#ifndef __lcd_lib__
#define __lcd_lib__

/**
 * 以默认的配置初始化
 * @return  0或错误号
 */
extern int lcd_init(void);
/**
 * 以常用的配置初始化
 * @return  0或错误号
 */
extern int lcd_init_normal(void);

extern void lcd_exit(void);

#define PIXEL_FORMAT_16 0x55
#define PIXEL_FORMAT_18 0x66
extern int lcd_pixel_format_set(int mode);

extern int lcd_sleep_out(void);
extern int lcd_display_on(void);

// -------------- memory access ----------------

#define MEMORY_ACCESS_MY 		(1<<7)
#define MEMORY_ACCESS_MX 		(1<<6)
#define MEMORY_ACCESS_MV 		(1<<5)
#define MEMORY_ACCESS_ML 		(1<<4)
#define MEMORY_ACCESS_BGR		(1<<3)
#define MEMORY_ACCESS_MH 		(1<<2)
#define MEMORY_ACCESS_NORMAL	0

extern int lcd_memory_access_control(int mode);
extern int lcd_memory_write(const unsigned char *buf, int size);
/**
 * 读取显存
 * @param  buf  接收数组
 * @param  size 接收数组尺寸（需要读取的字节数）
 * ......
 * @return      实际接收的字节数或错误号
 */
extern int lcd_memory_area_read(int x1, int y1, int x2, int y2, unsigned char *buf, int size);
#define lcd_memory_read(buf,size)  lcd_memory_area_read(0,0,0,0,buf,size)

// ------------ address access ------------------
extern int lcd_column_address_set(int x1, int x2);
extern int lcd_page_address_set(int y1, int y2);

#define lcd_address_set(x1, y1, x2, y2)	do \
										{	\
											lcd_column_address_set(x1,x2); \
											lcd_page_address_set(y1,y2); \
										}while(0)

#endif