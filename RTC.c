#include <at89c5131.h>
#include "lcd.h"
#include "i2cdriver.h"
#include "RTC.h"
#include "uart.h"

// Display 2-digit decimal on LCD
void lcd_write_dec2(unsigned char val) {
    lcd_write_char('0' + (val / 10));
    lcd_write_char('0' + (val % 10));
}

void display_time_lcd(unsigned char h, unsigned char m, unsigned char s) {
    lcd_cmd(0x80);              // Line 1, position 0
    lcd_write_string("TIME: ");
    lcd_write_dec2(h);
    lcd_write_char(':');
    lcd_write_dec2(m);
    lcd_write_char(':');
    lcd_write_dec2(s);
}

void send_time_uart(unsigned char h, unsigned char m, unsigned char s) {
    UART_SendString("TIME: ");
    UART_SendNumber(h);
    UART_SendChar(':');
    UART_SendNumber(m);
    UART_SendChar(':');
    UART_SendNumber(s);
    UART_SendString("\r\n");
}

void main(void) {
    unsigned char sec, min, hour;

    // Initialise peripherals
    lcd_init();
    I2C_Init();
    UART_Init();

    // Splash screen
    lcd_cmd(0x01);
    lcd_cmd(0x80);
    lcd_write_string("EE337 Phase 3");
    msdelay(1000);
    lcd_cmd(0x01);

    // Init RTC (waits 200ms, clears CH bit, disables SQW)
    DS1307_Init();

    // Set initial time: 00:00:00  <-- change as needed
    DS1307_SetTime(0, 0, 0);

    UART_SendString("RTC Initialised\r\n");

    while (1) {
        // Read time from DS1307
        DS1307_ReadTime(&sec, &min, &hour);

        // Display on LCD
        display_time_lcd(hour, min, sec);

        // Send over UART
        send_time_uart(hour, min, sec);

        msdelay(1000);  // Update every second
    }
}