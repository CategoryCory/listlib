#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/**
 * @brief A dynamically-sized array implementation for generic data.
 *
 * This struct represents a list that stores elements of any type by value.
 * It automatically grows and shrinks its internal buffer as elements are
 * added and removed.
 *
 * Each element is stored as a contiguous block of memory of size `elem_size`.
 * The user is responsible for passing pointers to correctly-sized data
 * when pushing or setting values.
 *
 * Use this in conjunction with the `list_` function family for allocation,
 * deallocation, insertion, and access.
 *
 * @var list::data
 *      Pointer to the internal buffer that stores the list's elements.
 *      Memory is dynamically allocated and managed by the `list_` functions.
 *
 * @var list::size
 *      The number of elements currently stored in the list.
 *      This value is updated automatically as elements are added or removed.
 *
 * @var list::capacity
 *      The total number of elements the list can hold before resizing is needed.
 *      The capacity is automatically adjusted as needed to accommodate new elements.
 *
 * @var list::elem_size
 *      The size of each element in bytes. This is determined at initialization
 *      and must match the size of the data being stored.
 *
 * ### Example Usage
 * @code
 * list my_list;
 * if (list_init(&my_list, sizeof(int)) != LIST_OK) {
 *     // Handle initialization error
 * }
 *
 * int value = 10;
 * if (list_push(&my_list, &value) != LIST_OK) {
 *     // Handle memory error
 * }
 *
 * // Access the first element
 * int out_value;
 * if (list_get(&my_list, 0, &out_value) == LIST_OK) {
 *     printf("Value: %d\n", out_value);
 * } else {
 *     printf("Error getting value.\n");
 * }
 *
 * list_destroy(&my_list);
 * @endcode
 */
typedef struct list {
    void*  data;
    size_t size;
    size_t capacity;
    size_t elem_size;
} list;

/**
 * @enum list_status
 * @brief Represents status codes for operations on a dynamic list.
 *
 * The `list_status` enum provides constants that describe the success
 * or failure of operations performed on a `list` data structure.
 * These codes enable error handling and debugging for various list
 * manipulation functions.
 *
 * @var LIST_OK
 *      Indicates that the operation completed successfully without errors.
 *
 * @var LIST_ERR_ALLOC
 *      Indicates a memory allocation failure.
 *      This error is typically returned by functions like `list_init` or
 *      `list_push` when dynamic memory allocation fails.
 *
 * @var LIST_ERR_INVALID
 *      Indicates that an invalid list or invalid arguments were passed to
 *      a function. For example, this is returned when a `NULL` pointer is
 *      supplied to an operation expecting a valid list.
 *
 * @var LIST_OUT_OF_BOUNDS
 *      Indicates an attempt to access an index or element that exceeds the
 *      bounds of the list. This is commonly returned by functions like
 *      `list_get` when accessing indices greater than or equal to the total
 *      number of elements in the list.
 *
 * Example usage:
 * @code
 * list my_list;
 * if (list_init(&my_list, sizeof(int)) != LIST_OK) {
 *     // Handle initialization failure
 * }
 *
 * int value = 42;
 * if (list_push(&my_list, &value) != LIST_OK) {
 *     // Handle memory allocation failure
 * }
 * @endcode
 */
typedef enum {
    LIST_OK            = 0,
    LIST_ERR_ALLOC     = 1,
    LIST_ERR_INVALID   = 2,
    LIST_OUT_OF_BOUNDS = 3,
} list_status;

/**
 * @brief Initializes a list with a specified element size.
 *
 * @param lst Pointer to the list to initialize.
 * @param elem_size Size of each element in bytes.
 * @return `LIST_OK` on success, `LIST_ERR_ALLOC` on allocation failure.
 */
list_status list_init(list* lst, size_t elem_size);

/**
 * @brief Initializes a list with a specified element size and initial capacity.
 *
 * @param lst Pointer to the list to initialize.
 * @param capacity Initial number of elements the list can hold.
 * @param elem_size Size of each element in bytes.
 * @return `LIST_OK` on success, `LIST_ERR_ALLOC` on allocation failure.
 */
list_status list_init_with_capacity(list* lst, size_t capacity, size_t elem_size);

/**
 * @brief Releases resources used by the list.
 *
 * @param lst Pointer to the list to destroy.
 */
void list_destroy(list* lst);

/**
 * @brief Gets the current size of the list (number of elements stored).
 *
 * @param lst Pointer to the list.
 * @return Number of elements in the list.
 */
size_t list_size(const list* lst);

/**
 * @brief Gets the current capacity of the list.
 *
 * @param lst Pointer to the list.
 * @return Number of elements the list can currently hold.
 */
size_t list_capacity(const list* lst);

/**
 * @brief Adds a new element to the end of the list.
 *
 * @param lst Pointer to the list.
 * @param value Pointer to the value to add.
 * @return `LIST_OK` on success, `LIST_ERR_ALLOC` if allocation fails.
 */
list_status list_push(list* lst, const void* value);

/**
 * @brief Removes the last element from the list and optionally retrieves its value.
 *
 * @param lst Pointer to the list.
 * @param out_value Pointer to where the popped value will be stored (optional, can be `NULL`).
 * @return `LIST_OK` on success, `LIST_ERR_INVALID` if the list is empty.
 */
list_status list_pop(list* lst, void* out_value);

/**
 * @brief Retrieves the value of the last element in the list without removing it.
 *
 * @param lst Pointer to the list.
 * @param out_value Pointer to where the value will be stored.
 * @return `LIST_OK` on success, `LIST_ERR_INVALID` if the list is empty.
 */
list_status list_peek(const list* lst, void* out_value);

/**
 * @brief Gets the value of an element at a specified index.
 *
 * @param lst Pointer to the list.
 * @param index Zero-based index of the element.
 * @param out_value Pointer to where the value will be stored.
 * @return `LIST_OK` on success, `LIST_OUT_OF_BOUNDS` if the index is invalid.
 */
list_status list_get(const list* lst, size_t index, void* out_value);

/**
 * @brief Sets the value of an element at a specified index.
 *
 * @param lst Pointer to the list.
 * @param index Zero-based index of the element.
 * @param value Pointer to the value to set at the specified index.
 * @return `LIST_OK` on success, `LIST_OUT_OF_BOUNDS` if the index is invalid.
 */
list_status list_set(const list* lst, size_t index, const void* value);

/**
 * @brief Clears all elements from the list without freeing its memory buffer.
 *
 * @param lst Pointer to the list.
 */
void list_clear(list* lst);

/**
 * @brief Converts a `list_status` code to a human-readable string.
 *
 * @param err The error code to convert.
 * @return A string representing the error message.
 */
const char* list_error_to_string(list_status err);

#endif //LIST_H
