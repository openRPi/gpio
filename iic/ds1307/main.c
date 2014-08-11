/*
 *	main.c 用IIC总线操作DS1307时钟芯片
 *	
 *	Copyright (C) 2014 concefly <h.wenjian@openrpi.org>
 *	Copyright (C) 2014 openRPi
 *	
 *		代码遵循GNU协议
 *	
 *	文档： http://www.openrpi.org/blogs/?p=242
 */

#include <stdio.h>
#include "ds1307.h"

int main()
{
	int year=0, month=0, date=0, hour=0, minute=0, second=0, day=0;
	char buf[50];
	printf("----------DS1307----------\n\n");

	if(!ds1307_init())
	{
		printf("ds1307 init ERROR!\n");
		return 1;
	}

	while(1)
	{
		printf("[S]how time, [E]dit Time, [Q]uit\n>> "); scanf("%c",&buf[0]);
		if(buf[0]=='S' || buf[0] == 's')
		{
			year = ds1307_get_year();
			month = ds1307_get_mon();
			date = ds1307_get_date();
			ds1307_get_hour_24(&hour);
			minute = ds1307_get_min();
			second = ds1307_get_sec();
			day = ds1307_get_day();
			printf("%d-%d-%d %d:%d:%d day%d \n",year,month,date,hour,minute,second,day);
		}
		else if(buf[0]=='E' || buf[0] == 'e')
		{
			printf("Input like this: 2014-8-1 10:0:0 5\n>> ");

			// 输入参数格式错误
			if(scanf("%d-%d-%d %d:%d:%d %d",&year,&month,&date,&hour,&minute,&second,&day) < 7)
			{
				printf("Format error!\n");
				continue;
			}
			ds1307_set_year(year);
			ds1307_set_month(month);
			ds1307_set_date(date);
			ds1307_set_hour_24(hour);
			ds1307_set_min(minute);
			ds1307_set_sec(second);
			ds1307_set_day(day);
		}
		else if(buf[0]=='Q' || buf[0] == 'q')
			break;
	}
	
	ds1307_end();

	return 0;
}
