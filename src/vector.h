#ifndef VECTOR_H
#define VECTOR_H
#include <stdbool.h>
#include <stddef.h>

typedef struct vec_vector vec_Vector;

// Initializes a new vector with items of sizeof(T)
vec_Vector *vec_new(size_t item_size);

// Resizes vector to fit length
bool vec_fit(vec_Vector *restrict vec);

// Pushes a value to vector
bool vec_push(vec_Vector *restrict vec, void *item);

// Returns how many items are in the vector
size_t vec_len(const vec_Vector *vec);

// Returns the total amount of items that can currently be stored
size_t vec_capacity(const vec_Vector *vec);

// Removes an item from the end of the vector and assigns it to dest
bool vec_pop(vec_Vector *restrict vec, void *dest);

// Clears out all the memory used by the vector
void vec_free(vec_Vector *restrict vec);

// Returns item at index
bool vec_get(const vec_Vector *vec, const size_t index, void *restrict dest);

// Prints the vector with the specified format specifier
void vec_printf(const char *restrict fmt, const vec_Vector *vec);

// Inserts a new item at index
bool vec_insert(vec_Vector *vec, const size_t index, void *item);

// Removes an item at index
bool vec_remove(vec_Vector *restrict vec, const size_t index, void *dest);

bool vec_is_empty(const vec_Vector *vec);

// Appends vector from src to dest
bool vec_append(vec_Vector *restrict dest, const vec_Vector *src);

// converts an array into a vector
vec_Vector *vec_from(void *const array, size_t length, size_t item_size);

#endif
