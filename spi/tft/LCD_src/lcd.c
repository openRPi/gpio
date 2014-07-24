#include "lcd.h"
#include "bcm2835.h"

#define TFT_RST RPI_V2_GPIO_P1_11
#define TFT_DC  RPI_V2_GPIO_P1_12 
#define TFT_CS  RPI_V2_GPIO_P1_24

void LCD_wr_data8(uint8_t da) 
{
    bcm2835_gpio_write(TFT_DC, HIGH);
	bcm2835_spi_transfer(da);
}  

void LCD_wr_data(uint16_t da)
{
    bcm2835_gpio_write(TFT_DC, HIGH);
	bcm2835_spi_transfer(da>>8);
	bcm2835_spi_transfer(da);
}	
  
void LCD_wr_reg(uint16_t da)	 
{
    bcm2835_gpio_write(TFT_DC, LOW);
	bcm2835_spi_transfer(da);
}

void LCD_set_addr(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{  
	LCD_wr_reg(0x2a);
	LCD_wr_data(x1);
	LCD_wr_data(x2);
  
	LCD_wr_reg(0x2b);
	LCD_wr_data(y1);
	LCD_wr_data(y2);

	LCD_wr_reg(0x2C);

}

void LCD_init(void)
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

		LCD_wr_reg(0xCB);  
        LCD_wr_data8(0x39); 
        LCD_wr_data8(0x2C); 
        LCD_wr_data8(0x00); 
        LCD_wr_data8(0x34); 
        LCD_wr_data8(0x02); 

        LCD_wr_reg(0xCF);  
        LCD_wr_data8(0x00); 
        LCD_wr_data8(0XC1); 
        LCD_wr_data8(0X30); 
 
        LCD_wr_reg(0xE8);  
        LCD_wr_data8(0x85); 
        LCD_wr_data8(0x00); 
        LCD_wr_data8(0x78); 
 
        LCD_wr_reg(0xEA);  
        LCD_wr_data8(0x00); 
        LCD_wr_data8(0x00); 
 
        LCD_wr_reg(0xED);  
        LCD_wr_data8(0x64); 
        LCD_wr_data8(0x03); 
        LCD_wr_data8(0X12); 
        LCD_wr_data8(0X81); 

        LCD_wr_reg(0xF7);  
        LCD_wr_data8(0x20); 
  
        LCD_wr_reg(0xC0);    //Power control 
        LCD_wr_data8(0x23);   //VRH[5:0] 
 
        LCD_wr_reg(0xC1);    //Power control 
        LCD_wr_data8(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_wr_reg(0xC5);    //VCM control 
        LCD_wr_data8(0x3e); //对比度调节
        LCD_wr_data8(0x28); 
 
        LCD_wr_reg(0xC7);    //VCM control2 
        LCD_wr_data8(0x86);  //--
 
        LCD_wr_reg(0x36);    // Memory Access Control 
        LCD_wr_data8(0x48); //C8	   //48 68竖屏//28 E8 横屏

        LCD_wr_reg(0x3A);    
        LCD_wr_data8(0x55); 

        LCD_wr_reg(0xB1);    
        LCD_wr_data8(0x00);  
        LCD_wr_data8(0x18); 
 
        LCD_wr_reg(0xB6);    // Display Function Control 
        LCD_wr_data8(0x08); 
        LCD_wr_data8(0x82);
        LCD_wr_data8(0x27);  
 
        LCD_wr_reg(0xF2);    // 3Gamma Function Disable 
        LCD_wr_data8(0x00); 
 
        LCD_wr_reg(0x26);    //Gamma curve selected 
        LCD_wr_data8(0x01); 
 
        LCD_wr_reg(0xE0);    //Set Gamma 
        LCD_wr_data8(0x0F); 
        LCD_wr_data8(0x31); 
        LCD_wr_data8(0x2B); 
        LCD_wr_data8(0x0C); 
        LCD_wr_data8(0x0E); 
        LCD_wr_data8(0x08); 
        LCD_wr_data8(0x4E); 
        LCD_wr_data8(0xF1); 
        LCD_wr_data8(0x37); 
        LCD_wr_data8(0x07); 
        LCD_wr_data8(0x10); 
        LCD_wr_data8(0x03); 
        LCD_wr_data8(0x0E); 
        LCD_wr_data8(0x09); 
        LCD_wr_data8(0x00); 

        LCD_wr_reg(0XE1);    //Set Gamma 
        LCD_wr_data8(0x00); 
        LCD_wr_data8(0x0E); 
        LCD_wr_data8(0x14); 
        LCD_wr_data8(0x03); 
        LCD_wr_data8(0x11); 
        LCD_wr_data8(0x07); 
        LCD_wr_data8(0x31); 
        LCD_wr_data8(0xC1); 
        LCD_wr_data8(0x48); 
        LCD_wr_data8(0x08); 
        LCD_wr_data8(0x0F); 
        LCD_wr_data8(0x0C); 
        LCD_wr_data8(0x31); 
        LCD_wr_data8(0x36); 
        LCD_wr_data8(0x0F); 
 
        LCD_wr_reg(0x11);    //Exit Sleep 
        bcm2835_delay(120); 
				
        LCD_wr_reg(0x29);    //Display on 
        LCD_wr_reg(0x2c); 

}

void LCD_close(void)
{
	bcm2835_spi_end();
	bcm2835_close();
}

void LCD_clear(uint16_t color)
{
	uint16_t i,j;

	LCD_set_addr(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	{
		for (j=0;j<LCD_H;j++)
		{

			LCD_wr_data(color);

		}
	 }
}
