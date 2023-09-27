#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include <stddef.h>

/**
 * @brief Allocate memory of the specified size.
 *
 * This function allocates a block of memory of the given size and returns a
 * pointer to the first byte of the block.
 *
 * @param size The size (in bytes) of the memory to allocate.
 * 
 * @return A pointer to the allocated memory, or NULL if allocation fails.
 */
void *my_malloc(size_t size);

/**
 * @brief Free memory previously allocated with my_malloc.
 *
 * This function deallocates the memory block pointed to by `ptr`, which must
 * have been previously allocated using my_malloc or my_realloc.
 *
 * @param ptr A pointer to the memory block to free.
 */
void my_free(void *ptr);

/**
 * @brief Change the size of a previously allocated memory block.
 *
 * This function changes the size of the memory block pointed to by `ptr` to the
 * given `size`. The contents of the block are preserved up to the minimum of
 * the old and new sizes. If the new size is larger, the value of the newly
 * allocated portion is indeterminate.
 *
 * @param ptr A pointer to the previously allocated memory block.
 * 
 * @param size The new size (in bytes) for the memory block.
 * 
 * @return A pointer to the reallocated memory, or NULL if reallocation fails.
 */
void *my_realloc(void *ptr, size_t size);

#endif /* MY_MALLOC_H */
