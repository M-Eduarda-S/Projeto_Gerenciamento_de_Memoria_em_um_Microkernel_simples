#include "memory.h"
#include <stdint.h>
#include "uart.h"

/*   Configuração do heap   */

#define HEAP_START 0x80400000UL
#define HEAP_SIZE  (8 * 1024 * 1024)   // 8 MB

/* Estrutura do bloco de memória */
typedef struct block
{
    uint64_t size;
    int free;
    struct block *next;
} block_t;

static uint8_t *heap_base = (uint8_t*)HEAP_START;
static block_t *free_list;

/*   Inicialização do heap   */

void memory_init(void)
{
    free_list = (block_t*)heap_base; // primeiro bloco começa no início do heap
    free_list->size = HEAP_SIZE - sizeof(block_t); // tamanho do bloco livre
    free_list->next = 0;
    free_list->free = 1; 
}

/*   Divisão heap em bloco */

static void dividir_block(block_t *block, uint64_t size)
{
    // não cria quando não há espaço suficiente para dividir
    if (block->size <= size + sizeof(block_t))
        return;

    block_t *new_block = (block_t*)((uint8_t*)block + sizeof(block_t) + size); // bloco novo começa após o bloco atual

    new_block->size = block->size - size - sizeof(block_t); // tem o tamanho restante
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
}

/*   Free List Allocator - First Fit   */

void *kmalloc(uint64_t size)
{
    block_t *atual = free_list; // passa pela lista de blocos livres

    if (size == 0)
        return 0;

    /* Alinhamento para 8 bytes */
    size = (size + 7) & ~7ULL;

    // faz a divisão do bloco se precisar
    while (atual)
    {
        if (atual->free && atual->size >= size) // first fit
        {
            /* divisão do bloco */
            if (atual->size >= size + sizeof(block_t) + 8)
                dividir_block(atual, size);

            atual->free = 0; // ocupado

            return (void*)(atual + 1);
        }

        atual = atual->next;
    }

    return 0;
}

/*   Coalescência de blocos   */
static void coalesce_blocks ()
{
    block_t *atual = free_list;

    while (atual)
    {
        // junta com o próximo enquanto der
        while (atual->next && atual->free && atual->next->free)
        {
            atual->size += sizeof(block_t) + atual->next->size;
            atual->next = atual->next->next;
        }
            
        atual = atual->next;
        
    }
}

/*   Free mínimo   */

void kfree(void *ptr)
{

    if (!ptr) 
        return;

    /* verifica se esta dentro do heap */
    if ((uint8_t*)ptr < heap_base || (uint8_t*)ptr >= heap_base + HEAP_SIZE)
        return;

    block_t *block = (block_t*)ptr - 1;

    // o bloco precisa existir na lista
    block_t *atual = free_list;
    int achou = 0;

    while (atual)
    {
        if (atual == block)
        {
            achou = 1;
            break;
        }
        atual = atual->next;
    }

    if (!achou)
        return;

    block->free = 1;

    coalesce_blocks(); // tenta juntar blocos livres que estao lado a lado
}

/*   Estatísticas   */

/* Faz o cálculo da memória usada */
uint64_t memory_used(void)
{   
    uint64_t usado = 0;
    block_t *atual = free_list;

    // passa pela lista de blocos para calcular o total usado
    while (atual)
    {
        if (!atual->free)
            usado += atual->size + sizeof(block_t); // conta tudo

        atual = atual->next;
    }

    return usado;
}

/* E essa faz o cálculo da memória livre */
uint64_t memory_free(void)
{   
    uint64_t memoria_livre = 0;
    block_t *atual = free_list;

    while (atual)
    {
        if (atual->free)
            memoria_livre += atual->size;

        atual = atual->next;
    }

    return memoria_livre;
}

/* Faz o cálculo da memória total do heap */
uint64_t memory_total(void)
{
    return HEAP_SIZE;
}

/* Imprime o estado do heap: endereços, tamanhos e status dos blocos */
void heap_dump(void)
{
    uart_print("--- HEAP DUMP ---\n");

    block_t *atual = free_list;
    int idx = 0;

    while (atual)
    {
        uart_print("Block ");
        uart_print_uint(idx++);
        uart_print(": addr ");
        uart_print_uint((uint64_t)atual);
        uart_print(" size ");
        uart_print_uint(atual->size);
        uart_print(" bytes - ");
        uart_print(atual->free ? "FREE\n" : "USED\n");

        atual = atual->next;
    }

    uart_print("--- END HEAP DUMP ---\n");
}