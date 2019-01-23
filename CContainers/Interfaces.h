#ifndef __CUTILS_CCONTAINERS_UTILS_H__
#define __CUTILS_CCONTAINERS_UTILS_H__

#include <ShortTypes.h>

typedef void *VirtualHandle;

typedef struct {
    /// Must be O(1).
    VirtualHandle (*Next) (VirtualHandle current);
    /// Must be O(1).
    void **(*Value) (VirtualHandle iterator);
} ISignleDirectionalIterator;

typedef struct {
    /// Must be O(1).
    VirtualHandle (*Next) (VirtualHandle current);
    /// Must be O(1).
    VirtualHandle (*Previous) (VirtualHandle current);
    /// Must be O(1).
    void **(*Value) (VirtualHandle iterator);
} IBiDirectionalIterator;

typedef struct {
    /// Must be O(1).
    ulint (*Size) (VirtualHandle container);
} ISizedContainer;

typedef struct {
    /// Must be O(1).
    VirtualHandle (*Begin) (VirtualHandle container);
    /// Must be O(1).
    VirtualHandle (*End) (VirtualHandle container);
    /// Variable complexity: O(1) for vectors and O(Size) for lists.
    VirtualHandle (*At) (VirtualHandle container, ulint index);
} IIterableContainer;

typedef struct {
    /// Must be O(1).
    VirtualHandle (*Insert) (VirtualHandle container, VirtualHandle position, void *value);
    /// Must be O(1).
    VirtualHandle (*Erase) (VirtualHandle container, VirtualHandle position);
} IMutableContainer;

typedef struct {
    /// Variable complexity.
    void (*Push) (VirtualHandle container, void *value);
    /// Must be O(1).
    const void *(*Top) (VirtualHandle container);
    /// Must be O(1).
    void (*Pop) (VirtualHandle container);
} IOrganizerContainer;

typedef struct {
    /// Variable complexity.
    char (*ContainsKey) (VirtualHandle container, void *key);
    /// Variable complexity.
    char (*Insert) (VirtualHandle container, void *key, void *value);
    /// Variable complexity.
    void **(*Get) (VirtualHandle container, void *key);
    /// Variable complexity.
    char (*Erase) (VirtualHandle container, void *key,
            void (*KeyDestructor) (void **), void (*ValueDestructor) (void **));
} IMapContainer;

#endif // __CUTILS_CCONTAINERS_UTILS_H__
