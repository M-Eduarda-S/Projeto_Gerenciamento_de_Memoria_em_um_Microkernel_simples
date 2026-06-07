#include "task.h"
#include "scheduler.h"
#include "memory.h"
#include "uart.h"

extern void uart_print(const char*);

static void delay_loop(uint64_t count)
{
    while (count--)
        asm volatile ("nop");
}

static void delay_seconds(uint64_t seconds)
{
    /* delay na impressão */
    const uint64_t count_per_second = 200000000ULL;
    delay_loop(seconds * count_per_second);
}

/*   Tasks   */

void task1()
{
    while (1)
    {
        uart_print("Task 1 running\n");

        uart_print("Memory used: ");
        uart_print_uint(memory_used());
        uart_print(" bytes\n");

        uart_print("Memory free: ");
        uart_print_uint(memory_free());
        uart_print(" bytes\n\n");

        delay_seconds(1);
        yield();
    }
}

void task2()
{
    while (1)
    {
        uart_print("Task 2 running\n");

        uart_print("Memory used: ");
        uart_print_uint(memory_used());
        uart_print(" bytes\n");

        uart_print("Memory free: ");
        uart_print_uint(memory_free());
        uart_print(" bytes\n\n");

        delay_seconds(1);
        yield();
    }
}

/*   Kernel   */

void kernel_main()
{
    memory_init();   // OBRIGATÓRIO

    /* Dump inicial do heap para debug via UART */
    heap_dump();

    uart_print("\n=== Kernel ===\n");

    xTaskCreate(task1, 2048, 1);
    xTaskCreate(task2, 2048, 1);

    scheduler_start();

    while (1);
}