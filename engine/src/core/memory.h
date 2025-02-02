#pragma once

#include "core/api.h"
#include "core/types.h"

#define MEMORY_TRACKING_ENABLED      1
#define MEMORY_ZERO_ON_ALLOC_ENABLED 1

#define byte(x) ((u64)(x))
#define kib(x) (byte(x) * 1024)
#define mib(x) (kib(x) * 1024)
#define gib(x) (mib(x) * 1024)
#define tib(x) (gib(x) * 1024)
#define pib(x) (tib(x) * 1024)

typedef enum Memory_Tag {
	MEMORY_TAG_UNKNOWN,
	// Allocators
	MEMORY_TAG_ARENA,
	// Collections
	MEMORY_TAG_ARRAY,
	MEMORY_TAG_DARRAY,
	MEMORY_TAG_STRING,
	MEMORY_TAG_HTABLE,
	// Engine subsystems
	MEMORY_TAG_PLATFORM,
	MEMORY_TAG_ENGINE,
	MEMORY_TAG_RENDER,
	MEMORY_TAG_EDITOR,
	// Application
	MEMORY_TAG_APP,
	// Count
	MEMORY_TAG_COUNT,
} Memory_Tag;

void memory_system_init(void);

void memory_system_shutdown(void);

HAPI void* memory_alloc(u64 size, Memory_Tag tag);

HAPI void memory_free(void* block, u64 size, Memory_Tag tag);

HAPI void* memory_zero(void* block, u64 size);

HAPI void* memory_copy(void* dest, const void* src, u64 size);

HAPI void* memory_set(void* dest, i32 value, u64 size);
