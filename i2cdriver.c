#include <at89c5131.h>
#include "i2cdriver.h"

void I2C_Init(void)
{
    SSCON = 0x40;   // Enable I2C
}

void I2C_Wait(void)
{
    while ((SSCON & 0x08) == 0);   // Wait for SI = 1
}

void I2C_Start(void)
{
    SSCON = 0x60;   // START
    I2C_Wait();
}

void I2C_Stop(void)
{
    SSCON = 0x50;   // STOP
}

unsigned char I2C_Write(unsigned char dat)
{
    SSDAT = dat;
    SSCON = 0x40;   // Clear SI
    I2C_Wait();
	  if (SSCON & 0x04)             // AA bit (ACK received)
        return 0;                 // Success
    else
        return 1;                 // NACK (failure)

}

unsigned char I2C_Read(void)
{
    SSCON = 0x40;  
    I2C_Wait();
    return SSDAT;
}

void I2C_Ack(void) {
    
     SSCON = 0x44;  
     I2C_Wait();
}

void I2C_Nack(void) {
     SSCON = 0x40; 
     I2C_Wait(); 
}
