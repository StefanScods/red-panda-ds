#ifndef COMMON_H
#define COMMON_H

/**
 * @brief Macro to delete and set a dynamic memory pointer back to nullptr.
 */
#define DELETE_DYNAMIC_POINTER(x) \
    if (x != nullptr) {           \
        delete x;                 \
    }                             \
    x = nullptr;
#define DELETE_DYNAMIC_ARRAY_POINTER(x) \
    if (x != nullptr) {               \
        delete[] x;                   \
    }                                 \
    x = nullptr;
#endif
