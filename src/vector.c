#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct vec_vector {
  size_t capacity;
  size_t len;
  size_t item_size;
  char *vec;
} vec_Vector;

// Initializes a new vector with items of sizeof(T)
vec_Vector *vec_new(size_t item_size) {
  vec_Vector *vector = malloc(sizeof(vec_Vector));
  if (!vector) {
    return NULL;
  }

  *vector = (vec_Vector){
      .capacity = 0,
      .len = 0,
      .item_size = item_size,
      .vec = NULL,
  };

  return vector;
}

// Resizes vector to fit length
bool vec_fit(vec_Vector *restrict vec) {
  const size_t power = ceilf(log2f(vec->len + 1));
  const size_t new_capacity = vec->item_size * powf(2, power);

  if (new_capacity < vec->capacity || new_capacity > vec->capacity) {
    void *tmp = realloc(vec->vec, new_capacity);
    if (!tmp) {
      return false;
    }
    vec->vec = tmp;
    vec->capacity = new_capacity;
  }
  return true;
}

// Pushes a value to vector
bool vec_push(vec_Vector *restrict vec, void *item) {
  if (vec->len == 0 && vec->capacity == 0) {
    vec->vec = malloc(vec->item_size);
    if (!vec->vec) {
      return false;
    }

    memcpy(vec->vec, item, vec->item_size);
    vec->len = 1;
    vec->capacity = vec->item_size;

    return true;
  }

  vec_fit(vec);
  memcpy(vec->vec + vec->item_size * vec->len, item, vec->item_size);
  vec->len++;

  return true;
}

// Returns how many items are in the vector
size_t vec_len(const vec_Vector *vec) { return vec->len; }

// Returns the total amount of items that can currently be stored
size_t vec_capacity(const vec_Vector *vec) { return vec->capacity; }

// Removes an item from the end of the vector and assigns it to dest
bool vec_pop(vec_Vector *restrict vec, void *dest) {
  if (!vec->vec || vec->len <= 0 || vec->capacity <= 0) {
    return false;
  }

  vec->len--;
  if (dest) {
    memcpy(dest, vec->vec + vec->item_size * vec->len, vec->item_size);
  }
  vec_fit(vec);
  return true;
}

// Clears out all the memory used by the vector
void vec_free(vec_Vector *restrict vec) {
  free(vec->vec);
  free(vec);
}

// Returns item at index
bool vec_get(const vec_Vector *vec, const size_t index, void *restrict dest) {
  if (!vec->vec || index >= vec->len || vec->capacity == 0) {
    return false;
  }

  memcpy(dest, vec->vec + vec->item_size * index, vec->item_size);
  return true;
}

// Prints the vector with the specified format specifier
void vec_printf(const char *restrict fmt, const vec_Vector *vec) {
  // TODO
  printf("[");
  for (size_t i = 0; i < vec_len(vec); i++) {
    printf(" ");
    printf(fmt, vec->vec[i * vec->item_size]);
  }
  printf(" ]\n");
}

// Inserts a new item at index
bool vec_insert(vec_Vector *vec, const size_t index, void *item) {
  size_t len = vec->len - 1; // length before growing
  if (index > len) {
    return false;
  }

  vec_push(vec, vec->vec + len * vec->item_size);

  for (size_t i = len; i > index; i--) {
    memcpy(vec->vec + (i - 1) * vec->item_size, vec->vec + i * vec->item_size,
           vec->item_size);
  }

  memcpy(vec->vec + vec->item_size * index, item, vec->item_size);
  return true;
}

// Removes an item at index
bool vec_remove(vec_Vector *restrict vec, const size_t index, void *dest) {
  size_t len = vec->len - 1;
  if (index > len) {
    return false;
  }

  if (dest) {
    memcpy(dest, vec->vec + index * vec->item_size, vec->item_size);
  }

  for (size_t i = index; i < vec->len; i++) {
    if (i + 1 < vec->len) {

      memcpy(vec->vec + vec->item_size * i, vec->vec + vec->item_size * (i + 1),
             vec->item_size);
    }
  }

  return vec_pop(vec, NULL);
}

bool vec_is_empty(const vec_Vector *vec) {
  if (vec->len == 0 || vec->capacity == 0 || !vec->vec) {
    return true;
  }

  return false;
}

// Appends vector from src to dest
bool vec_append(vec_Vector *restrict dest, const vec_Vector *src) {
  for (size_t i = 0; i < src->len; i++) {
    if (!vec_push(dest, src->vec + src->item_size * i)) {
      return false;
    }
  }

  return true;
}

vec_Vector *vec_from(void *const array, size_t length, size_t item_size) {
  vec_Vector *vec = vec_new(item_size);
  for (size_t i = 0; i < length; i++) {
    if (!vec_push(vec, array + i * item_size)) {
      vec_free(vec);
      return NULL;
    }
  }
  return vec;
}
