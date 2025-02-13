#ifndef HAUNT_CORE_ARENA_H
#define HAUNT_CORE_ARENA_H

/**
 * The arena module provides a way to allocate memory in a fast and efficient way by using memory
 * arenas.
 *
 * Memory arenas are used to allocate memory in a linear fashion, which means that memory
 * allocations are fast and deallocations are done in a single operation.
 *
 * @file arena.h
 */

#include "haunt/core/export.h"
#include "haunt/core/types.h"

#include <stdalign.h>

/**
 * Opaque structure that represents an arena.
 */
typedef struct Arena Arena;

/**
 * Allocates and creates a new arena.
 *
 * @param size The size of the arena in bytes.
 */
export Arena* arena_create(u64 size);

/**
 * Destroys the arena and frees the memory.
 *
 * @param arena The arena to destroy.
 */
export void arena_destroy(Arena* arena);

/**
 * Allocates memory
 *
 * @param arena The arena to allocate memory from.
 * @param T The type of the memory to allocate.
 */
#define arena_push(arena, T) (T*)_arena_push(arena, sizeof(T), alignof(T))

export void arena_pop(Arena* arena, u64 size);

export void arena_clear(Arena* arena);

/**
 * @internal
 */

export void* _arena_push(Arena* arena, u64 size, u64 alignment);

#endif // HAUNT_CORE_ARENA_H
