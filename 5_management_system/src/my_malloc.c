#include "my_malloc.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SIZE 4096
typedef struct block
{
    size_t size;
    struct block *next;
} block_t;

// Define a global variable to keep track of the memory pool
static void *memory_pool = NULL;

// Initialize the memory pool
void initialize_memory_pool(size_t size)
{
    memory_pool = malloc(size);
    if (memory_pool == NULL)
    {
        // Handle allocation failure
        exit(EXIT_FAILURE);
    }

    // Initialize the first block in the pool
    block_t *first_block = (block_t *)memory_pool;
    first_block->size = size - sizeof(block_t);
    first_block->next = NULL;
}

void *my_malloc(size_t size)
{
    if (memory_pool == NULL)
    {
        initialize_memory_pool(DEFAULT_SIZE);
    }

    // Find a suitable block in the memory pool
    block_t *current_block = (block_t *)memory_pool;
    block_t *previous_block = NULL;

    while (current_block)
    {
        if (current_block->size >= size)
        {
            // Allocate memory from this block
            if (current_block->size > size + sizeof(block_t))
            {
                // Split the block if it's larger than needed
                block_t *new_block = (block_t *)((char *)current_block + sizeof(block_t) + size);
                new_block->size = current_block->size - size - sizeof(block_t);
                new_block->next = current_block->next;

                current_block->size = size;
                current_block->next = new_block;
            }

            return (void *)((char *)current_block + sizeof(block_t));
        }

        previous_block = current_block;
        current_block = current_block->next;
    }

    size_t pool_size = (previous_block == NULL) ? DEFAULT_SIZE : previous_block->size * 2;
    initialize_memory_pool(pool_size);

    return my_malloc(size); // Try allocating again
}

void my_free(void *ptr)
{
    if (ptr == NULL)
    {
        return;
    }
    block_t *block_to_free = (block_t *)((char *)ptr - sizeof(block_t));
    block_to_free->next = NULL;
}

void *my_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
    {
        // If the pointer is NULL, equivalent to malloc
        return my_malloc(size);
    }

    if (size == 0)
    {
        // If the size is 0, equivalent to free
        my_free(ptr);
        return NULL;
    }

    // Reallocate memory by first allocating a new block
    void *new_ptr = my_malloc(size);

    if (new_ptr != NULL)
    {
        // Copy data from the old block to the new block
        block_t *old_block = (block_t *)((char *)ptr - sizeof(block_t));
        size_t copy_size = (size < old_block->size) ? size : old_block->size;
        memcpy(new_ptr, ptr, copy_size);

        // Free the old block
        my_free(ptr);
    }

    return new_ptr;
}
