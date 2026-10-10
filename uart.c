#include "uart.h"

// UART Init: 1200 baud, Mode 1 (8-bit async), using Timer 1
// Assumes 12 MHz crystal -> SMOD=0 -> TH1 = 256 - (12MHz / (384 * 1200)) = 256 - 26 = 0xE6
void UART_Init(void) {
    SCON  = 0x50;   // Mode 1, 8-bit UART, REN enabled
    TMOD |= 0x20;   // Timer 1, Mode 2 (auto-reload)
    TH1   = 0xE6;   // 1200 baud @ 12MHz, SMOD=0
    TR1   = 1;      // Start Timer 1
    TI    = 1;      // Set TI to indicate transmitter ready
}

void UART_SendChar(unsigned char c) {
    TI   = 0;
    SBUF = c;
    while (!TI);    // Wait until byte is sent
    TI   = 0;
}

void UART_SendString(char *str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}

// Sends a two-digit decimal number (0–59)
void UART_SendNumber(unsigned char num) {
    UART_SendChar('0' + (num / 10));
    UART_SendChar('0' + (num % 10));
}