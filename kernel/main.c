#include "task.h"
#include "scheduler.h"
#include "memory.h"
#include "uart.h"

static void *p1 = 0;
static void *p2 = 0;
static void *p3 = 0;
static void *p4 = 0;
static int tasks_created = 0;

static void task1(void);
static void task2(void);

static void print_memory_stats(const char *label)
{
    uart_print(label);
    uart_print("\n");

    uart_print("Heap total: ");
    uart_print_uint(memory_total());
    uart_print(" bytes\n");

    uart_print("Heap usado: ");
    uart_print_uint(memory_used());
    uart_print(" bytes\n");

    uart_print("Heap livre: ");
    uart_print_uint(memory_free());
    uart_print(" bytes\n\n");
}

static void show_menu(void)
{
    uart_print("=== Menu de demonstracao de memoria ===\n");
    uart_print("1 - Mostrar estatisticas do heap\n");
    uart_print("2 - Teste: alocar p1, p2, p3\n");
    uart_print("3 - Teste: liberar p2 e p1 (coalescencia)\n");
    uart_print("4 - Teste: alocar p4 em memoria liberada\n");
    uart_print("5 - Teste: liberar p3 e p4\n");
    uart_print("6 - Criar tasks com stacks dinamicas\n");
    uart_print("7 - Remover Task 0 (liberar stack)\n");
    uart_print("8 - Mostrar mapa do heap\n");
    uart_print("9 - Iniciar o scheduler\n");
    uart_print("0 - Sair do menu e travar kernel\n");
    uart_print("Escolha: ");
}

static void allocate_initial_blocks(void)
{
    if (p1 || p2 || p3)
    {
        uart_print("Blocos ja alocados. Liberar antes de executar novamente.\n\n");
        return;
    }

    uart_print("=== Alocando p1, p2 e p3 ===\n");

    p1 = kmalloc(1024);
    uart_print("p1 = "); uart_print_uint((uint64_t)p1); uart_print("\n");

    p2 = kmalloc(2048);
    uart_print("p2 = "); uart_print_uint((uint64_t)p2); uart_print("\n");

    p3 = kmalloc(512);
    uart_print("p3 = "); uart_print_uint((uint64_t)p3); uart_print("\n\n");

    print_memory_stats("Estado apos alocacoes iniciais");
}

static void free_p1_p2(void)
{
    uart_print("=== Liberando p2 e p1 ===\n");

    if (p2)
    {
        kfree(p2);
        p2 = 0;
        uart_print("p2 liberado\n");
    }
    else
    {
        uart_print("p2 ja foi liberado ou nao foi alocado\n");
    }

    if (p1)
    {
        kfree(p1);
        p1 = 0;
        uart_print("p1 liberado\n");
    }
    else
    {
        uart_print("p1 ja foi liberado ou nao foi alocado\n");
    }

    uart_print("\n");
    print_memory_stats("Estado apos coalescencia");
}

static void allocate_p4(void)
{
    if (p4)
    {
        uart_print("p4 ja esta alocado. Liberar antes de alocar novamente.\n\n");
        return;
    }

    uart_print("=== Alocando p4 (1536 bytes) sobre memoria liberada ===\n");
    p4 = kmalloc(1536);
    uart_print("p4 = "); uart_print_uint((uint64_t)p4); uart_print("\n\n");
    print_memory_stats("Estado apos alocacao de p4");
}

static void free_p3_p4(void)
{
    uart_print("=== Liberando p3 e p4 ===\n");

    if (p3)
    {
        kfree(p3);
        p3 = 0;
        uart_print("p3 liberado\n");
    }
    else
    {
        uart_print("p3 ja foi liberado ou nao foi alocado\n");
    }

    if (p4)
    {
        kfree(p4);
        p4 = 0;
        uart_print("p4 liberado\n");
    }
    else
    {
        uart_print("p4 ja foi liberado ou nao foi alocado\n");
    }

    uart_print("\n");
    print_memory_stats("Estado apos liberacoes finais");
}

static void create_tasks_menu(void)
{
    if (tasks_created)
    {
        uart_print("Tasks ja criadas\n\n");
        return;
    }

    uart_print("=== Criando tasks com stacks dinamicas ===\n");
    xTaskCreate(task1, 2048, 1);
    uart_print("Task 1 criada\n");
    xTaskCreate(task2, 2048, 1);
    uart_print("Task 2 criada\n\n");

    tasks_created = 1;
    print_memory_stats("Heap apos criacao de tasks");
}

static int handle_menu_choice(char choice)
{
    switch (choice)
    {
        case '1':
            print_memory_stats("Estatisticas do heap");
            break;
        case '2':
            allocate_initial_blocks();
            break;
        case '3':
            free_p1_p2();
            break;
        case '4':
            allocate_p4();
            break;
        case '5':
            free_p3_p4();
            break;
        case '6':
            create_tasks_menu();
            break;
        case '7':
            if (!tasks_created)
            {
                uart_print("Crie as tasks primeiro.\n");
                break;
            }

            uart_print("=== Removendo Task 0 ===\n");
            vTaskDelete(0);
            uart_print("Task 0 removida (stack liberada)\n\n");

            print_memory_stats("Heap apos remover task");
            break;
        case '8':
            heap_dump();
            break;
        case '9':
        
            if (!tasks_created)
            {
                uart_print("Crie as tasks primeiro antes de iniciar o scheduler.\n");
                break;
            }
            uart_print("Iniciando scheduler...\n");
            return 1;
        case '0':
            uart_print("Saindo do menu. Kernel travado.\n");
            while (1);
            break;
        default:
            uart_print("Opcao invalida. Tente novamente.\n");
            break;
    }

    uart_print("\n");
    return 0;
}

void task1()
{
    while (1)
    {
        uart_print("Task 1 rodando\n");
        print_memory_stats("Task 1 estatisticas");
        yield();
    }
}

void task2()
{
    while (1)
    {
        uart_print("Task 2 rodando\n");
        print_memory_stats("Task 2 estatisticas");
        yield();
    }
}

void kernel_main()
{
    memory_init();   // OBRIGATÓRIO

    uart_print("=== Kernel de demonstracao de alocador free list ===\n\n");
    show_menu();

    while (1)
    {
        char choice = uart_getc();

        if (choice == '\r' || choice == '\n')
            continue;

        uart_putc(choice);
        uart_print("\n");

        if (handle_menu_choice(choice))
            break;

        show_menu();
    }

    scheduler_start();

    while (1);
}
