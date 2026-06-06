#include "memory.h"
#include <stdint.h>

/*   Configuração do heap   */

#define HEAP_START 0x80400000UL
#define HEAP_SIZE  (8 * 1024 * 1024)   // 8 MB

/* Estrutura do bloco de memória */
typedef struct block {
    uint64_t size;
    struct block *next;
    int free;
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
    block_t *new_block = (block_t*)((uint8_t*)block + sizeof(block_t) + size); // bloco novo começa após o bloco atual

    new_block->size = block->size - size - sizeof(block_t); // tem o tamanho restante
    new_block->free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
}

/*   Alocador bump   */

void *kmalloc(uint64_t size)
{
    if (size == 0)
        return 0;

    /* Alinhamento para 8 bytes */
    size = (size + 7) & ~7ULL;

    block_t *aux = free_list; // passa pela lista de blocos livres

    // faz a divisão do bloco se precisar
    while (aux)
    {
        if (aux->free && aux->size >= size)
        {
            /* divisão do bloco */
            if (aux->size > size + sizeof(block_t))
                dividir_block(aux, size);

            aux->free = 0;

            return (void*)(aux + 1);
        }

        aux = aux->next;
    }

    return 0;
}

/*   Coalescência de blocos   */
static void coalesce_blocks ()
{
    block_t *aux = free_list;

    while (aux && aux->next)
    {
        if (aux->free && aux->next->free)
        {
            aux->size += sizeof(block_t) + aux->next->size;
            aux->next = aux->next->next;
        }
        else
        {
            aux = aux->next;
        }
    }
}

/*   Free mínimo   */

void kfree(void *ptr)
{

    if (!ptr) 
        return;

    block_t *block = (block_t*)ptr - 1;

    block->free = 1;

    coalesce_blocks(); // tenta juntar blocos livres que estao lado a lado
}

/*   Estatísticas   */

/* Faz o cálculo da memória usada */
uint64_t memory_used(void)
{   
    uint64_t usado = 0;
    block_t *aux = free_list;

    // passa pela lista de blocos para calcular o total usado
    while (aux)
    {
        if (!aux->free)
            usado += aux->size;

        aux = aux->next;
    }

    return usado;
}

/* E essa faz o cálculo da memória livre */
uint64_t memory_free(void)
{   
    uint64_t memoria_livre = 0;
    block_t *aux = free_list;

    while (aux)
    {
        if (aux->free)
            memoria_livre += aux->size;

        aux = aux->next;
    }

    return memoria_livre;
}

/* Faz o cálculo da memória total do heap */
uint64_t memory_total(void)
{
    return HEAP_SIZE;
}