#include "haunt/core/arena.h"

#include "platform.h"

#include "haunt/core/assert.h"

struct Arena {
	u8* base;
	u64 size;
	u64 used;
};

u64 get_aligned_offset(u64 size, u64 alignment) {
	return (size + alignment - 1) & ~(alignment - 1);
}

u64 get_arena_alloc_size(u64 size, u64 alignment) {
	return get_aligned_offset(sizeof(Arena), alignof(Arena)) + get_aligned_offset(size, alignment);
}

u64 get_arena_alloc_base_offset(u64 size, u64 alignment) {
	return get_aligned_offset(sizeof(Arena), alignof(Arena));
}

Arena* arena_create(u64 size) {
	Arena* arena = platform_memory_alloc(get_arena_alloc_size(size, alignof(Arena)), true);
	arena->base = (u8*)arena + get_arena_alloc_base_offset(size, alignof(Arena));
	arena->size = size;
	arena->used = 0;
	return arena;
}

void arena_destroy(Arena* arena) {
	platform_memory_free(arena->base, true);
}

void* _arena_push(Arena* arena, u64 size, u64 alignment) {
	assert(arena->used + size <= arena->size);
	u64 offset = (arena->used + alignment - 1) & ~(alignment - 1);
	arena->used = offset + size;
	return arena->base + offset;
}

void arena_pop(Arena* arena, u64 size) {
	assert(arena->used >= size);
	arena->used -= size;
}

void arena_clear(Arena* arena) {
	arena->used = 0;
}
