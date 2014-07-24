#include "lcd.h"

void LCD_WR_DATA8(uint8_t da) 
{
    bcm2835_gpio_write(TFT_DC, HIGH);
	bcm2835_spi_transfer(da);
}  

void LCD_WR_DATA(uint16_t da)
{
    bcm2835_gpio_write(TFT_DC, HIGH);
	bcm2835_spi_transfer(da>>8);
	bcm2835_spi_transfer(da);
}	
  
void LCD_WR_REG(uint16_t da)	 
{
    bcm2835_gpio_write(TFT_DC, LOW);
	bcm2835_spi_transfer(da);
}

void LCD_AddressSet(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{  
	LCD_WR_REG(0x2a);
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
  
	LCD_WR_REG(0x2b);
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);

	LCD_WR_REG(0x2C);

}

void LCD_Init(void)
{
	if(!bcm2835_init())
		printf("bcm2835 init ERROR!");
		
	bcm2835_gpio_fsel(TFT_RST, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(TFT_DC, BCM2835_GPIO_FSEL_OUTP);
		
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);
	bcm2835_spi_begin();

    bcm2835_gpio_write(TFT_RST, HIGH);;
    bcm2835_delay(5);	
	bcm2835_gpio_write(TFT_RST, LOW);;
	bcm2835_delay(5);
	bcm2835_gpio_write(TFT_RST, HIGH);;

	bcm2835_gpio_write(TFT_CS, HIGH);
	bcm2835_delay(5);
	bcm2835_gpio_write(TFT_CS, LOW);

		LCD_WR_REG(0xCB);  
        LCD_WR_DATA8(0x39); 
        LCD_WR_DATA8(0x2C); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x34); 
        LCD_WR_DATA8(0x02); 

        LCD_WR_REG(0xCF);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0XC1); 
        LCD_WR_DATA8(0X30); 
 
        LCD_WR_REG(0xE8);  
        LCD_WR_DATA8(0x85); 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x78); 
 
        LCD_WR_REG(0xEA);  
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_REG(0xED);  
        LCD_WR_DATA8(0x64); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0X12); 
        LCD_WR_DATA8(0X81); 

        LCD_WR_REG(0xF7);  
        LCD_WR_DATA8(0x20); 
  
        LCD_WR_REG(0xC0);    //Power control 
        LCD_WR_DATA8(0x23);   //VRH[5:0] 
 
        LCD_WR_REG(0xC1);    //Power control 
        LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_WR_REG(0xC5);    //VCM control 
        LCD_WR_DATA8(0x3e); //对比度调节
        LCD_WR_DATA8(0x28); 
 
        LCD_WR_REG(0xC7);    //VCM control2 
        LCD_WR_DATA8(0x86);  //--
 
        LCD_WR_REG(0x36);    // Memory Access Control 
        LCD_WR_DATA8(0x48); //C8	   //48 68竖屏//28 E8 横屏

        LCD_WR_REG(0x3A);    
        LCD_WR_DATA8(0x55); 

        LCD_WR_REG(0xB1);    
        LCD_WR_DATA8(0x00);  
        LCD_WR_DATA8(0x18); 
 
        LCD_WR_REG(0xB6);    // Display Function Control 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x82);
        LCD_WR_DATA8(0x27);  
 
        LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
        LCD_WR_DATA8(0x00); 
 
        LCD_WR_REG(0x26);    //Gamma curve selected 
        LCD_WR_DATA8(0x01); 
 
        LCD_WR_REG(0xE0);    //Set Gamma 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x2B); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x4E); 
        LCD_WR_DATA8(0xF1); 
        LCD_WR_DATA8(0x37); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x10); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x09); 
        LCD_WR_DATA8(0x00); 

        LCD_WR_REG(0XE1);    //Set Gamma 
        LCD_WR_DATA8(0x00); 
        LCD_WR_DATA8(0x0E); 
        LCD_WR_DATA8(0x14); 
        LCD_WR_DATA8(0x03); 
        LCD_WR_DATA8(0x11); 
        LCD_WR_DATA8(0x07); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0xC1); 
        LCD_WR_DATA8(0x48); 
        LCD_WR_DATA8(0x08); 
        LCD_WR_DATA8(0x0F); 
        LCD_WR_DATA8(0x0C); 
        LCD_WR_DATA8(0x31); 
        LCD_WR_DATA8(0x36); 
        LCD_WR_DATA8(0x0F); 
 
        LCD_WR_REG(0x11);    //Exit Sleep 
        bcm2835_delay(120); 
				
        LCD_WR_REG(0x29);    //Display on 
        LCD_WR_REG(0x2c); 

}

void LCD_end(void)
{
	bcm2835_spi_end();
	bcm2835_close();
}

void LCD_Clear(uint16_t color)
{
	uint16_t i,j;

	LCD_AddressSet(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	{
		for (j=0;j<LCD_H;j++)
		{

			LCD_WR_DATA(color);

		}
	 }
}
