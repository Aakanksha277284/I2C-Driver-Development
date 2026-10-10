#include <at89c5131.h>
//#include "lcd.h"
#include "i2cdriver.h"

// BMP280 definitions
#define BMP280_WRITE_ADDR 0xEC
#define BMP280_READ_ADDR  0xED
#define BMP280_ID_REG     0xD0


void debug(char *msg)
{
//    lcd_cmd(0x01);
  //  lcd_cmd(0x80);
  //  lcd_write_string(msg);
   // msdelay(800);
}

void display_hex(unsigned char val)
{
    char hex[] = "0123456789ABCDEF";
  //  lcd_write_char(hex[val >> 4]);
  //  lcd_write_char(hex[val & 0x0F]);
}

void main(void)
{
    unsigned char chip_id;
    unsigned char ack;

  //  lcd_init();
    I2C_Init();

    // START
    I2C_Start();
    debug("START");

    // ADDRESS
   ack =  I2C_Write(BMP280_WRITE_ADDR);
    
    ack = I2C_Write(BMP280_ID_REG);
   if (ack)
        debug("ADDR OK");
    else
        debug("ADDR ERR");


    // RESTART + READ
    I2C_Start();
   ack = I2C_Write(BMP280_READ_ADDR);
 
		
    chip_id = I2C_Read();

    // DATA DISPLAY
  //  lcd_cmd(0x01);
 //   lcd_cmd(0x80);
 //   lcd_write_string("DATA= ");
    display_hex(chip_id);
 //   msdelay(1000);

    // STOP
    I2C_Stop();

    while(1);
}