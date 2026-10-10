#include "RTC.h"
//#include "lcd.h"
// Convert BCD to decimal
unsigned char BCD_to_Dec(unsigned char bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// Convert decimal to BCD
unsigned char Dec_to_BCD(unsigned char dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

// Write a single byte to a DS1307 register
static void DS1307_WriteReg(unsigned char reg, unsigned char val) {
    I2C_Start();
    I2C_Write(DS1307_WRITE_ADDR);
    I2C_Write(reg);
    I2C_Write(val);
    I2C_Stop();
}

// Read a single byte from a DS1307 register
static unsigned char DS1307_ReadReg(unsigned char reg) {
    unsigned char val;
    I2C_Start();
    I2C_Write(DS1307_WRITE_ADDR);
    I2C_Write(reg);
    I2C_Start();                    // Repeated START
    I2C_Write(DS1307_READ_ADDR);
    val = I2C_Read();               // Read with NACK (last byte)
    I2C_Nack();
    I2C_Stop();
    return val;
}

void DS1307_Init(void) {
    unsigned char sec;          // << declare FIRST, before any statements

  //  msdelay(200);               // DS1307 needs 200ms on power-up

    sec = DS1307_ReadReg(DS1307_SEC_REG);
    sec &= 0x7F;                // Clear CH bit to start the clock
    DS1307_WriteReg(DS1307_SEC_REG, sec);

    DS1307_WriteReg(DS1307_CONTROL_REG, 0x00);  // Disable SQW output
}

// Set time: pass decimal values, function converts to BCD
void DS1307_SetTime(unsigned char sec, unsigned char min, unsigned char hour) {
    I2C_Start();
    I2C_Write(DS1307_WRITE_ADDR);
    I2C_Write(DS1307_SEC_REG);          // Start at seconds register
    I2C_Write(Dec_to_BCD(sec) & 0x7F); // Clear CH bit while writing
    I2C_Write(Dec_to_BCD(min));
    I2C_Write(Dec_to_BCD(hour) & 0x3F);// Ensure 24-hour mode (bit6=0)
    I2C_Stop();
}

// Read time: returns decimal values via pointers
void DS1307_ReadTime(unsigned char *sec, unsigned char *min, unsigned char *hour) {
    // Set register pointer to 0x00 (seconds)
    I2C_Start();
    I2C_Write(DS1307_WRITE_ADDR);
    I2C_Write(DS1307_SEC_REG);
    I2C_Stop();

    // Sequential read: seconds, minutes, hours
    I2C_Start();
    I2C_Write(DS1307_READ_ADDR);

    // Read seconds - ACK to continue
    *sec  = I2C_Read(); I2C_Ack();
    // Read minutes - ACK to continue
    *min  = I2C_Read(); I2C_Ack();
    // Read hours  - NACK to stop
    *hour = I2C_Read(); I2C_Nack();

    I2C_Stop();

    // Strip status bits and convert BCD to decimal
    *sec  = BCD_to_Dec(*sec  & 0x7F);  // Mask CH bit
    *min  = BCD_to_Dec(*min  & 0x7F);
    *hour = BCD_to_Dec(*hour & 0x3F);  // Mask to 24hr format
}