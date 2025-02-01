#include "core/mem.h"
#include "core/log.h"

#include "platform/platform.h"

#include <stdio.h>

#if MEM_TRACKING_ENABLED
typedef struct Mem_Stats {
	u64 total_allocated;
	u64 tagged_allocations[MEM_TAG_COUNT];
} Mem_Stats;

static Mem_Stats stats = {0};

static const char* mem_tag_strings[MEM_TAG_COUNT] = {
	"UNKNOWN ",
	"ARENA   ",
	"ARRAY   ",
	"DARRAY  ",
	"STRING  ",
	"HTABLE  ",
	"RINGBUF ",
	"PLATFORM",
	"ENGINE  ",
	"RENDER  ",
	"EDITOR  ",
	"APP     ",
};

static char* get_usage_string(void) {
	const int buffer_size = kib(10);
	static char buffer[buffer_size];
	int offset = 0;
	for (int i = 0; i < MEM_TAG_COUNT; i++) {
		u64 alloc_size = stats.tagged_allocations[i];
		if (alloc_size == 0) {
			continue;
		}
		char unit[4] = "XiB";
		f32 amount;
		if (alloc_size >= gib(1)) {
			unit[0] = 'G';
			amount = (f32)alloc_size / (f32)gib(1);
		} else if (alloc_size >= mib(1)) {
			unit[0] = 'M';
			amount = (f32)alloc_size / (f32)mib(1);
		} else if (alloc_size >= kib(1)) {
			unit[0] = 'K';
			amount = (f32)alloc_size / (f32)kib(1);
		} else {
			unit[0] = 'B';
			unit[1] = '\0';
			amount = (f32)alloc_size;
		}
		offset += snprintf(buffer + offset, buffer_size - offset, "%s : %6.2f %s\n", mem_tag_strings[i], amount, unit);
	}
	return buffer;
}
#endif

void _sys_mem_init(void) {
}

void _sys_mem_shutdown(void) {
#if MEM_TRACKING_ENABLED
	if (stats.total_allocated > 0) {
		log_warn("Memory leak detected\n%s", get_usage_string());
	}
#endif
}

void* mem_alloc(u64 size, Mem_Tag tag) {
#if MEM_TRACKING_ENABLED
	if (tag == MEM_TAG_UNKNOWN) {
		log_warn("Allocating memory with unknown tag");
	}

	stats.total_allocated += size;
	stats.tagged_allocations[tag] += size;
#endif

	// TODO: Memory alignment
	void* block = platform_mem_alloc(size, false);
#if MEM_ZERO_ON_ALLOC_ENABLED
	mem_zero(block, size);
#endif
	return block;
}

void mem_free(void* block, u64 size, Mem_Tag tag) {
#if MEM_TRACKING_ENABLED
	if (tag == MEM_TAG_UNKNOWN) {
		log_warn("Freeing memory with unknown tag");
	}

	stats.total_allocated -= size;
	stats.tagged_allocations[tag] -= size;
#endif
	// TODO: Memory alignment
	platform_mem_free(block, false);
}

void* mem_zero(void* block, u64 size) {
	return platform_mem_zero(block, size);
}

void* mem_copy(void* dest, const void* src, u64 size) {
	return platform_mem_copy(dest, src, size);
}

void* mem_set(void* dest, i32 value, u64 size) {
	return platform_mem_set(dest, value, size);
}
