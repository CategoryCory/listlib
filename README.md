# listlib
`listlib` is a lightweight, simple, and efficient C library for dynamic arrays. It provides functionality for managing lists of dynamically allocated elements with features like automatic resizing, easy insertion/retrieval, and memory management. It is designed to be flexible and easy to integrate into C projects.
## Features
- Easy initialization and destruction of lists.
- Dynamic resizing of lists to accommodate variable amounts of data.
- Push and pop operations for adding or removing elements.
- Random access to elements by index for both reading and writing.
- Clear and reset list contents efficiently.
- Human-readable error messages for troubleshooting.

## Usage Overview
The library exposes the following core functionality:
- **Initialization**: Create a list with or without an initial capacity.
- **Adding Elements**: Push data onto the list or set it to a specific index.
- **Accessing Elements**: Retrieve data from the list by index or peek at the most recently added element.
- **Resize Management**: Automatic resizing to ensure the list can always hold new elements.
- **Error Handling**: Functions return meaningful error codes that can be converted into descriptive strings.

Refer to the documentation in the header file `list.h` for a detailed overview of all functions and their behavior.
### Example
A brief example of how to use the library:
``` c
#include "list.h"

int main() {
    list my_list;
    
    // Initialize a list to store integers
    if (list_init(&my_list, sizeof(int)) != LIST_SUCCESS) {
        printf("Failed to initialize list.\n");
        return 1;
    }
    
    int value = 42;
    list_push(&my_list, &value);  // Add an element to the list

    int result;
    list_get(&my_list, 0, &result);  // Access the first element
    printf("First element: %d\n", result);

    list_destroy(&my_list);  // Free memory
    return 0;
}
```
## Building and Installing
The project uses [CMake](https://cmake.org/) as the build system. Below are the instructions to build the project and integrate it into your workflow.
### Prerequisites
Ensure that you have the following tools installed on your system:
- A C compiler (e.g., GCC, Clang, MSVC).
- CMake (minimum version: 3.13).

### Building the Library
1. Clone the repository:
``` bash
   git clone https://github.com/CategoryCory/listlib.git
   cd listlib
```
1. Create a build directory and configure the project using CMake:
``` bash
   mkdir build
   cd build
   cmake ..
```
1. Build the project:
``` bash
   cmake --build .
```
This will create the library file (e.g., `liblistlib.a` or `listlib.dll`) in the build directory.
### Installing the Library
If you want to install the library system-wide, use the following command after building:
``` bash
sudo cmake --install .
```
> **Note**: Ensure to pass a custom installation path if you want to install it in a non-default location.
>

## API Documentation
Detailed documentation for the library can be found in the `list.h` header file. Some key functions include:
- **Initialization**:
    - `list_init`
    - `list_init_with_capacity`

- **Memory Management**:
    - `list_destroy`
    - `list_clear`

- **Data Operations**:
    - `list_push`
    - `list_pop`
    - `list_get`
    - `list_set`

- **Utilities**:
    - `list_error_to_string`

The API is straightforward, with error codes such as `LIST_OK`, `LIST_ERR_ALLOC`, and `LIST_OUT_OF_MEMORY` to indicate operation status.
## Contributing
Contributions to `listlib` are welcome! Feel free to open issues, fork the project, and submit pull requests.
When contributing:
1. Ensure your code follows best practices for C programming.
2. Write helpful comments for new functionality.
3. Add tests for new features.

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
## Acknowledgments
This library was built to simplify the use of dynamic arrays in C projects and is inspired by common data structures found in standard libraries of higher-level languages.
