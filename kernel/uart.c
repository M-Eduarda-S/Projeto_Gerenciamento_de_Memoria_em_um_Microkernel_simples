#include "uart.h"
#include <stdint.h>

#define UART0 0x10000000L
#define UART_RBR 0x00
#define UART_LSR 0x05
#define UART_LSR_DR 0x01

char uart_getc(void)
{
    volatile uint8_t *lsr = (volatile uint8_t*)(UART0 + UART_LSR);

    while (!(*lsr & UART_LSR_DR))
        ;

    return *(volatile uint8_t*)(UART0 + UART_RBR);
}

/* Imprime número decimal */
void uart_print_uint(uint64_t value)
{
    char buf[32];
    int i = 0;

    if (value == 0)
    {
        uart_putc('0');
        return;
    }

    while (value > 0)
    {
        buf[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i--)
        uart_putc(buf[i]);
}


void uart_putc(char c)
{
    *(volatile uint8_t*)UART0 = c;
}

void uart_print(const char *s)
{
    while (*s)
        uart_putc(*s++);
}
