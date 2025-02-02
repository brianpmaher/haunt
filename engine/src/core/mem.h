#pragma once

#include "core/api.h"
#include "core/types.h"

#define MEM_TRACKING_ENABLED      1
#define MEM_ZERO_ON_ALLOC_ENABLED 1

#define byte(x) ((u64)(x))
#define kib(x) (byte(x) * 1024)
#define mib(x) (kib(x) * 1024)
#define gib(x) (mib(x) * 1024)
#define tib(x) (gib(x) * 1024)
#define pib(x) (tib(x) * 1024)

typedef enum Mem_Tag {
	MEM_TAG_UNKNOWN,
	// Allocators
	MEM_TAG_ARENA,
	// Collections
	MEM_TAG_ARRAY,
	MEM_TAG_DARRAY,
	MEM_TAG_STRING,
	MEM_TAG_HTABLE,
	MEM_TAG_RINGBUF,
	// Engine subsystems
	MEM_TAG_PLATFORM,
	MEM_TAG_ENGINE,
	MEM_TAG_RENDER,
	MEM_TAG_EDITOR,
	// Application
	MEM_TAG_APP,
	// Count
	MEM_TAG_COUNT,
} Mem_Tag;

void memory_system_init(void);

void memory_system_shutdown(void);

HAPI void* mem_alloc(u64 size, Mem_Tag tag);

HAPI void mem_free(void* block, u64 size, Mem_Tag tag);

HAPI void* mem_zero(void* block, u64 size);

HAPI void* mem_copy(void* dest, const void* src, u64 size);

HAPI void* mem_set(void* dest, i32 value, u64 size);
