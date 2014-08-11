/**
 * 简单的检测指定GPIO输入电平的函数库
 * 
 * Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 * Copyright (C) 2014 openRPi
 *
 *	代码遵循GNU协议
 *
 * 文档：http://www.openrpi.org/blogs/?p=232
 */
#include "button.h"
#include "bcm2835.h"

#define BTN_PIN		RPI_V2_GPIO_P1_11

/**
 * GPIO初始化，按键初始化
 * @return: 成功1，失败0
 */
int button_init(void)
{
	if(!bcm2835_init())
		return 0;
	bcm2835_gpio_fsel(BTN_PIN, BCM2835_GPIO_FSEL_INPT);
	
	// 设置GPIO下拉电阻
	bcm2835_gpio_set_pud(BTN_PIN, BCM2835_GPIO_PUD_DOWN);
	return 1;
}

int button_value(void)
{
	return bcm2835_gpio_lev(BTN_PIN);
}

void button_end(void)
{
	bcm2835_close();
}
