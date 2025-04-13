#include <stddef.h>
#include <string.h>
#include "list.h"

/**
 * @defgroup list_internal Internal List Functions
 * @brief Helper functions used internally by the list implementation.
 * @internal
 * @{
 */

/**
 * @ingroup list_internal
 * @brief Resizes the internal buffer of the list to a new capacity.
 * @internal
 *
 * This function reallocates the internal buffer to match the specified
 * capacity. Any existing elements are preserved during the resizing process.
 *
 * @param lst Pointer to the list.
 * @param new_capacity The new capacity for the list.
 * @return `LIST_OK` on success, `LIST_ERR_ALLOC` if memory allocation fails.
 */
static list_status list_resize(list* lst, size_t new_capacity);

/**
 * @ingroup list_internal
 * @brief Grows the capacity of the list when it reaches its current limit.
 * @internal
 *
 * This function doubles the capacity of the list to accommodate additional
 * elements when needed. It is typically called internally by `list_push`.
 *
 * @param lst Pointer to the list.
 * @return `LIST_OK` on success, `LIST_ERR_ALLOC` if memory allocation fails.
 */
static list_status list_grow(list* lst);

/** @} */ // end of list_internal

list_status list_init(list* lst, const size_t elem_size) {
    return list_init_with_capacity(lst, 0, elem_size);
}

list_status list_init_with_capacity(list* lst, const size_t capacity, const size_t elem_size) {
    if (lst == nullptr || elem_size == 0) return LIST_ERR_INVALID;

    lst->data = nullptr;
    lst->size = 0;
    lst->capacity = 0;
    lst->elem_size = elem_size;

    if (capacity == 0) return LIST_OK;

    lst->data = calloc(capacity, elem_size);
    if (lst->data == nullptr) return LIST_ERR_ALLOC;

    lst->capacity = capacity;
    return LIST_OK;
}

void list_destroy(list* lst) {
    if (lst == nullptr) return;

    free(lst->data);
    lst->data = nullptr;
    lst->size = 0;
    lst->capacity = 0;
}

size_t list_size(const list* lst) {
    return lst != nullptr ? lst->size : 0;
}

size_t list_capacity(const list* lst) {
    return lst != nullptr ? lst->capacity : 0;
}

list_status list_push(list* lst, const void* value) {
    if (lst == nullptr || value == nullptr) return LIST_ERR_INVALID;

    if (lst->size >= lst->capacity) {
        const list_status err = list_grow(lst);
        if (err != LIST_OK) return err;
    }

    void* dest = (uint8_t*) lst->data + lst->size * lst->elem_size;
    memcpy(dest, value, lst->elem_size);
    lst->size++;

    return LIST_OK;
}

list_status list_pop(list* lst, void* out_value) {
    if (lst->data == nullptr || lst->size == 0) return LIST_ERR_INVALID;

    lst->size--;

    if (out_value != nullptr) {
        const void* src = (const uint8_t*) lst->data + lst->size * lst->elem_size;
        memcpy(out_value, src, lst->elem_size);
    }

    // Shrink if we're using significantly less than the capacity
    if (lst->capacity > 1 && lst->size < lst->capacity / 4) {
        size_t new_capacity = lst->capacity / 2;

        // Don't allow a new capacity less than 1
        if (new_capacity < 1) new_capacity = 1;

        const list_status err = list_resize(lst, new_capacity);
        if (err != LIST_OK) return err;
    }

    return LIST_OK;
}

list_status list_peek(const list* lst, void* out_value) {
    if (lst->data == nullptr || lst->size == 0 || out_value == nullptr) return LIST_ERR_INVALID;

    return list_get(lst, lst->size - 1, out_value);
}

list_status list_get(const list* lst, const size_t index, void* out_value) {
    if (lst == nullptr || lst->data == nullptr || out_value == nullptr) return LIST_ERR_INVALID;

    if (index >= lst->size) return LIST_OUT_OF_BOUNDS;

    const void* src = (const uint8_t*) lst->data + index * lst->elem_size;
    memcpy(out_value, src, lst->elem_size);

    return LIST_OK;
}

list_status list_set(const list* lst, const size_t index, const void* value) {
    if (lst == nullptr || lst->data == nullptr) return LIST_ERR_INVALID;

    if (index >= lst->size) return LIST_OUT_OF_BOUNDS;

    void* dest = (uint8_t*) lst->data + index * lst->elem_size;
    memcpy(dest, value, lst->elem_size);

    return LIST_OK;
}

void list_clear(list* lst) {
    if (lst == nullptr) return;

    lst->size = 0;
}

const char* list_error_to_string(const list_status err) {
    switch (err) {
        case LIST_OK:            return "No error";
        case LIST_ERR_ALLOC:     return "Error allocating memory";
        case LIST_ERR_INVALID:   return "Invalid input";
        case LIST_OUT_OF_BOUNDS: return "Out of bounds access attempted";
        default:                 return "Unknown error";
    }
}

static list_status list_resize(list* lst, const size_t new_capacity) {
    if (lst == nullptr || new_capacity == 0) return LIST_ERR_INVALID;

    if (new_capacity == lst->capacity) return LIST_OK;  // No change needed

    void* new_data = realloc(lst->data, new_capacity * lst->elem_size);
    if (new_data == nullptr) return LIST_ERR_ALLOC;

    // Zero out new section
    if (new_capacity > lst->capacity) {
        const size_t diff = new_capacity - lst->capacity;
        memset((uint8_t*) new_data + lst->capacity * lst->elem_size, 0, diff * lst->elem_size);
    }

    lst->data = new_data;
    lst->capacity = new_capacity;

    // Adjust size if the internal data array shrunk
    if (lst->size > new_capacity) {
        lst->size = new_capacity;
    }

    return LIST_OK;
}

static list_status list_grow(list* lst) {
    const size_t new_capacity = lst->capacity == 0 ? 1 : lst->capacity * 2;
    return list_resize(lst, new_capacity);
}
