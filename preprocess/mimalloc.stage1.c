/* ----------------------------------------------------------------------------
Copyright (c) 2018-2020, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif // 7
#if 0 // #if defined(__sun)
// // same remarks as os.c for the static's context.
// #undef _XOPEN_SOURCE
// #undef _POSIX_C_SOURCE
#endif // 10


// --- Amalgamation START --- mimalloc/include/mimalloc.h
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/
// #ifndef MIMALLOC_H
// #define MIMALLOC_H

#define MI_MALLOC_VERSION 188   // major + 2 digits minor

// ------------------------------------------------------
// Compiler specific attributes
// ------------------------------------------------------

#if 0 // #ifdef __cplusplus
#if 0 //   #if (__cplusplus >= 201103L) || (_MSC_VER > 1900)  // C++11
//     #define mi_attr_noexcept   noexcept
  #else
//     #define mi_attr_noexcept   throw()
  #endif // 34
#else
  #define mi_attr_noexcept
#endif // 33

#if 0 // #if defined(__cplusplus) && (__cplusplus >= 201703)
//   #define mi_decl_nodiscard    [[nodiscard]]
#elif (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__)  // includes clang, icc, and clang-cl
  #define mi_decl_nodiscard    __attribute__((warn_unused_result))
#elif defined(_HAS_NODISCARD)
  #define mi_decl_nodiscard    _NODISCARD
#elif 0 // #elif (_MSC_VER >= 1700)
//   #define mi_decl_nodiscard    _Check_return_
#else
  #define mi_decl_nodiscard
#endif // 43

#if 0 // #if defined(_MSC_VER) || defined(__MINGW32__)
#if 0 //   #if !defined(MI_SHARED_LIB)
//     #define mi_decl_export
#elif 0 //   #elif defined(MI_SHARED_LIB_EXPORT)
//     #define mi_decl_export              __declspec(dllexport)
  #else
//     #define mi_decl_export              __declspec(dllimport)
  #endif // 56
#if 0 //   #if defined(__MINGW32__)
//     #define mi_decl_restrict
//     #define mi_attr_malloc              __attribute__((malloc))
  #else
#if 0 //     #if (_MSC_VER >= 1900) && !defined(__EDG__)
//       #define mi_decl_restrict          __declspec(allocator) __declspec(restrict)
    #else
//       #define mi_decl_restrict          __declspec(restrict)
    #endif // 67
//     #define mi_attr_malloc
  #endif // 63
//   #define mi_cdecl                      __cdecl
//   #define mi_attr_alloc_size(s)
//   #define mi_attr_alloc_size2(s1,s2)
//   #define mi_attr_alloc_align(p)
#elif defined(__GNUC__)                 // includes clang and icc
  #if defined(MI_SHARED_LIB) && defined(MI_SHARED_LIB_EXPORT)
    #define mi_decl_export              __attribute__((visibility("default")))
  #else
    #define mi_decl_export
  #endif // 79
  #define mi_cdecl                      // leads to warnings... __attribute__((cdecl))
  #define mi_decl_restrict
  #define mi_attr_malloc                __attribute__((malloc))
  #if (defined(__clang_major__) && (__clang_major__ < 4)) || (__GNUC__ < 5)
    #define mi_attr_alloc_size(s)
    #define mi_attr_alloc_size2(s1,s2)
    #define mi_attr_alloc_align(p)
  #elif defined(__INTEL_COMPILER)
    #define mi_attr_alloc_size(s)       __attribute__((alloc_size(s)))
    #define mi_attr_alloc_size2(s1,s2)  __attribute__((alloc_size(s1,s2)))
    #define mi_attr_alloc_align(p)
  #else
    #define mi_attr_alloc_size(s)       __attribute__((alloc_size(s)))
    #define mi_attr_alloc_size2(s1,s2)  __attribute__((alloc_size(s1,s2)))
    #define mi_attr_alloc_align(p)      __attribute__((alloc_align(p)))
  #endif // 87
#else
  #define mi_cdecl
  #define mi_decl_export
  #define mi_decl_restrict
  #define mi_attr_malloc
  #define mi_attr_alloc_size(s)
  #define mi_attr_alloc_size2(s1,s2)
  #define mi_attr_alloc_align(p)
#endif // 55

// ------------------------------------------------------
// Includes
// ------------------------------------------------------

#include <stddef.h>     // size_t
#include <stdbool.h>    // bool

#if 0 // #ifdef __cplusplus
// extern "C" {
#endif // 117

// ------------------------------------------------------
// Standard malloc interface
// ------------------------------------------------------

mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_malloc(size_t size)  mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_calloc(size_t count, size_t size)  mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(1,2);
mi_decl_nodiscard mi_decl_export void* mi_realloc(void* p, size_t newsize)      mi_attr_noexcept mi_attr_alloc_size(2);
mi_decl_export void* mi_expand(void* p, size_t newsize)                         mi_attr_noexcept mi_attr_alloc_size(2);

mi_decl_export void mi_free(void* p) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_strdup(const char* s) mi_attr_noexcept mi_attr_malloc;
mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_strndup(const char* s, size_t n) mi_attr_noexcept mi_attr_malloc;
mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_realpath(const char* fname, char* resolved_name) mi_attr_noexcept mi_attr_malloc;

// ------------------------------------------------------
// Extended functionality
// ------------------------------------------------------
#define MI_SMALL_WSIZE_MAX  (128)
#define MI_SMALL_SIZE_MAX   (MI_SMALL_WSIZE_MAX*sizeof(void*))

mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_malloc_small(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_zalloc_small(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_zalloc(size_t size)       mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);

mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_mallocn(size_t count, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(1,2);
mi_decl_nodiscard mi_decl_export void* mi_reallocn(void* p, size_t count, size_t size)        mi_attr_noexcept mi_attr_alloc_size2(2,3);
mi_decl_nodiscard mi_decl_export void* mi_reallocf(void* p, size_t newsize)                   mi_attr_noexcept mi_attr_alloc_size(2);

mi_decl_nodiscard mi_decl_export size_t mi_usable_size(const void* p) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export size_t mi_good_size(size_t size)     mi_attr_noexcept;


// ------------------------------------------------------
// Internals
// ------------------------------------------------------

typedef void (mi_cdecl mi_deferred_free_fun)(bool force, unsigned long long heartbeat, void* arg);
mi_decl_export void mi_register_deferred_free(mi_deferred_free_fun* deferred_free, void* arg) mi_attr_noexcept;

typedef void (mi_cdecl mi_output_fun)(const char* msg, void* arg);
mi_decl_export void mi_register_output(mi_output_fun* out, void* arg) mi_attr_noexcept;

typedef void (mi_cdecl mi_error_fun)(int err, void* arg);
mi_decl_export void mi_register_error(mi_error_fun* fun, void* arg);

mi_decl_export void mi_collect(bool force)    mi_attr_noexcept;
mi_decl_export void mi_collect_reduce(size_t target_thread_owned) mi_attr_noexcept;
mi_decl_export int  mi_version(void)          mi_attr_noexcept;
mi_decl_export void mi_stats_reset(void)      mi_attr_noexcept;
mi_decl_export void mi_stats_merge(void)      mi_attr_noexcept;
mi_decl_export void mi_stats_print(void* out) mi_attr_noexcept;  // backward compatibility: `out` is ignored and should be NULL
mi_decl_export void mi_stats_print_out(mi_output_fun* out, void* arg) mi_attr_noexcept;

mi_decl_export void mi_process_init(void)     mi_attr_noexcept;
mi_decl_export void mi_thread_init(void)      mi_attr_noexcept;
mi_decl_export void mi_thread_done(void)      mi_attr_noexcept;
mi_decl_export void mi_thread_stats_print_out(mi_output_fun* out, void* arg) mi_attr_noexcept;

mi_decl_export void mi_process_info(size_t* elapsed_msecs, size_t* user_msecs, size_t* system_msecs,
                                    size_t* current_rss, size_t* peak_rss,
                                    size_t* current_commit, size_t* peak_commit, size_t* page_faults) mi_attr_noexcept;

// -------------------------------------------------------------------------------------
// Aligned allocation
// Note that `alignment` always follows `size` for consistency with unaligned
// allocation, but unfortunately this differs from `posix_memalign` and `aligned_alloc`.
// -------------------------------------------------------------------------------------

mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_malloc_aligned(size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_malloc_aligned_at(size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_zalloc_aligned(size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_zalloc_aligned_at(size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_calloc_aligned(size_t count, size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(1,2) mi_attr_alloc_align(3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_calloc_aligned_at(size_t count, size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(1,2);
mi_decl_nodiscard mi_decl_export void* mi_realloc_aligned(void* p, size_t newsize, size_t alignment) mi_attr_noexcept mi_attr_alloc_size(2) mi_attr_alloc_align(3);
mi_decl_nodiscard mi_decl_export void* mi_realloc_aligned_at(void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size(2);


// -------------------------------------------------------------------------------------
// Heaps: first-class, but can only allocate from the same thread that created it.
// -------------------------------------------------------------------------------------

struct mi_heap_s;
typedef struct mi_heap_s mi_heap_t;

mi_decl_nodiscard mi_decl_export mi_heap_t* mi_heap_new(void);
mi_decl_export void       mi_heap_delete(mi_heap_t* heap);
mi_decl_export void       mi_heap_destroy(mi_heap_t* heap);
mi_decl_export mi_heap_t* mi_heap_set_default(mi_heap_t* heap);
mi_decl_export mi_heap_t* mi_heap_get_default(void);
mi_decl_export mi_heap_t* mi_heap_get_backing(void);
mi_decl_export void       mi_heap_collect(mi_heap_t* heap, bool force) mi_attr_noexcept;

mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_malloc(mi_heap_t* heap, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_zalloc(mi_heap_t* heap, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_calloc(mi_heap_t* heap, size_t count, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(2, 3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_mallocn(mi_heap_t* heap, size_t count, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(2, 3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_malloc_small(mi_heap_t* heap, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2);

mi_decl_nodiscard mi_decl_export void* mi_heap_realloc(mi_heap_t* heap, void* p, size_t newsize)              mi_attr_noexcept mi_attr_alloc_size(3);
mi_decl_nodiscard mi_decl_export void* mi_heap_reallocn(mi_heap_t* heap, void* p, size_t count, size_t size)  mi_attr_noexcept mi_attr_alloc_size2(3,4);
mi_decl_nodiscard mi_decl_export void* mi_heap_reallocf(mi_heap_t* heap, void* p, size_t newsize)             mi_attr_noexcept mi_attr_alloc_size(3);

mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_heap_strdup(mi_heap_t* heap, const char* s)            mi_attr_noexcept mi_attr_malloc;
mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_heap_strndup(mi_heap_t* heap, const char* s, size_t n) mi_attr_noexcept mi_attr_malloc;
mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_heap_realpath(mi_heap_t* heap, const char* fname, char* resolved_name) mi_attr_noexcept mi_attr_malloc;

mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_malloc_aligned(mi_heap_t* heap, size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_malloc_aligned_at(mi_heap_t* heap, size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_zalloc_aligned(mi_heap_t* heap, size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_zalloc_aligned_at(mi_heap_t* heap, size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_calloc_aligned(mi_heap_t* heap, size_t count, size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(2, 3) mi_attr_alloc_align(4);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_calloc_aligned_at(mi_heap_t* heap, size_t count, size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(2, 3);
mi_decl_nodiscard mi_decl_export void* mi_heap_realloc_aligned(mi_heap_t* heap, void* p, size_t newsize, size_t alignment) mi_attr_noexcept mi_attr_alloc_size(3) mi_attr_alloc_align(4);
mi_decl_nodiscard mi_decl_export void* mi_heap_realloc_aligned_at(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size(3);


// --------------------------------------------------------------------------------
// Zero initialized re-allocation.
// Only valid on memory that was originally allocated with zero initialization too.
// e.g. `mi_calloc`, `mi_zalloc`, `mi_zalloc_aligned` etc.
// see <https://github.com/microsoft/mimalloc/issues/63#issuecomment-508272992>
// --------------------------------------------------------------------------------

mi_decl_nodiscard mi_decl_export void* mi_rezalloc(void* p, size_t newsize)                mi_attr_noexcept mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export void* mi_recalloc(void* p, size_t newcount, size_t size)  mi_attr_noexcept mi_attr_alloc_size2(2,3);

mi_decl_nodiscard mi_decl_export void* mi_rezalloc_aligned(void* p, size_t newsize, size_t alignment) mi_attr_noexcept mi_attr_alloc_size(2) mi_attr_alloc_align(3);
mi_decl_nodiscard mi_decl_export void* mi_rezalloc_aligned_at(void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export void* mi_recalloc_aligned(void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept mi_attr_alloc_size2(2,3) mi_attr_alloc_align(4);
mi_decl_nodiscard mi_decl_export void* mi_recalloc_aligned_at(void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size2(2,3);

mi_decl_nodiscard mi_decl_export void* mi_heap_rezalloc(mi_heap_t* heap, void* p, size_t newsize)                mi_attr_noexcept mi_attr_alloc_size(3);
mi_decl_nodiscard mi_decl_export void* mi_heap_recalloc(mi_heap_t* heap, void* p, size_t newcount, size_t size)  mi_attr_noexcept mi_attr_alloc_size2(3,4);

mi_decl_nodiscard mi_decl_export void* mi_heap_rezalloc_aligned(mi_heap_t* heap, void* p, size_t newsize, size_t alignment) mi_attr_noexcept mi_attr_alloc_size(3) mi_attr_alloc_align(4);
mi_decl_nodiscard mi_decl_export void* mi_heap_rezalloc_aligned_at(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size(3);
mi_decl_nodiscard mi_decl_export void* mi_heap_recalloc_aligned(mi_heap_t* heap, void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept mi_attr_alloc_size2(3,4) mi_attr_alloc_align(5);
mi_decl_nodiscard mi_decl_export void* mi_heap_recalloc_aligned_at(mi_heap_t* heap, void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size2(3,4);


// ------------------------------------------------------
// Analysis
// ------------------------------------------------------

mi_decl_export bool mi_heap_contains_block(mi_heap_t* heap, const void* p);
mi_decl_export bool mi_heap_check_owned(mi_heap_t* heap, const void* p);
mi_decl_export bool mi_check_owned(const void* p);

// An area of heap space contains blocks of a single size.
typedef struct mi_heap_area_s {
  void*  blocks;      // start of the area containing heap blocks
  size_t reserved;    // bytes reserved for this area (virtual)
  size_t committed;   // current available bytes for this area
  size_t used;        // number of allocated blocks
  size_t block_size;  // size in bytes of each block
  size_t full_block_size; // size in bytes of a full block including padding and metadata.
  int    heap_tag;    // heap tag associated with this area
} mi_heap_area_t;

typedef bool (mi_cdecl mi_block_visit_fun)(const mi_heap_t* heap, const mi_heap_area_t* area, void* block, size_t block_size, void* arg);

mi_decl_export bool mi_heap_visit_blocks(const mi_heap_t* heap, bool visit_blocks, mi_block_visit_fun* visitor, void* arg);

// Experimental
mi_decl_nodiscard mi_decl_export bool mi_is_in_heap_region(const void* p) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export bool mi_is_redirected(void) mi_attr_noexcept;

mi_decl_export int mi_reserve_huge_os_pages_interleave(size_t pages, size_t numa_nodes, size_t timeout_msecs) mi_attr_noexcept;
mi_decl_export int mi_reserve_huge_os_pages_at(size_t pages, int numa_node, size_t timeout_msecs) mi_attr_noexcept;

mi_decl_export int  mi_reserve_os_memory(size_t size, bool commit, bool allow_large) mi_attr_noexcept;
mi_decl_export bool mi_manage_os_memory(void* start, size_t size, bool is_committed, bool is_large, bool is_zero, int numa_node) mi_attr_noexcept;

mi_decl_export void mi_debug_show_arenas(bool show_inuse, bool show_abandoned, bool show_purge) mi_attr_noexcept;

// Experimental: heaps associated with specific memory arena's
typedef int mi_arena_id_t;
mi_decl_export void* mi_arena_area(mi_arena_id_t arena_id, size_t* size);
mi_decl_export int   mi_reserve_huge_os_pages_at_ex(size_t pages, int numa_node, size_t timeout_msecs, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept;
mi_decl_export int   mi_reserve_os_memory_ex(size_t size, bool commit, bool allow_large, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept;
mi_decl_export bool  mi_manage_os_memory_ex(void* start, size_t size, bool is_committed, bool is_large, bool is_zero, int numa_node, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept;

#if 0 // #if MI_MALLOC_VERSION >= 182
// // Create a heap that only allocates in the specified arena
// mi_decl_nodiscard mi_decl_export mi_heap_t* mi_heap_new_in_arena(mi_arena_id_t arena_id);
#endif // 304


// Experimental: allow sub-processes whose memory segments stay separated (and no reclamation between them)
// Used for example for separate interpreter's in one process.
typedef void* mi_subproc_id_t;
mi_decl_export mi_subproc_id_t mi_subproc_main(void);
mi_decl_export mi_subproc_id_t mi_subproc_new(void);
mi_decl_export void mi_subproc_delete(mi_subproc_id_t subproc);
mi_decl_export void mi_subproc_add_current_thread(mi_subproc_id_t subproc); // this should be called right after a thread is created (and no allocation has taken place yet)

// Experimental: visit abandoned heap areas (from threads that have been terminated)
mi_decl_export bool mi_abandoned_visit_blocks(mi_subproc_id_t subproc_id, int heap_tag, bool visit_blocks, mi_block_visit_fun* visitor, void* arg);

// Experimental: create a new heap with a specified heap tag. Set `allow_destroy` to false to allow the thread
// to reclaim abandoned memory (with a compatible heap_tag and arena_id) but in that case `mi_heap_destroy` will
// fall back to `mi_heap_delete`.
mi_decl_nodiscard mi_decl_export mi_heap_t* mi_heap_new_ex(int heap_tag, bool allow_destroy, mi_arena_id_t arena_id);

// deprecated
mi_decl_export int mi_reserve_huge_os_pages(size_t pages, double max_secs, size_t* pages_reserved) mi_attr_noexcept;

// Experimental: objects followed by a guard page.
// A sample rate of 0 disables guarded objects, while 1 uses a guard page for every object.
// A seed of 0 uses a random start point. Only objects within the size bound are eligable for guard pages.
mi_decl_export void mi_heap_guarded_set_sample_rate(mi_heap_t* heap, size_t sample_rate, size_t seed);
mi_decl_export void mi_heap_guarded_set_size_bound(mi_heap_t* heap, size_t min, size_t max);


// ------------------------------------------------------
// Convenience
// ------------------------------------------------------

#define mi_malloc_tp(tp)                ((tp*)mi_malloc(sizeof(tp)))
#define mi_zalloc_tp(tp)                ((tp*)mi_zalloc(sizeof(tp)))
#define mi_calloc_tp(tp,n)              ((tp*)mi_calloc(n,sizeof(tp)))
#define mi_mallocn_tp(tp,n)             ((tp*)mi_mallocn(n,sizeof(tp)))
#define mi_reallocn_tp(p,tp,n)          ((tp*)mi_reallocn(p,n,sizeof(tp)))
#define mi_recalloc_tp(p,tp,n)          ((tp*)mi_recalloc(p,n,sizeof(tp)))

#define mi_heap_malloc_tp(hp,tp)        ((tp*)mi_heap_malloc(hp,sizeof(tp)))
#define mi_heap_zalloc_tp(hp,tp)        ((tp*)mi_heap_zalloc(hp,sizeof(tp)))
#define mi_heap_calloc_tp(hp,tp,n)      ((tp*)mi_heap_calloc(hp,n,sizeof(tp)))
#define mi_heap_mallocn_tp(hp,tp,n)     ((tp*)mi_heap_mallocn(hp,n,sizeof(tp)))
#define mi_heap_reallocn_tp(hp,p,tp,n)  ((tp*)mi_heap_reallocn(hp,p,n,sizeof(tp)))
#define mi_heap_recalloc_tp(hp,p,tp,n)  ((tp*)mi_heap_recalloc(hp,p,n,sizeof(tp)))


// ------------------------------------------------------
// Options
// ------------------------------------------------------

typedef enum mi_option_e {
  // stable options
  mi_option_show_errors,                // print error messages
  mi_option_show_stats,                 // print statistics on termination
  mi_option_verbose,                    // print verbose messages
  // advanced options
  mi_option_eager_commit,               // eager commit segments? (after `eager_commit_delay` segments) (=1)
  mi_option_arena_eager_commit,         // eager commit arenas? Use 2 to enable just on overcommit systems (=2)
  mi_option_purge_decommits,            // should a memory purge decommit? (=1). Set to 0 to use memory reset on a purge (instead of decommit)
  mi_option_allow_large_os_pages,       // allow large (2 or 4 MiB) OS pages, implies eager commit. If false, also disables THP for the process.
  mi_option_reserve_huge_os_pages,      // reserve N huge OS pages (1GiB pages) at startup
  mi_option_reserve_huge_os_pages_at,   // reserve huge OS pages at a specific NUMA node
  mi_option_reserve_os_memory,          // reserve specified amount of OS memory in an arena at startup (internally, this value is in KiB; use `mi_option_get_size`)
  mi_option_deprecated_segment_cache,
  mi_option_deprecated_page_reset,
  mi_option_abandoned_page_purge,       // immediately purge delayed purges on thread termination
  mi_option_deprecated_segment_reset,
  mi_option_eager_commit_delay,         // the first N segments per thread are not eagerly committed (but per page in the segment on demand)
  mi_option_purge_delay,                // memory purging is delayed by N milli seconds; use 0 for immediate purging or -1 for no purging at all. (=10)
  mi_option_use_numa_nodes,             // 0 = use all available numa nodes, otherwise use at most N nodes.
  mi_option_disallow_os_alloc,          // 1 = do not use OS memory for allocation (but only programmatically reserved arenas)
  mi_option_os_tag,                     // tag used for OS logging (macOS only for now) (=100)
  mi_option_max_errors,                 // issue at most N error messages
  mi_option_max_warnings,               // issue at most N warning messages
  mi_option_max_segment_reclaim,        // max. percentage of the abandoned segments can be reclaimed per try (=10%)
  mi_option_destroy_on_exit,            // if set, release all memory on exit; sometimes used for dynamic unloading but can be unsafe
  mi_option_arena_reserve,              // initial memory size for arena reservation (= 1 GiB on 64-bit) (internally, this value is in KiB; use `mi_option_get_size`)
  mi_option_arena_purge_mult,           // multiplier for `purge_delay` for the purging delay for arenas (=10)
  mi_option_purge_extend_delay,
  mi_option_abandoned_reclaim_on_free,  // allow to reclaim an abandoned segment on a free (=1)
  mi_option_disallow_arena_alloc,       // 1 = do not use arena's for allocation (except if using specific arena id's)
  mi_option_retry_on_oom,               // retry on out-of-memory for N milli seconds (=400), set to 0 to disable retries. (only on windows)
  mi_option_visit_abandoned,            // allow visiting heap blocks from abandoned threads (=0)
  mi_option_guarded_min,                // only used when building with MI_GUARDED: minimal rounded object size for guarded objects (=0)
  mi_option_guarded_max,                // only used when building with MI_GUARDED: maximal rounded object size for guarded objects (=0)
  mi_option_guarded_precise,            // disregard minimal alignment requirement to always place guarded blocks exactly in front of a guard page (=0)
  mi_option_guarded_sample_rate,        // 1 out of N allocations in the min/max range will be guarded (=1000)
  mi_option_guarded_sample_seed,        // can be set to allow for a (more) deterministic re-execution when a guard page is triggered (=0)
  mi_option_target_segments_per_thread, // experimental (=0)
  _mi_option_last,
  // legacy option names
  mi_option_large_os_pages = mi_option_allow_large_os_pages,
  mi_option_eager_region_commit = mi_option_arena_eager_commit,
  mi_option_reset_decommits = mi_option_purge_decommits,
  mi_option_reset_delay = mi_option_purge_delay,
  mi_option_abandoned_page_reset = mi_option_abandoned_page_purge,
  mi_option_limit_os_alloc = mi_option_disallow_os_alloc
} mi_option_t;


mi_decl_nodiscard mi_decl_export bool mi_option_is_enabled(mi_option_t option);
mi_decl_export void mi_option_enable(mi_option_t option);
mi_decl_export void mi_option_disable(mi_option_t option);
mi_decl_export void mi_option_set_enabled(mi_option_t option, bool enable);
mi_decl_export void mi_option_set_enabled_default(mi_option_t option, bool enable);

mi_decl_nodiscard mi_decl_export long   mi_option_get(mi_option_t option);
mi_decl_nodiscard mi_decl_export long   mi_option_get_clamp(mi_option_t option, long min, long max);
mi_decl_nodiscard mi_decl_export size_t mi_option_get_size(mi_option_t option);
mi_decl_export void mi_option_set(mi_option_t option, long value);
mi_decl_export void mi_option_set_default(mi_option_t option, long value);


// -------------------------------------------------------------------------------------------------------
// "mi" prefixed implementations of various posix, Unix, Windows, and C++ allocation functions.
// (This can be convenient when providing overrides of these functions as done in `mimalloc-override.h`.)
// note: we use `mi_cfree` as "checked free" and it checks if the pointer is in our heap before free-ing.
// -------------------------------------------------------------------------------------------------------

mi_decl_export void  mi_cfree(void* p) mi_attr_noexcept;
mi_decl_export void* mi__expand(void* p, size_t newsize) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export size_t mi_malloc_size(const void* p)        mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export size_t mi_malloc_good_size(size_t size)     mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export size_t mi_malloc_usable_size(const void *p) mi_attr_noexcept;

mi_decl_export int mi_posix_memalign(void** p, size_t alignment, size_t size)   mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_memalign(size_t alignment, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_valloc(size_t size)  mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_pvalloc(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_aligned_alloc(size_t alignment, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(1);

mi_decl_nodiscard mi_decl_export void* mi_reallocarray(void* p, size_t count, size_t size) mi_attr_noexcept mi_attr_alloc_size2(2,3);
mi_decl_nodiscard mi_decl_export int   mi_reallocarr(void* p, size_t count, size_t size) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export void* mi_aligned_recalloc(void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export void* mi_aligned_offset_recalloc(void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept;

mi_decl_nodiscard mi_decl_export mi_decl_restrict unsigned short* mi_wcsdup(const unsigned short* s) mi_attr_noexcept mi_attr_malloc;
mi_decl_nodiscard mi_decl_export mi_decl_restrict unsigned char*  mi_mbsdup(const unsigned char* s)  mi_attr_noexcept mi_attr_malloc;
mi_decl_export int mi_dupenv_s(char** buf, size_t* size, const char* name)                      mi_attr_noexcept;
mi_decl_export int mi_wdupenv_s(unsigned short** buf, size_t* size, const unsigned short* name) mi_attr_noexcept;

mi_decl_export void mi_free_size(void* p, size_t size)                           mi_attr_noexcept;
mi_decl_export void mi_free_size_aligned(void* p, size_t size, size_t alignment) mi_attr_noexcept;
mi_decl_export void mi_free_aligned(void* p, size_t alignment)                   mi_attr_noexcept;

// The `mi_new` wrappers implement C++ semantics on out-of-memory instead of directly returning `NULL`.
// (and call `std::get_new_handler` and potentially raise a `std::bad_alloc` exception).
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new(size_t size)                   mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new_aligned(size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new_nothrow(size_t size)           mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new_aligned_nothrow(size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new_n(size_t count, size_t size)   mi_attr_malloc mi_attr_alloc_size2(1, 2);
mi_decl_nodiscard mi_decl_export void* mi_new_realloc(void* p, size_t newsize)                mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export void* mi_new_reallocn(void* p, size_t newcount, size_t size) mi_attr_alloc_size2(2, 3);

mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_alloc_new(mi_heap_t* heap, size_t size)                mi_attr_malloc mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_alloc_new_n(mi_heap_t* heap, size_t count, size_t size) mi_attr_malloc mi_attr_alloc_size2(2, 3);

#if 0 // #ifdef __cplusplus
// }
#endif // 467

// ---------------------------------------------------------------------------------------------
// Implement the C++ std::allocator interface for use in STL containers.
// (note: see `mimalloc-new-delete.h` for overriding the new/delete operators globally)
// ---------------------------------------------------------------------------------------------
#if 0 // #ifdef __cplusplus
// 
// #include <cstddef>     // std::size_t
// #include <cstdint>     // PTRDIFF_MAX
#if 0 // #if (__cplusplus >= 201103L) || (_MSC_VER > 1900)  // C++11
// #include <type_traits> // std::true_type
// #include <utility>     // std::forward
#endif // 479
// 
// template<class T> struct _mi_stl_allocator_common {
//   typedef T                 value_type;
//   typedef std::size_t       size_type;
//   typedef std::ptrdiff_t    difference_type;
//   typedef value_type&       reference;
//   typedef value_type const& const_reference;
//   typedef value_type*       pointer;
//   typedef value_type const* const_pointer;
// 
#if 0 //   #if ((__cplusplus >= 201103L) || (_MSC_VER > 1900))  // C++11
//   using propagate_on_container_copy_assignment = std::true_type;
//   using propagate_on_container_move_assignment = std::true_type;
//   using propagate_on_container_swap            = std::true_type;
//   template <class U, class ...Args> void construct(U* p, Args&& ...args) { ::new(p) U(std::forward<Args>(args)...); }
//   template <class U> void destroy(U* p) mi_attr_noexcept { p->~U(); }
  #else
//   void construct(pointer p, value_type const& val) { ::new(p) value_type(val); }
//   void destroy(pointer p) { p->~value_type(); }
  #endif // 493
// 
//   size_type     max_size() const mi_attr_noexcept { return (PTRDIFF_MAX/sizeof(value_type)); }
//   pointer       address(reference x) const        { return &x; }
//   const_pointer address(const_reference x) const  { return &x; }
// };
// 
// template<class T> struct mi_stl_allocator : public _mi_stl_allocator_common<T> {
//   using typename _mi_stl_allocator_common<T>::size_type;
//   using typename _mi_stl_allocator_common<T>::value_type;
//   using typename _mi_stl_allocator_common<T>::pointer;
//   template <class U> struct rebind { typedef mi_stl_allocator<U> other; };
// 
//   mi_stl_allocator()                                             mi_attr_noexcept = default;
//   mi_stl_allocator(const mi_stl_allocator&)                      mi_attr_noexcept = default;
//   template<class U> mi_stl_allocator(const mi_stl_allocator<U>&) mi_attr_noexcept { }
//   mi_stl_allocator  select_on_container_copy_construction() const { return *this; }
//   void              deallocate(T* p, size_type) { mi_free(p); }
// 
#if 0 //   #if (__cplusplus >= 201703L)  // C++17
//   mi_decl_nodiscard T* allocate(size_type count) { return static_cast<T*>(mi_new_n(count, sizeof(T))); }
//   mi_decl_nodiscard T* allocate(size_type count, const void*) { return allocate(count); }
  #else
//   mi_decl_nodiscard pointer allocate(size_type count, const void* = 0) { return static_cast<pointer>(mi_new_n(count, sizeof(value_type))); }
  #endif // 521
// 
#if 0 //   #if ((__cplusplus >= 201103L) || (_MSC_VER > 1900))  // C++11
//   using is_always_equal = std::true_type;
  #endif // 528
// };
// 
// template<class T1,class T2> bool operator==(const mi_stl_allocator<T1>& , const mi_stl_allocator<T2>& ) mi_attr_noexcept { return true; }
// template<class T1,class T2> bool operator!=(const mi_stl_allocator<T1>& , const mi_stl_allocator<T2>& ) mi_attr_noexcept { return false; }
// 
// 
#if 0 // #if (__cplusplus >= 201103L) || (_MSC_VER >= 1900)  // C++11
// #define MI_HAS_HEAP_STL_ALLOCATOR 1
// 
// #include <memory>      // std::shared_ptr
// 
// // Common base class for STL allocators in a specific heap
// template<class T, bool _mi_destroy> struct _mi_heap_stl_allocator_common : public _mi_stl_allocator_common<T> {
//   using typename _mi_stl_allocator_common<T>::size_type;
//   using typename _mi_stl_allocator_common<T>::value_type;
//   using typename _mi_stl_allocator_common<T>::pointer;
// 
//   _mi_heap_stl_allocator_common(mi_heap_t* hp) : heap(hp, [](mi_heap_t*) {}) {}    /* will not delete nor destroy the passed in heap */
// 
#if 0 //   #if (__cplusplus >= 201703L)  // C++17
//   mi_decl_nodiscard T* allocate(size_type count) { return static_cast<T*>(mi_heap_alloc_new_n(this->heap.get(), count, sizeof(T))); }
//   mi_decl_nodiscard T* allocate(size_type count, const void*) { return allocate(count); }
  #else
//   mi_decl_nodiscard pointer allocate(size_type count, const void* = 0) { return static_cast<pointer>(mi_heap_alloc_new_n(this->heap.get(), count, sizeof(value_type))); }
  #endif // 550
// 
#if 0 //   #if ((__cplusplus >= 201103L) || (_MSC_VER > 1900))  // C++11
//   using is_always_equal = std::false_type;
  #endif // 557
// 
//   void collect(bool force) { mi_heap_collect(this->heap.get(), force); }
//   template<class U> bool is_equal(const _mi_heap_stl_allocator_common<U, _mi_destroy>& x) const { return (this->heap == x.heap); }
// 
// protected:
//   std::shared_ptr<mi_heap_t> heap;
//   template<class U, bool D> friend struct _mi_heap_stl_allocator_common;
// 
//   _mi_heap_stl_allocator_common() {
//     mi_heap_t* hp = mi_heap_new();
//     this->heap.reset(hp, (_mi_destroy ? &heap_destroy : &heap_delete));  /* calls heap_delete/destroy when the refcount drops to zero */
//   }
//   _mi_heap_stl_allocator_common(const _mi_heap_stl_allocator_common& x) mi_attr_noexcept : heap(x.heap) { }
//   template<class U> _mi_heap_stl_allocator_common(const _mi_heap_stl_allocator_common<U, _mi_destroy>& x) mi_attr_noexcept : heap(x.heap) { }
// 
// private:
//   static void heap_delete(mi_heap_t* hp)  { if (hp != NULL) { mi_heap_delete(hp); } }
//   static void heap_destroy(mi_heap_t* hp) { if (hp != NULL) { mi_heap_destroy(hp); } }
// };
// 
// // STL allocator allocation in a specific heap
// template<class T> struct mi_heap_stl_allocator : public _mi_heap_stl_allocator_common<T, false> {
//   using typename _mi_heap_stl_allocator_common<T, false>::size_type;
//   mi_heap_stl_allocator() : _mi_heap_stl_allocator_common<T, false>() { } // creates fresh heap that is deleted when the destructor is called
//   mi_heap_stl_allocator(mi_heap_t* hp) : _mi_heap_stl_allocator_common<T, false>(hp) { }  // no delete nor destroy on the passed in heap
//   template<class U> mi_heap_stl_allocator(const mi_heap_stl_allocator<U>& x) mi_attr_noexcept : _mi_heap_stl_allocator_common<T, false>(x) { }
// 
//   mi_heap_stl_allocator select_on_container_copy_construction() const { return *this; }
//   void deallocate(T* p, size_type) { mi_free(p); }
//   template<class U> struct rebind { typedef mi_heap_stl_allocator<U> other; };
// };
// 
// template<class T1, class T2> bool operator==(const mi_heap_stl_allocator<T1>& x, const mi_heap_stl_allocator<T2>& y) mi_attr_noexcept { return (x.is_equal(y)); }
// template<class T1, class T2> bool operator!=(const mi_heap_stl_allocator<T1>& x, const mi_heap_stl_allocator<T2>& y) mi_attr_noexcept { return (!x.is_equal(y)); }
// 
// 
// // STL allocator allocation in a specific heap, where `free` does nothing and
// // the heap is destroyed in one go on destruction -- use with care!
// template<class T> struct mi_heap_destroy_stl_allocator : public _mi_heap_stl_allocator_common<T, true> {
//   using typename _mi_heap_stl_allocator_common<T, true>::size_type;
//   mi_heap_destroy_stl_allocator() : _mi_heap_stl_allocator_common<T, true>() { } // creates fresh heap that is destroyed when the destructor is called
//   mi_heap_destroy_stl_allocator(mi_heap_t* hp) : _mi_heap_stl_allocator_common<T, true>(hp) { }  // no delete nor destroy on the passed in heap
//   template<class U> mi_heap_destroy_stl_allocator(const mi_heap_destroy_stl_allocator<U>& x) mi_attr_noexcept : _mi_heap_stl_allocator_common<T, true>(x) { }
// 
//   mi_heap_destroy_stl_allocator select_on_container_copy_construction() const { return *this; }
//   void deallocate(T*, size_type) { /* do nothing as we destroy the heap on destruct. */ }
//   template<class U> struct rebind { typedef mi_heap_destroy_stl_allocator<U> other; };
// };
// 
// template<class T1, class T2> bool operator==(const mi_heap_destroy_stl_allocator<T1>& x, const mi_heap_destroy_stl_allocator<T2>& y) mi_attr_noexcept { return (x.is_equal(y)); }
// template<class T1, class T2> bool operator!=(const mi_heap_destroy_stl_allocator<T1>& x, const mi_heap_destroy_stl_allocator<T2>& y) mi_attr_noexcept { return (!x.is_equal(y)); }
// 
#endif // C++11 // 537
// 
#endif // __cplusplus // 475

// #endif // 24
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/
// #ifndef MIMALLOC_INTERNAL_H
// #define MIMALLOC_INTERNAL_H


// --------------------------------------------------------------------------
// This file contains the interal API's of mimalloc and various utility
// functions and macros.
// --------------------------------------------------------------------------


// --- Amalgamation START --- mimalloc/include/mimalloc/types.h
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2024, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/
// #ifndef MIMALLOC_TYPES_H
// #define MIMALLOC_TYPES_H

// --------------------------------------------------------------------------
// This file contains the main type definitions for mimalloc:
// mi_heap_t      : all data for a thread-local heap, contains
//                  lists of all managed heap pages.
// mi_segment_t   : a larger chunk of memory (32GiB) from where pages
//                  are allocated.
// mi_page_t      : a mimalloc page (usually 64KiB or 512KiB) from
//                  where objects are allocated.
//                  Note: we write "OS page" for OS memory pages while
//                  using plain "page" for mimalloc pages (`mi_page_t`).
// --------------------------------------------------------------------------


#include <stddef.h>   // ptrdiff_t
#include <stdint.h>   // uintptr_t, uint16_t, etc

// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023 Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/
// #ifndef MIMALLOC_ATOMIC_H
// #define MIMALLOC_ATOMIC_H

// include windows.h or pthreads.h
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // 675
#include <windows.h>
#elif !defined(__wasi__) && (!defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__))
#define  MI_USE_PTHREADS
#include <pthread.h>
#endif // 674

// --------------------------------------------------------------------------------------------
// Atomics
// We need to be portable between C, C++, and MSVC.
// We base the primitives on the C/C++ atomics and create a mimimal wrapper for MSVC in C compilation mode.
// This is why we try to use only `uintptr_t` and `<type>*` as atomic types.
// To gain better insight in the range of used atomics, we use explicitly named memory order operations
// instead of passing the memory order as a parameter.
// -----------------------------------------------------------------------------------------------

#if 0 // #if defined(__cplusplus)
// // Use C++ atomics
// #include <atomic>
// #define  _Atomic(tp)            std::atomic<tp>
// #define  mi_atomic(name)        std::atomic_##name
// #define  mi_memory_order(name)  std::memory_order_##name
#if 0 // #if (__cplusplus >= 202002L)    // c++20, see issue #571
//  #define MI_ATOMIC_VAR_INIT(x)  x
#elif 0 // #elif !defined(ATOMIC_VAR_INIT)
//  #define MI_ATOMIC_VAR_INIT(x)  x
#else
//  #define MI_ATOMIC_VAR_INIT(x)  ATOMIC_VAR_INIT(x)
#endif // 699
#elif 0 // #elif defined(_MSC_VER)
// // Use MSVC C wrapper for C11 atomics
// #define  _Atomic(tp)            tp
// #define  MI_ATOMIC_VAR_INIT(x)  x
// #define  mi_atomic(name)        mi_atomic_##name
// #define  mi_memory_order(name)  mi_memory_order_##name
#else
// Use C11 atomics
#include <stdatomic.h>
#define  mi_atomic(name)        atomic_##name
#define  mi_memory_order(name)  memory_order_##name
#if (__STDC_VERSION__ >= 201710L) // c17, see issue #735
 #define MI_ATOMIC_VAR_INIT(x) x
#elif !defined(ATOMIC_VAR_INIT)
 #define MI_ATOMIC_VAR_INIT(x) x
#else
 #define MI_ATOMIC_VAR_INIT(x) ATOMIC_VAR_INIT(x)
#endif // 717
#endif // 693

// Various defines for all used memory orders in mimalloc
#define mi_atomic_cas_weak(p,expected,desired,mem_success,mem_fail)  \
  mi_atomic(compare_exchange_weak_explicit)(p,expected,desired,mem_success,mem_fail)

#define mi_atomic_cas_strong(p,expected,desired,mem_success,mem_fail)  \
  mi_atomic(compare_exchange_strong_explicit)(p,expected,desired,mem_success,mem_fail)

#define mi_atomic_load_acquire(p)                mi_atomic(load_explicit)(p,mi_memory_order(acquire))
#define mi_atomic_load_relaxed(p)                mi_atomic(load_explicit)(p,mi_memory_order(relaxed))
#define mi_atomic_store_release(p,x)             mi_atomic(store_explicit)(p,x,mi_memory_order(release))
#define mi_atomic_store_relaxed(p,x)             mi_atomic(store_explicit)(p,x,mi_memory_order(relaxed))
#define mi_atomic_exchange_release(p,x)          mi_atomic(exchange_explicit)(p,x,mi_memory_order(release))
#define mi_atomic_exchange_acq_rel(p,x)          mi_atomic(exchange_explicit)(p,x,mi_memory_order(acq_rel))
#define mi_atomic_cas_weak_release(p,exp,des)    mi_atomic_cas_weak(p,exp,des,mi_memory_order(release),mi_memory_order(relaxed))
#define mi_atomic_cas_weak_acq_rel(p,exp,des)    mi_atomic_cas_weak(p,exp,des,mi_memory_order(acq_rel),mi_memory_order(acquire))
#define mi_atomic_cas_strong_release(p,exp,des)  mi_atomic_cas_strong(p,exp,des,mi_memory_order(release),mi_memory_order(relaxed))
#define mi_atomic_cas_strong_acq_rel(p,exp,des)  mi_atomic_cas_strong(p,exp,des,mi_memory_order(acq_rel),mi_memory_order(acquire))

#define mi_atomic_add_relaxed(p,x)               mi_atomic(fetch_add_explicit)(p,x,mi_memory_order(relaxed))
#define mi_atomic_sub_relaxed(p,x)               mi_atomic(fetch_sub_explicit)(p,x,mi_memory_order(relaxed))
#define mi_atomic_add_acq_rel(p,x)               mi_atomic(fetch_add_explicit)(p,x,mi_memory_order(acq_rel))
#define mi_atomic_sub_acq_rel(p,x)               mi_atomic(fetch_sub_explicit)(p,x,mi_memory_order(acq_rel))
#define mi_atomic_and_acq_rel(p,x)               mi_atomic(fetch_and_explicit)(p,x,mi_memory_order(acq_rel))
#define mi_atomic_or_acq_rel(p,x)                mi_atomic(fetch_or_explicit)(p,x,mi_memory_order(acq_rel))

#define mi_atomic_increment_relaxed(p)           mi_atomic_add_relaxed(p,(uintptr_t)1)
#define mi_atomic_decrement_relaxed(p)           mi_atomic_sub_relaxed(p,(uintptr_t)1)
#define mi_atomic_increment_acq_rel(p)           mi_atomic_add_acq_rel(p,(uintptr_t)1)
#define mi_atomic_decrement_acq_rel(p)           mi_atomic_sub_acq_rel(p,(uintptr_t)1)

static inline void mi_atomic_yield(void);
static inline intptr_t mi_atomic_addi(_Atomic(intptr_t)*p, intptr_t add);
static inline intptr_t mi_atomic_subi(_Atomic(intptr_t)*p, intptr_t sub);


#if defined(__cplusplus) || !defined(_MSC_VER)

// In C++/C11 atomics we have polymorphic atomics so can use the typed `ptr` variants (where `tp` is the type of atomic value)
// We use these macros so we can provide a typed wrapper in MSVC in C compilation mode as well
#define mi_atomic_load_ptr_acquire(tp,p)                mi_atomic_load_acquire(p)
#define mi_atomic_load_ptr_relaxed(tp,p)                mi_atomic_load_relaxed(p)

// In C++ we need to add casts to help resolve templates if NULL is passed
#if 0 // #if defined(__cplusplus)
// #define mi_atomic_store_ptr_release(tp,p,x)             mi_atomic_store_release(p,(tp*)x)
// #define mi_atomic_store_ptr_relaxed(tp,p,x)             mi_atomic_store_relaxed(p,(tp*)x)
// #define mi_atomic_cas_ptr_weak_release(tp,p,exp,des)    mi_atomic_cas_weak_release(p,exp,(tp*)des)
// #define mi_atomic_cas_ptr_weak_acq_rel(tp,p,exp,des)    mi_atomic_cas_weak_acq_rel(p,exp,(tp*)des)
// #define mi_atomic_cas_ptr_strong_release(tp,p,exp,des)  mi_atomic_cas_strong_release(p,exp,(tp*)des)
// #define mi_atomic_exchange_ptr_release(tp,p,x)          mi_atomic_exchange_release(p,(tp*)x)
// #define mi_atomic_exchange_ptr_acq_rel(tp,p,x)          mi_atomic_exchange_acq_rel(p,(tp*)x)
#else
#define mi_atomic_store_ptr_release(tp,p,x)             mi_atomic_store_release(p,x)
#define mi_atomic_store_ptr_relaxed(tp,p,x)             mi_atomic_store_relaxed(p,x)
#define mi_atomic_cas_ptr_weak_release(tp,p,exp,des)    mi_atomic_cas_weak_release(p,exp,des)
#define mi_atomic_cas_ptr_weak_acq_rel(tp,p,exp,des)    mi_atomic_cas_weak_acq_rel(p,exp,des)
#define mi_atomic_cas_ptr_strong_release(tp,p,exp,des)  mi_atomic_cas_strong_release(p,exp,des)
#define mi_atomic_exchange_ptr_release(tp,p,x)          mi_atomic_exchange_release(p,x)
#define mi_atomic_exchange_ptr_acq_rel(tp,p,x)          mi_atomic_exchange_acq_rel(p,x)
#endif // 769

// These are used by the statistics
static inline int64_t mi_atomic_addi64_relaxed(volatile int64_t* p, int64_t add) {
  return mi_atomic(fetch_add_explicit)((_Atomic(int64_t)*)p, add, mi_memory_order(relaxed));
}
static inline void mi_atomic_maxi64_relaxed(volatile int64_t* p, int64_t x) {
  int64_t current = mi_atomic_load_relaxed((_Atomic(int64_t)*)p);
  while (current < x && !mi_atomic_cas_weak_release((_Atomic(int64_t)*)p, &current, x)) { /* nothing */ };
}

// Used by timers
#define mi_atomic_loadi64_acquire(p)            mi_atomic(load_explicit)(p,mi_memory_order(acquire))
#define mi_atomic_loadi64_relaxed(p)            mi_atomic(load_explicit)(p,mi_memory_order(relaxed))
#define mi_atomic_storei64_release(p,x)         mi_atomic(store_explicit)(p,x,mi_memory_order(release))
#define mi_atomic_storei64_relaxed(p,x)         mi_atomic(store_explicit)(p,x,mi_memory_order(relaxed))

#define mi_atomic_casi64_strong_acq_rel(p,e,d)  mi_atomic_cas_strong_acq_rel(p,e,d)
#define mi_atomic_addi64_acq_rel(p,i)           mi_atomic_add_acq_rel(p,i)


#elif 0 // #elif defined(_MSC_VER)
// 
// // Legacy MSVC plain C compilation wrapper that uses Interlocked operations to model C11 atomics.
// #include <intrin.h>
#if 0 // #ifdef _WIN64
// typedef LONG64   msc_intptr_t;
// #define MI_64(f) f##64
#else
// typedef LONG     msc_intptr_t;
// #define MI_64(f) f
#endif // 810
// 
// typedef enum mi_memory_order_e {
//   mi_memory_order_relaxed,
//   mi_memory_order_consume,
//   mi_memory_order_acquire,
//   mi_memory_order_release,
//   mi_memory_order_acq_rel,
//   mi_memory_order_seq_cst
// } mi_memory_order;
// 
// static inline uintptr_t mi_atomic_fetch_add_explicit(_Atomic(uintptr_t)*p, uintptr_t add, mi_memory_order mo) {
//   (void)(mo);
//   return (uintptr_t)MI_64(_InterlockedExchangeAdd)((volatile msc_intptr_t*)p, (msc_intptr_t)add);
// }
// static inline uintptr_t mi_atomic_fetch_sub_explicit(_Atomic(uintptr_t)*p, uintptr_t sub, mi_memory_order mo) {
//   (void)(mo);
//   return (uintptr_t)MI_64(_InterlockedExchangeAdd)((volatile msc_intptr_t*)p, -((msc_intptr_t)sub));
// }
// static inline uintptr_t mi_atomic_fetch_and_explicit(_Atomic(uintptr_t)*p, uintptr_t x, mi_memory_order mo) {
//   (void)(mo);
//   return (uintptr_t)MI_64(_InterlockedAnd)((volatile msc_intptr_t*)p, (msc_intptr_t)x);
// }
// static inline uintptr_t mi_atomic_fetch_or_explicit(_Atomic(uintptr_t)*p, uintptr_t x, mi_memory_order mo) {
//   (void)(mo);
//   return (uintptr_t)MI_64(_InterlockedOr)((volatile msc_intptr_t*)p, (msc_intptr_t)x);
// }
// static inline bool mi_atomic_compare_exchange_strong_explicit(_Atomic(uintptr_t)*p, uintptr_t* expected, uintptr_t desired, mi_memory_order mo1, mi_memory_order mo2) {
//   (void)(mo1); (void)(mo2);
//   uintptr_t read = (uintptr_t)MI_64(_InterlockedCompareExchange)((volatile msc_intptr_t*)p, (msc_intptr_t)desired, (msc_intptr_t)(*expected));
//   if (read == *expected) {
//     return true;
//   }
//   else {
//     *expected = read;
//     return false;
//   }
// }
// static inline bool mi_atomic_compare_exchange_weak_explicit(_Atomic(uintptr_t)*p, uintptr_t* expected, uintptr_t desired, mi_memory_order mo1, mi_memory_order mo2) {
//   return mi_atomic_compare_exchange_strong_explicit(p, expected, desired, mo1, mo2);
// }
// static inline uintptr_t mi_atomic_exchange_explicit(_Atomic(uintptr_t)*p, uintptr_t exchange, mi_memory_order mo) {
//   (void)(mo);
//   return (uintptr_t)MI_64(_InterlockedExchange)((volatile msc_intptr_t*)p, (msc_intptr_t)exchange);
// }
// static inline void mi_atomic_thread_fence(mi_memory_order mo) {
//   (void)(mo);
//   _Atomic(uintptr_t) x = 0;
//   mi_atomic_exchange_explicit(&x, 1, mo);
// }
// static inline uintptr_t mi_atomic_load_explicit(_Atomic(uintptr_t) const* p, mi_memory_order mo) {
//   (void)(mo);
#if 0 // #if defined(_M_IX86) || defined(_M_X64)
//   return *p;
#else
//   uintptr_t x = *p;
//   if (mo > mi_memory_order_relaxed) {
//     while (!mi_atomic_compare_exchange_weak_explicit((_Atomic(uintptr_t)*)p, &x, x, mo, mi_memory_order_relaxed)) { /* nothing */ };
//   }
//   return x;
#endif // 868
// }
// static inline void mi_atomic_store_explicit(_Atomic(uintptr_t)*p, uintptr_t x, mi_memory_order mo) {
//   (void)(mo);
#if 0 // #if defined(_M_IX86) || defined(_M_X64)
//   *p = x;
#else
//   mi_atomic_exchange_explicit(p, x, mo);
#endif // 880
// }
// static inline int64_t mi_atomic_loadi64_explicit(_Atomic(int64_t)*p, mi_memory_order mo) {
//   (void)(mo);
#if 0 // #if defined(_M_X64)
//   return *p;
#else
//   int64_t old = *p;
//   int64_t x = old;
//   while ((old = InterlockedCompareExchange64(p, x, old)) != x) {
//     x = old;
//   }
//   return x;
#endif // 888
// }
// static inline void mi_atomic_storei64_explicit(_Atomic(int64_t)*p, int64_t x, mi_memory_order mo) {
//   (void)(mo);
#if 0 // #if defined(x_M_IX86) || defined(_M_X64)
//   *p = x;
#else
//   InterlockedExchange64(p, x);
#endif // 901
// }
// 
// // These are used by the statistics
// static inline int64_t mi_atomic_addi64_relaxed(volatile _Atomic(int64_t)*p, int64_t add) {
#if 0 // #ifdef _WIN64
//   return (int64_t)mi_atomic_addi((int64_t*)p, add);
#else
//   int64_t current;
//   int64_t sum;
//   do {
//     current = *p;
//     sum = current + add;
//   } while (_InterlockedCompareExchange64(p, sum, current) != current);
//   return current;
#endif // 910
// }
// static inline void mi_atomic_maxi64_relaxed(volatile _Atomic(int64_t)*p, int64_t x) {
//   int64_t current;
//   do {
//     current = *p;
//   } while (current < x && _InterlockedCompareExchange64(p, x, current) != current);
// }
// 
// static inline void mi_atomic_addi64_acq_rel(volatile _Atomic(int64_t*)p, int64_t i) {
//   mi_atomic_addi64_relaxed(p, i);
// }
// 
// static inline bool mi_atomic_casi64_strong_acq_rel(volatile _Atomic(int64_t*)p, int64_t* exp, int64_t des) {
//   int64_t read = _InterlockedCompareExchange64(p, des, *exp);
//   if (read == *exp) {
//     return true;
//   }
//   else {
//     *exp = read;
//     return false;
//   }
// }
// 
// // The pointer macros cast to `uintptr_t`.
// #define mi_atomic_load_ptr_acquire(tp,p)                (tp*)mi_atomic_load_acquire((_Atomic(uintptr_t)*)(p))
// #define mi_atomic_load_ptr_relaxed(tp,p)                (tp*)mi_atomic_load_relaxed((_Atomic(uintptr_t)*)(p))
// #define mi_atomic_store_ptr_release(tp,p,x)             mi_atomic_store_release((_Atomic(uintptr_t)*)(p),(uintptr_t)(x))
// #define mi_atomic_store_ptr_relaxed(tp,p,x)             mi_atomic_store_relaxed((_Atomic(uintptr_t)*)(p),(uintptr_t)(x))
// #define mi_atomic_cas_ptr_weak_release(tp,p,exp,des)    mi_atomic_cas_weak_release((_Atomic(uintptr_t)*)(p),(uintptr_t*)exp,(uintptr_t)des)
// #define mi_atomic_cas_ptr_weak_acq_rel(tp,p,exp,des)    mi_atomic_cas_weak_acq_rel((_Atomic(uintptr_t)*)(p),(uintptr_t*)exp,(uintptr_t)des)
// #define mi_atomic_cas_ptr_strong_release(tp,p,exp,des)  mi_atomic_cas_strong_release((_Atomic(uintptr_t)*)(p),(uintptr_t*)exp,(uintptr_t)des)
// #define mi_atomic_exchange_ptr_release(tp,p,x)          (tp*)mi_atomic_exchange_release((_Atomic(uintptr_t)*)(p),(uintptr_t)x)
// #define mi_atomic_exchange_ptr_acq_rel(tp,p,x)          (tp*)mi_atomic_exchange_acq_rel((_Atomic(uintptr_t)*)(p),(uintptr_t)x)
// 
// #define mi_atomic_loadi64_acquire(p)    mi_atomic(loadi64_explicit)(p,mi_memory_order(acquire))
// #define mi_atomic_loadi64_relaxed(p)    mi_atomic(loadi64_explicit)(p,mi_memory_order(relaxed))
// #define mi_atomic_storei64_release(p,x) mi_atomic(storei64_explicit)(p,x,mi_memory_order(release))
// #define mi_atomic_storei64_relaxed(p,x) mi_atomic(storei64_explicit)(p,x,mi_memory_order(relaxed))
// 
// 
#endif // 761


// Atomically add a signed value; returns the previous value.
static inline intptr_t mi_atomic_addi(_Atomic(intptr_t)*p, intptr_t add) {
  return (intptr_t)mi_atomic_add_acq_rel((_Atomic(uintptr_t)*)p, (uintptr_t)add);
}

// Atomically subtract a signed value; returns the previous value.
static inline intptr_t mi_atomic_subi(_Atomic(intptr_t)*p, intptr_t sub) {
  return (intptr_t)mi_atomic_addi(p, -sub);
}


// ----------------------------------------------------------------------
// Once and Guard
// ----------------------------------------------------------------------

typedef _Atomic(uintptr_t) mi_atomic_once_t;

// Returns true only on the first invocation
static inline bool mi_atomic_once( mi_atomic_once_t* once ) {
  if (mi_atomic_load_relaxed(once) != 0) return false;     // quick test
  uintptr_t expected = 0;
  return mi_atomic_cas_strong_acq_rel(once, &expected, (uintptr_t)1); // try to set to 1
}

typedef _Atomic(uintptr_t) mi_atomic_guard_t;

// Allows only one thread to execute at a time
#define mi_atomic_guard(guard) \
  uintptr_t _mi_guard_expected = 0; \
  for(bool _mi_guard_once = true; \
      _mi_guard_once && mi_atomic_cas_strong_acq_rel(guard,&_mi_guard_expected,(uintptr_t)1); \
      (mi_atomic_store_release(guard,(uintptr_t)0), _mi_guard_once = false) )



// ----------------------------------------------------------------------
// Yield
// ----------------------------------------------------------------------

#if 0 // #if defined(__cplusplus)
// #include <thread>
// static inline void mi_atomic_yield(void) {
//   std::this_thread::yield();
// }
#elif defined(_WIN32)
static inline void mi_atomic_yield(void) {
  YieldProcessor();
}
#elif defined(__SSE2__)
#include <emmintrin.h>
static inline void mi_atomic_yield(void) {
  _mm_pause();
}
#elif (defined(__GNUC__) || defined(__clang__)) && \
      (defined(__x86_64__) || defined(__i386__) || defined(__arm__) || defined(__armel__) || defined(__ARMEL__) || \
       defined(__aarch64__) || defined(__powerpc__) || defined(__ppc__) || defined(__PPC__)) || defined(__POWERPC__)
#if defined(__x86_64__) || defined(__i386__)
static inline void mi_atomic_yield(void) {
  __asm__ volatile ("pause" ::: "memory");
}
#elif defined(__aarch64__)
static inline void mi_atomic_yield(void) {
  __asm__ volatile("wfe");
}
#elif (defined(__arm__) && __ARM_ARCH__ >= 7)
static inline void mi_atomic_yield(void) {
  __asm__ volatile("yield" ::: "memory");
}
#elif defined(__powerpc__) || defined(__ppc__) || defined(__PPC__) || defined(__POWERPC__)
#ifdef __APPLE__
static inline void mi_atomic_yield(void) {
  __asm__ volatile ("or r27,r27,r27" ::: "memory");
}
#else
static inline void mi_atomic_yield(void) {
  __asm__ __volatile__ ("or 27,27,27" ::: "memory");
}
#endif // 1033
#elif defined(__armel__) || defined(__ARMEL__)
static inline void mi_atomic_yield(void) {
  __asm__ volatile ("nop" ::: "memory");
}
#endif // 1020
#elif defined(__sun)
// Fallback for other archs
#include <synch.h>
static inline void mi_atomic_yield(void) {
  smt_pause();
}
#elif defined(__wasi__)
#include <sched.h>
static inline void mi_atomic_yield(void) {
  sched_yield();
}
#else
#include <unistd.h>
static inline void mi_atomic_yield(void) {
  sleep(0);
}
#endif // 1003


// ----------------------------------------------------------------------
// Locks are only used for abandoned segment visiting in `arena.c`
// ----------------------------------------------------------------------

#if defined(_WIN32)

#define mi_lock_t  CRITICAL_SECTION

static inline bool mi_lock_try_acquire(mi_lock_t* lock) {
  return TryEnterCriticalSection(lock);
}
static inline bool mi_lock_acquire(mi_lock_t* lock) {
  EnterCriticalSection(lock);
  return true;
}
static inline void mi_lock_release(mi_lock_t* lock) {
  LeaveCriticalSection(lock);
}
static inline void mi_lock_init(mi_lock_t* lock) {
  InitializeCriticalSection(lock);
}
static inline void mi_lock_done(mi_lock_t* lock) {
  DeleteCriticalSection(lock);
}


#elif defined(MI_USE_PTHREADS)

#define mi_lock_t  pthread_mutex_t

static inline bool mi_lock_try_acquire(mi_lock_t* lock) {
  return (pthread_mutex_trylock(lock) == 0);
}
static inline bool mi_lock_acquire(mi_lock_t* lock) {
  return (pthread_mutex_lock(lock) == 0);
}
static inline void mi_lock_release(mi_lock_t* lock) {
  pthread_mutex_unlock(lock);
}
static inline void mi_lock_init(mi_lock_t* lock) {
  pthread_mutex_init(lock, NULL);
}
static inline void mi_lock_done(mi_lock_t* lock) {
  pthread_mutex_destroy(lock);
}

/*
#elif 0 // #elif defined(__cplusplus)
// 
// #include <mutex>
// #define mi_lock_t  std::mutex
// 
// static inline bool mi_lock_try_acquire(mi_lock_t* lock) {
//   return lock->lock_try_acquire();
// }
// static inline bool mi_lock_acquire(mi_lock_t* lock) {
//   lock->lock();
//   return true;
// }
// static inline void mi_lock_release(mi_lock_t* lock) {
//   lock->unlock();
// }
// static inline void mi_lock_init(mi_lock_t* lock) {
//   (void)(lock);
// }
// static inline void mi_lock_done(mi_lock_t* lock) {
//   (void)(lock);
// }
// */
// 
#else

// fall back to poor man's locks.
// this should only be the case in a single-threaded environment (like __wasi__)

#define mi_lock_t  _Atomic(uintptr_t)

static inline bool mi_lock_try_acquire(mi_lock_t* lock) {
  uintptr_t expected = 0;
  return mi_atomic_cas_strong_acq_rel(lock, &expected, (uintptr_t)1);
}
static inline bool mi_lock_acquire(mi_lock_t* lock) {
  for (int i = 0; i < 1000; i++) {  // for at most 1000 tries?
    if (mi_lock_try_acquire(lock)) return true;
    mi_atomic_yield();
  }
  return true;
}
static inline void mi_lock_release(mi_lock_t* lock) {
  mi_atomic_store_release(lock, (uintptr_t)0);
}
static inline void mi_lock_init(mi_lock_t* lock) {
  mi_lock_release(lock);
}
static inline void mi_lock_done(mi_lock_t* lock) {
  (void)(lock);
}

#endif // 1070




// #endif // __MIMALLOC_ATOMIC_H // 670
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


#if 0 // #ifdef _MSC_VER
// #pragma warning(disable:4214) // bitfield is not int
#endif // 1173

// Minimal alignment necessary. On most platforms 16 bytes are needed
// due to SSE registers for example. This must be at least `sizeof(void*)`
#ifndef MI_MAX_ALIGN_SIZE
#define MI_MAX_ALIGN_SIZE  16   // sizeof(max_align_t)
#endif // 1179

// ------------------------------------------------------
// Variants
// ------------------------------------------------------

// Define NDEBUG in the release version to disable assertions.
// #define NDEBUG

// Define MI_TRACK_<tool> to enable tracking support
// #define MI_TRACK_VALGRIND 1
// #define MI_TRACK_ASAN     1
// #define MI_TRACK_ETW      1

// Define MI_STAT as 1 to maintain statistics; set it to 2 to have detailed statistics (but costs some performance).
// #define MI_STAT 1

// Define MI_SECURE to enable security mitigations
// #define MI_SECURE 1  // guard page around metadata
// #define MI_SECURE 2  // guard page around each mimalloc page
// #define MI_SECURE 3  // encode free lists (detect corrupted free list (buffer overflow), and invalid pointer free)
// #define MI_SECURE 4  // checks for double free. (may be more expensive)

#if !defined(MI_SECURE)
#define MI_SECURE 0
#endif // 1204

// Define MI_DEBUG for debug mode
// #define MI_DEBUG 1  // basic assertion checks and statistics, check double free, corrupted free list, and invalid pointer free.
// #define MI_DEBUG 2  // + internal assertion checks
// #define MI_DEBUG 3  // + extensive internal invariant checking (cmake -DMI_DEBUG_FULL=ON)
#if !defined(MI_DEBUG)
#if !defined(NDEBUG) || defined(_DEBUG)
#define MI_DEBUG 2
#else
#define MI_DEBUG 0
#endif // 1213
#endif // 1212

// Use guard pages behind objects of a certain size (set by the MIMALLOC_DEBUG_GUARDED_MIN/MAX options)
// Padding should be disabled when using guard pages
// #define MI_GUARDED 1
#if defined(MI_GUARDED)
#define MI_PADDING  0
#endif // 1223

// Reserve extra padding at the end of each block to be more resilient against heap block overflows.
// The padding can detect buffer overflow on free.
#if !defined(MI_PADDING) && (MI_SECURE>=3 || MI_DEBUG>=1 || (MI_TRACK_VALGRIND || MI_TRACK_ASAN || MI_TRACK_ETW))
#define MI_PADDING  1
#endif // 1229

// Check padding bytes; allows byte-precise buffer overflow detection
#if !defined(MI_PADDING_CHECK) && MI_PADDING && (MI_SECURE>=3 || MI_DEBUG>=1)
#define MI_PADDING_CHECK 1
#endif // 1234


// Encoded free lists allow detection of corrupted free lists
// and can detect buffer overflows, modify after free, and double `free`s.
#if (MI_SECURE>=3 || MI_DEBUG>=1)
#define MI_ENCODE_FREELIST  1
#endif // 1241


// We used to abandon huge pages in order to eagerly deallocate it if freed from another thread.
// Unfortunately, that makes it not possible to visit them during a heap walk or include them in a
// `mi_heap_destroy`. We therefore instead reset/decommit the huge blocks nowadays if freed from
// another thread so the memory becomes "virtually" available (and eventually gets properly freed by
// the owning thread).
// #define MI_HUGE_PAGE_ABANDON 1


// ------------------------------------------------------
// Platform specific values
// ------------------------------------------------------

// ------------------------------------------------------
// Size of a pointer.
// We assume that `sizeof(void*)==sizeof(intptr_t)`
// and it holds for all platforms we know of.
//
// However, the C standard only requires that:
//  p == (void*)((intptr_t)p))
// but we also need:
//  i == (intptr_t)((void*)i)
// or otherwise one might define an intptr_t type that is larger than a pointer...
// ------------------------------------------------------

#if INTPTR_MAX > INT64_MAX
# define MI_INTPTR_SHIFT (4)  // assume 128-bit  (as on arm CHERI for example)
#elif INTPTR_MAX == INT64_MAX
# define MI_INTPTR_SHIFT (3)
#elif INTPTR_MAX == INT32_MAX
# define MI_INTPTR_SHIFT (2)
#else
#error platform pointers must be 32, 64, or 128 bits
#endif // 1270

#if SIZE_MAX == UINT64_MAX
# define MI_SIZE_SHIFT (3)
typedef int64_t  mi_ssize_t;
#elif SIZE_MAX == UINT32_MAX
# define MI_SIZE_SHIFT (2)
typedef int32_t  mi_ssize_t;
#else
#error platform objects must be 32 or 64 bits
#endif // 1280

#if (SIZE_MAX/2) > LONG_MAX
# define MI_ZU(x)  x##ULL
# define MI_ZI(x)  x##LL
#else
# define MI_ZU(x)  x##UL
# define MI_ZI(x)  x##L
#endif // 1290

#define MI_INTPTR_SIZE  (1<<MI_INTPTR_SHIFT)
#define MI_INTPTR_BITS  (MI_INTPTR_SIZE*8)

#define MI_SIZE_SIZE  (1<<MI_SIZE_SHIFT)
#define MI_SIZE_BITS  (MI_SIZE_SIZE*8)

#define MI_KiB     (MI_ZU(1024))
#define MI_MiB     (MI_KiB*MI_KiB)
#define MI_GiB     (MI_MiB*MI_KiB)


// ------------------------------------------------------
// Main internal data-structures
// ------------------------------------------------------

// Main tuning parameters for segment and page sizes
// Sizes for 64-bit, divide by two for 32-bit
#ifndef MI_SMALL_PAGE_SHIFT
#define MI_SMALL_PAGE_SHIFT               (13 + MI_INTPTR_SHIFT)      // 64KiB
#endif // 1315
#ifndef MI_MEDIUM_PAGE_SHIFT
#define MI_MEDIUM_PAGE_SHIFT              ( 3 + MI_SMALL_PAGE_SHIFT)  // 512KiB
#endif // 1318
#ifndef MI_LARGE_PAGE_SHIFT
#define MI_LARGE_PAGE_SHIFT               ( 3 + MI_MEDIUM_PAGE_SHIFT) // 4MiB
#endif // 1321
#ifndef MI_SEGMENT_SHIFT
#define MI_SEGMENT_SHIFT                  ( MI_LARGE_PAGE_SHIFT)      // 4MiB -- must be equal to `MI_LARGE_PAGE_SHIFT`
#endif // 1324

// Derived constants
#define MI_SEGMENT_SIZE                   (MI_ZU(1)<<MI_SEGMENT_SHIFT)
#define MI_SEGMENT_ALIGN                  (MI_SEGMENT_SIZE)
#define MI_SEGMENT_MASK                   ((uintptr_t)(MI_SEGMENT_ALIGN - 1))

#define MI_SMALL_PAGE_SIZE                (MI_ZU(1)<<MI_SMALL_PAGE_SHIFT)
#define MI_MEDIUM_PAGE_SIZE               (MI_ZU(1)<<MI_MEDIUM_PAGE_SHIFT)
#define MI_LARGE_PAGE_SIZE                (MI_ZU(1)<<MI_LARGE_PAGE_SHIFT)

#define MI_SMALL_PAGES_PER_SEGMENT        (MI_SEGMENT_SIZE/MI_SMALL_PAGE_SIZE)
#define MI_MEDIUM_PAGES_PER_SEGMENT       (MI_SEGMENT_SIZE/MI_MEDIUM_PAGE_SIZE)
#define MI_LARGE_PAGES_PER_SEGMENT        (MI_SEGMENT_SIZE/MI_LARGE_PAGE_SIZE)

// The max object size are checked to not waste more than 12.5% internally over the page sizes.
// (Except for large pages since huge objects are allocated in 4MiB chunks)
#define MI_SMALL_OBJ_SIZE_MAX             (MI_SMALL_PAGE_SIZE/4)   // 16KiB
#define MI_MEDIUM_OBJ_SIZE_MAX            (MI_MEDIUM_PAGE_SIZE/4)  // 128KiB
#define MI_LARGE_OBJ_SIZE_MAX             (MI_LARGE_PAGE_SIZE/2)   // 2MiB
#define MI_LARGE_OBJ_WSIZE_MAX            (MI_LARGE_OBJ_SIZE_MAX/MI_INTPTR_SIZE)

// Maximum number of size classes. (spaced exponentially in 12.5% increments)
#define MI_BIN_HUGE  (73U)

#if (MI_LARGE_OBJ_WSIZE_MAX >= 655360)
#error "mimalloc internal: define more bins"
#endif // 1351

// Maximum block size for which blocks are guaranteed to be block size aligned. (see `segment.c:_mi_segment_page_start`)
#define MI_MAX_ALIGN_GUARANTEE   (MI_MEDIUM_OBJ_SIZE_MAX)

// Alignments over MI_BLOCK_ALIGNMENT_MAX are allocated in dedicated huge page segments
#define MI_BLOCK_ALIGNMENT_MAX   (MI_SEGMENT_SIZE >> 1)

// We never allocate more than PTRDIFF_MAX (see also <https://sourceware.org/ml/libc-announce/2019/msg00001.html>)
#define MI_MAX_ALLOC_SIZE   PTRDIFF_MAX

// ------------------------------------------------------
// Mimalloc pages contain allocated blocks
// ------------------------------------------------------

// The free lists use encoded next fields
// (Only actually encodes when MI_ENCODED_FREELIST is defined.)
typedef uintptr_t  mi_encoded_t;

// thread id's
typedef size_t     mi_threadid_t;

// free lists contain blocks
typedef struct mi_block_s {
  mi_encoded_t next;
} mi_block_t;

#if MI_GUARDED
// we always align guarded pointers in a block at an offset
// the block `next` field is then used as a tag to distinguish regular offset aligned blocks from guarded ones
#define MI_BLOCK_TAG_ALIGNED   ((mi_encoded_t)(0))
#define MI_BLOCK_TAG_GUARDED   (~MI_BLOCK_TAG_ALIGNED)
#endif // 1380


// The delayed flags are used for efficient multi-threaded free-ing
typedef enum mi_delayed_e {
  MI_USE_DELAYED_FREE   = 0, // push on the owning heap thread delayed list
  MI_DELAYED_FREEING    = 1, // temporary: another thread is accessing the owning heap
  MI_NO_DELAYED_FREE    = 2, // optimize: push on page local thread free queue if another block is already in the heap thread delayed free list
  MI_NEVER_DELAYED_FREE = 3  // sticky: used for abondoned pages without a owning heap; this only resets on page reclaim
} mi_delayed_t;


// The `in_full` and `has_aligned` page flags are put in a union to efficiently
// test if both are false (`full_aligned == 0`) in the `mi_free` routine.
#if !MI_TSAN
typedef union mi_page_flags_s {
  uint8_t full_aligned;
  struct {
    uint8_t in_full : 1;
    uint8_t has_aligned : 1;
  } x;
} mi_page_flags_t;
#else
// under thread sanitizer, use a byte for each flag to suppress warning, issue #130
typedef union mi_page_flags_s {
  uint32_t full_aligned;
  struct {
    uint8_t in_full;
    uint8_t has_aligned;
  } x;
} mi_page_flags_t;
#endif // 1399

// Thread free list.
// We use the bottom 2 bits of the pointer for mi_delayed_t flags
typedef uintptr_t mi_thread_free_t;

// A page contains blocks of one specific size (`block_size`).
// Each page has three list of free blocks:
// `free` for blocks that can be allocated,
// `local_free` for freed blocks that are not yet available to `mi_malloc`
// `thread_free` for freed blocks by other threads
// The `local_free` and `thread_free` lists are migrated to the `free` list
// when it is exhausted. The separate `local_free` list is necessary to
// implement a monotonic heartbeat. The `thread_free` list is needed for
// avoiding atomic operations in the common case.
//
// `used - |thread_free|` == actual blocks that are in use (alive)
// `used - |thread_free| + |free| + |local_free| == capacity`
//
// We don't count `freed` (as |free|) but use `used` to reduce
// the number of memory accesses in the `mi_page_all_free` function(s).
//
// Notes:
// - Access is optimized for `free.c:mi_free` and `alloc.c:mi_page_alloc`
// - Using `uint16_t` does not seem to slow things down
// - The size is 10 words on 64-bit which helps the page index calculations
//   (and 12 words on 32-bit, and encoded free lists add 2 words)
// - `xthread_free` uses the bottom bits as a delayed-free flags to optimize
//   concurrent frees where only the first concurrent free adds to the owning
//   heap `thread_delayed_free` list (see `free.c:mi_free_block_mt`).
//   The invariant is that no-delayed-free is only set if there is
//   at least one block that will be added, or as already been added, to
//   the owning heap `thread_delayed_free` list. This guarantees that pages
//   will be freed correctly even if only other threads free blocks.
typedef struct mi_page_s {
  // "owned" by the segment
  uint8_t               segment_idx;       // index in the segment `pages` array, `page == &segment->pages[page->segment_idx]`
  uint8_t               segment_in_use:1;  // `true` if the segment allocated this page
  uint8_t               is_committed:1;    // `true` if the page virtual memory is committed
  uint8_t               is_zero_init:1;    // `true` if the page was initially zero initialized
  uint8_t               is_huge:1;         // `true` if the page is in a huge segment

  // layout like this to optimize access in `mi_malloc` and `mi_free`
  uint16_t              capacity;          // number of blocks committed, must be the first field, see `segment.c:page_clear`
  uint16_t              reserved;          // number of blocks reserved in memory
  mi_page_flags_t       flags;             // `in_full` and `has_aligned` flags (8 bits)
  uint8_t               free_is_zero:1;    // `true` if the blocks in the free list are zero initialized
  uint8_t               retire_expire:7;   // expiration count for retired blocks

  mi_block_t*           free;              // list of available free blocks (`malloc` allocates from this list)
  mi_block_t*           local_free;        // list of deferred free blocks by this thread (migrates to `free`)
  uint16_t              used;              // number of blocks in use (including blocks in `thread_free`)
  uint8_t               block_size_shift;  // if not zero, then `(1 << block_size_shift) == block_size` (only used for fast path in `free.c:_mi_page_ptr_unalign`)
  uint8_t               heap_tag;          // tag of the owning heap, used to separate heaps by object type
                                           // padding
  size_t                block_size;        // size available in each block (always `>0`)
  uint8_t*              page_start;        // start of the page area containing the blocks

  #if (MI_ENCODE_FREELIST || MI_PADDING)
  uintptr_t             keys[2];           // two random keys to encode the free lists (see `_mi_block_next`) or padding canary
  #endif // 1474

  _Atomic(mi_thread_free_t) xthread_free;  // list of deferred free blocks freed by other threads
  _Atomic(uintptr_t)        xheap;

  struct mi_page_s*     next;              // next page owned by the heap with the same `block_size`
  struct mi_page_s*     prev;              // previous page owned by the heap with the same `block_size`

  #if MI_INTPTR_SIZE==4                    // pad to 12 words on 32-bit
  void* padding[1];
  #endif // 1484
} mi_page_t;



// ------------------------------------------------------
// Mimalloc segments contain mimalloc pages
// ------------------------------------------------------

typedef enum mi_page_kind_e {
  MI_PAGE_SMALL,    // small blocks go into 64KiB pages inside a segment
  MI_PAGE_MEDIUM,   // medium blocks go into 512KiB pages inside a segment
  MI_PAGE_LARGE,    // larger blocks go into a single page spanning a whole segment
  MI_PAGE_HUGE      // a huge page is a single page in a segment of variable size (but still 2MiB aligned)
                    // used for blocks `> MI_LARGE_OBJ_SIZE_MAX` or an aligment `> MI_BLOCK_ALIGNMENT_MAX`.
} mi_page_kind_t;


// ---------------------------------------------------------------
// a memory id tracks the provenance of arena/OS allocated memory
// ---------------------------------------------------------------

// Memory can reside in arena's, direct OS allocated, or statically allocated. The memid keeps track of this.
typedef enum mi_memkind_e {
  MI_MEM_NONE,      // not allocated
  MI_MEM_EXTERNAL,  // not owned by mimalloc but provided externally (via `mi_manage_os_memory` for example)
  MI_MEM_STATIC,    // allocated in a static area and should not be freed (for arena meta data for example)
  MI_MEM_OS,        // allocated from the OS
  MI_MEM_OS_HUGE,   // allocated as huge OS pages (usually 1GiB, pinned to physical memory)
  MI_MEM_OS_REMAP,  // allocated in a remapable area (i.e. using `mremap`)
  MI_MEM_ARENA      // allocated from an arena (the usual case)
} mi_memkind_t;

static inline bool mi_memkind_is_os(mi_memkind_t memkind) {
  return (memkind >= MI_MEM_OS && memkind <= MI_MEM_OS_REMAP);
}

typedef struct mi_memid_os_info {
  void*         base;               // actual base address of the block (used for offset aligned allocations)
  size_t        alignment;          // alignment at allocation
} mi_memid_os_info_t;

typedef struct mi_memid_arena_info {
  size_t        block_index;        // index in the arena
  mi_arena_id_t id;                 // arena id (>= 1)
  bool          is_exclusive;       // this arena can only be used for specific arena allocations
} mi_memid_arena_info_t;

typedef struct mi_memid_s {
  union {
    mi_memid_os_info_t    os;       // only used for MI_MEM_OS
    mi_memid_arena_info_t arena;    // only used for MI_MEM_ARENA
  } mem;
  bool          is_pinned;          // `true` if we cannot decommit/reset/protect in this memory (e.g. when allocated using large (2Mib) or huge (1GiB) OS pages)
  bool          initially_committed;// `true` if the memory was originally allocated as committed
  bool          initially_zero;     // `true` if the memory was originally zero initialized
  mi_memkind_t  memkind;
} mi_memid_t;


// ---------------------------------------------------------------
// Segments contain mimalloc pages
// ---------------------------------------------------------------
typedef struct mi_subproc_s mi_subproc_t;

// Segments are large allocated memory blocks (2MiB on 64 bit) from the OS.
// Inside segments we allocated fixed size _pages_ that contain blocks.
typedef struct mi_segment_s {
  // constant fields
  mi_memid_t           memid;            // memory id to track provenance
  bool                 allow_decommit;
  bool                 allow_purge;
  size_t               segment_size;     // for huge pages this may be different from `MI_SEGMENT_SIZE`
  mi_subproc_t*        subproc;          // segment belongs to sub process

  // segment fields
  struct mi_segment_s* next;             // must be the first (non-constant) segment field  -- see `segment.c:segment_init`
  struct mi_segment_s* prev;
  bool                 was_reclaimed;    // true if it was reclaimed (used to limit reclaim-on-free reclamation)
  bool                 dont_free;        // can be temporarily true to ensure the segment is not freed

  size_t               abandoned;        // abandoned pages (i.e. the original owning thread stopped) (`abandoned <= used`)
  size_t               abandoned_visits; // count how often this segment is visited for reclaiming (to force reclaim if it is too long)

  size_t               used;             // count of pages in use (`used <= capacity`)
  size_t               capacity;         // count of available pages (`#free + used`)
  size_t               segment_info_size;// space we are using from the first page for segment meta-data and possible guard pages.
  uintptr_t            cookie;           // verify addresses in secure mode: `_mi_ptr_cookie(segment) == segment->cookie`

  struct mi_segment_s* abandoned_os_next; // only used for abandoned segments outside arena's, and only if `mi_option_visit_abandoned` is enabled
  struct mi_segment_s* abandoned_os_prev;

  // layout like this to optimize access in `mi_free`
  _Atomic(mi_threadid_t) thread_id;      // unique id of the thread owning this segment
  size_t               page_shift;       // `1 << page_shift` == the page sizes == `page->block_size * page->reserved` (unless the first page, then `-segment_info_size`).
  mi_page_kind_t       page_kind;        // kind of pages: small, medium, large, or huge
  mi_page_t            pages[1];         // up to `MI_SMALL_PAGES_PER_SEGMENT` pages
} mi_segment_t;


// ------------------------------------------------------
// Heaps
// Provide first-class heaps to allocate from.
// A heap just owns a set of pages for allocation and
// can only be allocate/reallocate from the thread that created it.
// Freeing blocks can be done from any thread though.
// Per thread, the segments are shared among its heaps.
// Per thread, there is always a default heap that is
// used for allocation; it is initialized to statically
// point to an empty heap to avoid initialization checks
// in the fast path.
// ------------------------------------------------------

// Thread local data
typedef struct mi_tld_s mi_tld_t;

// Pages of a certain block size are held in a queue.
typedef struct mi_page_queue_s {
  mi_page_t* first;
  mi_page_t* last;
  size_t     block_size;
} mi_page_queue_t;

#define MI_BIN_FULL  (MI_BIN_HUGE+1)

// Random context
typedef struct mi_random_cxt_s {
  uint32_t input[16];
  uint32_t output[16];
  int      output_available;
  bool     weak;
} mi_random_ctx_t;


// In debug mode there is a padding structure at the end of the blocks to check for buffer overflows
#if (MI_PADDING)
typedef struct mi_padding_s {
  uint32_t canary; // encoded block value to check validity of the padding (in case of overflow)
  uint32_t delta;  // padding bytes before the block. (mi_usable_size(p) - delta == exact allocated bytes)
} mi_padding_t;
#define MI_PADDING_SIZE   (sizeof(mi_padding_t))
#define MI_PADDING_WSIZE  ((MI_PADDING_SIZE + MI_INTPTR_SIZE - 1) / MI_INTPTR_SIZE)
#else
#define MI_PADDING_SIZE   0
#define MI_PADDING_WSIZE  0
#endif // 1621

#define MI_PAGES_DIRECT   (MI_SMALL_WSIZE_MAX + MI_PADDING_WSIZE + 1)


// A heap owns a set of pages.
struct mi_heap_s {
  mi_tld_t*             tld;
  _Atomic(mi_block_t*)  thread_delayed_free;
  mi_threadid_t         thread_id;                           // thread this heap belongs too
  mi_arena_id_t         arena_id;                            // arena id if the heap belongs to a specific arena (or 0)
  uintptr_t             cookie;                              // random cookie to verify pointers (see `_mi_ptr_cookie`)
  uintptr_t             keys[2];                             // two random keys used to encode the `thread_delayed_free` list
  mi_random_ctx_t       random;                              // random number context used for secure allocation
  size_t                page_count;                          // total number of pages in the `pages` queues.
  size_t                page_retired_min;                    // smallest retired index (retired pages are fully free, but still in the page queues)
  size_t                page_retired_max;                    // largest retired index into the `pages` array.
  mi_heap_t*            next;                                // list of heaps per thread
  bool                  no_reclaim;                          // `true` if this heap should not reclaim abandoned pages
  uint8_t               tag;                                 // custom tag, can be used for separating heaps based on the object types
  #if MI_GUARDED
  size_t                guarded_size_min;                    // minimal size for guarded objects
  size_t                guarded_size_max;                    // maximal size for guarded objects
  size_t                guarded_sample_rate;                 // sample rate (set to 0 to disable guarded pages)
  size_t                guarded_sample_seed;                 // starting sample count
  size_t                guarded_sample_count;                // current sample count (counting down to 0)
  #endif // 1651
  mi_page_t*            pages_free_direct[MI_PAGES_DIRECT];  // optimize: array where every entry points a page with possibly free blocks in the corresponding queue for that size.
  mi_page_queue_t       pages[MI_BIN_FULL + 1];              // queue of pages for each size class (or "bin")
};



// ------------------------------------------------------
// Debug
// ------------------------------------------------------

#if !defined(MI_DEBUG_UNINIT)
#define MI_DEBUG_UNINIT     (0xD0)
#endif // 1668
#if !defined(MI_DEBUG_FREED)
#define MI_DEBUG_FREED      (0xDF)
#endif // 1671
#if !defined(MI_DEBUG_PADDING)
#define MI_DEBUG_PADDING    (0xDE)
#endif // 1674

#if (MI_DEBUG)
// use our own assertion to print without memory allocation
void _mi_assert_fail(const char* assertion, const char* fname, unsigned int line, const char* func );
#define mi_assert(expr)     ((expr) ? (void)0 : _mi_assert_fail(#expr,__FILE__,__LINE__,__func__))
#else
#define mi_assert(x)
#endif // 1678

#if (MI_DEBUG>1)
#define mi_assert_internal    mi_assert
#else
#define mi_assert_internal(x)
#endif // 1686

#if (MI_DEBUG>2)
#define mi_assert_expensive   mi_assert
#else
#define mi_assert_expensive(x)
#endif // 1692

// ------------------------------------------------------
// Statistics
// ------------------------------------------------------

#ifndef MI_STAT
#if (MI_DEBUG>0)
#define MI_STAT 2
#else
#define MI_STAT 0
#endif // 1703
#endif // 1702

typedef struct mi_stat_count_s {
  int64_t allocated;
  int64_t freed;
  int64_t peak;
  int64_t current;
} mi_stat_count_t;

typedef struct mi_stat_counter_s {
  int64_t total;
  int64_t count;
} mi_stat_counter_t;

typedef struct mi_stats_s {
  mi_stat_count_t segments;
  mi_stat_count_t pages;
  mi_stat_count_t reserved;
  mi_stat_count_t committed;
  mi_stat_count_t reset;
  mi_stat_count_t purged;
  mi_stat_count_t page_committed;
  mi_stat_count_t segments_abandoned;
  mi_stat_count_t pages_abandoned;
  mi_stat_count_t threads;
  mi_stat_count_t normal;
  mi_stat_count_t huge;
  mi_stat_count_t giant;
  mi_stat_count_t malloc;
  mi_stat_count_t segments_cache;
  mi_stat_counter_t pages_extended;
  mi_stat_counter_t mmap_calls;
  mi_stat_counter_t commit_calls;
  mi_stat_counter_t reset_calls;
  mi_stat_counter_t purge_calls;
  mi_stat_counter_t page_no_retire;
  mi_stat_counter_t searches;
  mi_stat_counter_t normal_count;
  mi_stat_counter_t huge_count;
  mi_stat_counter_t arena_count;
  mi_stat_counter_t arena_crossover_count;
  mi_stat_counter_t arena_rollback_count;
  mi_stat_counter_t guarded_alloc_count;
#if MI_STAT>1
  mi_stat_count_t normal_bins[MI_BIN_HUGE+1];
#endif // 1751
} mi_stats_t;


void _mi_stat_increase(mi_stat_count_t* stat, size_t amount);
void _mi_stat_decrease(mi_stat_count_t* stat, size_t amount);
void _mi_stat_counter_increase(mi_stat_counter_t* stat, size_t amount);

#if (MI_STAT)
#define mi_stat_increase(stat,amount)         _mi_stat_increase( &(stat), amount)
#define mi_stat_decrease(stat,amount)         _mi_stat_decrease( &(stat), amount)
#define mi_stat_counter_increase(stat,amount) _mi_stat_counter_increase( &(stat), amount)
#else
#define mi_stat_increase(stat,amount)         (void)0
#define mi_stat_decrease(stat,amount)         (void)0
#define mi_stat_counter_increase(stat,amount) (void)0
#endif // 1761

#define mi_heap_stat_counter_increase(heap,stat,amount)  mi_stat_counter_increase( (heap)->tld->stats.stat, amount)
#define mi_heap_stat_increase(heap,stat,amount)  mi_stat_increase( (heap)->tld->stats.stat, amount)
#define mi_heap_stat_decrease(heap,stat,amount)  mi_stat_decrease( (heap)->tld->stats.stat, amount)


// ------------------------------------------------------
// Sub processes do not reclaim or visit segments
// from other sub processes
// ------------------------------------------------------

struct mi_subproc_s {
  _Atomic(size_t)    abandoned_count;         // count of abandoned segments for this sub-process
  _Atomic(size_t)    abandoned_os_list_count; // count of abandoned segments in the os-list
  mi_lock_t          abandoned_os_lock;       // lock for the abandoned os segment list (outside of arena's) (this lock protect list operations)
  mi_lock_t          abandoned_os_visit_lock; // ensure only one thread per subproc visits the abandoned os list
  mi_segment_t*      abandoned_os_list;       // doubly-linked list of abandoned segments outside of arena's (in OS allocated memory)
  mi_segment_t*      abandoned_os_list_tail;  // the tail-end of the list
  mi_memid_t         memid;                   // provenance of this memory block
};

// ------------------------------------------------------
// Thread Local data
// ------------------------------------------------------

// Milliseconds as in `int64_t` to avoid overflows
typedef int64_t  mi_msecs_t;

// Queue of segments
typedef struct mi_segment_queue_s {
  mi_segment_t* first;
  mi_segment_t* last;
} mi_segment_queue_t;

// OS thread local data
typedef struct mi_os_tld_s {
  size_t                region_idx;   // start point for next allocation
  mi_stats_t*           stats;        // points to tld stats
} mi_os_tld_t;

// Segments thread local data
typedef struct mi_segments_tld_s {
  mi_segment_queue_t  small_free;   // queue of segments with free small pages
  mi_segment_queue_t  medium_free;  // queue of segments with free medium pages
  mi_page_queue_t     pages_purge;  // queue of freed pages that are delay purged
  size_t              count;        // current number of segments;
  size_t              peak_count;   // peak number of segments
  size_t              current_size; // current size of all segments
  size_t              peak_size;    // peak size of all segments
  size_t              reclaim_count;// number of reclaimed (abandoned) segments
  mi_subproc_t*       subproc;      // sub-process this thread belongs to.
  mi_stats_t*         stats;        // points to tld stats
  mi_os_tld_t*        os;           // points to os tld
} mi_segments_tld_t;

// Thread local data
struct mi_tld_s {
  unsigned long long  heartbeat;     // monotonic heartbeat count
  bool                recurse;       // true if deferred was called; used to prevent infinite recursion.
  mi_heap_t*          heap_backing;  // backing heap of this thread (cannot be deleted)
  mi_heap_t*          heaps;         // list of heaps in this thread (so we can abandon all when the thread terminates)
  mi_segments_tld_t   segments;      // segment tld
  mi_os_tld_t         os;            // os tld
  mi_stats_t          stats;         // statistics
};

// #endif // 644
// --- Amalgamation END --- mimalloc/include/mimalloc/types.h


// --- Amalgamation START --- mimalloc/include/mimalloc/track.h
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/
// #ifndef MIMALLOC_TRACK_H
// #define MIMALLOC_TRACK_H

/* ------------------------------------------------------------------------------------------------------
Track memory ranges with macros for tools like Valgrind address sanitizer, or other memory checkers.
These can be defined for tracking allocation:

  #define mi_track_malloc_size(p,reqsize,size,zero)
  #define mi_track_free_size(p,_size)

The macros are set up such that the size passed to `mi_track_free_size`
always matches the size of `mi_track_malloc_size`. (currently, `size == mi_usable_size(p)`).
The `reqsize` is what the user requested, and `size >= reqsize`.
The `size` is either byte precise (and `size==reqsize`) if `MI_PADDING` is enabled,
or otherwise it is the usable block size which may be larger than the original request.
Use `_mi_block_size_of(void* p)` to get the full block size that was allocated (including padding etc).
The `zero` parameter is `true` if the allocated block is zero initialized.

Optional:

  #define mi_track_align(p,alignedp,offset,size)
  #define mi_track_resize(p,oldsize,newsize)
  #define mi_track_init()

The `mi_track_align` is called right after a `mi_track_malloc` for aligned pointers in a block.
The corresponding `mi_track_free` still uses the block start pointer and original size (corresponding to the `mi_track_malloc`).
The `mi_track_resize` is currently unused but could be called on reallocations within a block.
`mi_track_init` is called at program start.

The following macros are for tools like asan and valgrind to track whether memory is
defined, undefined, or not accessible at all:

  #define mi_track_mem_defined(p,size)
  #define mi_track_mem_undefined(p,size)
  #define mi_track_mem_noaccess(p,size)

-------------------------------------------------------------------------------------------------------*/

#if MI_TRACK_VALGRIND
// valgrind tool

#define MI_TRACK_ENABLED      1
#define MI_TRACK_HEAP_DESTROY 1           // track free of individual blocks on heap_destroy
#define MI_TRACK_TOOL         "valgrind"

#include <valgrind/valgrind.h>
#include <valgrind/memcheck.h>

#define mi_track_malloc_size(p,reqsize,size,zero) VALGRIND_MALLOCLIKE_BLOCK(p,size,MI_PADDING_SIZE /*red zone*/,zero)
#define mi_track_free_size(p,_size)               VALGRIND_FREELIKE_BLOCK(p,MI_PADDING_SIZE /*red zone*/)
#define mi_track_resize(p,oldsize,newsize)        VALGRIND_RESIZEINPLACE_BLOCK(p,oldsize,newsize,MI_PADDING_SIZE /*red zone*/)
#define mi_track_mem_defined(p,size)              VALGRIND_MAKE_MEM_DEFINED(p,size)
#define mi_track_mem_undefined(p,size)            VALGRIND_MAKE_MEM_UNDEFINED(p,size)
#define mi_track_mem_noaccess(p,size)             VALGRIND_MAKE_MEM_NOACCESS(p,size)

#elif MI_TRACK_ASAN
// address sanitizer

#define MI_TRACK_ENABLED      1
#define MI_TRACK_HEAP_DESTROY 0
#define MI_TRACK_TOOL         "asan"

#include <sanitizer/asan_interface.h>

#define mi_track_malloc_size(p,reqsize,size,zero) ASAN_UNPOISON_MEMORY_REGION(p,size)
#define mi_track_free_size(p,size)                ASAN_POISON_MEMORY_REGION(p,size)
#define mi_track_mem_defined(p,size)              ASAN_UNPOISON_MEMORY_REGION(p,size)
#define mi_track_mem_undefined(p,size)            ASAN_UNPOISON_MEMORY_REGION(p,size)
#define mi_track_mem_noaccess(p,size)             ASAN_POISON_MEMORY_REGION(p,size)

#elif MI_TRACK_ETW
// windows event tracing

#define MI_TRACK_ENABLED      1
#define MI_TRACK_HEAP_DESTROY 1
#define MI_TRACK_TOOL         "ETW"


// --- Amalgamation START --- mimalloc/src/../src/prim/windows/etw.h
//**********************************************************************`
//* This is an include file generated by Message Compiler.             *`
//*                                                                    *`
//* Copyright (c) Microsoft Corporation. All Rights Reserved.          *`
//**********************************************************************`

//*****************************************************************************
//
// Notes on the ETW event code generated by MC:
//
// - Structures and arrays of structures are treated as an opaque binary blob.
//   The caller is responsible for packing the data for the structure into a
//   single region of memory, with no padding between values. The macro will
//   have an extra parameter for the length of the blob.
// - Arrays of nul-terminated strings must be packed by the caller into a
//   single binary blob containing the correct number of strings, with a nul
//   after each string. The size of the blob is specified in characters, and
//   includes the final nul.
// - Arrays of SID are treated as a single binary blob. The caller is
//   responsible for packing the SID values into a single region of memory with
//   no padding.
// - The length attribute on the data element in the manifest is significant
//   for values with intype win:UnicodeString, win:AnsiString, or win:Binary.
//   The length attribute must be specified for win:Binary, and is optional for
//   win:UnicodeString and win:AnsiString (if no length is given, the strings
//   are assumed to be nul-terminated). For win:UnicodeString, the length is
//   measured in characters, not bytes.
// - For an array of win:UnicodeString, win:AnsiString, or win:Binary, the
//   length attribute applies to every value in the array, so every value in
//   the array must have the same length. The values in the array are provided
//   to the macro via a single pointer -- the caller is responsible for packing
//   all of the values into a single region of memory with no padding between
//   values.
// - Values of type win:CountedUnicodeString, win:CountedAnsiString, and
//   win:CountedBinary can be generated and collected on Vista or later.
//   However, they may not decode properly without the Windows 10 2018 Fall
//   Update.
// - Arrays of type win:CountedUnicodeString, win:CountedAnsiString, and
//   win:CountedBinary must be packed by the caller into a single region of
//   memory. The format for each item is a UINT16 byte-count followed by that
//   many bytes of data. When providing the array to the generated macro, you
//   must provide the total size of the packed array data, including the UINT16
//   sizes for each item. In the case of win:CountedUnicodeString, the data
//   size is specified in WCHAR (16-bit) units. In the case of
//   win:CountedAnsiString and win:CountedBinary, the data size is specified in
//   bytes.
//
//*****************************************************************************

#include <wmistr.h>
#include <evntrace.h>
#include <evntprov.h>

#ifndef ETW_INLINE
  #ifdef _ETW_KM_
    // In kernel mode, save stack space by never inlining templates.
    #define ETW_INLINE DECLSPEC_NOINLINE __inline
  #else
    // In user mode, save code size by inlining templates as appropriate.
    #define ETW_INLINE __inline
  #endif // 1980
#endif // ETW_INLINE // 1979

#if 0 // #if defined(__cplusplus)
// extern "C" {
#endif // 1989

//
// MCGEN_DISABLE_PROVIDER_CODE_GENERATION macro:
// Define this macro to have the compiler skip the generated functions in this
// header.
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION

//
// MCGEN_USE_KERNEL_MODE_APIS macro:
// Controls whether the generated code uses kernel-mode or user-mode APIs.
// - Set to 0 to use Windows user-mode APIs such as EventRegister.
// - Set to 1 to use Windows kernel-mode APIs such as EtwRegister.
// Default is based on whether the _ETW_KM_ macro is defined (i.e. by wdm.h).
// Note that the APIs can also be overridden directly, e.g. by setting the
// MCGEN_EVENTWRITETRANSFER or MCGEN_EVENTREGISTER macros.
//
#ifndef MCGEN_USE_KERNEL_MODE_APIS
  #ifdef _ETW_KM_
    #define MCGEN_USE_KERNEL_MODE_APIS 1
  #else
    #define MCGEN_USE_KERNEL_MODE_APIS 0
  #endif // 2010
#endif // MCGEN_USE_KERNEL_MODE_APIS // 2009

//
// MCGEN_HAVE_EVENTSETINFORMATION macro:
// Controls how McGenEventSetInformation uses the EventSetInformation API.
// - Set to 0 to disable the use of EventSetInformation
//   (McGenEventSetInformation will always return an error).
// - Set to 1 to directly invoke MCGEN_EVENTSETINFORMATION.
// - Set to 2 to to locate EventSetInformation at runtime via GetProcAddress
//   (user-mode) or MmGetSystemRoutineAddress (kernel-mode).
// Default is determined as follows:
// - If MCGEN_EVENTSETINFORMATION has been customized, set to 1
//   (i.e. use MCGEN_EVENTSETINFORMATION).
// - Else if the target OS version has EventSetInformation, set to 1
//   (i.e. use MCGEN_EVENTSETINFORMATION).
// - Else set to 2 (i.e. try to dynamically locate EventSetInformation).
// Note that an McGenEventSetInformation function will only be generated if one
// or more provider in a manifest has provider traits.
//
#ifndef MCGEN_HAVE_EVENTSETINFORMATION
  #ifdef MCGEN_EVENTSETINFORMATION             // if MCGEN_EVENTSETINFORMATION has been customized,
    #define MCGEN_HAVE_EVENTSETINFORMATION   1 //   directly invoke MCGEN_EVENTSETINFORMATION(...).
  #elif MCGEN_USE_KERNEL_MODE_APIS             // else if using kernel-mode APIs,
    #if NTDDI_VERSION >= 0x06040000            //   if target OS is Windows 10 or later,
      #define MCGEN_HAVE_EVENTSETINFORMATION 1 //     directly invoke MCGEN_EVENTSETINFORMATION(...).
    #else                                      //   else
      #define MCGEN_HAVE_EVENTSETINFORMATION 2 //     find "EtwSetInformation" via MmGetSystemRoutineAddress.
    #endif                                     // else (using user-mode APIs) // 2038
  #else                                        //   if target OS and SDK is Windows 8 or later,
    #if WINVER >= 0x0602 && defined(EVENT_FILTER_TYPE_SCHEMATIZED)
      #define MCGEN_HAVE_EVENTSETINFORMATION 1 //     directly invoke MCGEN_EVENTSETINFORMATION(...).
    #else                                      //   else
      #define MCGEN_HAVE_EVENTSETINFORMATION 2 //     find "EventSetInformation" via GetModuleHandleExW/GetProcAddress.
    #endif // 2044
  #endif // 2035
#endif // MCGEN_HAVE_EVENTSETINFORMATION // 2034

//
// MCGEN Override Macros
//
// The following override macros may be defined before including this header
// to control the APIs used by this header:
//
// - MCGEN_EVENTREGISTER
// - MCGEN_EVENTUNREGISTER
// - MCGEN_EVENTSETINFORMATION
// - MCGEN_EVENTWRITETRANSFER
//
// If the the macro is undefined, the MC implementation will default to the
// corresponding ETW APIs. For example, if the MCGEN_EVENTREGISTER macro is
// undefined, the EventRegister[MyProviderName] macro will use EventRegister
// in user mode and will use EtwRegister in kernel mode.
//
// To prevent issues from conflicting definitions of these macros, the value
// of the override macro will be used as a suffix in certain internal function
// names. Because of this, the override macros must follow certain rules:
//
// - The macro must be defined before any MC-generated header is included and
//   must not be undefined or redefined after any MC-generated header is
//   included. Different translation units (i.e. different .c or .cpp files)
//   may set the macros to different values, but within a translation unit
//   (within a single .c or .cpp file), the macro must be set once and not
//   changed.
// - The override must be an object-like macro, not a function-like macro
//   (i.e. the override macro must not have a parameter list).
// - The override macro's value must be a simple identifier, i.e. must be
//   something that starts with a letter or '_' and contains only letters,
//   numbers, and '_' characters.
// - If the override macro's value is the name of a second object-like macro,
//   the second object-like macro must follow the same rules. (The override
//   macro's value can also be the name of a function-like macro, in which
//   case the function-like macro does not need to follow the same rules.)
//
// For example, the following will cause compile errors:
//
//   #define MCGEN_EVENTWRITETRANSFER MyNamespace::MyClass::MyFunction // Value has non-identifier characters (colon).
//   #define MCGEN_EVENTWRITETRANSFER GetEventWriteFunctionPointer(7)  // Value has non-identifier characters (parentheses).
//   #define MCGEN_EVENTWRITETRANSFER(h,e,a,r,c,d) EventWrite(h,e,c,d) // Override is defined as a function-like macro.
//   #define MY_OBJECT_LIKE_MACRO     MyNamespace::MyClass::MyEventWriteFunction
//   #define MCGEN_EVENTWRITETRANSFER MY_OBJECT_LIKE_MACRO // Evaluates to something with non-identifier characters (colon).
//
// The following would be ok:
//
//   #define MCGEN_EVENTWRITETRANSFER  MyEventWriteFunction1  // OK, suffix will be "MyEventWriteFunction1".
//   #define MY_OBJECT_LIKE_MACRO      MyEventWriteFunction2
//   #define MCGEN_EVENTWRITETRANSFER  MY_OBJECT_LIKE_MACRO   // OK, suffix will be "MyEventWriteFunction2".
//   #define MY_FUNCTION_LIKE_MACRO(h,e,a,r,c,d) MyNamespace::MyClass::MyEventWriteFunction3(h,e,c,d)
//   #define MCGEN_EVENTWRITETRANSFER  MY_FUNCTION_LIKE_MACRO // OK, suffix will be "MY_FUNCTION_LIKE_MACRO".
//
#ifndef MCGEN_EVENTREGISTER
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define MCGEN_EVENTREGISTER        EtwRegister
  #else
    #define MCGEN_EVENTREGISTER        EventRegister
  #endif // 2105
#endif // MCGEN_EVENTREGISTER // 2104
#ifndef MCGEN_EVENTUNREGISTER
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define MCGEN_EVENTUNREGISTER      EtwUnregister
  #else
    #define MCGEN_EVENTUNREGISTER      EventUnregister
  #endif // 2112
#endif // MCGEN_EVENTUNREGISTER // 2111
#ifndef MCGEN_EVENTSETINFORMATION
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define MCGEN_EVENTSETINFORMATION  EtwSetInformation
  #else
    #define MCGEN_EVENTSETINFORMATION  EventSetInformation
  #endif // 2119
#endif // MCGEN_EVENTSETINFORMATION // 2118
#ifndef MCGEN_EVENTWRITETRANSFER
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define MCGEN_EVENTWRITETRANSFER   EtwWriteTransfer
  #else
    #define MCGEN_EVENTWRITETRANSFER   EventWriteTransfer
  #endif // 2126
#endif // MCGEN_EVENTWRITETRANSFER // 2125

//
// MCGEN_EVENT_ENABLED macro:
// Override to control how the EventWrite[EventName] macros determine whether
// an event is enabled. The default behavior is for EventWrite[EventName] to
// use the EventEnabled[EventName] macros.
//
#ifndef MCGEN_EVENT_ENABLED
#define MCGEN_EVENT_ENABLED(EventName) EventEnabled##EventName()
#endif // 2139

//
// MCGEN_EVENT_ENABLED_FORCONTEXT macro:
// Override to control how the EventWrite[EventName]_ForContext macros
// determine whether an event is enabled. The default behavior is for
// EventWrite[EventName]_ForContext to use the
// EventEnabled[EventName]_ForContext macros.
//
#ifndef MCGEN_EVENT_ENABLED_FORCONTEXT
#define MCGEN_EVENT_ENABLED_FORCONTEXT(pContext, EventName) EventEnabled##EventName##_ForContext(pContext)
#endif // 2150

//
// MCGEN_ENABLE_CHECK macro:
// Determines whether the specified event would be considered as enabled
// based on the state of the specified context. Slightly faster than calling
// McGenEventEnabled directly.
//
#ifndef MCGEN_ENABLE_CHECK
#define MCGEN_ENABLE_CHECK(Context, Descriptor) (Context.IsEnabled && McGenEventEnabled(&Context, &Descriptor))
#endif // 2160

#if !defined(MCGEN_TRACE_CONTEXT_DEF)
#define MCGEN_TRACE_CONTEXT_DEF
// This structure is for use by MC-generated code and should not be used directly.
typedef struct _MCGEN_TRACE_CONTEXT
{
    TRACEHANDLE            RegistrationHandle;
    TRACEHANDLE            Logger;      // Used as pointer to provider traits.
    ULONGLONG              MatchAnyKeyword;
    ULONGLONG              MatchAllKeyword;
    ULONG                  Flags;
    ULONG                  IsEnabled;
    UCHAR                  Level;
    UCHAR                  Reserve;
    USHORT                 EnableBitsCount;
    PULONG                 EnableBitMask;
    const ULONGLONG*       EnableKeyWords;
    const UCHAR*           EnableLevel;
} MCGEN_TRACE_CONTEXT, *PMCGEN_TRACE_CONTEXT;
#endif // MCGEN_TRACE_CONTEXT_DEF // 2164

#if !defined(MCGEN_LEVEL_KEYWORD_ENABLED_DEF)
#define MCGEN_LEVEL_KEYWORD_ENABLED_DEF
//
// Determines whether an event with a given Level and Keyword would be
// considered as enabled based on the state of the specified context.
// Note that you may want to use MCGEN_ENABLE_CHECK instead of calling this
// function directly.
//
FORCEINLINE
BOOLEAN
McGenLevelKeywordEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ UCHAR Level,
    _In_ ULONGLONG Keyword
    )
{
    //
    // Check if the event Level is lower than the level at which
    // the channel is enabled.
    // If the event Level is 0 or the channel is enabled at level 0,
    // all levels are enabled.
    //

    if ((Level <= EnableInfo->Level) || // This also covers the case of Level == 0.
        (EnableInfo->Level == 0)) {

        //
        // Check if Keyword is enabled
        //

        if ((Keyword == (ULONGLONG)0) ||
            ((Keyword & EnableInfo->MatchAnyKeyword) &&
             ((Keyword & EnableInfo->MatchAllKeyword) == EnableInfo->MatchAllKeyword))) {
            return TRUE;
        }
    }

    return FALSE;
}
#endif // MCGEN_LEVEL_KEYWORD_ENABLED_DEF // 2184

#if !defined(MCGEN_EVENT_ENABLED_DEF)
#define MCGEN_EVENT_ENABLED_DEF
//
// Determines whether the specified event would be considered as enabled based
// on the state of the specified context. Note that you may want to use
// MCGEN_ENABLE_CHECK instead of calling this function directly.
//
FORCEINLINE
BOOLEAN
McGenEventEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{
    return McGenLevelKeywordEnabled(EnableInfo, EventDescriptor->Level, EventDescriptor->Keyword);
}
#endif // MCGEN_EVENT_ENABLED_DEF // 2225

#if !defined(MCGEN_CONTROL_CALLBACK)
#define MCGEN_CONTROL_CALLBACK

// This function is for use by MC-generated code and should not be used directly.
DECLSPEC_NOINLINE __inline
VOID
__stdcall
McGenControlCallbackV2(
    _In_ LPCGUID SourceId,
    _In_ ULONG ControlCode,
    _In_ UCHAR Level,
    _In_ ULONGLONG MatchAnyKeyword,
    _In_ ULONGLONG MatchAllKeyword,
    _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData,
    _Inout_opt_ PVOID CallbackContext
    )
/*++

Routine Description:

    This is the notification callback for Windows Vista and later.

Arguments:

    SourceId - The GUID that identifies the session that enabled the provider.

    ControlCode - The parameter indicates whether the provider
                  is being enabled or disabled.

    Level - The level at which the event is enabled.

    MatchAnyKeyword - The bitmask of keywords that the provider uses to
                      determine the category of events that it writes.

    MatchAllKeyword - This bitmask additionally restricts the category
                      of events that the provider writes.

    FilterData - The provider-defined data.

    CallbackContext - The context of the callback that is defined when the provider
                      called EtwRegister to register itself.

Remarks:

    ETW calls this function to notify provider of enable/disable

--*/
{
    PMCGEN_TRACE_CONTEXT Ctx = (PMCGEN_TRACE_CONTEXT)CallbackContext;
    ULONG Ix;
#ifndef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    UNREFERENCED_PARAMETER(SourceId);
    UNREFERENCED_PARAMETER(FilterData);
#endif // 2293

    if (Ctx == NULL) {
        return;
    }

    switch (ControlCode) {

        case EVENT_CONTROL_CODE_ENABLE_PROVIDER:
            Ctx->Level = Level;
            Ctx->MatchAnyKeyword = MatchAnyKeyword;
            Ctx->MatchAllKeyword = MatchAllKeyword;
            Ctx->IsEnabled = EVENT_CONTROL_CODE_ENABLE_PROVIDER;

            for (Ix = 0; Ix < Ctx->EnableBitsCount; Ix += 1) {
                if (McGenLevelKeywordEnabled(Ctx, Ctx->EnableLevel[Ix], Ctx->EnableKeyWords[Ix]) != FALSE) {
                    Ctx->EnableBitMask[Ix >> 5] |= (1 << (Ix % 32));
                } else {
                    Ctx->EnableBitMask[Ix >> 5] &= ~(1 << (Ix % 32));
                }
            }
            break;

        case EVENT_CONTROL_CODE_DISABLE_PROVIDER:
            Ctx->IsEnabled = EVENT_CONTROL_CODE_DISABLE_PROVIDER;
            Ctx->Level = 0;
            Ctx->MatchAnyKeyword = 0;
            Ctx->MatchAllKeyword = 0;
            if (Ctx->EnableBitsCount > 0) {
#pragma warning(suppress: 26451) // Arithmetic overflow cannot occur, no matter the value of EnableBitCount
                RtlZeroMemory(Ctx->EnableBitMask, (((Ctx->EnableBitsCount - 1) / 32) + 1) * sizeof(ULONG));
            }
            break;

        default:
            break;
    }

#ifdef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    //
    // Call user defined callback
    //
    MCGEN_PRIVATE_ENABLE_CALLBACK_V2(
        SourceId,
        ControlCode,
        Level,
        MatchAnyKeyword,
        MatchAllKeyword,
        FilterData,
        CallbackContext
        );
#endif // MCGEN_PRIVATE_ENABLE_CALLBACK_V2 // 2334

    return;
}

#endif // MCGEN_CONTROL_CALLBACK // 2243

#ifndef _mcgen_PENABLECALLBACK
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define _mcgen_PENABLECALLBACK      PETWENABLECALLBACK
  #else
    #define _mcgen_PENABLECALLBACK      PENABLECALLBACK
  #endif // 2355
#endif // _mcgen_PENABLECALLBACK // 2354

#if !defined(_mcgen_PASTE2)
// This macro is for use by MC-generated code and should not be used directly.
#define _mcgen_PASTE2(a, b) _mcgen_PASTE2_imp(a, b)
#define _mcgen_PASTE2_imp(a, b) a##b
#endif // _mcgen_PASTE2 // 2362

#if !defined(_mcgen_PASTE3)
// This macro is for use by MC-generated code and should not be used directly.
#define _mcgen_PASTE3(a, b, c) _mcgen_PASTE3_imp(a, b, c)
#define _mcgen_PASTE3_imp(a, b, c) a##b##_##c
#endif // _mcgen_PASTE3 // 2368

//
// Macro validation
//

// Validate MCGEN_EVENTREGISTER:

// Trigger an error if MCGEN_EVENTREGISTER is not an unqualified (simple) identifier:
struct _mcgen_PASTE2(MCGEN_EVENTREGISTER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTREGISTER);

// Trigger an error if MCGEN_EVENTREGISTER is redefined:
typedef struct _mcgen_PASTE2(MCGEN_EVENTREGISTER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTREGISTER)
    MCGEN_EVENTREGISTER_must_not_be_redefined_between_headers;

// Trigger an error if MCGEN_EVENTREGISTER is defined as a function-like macro:
typedef void MCGEN_EVENTREGISTER_must_not_be_a_functionLike_macro_MCGEN_EVENTREGISTER;
typedef int _mcgen_PASTE2(MCGEN_EVENTREGISTER_must_not_be_a_functionLike_macro_, MCGEN_EVENTREGISTER);

// Validate MCGEN_EVENTUNREGISTER:

// Trigger an error if MCGEN_EVENTUNREGISTER is not an unqualified (simple) identifier:
struct _mcgen_PASTE2(MCGEN_EVENTUNREGISTER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTUNREGISTER);

// Trigger an error if MCGEN_EVENTUNREGISTER is redefined:
typedef struct _mcgen_PASTE2(MCGEN_EVENTUNREGISTER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTUNREGISTER)
    MCGEN_EVENTUNREGISTER_must_not_be_redefined_between_headers;

// Trigger an error if MCGEN_EVENTUNREGISTER is defined as a function-like macro:
typedef void MCGEN_EVENTUNREGISTER_must_not_be_a_functionLike_macro_MCGEN_EVENTUNREGISTER;
typedef int _mcgen_PASTE2(MCGEN_EVENTUNREGISTER_must_not_be_a_functionLike_macro_, MCGEN_EVENTUNREGISTER);

// Validate MCGEN_EVENTSETINFORMATION:

// Trigger an error if MCGEN_EVENTSETINFORMATION is not an unqualified (simple) identifier:
struct _mcgen_PASTE2(MCGEN_EVENTSETINFORMATION_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTSETINFORMATION);

// Trigger an error if MCGEN_EVENTSETINFORMATION is redefined:
typedef struct _mcgen_PASTE2(MCGEN_EVENTSETINFORMATION_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTSETINFORMATION)
    MCGEN_EVENTSETINFORMATION_must_not_be_redefined_between_headers;

// Trigger an error if MCGEN_EVENTSETINFORMATION is defined as a function-like macro:
typedef void MCGEN_EVENTSETINFORMATION_must_not_be_a_functionLike_macro_MCGEN_EVENTSETINFORMATION;
typedef int _mcgen_PASTE2(MCGEN_EVENTSETINFORMATION_must_not_be_a_functionLike_macro_, MCGEN_EVENTSETINFORMATION);

// Validate MCGEN_EVENTWRITETRANSFER:

// Trigger an error if MCGEN_EVENTWRITETRANSFER is not an unqualified (simple) identifier:
struct _mcgen_PASTE2(MCGEN_EVENTWRITETRANSFER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTWRITETRANSFER);

// Trigger an error if MCGEN_EVENTWRITETRANSFER is redefined:
typedef struct _mcgen_PASTE2(MCGEN_EVENTWRITETRANSFER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTWRITETRANSFER)
    MCGEN_EVENTWRITETRANSFER_must_not_be_redefined_between_headers;;

// Trigger an error if MCGEN_EVENTWRITETRANSFER is defined as a function-like macro:
typedef void MCGEN_EVENTWRITETRANSFER_must_not_be_a_functionLike_macro_MCGEN_EVENTWRITETRANSFER;
typedef int _mcgen_PASTE2(MCGEN_EVENTWRITETRANSFER_must_not_be_a_functionLike_macro_, MCGEN_EVENTWRITETRANSFER);

#ifndef McGenEventWrite_def
#define McGenEventWrite_def

// This macro is for use by MC-generated code and should not be used directly.
#define McGenEventWrite _mcgen_PASTE2(McGenEventWrite_, MCGEN_EVENTWRITETRANSFER)

// This function is for use by MC-generated code and should not be used directly.
DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventWrite(
    _In_ PMCGEN_TRACE_CONTEXT Context,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCGUID ActivityId,
    _In_range_(1, 128) ULONG EventDataCount,
    _Pre_cap_(EventDataCount) EVENT_DATA_DESCRIPTOR* EventData
    )
{
    const USHORT UNALIGNED* Traits;

    // Some customized MCGEN_EVENTWRITETRANSFER macros might ignore ActivityId.
    UNREFERENCED_PARAMETER(ActivityId);

    Traits = (const USHORT UNALIGNED*)(UINT_PTR)Context->Logger;

    if (Traits == NULL) {
        EventData[0].Ptr = 0;
        EventData[0].Size = 0;
        EventData[0].Reserved = 0;
    } else {
        EventData[0].Ptr = (ULONG_PTR)Traits;
        EventData[0].Size = *Traits;
        EventData[0].Reserved = 2; // EVENT_DATA_DESCRIPTOR_TYPE_PROVIDER_METADATA
    }

    return MCGEN_EVENTWRITETRANSFER(
        Context->RegistrationHandle,
        Descriptor,
        ActivityId,
        NULL,
        EventDataCount,
        EventData);
}
#endif // McGenEventWrite_def // 2430

#if !defined(McGenEventRegisterUnregister)
#define McGenEventRegisterUnregister

// This macro is for use by MC-generated code and should not be used directly.
#define McGenEventRegister _mcgen_PASTE2(McGenEventRegister_, MCGEN_EVENTREGISTER)

#pragma warning(push)
#pragma warning(disable:6103)
// This function is for use by MC-generated code and should not be used directly.
DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventRegister(
    _In_ LPCGUID ProviderId,
    _In_opt_ _mcgen_PENABLECALLBACK EnableCallback,
    _In_opt_ PVOID CallbackContext,
    _Inout_ PREGHANDLE RegHandle
    )
/*++

Routine Description:

    This function registers the provider with ETW.

Arguments:

    ProviderId - Provider ID to register with ETW.

    EnableCallback - Callback to be used.

    CallbackContext - Context for the callback.

    RegHandle - Pointer to registration handle.

Remarks:

    Should not be called if the provider is already registered (i.e. should not
    be called if *RegHandle != 0). Repeatedly registering a provider is a bug
    and may indicate a race condition. However, for compatibility with previous
    behavior, this function will return SUCCESS in this case.

--*/
{
    ULONG Error;

    if (*RegHandle != 0)
    {
        Error = 0; // ERROR_SUCCESS
    }
    else
    {
        Error = MCGEN_EVENTREGISTER(ProviderId, EnableCallback, CallbackContext, RegHandle);
    }

    return Error;
}
#pragma warning(pop)

// This macro is for use by MC-generated code and should not be used directly.
#define McGenEventUnregister _mcgen_PASTE2(McGenEventUnregister_, MCGEN_EVENTUNREGISTER)

// This function is for use by MC-generated code and should not be used directly.
DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventUnregister(_Inout_ PREGHANDLE RegHandle)
/*++

Routine Description:

    Unregister from ETW and set *RegHandle = 0.

Arguments:

    RegHandle - the pointer to the provider registration handle

Remarks:

    If provider has not been registered (i.e. if *RegHandle == 0),
    return SUCCESS. It is safe to call McGenEventUnregister even if the
    call to McGenEventRegister returned an error.

--*/
{
    ULONG Error;

    if(*RegHandle == 0)
    {
        Error = 0; // ERROR_SUCCESS
    }
    else
    {
        Error = MCGEN_EVENTUNREGISTER(*RegHandle);
        *RegHandle = (REGHANDLE)0;
    }

    return Error;
}

#endif // McGenEventRegisterUnregister // 2474

#ifndef _mcgen_EVENT_BIT_SET
  #if defined(_M_IX86) || defined(_M_X64)
    // This macro is for use by MC-generated code and should not be used directly.
    #define _mcgen_EVENT_BIT_SET(EnableBits, BitPosition) ((((const unsigned char*)EnableBits)[BitPosition >> 3] & (1u << (BitPosition & 7))) != 0)
  #else // CPU type
    // This macro is for use by MC-generated code and should not be used directly.
    #define _mcgen_EVENT_BIT_SET(EnableBits, BitPosition) ((EnableBits[BitPosition >> 5] & (1u << (BitPosition & 31))) != 0)
  #endif // CPU type // 2574
#endif // _mcgen_EVENT_BIT_SET // 2573

#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION // 1998

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Provider "microsoft-windows-mimalloc" event count 2
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Provider GUID = 138f4dbb-ee04-4899-aa0a-572ad4475779
EXTERN_C __declspec(selectany) const GUID ETW_MI_Provider = {0x138f4dbb, 0xee04, 0x4899, {0xaa, 0x0a, 0x57, 0x2a, 0xd4, 0x47, 0x57, 0x79}};

#ifndef ETW_MI_Provider_Traits
#define ETW_MI_Provider_Traits NULL
#endif // ETW_MI_Provider_Traits // 2592

//
// Event Descriptors
//
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR ETW_MI_ALLOC = {0x64, 0x1, 0x0, 0x4, 0x0, 0x0, 0x0};
#define ETW_MI_ALLOC_value 0x64
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR ETW_MI_FREE = {0x65, 0x1, 0x0, 0x4, 0x0, 0x0, 0x0};
#define ETW_MI_FREE_value 0x65

//
// MCGEN_DISABLE_PROVIDER_CODE_GENERATION macro:
// Define this macro to have the compiler skip the generated functions in this
// header.
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION

//
// Event Enablement Bits
// These variables are for use by MC-generated code and should not be used directly.
//
EXTERN_C __declspec(selectany) DECLSPEC_CACHEALIGN ULONG microsoft_windows_mimallocEnableBits[1];
EXTERN_C __declspec(selectany) const ULONGLONG microsoft_windows_mimallocKeywords[1] = {0x0};
EXTERN_C __declspec(selectany) const unsigned char microsoft_windows_mimallocLevels[1] = {4};

//
// Provider context
//
EXTERN_C __declspec(selectany) MCGEN_TRACE_CONTEXT ETW_MI_Provider_Context = {0, (ULONG_PTR)ETW_MI_Provider_Traits, 0, 0, 0, 0, 0, 0, 1, microsoft_windows_mimallocEnableBits, microsoft_windows_mimallocKeywords, microsoft_windows_mimallocLevels};

//
// Provider REGHANDLE
//
#define microsoft_windows_mimallocHandle (ETW_MI_Provider_Context.RegistrationHandle)

//
// This macro is set to 0, indicating that the EventWrite[Name] macros do not
// have an Activity parameter. This is controlled by the -km and -um options.
//
#define ETW_MI_Provider_EventWriteActivity 0

//
// Register with ETW using the control GUID specified in the manifest.
// Invoke this macro during module initialization (i.e. program startup,
// DLL process attach, or driver load) to initialize the provider.
// Note that if this function returns an error, the error means that
// will not work, but no action needs to be taken -- even if EventRegister
// returns an error, it is generally safe to use EventWrite and
// EventUnregister macros (they will be no-ops if EventRegister failed).
//
#ifndef EventRegistermicrosoft_windows_mimalloc
#define EventRegistermicrosoft_windows_mimalloc() McGenEventRegister(&ETW_MI_Provider, McGenControlCallbackV2, &ETW_MI_Provider_Context, &microsoft_windows_mimallocHandle)
#endif // 2644

//
// Register with ETW using a specific control GUID (i.e. a GUID other than what
// is specified in the manifest). Advanced scenarios only.
//
#ifndef EventRegisterByGuidmicrosoft_windows_mimalloc
#define EventRegisterByGuidmicrosoft_windows_mimalloc(Guid) McGenEventRegister(&(Guid), McGenControlCallbackV2, &ETW_MI_Provider_Context, &microsoft_windows_mimallocHandle)
#endif // 2652

//
// Unregister with ETW and close the provider.
// Invoke this macro during module shutdown (i.e. program exit, DLL process
// detach, or driver unload) to unregister the provider.
// Note that you MUST call EventUnregister before DLL or driver unload
// (not optional): failure to unregister a provider before DLL or driver unload
// will result in crashes.
//
#ifndef EventUnregistermicrosoft_windows_mimalloc
#define EventUnregistermicrosoft_windows_mimalloc() McGenEventUnregister(&microsoft_windows_mimallocHandle)
#endif // 2664

//
// MCGEN_ENABLE_FORCONTEXT_CODE_GENERATION macro:
// Define this macro to enable support for caller-allocated provider context.
//
#ifdef MCGEN_ENABLE_FORCONTEXT_CODE_GENERATION

//
// Advanced scenarios: Caller-allocated provider context.
// Use when multiple differently-configured provider handles are needed,
// e.g. for container-aware drivers, one context per container.
//
// Usage:
//
// - Caller enables the feature before including this header, e.g.
//   #define MCGEN_ENABLE_FORCONTEXT_CODE_GENERATION 1
// - Caller allocates memory, e.g. pContext = malloc(sizeof(McGenContext_microsoft_windows_mimalloc));
// - Caller registers the provider, e.g. EventRegistermicrosoft_windows_mimalloc_ForContext(pContext);
// - Caller writes events, e.g. EventWriteMyEvent_ForContext(pContext, ...);
// - Caller unregisters, e.g. EventUnregistermicrosoft_windows_mimalloc_ForContext(pContext);
// - Caller frees memory, e.g. free(pContext);
//

typedef struct tagMcGenContext_microsoft_windows_mimalloc {
    // The fields of this structure are subject to change and should
    // not be accessed directly. To access the provider's REGHANDLE,
    // use microsoft_windows_mimallocHandle_ForContext(pContext).
    MCGEN_TRACE_CONTEXT Context;
    ULONG EnableBits[1];
} McGenContext_microsoft_windows_mimalloc;

#define EventRegistermicrosoft_windows_mimalloc_ForContext(pContext)             _mcgen_PASTE2(_mcgen_RegisterForContext_microsoft_windows_mimalloc_, MCGEN_EVENTREGISTER)(&ETW_MI_Provider, pContext)
#define EventRegisterByGuidmicrosoft_windows_mimalloc_ForContext(Guid, pContext) _mcgen_PASTE2(_mcgen_RegisterForContext_microsoft_windows_mimalloc_, MCGEN_EVENTREGISTER)(&(Guid), pContext)
#define EventUnregistermicrosoft_windows_mimalloc_ForContext(pContext)           McGenEventUnregister(&(pContext)->Context.RegistrationHandle)

//
// Provider REGHANDLE for caller-allocated context.
//
#define microsoft_windows_mimallocHandle_ForContext(pContext) ((pContext)->Context.RegistrationHandle)

// This function is for use by MC-generated code and should not be used directly.
// Initialize and register the caller-allocated context.
__inline
ULONG __stdcall
_mcgen_PASTE2(_mcgen_RegisterForContext_microsoft_windows_mimalloc_, MCGEN_EVENTREGISTER)(
    _In_ LPCGUID pProviderId,
    _Out_ McGenContext_microsoft_windows_mimalloc* pContext)
{
    RtlZeroMemory(pContext, sizeof(*pContext));
    pContext->Context.Logger = (ULONG_PTR)ETW_MI_Provider_Traits;
    pContext->Context.EnableBitsCount = 1;
    pContext->Context.EnableBitMask = pContext->EnableBits;
    pContext->Context.EnableKeyWords = microsoft_windows_mimallocKeywords;
    pContext->Context.EnableLevel = microsoft_windows_mimallocLevels;
    return McGenEventRegister(
        pProviderId,
        McGenControlCallbackV2,
        &pContext->Context,
        &pContext->Context.RegistrationHandle);
}

// This function is for use by MC-generated code and should not be used directly.
// Trigger a compile error if called with the wrong parameter type.
FORCEINLINE
_Ret_ McGenContext_microsoft_windows_mimalloc*
_mcgen_CheckContextType_microsoft_windows_mimalloc(_In_ McGenContext_microsoft_windows_mimalloc* pContext)
{
    return pContext;
}

#endif // MCGEN_ENABLE_FORCONTEXT_CODE_GENERATION // 2672

//
// Enablement check macro for event "ETW_MI_ALLOC"
//
#define EventEnabledETW_MI_ALLOC() _mcgen_EVENT_BIT_SET(microsoft_windows_mimallocEnableBits, 0)
#define EventEnabledETW_MI_ALLOC_ForContext(pContext) _mcgen_EVENT_BIT_SET(_mcgen_CheckContextType_microsoft_windows_mimalloc(pContext)->EnableBits, 0)

//
// Event write macros for event "ETW_MI_ALLOC"
//
#define EventWriteETW_MI_ALLOC(Address, Size) \
        MCGEN_EVENT_ENABLED(ETW_MI_ALLOC) \
        ? _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC(&ETW_MI_Provider_Context, &ETW_MI_ALLOC, Address, Size) : 0
#define EventWriteETW_MI_ALLOC_AssumeEnabled(Address, Size) \
        _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC(&ETW_MI_Provider_Context, &ETW_MI_ALLOC, Address, Size)
#define EventWriteETW_MI_ALLOC_ForContext(pContext, Address, Size) \
        MCGEN_EVENT_ENABLED_FORCONTEXT(pContext, ETW_MI_ALLOC) \
        ? _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC(&(pContext)->Context, &ETW_MI_ALLOC, Address, Size) : 0
#define EventWriteETW_MI_ALLOC_ForContextAssumeEnabled(pContext, Address, Size) \
        _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC(&_mcgen_CheckContextType_microsoft_windows_mimalloc(pContext)->Context, &ETW_MI_ALLOC, Address, Size)

// This macro is for use by MC-generated code and should not be used directly.
#define _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC _mcgen_PASTE2(McTemplateU0xx_, MCGEN_EVENTWRITETRANSFER)

//
// Enablement check macro for event "ETW_MI_FREE"
//
#define EventEnabledETW_MI_FREE() _mcgen_EVENT_BIT_SET(microsoft_windows_mimallocEnableBits, 0)
#define EventEnabledETW_MI_FREE_ForContext(pContext) _mcgen_EVENT_BIT_SET(_mcgen_CheckContextType_microsoft_windows_mimalloc(pContext)->EnableBits, 0)

//
// Event write macros for event "ETW_MI_FREE"
//
#define EventWriteETW_MI_FREE(Address, Size) \
        MCGEN_EVENT_ENABLED(ETW_MI_FREE) \
        ? _mcgen_TEMPLATE_FOR_ETW_MI_FREE(&ETW_MI_Provider_Context, &ETW_MI_FREE, Address, Size) : 0
#define EventWriteETW_MI_FREE_AssumeEnabled(Address, Size) \
        _mcgen_TEMPLATE_FOR_ETW_MI_FREE(&ETW_MI_Provider_Context, &ETW_MI_FREE, Address, Size)
#define EventWriteETW_MI_FREE_ForContext(pContext, Address, Size) \
        MCGEN_EVENT_ENABLED_FORCONTEXT(pContext, ETW_MI_FREE) \
        ? _mcgen_TEMPLATE_FOR_ETW_MI_FREE(&(pContext)->Context, &ETW_MI_FREE, Address, Size) : 0
#define EventWriteETW_MI_FREE_ForContextAssumeEnabled(pContext, Address, Size) \
        _mcgen_TEMPLATE_FOR_ETW_MI_FREE(&_mcgen_CheckContextType_microsoft_windows_mimalloc(pContext)->Context, &ETW_MI_FREE, Address, Size)

// This macro is for use by MC-generated code and should not be used directly.
#define _mcgen_TEMPLATE_FOR_ETW_MI_FREE _mcgen_PASTE2(McTemplateU0xx_, MCGEN_EVENTWRITETRANSFER)

#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION // 2609

//
// MCGEN_DISABLE_PROVIDER_CODE_GENERATION macro:
// Define this macro to have the compiler skip the generated functions in this
// header.
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION

//
// Template Functions
//

//
// Function for template "ETW_CUSTOM_HEAP_ALLOC_DATA" (and possibly others).
// This function is for use by MC-generated code and should not be used directly.
//
#ifndef McTemplateU0xx_def
#define McTemplateU0xx_def
ETW_INLINE
ULONG
_mcgen_PASTE2(McTemplateU0xx_, MCGEN_EVENTWRITETRANSFER)(
    _In_ PMCGEN_TRACE_CONTEXT Context,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_ const unsigned __int64  _Arg0,
    _In_ const unsigned __int64  _Arg1
    )
{
#define McTemplateU0xx_ARGCOUNT 2

    EVENT_DATA_DESCRIPTOR EventData[McTemplateU0xx_ARGCOUNT + 1];

    EventDataDescCreate(&EventData[1],&_Arg0, sizeof(const unsigned __int64)  );

    EventDataDescCreate(&EventData[2],&_Arg1, sizeof(const unsigned __int64)  );

    return McGenEventWrite(Context, Descriptor, NULL, McTemplateU0xx_ARGCOUNT + 1, EventData);
}
#endif // McTemplateU0xx_def // 2802

#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION // 2792

#if 0 // #if defined(__cplusplus)
// }
#endif // 2827
// --- Amalgamation END --- mimalloc/src/../src/prim/windows/etw.h


#define mi_track_init()                           EventRegistermicrosoft_windows_mimalloc();
#define mi_track_malloc_size(p,reqsize,size,zero) EventWriteETW_MI_ALLOC((UINT64)(p), size)
#define mi_track_free_size(p,size)                EventWriteETW_MI_FREE((UINT64)(p), size)

#else
// no tracking

#define MI_TRACK_ENABLED      0
#define MI_TRACK_HEAP_DESTROY 0
#define MI_TRACK_TOOL         "none"

#define mi_track_malloc_size(p,reqsize,size,zero)
#define mi_track_free_size(p,_size)

#endif // 1885

// -------------------
// Utility definitions

#ifndef mi_track_resize
#define mi_track_resize(p,oldsize,newsize)      mi_track_free_size(p,oldsize); mi_track_malloc(p,newsize,false)
#endif // 2852

#ifndef mi_track_align
#define mi_track_align(p,alignedp,offset,size)  mi_track_mem_noaccess(p,offset)
#endif // 2856

#ifndef mi_track_init
#define mi_track_init()
#endif // 2860

#ifndef mi_track_mem_defined
#define mi_track_mem_defined(p,size)
#endif // 2864

#ifndef mi_track_mem_undefined
#define mi_track_mem_undefined(p,size)
#endif // 2868

#ifndef mi_track_mem_noaccess
#define mi_track_mem_noaccess(p,size)
#endif // 2872


#if MI_PADDING
#define mi_track_malloc(p,reqsize,zero) \
  if ((p)!=NULL) { \
    mi_assert_internal(mi_usable_size(p)==(reqsize)); \
    mi_track_malloc_size(p,reqsize,reqsize,zero); \
  }
#else
#define mi_track_malloc(p,reqsize,zero) \
  if ((p)!=NULL) { \
    mi_assert_internal(mi_usable_size(p)>=(reqsize)); \
    mi_track_malloc_size(p,reqsize,mi_usable_size(p),zero); \
  }
#endif // 2877

// #endif // 1847
// --- Amalgamation END --- mimalloc/include/mimalloc/track.h


#if (MI_DEBUG>0)
#define mi_trace_message(...)  _mi_trace_message(__VA_ARGS__)
#else
#define mi_trace_message(...)
#endif // 2895

#define MI_CACHE_LINE          64
#if 0 // #if defined(_MSC_VER)
// #pragma warning(disable:4127)   // suppress constant conditional warning (due to MI_SECURE paths)
// #pragma warning(disable:26812)  // unscoped enum warning
// #define mi_decl_noinline        __declspec(noinline)
// #define mi_decl_thread          __declspec(thread)
// #define mi_decl_cache_align     __declspec(align(MI_CACHE_LINE))
// #define mi_decl_weak
#elif (defined(__GNUC__) && (__GNUC__ >= 3)) || defined(__clang__) // includes clang and icc
#define mi_decl_noinline        __attribute__((noinline))
#define mi_decl_thread          __thread
#define mi_decl_cache_align     __attribute__((aligned(MI_CACHE_LINE)))
#define mi_decl_weak            __attribute__((weak))
#else
#define mi_decl_noinline
#define mi_decl_thread          __thread        // hope for the best :-)
#define mi_decl_cache_align
#define mi_decl_weak
#endif // 2902

#if defined(__EMSCRIPTEN__) && !defined(__wasi__)
#define __wasi__
#endif // 2921

#if 0 // #if defined(__cplusplus)
// #define mi_decl_externc       extern "C"
#else
#define mi_decl_externc
#endif // 2925


// "options.c"
void       _mi_fputs(mi_output_fun* out, void* arg, const char* prefix, const char* message);
void       _mi_fprintf(mi_output_fun* out, void* arg, const char* fmt, ...);
void       _mi_warning_message(const char* fmt, ...);
void       _mi_verbose_message(const char* fmt, ...);
void       _mi_trace_message(const char* fmt, ...);
void       _mi_options_init(void);
long       _mi_option_get_fast(mi_option_t option);
void       _mi_error_message(int err, const char* fmt, ...);

// random.c
void       _mi_random_init(mi_random_ctx_t* ctx);
void       _mi_random_init_weak(mi_random_ctx_t* ctx);
void       _mi_random_reinit_if_weak(mi_random_ctx_t * ctx);
void       _mi_random_split(mi_random_ctx_t* ctx, mi_random_ctx_t* new_ctx);
uintptr_t  _mi_random_next(mi_random_ctx_t* ctx);
uintptr_t  _mi_heap_random_next(mi_heap_t* heap);
uintptr_t  _mi_os_random_weak(uintptr_t extra_seed);
static inline uintptr_t _mi_random_shuffle(uintptr_t x);

// init.c
extern mi_decl_cache_align mi_stats_t       _mi_stats_main;
extern mi_decl_cache_align const mi_page_t  _mi_page_empty;
void       _mi_process_load(void);
void mi_cdecl _mi_process_done(void);
bool       _mi_is_redirected(void);
bool       _mi_allocator_init(const char** message);
void       _mi_allocator_done(void);
bool       _mi_is_main_thread(void);
size_t     _mi_current_thread_count(void);
bool       _mi_preloading(void);           // true while the C runtime is not initialized yet
void       _mi_thread_done(mi_heap_t* heap);
void       _mi_thread_data_collect(void);
void       _mi_tld_init(mi_tld_t* tld, mi_heap_t* bheap);
mi_threadid_t _mi_thread_id(void) mi_attr_noexcept;
mi_heap_t*    _mi_heap_main_get(void);     // statically allocated main backing heap
mi_subproc_t* _mi_subproc_from_id(mi_subproc_id_t subproc_id);
void       _mi_heap_guarded_init(mi_heap_t* heap);

// os.c
void       _mi_os_init(void);                                            // called from process init
void*      _mi_os_alloc(size_t size, mi_memid_t* memid, mi_stats_t* stats);
void       _mi_os_free(void* p, size_t size, mi_memid_t memid, mi_stats_t* stats);
void       _mi_os_free_ex(void* p, size_t size, bool still_committed, mi_memid_t memid, mi_stats_t* stats);

size_t     _mi_os_page_size(void);
size_t     _mi_os_good_alloc_size(size_t size);
bool       _mi_os_has_overcommit(void);
bool       _mi_os_has_virtual_reserve(void);

bool       _mi_os_reset(void* addr, size_t size, mi_stats_t* tld_stats);
bool       _mi_os_commit(void* p, size_t size, bool* is_zero, mi_stats_t* stats);
bool       _mi_os_decommit(void* addr, size_t size, mi_stats_t* stats);
bool       _mi_os_protect(void* addr, size_t size);
bool       _mi_os_unprotect(void* addr, size_t size);
bool       _mi_os_purge(void* p, size_t size, mi_stats_t* stats);
bool       _mi_os_purge_ex(void* p, size_t size, bool allow_reset, mi_stats_t* stats);

void*      _mi_os_alloc_aligned(size_t size, size_t alignment, bool commit, bool allow_large, mi_memid_t* memid, mi_stats_t* stats);
void*      _mi_os_alloc_aligned_at_offset(size_t size, size_t alignment, size_t align_offset, bool commit, bool allow_large, mi_memid_t* memid, mi_stats_t* tld_stats);

void*      _mi_os_get_aligned_hint(size_t try_alignment, size_t size);
bool       _mi_os_use_large_page(size_t size, size_t alignment);
size_t     _mi_os_large_page_size(void);

void*      _mi_os_alloc_huge_os_pages(size_t pages, int numa_node, mi_msecs_t max_secs, size_t* pages_reserved, size_t* psize, mi_memid_t* memid);

// arena.c
mi_arena_id_t _mi_arena_id_none(void);
void       _mi_arena_free(void* p, size_t size, size_t still_committed_size, mi_memid_t memid, mi_stats_t* stats);
void*      _mi_arena_alloc(size_t size, bool commit, bool allow_large, mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld);
void*      _mi_arena_alloc_aligned(size_t size, size_t alignment, size_t align_offset, bool commit, bool allow_large, mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld);
bool       _mi_arena_memid_is_suitable(mi_memid_t memid, mi_arena_id_t request_arena_id);
bool       _mi_arena_contains(const void* p);
void       _mi_arenas_collect(bool force_purge, mi_stats_t* stats);
void       _mi_arena_unsafe_destroy_all(mi_stats_t* stats);

bool       _mi_arena_segment_clear_abandoned(mi_segment_t* segment);
void       _mi_arena_segment_mark_abandoned(mi_segment_t* segment);

void*      _mi_arena_meta_zalloc(size_t size, mi_memid_t* memid);
void       _mi_arena_meta_free(void* p, mi_memid_t memid, size_t size);

typedef struct mi_arena_field_cursor_s { // abstract struct
  size_t         os_list_count;           // max entries to visit in the OS abandoned list
  size_t         start;                   // start arena idx (may need to be wrapped)
  size_t         end;                     // end arena idx (exclusive, may need to be wrapped)
  size_t         bitmap_idx;              // current bit idx for an arena
  mi_subproc_t*  subproc;                 // only visit blocks in this sub-process
  bool           visit_all;               // ensure all abandoned blocks are seen (blocking)
  bool           hold_visit_lock;         // if the subproc->abandoned_os_visit_lock is held
} mi_arena_field_cursor_t;
void          _mi_arena_field_cursor_init(mi_heap_t* heap, mi_subproc_t* subproc, bool visit_all, mi_arena_field_cursor_t* current);
mi_segment_t* _mi_arena_segment_clear_abandoned_next(mi_arena_field_cursor_t* previous);
void          _mi_arena_field_cursor_done(mi_arena_field_cursor_t* current);

// "segment-map.c"
void       _mi_segment_map_allocated_at(const mi_segment_t* segment);
void       _mi_segment_map_freed_at(const mi_segment_t* segment);

// "segment.c"
mi_page_t* _mi_segment_page_alloc(mi_heap_t* heap, size_t block_size, size_t page_alignment, mi_segments_tld_t* tld, mi_os_tld_t* os_tld);
void       _mi_segment_page_free(mi_page_t* page, bool force, mi_segments_tld_t* tld);
void       _mi_segment_page_abandon(mi_page_t* page, mi_segments_tld_t* tld);
uint8_t*   _mi_segment_page_start(const mi_segment_t* segment, const mi_page_t* page, size_t* page_size);

#if MI_HUGE_PAGE_ABANDON
void       _mi_segment_huge_page_free(mi_segment_t* segment, mi_page_t* page, mi_block_t* block);
#else
void       _mi_segment_huge_page_reset(mi_segment_t* segment, mi_page_t* page, mi_block_t* block);
#endif // 3038

void       _mi_segments_collect(bool force, mi_segments_tld_t* tld);
void       _mi_abandoned_reclaim_all(mi_heap_t* heap, mi_segments_tld_t* tld);
bool       _mi_segment_attempt_reclaim(mi_heap_t* heap, mi_segment_t* segment);
bool       _mi_segment_visit_blocks(mi_segment_t* segment, int heap_tag, bool visit_blocks, mi_block_visit_fun* visitor, void* arg);

// "page.c"
void*      _mi_malloc_generic(mi_heap_t* heap, size_t size, bool zero, size_t huge_alignment)  mi_attr_noexcept mi_attr_malloc;

void       _mi_page_retire(mi_page_t* page) mi_attr_noexcept;                  // free the page if there are no other pages with many free blocks
void       _mi_page_unfull(mi_page_t* page);
void       _mi_page_free(mi_page_t* page, mi_page_queue_t* pq, bool force);   // free the page
void       _mi_page_abandon(mi_page_t* page, mi_page_queue_t* pq);            // abandon the page, to be picked up by another thread...
void       _mi_page_force_abandon(mi_page_t* page);

void       _mi_heap_delayed_free_all(mi_heap_t* heap);
bool       _mi_heap_delayed_free_partial(mi_heap_t* heap);
void       _mi_heap_collect_retired(mi_heap_t* heap, bool force);

void       _mi_page_use_delayed_free(mi_page_t* page, mi_delayed_t delay, bool override_never);
bool       _mi_page_try_use_delayed_free(mi_page_t* page, mi_delayed_t delay, bool override_never);
size_t     _mi_page_queue_append(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_queue_t* append);
void       _mi_deferred_free(mi_heap_t* heap, bool force);

void       _mi_page_free_collect(mi_page_t* page,bool force);
void       _mi_page_reclaim(mi_heap_t* heap, mi_page_t* page);   // callback from segments

size_t     _mi_bin_size(uint8_t bin);           // for stats
uint8_t    _mi_bin(size_t size);                // for stats

// "heap.c"
void       _mi_heap_init(mi_heap_t* heap, mi_tld_t* tld, mi_arena_id_t arena_id, bool noreclaim, uint8_t tag);
void       _mi_heap_destroy_pages(mi_heap_t* heap);
void       _mi_heap_collect_abandon(mi_heap_t* heap);
void       _mi_heap_set_default_direct(mi_heap_t* heap);
bool       _mi_heap_memid_is_suitable(mi_heap_t* heap, mi_memid_t memid);
void       _mi_heap_unsafe_destroy_all(void);
mi_heap_t* _mi_heap_by_tag(mi_heap_t* heap, uint8_t tag);
void       _mi_heap_area_init(mi_heap_area_t* area, mi_page_t* page);
bool       _mi_heap_area_visit_blocks(const mi_heap_area_t* area, mi_page_t* page, mi_block_visit_fun* visitor, void* arg);

// "stats.c"
void       _mi_stats_done(mi_stats_t* stats);
mi_msecs_t  _mi_clock_now(void);
mi_msecs_t  _mi_clock_end(mi_msecs_t start);
mi_msecs_t  _mi_clock_start(void);

// "alloc.c"
void*       _mi_page_malloc_zero(mi_heap_t* heap, mi_page_t* page, size_t size, bool zero) mi_attr_noexcept;  // called from `_mi_malloc_generic`
void*       _mi_page_malloc(mi_heap_t* heap, mi_page_t* page, size_t size) mi_attr_noexcept;                  // called from `_mi_heap_malloc_aligned`
void*       _mi_page_malloc_zeroed(mi_heap_t* heap, mi_page_t* page, size_t size) mi_attr_noexcept;           // called from `_mi_heap_malloc_aligned`
void*       _mi_heap_malloc_zero(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept;
void*       _mi_heap_malloc_zero_ex(mi_heap_t* heap, size_t size, bool zero, size_t huge_alignment) mi_attr_noexcept;     // called from `_mi_heap_malloc_aligned`
void*       _mi_heap_realloc_zero(mi_heap_t* heap, void* p, size_t newsize, bool zero) mi_attr_noexcept;
mi_block_t* _mi_page_ptr_unalign(const mi_page_t* page, const void* p);
bool        _mi_free_delayed_block(mi_block_t* block);
void        _mi_free_generic(mi_segment_t* segment, mi_page_t* page, bool is_local, void* p) mi_attr_noexcept;  // for runtime integration
void        _mi_padding_shrink(const mi_page_t* page, const mi_block_t* block, const size_t min_size);

// "libc.c"
#include    <stdarg.h>
void        _mi_vsnprintf(char* buf, size_t bufsize, const char* fmt, va_list args);
void        _mi_snprintf(char* buf, size_t buflen, const char* fmt, ...);
char        _mi_toupper(char c);
int         _mi_strnicmp(const char* s, const char* t, size_t n);
void        _mi_strlcpy(char* dest, const char* src, size_t dest_size);
void        _mi_strlcat(char* dest, const char* src, size_t dest_size);
size_t      _mi_strlen(const char* s);
size_t      _mi_strnlen(const char* s, size_t max_len);
bool        _mi_getenv(const char* name, char* result, size_t result_size);

#if MI_DEBUG>1
bool        _mi_page_is_valid(mi_page_t* page);
#endif // 3114


// ------------------------------------------------------
// Branches
// ------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)
#define mi_unlikely(x)     (__builtin_expect(!!(x),false))
#define mi_likely(x)       (__builtin_expect(!!(x),true))
#elif 0 // #elif (defined(__cplusplus) && (__cplusplus >= 202002L)) || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
// #define mi_unlikely(x)     (x) [[unlikely]]
// #define mi_likely(x)       (x) [[likely]]
#else
#define mi_unlikely(x)     (x)
#define mi_likely(x)       (x)
#endif // 3123

#ifndef __has_builtin
#define __has_builtin(x)  0
#endif // 3134


/* -----------------------------------------------------------
  Error codes passed to `_mi_fatal_error`
  All are recoverable but EFAULT is a serious error and aborts by default in secure mode.
  For portability define undefined error codes using common Unix codes:
  <https://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html>
----------------------------------------------------------- */
#include <errno.h>
#ifndef EAGAIN         // double free
#define EAGAIN (11)
#endif // 3146
#ifndef ENOMEM         // out of memory
#define ENOMEM (12)
#endif // 3149
#ifndef EFAULT         // corrupted free-list or meta-data
#define EFAULT (14)
#endif // 3152
#ifndef EINVAL         // trying to free an invalid pointer
#define EINVAL (22)
#endif // 3155
#ifndef EOVERFLOW      // count*size overflow
#define EOVERFLOW (75)
#endif // 3158


/* -----------------------------------------------------------
  Inlined definitions
----------------------------------------------------------- */
#define MI_UNUSED(x)     (void)(x)
#if (MI_DEBUG>0)
#define MI_UNUSED_RELEASE(x)
#else
#define MI_UNUSED_RELEASE(x)  MI_UNUSED(x)
#endif // 3167

#define MI_INIT4(x)   x(),x(),x(),x()
#define MI_INIT8(x)   MI_INIT4(x),MI_INIT4(x)
#define MI_INIT16(x)  MI_INIT8(x),MI_INIT8(x)
#define MI_INIT32(x)  MI_INIT16(x),MI_INIT16(x)
#define MI_INIT64(x)  MI_INIT32(x),MI_INIT32(x)
#define MI_INIT128(x) MI_INIT64(x),MI_INIT64(x)
#define MI_INIT256(x) MI_INIT128(x),MI_INIT128(x)


#include <string.h>
// initialize a local variable to zero; use memset as compilers optimize constant sized memset's
#define _mi_memzero_var(x)  memset(&x,0,sizeof(x))

// Is `x` a power of two? (0 is considered a power of two)
static inline bool _mi_is_power_of_two(uintptr_t x) {
  return ((x & (x - 1)) == 0);
}

// Is a pointer aligned?
static inline bool _mi_is_aligned(void* p, size_t alignment) {
  mi_assert_internal(alignment != 0);
  return (((uintptr_t)p % alignment) == 0);
}

// Align upwards
static inline uintptr_t _mi_align_up(uintptr_t sz, size_t alignment) {
  mi_assert_internal(alignment != 0);
  uintptr_t mask = alignment - 1;
  if ((alignment & mask) == 0) {  // power of two?
    return ((sz + mask) & ~mask);
  }
  else {
    return (((sz + mask)/alignment)*alignment);
  }
}


// Align a pointer upwards
static inline void* mi_align_up_ptr(void* p, size_t alignment) {
  return (void*)_mi_align_up((uintptr_t)p, alignment);
}


// Divide upwards: `s <= _mi_divide_up(s,d)*d < s+d`.
static inline uintptr_t _mi_divide_up(uintptr_t size, size_t divider) {
  mi_assert_internal(divider != 0);
  return (divider == 0 ? size : ((size + divider - 1) / divider));
}


// clamp an integer
static inline size_t _mi_clamp(size_t sz, size_t min, size_t max) {
  if (sz < min) return min;
  else if (sz > max) return max;
  else return sz;
}

// Is memory zero initialized?
static inline bool mi_mem_is_zero(const void* p, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (((uint8_t*)p)[i] != 0) return false;
  }
  return true;
}

// Align a byte size to a size in _machine words_,
// i.e. byte size == `wsize*sizeof(void*)`.
static inline size_t _mi_wsize_from_size(size_t size) {
  mi_assert_internal(size <= SIZE_MAX - sizeof(uintptr_t));
  return (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
}

// Overflow detecting multiply
#if __has_builtin(__builtin_umul_overflow) || (defined(__GNUC__) && (__GNUC__ >= 5))
#include <limits.h>      // UINT_MAX, ULONG_MAX
#if defined(_CLOCK_T)    // for Illumos
#undef _CLOCK_T
#endif // 3248
static inline bool mi_mul_overflow(size_t count, size_t size, size_t* total) {
  #if (SIZE_MAX == ULONG_MAX)
    return __builtin_umull_overflow(count, size, (unsigned long *)total);
  #elif (SIZE_MAX == UINT_MAX)
    return __builtin_umul_overflow(count, size, (unsigned int *)total);
  #else
    return __builtin_umulll_overflow(count, size, (unsigned long long *)total);
  #endif // 3252
}
#else /* __builtin_umul_overflow is unavailable */
static inline bool mi_mul_overflow(size_t count, size_t size, size_t* total) {
  #define MI_MUL_COULD_OVERFLOW ((size_t)1 << (4*sizeof(size_t)))  // sqrt(SIZE_MAX)
  *total = count * size;
  // note: gcc/clang optimize this to directly check the overflow flag
  return ((size >= MI_MUL_COULD_OVERFLOW || count >= MI_MUL_COULD_OVERFLOW) && size > 0 && (SIZE_MAX / size) < count);
}
#endif // 3246

// Safe multiply `count*size` into `total`; return `true` on overflow.
static inline bool mi_count_size_overflow(size_t count, size_t size, size_t* total) {
  if (count==1) {  // quick check for the case where count is one (common for C++ allocators)
    *total = size;
    return false;
  }
  else if mi_unlikely(mi_mul_overflow(count, size, total)) {
    #if MI_DEBUG > 0
    _mi_error_message(EOVERFLOW, "allocation request is too large (%zu * %zu bytes)\n", count, size);
    #endif // 3276
    *total = SIZE_MAX;
    return true;
  }
  else return false;
}


/*----------------------------------------------------------------------------------------
  Heap functions
------------------------------------------------------------------------------------------- */

extern const mi_heap_t _mi_heap_empty;  // read-only empty heap, initial value of the thread local default heap

static inline bool mi_heap_is_backing(const mi_heap_t* heap) {
  return (heap->tld->heap_backing == heap);
}

static inline bool mi_heap_is_initialized(mi_heap_t* heap) {
  mi_assert_internal(heap != NULL);
  return (heap != &_mi_heap_empty);
}

extern mi_heap_t _mi_heap_main;
static inline uintptr_t _mi_ptr_cookie(const void* p) {
  mi_assert_internal(_mi_heap_main.cookie != 0);
  return ((uintptr_t)p ^ _mi_heap_main.cookie);
}

/* -----------------------------------------------------------
  Pages
----------------------------------------------------------- */

static inline mi_page_t* _mi_heap_get_free_small_page(mi_heap_t* heap, size_t size) {
  mi_assert_internal(size <= (MI_SMALL_SIZE_MAX + MI_PADDING_SIZE));
  const size_t idx = _mi_wsize_from_size(size);
  mi_assert_internal(idx < MI_PAGES_DIRECT);
  return heap->pages_free_direct[idx];
}

// Segment that contains the pointer
// Large aligned blocks may be aligned at N*MI_SEGMENT_SIZE (inside a huge segment > MI_SEGMENT_SIZE),
// and we need align "down" to the segment info which is `MI_SEGMENT_SIZE` bytes before it;
// therefore we align one byte before `p`.
// We check for NULL afterwards on 64-bit systems to improve codegen for `mi_free`.
static inline mi_segment_t* _mi_ptr_segment(const void* p) {
  mi_segment_t* const segment = (mi_segment_t*)(((uintptr_t)p - 1) & ~MI_SEGMENT_MASK);
  #if MI_INTPTR_SIZE <= 4
  return (p==NULL ? NULL : segment);
  #else
  return ((intptr_t)segment <= 0 ? NULL : segment);
  #endif // 3325
}

// Segment belonging to a page
static inline mi_segment_t* _mi_page_segment(const mi_page_t* page) {
  mi_assert_internal(page!=NULL);
  mi_segment_t* segment = _mi_ptr_segment(page);
  mi_assert_internal(segment == NULL || page == &segment->pages[page->segment_idx]);
  return segment;
}

// used internally
static inline size_t _mi_segment_page_idx_of(const mi_segment_t* segment, const void* p) {
  // if (segment->page_size > MI_SEGMENT_SIZE) return &segment->pages[0];  // huge pages
  ptrdiff_t diff = (uint8_t*)p - (uint8_t*)segment;
  mi_assert_internal(diff >= 0 && (size_t)diff <= MI_SEGMENT_SIZE /* for huge alignment it can be equal */);
  size_t idx = (size_t)diff >> segment->page_shift;
  mi_assert_internal(idx < segment->capacity);
  mi_assert_internal(segment->page_kind <= MI_PAGE_MEDIUM || idx == 0);
  return idx;
}

// Get the page containing the pointer
static inline mi_page_t* _mi_segment_page_of(const mi_segment_t* segment, const void* p) {
  size_t idx = _mi_segment_page_idx_of(segment, p);
  return &((mi_segment_t*)segment)->pages[idx];
}

// Quick page start for initialized pages
static inline uint8_t* mi_page_start(const mi_page_t* page) {
  mi_assert_internal(page->page_start != NULL);
  mi_assert_expensive(_mi_segment_page_start(_mi_page_segment(page),page,NULL) == page->page_start);
  return page->page_start;
}

// Get the page containing the pointer
static inline mi_page_t* _mi_ptr_page(void* p) {
  mi_assert_internal(p!=NULL);
  return _mi_segment_page_of(_mi_ptr_segment(p), p);
}

// Get the block size of a page (special case for huge objects)
static inline size_t mi_page_block_size(const mi_page_t* page) {
  mi_assert_internal(page->block_size > 0);
  return page->block_size;
}

static inline bool mi_page_is_huge(const mi_page_t* page) {
  mi_assert_internal((page->is_huge && _mi_page_segment(page)->page_kind == MI_PAGE_HUGE) ||
                     (!page->is_huge && _mi_page_segment(page)->page_kind != MI_PAGE_HUGE));
  return page->is_huge;
}

// Get the usable block size of a page without fixed padding.
// This may still include internal padding due to alignment and rounding up size classes.
static inline size_t mi_page_usable_block_size(const mi_page_t* page) {
  return mi_page_block_size(page) - MI_PADDING_SIZE;
}

// size of a segment
static inline size_t mi_segment_size(mi_segment_t* segment) {
  return segment->segment_size;
}

// Thread free access
static inline mi_block_t* mi_page_thread_free(const mi_page_t* page) {
  return (mi_block_t*)(mi_atomic_load_relaxed(&((mi_page_t*)page)->xthread_free) & ~3);
}

static inline mi_delayed_t mi_page_thread_free_flag(const mi_page_t* page) {
  return (mi_delayed_t)(mi_atomic_load_relaxed(&((mi_page_t*)page)->xthread_free) & 3);
}

// Heap access
static inline mi_heap_t* mi_page_heap(const mi_page_t* page) {
  return (mi_heap_t*)(mi_atomic_load_relaxed(&((mi_page_t*)page)->xheap));
}

static inline void mi_page_set_heap(mi_page_t* page, mi_heap_t* heap) {
  mi_assert_internal(mi_page_thread_free_flag(page) != MI_DELAYED_FREEING);
  mi_atomic_store_release(&page->xheap,(uintptr_t)heap);
  if (heap != NULL) { page->heap_tag = heap->tag; }
}

// Thread free flag helpers
static inline mi_block_t* mi_tf_block(mi_thread_free_t tf) {
  return (mi_block_t*)(tf & ~0x03);
}
static inline mi_delayed_t mi_tf_delayed(mi_thread_free_t tf) {
  return (mi_delayed_t)(tf & 0x03);
}
static inline mi_thread_free_t mi_tf_make(mi_block_t* block, mi_delayed_t delayed) {
  return (mi_thread_free_t)((uintptr_t)block | (uintptr_t)delayed);
}
static inline mi_thread_free_t mi_tf_set_delayed(mi_thread_free_t tf, mi_delayed_t delayed) {
  return mi_tf_make(mi_tf_block(tf),delayed);
}
static inline mi_thread_free_t mi_tf_set_block(mi_thread_free_t tf, mi_block_t* block) {
  return mi_tf_make(block, mi_tf_delayed(tf));
}

// are all blocks in a page freed?
// note: needs up-to-date used count, (as the `xthread_free` list may not be empty). see `_mi_page_collect_free`.
static inline bool mi_page_all_free(const mi_page_t* page) {
  mi_assert_internal(page != NULL);
  return (page->used == 0);
}

// are there any available blocks?
static inline bool mi_page_has_any_available(const mi_page_t* page) {
  mi_assert_internal(page != NULL && page->reserved > 0);
  return (page->used < page->reserved || (mi_page_thread_free(page) != NULL));
}

// are there immediately available blocks, i.e. blocks available on the free list.
static inline bool mi_page_immediate_available(const mi_page_t* page) {
  mi_assert_internal(page != NULL);
  return (page->free != NULL);
}

// is more than 7/8th of a page in use?
static inline bool mi_page_mostly_used(const mi_page_t* page) {
  if (page==NULL) return true;
  uint16_t frac = page->reserved / 8U;
  return (page->reserved - page->used <= frac);
}

static inline mi_page_queue_t* mi_page_queue(const mi_heap_t* heap, size_t size) {
  return &((mi_heap_t*)heap)->pages[_mi_bin(size)];
}



//-----------------------------------------------------------
// Page flags
//-----------------------------------------------------------
static inline bool mi_page_is_in_full(const mi_page_t* page) {
  return page->flags.x.in_full;
}

static inline void mi_page_set_in_full(mi_page_t* page, bool in_full) {
  page->flags.x.in_full = in_full;
}

static inline bool mi_page_has_aligned(const mi_page_t* page) {
  return page->flags.x.has_aligned;
}

static inline void mi_page_set_has_aligned(mi_page_t* page, bool has_aligned) {
  page->flags.x.has_aligned = has_aligned;
}

/* -------------------------------------------------------------------
  Guarded objects
------------------------------------------------------------------- */
#if MI_GUARDED
static inline bool mi_block_ptr_is_guarded(const mi_block_t* block, const void* p) {
  const ptrdiff_t offset = (uint8_t*)p - (uint8_t*)block;
  return (offset >= (ptrdiff_t)(sizeof(mi_block_t)) && block->next == MI_BLOCK_TAG_GUARDED);
}

static inline bool mi_heap_malloc_use_guarded(mi_heap_t* heap, size_t size) {
  // this code is written to result in fast assembly as it is on the hot path for allocation
  const size_t count = heap->guarded_sample_count - 1;  // if the rate was 0, this will underflow and count for a long time..
  if mi_likely(count != 0) {
    // no sample
    heap->guarded_sample_count = count;
    return false;
  }
  else if (size >= heap->guarded_size_min && size <= heap->guarded_size_max) {
    // use guarded allocation
    heap->guarded_sample_count = heap->guarded_sample_rate;  // reset
    return (heap->guarded_sample_rate != 0);
  }
  else {
    // failed size criteria, rewind count (but don't write to an empty heap)
    if (heap->guarded_sample_rate != 0) { heap->guarded_sample_count = 1; }
    return false;
  }
}

mi_decl_restrict void* _mi_heap_malloc_guarded(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept;

#endif // 3484


/* -------------------------------------------------------------------
Encoding/Decoding the free list next pointers

This is to protect against buffer overflow exploits where the
free list is mutated. Many hardened allocators xor the next pointer `p`
with a secret key `k1`, as `p^k1`. This prevents overwriting with known
values but might be still too weak: if the attacker can guess
the pointer `p` this  can reveal `k1` (since `p^k1^p == k1`).
Moreover, if multiple blocks can be read as well, the attacker can
xor both as `(p1^k1) ^ (p2^k1) == p1^p2` which may reveal a lot
about the pointers (and subsequently `k1`).

Instead mimalloc uses an extra key `k2` and encodes as `((p^k2)<<<k1)+k1`.
Since these operations are not associative, the above approaches do not
work so well any more even if the `p` can be guesstimated. For example,
for the read case we can subtract two entries to discard the `+k1` term,
but that leads to `((p1^k2)<<<k1) - ((p2^k2)<<<k1)` at best.
We include the left-rotation since xor and addition are otherwise linear
in the lowest bit. Finally, both keys are unique per page which reduces
the re-use of keys by a large factor.

We also pass a separate `null` value to be used as `NULL` or otherwise
`(k2<<<k1)+k1` would appear (too) often as a sentinel value.
------------------------------------------------------------------- */

static inline bool mi_is_in_same_segment(const void* p, const void* q) {
  return (_mi_ptr_segment(p) == _mi_ptr_segment(q));
}

static inline bool mi_is_in_same_page(const void* p, const void* q) {
  mi_segment_t* segmentp = _mi_ptr_segment(p);
  mi_segment_t* segmentq = _mi_ptr_segment(q);
  if (segmentp != segmentq) return false;
  size_t idxp = _mi_segment_page_idx_of(segmentp, p);
  size_t idxq = _mi_segment_page_idx_of(segmentq, q);
  return (idxp == idxq);
}

static inline uintptr_t mi_rotl(uintptr_t x, uintptr_t shift) {
  shift %= MI_INTPTR_BITS;
  return (shift==0 ? x : ((x << shift) | (x >> (MI_INTPTR_BITS - shift))));
}
static inline uintptr_t mi_rotr(uintptr_t x, uintptr_t shift) {
  shift %= MI_INTPTR_BITS;
  return (shift==0 ? x : ((x >> shift) | (x << (MI_INTPTR_BITS - shift))));
}

static inline void* mi_ptr_decode(const void* null, const mi_encoded_t x, const uintptr_t* keys) {
  void* p = (void*)(mi_rotr(x - keys[0], keys[0]) ^ keys[1]);
  return (p==null ? NULL : p);
}

static inline mi_encoded_t mi_ptr_encode(const void* null, const void* p, const uintptr_t* keys) {
  uintptr_t x = (uintptr_t)(p==NULL ? null : p);
  return mi_rotl(x ^ keys[1], keys[0]) + keys[0];
}

static inline uint32_t mi_ptr_encode_canary(const void* null, const void* p, const uintptr_t* keys) {
  const uint32_t x = (uint32_t)(mi_ptr_encode(null,p,keys));
  // make the lowest byte 0 to prevent spurious read overflows which could be a security issue (issue #951)
  #ifdef MI_BIG_ENDIAN
  return (x & 0x00FFFFFF);
  #else
  return (x & 0xFFFFFF00);
  #endif // 3575
}

static inline mi_block_t* mi_block_nextx( const void* null, const mi_block_t* block, const uintptr_t* keys ) {
  mi_track_mem_defined(block,sizeof(mi_block_t));
  mi_block_t* next;
  #ifdef MI_ENCODE_FREELIST
  next = (mi_block_t*)mi_ptr_decode(null, block->next, keys);
  #else
  MI_UNUSED(keys); MI_UNUSED(null);
  next = (mi_block_t*)block->next;
  #endif // 3585
  mi_track_mem_noaccess(block,sizeof(mi_block_t));
  return next;
}

static inline void mi_block_set_nextx(const void* null, mi_block_t* block, const mi_block_t* next, const uintptr_t* keys) {
  mi_track_mem_undefined(block,sizeof(mi_block_t));
  #ifdef MI_ENCODE_FREELIST
  block->next = mi_ptr_encode(null, next, keys);
  #else
  MI_UNUSED(keys); MI_UNUSED(null);
  block->next = (mi_encoded_t)next;
  #endif // 3597
  mi_track_mem_noaccess(block,sizeof(mi_block_t));
}

static inline mi_block_t* mi_block_next(const mi_page_t* page, const mi_block_t* block) {
  #ifdef MI_ENCODE_FREELIST
  mi_block_t* next = mi_block_nextx(page,block,page->keys);
  // check for free list corruption: is `next` at least in the same page?
  // TODO: check if `next` is `page->block_size` aligned?
  if mi_unlikely(next!=NULL && !mi_is_in_same_page(block, next)) {
    _mi_error_message(EFAULT, "corrupted free list entry of size %zub at %p: value 0x%zx\n", mi_page_block_size(page), block, (uintptr_t)next);
    next = NULL;
  }
  return next;
  #else
  MI_UNUSED(page);
  return mi_block_nextx(page,block,NULL);
  #endif // 3607
}

static inline void mi_block_set_next(const mi_page_t* page, mi_block_t* block, const mi_block_t* next) {
  #ifdef MI_ENCODE_FREELIST
  mi_block_set_nextx(page,block,next, page->keys);
  #else
  MI_UNUSED(page);
  mi_block_set_nextx(page,block,next,NULL);
  #endif // 3623
}


/* -----------------------------------------------------------
  memory id's
----------------------------------------------------------- */

static inline mi_memid_t _mi_memid_create(mi_memkind_t memkind) {
  mi_memid_t memid;
  _mi_memzero_var(memid);
  memid.memkind = memkind;
  return memid;
}

static inline mi_memid_t _mi_memid_none(void) {
  return _mi_memid_create(MI_MEM_NONE);
}

static inline mi_memid_t _mi_memid_create_os(bool committed, bool is_zero, bool is_large) {
  mi_memid_t memid = _mi_memid_create(MI_MEM_OS);
  memid.initially_committed = committed;
  memid.initially_zero = is_zero;
  memid.is_pinned = is_large;
  return memid;
}


// -------------------------------------------------------------------
// Fast "random" shuffle
// -------------------------------------------------------------------

static inline uintptr_t _mi_random_shuffle(uintptr_t x) {
  if (x==0) { x = 17; }   // ensure we don't get stuck in generating zeros
#if (MI_INTPTR_SIZE>=8)
  // by Sebastiano Vigna, see: <http://xoshiro.di.unimi.it/splitmix64.c>
  x ^= x >> 30;
  x *= 0xbf58476d1ce4e5b9UL;
  x ^= x >> 27;
  x *= 0x94d049bb133111ebUL;
  x ^= x >> 31;
#elif (MI_INTPTR_SIZE==4)
  // by Chris Wellons, see: <https://nullprogram.com/blog/2018/07/31/>
  x ^= x >> 16;
  x *= 0x7feb352dUL;
  x ^= x >> 15;
  x *= 0x846ca68bUL;
  x ^= x >> 16;
#endif // 3662
  return x;
}

// -------------------------------------------------------------------
// Optimize numa node access for the common case (= one node)
// -------------------------------------------------------------------

int    _mi_os_numa_node_get(mi_os_tld_t* tld);
size_t _mi_os_numa_node_count_get(void);

extern _Atomic(size_t) _mi_numa_node_count;
static inline int _mi_os_numa_node(mi_os_tld_t* tld) {
  if mi_likely(mi_atomic_load_relaxed(&_mi_numa_node_count) == 1) { return 0; }
  else return _mi_os_numa_node_get(tld);
}
static inline size_t _mi_os_numa_node_count(void) {
  const size_t count = mi_atomic_load_relaxed(&_mi_numa_node_count);
  if mi_likely(count > 0) { return count; }
  else return _mi_os_numa_node_count_get();
}



// -----------------------------------------------------------------------
// Count bits: trailing or leading zeros (with MI_INTPTR_BITS on all zero)
// -----------------------------------------------------------------------

#if defined(__GNUC__)

#include <limits.h>       // LONG_MAX
#define MI_HAVE_FAST_BITSCAN
static inline size_t mi_clz(uintptr_t x) {
  if (x==0) return MI_INTPTR_BITS;
#if (INTPTR_MAX == LONG_MAX)
  return __builtin_clzl(x);
#else
  return __builtin_clzll(x);
#endif // 3710
}
static inline size_t mi_ctz(uintptr_t x) {
  if (x==0) return MI_INTPTR_BITS;
#if (INTPTR_MAX == LONG_MAX)
  return __builtin_ctzl(x);
#else
  return __builtin_ctzll(x);
#endif // 3718
}

#elif 0 // #elif defined(_MSC_VER)
// 
// #include <limits.h>       // LONG_MAX
// #include <intrin.h>       // BitScanReverse64
// #define MI_HAVE_FAST_BITSCAN
// static inline size_t mi_clz(uintptr_t x) {
//   if (x==0) return MI_INTPTR_BITS;
//   unsigned long idx;
#if 0 // #if (INTPTR_MAX == LONG_MAX)
//   _BitScanReverse(&idx, x);
#else
//   _BitScanReverse64(&idx, x);
#endif // 3733
//   return ((MI_INTPTR_BITS - 1) - idx);
// }
// static inline size_t mi_ctz(uintptr_t x) {
//   if (x==0) return MI_INTPTR_BITS;
//   unsigned long idx;
#if 0 // #if (INTPTR_MAX == LONG_MAX)
//   _BitScanForward(&idx, x);
#else
//   _BitScanForward64(&idx, x);
#endif // 3743
//   return idx;
// }
// 
#else
static inline size_t mi_ctz32(uint32_t x) {
  // de Bruijn multiplication, see <http://supertech.csail.mit.edu/papers/debruijn.pdf>
  static const unsigned char debruijn[32] = {
    0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
  };
  if (x==0) return 32;
  return debruijn[((x & -(int32_t)x) * 0x077CB531UL) >> 27];
}
static inline size_t mi_clz32(uint32_t x) {
  // de Bruijn multiplication, see <http://supertech.csail.mit.edu/papers/debruijn.pdf>
  static const uint8_t debruijn[32] = {
    31, 22, 30, 21, 18, 10, 29, 2, 20, 17, 15, 13, 9, 6, 28, 1,
    23, 19, 11, 3, 16, 14, 7, 24, 12, 4, 8, 25, 5, 26, 27, 0
  };
  if (x==0) return 32;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return debruijn[(uint32_t)(x * 0x07C4ACDDUL) >> 27];
}

static inline size_t mi_clz(uintptr_t x) {
  if (x==0) return MI_INTPTR_BITS;
#if (MI_INTPTR_BITS <= 32)
  return mi_clz32((uint32_t)x);
#else
  size_t count = mi_clz32((uint32_t)(x >> 32));
  if (count < 32) return count;
  return (32 + mi_clz32((uint32_t)x));
#endif // 3778
}
static inline size_t mi_ctz(uintptr_t x) {
  if (x==0) return MI_INTPTR_BITS;
#if (MI_INTPTR_BITS <= 32)
  return mi_ctz32((uint32_t)x);
#else
  size_t count = mi_ctz32((uint32_t)x);
  if (count < 32) return count;
  return (32 + mi_ctz32((uint32_t)(x>>32)));
#endif // 3788
}

#endif // 3704

// "bit scan reverse": Return index of the highest bit (or MI_INTPTR_BITS if `x` is zero)
static inline size_t mi_bsr(uintptr_t x) {
  return (x==0 ? MI_INTPTR_BITS : MI_INTPTR_BITS - 1 - mi_clz(x));
}


// ---------------------------------------------------------------------------------
// Provide our own `_mi_memcpy` for potential performance optimizations.
//
// For now, only on Windows with msvc/clang-cl we optimize to `rep movsb` if
// we happen to run on x86/x64 cpu's that have "fast short rep movsb" (FSRM) support
// (AMD Zen3+ (~2020) or Intel Ice Lake+ (~2017). See also issue #201 and pr #253.
// ---------------------------------------------------------------------------------

#if !MI_TRACK_ENABLED && defined(_WIN32) && (defined(_M_IX86) || defined(_M_X64))
#include <intrin.h>
extern bool _mi_cpu_has_fsrm;
extern bool _mi_cpu_has_erms;
static inline void _mi_memcpy(void* dst, const void* src, size_t n) {
  if ((_mi_cpu_has_fsrm && n <= 128) || (_mi_cpu_has_erms && n > 128)) {
    __movsb((unsigned char*)dst, (const unsigned char*)src, n);
  }
  else {
    memcpy(dst, src, n);
  }
}
static inline void _mi_memzero(void* dst, size_t n) {
  if ((_mi_cpu_has_fsrm && n <= 128) || (_mi_cpu_has_erms && n > 128)) {
    __stosb((unsigned char*)dst, 0, n);
  }
  else {
    memset(dst, 0, n);
  }
}
#else
static inline void _mi_memcpy(void* dst, const void* src, size_t n) {
  memcpy(dst, src, n);
}
static inline void _mi_memzero(void* dst, size_t n) {
  memset(dst, 0, n);
}
#endif // 3813

// -------------------------------------------------------------------------------
// The `_mi_memcpy_aligned` can be used if the pointers are machine-word aligned
// This is used for example in `mi_realloc`.
// -------------------------------------------------------------------------------

#if (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__)
// On GCC/CLang we provide a hint that the pointers are word aligned.
static inline void _mi_memcpy_aligned(void* dst, const void* src, size_t n) {
  mi_assert_internal(((uintptr_t)dst % MI_INTPTR_SIZE == 0) && ((uintptr_t)src % MI_INTPTR_SIZE == 0));
  void* adst = __builtin_assume_aligned(dst, MI_INTPTR_SIZE);
  const void* asrc = __builtin_assume_aligned(src, MI_INTPTR_SIZE);
  _mi_memcpy(adst, asrc, n);
}

static inline void _mi_memzero_aligned(void* dst, size_t n) {
  mi_assert_internal((uintptr_t)dst % MI_INTPTR_SIZE == 0);
  void* adst = __builtin_assume_aligned(dst, MI_INTPTR_SIZE);
  _mi_memzero(adst, n);
}
#else
// Default fallback on `_mi_memcpy`
static inline void _mi_memcpy_aligned(void* dst, const void* src, size_t n) {
  mi_assert_internal(((uintptr_t)dst % MI_INTPTR_SIZE == 0) && ((uintptr_t)src % MI_INTPTR_SIZE == 0));
  _mi_memcpy(dst, src, n);
}

static inline void _mi_memzero_aligned(void* dst, size_t n) {
  mi_assert_internal((uintptr_t)dst % MI_INTPTR_SIZE == 0);
  _mi_memzero(dst, n);
}
#endif // 3847


// #endif // 627
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// For a static override we create a single object file
// containing the whole library. If it is linked first
// it will override all the standard library allocation
// functions (on Unix's).

// --- Amalgamation START --- mimalloc/src/alloc.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2024, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE   // for realpath() on Linux
#endif // 3891


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/
// #ifndef MIMALLOC_PRIM_H
// #define MIMALLOC_PRIM_H


// --------------------------------------------------------------------------
// This file specifies the primitive portability API.
// Each OS/host needs to implement these primitives, see `src/prim`
// for implementations on Window, macOS, WASI, and Linux/Unix.
//
// note: on all primitive functions, we always have result parameters != NULL, and:
//  addr != NULL and page aligned
//  size > 0     and page aligned
//  the return value is an error code as an `int` where 0 is success
// --------------------------------------------------------------------------

// OS memory configuration
typedef struct mi_os_mem_config_s {
  size_t  page_size;            // default to 4KiB
  size_t  large_page_size;      // 0 if not supported, usually 2MiB (4MiB on Windows)
  size_t  alloc_granularity;    // smallest allocation size (usually 4KiB, on Windows 64KiB)
  size_t  physical_memory;      // physical memory size
  size_t  virtual_address_bits; // usually 48 or 56 bits on 64-bit systems. (used to determine secure randomization)
  bool    has_overcommit;       // can we reserve more memory than can be actually committed?
  bool    has_partial_free;     // can allocated blocks be freed partially? (true for mmap, false for VirtualAlloc)
  bool    has_virtual_reserve;  // supports virtual address space reservation? (if true we can reserve virtual address space without using commit or physical memory)
} mi_os_mem_config_t;

// Initialize
void _mi_prim_mem_init( mi_os_mem_config_t* config );

// Free OS memory
int _mi_prim_free(void* addr, size_t size );

// Allocate OS memory. Return NULL on error.
// The `try_alignment` is just a hint and the returned pointer does not have to be aligned.
// If `commit` is false, the virtual memory range only needs to be reserved (with no access)
// which will later be committed explicitly using `_mi_prim_commit`.
// `is_zero` is set to true if the memory was zero initialized (as on most OS's)
// The `hint_addr` address is either `NULL` or a preferred allocation address but can be ignored.
// pre: !commit => !allow_large
//      try_alignment >= _mi_os_page_size() and a power of 2
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr);

// Commit memory. Returns error code or 0 on success.
// For example, on Linux this would make the memory PROT_READ|PROT_WRITE.
// `is_zero` is set to true if the memory was zero initialized (e.g. on Windows)
int _mi_prim_commit(void* addr, size_t size, bool* is_zero);

// Decommit memory. Returns error code or 0 on success. The `needs_recommit` result is true
// if the memory would need to be re-committed. For example, on Windows this is always true,
// but on Linux we could use MADV_DONTNEED to decommit which does not need a recommit.
// pre: needs_recommit != NULL
int _mi_prim_decommit(void* addr, size_t size, bool* needs_recommit);

// Reset memory. The range keeps being accessible but the content might be reset.
// Returns error code or 0 on success.
int _mi_prim_reset(void* addr, size_t size);

// Protect memory. Returns error code or 0 on success.
int _mi_prim_protect(void* addr, size_t size, bool protect);

// Allocate huge (1GiB) pages possibly associated with a NUMA node.
// `is_zero` is set to true if the memory was zero initialized (as on most OS's)
// pre: size > 0  and a multiple of 1GiB.
//      numa_node is either negative (don't care), or a numa node number.
int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr);

// Return the current NUMA node
size_t _mi_prim_numa_node(void);

// Return the number of logical NUMA nodes
size_t _mi_prim_numa_node_count(void);

// Clock ticks
mi_msecs_t _mi_prim_clock_now(void);

// Return process information (only for statistics)
typedef struct mi_process_info_s {
  mi_msecs_t  elapsed;
  mi_msecs_t  utime;
  mi_msecs_t  stime;
  size_t      current_rss;
  size_t      peak_rss;
  size_t      current_commit;
  size_t      peak_commit;
  size_t      page_faults;
} mi_process_info_t;

void _mi_prim_process_info(mi_process_info_t* pinfo);

// Default stderr output. (only for warnings etc. with verbose enabled)
// msg != NULL && _mi_strlen(msg) > 0
void _mi_prim_out_stderr( const char* msg );

// Get an environment variable. (only for options)
// name != NULL, result != NULL, result_size >= 64
bool _mi_prim_getenv(const char* name, char* result, size_t result_size);


// Fill a buffer with strong randomness; return `false` on error or if
// there is no strong randomization available.
bool _mi_prim_random_buf(void* buf, size_t buf_len);

// Called on the first thread start, and should ensure `_mi_thread_done` is called on thread termination.
void _mi_prim_thread_init_auto_done(void);

// Called on process exit and may take action to clean up resources associated with the thread auto done.
void _mi_prim_thread_done_auto_done(void);

// Called when the default heap for a thread changes
void _mi_prim_thread_associate_default_heap(mi_heap_t* heap);



//-------------------------------------------------------------------
// Thread id: `_mi_prim_thread_id()`
//
// Getting the thread id should be performant as it is called in the
// fast path of `_mi_free` and we specialize for various platforms as
// inlined definitions. Regular code should call `init.c:_mi_thread_id()`.
// We only require _mi_prim_thread_id() to return a unique id
// for each thread (unequal to zero).
//-------------------------------------------------------------------

// On some libc + platform combinations we can directly access a thread-local storage (TLS) slot.
// The TLS layout depends on both the OS and libc implementation so we use specific tests for each main platform.
// If you test on another platform and it works please send a PR :-)
// see also https://akkadia.org/drepper/tls.pdf for more info on the TLS register.
//
// Note: we would like to prefer `__builtin_thread_pointer()` nowadays instead of using assembly,
// but unfortunately we can not detect support reliably (see issue #883)
// We also use it on Apple OS as we use a TLS slot for the default heap there.
#if defined(__GNUC__) && ( \
           (defined(__GLIBC__)   && (defined(__x86_64__) || defined(__i386__) || (defined(__arm__) && __ARM_ARCH >= 7) || defined(__aarch64__))) \
        || (defined(__APPLE__)   && (defined(__x86_64__) || defined(__aarch64__) || defined(__POWERPC__))) \
        || (defined(__BIONIC__)  && (defined(__x86_64__) || defined(__i386__) || (defined(__arm__) && __ARM_ARCH >= 7) || defined(__aarch64__))) \
        || (defined(__FreeBSD__) && (defined(__x86_64__) || defined(__i386__) || defined(__aarch64__))) \
        || (defined(__OpenBSD__) && (defined(__x86_64__) || defined(__i386__) || defined(__aarch64__))) \
      )

#define MI_HAS_TLS_SLOT

static inline void* mi_prim_tls_slot(size_t slot) mi_attr_noexcept {
  void* res;
  const size_t ofs = (slot*sizeof(void*));
  #if defined(__i386__)
    __asm__("movl %%gs:%1, %0" : "=r" (res) : "m" (*((void**)ofs)) : );  // x86 32-bit always uses GS
  #elif defined(__APPLE__) && defined(__x86_64__)
    __asm__("movq %%gs:%1, %0" : "=r" (res) : "m" (*((void**)ofs)) : );  // x86_64 macOSX uses GS
  #elif defined(__x86_64__) && (MI_INTPTR_SIZE==4)
    __asm__("movl %%fs:%1, %0" : "=r" (res) : "m" (*((void**)ofs)) : );  // x32 ABI
  #elif defined(__x86_64__)
    __asm__("movq %%fs:%1, %0" : "=r" (res) : "m" (*((void**)ofs)) : );  // x86_64 Linux, BSD uses FS
  #elif defined(__arm__)
    void** tcb; MI_UNUSED(ofs);
    __asm__ volatile ("mrc p15, 0, %0, c13, c0, 3\nbic %0, %0, #3" : "=r" (tcb));
    res = tcb[slot];
  #elif defined(__aarch64__)
    void** tcb; MI_UNUSED(ofs);
    #if defined(__APPLE__) // M1, issue #343
    __asm__ volatile ("mrs %0, tpidrro_el0\nbic %0, %0, #7" : "=r" (tcb));
    #else
    __asm__ volatile ("mrs %0, tpidr_el0" : "=r" (tcb));
    #endif // 4074
    res = tcb[slot];
  #elif defined(__APPLE__) && defined(__POWERPC__) // ppc, issue #781
    MI_UNUSED(ofs);
    res = pthread_getspecific(slot);
  #endif // 4060
  return res;
}

// setting a tls slot is only used on macOS for now
static inline void mi_prim_tls_slot_set(size_t slot, void* value) mi_attr_noexcept {
  const size_t ofs = (slot*sizeof(void*));
  #if defined(__i386__)
    __asm__("movl %1,%%gs:%0" : "=m" (*((void**)ofs)) : "rn" (value) : );  // 32-bit always uses GS
  #elif defined(__APPLE__) && defined(__x86_64__)
    __asm__("movq %1,%%gs:%0" : "=m" (*((void**)ofs)) : "rn" (value) : );  // x86_64 macOS uses GS
  #elif defined(__x86_64__) && (MI_INTPTR_SIZE==4)
    __asm__("movl %1,%%fs:%0" : "=m" (*((void**)ofs)) : "rn" (value) : );  // x32 ABI
  #elif defined(__x86_64__)
    __asm__("movq %1,%%fs:%0" : "=m" (*((void**)ofs)) : "rn" (value) : );  // x86_64 Linux, BSD uses FS
  #elif defined(__arm__)
    void** tcb; MI_UNUSED(ofs);
    __asm__ volatile ("mrc p15, 0, %0, c13, c0, 3\nbic %0, %0, #3" : "=r" (tcb));
    tcb[slot] = value;
  #elif defined(__aarch64__)
    void** tcb; MI_UNUSED(ofs);
    #if defined(__APPLE__) // M1, issue #343
    __asm__ volatile ("mrs %0, tpidrro_el0\nbic %0, %0, #7" : "=r" (tcb));
    #else
    __asm__ volatile ("mrs %0, tpidr_el0" : "=r" (tcb));
    #endif // 4104
    tcb[slot] = value;
  #elif defined(__APPLE__) && defined(__POWERPC__) // ppc, issue #781
    MI_UNUSED(ofs);
    pthread_setspecific(slot, value);
  #endif // 4090
}

#endif // 4047

// Do we have __builtin_thread_pointer? This would be the preferred way to get a unique thread id
// but unfortunately, it seems we cannot test for this reliably at this time (see issue #883)
// Nevertheless, it seems needed on older graviton platforms (see issue #851).
// For now, we only enable this for specific platforms.
#if !defined(__APPLE__)  /* on apple (M1) the wrong register is read (tpidr_el0 instead of tpidrro_el0) so fall back to TLS slot assembly (<https://github.com/microsoft/mimalloc/issues/343#issuecomment-763272369>)*/ \
    && !defined(MI_LIBC_MUSL) \
    && (!defined(__clang_major__) || __clang_major__ >= 14)  /* older clang versions emit bad code; fall back to using the TLS slot (<https://lore.kernel.org/linux-arm-kernel/202110280952.352F66D8@keescook/T/>) */
  #if    (defined(__GNUC__) && (__GNUC__ >= 7)  && defined(__aarch64__)) /* aarch64 for older gcc versions (issue #851) */ \
      || (defined(__GNUC__) && (__GNUC__ >= 11) && defined(__x86_64__)) \
      || (defined(__clang_major__) && (__clang_major__ >= 14) && (defined(__aarch64__) || defined(__x86_64__)))
    #define MI_USE_BUILTIN_THREAD_POINTER  1
  #endif // 4125
#endif // 4122



// defined in `init.c`; do not use these directly
extern mi_decl_thread mi_heap_t* _mi_heap_default;  // default heap to allocate from
extern bool _mi_process_is_initialized;             // has mi_process_init been called?

static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept;

// Get a unique id for the current thread.
#if defined(MI_PRIM_THREAD_ID)

static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept {
  return MI_PRIM_THREAD_ID();  // used for example by CPython for a free threaded build (see python/cpython#115488)
}

#elif defined(_WIN32)

static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept {
  // Windows: works on Intel and ARM in both 32- and 64-bit
  return (uintptr_t)NtCurrentTeb();
}

#elif 0 // #elif MI_USE_BUILTIN_THREAD_POINTER
// 
// static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept {
//   // Works on most Unix based platforms with recent compilers
//   return (uintptr_t)__builtin_thread_pointer();
// }
// 
#elif defined(MI_HAS_TLS_SLOT)

static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept {
  #if defined(__BIONIC__)
    // issue #384, #495: on the Bionic libc (Android), slot 1 is the thread id
    // see: https://github.com/aosp-mirror/platform_bionic/blob/c44b1d0676ded732df4b3b21c5f798eacae93228/libc/platform/bionic/tls_defines.h#L86
    return (uintptr_t)mi_prim_tls_slot(1);
  #else
    // in all our other targets, slot 0 is the thread id
    // glibc: https://sourceware.org/git/?p=glibc.git;a=blob_plain;f=sysdeps/x86_64/nptl/tls.h
    // apple: https://github.com/apple/darwin-xnu/blob/main/libsyscall/os/tsd.h#L36
    return (uintptr_t)mi_prim_tls_slot(0);
  #endif // 4164
}

#else

// otherwise use portable C, taking the address of a thread local variable (this is still very fast on most platforms).
static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept {
  return (uintptr_t)&_mi_heap_default;
}

#endif // 4141



/* ----------------------------------------------------------------------------------------
The thread local default heap: `_mi_prim_get_default_heap()`
This is inlined here as it is on the fast path for allocation functions.

On most platforms (Windows, Linux, FreeBSD, NetBSD, etc), this just returns a
__thread local variable (`_mi_heap_default`). With the initial-exec TLS model this ensures
that the storage will always be available (allocated on the thread stacks).

On some platforms though we cannot use that when overriding `malloc` since the underlying
TLS implementation (or the loader) will call itself `malloc` on a first access and recurse.
We try to circumvent this in an efficient way:
- macOSX : we use an unused TLS slot from the OS allocated slots (MI_TLS_SLOT). On OSX, the
           loader itself calls `malloc` even before the modules are initialized.
- OpenBSD: we use an unused slot from the pthread block (MI_TLS_PTHREAD_SLOT_OFS).
- DragonFly: defaults are working but seem slow compared to freeBSD (see PR #323)
------------------------------------------------------------------------------------------- */

static inline mi_heap_t* mi_prim_get_default_heap(void);

#if 0 // #if defined(MI_MALLOC_OVERRIDE)
#if 0 // #if defined(__APPLE__) // macOS
//   #define MI_TLS_SLOT               89  // seems unused?
//   // other possible unused ones are 9, 29, __PTK_FRAMEWORK_JAVASCRIPTCORE_KEY4 (94), __PTK_FRAMEWORK_GC_KEY9 (112) and __PTK_FRAMEWORK_OLDGC_KEY9 (89)
//   // see <https://github.com/rweichler/substrate/blob/master/include/pthread_machdep.h>
#elif 0 // #elif defined(__OpenBSD__)
//   // use end bytes of a name; goes wrong if anyone uses names > 23 characters (ptrhread specifies 16)
//   // see <https://github.com/openbsd/src/blob/master/lib/libc/include/thread_private.h#L371>
//   #define MI_TLS_PTHREAD_SLOT_OFS   (6*sizeof(int) + 4*sizeof(void*) + 24)
//   // #elif defined(__DragonFly__)
//   // #warning "mimalloc is not working correctly on DragonFly yet."
//   // #define MI_TLS_PTHREAD_SLOT_OFS   (4 + 1*sizeof(void*))  // offset `uniqueid` (also used by gdb?) <https://github.com/DragonFlyBSD/DragonFlyBSD/blob/master/lib/libthread_xu/thread/thr_private.h#L458>
#elif 0 // #elif defined(__ANDROID__)
//   // See issue #381
//   #define MI_TLS_PTHREAD
#endif // 4207
#endif // 4206


#if defined(MI_TLS_SLOT)
# if !defined(MI_HAS_TLS_SLOT)
#  error "trying to use a TLS slot for the default heap, but the mi_prim_tls_slot primitives are not defined"
# endif

static inline mi_heap_t* mi_prim_get_default_heap(void) {
  mi_heap_t* heap = (mi_heap_t*)mi_prim_tls_slot(MI_TLS_SLOT);
  if mi_unlikely(heap == NULL) {
    #ifdef __GNUC__
    __asm(""); // prevent conditional load of the address of _mi_heap_empty
    #endif // 4233
    heap = (mi_heap_t*)&_mi_heap_empty;
  }
  return heap;
}

#elif defined(MI_TLS_PTHREAD_SLOT_OFS)

static inline mi_heap_t** mi_prim_tls_pthread_heap_slot(void) {
  pthread_t self = pthread_self();
  #if defined(__DragonFly__)
  if (self==NULL) return NULL;
  #endif // 4245
  return (mi_heap_t**)((uint8_t*)self + MI_TLS_PTHREAD_SLOT_OFS);
}

static inline mi_heap_t* mi_prim_get_default_heap(void) {
  mi_heap_t** pheap = mi_prim_tls_pthread_heap_slot();
  if mi_unlikely(pheap == NULL) return _mi_heap_main_get();
  mi_heap_t* heap = *pheap;
  if mi_unlikely(heap == NULL) return (mi_heap_t*)&_mi_heap_empty;
  return heap;
}

#elif defined(MI_TLS_PTHREAD)

extern pthread_key_t _mi_heap_default_key;
static inline mi_heap_t* mi_prim_get_default_heap(void) {
  mi_heap_t* heap = (mi_unlikely(_mi_heap_default_key == (pthread_key_t)(-1)) ? _mi_heap_main_get() : (mi_heap_t*)pthread_getspecific(_mi_heap_default_key));
  return (mi_unlikely(heap == NULL) ? (mi_heap_t*)&_mi_heap_empty : heap);
}

#else // default using a thread local variable; used on most platforms.

static inline mi_heap_t* mi_prim_get_default_heap(void) {
  #if defined(MI_TLS_RECURSE_GUARD)
  if (mi_unlikely(!_mi_process_is_initialized)) return _mi_heap_main_get();
  #endif // 4270
  return _mi_heap_default;
}

#endif  // mi_prim_get_default_heap() // 4225





// #endif  // MIMALLOC_PRIM_H // 3915
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


#include <string.h>      // memset, strlen (for mi_strdup)
#include <stdlib.h>      // malloc, abort

#define MI_IN_ALLOC_C

// --- Amalgamation START --- mimalloc/src/alloc-override.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2021, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

#if !defined(MI_IN_ALLOC_C)
#error "this file should be included from 'alloc.c' (so aliases can work)"
#endif // 4299

#if 0 // #if defined(MI_MALLOC_OVERRIDE) && defined(_WIN32) && !(defined(MI_SHARED_LIB) && defined(_DLL))
// #error "It is only possible to override "malloc" on Windows when building as a DLL (and linking the C runtime as a DLL)"
#endif // 4303

#if 0 // #if defined(MI_MALLOC_OVERRIDE) && !(defined(_WIN32))
// 
#if 0 // #if defined(__APPLE__)
// #include <AvailabilityMacros.h>
// mi_decl_externc void   vfree(void* p);
// mi_decl_externc size_t malloc_size(const void* p);
// mi_decl_externc size_t malloc_good_size(size_t size);
#endif // 4309
// 
// // helper definition for C override of C++ new
// typedef void* mi_nothrow_t;
// 
// // ------------------------------------------------------
// // Override system malloc
// // ------------------------------------------------------
// 
#if 0 // #if (defined(__GNUC__) || defined(__clang__)) && !defined(__APPLE__) && !MI_TRACK_ENABLED
//   // gcc, clang: use aliasing to alias the exported function to one of our `mi_` functions
#if 0 //   #if (defined(__GNUC__) && __GNUC__ >= 9)
//     #pragma GCC diagnostic ignored "-Wattributes"  // or we get warnings that nodiscard is ignored on a forward
//     #define MI_FORWARD(fun)      __attribute__((alias(#fun), used, visibility("default"), copy(fun)));
  #else
//     #define MI_FORWARD(fun)      __attribute__((alias(#fun), used, visibility("default")));
  #endif // 4325
//   #define MI_FORWARD1(fun,x)      MI_FORWARD(fun)
//   #define MI_FORWARD2(fun,x,y)    MI_FORWARD(fun)
//   #define MI_FORWARD3(fun,x,y,z)  MI_FORWARD(fun)
//   #define MI_FORWARD0(fun,x)      MI_FORWARD(fun)
//   #define MI_FORWARD02(fun,x,y)   MI_FORWARD(fun)
#else
//   // otherwise use forwarding by calling our `mi_` function
//   #define MI_FORWARD1(fun,x)      { return fun(x); }
//   #define MI_FORWARD2(fun,x,y)    { return fun(x,y); }
//   #define MI_FORWARD3(fun,x,y,z)  { return fun(x,y,z); }
//   #define MI_FORWARD0(fun,x)      { fun(x); }
//   #define MI_FORWARD02(fun,x,y)   { fun(x,y); }
#endif // 4323
// 
// 
#if 0 // #if defined(__APPLE__) && defined(MI_SHARED_LIB_EXPORT) && defined(MI_OSX_INTERPOSE)
//   // define MI_OSX_IS_INTERPOSED as we should not provide forwarding definitions for
//   // functions that are interposed (or the interposing does not work)
//   #define MI_OSX_IS_INTERPOSED
// 
//   mi_decl_externc size_t mi_malloc_size_checked(void *p) {
//     if (!mi_is_in_heap_region(p)) return 0;
//     return mi_usable_size(p);
//   }
// 
//   // use interposing so `DYLD_INSERT_LIBRARIES` works without `DYLD_FORCE_FLAT_NAMESPACE=1`
//   // See: <https://books.google.com/books?id=K8vUkpOXhN4C&pg=PA73>
//   struct mi_interpose_s {
//     const void* replacement;
//     const void* target;
//   };
//   #define MI_INTERPOSE_FUN(oldfun,newfun) { (const void*)&newfun, (const void*)&oldfun }
//   #define MI_INTERPOSE_MI(fun)            MI_INTERPOSE_FUN(fun,mi_##fun)
// 
//   __attribute__((used)) static struct mi_interpose_s _mi_interposes[]  __attribute__((section("__DATA, __interpose"))) =
//   {
//     MI_INTERPOSE_MI(malloc),
//     MI_INTERPOSE_MI(calloc),
//     MI_INTERPOSE_MI(realloc),
//     MI_INTERPOSE_MI(strdup),
#if 0 //     #if defined(MAC_OS_X_VERSION_10_7) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7
//     MI_INTERPOSE_MI(strndup),
    #endif // 4371
//     MI_INTERPOSE_MI(realpath),
//     MI_INTERPOSE_MI(posix_memalign),
//     MI_INTERPOSE_MI(reallocf),
//     MI_INTERPOSE_MI(valloc),
//     MI_INTERPOSE_FUN(malloc_size,mi_malloc_size_checked),
//     MI_INTERPOSE_MI(malloc_good_size),
#if 0 //     #if defined(MAC_OS_X_VERSION_10_15) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_15
//     MI_INTERPOSE_MI(aligned_alloc),
    #endif // 4380
#if 0 //     #ifdef MI_OSX_ZONE
//     // we interpose malloc_default_zone in alloc-override-osx.c so we can use mi_free safely
//     MI_INTERPOSE_MI(free),
//     MI_INTERPOSE_FUN(vfree,mi_free),
    #else
//     // sometimes code allocates from default zone but deallocates using plain free :-( (like NxHashResizeToCapacity <https://github.com/nneonneo/osx-10.9-opensource/blob/master/objc4-551.1/runtime/hashtable2.mm>)
//     MI_INTERPOSE_FUN(free,mi_cfree), // use safe free that checks if pointers are from us
//     MI_INTERPOSE_FUN(vfree,mi_cfree),
    #endif // 4383
//   };
// 
#if 0 //   #ifdef __cplusplus
//   extern "C" {
  #endif // 4394
//   void  _ZdlPv(void* p);   // delete
//   void  _ZdaPv(void* p);   // delete[]
//   void  _ZdlPvm(void* p, size_t n);  // delete
//   void  _ZdaPvm(void* p, size_t n);  // delete[]
//   void* _Znwm(size_t n);  // new
//   void* _Znam(size_t n);  // new[]
//   void* _ZnwmRKSt9nothrow_t(size_t n, mi_nothrow_t tag); // new nothrow
//   void* _ZnamRKSt9nothrow_t(size_t n, mi_nothrow_t tag); // new[] nothrow
#if 0 //   #ifdef __cplusplus
//   }
  #endif // 4405
//   __attribute__((used)) static struct mi_interpose_s _mi_cxx_interposes[]  __attribute__((section("__DATA, __interpose"))) =
//   {
//     MI_INTERPOSE_FUN(_ZdlPv,mi_free),
//     MI_INTERPOSE_FUN(_ZdaPv,mi_free),
//     MI_INTERPOSE_FUN(_ZdlPvm,mi_free_size),
//     MI_INTERPOSE_FUN(_ZdaPvm,mi_free_size),
//     MI_INTERPOSE_FUN(_Znwm,mi_new),
//     MI_INTERPOSE_FUN(_Znam,mi_new),
//     MI_INTERPOSE_FUN(_ZnwmRKSt9nothrow_t,mi_new_nothrow),
//     MI_INTERPOSE_FUN(_ZnamRKSt9nothrow_t,mi_new_nothrow),
//   };
// 
#elif 0 // #elif defined(_MSC_VER)
//   // cannot override malloc unless using a dll.
//   // we just override new/delete which does work in a static library.
#else
//   // On all other systems forward allocation primitives to our API
//   mi_decl_export void* malloc(size_t size)              MI_FORWARD1(mi_malloc, size)
//   mi_decl_export void* calloc(size_t size, size_t n)    MI_FORWARD2(mi_calloc, size, n)
//   mi_decl_export void* realloc(void* p, size_t newsize) MI_FORWARD2(mi_realloc, p, newsize)
//   mi_decl_export void  free(void* p)                    MI_FORWARD0(mi_free, p)  
//   // In principle we do not need to forward `strdup`/`strndup` but on some systems these do not use `malloc` internally (but a more primitive call)
//   // We only override if `strdup` is not a macro (as on some older libc's, see issue #885)
#if 0 //   #if !defined(strdup)
//   mi_decl_export char* strdup(const char* str)             MI_FORWARD1(mi_strdup, str)
  #endif // 4431
#if 0 //   #if !defined(strndup) && (!defined(__APPLE__) || (defined(MAC_OS_X_VERSION_10_7) && MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7))
//   mi_decl_export char* strndup(const char* str, size_t n)  MI_FORWARD2(mi_strndup, str, n)   
  #endif // 4434
#endif // 4346
// 
#if 0 // #if (defined(__GNUC__) || defined(__clang__)) && !defined(__APPLE__)
// #pragma GCC visibility push(default)
#endif // 4439
// 
// // ------------------------------------------------------
// // Override new/delete
// // This is not really necessary as they usually call
// // malloc/free anyway, but it improves performance.
// // ------------------------------------------------------
#if 0 // #ifdef __cplusplus
//   // ------------------------------------------------------
//   // With a C++ compiler we override the new/delete operators.
//   // see <https://en.cppreference.com/w/cpp/memory/new/operator_new>
//   // ------------------------------------------------------
//   #include <new>
// 
#if 0 //   #ifndef MI_OSX_IS_INTERPOSED
//     void operator delete(void* p) noexcept              MI_FORWARD0(mi_free,p)
//     void operator delete[](void* p) noexcept            MI_FORWARD0(mi_free,p)
// 
//     void* operator new(std::size_t n) noexcept(false)   MI_FORWARD1(mi_new,n)
//     void* operator new[](std::size_t n) noexcept(false) MI_FORWARD1(mi_new,n)
// 
//     void* operator new  (std::size_t n, const std::nothrow_t& tag) noexcept { MI_UNUSED(tag); return mi_new_nothrow(n); }
//     void* operator new[](std::size_t n, const std::nothrow_t& tag) noexcept { MI_UNUSED(tag); return mi_new_nothrow(n); }
// 
#if 0 //     #if (__cplusplus >= 201402L || _MSC_VER >= 1916)
//     void operator delete  (void* p, std::size_t n) noexcept MI_FORWARD02(mi_free_size,p,n)
//     void operator delete[](void* p, std::size_t n) noexcept MI_FORWARD02(mi_free_size,p,n)
    #endif // 4465
  #endif // 4455
// 
#if 0 //   #if (__cplusplus > 201402L && defined(__cpp_aligned_new)) && (!defined(__GNUC__) || (__GNUC__ > 5))
//   void operator delete  (void* p, std::align_val_t al) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
//   void operator delete[](void* p, std::align_val_t al) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
//   void operator delete  (void* p, std::size_t n, std::align_val_t al) noexcept { mi_free_size_aligned(p, n, static_cast<size_t>(al)); };
//   void operator delete[](void* p, std::size_t n, std::align_val_t al) noexcept { mi_free_size_aligned(p, n, static_cast<size_t>(al)); };
//   void operator delete  (void* p, std::align_val_t al, const std::nothrow_t&) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
//   void operator delete[](void* p, std::align_val_t al, const std::nothrow_t&) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
// 
//   void* operator new( std::size_t n, std::align_val_t al)   noexcept(false) { return mi_new_aligned(n, static_cast<size_t>(al)); }
//   void* operator new[]( std::size_t n, std::align_val_t al) noexcept(false) { return mi_new_aligned(n, static_cast<size_t>(al)); }
//   void* operator new  (std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept { return mi_new_aligned_nothrow(n, static_cast<size_t>(al)); }
//   void* operator new[](std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept { return mi_new_aligned_nothrow(n, static_cast<size_t>(al)); }
  #endif // 4471
// 
#elif 0 // #elif (defined(__GNUC__) || defined(__clang__))
//   // ------------------------------------------------------
//   // Override by defining the mangled C++ names of the operators (as
//   // used by GCC and CLang).
//   // See <https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling>
//   // ------------------------------------------------------
// 
//   void _ZdlPv(void* p)            MI_FORWARD0(mi_free,p) // delete
//   void _ZdaPv(void* p)            MI_FORWARD0(mi_free,p) // delete[]
//   void _ZdlPvm(void* p, size_t n) MI_FORWARD02(mi_free_size,p,n)
//   void _ZdaPvm(void* p, size_t n) MI_FORWARD02(mi_free_size,p,n)
//   
//   void _ZdlPvSt11align_val_t(void* p, size_t al)            { mi_free_aligned(p,al); }
//   void _ZdaPvSt11align_val_t(void* p, size_t al)            { mi_free_aligned(p,al); }
//   void _ZdlPvmSt11align_val_t(void* p, size_t n, size_t al) { mi_free_size_aligned(p,n,al); }
//   void _ZdaPvmSt11align_val_t(void* p, size_t n, size_t al) { mi_free_size_aligned(p,n,al); }
// 
//   void _ZdlPvRKSt9nothrow_t(void* p, mi_nothrow_t tag)      { MI_UNUSED(tag); mi_free(p); }  // operator delete(void*, std::nothrow_t const&) 
//   void _ZdaPvRKSt9nothrow_t(void* p, mi_nothrow_t tag)      { MI_UNUSED(tag); mi_free(p); }  // operator delete[](void*, std::nothrow_t const&)
//   void _ZdlPvSt11align_val_tRKSt9nothrow_t(void* p, size_t al, mi_nothrow_t tag) { MI_UNUSED(tag); mi_free_aligned(p,al); } // operator delete(void*, std::align_val_t, std::nothrow_t const&) 
//   void _ZdaPvSt11align_val_tRKSt9nothrow_t(void* p, size_t al, mi_nothrow_t tag) { MI_UNUSED(tag); mi_free_aligned(p,al); } // operator delete[](void*, std::align_val_t, std::nothrow_t const&) 
//   
#if 0 //   #if (MI_INTPTR_SIZE==8)
//     void* _Znwm(size_t n)                             MI_FORWARD1(mi_new,n)  // new 64-bit
//     void* _Znam(size_t n)                             MI_FORWARD1(mi_new,n)  // new[] 64-bit
//     void* _ZnwmRKSt9nothrow_t(size_t n, mi_nothrow_t tag) { MI_UNUSED(tag); return mi_new_nothrow(n); }
//     void* _ZnamRKSt9nothrow_t(size_t n, mi_nothrow_t tag) { MI_UNUSED(tag); return mi_new_nothrow(n); }
//     void* _ZnwmSt11align_val_t(size_t n, size_t al)   MI_FORWARD2(mi_new_aligned, n, al)
//     void* _ZnamSt11align_val_t(size_t n, size_t al)   MI_FORWARD2(mi_new_aligned, n, al)
//     void* _ZnwmSt11align_val_tRKSt9nothrow_t(size_t n, size_t al, mi_nothrow_t tag) { MI_UNUSED(tag); return mi_new_aligned_nothrow(n,al); }
//     void* _ZnamSt11align_val_tRKSt9nothrow_t(size_t n, size_t al, mi_nothrow_t tag) { MI_UNUSED(tag); return mi_new_aligned_nothrow(n,al); }
#elif 0 //   #elif (MI_INTPTR_SIZE==4)
//     void* _Znwj(size_t n)                             MI_FORWARD1(mi_new,n)  // new 64-bit
//     void* _Znaj(size_t n)                             MI_FORWARD1(mi_new,n)  // new[] 64-bit
//     void* _ZnwjRKSt9nothrow_t(size_t n, mi_nothrow_t tag) { MI_UNUSED(tag); return mi_new_nothrow(n); }
//     void* _ZnajRKSt9nothrow_t(size_t n, mi_nothrow_t tag) { MI_UNUSED(tag); return mi_new_nothrow(n); }
//     void* _ZnwjSt11align_val_t(size_t n, size_t al)   MI_FORWARD2(mi_new_aligned, n, al)
//     void* _ZnajSt11align_val_t(size_t n, size_t al)   MI_FORWARD2(mi_new_aligned, n, al)
//     void* _ZnwjSt11align_val_tRKSt9nothrow_t(size_t n, size_t al, mi_nothrow_t tag) { MI_UNUSED(tag); return mi_new_aligned_nothrow(n,al); }
//     void* _ZnajSt11align_val_tRKSt9nothrow_t(size_t n, size_t al, mi_nothrow_t tag) { MI_UNUSED(tag); return mi_new_aligned_nothrow(n,al); }
  #else
//     #error "define overloads for new/delete for this platform (just for performance, can be skipped)"
  #endif // 4507
#endif // __cplusplus // 4448
// 
// // ------------------------------------------------------
// // Further Posix & Unix functions definitions
// // ------------------------------------------------------
// 
#if 0 // #ifdef __cplusplus
// extern "C" {
#endif // 4534
// 
#if 0 // #ifndef MI_OSX_IS_INTERPOSED
//   // Forward Posix/Unix calls as well
//   void*  reallocf(void* p, size_t newsize) MI_FORWARD2(mi_reallocf,p,newsize)
//   size_t malloc_size(const void* p)        MI_FORWARD1(mi_usable_size,p)
#if 0 //   #if !defined(__ANDROID__) && !defined(__FreeBSD__)
//   size_t malloc_usable_size(void *p)       MI_FORWARD1(mi_usable_size,p)
  #else
//   size_t malloc_usable_size(const void *p) MI_FORWARD1(mi_usable_size,p)
  #endif // 4542
// 
//   // No forwarding here due to aliasing/name mangling issues
//   void*  valloc(size_t size)               { return mi_valloc(size); }
//   void   vfree(void* p)                    { mi_free(p); }
//   size_t malloc_good_size(size_t size)     { return mi_malloc_good_size(size); }
//   int    posix_memalign(void** p, size_t alignment, size_t size) { return mi_posix_memalign(p, alignment, size); }
// 
//   // `aligned_alloc` is only available when __USE_ISOC11 is defined.
//   // Note: it seems __USE_ISOC11 is not defined in musl (and perhaps other libc's) so we only check
//   // for it if using glibc.
//   // Note: Conda has a custom glibc where `aligned_alloc` is declared `static inline` and we cannot
//   // override it, but both _ISOC11_SOURCE and __USE_ISOC11 are undefined in Conda GCC7 or GCC9.
//   // Fortunately, in the case where `aligned_alloc` is declared as `static inline` it
//   // uses internally `memalign`, `posix_memalign`, or `_aligned_malloc` so we  can avoid overriding it ourselves.
#if 0 //   #if !defined(__GLIBC__) || __USE_ISOC11
//   void* aligned_alloc(size_t alignment, size_t size) { return mi_aligned_alloc(alignment, size); }
  #endif // 4561
#endif // 4538
// 
// // no forwarding here due to aliasing/name mangling issues
// void  cfree(void* p)                                    { mi_free(p); }
// void* pvalloc(size_t size)                              { return mi_pvalloc(size); }
// void* memalign(size_t alignment, size_t size)           { return mi_memalign(alignment, size); }
// void* _aligned_malloc(size_t alignment, size_t size)    { return mi_aligned_alloc(alignment, size); }
// void* reallocarray(void* p, size_t count, size_t size)  { return mi_reallocarray(p, count, size); }
// // some systems define reallocarr so mark it as a weak symbol (#751)
// mi_decl_weak int reallocarr(void* p, size_t count, size_t size)    { return mi_reallocarr(p, count, size); }
// 
#if 0 // #if defined(__wasi__)
//   // forward __libc interface (see PR #667)
//   void* __libc_malloc(size_t size)                      MI_FORWARD1(mi_malloc, size)
//   void* __libc_calloc(size_t count, size_t size)        MI_FORWARD2(mi_calloc, count, size)
//   void* __libc_realloc(void* p, size_t size)            MI_FORWARD2(mi_realloc, p, size)
//   void  __libc_free(void* p)                            MI_FORWARD0(mi_free, p)
//   void* __libc_memalign(size_t alignment, size_t size)  { return mi_memalign(alignment, size); }
// 
#elif 0 // #elif defined(__linux__)
//   // forward __libc interface (needed for glibc-based and musl-based Linux distributions)
//   void* __libc_malloc(size_t size)                      MI_FORWARD1(mi_malloc,size)
//   void* __libc_calloc(size_t count, size_t size)        MI_FORWARD2(mi_calloc,count,size)
//   void* __libc_realloc(void* p, size_t size)            MI_FORWARD2(mi_realloc,p,size)
//   void  __libc_free(void* p)                            MI_FORWARD0(mi_free,p)
//   void  __libc_cfree(void* p)                           MI_FORWARD0(mi_free,p)
// 
//   void* __libc_valloc(size_t size)                      { return mi_valloc(size); }
//   void* __libc_pvalloc(size_t size)                     { return mi_pvalloc(size); }
//   void* __libc_memalign(size_t alignment, size_t size)  { return mi_memalign(alignment,size); }
//   int   __posix_memalign(void** p, size_t alignment, size_t size) { return mi_posix_memalign(p,alignment,size); }
#endif // 4575
// 
#if 0 // #ifdef __cplusplus
// }
#endif // 4597
// 
#if 0 // #if (defined(__GNUC__) || defined(__clang__)) && !defined(__APPLE__)
// #pragma GCC visibility pop
#endif // 4601
// 
#endif // MI_MALLOC_OVERRIDE && !_WIN32 // 4307
// --- Amalgamation END --- mimalloc/src/alloc-override.c


// --- Amalgamation START --- mimalloc/src/free.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2024, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/
#if !defined(MI_IN_ALLOC_C)
#error "this file should be included from 'alloc.c' (so aliases can work from alloc-override)"
// add includes help an IDE

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h

#endif // 4616

// forward declarations
static void   mi_check_padding(const mi_page_t* page, const mi_block_t* block);
static bool   mi_check_is_double_free(const mi_page_t* page, const mi_block_t* block);
static size_t mi_page_usable_size_of(const mi_page_t* page, const mi_block_t* block);
static void   mi_stat_free(const mi_page_t* page, const mi_block_t* block);


// ------------------------------------------------------
// Free
// ------------------------------------------------------

// forward declaration of multi-threaded free (`_mt`) (or free in huge block if compiled with MI_HUGE_PAGE_ABANDON)
static mi_decl_noinline void mi_free_block_mt(mi_page_t* page, mi_segment_t* segment, mi_block_t* block);

// regular free of a (thread local) block pointer
// fast path written carefully to prevent spilling on the stack
static inline void mi_free_block_local(mi_page_t* page, mi_block_t* block, bool track_stats, bool check_full)
{
  // checks
  if mi_unlikely(mi_check_is_double_free(page, block)) return;
  mi_check_padding(page, block);
  if (track_stats) { mi_stat_free(page, block); }
  #if (MI_DEBUG>0) && !MI_TRACK_ENABLED  && !MI_TSAN && !MI_GUARDED
  memset(block, MI_DEBUG_FREED, mi_page_block_size(page));
  #endif // 4655
  if (track_stats) { mi_track_free_size(block, mi_page_usable_size_of(page, block)); } // faster then mi_usable_size as we already know the page and that p is unaligned

  // actual free: push on the local free list
  mi_block_set_next(page, block, page->local_free);
  page->local_free = block;
  if mi_unlikely(--page->used == 0) {
    _mi_page_retire(page);
  }
  else if mi_unlikely(check_full && mi_page_is_in_full(page)) {
    _mi_page_unfull(page);
  }
}

// Adjust a block that was allocated aligned, to the actual start of the block in the page.
// note: this can be called from `mi_free_generic_mt` where a non-owning thread accesses the
// `page_start` and `block_size` fields; however these are constant and the page won't be
// deallocated (as the block we are freeing keeps it alive) and thus safe to read concurrently.
mi_block_t* _mi_page_ptr_unalign(const mi_page_t* page, const void* p) {
  mi_assert_internal(page!=NULL && p!=NULL);

  size_t diff = (uint8_t*)p - page->page_start;
  size_t adjust;
  if mi_likely(page->block_size_shift != 0) {
    adjust = diff & (((size_t)1 << page->block_size_shift) - 1);
  }
  else {
    adjust = diff % mi_page_block_size(page);
  }

  return (mi_block_t*)((uintptr_t)p - adjust);
}

// forward declaration for a MI_GUARDED build
#if MI_GUARDED
static void mi_block_unguard(mi_page_t* page, mi_block_t* block, void* p); // forward declaration
static inline void mi_block_check_unguard(mi_page_t* page, mi_block_t* block, void* p) {
  if (mi_block_ptr_is_guarded(block, p)) { mi_block_unguard(page, block, p); }
}
#else
static inline void mi_block_check_unguard(mi_page_t* page, mi_block_t* block, void* p) {
  MI_UNUSED(page); MI_UNUSED(block); MI_UNUSED(p);
}
#endif // 4691

// free a local pointer  (page parameter comes first for better codegen)
static void mi_decl_noinline mi_free_generic_local(mi_page_t* page, mi_segment_t* segment, void* p) mi_attr_noexcept {
  MI_UNUSED(segment);
  mi_block_t* const block = (mi_page_has_aligned(page) ? _mi_page_ptr_unalign(page, p) : (mi_block_t*)p);
  mi_block_check_unguard(page, block, p);
  mi_free_block_local(page, block, true /* track stats */, true /* check for a full page */);
}

// free a pointer owned by another thread (page parameter comes first for better codegen)
static void mi_decl_noinline mi_free_generic_mt(mi_page_t* page, mi_segment_t* segment, void* p) mi_attr_noexcept {
  mi_block_t* const block = _mi_page_ptr_unalign(page, p); // don't check `has_aligned` flag to avoid a race (issue #865)
  mi_block_check_unguard(page, block, p);
  mi_free_block_mt(page, segment, block);
}

// generic free (for runtime integration)
void mi_decl_noinline _mi_free_generic(mi_segment_t* segment, mi_page_t* page, bool is_local, void* p) mi_attr_noexcept {
  if (is_local) mi_free_generic_local(page,segment,p);
           else mi_free_generic_mt(page,segment,p);
}

// Get the segment data belonging to a pointer
// This is just a single `and` in release mode but does further checks in debug mode
// (and secure mode) to see if this was a valid pointer.
static inline mi_segment_t* mi_checked_ptr_segment(const void* p, const char* msg)
{
  MI_UNUSED(msg);

  #if (MI_DEBUG>0)
  if mi_unlikely(((uintptr_t)p & (MI_INTPTR_SIZE - 1)) != 0 && !mi_option_is_enabled(mi_option_guarded_precise)) {
    _mi_error_message(EINVAL, "%s: invalid (unaligned) pointer: %p\n", msg, p);
    return NULL;
  }
  #endif // 4730

  mi_segment_t* const segment = _mi_ptr_segment(p);
  if mi_unlikely(segment==NULL) return segment;

  #if (MI_DEBUG>0)
  if mi_unlikely(!mi_is_in_heap_region(p)) {
    _mi_warning_message("%s: pointer might not point to a valid heap region: %p\n"
      "(this may still be a valid very large allocation (over 64MiB))\n", msg, p);
    if mi_likely(_mi_ptr_cookie(segment) == segment->cookie) {
      _mi_warning_message("(yes, the previous pointer %p was valid after all)\n", p);
    }
  }
  #endif // 4740
  #if (MI_DEBUG>0 || MI_SECURE>=4)
  if mi_unlikely(_mi_ptr_cookie(segment) != segment->cookie) {
    _mi_error_message(EINVAL, "%s: pointer does not point to a valid heap space: %p\n", msg, p);
    return NULL;
  }
  #endif // 4749

  return segment;
}

// Free a block
// Fast path written carefully to prevent register spilling on the stack
void mi_free(void* p) mi_attr_noexcept
{
  mi_segment_t* const segment = mi_checked_ptr_segment(p,"mi_free");
  if mi_unlikely(segment==NULL) return;

  const bool is_local = (_mi_prim_thread_id() == mi_atomic_load_relaxed(&segment->thread_id));
  mi_page_t* const page = _mi_segment_page_of(segment, p);

  if mi_likely(is_local) {                        // thread-local free?
    if mi_likely(page->flags.full_aligned == 0) { // and it is not a full page (full pages need to move from the full bin), nor has aligned blocks (aligned blocks need to be unaligned)
      // thread-local, aligned, and not a full page
      mi_block_t* const block = (mi_block_t*)p;
      mi_free_block_local(page, block, true /* track stats */, false /* no need to check if the page is full */);
    }
    else {
      // page is full or contains (inner) aligned blocks; use generic path
      mi_free_generic_local(page, segment, p);
    }
  }
  else {
    // not thread-local; use generic path
    mi_free_generic_mt(page, segment, p);
  }
}

// return true if successful
bool _mi_free_delayed_block(mi_block_t* block) {
  // get segment and page
  mi_assert_internal(block!=NULL);
  const mi_segment_t* const segment = _mi_ptr_segment(block);
  mi_assert_internal(_mi_ptr_cookie(segment) == segment->cookie);
  mi_assert_internal(_mi_thread_id() == segment->thread_id);
  mi_page_t* const page = _mi_segment_page_of(segment, block);

  // Clear the no-delayed flag so delayed freeing is used again for this page.
  // This must be done before collecting the free lists on this page -- otherwise
  // some blocks may end up in the page `thread_free` list with no blocks in the
  // heap `thread_delayed_free` list which may cause the page to be never freed!
  // (it would only be freed if we happen to scan it in `mi_page_queue_find_free_ex`)
  if (!_mi_page_try_use_delayed_free(page, MI_USE_DELAYED_FREE, false /* dont overwrite never delayed */)) {
    return false;
  }

  // collect all other non-local frees (move from `thread_free` to `free`) to ensure up-to-date `used` count
  _mi_page_free_collect(page, false);

  // and free the block (possibly freeing the page as well since `used` is updated)
  mi_free_block_local(page, block, false /* stats have already been adjusted */, true /* check for a full page */);
  return true;
}

// ------------------------------------------------------
// Multi-threaded Free (`_mt`)
// ------------------------------------------------------

// Push a block that is owned by another thread on its page-local thread free
// list or it's heap delayed free list. Such blocks are later collected by
// the owning thread in `_mi_free_delayed_block`.
static void mi_decl_noinline mi_free_block_delayed_mt( mi_page_t* page, mi_block_t* block )
{
  // Try to put the block on either the page-local thread free list,
  // or the heap delayed free list (if this is the first non-local free in that page)
  mi_thread_free_t tfreex;
  bool use_delayed;
  mi_thread_free_t tfree = mi_atomic_load_relaxed(&page->xthread_free);
  do {
    use_delayed = (mi_tf_delayed(tfree) == MI_USE_DELAYED_FREE);
    if mi_unlikely(use_delayed) {
      // unlikely: this only happens on the first concurrent free in a page that is in the full list
      tfreex = mi_tf_set_delayed(tfree,MI_DELAYED_FREEING);
    }
    else {
      // usual: directly add to page thread_free list
      mi_block_set_next(page, block, mi_tf_block(tfree));
      tfreex = mi_tf_set_block(tfree,block);
    }
  } while (!mi_atomic_cas_weak_release(&page->xthread_free, &tfree, tfreex));

  // If this was the first non-local free, we need to push it on the heap delayed free list instead
  if mi_unlikely(use_delayed) {
    // racy read on `heap`, but ok because MI_DELAYED_FREEING is set (see `mi_heap_delete` and `mi_heap_collect_abandon`)
    mi_heap_t* const heap = (mi_heap_t*)(mi_atomic_load_acquire(&page->xheap)); //mi_page_heap(page);
    mi_assert_internal(heap != NULL);
    if (heap != NULL) {
      // add to the delayed free list of this heap. (do this atomically as the lock only protects heap memory validity)
      mi_block_t* dfree = mi_atomic_load_ptr_relaxed(mi_block_t, &heap->thread_delayed_free);
      do {
        mi_block_set_nextx(heap,block,dfree, heap->keys);
      } while (!mi_atomic_cas_ptr_weak_release(mi_block_t,&heap->thread_delayed_free, &dfree, block));
    }

    // and reset the MI_DELAYED_FREEING flag
    tfree = mi_atomic_load_relaxed(&page->xthread_free);
    do {
      tfreex = tfree;
      mi_assert_internal(mi_tf_delayed(tfree) == MI_DELAYED_FREEING);
      tfreex = mi_tf_set_delayed(tfree,MI_NO_DELAYED_FREE);
    } while (!mi_atomic_cas_weak_release(&page->xthread_free, &tfree, tfreex));
  }
}

// Multi-threaded free (`_mt`) (or free in huge block if compiled with MI_HUGE_PAGE_ABANDON)
static void mi_decl_noinline mi_free_block_mt(mi_page_t* page, mi_segment_t* segment, mi_block_t* block)
{
  // first see if the segment was abandoned and if we can reclaim it into our thread
  if (_mi_option_get_fast(mi_option_abandoned_reclaim_on_free) != 0 &&
      #if MI_HUGE_PAGE_ABANDON
      segment->page_kind != MI_PAGE_HUGE &&
      #endif // 4867
      mi_atomic_load_relaxed(&segment->thread_id) == 0 &&  // segment is abandoned?
      mi_prim_get_default_heap() != (mi_heap_t*)&_mi_heap_empty) // and we did not already exit this thread (without this check, a fresh heap will be initalized (issue #944))
  {
    // the segment is abandoned, try to reclaim it into our heap
    if (_mi_segment_attempt_reclaim(mi_heap_get_default(), segment)) {
      mi_assert_internal(_mi_thread_id() == mi_atomic_load_relaxed(&segment->thread_id));
      mi_assert_internal(mi_heap_get_default()->tld->segments.subproc == segment->subproc);
      mi_free(block);  // recursively free as now it will be a local free in our heap
      return;
    }
  }

  // The padding check may access the non-thread-owned page for the key values.
  // that is safe as these are constant and the page won't be freed (as the block is not freed yet).
  mi_check_padding(page, block);

  // adjust stats (after padding check and potentially recursive `mi_free` above)
  mi_stat_free(page, block);    // stat_free may access the padding
  mi_track_free_size(block, mi_page_usable_size_of(page,block));

  // for small size, ensure we can fit the delayed thread pointers without triggering overflow detection
  _mi_padding_shrink(page, block, sizeof(mi_block_t));

  if (segment->page_kind == MI_PAGE_HUGE) {
    #if MI_HUGE_PAGE_ABANDON
    // huge page segments are always abandoned and can be freed immediately
    _mi_segment_huge_page_free(segment, page, block);
    return;
    #else
    // huge pages are special as they occupy the entire segment
    // as these are large we reset the memory occupied by the page so it is available to other threads
    // (as the owning thread needs to actually free the memory later).
    _mi_segment_huge_page_reset(segment, page, block);
    #endif // 4894
  }
  else {
    #if (MI_DEBUG>0) && !MI_TRACK_ENABLED  && !MI_TSAN       // note: when tracking, cannot use mi_usable_size with multi-threading
    memset(block, MI_DEBUG_FREED, mi_usable_size(block));
    #endif // 4906
  }

  // and finally free the actual block by pushing it on the owning heap
  // thread_delayed free list (or heap delayed free list)
  mi_free_block_delayed_mt(page,block);
}


// ------------------------------------------------------
// Usable size
// ------------------------------------------------------

// Bytes available in a block
static size_t mi_decl_noinline mi_page_usable_aligned_size_of(const mi_page_t* page, const void* p) mi_attr_noexcept {
  const mi_block_t* block = _mi_page_ptr_unalign(page, p);
  const size_t size = mi_page_usable_size_of(page, block);
  const ptrdiff_t adjust = (uint8_t*)p - (uint8_t*)block;
  mi_assert_internal(adjust >= 0 && (size_t)adjust <= size);
  const size_t aligned_size = (size - adjust);
  #if MI_GUARDED
  if (mi_block_ptr_is_guarded(block, p)) {
    return aligned_size - _mi_os_page_size();
  }
  #endif // 4928
  return aligned_size;
}

static inline size_t _mi_usable_size(const void* p, const char* msg) mi_attr_noexcept {
  const mi_segment_t* const segment = mi_checked_ptr_segment(p, msg);
  if mi_unlikely(segment==NULL) return 0;
  const mi_page_t* const page = _mi_segment_page_of(segment, p);
  if mi_likely(!mi_page_has_aligned(page)) {
    const mi_block_t* block = (const mi_block_t*)p;
    return mi_page_usable_size_of(page, block);
  }
  else {
    // split out to separate routine for improved code generation
    return mi_page_usable_aligned_size_of(page, p);
  }
}

mi_decl_nodiscard size_t mi_usable_size(const void* p) mi_attr_noexcept {
  return _mi_usable_size(p, "mi_usable_size");
}


// ------------------------------------------------------
// Free variants
// ------------------------------------------------------

void mi_free_size(void* p, size_t size) mi_attr_noexcept {
  MI_UNUSED_RELEASE(size);
  mi_assert(p == NULL || size <= _mi_usable_size(p,"mi_free_size"));
  mi_free(p);
}

void mi_free_size_aligned(void* p, size_t size, size_t alignment) mi_attr_noexcept {
  MI_UNUSED_RELEASE(alignment);
  mi_assert(((uintptr_t)p % alignment) == 0);
  mi_free_size(p,size);
}

void mi_free_aligned(void* p, size_t alignment) mi_attr_noexcept {
  MI_UNUSED_RELEASE(alignment);
  mi_assert(((uintptr_t)p % alignment) == 0);
  mi_free(p);
}


// ------------------------------------------------------
// Check for double free in secure and debug mode
// This is somewhat expensive so only enabled for secure mode 4
// ------------------------------------------------------

#if (MI_ENCODE_FREELIST && (MI_SECURE>=4 || MI_DEBUG!=0))
// linear check if the free list contains a specific element
static bool mi_list_contains(const mi_page_t* page, const mi_block_t* list, const mi_block_t* elem) {
  while (list != NULL) {
    if (elem==list) return true;
    list = mi_block_next(page, list);
  }
  return false;
}

static mi_decl_noinline bool mi_check_is_double_freex(const mi_page_t* page, const mi_block_t* block) {
  // The decoded value is in the same page (or NULL).
  // Walk the free lists to verify positively if it is already freed
  if (mi_list_contains(page, page->free, block) ||
      mi_list_contains(page, page->local_free, block) ||
      mi_list_contains(page, mi_page_thread_free(page), block))
  {
    _mi_error_message(EAGAIN, "double free detected of block %p with size %zu\n", block, mi_page_block_size(page));
    return true;
  }
  return false;
}

#define mi_track_page(page,access)  { size_t psize; void* pstart = _mi_page_start(_mi_page_segment(page),page,&psize); mi_track_mem_##access( pstart, psize); }

static inline bool mi_check_is_double_free(const mi_page_t* page, const mi_block_t* block) {
  bool is_double_free = false;
  mi_block_t* n = mi_block_nextx(page, block, page->keys); // pretend it is freed, and get the decoded first field
  if (((uintptr_t)n & (MI_INTPTR_SIZE-1))==0 &&  // quick check: aligned pointer?
      (n==NULL || mi_is_in_same_page(block, n))) // quick check: in same page or NULL?
  {
    // Suspicious: decoded value a in block is in the same page (or NULL) -- maybe a double free?
    // (continue in separate function to improve code generation)
    is_double_free = mi_check_is_double_freex(page, block);
  }
  return is_double_free;
}
#else
static inline bool mi_check_is_double_free(const mi_page_t* page, const mi_block_t* block) {
  MI_UNUSED(page);
  MI_UNUSED(block);
  return false;
}
#endif // 4983


// ---------------------------------------------------------------------------
// Check for heap block overflow by setting up padding at the end of the block
// ---------------------------------------------------------------------------

#if MI_PADDING // && !MI_TRACK_ENABLED
static bool mi_page_decode_padding(const mi_page_t* page, const mi_block_t* block, size_t* delta, size_t* bsize) {
  *bsize = mi_page_usable_block_size(page);
  const mi_padding_t* const padding = (mi_padding_t*)((uint8_t*)block + *bsize);
  mi_track_mem_defined(padding,sizeof(mi_padding_t));
  *delta = padding->delta;
  uint32_t canary = padding->canary;
  uintptr_t keys[2];
  keys[0] = page->keys[0];
  keys[1] = page->keys[1];
  bool ok = (mi_ptr_encode_canary(page,block,keys) == canary && *delta <= *bsize);
  mi_track_mem_noaccess(padding,sizeof(mi_padding_t));
  return ok;
}

// Return the exact usable size of a block.
static size_t mi_page_usable_size_of(const mi_page_t* page, const mi_block_t* block) {
  size_t bsize;
  size_t delta;
  bool ok = mi_page_decode_padding(page, block, &delta, &bsize);
  mi_assert_internal(ok); mi_assert_internal(delta <= bsize);
  return (ok ? bsize - delta : 0);
}

// When a non-thread-local block is freed, it becomes part of the thread delayed free
// list that is freed later by the owning heap. If the exact usable size is too small to
// contain the pointer for the delayed list, then shrink the padding (by decreasing delta)
// so it will later not trigger an overflow error in `mi_free_block`.
void _mi_padding_shrink(const mi_page_t* page, const mi_block_t* block, const size_t min_size) {
  size_t bsize;
  size_t delta;
  bool ok = mi_page_decode_padding(page, block, &delta, &bsize);
  mi_assert_internal(ok);
  if (!ok || (bsize - delta) >= min_size) return;  // usually already enough space
  mi_assert_internal(bsize >= min_size);
  if (bsize < min_size) return;  // should never happen
  size_t new_delta = (bsize - min_size);
  mi_assert_internal(new_delta < bsize);
  mi_padding_t* padding = (mi_padding_t*)((uint8_t*)block + bsize);
  mi_track_mem_defined(padding,sizeof(mi_padding_t));
  padding->delta = (uint32_t)new_delta;
  mi_track_mem_noaccess(padding,sizeof(mi_padding_t));
}
#else
static size_t mi_page_usable_size_of(const mi_page_t* page, const mi_block_t* block) {
  MI_UNUSED(block);
  return mi_page_usable_block_size(page);
}

void _mi_padding_shrink(const mi_page_t* page, const mi_block_t* block, const size_t min_size) {
  MI_UNUSED(page);
  MI_UNUSED(block);
  MI_UNUSED(min_size);
}
#endif // 5033

#if MI_PADDING && MI_PADDING_CHECK

static bool mi_verify_padding(const mi_page_t* page, const mi_block_t* block, size_t* size, size_t* wrong) {
  size_t bsize;
  size_t delta;
  bool ok = mi_page_decode_padding(page, block, &delta, &bsize);
  *size = *wrong = bsize;
  if (!ok) return false;
  mi_assert_internal(bsize >= delta);
  *size = bsize - delta;
  if (!mi_page_is_huge(page)) {
    uint8_t* fill = (uint8_t*)block + bsize - delta;
    const size_t maxpad = (delta > MI_MAX_ALIGN_SIZE ? MI_MAX_ALIGN_SIZE : delta); // check at most the first N padding bytes
    mi_track_mem_defined(fill, maxpad);
    for (size_t i = 0; i < maxpad; i++) {
      if (fill[i] != MI_DEBUG_PADDING) {
        *wrong = bsize - delta + i;
        ok = false;
        break;
      }
    }
    mi_track_mem_noaccess(fill, maxpad);
  }
  return ok;
}

static void mi_check_padding(const mi_page_t* page, const mi_block_t* block) {
  size_t size;
  size_t wrong;
  if (!mi_verify_padding(page,block,&size,&wrong)) {
    _mi_error_message(EFAULT, "buffer overflow in heap block %p of size %zu: write after %zu bytes\n", block, size, wrong );
  }
}

#else

static void mi_check_padding(const mi_page_t* page, const mi_block_t* block) {
  MI_UNUSED(page);
  MI_UNUSED(block);
}

#endif // 5089

// only maintain stats for smaller objects if requested
#if (MI_STAT>0)
static void mi_stat_free(const mi_page_t* page, const mi_block_t* block) {
#if (MI_STAT < 2)
  MI_UNUSED(block);
#endif // 5135
  mi_heap_t* const heap = mi_heap_get_default();
  const size_t bsize = mi_page_usable_block_size(page);
#if (MI_STAT>1)
  const size_t usize = mi_page_usable_size_of(page, block);
  mi_heap_stat_decrease(heap, malloc, usize);
#endif // 5140
  if (bsize <= MI_LARGE_OBJ_SIZE_MAX) {
    mi_heap_stat_decrease(heap, normal, bsize);
#if (MI_STAT > 1)
    mi_heap_stat_decrease(heap, normal_bins[_mi_bin(bsize)], 1);
#endif // 5146
  }
  else {
    const size_t bpsize = mi_page_block_size(page);  // match stat in page.c:mi_huge_page_alloc
    mi_heap_stat_decrease(heap, huge, bpsize);
  }
}
#else
static void mi_stat_free(const mi_page_t* page, const mi_block_t* block) {
  MI_UNUSED(page); MI_UNUSED(block);
}
#endif // 5133


// Remove guard page when building with MI_GUARDED
#if MI_GUARDED
static void mi_block_unguard(mi_page_t* page, mi_block_t* block, void* p) {
  MI_UNUSED(p);
  mi_assert_internal(mi_block_ptr_is_guarded(block, p));
  mi_assert_internal(mi_page_has_aligned(page));
  mi_assert_internal((uint8_t*)p - (uint8_t*)block >= (ptrdiff_t)sizeof(mi_block_t));
  mi_assert_internal(block->next == MI_BLOCK_TAG_GUARDED);

  const size_t bsize = mi_page_block_size(page);
  const size_t psize = _mi_os_page_size();
  mi_assert_internal(bsize > psize);
  mi_assert_internal(_mi_page_segment(page)->allow_decommit);
  void* gpage = (uint8_t*)block + bsize - psize;
  mi_assert_internal(_mi_is_aligned(gpage, psize));
  _mi_os_unprotect(gpage, psize);
}
#endif // 5163
// --- Amalgamation END --- mimalloc/src/free.c

#undef MI_IN_ALLOC_C

// ------------------------------------------------------
// Allocation
// ------------------------------------------------------

// Fast allocation in a page: just pop from the free list.
// Fall back to generic allocation only if the list is empty.
// Note: in release mode the (inlined) routine is about 7 instructions with a single test.
extern inline void* _mi_page_malloc_zero(mi_heap_t* heap, mi_page_t* page, size_t size, bool zero) mi_attr_noexcept
{
  mi_assert_internal(page->block_size == 0 /* empty heap */ || mi_page_block_size(page) >= size);

  // check the free list
  mi_block_t* const block = page->free;
  if mi_unlikely(block == NULL) {
    return _mi_malloc_generic(heap, size, zero, 0);
  }
  mi_assert_internal(block != NULL && _mi_ptr_page(block) == page);

  // pop from the free list
  page->free = mi_block_next(page, block);
  page->used++;
  mi_assert_internal(page->free == NULL || _mi_ptr_page(page->free) == page);
  mi_assert_internal(page->block_size < MI_MAX_ALIGN_SIZE || _mi_is_aligned(block, MI_MAX_ALIGN_SIZE));

  #if MI_DEBUG>3
  if (page->free_is_zero && size > sizeof(*block)) {
    mi_assert_expensive(mi_mem_is_zero(block+1,size - sizeof(*block)));
  }
  #endif // 5208

  // allow use of the block internally
  // note: when tracking we need to avoid ever touching the MI_PADDING since
  // that is tracked by valgrind etc. as non-accessible (through the red-zone, see `mimalloc/track.h`)
  mi_track_mem_undefined(block, mi_page_usable_block_size(page));

  // zero the block? note: we need to zero the full block size (issue #63)
  if mi_unlikely(zero) {
    mi_assert_internal(page->block_size != 0); // do not call with zero'ing for huge blocks (see _mi_malloc_generic)
    mi_assert_internal(!mi_page_is_huge(page));
    #if MI_PADDING
    mi_assert_internal(page->block_size >= MI_PADDING_SIZE);
    #endif // 5223
    if (page->free_is_zero) {
      block->next = 0;
      mi_track_mem_defined(block, page->block_size - MI_PADDING_SIZE);
    }
    else {
      _mi_memzero_aligned(block, page->block_size - MI_PADDING_SIZE);
    }
  }

  #if (MI_DEBUG>0) && !MI_TRACK_ENABLED && !MI_TSAN
  if (!zero && !mi_page_is_huge(page)) {
    memset(block, MI_DEBUG_UNINIT, mi_page_usable_block_size(page));
  }
  #elif (MI_SECURE!=0)
  if (!zero) { block->next = 0; } // don't leak internal data
  #endif // 5235

  #if (MI_STAT>0)
  const size_t bsize = mi_page_usable_block_size(page);
  if (bsize <= MI_LARGE_OBJ_SIZE_MAX) {
    mi_heap_stat_increase(heap, normal, bsize);
    mi_heap_stat_counter_increase(heap, normal_count, 1);
    #if (MI_STAT>1)
    const size_t bin = _mi_bin(bsize);
    mi_heap_stat_increase(heap, normal_bins[bin], 1);
    #endif // 5248
  }
  #endif // 5243

  #if MI_PADDING // && !MI_TRACK_ENABLED
    mi_padding_t* const padding = (mi_padding_t*)((uint8_t*)block + mi_page_usable_block_size(page));
    ptrdiff_t delta = ((uint8_t*)padding - (uint8_t*)block - (size - MI_PADDING_SIZE));
    #if (MI_DEBUG>=2)
    mi_assert_internal(delta >= 0 && mi_page_usable_block_size(page) >= (size - MI_PADDING_SIZE + delta));
    #endif // 5258
    mi_track_mem_defined(padding,sizeof(mi_padding_t));  // note: re-enable since mi_page_usable_block_size may set noaccess
    padding->canary = mi_ptr_encode_canary(page,block,page->keys);
    padding->delta  = (uint32_t)(delta);
    #if MI_PADDING_CHECK
    if (!mi_page_is_huge(page)) {
      uint8_t* fill = (uint8_t*)padding - delta;
      const size_t maxpad = (delta > MI_MAX_ALIGN_SIZE ? MI_MAX_ALIGN_SIZE : delta); // set at most N initial padding bytes
      for (size_t i = 0; i < maxpad; i++) { fill[i] = MI_DEBUG_PADDING; }
    }
    #endif // 5264
  #endif // 5255

  return block;
}

// extra entries for improved efficiency in `alloc-aligned.c`.
extern void* _mi_page_malloc(mi_heap_t* heap, mi_page_t* page, size_t size) mi_attr_noexcept {
  return _mi_page_malloc_zero(heap,page,size,false);
}
extern void* _mi_page_malloc_zeroed(mi_heap_t* heap, mi_page_t* page, size_t size) mi_attr_noexcept {
  return _mi_page_malloc_zero(heap,page,size,true);
}

#if MI_GUARDED
mi_decl_restrict void* _mi_heap_malloc_guarded(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept;
#endif // 5284

static inline mi_decl_restrict void* mi_heap_malloc_small_zero(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept {
  mi_assert(heap != NULL);
  mi_assert(size <= MI_SMALL_SIZE_MAX);
  #if MI_DEBUG
  const uintptr_t tid = _mi_thread_id();
  mi_assert(heap->thread_id == 0 || heap->thread_id == tid); // heaps are thread local
  #endif // 5291
  #if (MI_PADDING || MI_GUARDED)
  if (size == 0) { size = sizeof(void*); }
  #endif // 5295
  #if MI_GUARDED
  if (mi_heap_malloc_use_guarded(heap,size)) {
    return _mi_heap_malloc_guarded(heap, size, zero);
  }
  #endif // 5298

  // get page in constant time, and allocate from it
  mi_page_t* page = _mi_heap_get_free_small_page(heap, size + MI_PADDING_SIZE);
  void* const p = _mi_page_malloc_zero(heap, page, size + MI_PADDING_SIZE, zero);
  mi_track_malloc(p,size,zero);

  #if MI_STAT>1
  if (p != NULL) {
    if (!mi_heap_is_initialized(heap)) { heap = mi_prim_get_default_heap(); }
    mi_heap_stat_increase(heap, malloc, mi_usable_size(p));
  }
  #endif // 5309
  #if MI_DEBUG>3
  if (p != NULL && zero) {
    mi_assert_expensive(mi_mem_is_zero(p, size));
  }
  #endif // 5315
  return p;
}

// allocate a small block
mi_decl_nodiscard extern inline mi_decl_restrict void* mi_heap_malloc_small(mi_heap_t* heap, size_t size) mi_attr_noexcept {
  return mi_heap_malloc_small_zero(heap, size, false);
}

mi_decl_nodiscard extern inline mi_decl_restrict void* mi_malloc_small(size_t size) mi_attr_noexcept {
  return mi_heap_malloc_small(mi_prim_get_default_heap(), size);
}

// The main allocation function
extern inline void* _mi_heap_malloc_zero_ex(mi_heap_t* heap, size_t size, bool zero, size_t huge_alignment) mi_attr_noexcept {
  // fast path for small objects
  if mi_likely(size <= MI_SMALL_SIZE_MAX) {
    mi_assert_internal(huge_alignment == 0);
    return mi_heap_malloc_small_zero(heap, size, zero);
  }
  #if MI_GUARDED
  else if (huge_alignment==0 && mi_heap_malloc_use_guarded(heap,size)) {
    return _mi_heap_malloc_guarded(heap, size, zero);
  }
  #endif // 5339
  else {
    // regular allocation
    mi_assert(heap!=NULL);
    mi_assert(heap->thread_id == 0 || heap->thread_id == _mi_thread_id());   // heaps are thread local
    void* const p = _mi_malloc_generic(heap, size + MI_PADDING_SIZE, zero, huge_alignment);  // note: size can overflow but it is detected in malloc_generic
    mi_track_malloc(p,size,zero);

    #if MI_STAT>1
    if (p != NULL) {
      if (!mi_heap_is_initialized(heap)) { heap = mi_prim_get_default_heap(); }
      mi_heap_stat_increase(heap, malloc, mi_usable_size(p));
    }
    #endif // 5351
    #if MI_DEBUG>3
    if (p != NULL && zero) {
      mi_assert_expensive(mi_mem_is_zero(p, size));
    }
    #endif // 5357
    return p;
  }
}

extern inline void* _mi_heap_malloc_zero(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept {
  return _mi_heap_malloc_zero_ex(heap, size, zero, 0);
}

mi_decl_nodiscard extern inline mi_decl_restrict void* mi_heap_malloc(mi_heap_t* heap, size_t size) mi_attr_noexcept {
  return _mi_heap_malloc_zero(heap, size, false);
}

mi_decl_nodiscard extern inline mi_decl_restrict void* mi_malloc(size_t size) mi_attr_noexcept {
  return mi_heap_malloc(mi_prim_get_default_heap(), size);
}

// zero initialized small block
mi_decl_nodiscard mi_decl_restrict void* mi_zalloc_small(size_t size) mi_attr_noexcept {
  return mi_heap_malloc_small_zero(mi_prim_get_default_heap(), size, true);
}

mi_decl_nodiscard extern inline mi_decl_restrict void* mi_heap_zalloc(mi_heap_t* heap, size_t size) mi_attr_noexcept {
  return _mi_heap_malloc_zero(heap, size, true);
}

mi_decl_nodiscard mi_decl_restrict void* mi_zalloc(size_t size) mi_attr_noexcept {
  return mi_heap_zalloc(mi_prim_get_default_heap(),size);
}


mi_decl_nodiscard extern inline mi_decl_restrict void* mi_heap_calloc(mi_heap_t* heap, size_t count, size_t size) mi_attr_noexcept {
  size_t total;
  if (mi_count_size_overflow(count,size,&total)) return NULL;
  return mi_heap_zalloc(heap,total);
}

mi_decl_nodiscard mi_decl_restrict void* mi_calloc(size_t count, size_t size) mi_attr_noexcept {
  return mi_heap_calloc(mi_prim_get_default_heap(),count,size);
}

// Uninitialized `calloc`
mi_decl_nodiscard extern mi_decl_restrict void* mi_heap_mallocn(mi_heap_t* heap, size_t count, size_t size) mi_attr_noexcept {
  size_t total;
  if (mi_count_size_overflow(count, size, &total)) return NULL;
  return mi_heap_malloc(heap, total);
}

mi_decl_nodiscard mi_decl_restrict void* mi_mallocn(size_t count, size_t size) mi_attr_noexcept {
  return mi_heap_mallocn(mi_prim_get_default_heap(),count,size);
}

// Expand (or shrink) in place (or fail)
void* mi_expand(void* p, size_t newsize) mi_attr_noexcept {
  #if MI_PADDING
  // we do not shrink/expand with padding enabled
  MI_UNUSED(p); MI_UNUSED(newsize);
  return NULL;
  #else
  if (p == NULL) return NULL;
  const size_t size = _mi_usable_size(p,"mi_expand");
  if (newsize > size) return NULL;
  return p; // it fits
  #endif // 5415
}

void* _mi_heap_realloc_zero(mi_heap_t* heap, void* p, size_t newsize, bool zero) mi_attr_noexcept {
  // if p == NULL then behave as malloc.
  // else if size == 0 then reallocate to a zero-sized block (and don't return NULL, just as mi_malloc(0)).
  // (this means that returning NULL always indicates an error, and `p` will not have been freed in that case.)
  const size_t size = _mi_usable_size(p,"mi_realloc"); // also works if p == NULL (with size 0)
  if mi_unlikely(newsize <= size && newsize >= (size / 2) && newsize > 0) {  // note: newsize must be > 0 or otherwise we return NULL for realloc(NULL,0)
    mi_assert_internal(p!=NULL);
    // todo: do not track as the usable size is still the same in the free; adjust potential padding?
    // mi_track_resize(p,size,newsize)
    // if (newsize < size) { mi_track_mem_noaccess((uint8_t*)p + newsize, size - newsize); }
    return p;  // reallocation still fits and not more than 50% waste
  }
  void* newp = mi_heap_malloc(heap,newsize);
  if mi_likely(newp != NULL) {
    if (zero && newsize > size) {
      // also set last word in the previous allocation to zero to ensure any padding is zero-initialized
      const size_t start = (size >= sizeof(intptr_t) ? size - sizeof(intptr_t) : 0);
      _mi_memzero((uint8_t*)newp + start, newsize - start);
    }
    else if (newsize == 0) {
      ((uint8_t*)newp)[0] = 0; // work around for applications that expect zero-reallocation to be zero initialized (issue #725)
    }
    if mi_likely(p != NULL) {
      const size_t copysize = (newsize > size ? size : newsize);
      mi_track_mem_defined(p,copysize);  // _mi_useable_size may be too large for byte precise memory tracking..
      _mi_memcpy(newp, p, copysize);
      mi_free(p); // only free the original pointer if successful
    }
  }
  return newp;
}

mi_decl_nodiscard void* mi_heap_realloc(mi_heap_t* heap, void* p, size_t newsize) mi_attr_noexcept {
  return _mi_heap_realloc_zero(heap, p, newsize, false);
}

mi_decl_nodiscard void* mi_heap_reallocn(mi_heap_t* heap, void* p, size_t count, size_t size) mi_attr_noexcept {
  size_t total;
  if (mi_count_size_overflow(count, size, &total)) return NULL;
  return mi_heap_realloc(heap, p, total);
}


// Reallocate but free `p` on errors
mi_decl_nodiscard void* mi_heap_reallocf(mi_heap_t* heap, void* p, size_t newsize) mi_attr_noexcept {
  void* newp = mi_heap_realloc(heap, p, newsize);
  if (newp==NULL && p!=NULL) mi_free(p);
  return newp;
}

mi_decl_nodiscard void* mi_heap_rezalloc(mi_heap_t* heap, void* p, size_t newsize) mi_attr_noexcept {
  return _mi_heap_realloc_zero(heap, p, newsize, true);
}

mi_decl_nodiscard void* mi_heap_recalloc(mi_heap_t* heap, void* p, size_t count, size_t size) mi_attr_noexcept {
  size_t total;
  if (mi_count_size_overflow(count, size, &total)) return NULL;
  return mi_heap_rezalloc(heap, p, total);
}


mi_decl_nodiscard void* mi_realloc(void* p, size_t newsize) mi_attr_noexcept {
  return mi_heap_realloc(mi_prim_get_default_heap(),p,newsize);
}

mi_decl_nodiscard void* mi_reallocn(void* p, size_t count, size_t size) mi_attr_noexcept {
  return mi_heap_reallocn(mi_prim_get_default_heap(),p,count,size);
}

// Reallocate but free `p` on errors
mi_decl_nodiscard void* mi_reallocf(void* p, size_t newsize) mi_attr_noexcept {
  return mi_heap_reallocf(mi_prim_get_default_heap(),p,newsize);
}

mi_decl_nodiscard void* mi_rezalloc(void* p, size_t newsize) mi_attr_noexcept {
  return mi_heap_rezalloc(mi_prim_get_default_heap(), p, newsize);
}

mi_decl_nodiscard void* mi_recalloc(void* p, size_t count, size_t size) mi_attr_noexcept {
  return mi_heap_recalloc(mi_prim_get_default_heap(), p, count, size);
}



// ------------------------------------------------------
// strdup, strndup, and realpath
// ------------------------------------------------------

// `strdup` using mi_malloc
mi_decl_nodiscard mi_decl_restrict char* mi_heap_strdup(mi_heap_t* heap, const char* s) mi_attr_noexcept {
  if (s == NULL) return NULL;
  size_t len = _mi_strlen(s);
  char* t = (char*)mi_heap_malloc(heap,len+1);
  if (t == NULL) return NULL;
  _mi_memcpy(t, s, len);
  t[len] = 0;
  return t;
}

mi_decl_nodiscard mi_decl_restrict char* mi_strdup(const char* s) mi_attr_noexcept {
  return mi_heap_strdup(mi_prim_get_default_heap(), s);
}

// `strndup` using mi_malloc
mi_decl_nodiscard mi_decl_restrict char* mi_heap_strndup(mi_heap_t* heap, const char* s, size_t n) mi_attr_noexcept {
  if (s == NULL) return NULL;
  const size_t len = _mi_strnlen(s,n);  // len <= n
  char* t = (char*)mi_heap_malloc(heap, len+1);
  if (t == NULL) return NULL;
  _mi_memcpy(t, s, len);
  t[len] = 0;
  return t;
}

mi_decl_nodiscard mi_decl_restrict char* mi_strndup(const char* s, size_t n) mi_attr_noexcept {
  return mi_heap_strndup(mi_prim_get_default_heap(),s,n);
}

#ifndef __wasi__
// `realpath` using mi_malloc
#ifdef _WIN32
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif // 5548

mi_decl_nodiscard mi_decl_restrict char* mi_heap_realpath(mi_heap_t* heap, const char* fname, char* resolved_name) mi_attr_noexcept {
  // todo: use GetFullPathNameW to allow longer file names
  char buf[PATH_MAX];
  DWORD res = GetFullPathNameA(fname, PATH_MAX, (resolved_name == NULL ? buf : resolved_name), NULL);
  if (res == 0) {
    errno = GetLastError(); return NULL;
  }
  else if (res > PATH_MAX) {
    errno = EINVAL; return NULL;
  }
  else if (resolved_name != NULL) {
    return resolved_name;
  }
  else {
    return mi_heap_strndup(heap, buf, PATH_MAX);
  }
}
#else
/*
#include <unistd.h>  // pathconf
static size_t mi_path_max(void) {
  static size_t path_max = 0;
  if (path_max <= 0) {
    long m = pathconf("/",_PC_PATH_MAX);
    if (m <= 0) path_max = 4096;      // guess
    else if (m < 256) path_max = 256; // at least 256
    else path_max = m;
  }
  return path_max;
}
*/
char* mi_heap_realpath(mi_heap_t* heap, const char* fname, char* resolved_name) mi_attr_noexcept {
  if (resolved_name != NULL) {
    return realpath(fname,resolved_name);
  }
  else {
    char* rname = realpath(fname, NULL);
    if (rname == NULL) return NULL;
    char* result = mi_heap_strdup(heap, rname);
    mi_cfree(rname);  // use checked free (which may be redirected to our free but that's ok)
    // note: with ASAN realpath is intercepted and mi_cfree may leak the returned pointer :-(
    return result;
  }
  /*
    const size_t n  = mi_path_max();
    char* buf = (char*)mi_malloc(n+1);
    if (buf == NULL) {
      errno = ENOMEM;
      return NULL;
    }
    char* rname  = realpath(fname,buf);
    char* result = mi_heap_strndup(heap,rname,n); // ok if `rname==NULL`
    mi_free(buf);
    return result;
  }
  */
}
#endif // 5547

mi_decl_nodiscard mi_decl_restrict char* mi_realpath(const char* fname, char* resolved_name) mi_attr_noexcept {
  return mi_heap_realpath(mi_prim_get_default_heap(),fname,resolved_name);
}
#endif // 5545

/*-------------------------------------------------------
C++ new and new_aligned
The standard requires calling into `get_new_handler` and
throwing the bad_alloc exception on failure. If we compile
with a C++ compiler we can implement this precisely. If we
use a C compiler we cannot throw a `bad_alloc` exception
but we call `exit` instead (i.e. not returning).
-------------------------------------------------------*/

#if 0 // #ifdef __cplusplus
// #include <new>
// static bool mi_try_new_handler(bool nothrow) {
#if 0 //   #if defined(_MSC_VER) || (__cplusplus >= 201103L)
//     std::new_handler h = std::get_new_handler();
  #else
//     std::new_handler h = std::set_new_handler();
//     std::set_new_handler(h);
  #endif // 5628
//   if (h==NULL) {
//     _mi_error_message(ENOMEM, "out of memory in 'new'");
#if 0 //     #if defined(_CPPUNWIND) || defined(__cpp_exceptions)  // exceptions are not always enabled
//     if (!nothrow) {
//       throw std::bad_alloc();
//     }
    #else
//     MI_UNUSED(nothrow);
    #endif // 5636
//     return false;
//   }
//   else {
//     h();
//     return true;
//   }
// }
#else
typedef void (*std_new_handler_t)(void);

#if (defined(__GNUC__) || (defined(__clang__) && !defined(_MSC_VER)))  // exclude clang-cl, see issue #631
std_new_handler_t __attribute__((weak)) _ZSt15get_new_handlerv(void) {
  return NULL;
}
static std_new_handler_t mi_get_new_handler(void) {
  return _ZSt15get_new_handlerv();
}
#else
// note: on windows we could dynamically link to `?get_new_handler@std@@YAP6AXXZXZ`.
static std_new_handler_t mi_get_new_handler() {
  return NULL;
}
#endif // 5653

static bool mi_try_new_handler(bool nothrow) {
  std_new_handler_t h = mi_get_new_handler();
  if (h==NULL) {
    _mi_error_message(ENOMEM, "out of memory in 'new'");
    if (!nothrow) {
      abort();  // cannot throw in plain C, use abort
    }
    return false;
  }
  else {
    h();
    return true;
  }
}
#endif // 5625

mi_decl_export mi_decl_noinline void* mi_heap_try_new(mi_heap_t* heap, size_t size, bool nothrow ) {
  void* p = NULL;
  while(p == NULL && mi_try_new_handler(nothrow)) {
    p = mi_heap_malloc(heap,size);
  }
  return p;
}

static mi_decl_noinline void* mi_try_new(size_t size, bool nothrow) {
  return mi_heap_try_new(mi_prim_get_default_heap(), size, nothrow);
}


mi_decl_nodiscard mi_decl_restrict void* mi_heap_alloc_new(mi_heap_t* heap, size_t size) {
  void* p = mi_heap_malloc(heap,size);
  if mi_unlikely(p == NULL) return mi_heap_try_new(heap, size, false);
  return p;
}

mi_decl_nodiscard mi_decl_restrict void* mi_new(size_t size) {
  return mi_heap_alloc_new(mi_prim_get_default_heap(), size);
}


mi_decl_nodiscard mi_decl_restrict void* mi_heap_alloc_new_n(mi_heap_t* heap, size_t count, size_t size) {
  size_t total;
  if mi_unlikely(mi_count_size_overflow(count, size, &total)) {
    mi_try_new_handler(false);  // on overflow we invoke the try_new_handler once to potentially throw std::bad_alloc
    return NULL;
  }
  else {
    return mi_heap_alloc_new(heap,total);
  }
}

mi_decl_nodiscard mi_decl_restrict void* mi_new_n(size_t count, size_t size) {
  return mi_heap_alloc_new_n(mi_prim_get_default_heap(), count, size);
}


mi_decl_nodiscard mi_decl_restrict void* mi_new_nothrow(size_t size) mi_attr_noexcept {
  void* p = mi_malloc(size);
  if mi_unlikely(p == NULL) return mi_try_new(size, true);
  return p;
}

mi_decl_nodiscard mi_decl_restrict void* mi_new_aligned(size_t size, size_t alignment) {
  void* p;
  do {
    p = mi_malloc_aligned(size, alignment);
  }
  while(p == NULL && mi_try_new_handler(false));
  return p;
}

mi_decl_nodiscard mi_decl_restrict void* mi_new_aligned_nothrow(size_t size, size_t alignment) mi_attr_noexcept {
  void* p;
  do {
    p = mi_malloc_aligned(size, alignment);
  }
  while(p == NULL && mi_try_new_handler(true));
  return p;
}

mi_decl_nodiscard void* mi_new_realloc(void* p, size_t newsize) {
  void* q;
  do {
    q = mi_realloc(p, newsize);
  } while (q == NULL && mi_try_new_handler(false));
  return q;
}

mi_decl_nodiscard void* mi_new_reallocn(void* p, size_t newcount, size_t size) {
  size_t total;
  if mi_unlikely(mi_count_size_overflow(newcount, size, &total)) {
    mi_try_new_handler(false);  // on overflow we invoke the try_new_handler once to potentially throw std::bad_alloc
    return NULL;
  }
  else {
    return mi_new_realloc(p, total);
  }
}

#if MI_GUARDED
// We always allocate a guarded allocation at an offset (`mi_page_has_aligned` will be true).
// We then set the first word of the block to `0` for regular offset aligned allocations (in `alloc-aligned.c`)
// and the first word to `~0` for guarded allocations to have a correct `mi_usable_size`

static void* mi_block_ptr_set_guarded(mi_block_t* block, size_t obj_size) {
  // TODO: we can still make padding work by moving it out of the guard page area
  mi_page_t* const page = _mi_ptr_page(block);
  mi_page_set_has_aligned(page, true);
  block->next = MI_BLOCK_TAG_GUARDED;

  // set guard page at the end of the block
  mi_segment_t* const segment = _mi_page_segment(page);
  const size_t block_size = mi_page_block_size(page);  // must use `block_size` to match `mi_free_local`
  const size_t os_page_size = _mi_os_page_size();
  mi_assert_internal(block_size >= obj_size + os_page_size + sizeof(mi_block_t));
  if (block_size < obj_size + os_page_size + sizeof(mi_block_t)) {
    // should never happen
    mi_free(block);
    return NULL;
  }
  uint8_t* guard_page = (uint8_t*)block + block_size - os_page_size;
  mi_assert_internal(_mi_is_aligned(guard_page, os_page_size));
  if (segment->allow_decommit && _mi_is_aligned(guard_page, os_page_size)) {
    _mi_os_protect(guard_page, os_page_size);
  }
  else {
    _mi_warning_message("unable to set a guard page behind an object due to pinned memory (large OS pages?) (object %p of size %zu)\n", block, block_size);
  }

  // align pointer just in front of the guard page
  size_t offset = block_size - os_page_size - obj_size;
  mi_assert_internal(offset > sizeof(mi_block_t));
  if (offset > MI_BLOCK_ALIGNMENT_MAX) {
    // give up to place it right in front of the guard page if the offset is too large for unalignment
    offset = MI_BLOCK_ALIGNMENT_MAX;
  }
  void* p = (uint8_t*)block + offset;  
  mi_track_align(block, p, offset, obj_size);
  mi_track_mem_defined(block, sizeof(mi_block_t));
  return p;
}

mi_decl_restrict void* _mi_heap_malloc_guarded(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept
{
  #if defined(MI_PADDING_SIZE)
  mi_assert(MI_PADDING_SIZE==0);
  #endif // 5811
  // allocate multiple of page size ending in a guard page
  // ensure minimal alignment requirement?
  const size_t os_page_size = _mi_os_page_size();
  const size_t obj_size = (mi_option_is_enabled(mi_option_guarded_precise) ? size : _mi_align_up(size, MI_MAX_ALIGN_SIZE));
  const size_t bsize    = _mi_align_up(_mi_align_up(obj_size, MI_MAX_ALIGN_SIZE) + sizeof(mi_block_t), MI_MAX_ALIGN_SIZE);
  const size_t req_size = _mi_align_up(bsize + os_page_size, os_page_size);
  mi_block_t* const block = (mi_block_t*)_mi_malloc_generic(heap, req_size, zero, 0 /* huge_alignment */);
  if (block==NULL) return NULL;
  void* const p   = mi_block_ptr_set_guarded(block, obj_size);

  // stats
  mi_track_malloc(p, size, zero);  
  if (p != NULL) {
    if (!mi_heap_is_initialized(heap)) { heap = mi_prim_get_default_heap(); }
    #if MI_STAT>1
    mi_heap_stat_increase(heap, malloc, mi_usable_size(p));
    #endif // 5828
    _mi_stat_counter_increase(&heap->tld->stats.guarded_alloc_count, 1);
  }
  #if MI_DEBUG>3
  if (p != NULL && zero) {
    mi_assert_expensive(mi_mem_is_zero(p, size));
  }
  #endif // 5833
  return p;
}
#endif // 5766

// ------------------------------------------------------
// ensure explicit external inline definitions are emitted!
// ------------------------------------------------------

#if 0 // #ifdef __cplusplus
// void* _mi_externs[] = {
//   (void*)&_mi_page_malloc,
//   (void*)&_mi_page_malloc_zero,
//   (void*)&_mi_heap_malloc_zero,
//   (void*)&_mi_heap_malloc_zero_ex,
//   (void*)&mi_malloc,
//   (void*)&mi_malloc_small,
//   (void*)&mi_zalloc_small,
//   (void*)&mi_heap_malloc,
//   (void*)&mi_heap_zalloc,
//   (void*)&mi_heap_malloc_small
//   // (void*)&mi_heap_alloc_new,
//   // (void*)&mi_heap_alloc_new_n
// };
#endif // 5846
// --- Amalgamation END --- mimalloc/src/alloc.c


// --- Amalgamation START --- mimalloc/src/alloc-aligned.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2021, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


#include <string.h>     // memset

// ------------------------------------------------------
// Aligned Allocation
// ------------------------------------------------------

static bool mi_malloc_is_naturally_aligned( size_t size, size_t alignment ) {
  // objects up to `MI_MAX_ALIGN_GUARANTEE` are allocated aligned to their size (see `segment.c:_mi_segment_page_start`).
  mi_assert_internal(_mi_is_power_of_two(alignment) && (alignment > 0));
  if (alignment > size) return false;
  if (alignment <= MI_MAX_ALIGN_SIZE) return true;
  const size_t bsize = mi_good_size(size);
  return (bsize <= MI_MAX_ALIGN_GUARANTEE && (bsize & (alignment-1)) == 0);
}

#if MI_GUARDED
static mi_decl_restrict void* mi_heap_malloc_guarded_aligned(mi_heap_t* heap, size_t size, size_t alignment, bool zero) mi_attr_noexcept {
  // use over allocation for guarded blocksl
  mi_assert_internal(alignment > 0 && alignment < MI_BLOCK_ALIGNMENT_MAX);
  const size_t oversize = size + alignment - 1;
  void* base = _mi_heap_malloc_guarded(heap, oversize, zero);
  void* p = mi_align_up_ptr(base, alignment);
  mi_track_align(base, p, (uint8_t*)p - (uint8_t*)base, size);
  mi_assert_internal(mi_usable_size(p) >= size);
  mi_assert_internal(_mi_is_aligned(p, alignment));
  return p;
}

static void* mi_heap_malloc_zero_no_guarded(mi_heap_t* heap, size_t size, bool zero) {
  const size_t rate = heap->guarded_sample_rate;
  heap->guarded_sample_rate = 0;
  void* p = _mi_heap_malloc_zero(heap, size, zero);
  heap->guarded_sample_rate = rate;
  return p;
}
#else
static void* mi_heap_malloc_zero_no_guarded(mi_heap_t* heap, size_t size, bool zero) {
  return _mi_heap_malloc_zero(heap, size, zero);
}
#endif // 5901

// Fallback aligned allocation that over-allocates -- split out for better codegen
static mi_decl_noinline void* mi_heap_malloc_zero_aligned_at_overalloc(mi_heap_t* const heap, const size_t size, const size_t alignment, const size_t offset, const bool zero) mi_attr_noexcept
{
  mi_assert_internal(size <= (MI_MAX_ALLOC_SIZE - MI_PADDING_SIZE));
  mi_assert_internal(alignment != 0 && _mi_is_power_of_two(alignment));

  void* p;
  size_t oversize;
  if mi_unlikely(alignment > MI_BLOCK_ALIGNMENT_MAX) {
    // use OS allocation for very large alignment and allocate inside a huge page (dedicated segment with 1 page)
    // This can support alignments >= MI_SEGMENT_SIZE by ensuring the object can be aligned at a point in the
    // first (and single) page such that the segment info is `MI_SEGMENT_SIZE` bytes before it (so it can be found by aligning the pointer down)
    if mi_unlikely(offset != 0) {
      // todo: cannot support offset alignment for very large alignments yet
#if MI_DEBUG > 0
      _mi_error_message(EOVERFLOW, "aligned allocation with a very large alignment cannot be used with an alignment offset (size %zu, alignment %zu, offset %zu)\n", size, alignment, offset);
#endif // 5941
      return NULL;
    }
    oversize = (size <= MI_SMALL_SIZE_MAX ? MI_SMALL_SIZE_MAX + 1 /* ensure we use generic malloc path */ : size);
    // note: no guarded as alignment > 0
    p = _mi_heap_malloc_zero_ex(heap, oversize, false, alignment); // the page block size should be large enough to align in the single huge page block
    // zero afterwards as only the area from the aligned_p may be committed!
    if (p == NULL) return NULL;
  }
  else {
    // otherwise over-allocate
    oversize = size + alignment - 1;
    p = mi_heap_malloc_zero_no_guarded(heap, oversize, zero);
    if (p == NULL) return NULL;
  }
  mi_page_t* page = _mi_ptr_page(p);

  // .. and align within the allocation
  const uintptr_t align_mask = alignment - 1;  // for any x, `(x & align_mask) == (x % alignment)`
  const uintptr_t poffset = ((uintptr_t)p + offset) & align_mask;
  const uintptr_t adjust  = (poffset == 0 ? 0 : alignment - poffset);
  mi_assert_internal(adjust < alignment);
  void* aligned_p = (void*)((uintptr_t)p + adjust);
  if (aligned_p != p) {
    mi_page_set_has_aligned(page, true);
    #if MI_GUARDED
    // set tag to aligned so mi_usable_size works with guard pages
    if (adjust >= sizeof(mi_block_t)) {
      mi_block_t* const block = (mi_block_t*)p;
      block->next = MI_BLOCK_TAG_ALIGNED;
    }
    #endif // 5968
    _mi_padding_shrink(page, (mi_block_t*)p, adjust + size);
  }
  // todo: expand padding if overallocated ?

  mi_assert_internal(mi_page_usable_block_size(page) >= adjust + size);
  mi_assert_internal(((uintptr_t)aligned_p + offset) % alignment == 0);
  mi_assert_internal(mi_usable_size(aligned_p)>=size);
  mi_assert_internal(mi_usable_size(p) == mi_usable_size(aligned_p)+adjust);
  #if MI_DEBUG > 1
  mi_page_t* const apage = _mi_ptr_page(aligned_p);
  void* unalign_p = _mi_page_ptr_unalign(apage, aligned_p);
  mi_assert_internal(p == unalign_p);
  #endif // 5983

  // now zero the block if needed
  if (alignment > MI_BLOCK_ALIGNMENT_MAX) {
    // for the tracker, on huge aligned allocations only from the start of the large block is defined
    mi_track_mem_undefined(aligned_p, size);
    if (zero) {
      _mi_memzero_aligned(aligned_p, mi_usable_size(aligned_p));
    }
  }

  if (p != aligned_p) {
    mi_track_align(p,aligned_p,adjust,mi_usable_size(aligned_p));
    #if MI_GUARDED
    mi_track_mem_defined(p, sizeof(mi_block_t));
    #endif // 6000
  }
  return aligned_p;
}

// Generic primitive aligned allocation -- split out for better codegen
static mi_decl_noinline void* mi_heap_malloc_zero_aligned_at_generic(mi_heap_t* const heap, const size_t size, const size_t alignment, const size_t offset, const bool zero) mi_attr_noexcept
{
  mi_assert_internal(alignment != 0 && _mi_is_power_of_two(alignment));
  // we don't allocate more than MI_MAX_ALLOC_SIZE (see <https://sourceware.org/ml/libc-announce/2019/msg00001.html>)
  if mi_unlikely(size > (MI_MAX_ALLOC_SIZE - MI_PADDING_SIZE)) {
    #if MI_DEBUG > 0
    _mi_error_message(EOVERFLOW, "aligned allocation request is too large (size %zu, alignment %zu)\n", size, alignment);
    #endif // 6013
    return NULL;
  }

  // use regular allocation if it is guaranteed to fit the alignment constraints.
  // this is important to try as the fast path in `mi_heap_malloc_zero_aligned` only works when there exist
  // a page with the right block size, and if we always use the over-alloc fallback that would never happen.
  if (offset == 0 && mi_malloc_is_naturally_aligned(size,alignment)) {
    void* p = mi_heap_malloc_zero_no_guarded(heap, size, zero);
    mi_assert_internal(p == NULL || ((uintptr_t)p % alignment) == 0);
    const bool is_aligned_or_null = (((uintptr_t)p) & (alignment-1))==0;
    if mi_likely(is_aligned_or_null) {
      return p;
    }
    else {
      // this should never happen if the `mi_malloc_is_naturally_aligned` check is correct..
      mi_assert(false);
      mi_free(p);
    }
  }

  // fall back to over-allocation
  return mi_heap_malloc_zero_aligned_at_overalloc(heap,size,alignment,offset,zero);
}


// Primitive aligned allocation
static void* mi_heap_malloc_zero_aligned_at(mi_heap_t* const heap, const size_t size, const size_t alignment, const size_t offset, const bool zero) mi_attr_noexcept
{
  // note: we don't require `size > offset`, we just guarantee that the address at offset is aligned regardless of the allocated size.
  if mi_unlikely(alignment == 0 || !_mi_is_power_of_two(alignment)) { // require power-of-two (see <https://en.cppreference.com/w/c/memory/aligned_alloc>)
    #if MI_DEBUG > 0
    _mi_error_message(EOVERFLOW, "aligned allocation requires the alignment to be a power-of-two (size %zu, alignment %zu)\n", size, alignment);
    #endif // 6046
    return NULL;
  }

  #if MI_GUARDED
  if (offset==0 && alignment < MI_BLOCK_ALIGNMENT_MAX && mi_heap_malloc_use_guarded(heap,size)) {
    return mi_heap_malloc_guarded_aligned(heap, size, alignment, zero);
  }
  #endif // 6052

  // try first if there happens to be a small block available with just the right alignment
  if mi_likely(size <= MI_SMALL_SIZE_MAX && alignment <= size) {
    const uintptr_t align_mask = alignment-1;       // for any x, `(x & align_mask) == (x % alignment)`
    const size_t padsize = size + MI_PADDING_SIZE;
    mi_page_t* page = _mi_heap_get_free_small_page(heap, padsize);
    if mi_likely(page->free != NULL) {
      const bool is_aligned = (((uintptr_t)page->free + offset) & align_mask)==0;
      if mi_likely(is_aligned)
      {
        #if MI_STAT>1
        mi_heap_stat_increase(heap, malloc, size);
        #endif // 6067
        void* p = (zero ? _mi_page_malloc_zeroed(heap,page,padsize) : _mi_page_malloc(heap,page,padsize)); // call specific page malloc for better codegen
        mi_assert_internal(p != NULL);
        mi_assert_internal(((uintptr_t)p + offset) % alignment == 0);
        mi_track_malloc(p,size,zero);
        return p;
      }
    }
  }

  // fallback to generic aligned allocation
  return mi_heap_malloc_zero_aligned_at_generic(heap, size, alignment, offset, zero);
}


// ------------------------------------------------------
// Optimized mi_heap_malloc_aligned / mi_malloc_aligned
// ------------------------------------------------------

mi_decl_nodiscard mi_decl_restrict void* mi_heap_malloc_aligned_at(mi_heap_t* heap, size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_malloc_zero_aligned_at(heap, size, alignment, offset, false);
}

mi_decl_nodiscard mi_decl_restrict void* mi_heap_malloc_aligned(mi_heap_t* heap, size_t size, size_t alignment) mi_attr_noexcept {
  return mi_heap_malloc_aligned_at(heap, size, alignment, 0);
}

// ------------------------------------------------------
// Aligned Allocation
// ------------------------------------------------------

mi_decl_nodiscard mi_decl_restrict void* mi_heap_zalloc_aligned_at(mi_heap_t* heap, size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_malloc_zero_aligned_at(heap, size, alignment, offset, true);
}

mi_decl_nodiscard mi_decl_restrict void* mi_heap_zalloc_aligned(mi_heap_t* heap, size_t size, size_t alignment) mi_attr_noexcept {
  return mi_heap_zalloc_aligned_at(heap, size, alignment, 0);
}

mi_decl_nodiscard mi_decl_restrict void* mi_heap_calloc_aligned_at(mi_heap_t* heap, size_t count, size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  size_t total;
  if (mi_count_size_overflow(count, size, &total)) return NULL;
  return mi_heap_zalloc_aligned_at(heap, total, alignment, offset);
}

mi_decl_nodiscard mi_decl_restrict void* mi_heap_calloc_aligned(mi_heap_t* heap, size_t count, size_t size, size_t alignment) mi_attr_noexcept {
  return mi_heap_calloc_aligned_at(heap,count,size,alignment,0);
}

mi_decl_nodiscard mi_decl_restrict void* mi_malloc_aligned_at(size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_malloc_aligned_at(mi_prim_get_default_heap(), size, alignment, offset);
}

mi_decl_nodiscard mi_decl_restrict void* mi_malloc_aligned(size_t size, size_t alignment) mi_attr_noexcept {
  return mi_heap_malloc_aligned(mi_prim_get_default_heap(), size, alignment);
}

mi_decl_nodiscard mi_decl_restrict void* mi_zalloc_aligned_at(size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_zalloc_aligned_at(mi_prim_get_default_heap(), size, alignment, offset);
}

mi_decl_nodiscard mi_decl_restrict void* mi_zalloc_aligned(size_t size, size_t alignment) mi_attr_noexcept {
  return mi_heap_zalloc_aligned(mi_prim_get_default_heap(), size, alignment);
}

mi_decl_nodiscard mi_decl_restrict void* mi_calloc_aligned_at(size_t count, size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_calloc_aligned_at(mi_prim_get_default_heap(), count, size, alignment, offset);
}

mi_decl_nodiscard mi_decl_restrict void* mi_calloc_aligned(size_t count, size_t size, size_t alignment) mi_attr_noexcept {
  return mi_heap_calloc_aligned(mi_prim_get_default_heap(), count, size, alignment);
}


// ------------------------------------------------------
// Aligned re-allocation
// ------------------------------------------------------

static void* mi_heap_realloc_zero_aligned_at(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, size_t offset, bool zero) mi_attr_noexcept {
  mi_assert(alignment > 0);
  if (alignment <= sizeof(uintptr_t)) return _mi_heap_realloc_zero(heap,p,newsize,zero);
  if (p == NULL) return mi_heap_malloc_zero_aligned_at(heap,newsize,alignment,offset,zero);
  size_t size = mi_usable_size(p);
  if (newsize <= size && newsize >= (size - (size / 2))
      && (((uintptr_t)p + offset) % alignment) == 0) {
    return p;  // reallocation still fits, is aligned and not more than 50% waste
  }
  else {
    // note: we don't zero allocate upfront so we only zero initialize the expanded part
    void* newp = mi_heap_malloc_aligned_at(heap,newsize,alignment,offset);
    if (newp != NULL) {
      if (zero && newsize > size) {
        // also set last word in the previous allocation to zero to ensure any padding is zero-initialized
        size_t start = (size >= sizeof(intptr_t) ? size - sizeof(intptr_t) : 0);
        _mi_memzero((uint8_t*)newp + start, newsize - start);
      }
      _mi_memcpy_aligned(newp, p, (newsize > size ? size : newsize));
      mi_free(p); // only free if successful
    }
    return newp;
  }
}

static void* mi_heap_realloc_zero_aligned(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, bool zero) mi_attr_noexcept {
  mi_assert(alignment > 0);
  if (alignment <= sizeof(uintptr_t)) return _mi_heap_realloc_zero(heap,p,newsize,zero);
  size_t offset = ((uintptr_t)p % alignment); // use offset of previous allocation (p can be NULL)
  return mi_heap_realloc_zero_aligned_at(heap,p,newsize,alignment,offset,zero);
}

mi_decl_nodiscard void* mi_heap_realloc_aligned_at(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_realloc_zero_aligned_at(heap,p,newsize,alignment,offset,false);
}

mi_decl_nodiscard void* mi_heap_realloc_aligned(mi_heap_t* heap, void* p, size_t newsize, size_t alignment) mi_attr_noexcept {
  return mi_heap_realloc_zero_aligned(heap,p,newsize,alignment,false);
}

mi_decl_nodiscard void* mi_heap_rezalloc_aligned_at(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_realloc_zero_aligned_at(heap, p, newsize, alignment, offset, true);
}

mi_decl_nodiscard void* mi_heap_rezalloc_aligned(mi_heap_t* heap, void* p, size_t newsize, size_t alignment) mi_attr_noexcept {
  return mi_heap_realloc_zero_aligned(heap, p, newsize, alignment, true);
}

mi_decl_nodiscard void* mi_heap_recalloc_aligned_at(mi_heap_t* heap, void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  size_t total;
  if (mi_count_size_overflow(newcount, size, &total)) return NULL;
  return mi_heap_rezalloc_aligned_at(heap, p, total, alignment, offset);
}

mi_decl_nodiscard void* mi_heap_recalloc_aligned(mi_heap_t* heap, void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept {
  size_t total;
  if (mi_count_size_overflow(newcount, size, &total)) return NULL;
  return mi_heap_rezalloc_aligned(heap, p, total, alignment);
}

mi_decl_nodiscard void* mi_realloc_aligned_at(void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_realloc_aligned_at(mi_prim_get_default_heap(), p, newsize, alignment, offset);
}

mi_decl_nodiscard void* mi_realloc_aligned(void* p, size_t newsize, size_t alignment) mi_attr_noexcept {
  return mi_heap_realloc_aligned(mi_prim_get_default_heap(), p, newsize, alignment);
}

mi_decl_nodiscard void* mi_rezalloc_aligned_at(void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_rezalloc_aligned_at(mi_prim_get_default_heap(), p, newsize, alignment, offset);
}

mi_decl_nodiscard void* mi_rezalloc_aligned(void* p, size_t newsize, size_t alignment) mi_attr_noexcept {
  return mi_heap_rezalloc_aligned(mi_prim_get_default_heap(), p, newsize, alignment);
}

mi_decl_nodiscard void* mi_recalloc_aligned_at(void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_recalloc_aligned_at(mi_prim_get_default_heap(), p, newcount, size, alignment, offset);
}

mi_decl_nodiscard void* mi_recalloc_aligned(void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept {
  return mi_heap_recalloc_aligned(mi_prim_get_default_heap(), p, newcount, size, alignment);
}


// --- Amalgamation END --- mimalloc/src/alloc-aligned.c


// --- Amalgamation START --- mimalloc/src/alloc-posix.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2021, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// ------------------------------------------------------------------------
// mi prefixed publi definitions of various Posix, Unix, and C++ functions
// for convenience and used when overriding these functions.
// ------------------------------------------------------------------------

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// ------------------------------------------------------
// Posix & Unix functions definitions
// ------------------------------------------------------

#include <errno.h>
#include <string.h>  // memset
#include <stdlib.h>  // getenv

#if 0 // #ifdef _MSC_VER
// #pragma warning(disable:4996)  // getenv _wgetenv
#endif // 6264

#ifndef EINVAL
#define EINVAL 22
#endif // 6268
#ifndef ENOMEM
#define ENOMEM 12
#endif // 6271


mi_decl_nodiscard size_t mi_malloc_size(const void* p) mi_attr_noexcept {
  // if (!mi_is_in_heap_region(p)) return 0;
  return mi_usable_size(p);
}

mi_decl_nodiscard size_t mi_malloc_usable_size(const void *p) mi_attr_noexcept {
  // if (!mi_is_in_heap_region(p)) return 0;
  return mi_usable_size(p);
}

mi_decl_nodiscard size_t mi_malloc_good_size(size_t size) mi_attr_noexcept {
  return mi_good_size(size);
}

void mi_cfree(void* p) mi_attr_noexcept {
  if (mi_is_in_heap_region(p)) {
    mi_free(p);
  }
}

int mi_posix_memalign(void** p, size_t alignment, size_t size) mi_attr_noexcept {
  // Note: The spec dictates we should not modify `*p` on an error. (issue#27)
  // <http://man7.org/linux/man-pages/man3/posix_memalign.3.html>
  if (p == NULL) return EINVAL;
  if ((alignment % sizeof(void*)) != 0) return EINVAL;                 // natural alignment
  // it is also required that alignment is a power of 2 and > 0; this is checked in `mi_malloc_aligned`
  if (alignment==0 || !_mi_is_power_of_two(alignment)) return EINVAL;  // not a power of 2
  void* q = mi_malloc_aligned(size, alignment);
  if (q==NULL && size != 0) return ENOMEM;
  mi_assert_internal(((uintptr_t)q % alignment) == 0);
  *p = q;
  return 0;
}

mi_decl_nodiscard mi_decl_restrict void* mi_memalign(size_t alignment, size_t size) mi_attr_noexcept {
  void* p = mi_malloc_aligned(size, alignment);
  mi_assert_internal(((uintptr_t)p % alignment) == 0);
  return p;
}

mi_decl_nodiscard mi_decl_restrict void* mi_valloc(size_t size) mi_attr_noexcept {
  return mi_memalign( _mi_os_page_size(), size );
}

mi_decl_nodiscard mi_decl_restrict void* mi_pvalloc(size_t size) mi_attr_noexcept {
  size_t psize = _mi_os_page_size();
  if (size >= SIZE_MAX - psize) return NULL; // overflow
  size_t asize = _mi_align_up(size, psize);
  return mi_malloc_aligned(asize, psize);
}

mi_decl_nodiscard mi_decl_restrict void* mi_aligned_alloc(size_t alignment, size_t size) mi_attr_noexcept {
  // C11 requires the size to be an integral multiple of the alignment, see <https://en.cppreference.com/w/c/memory/aligned_alloc>.
  // unfortunately, it turns out quite some programs pass a size that is not an integral multiple so skip this check..
  /* if mi_unlikely((size & (alignment - 1)) != 0) { // C11 requires alignment>0 && integral multiple, see <https://en.cppreference.com/w/c/memory/aligned_alloc>
      #if MI_DEBUG > 0
      _mi_error_message(EOVERFLOW, "(mi_)aligned_alloc requires the size to be an integral multiple of the alignment (size %zu, alignment %zu)\n", size, alignment);
      #endif // 6331
      return NULL;
    }
  */
  // C11 also requires alignment to be a power-of-two (and > 0) which is checked in mi_malloc_aligned
  void* p = mi_malloc_aligned(size, alignment);
  mi_assert_internal(((uintptr_t)p % alignment) == 0);
  return p;
}

mi_decl_nodiscard void* mi_reallocarray( void* p, size_t count, size_t size ) mi_attr_noexcept {  // BSD
  void* newp = mi_reallocn(p,count,size);
  if (newp==NULL) { errno = ENOMEM; }
  return newp;
}

mi_decl_nodiscard int mi_reallocarr( void* p, size_t count, size_t size ) mi_attr_noexcept { // NetBSD
  mi_assert(p != NULL);
  if (p == NULL) {
    errno = EINVAL;
    return EINVAL;
  }
  void** op = (void**)p;
  void* newp = mi_reallocarray(*op, count, size);
  if mi_unlikely(newp == NULL) { return errno; }
  *op = newp;
  return 0;
}

void* mi__expand(void* p, size_t newsize) mi_attr_noexcept {  // Microsoft
  void* res = mi_expand(p, newsize);
  if (res == NULL) { errno = ENOMEM; }
  return res;
}

mi_decl_nodiscard mi_decl_restrict unsigned short* mi_wcsdup(const unsigned short* s) mi_attr_noexcept {
  if (s==NULL) return NULL;
  size_t len;
  for(len = 0; s[len] != 0; len++) { }
  size_t size = (len+1)*sizeof(unsigned short);
  unsigned short* p = (unsigned short*)mi_malloc(size);
  if (p != NULL) {
    _mi_memcpy(p,s,size);
  }
  return p;
}

mi_decl_nodiscard mi_decl_restrict unsigned char* mi_mbsdup(const unsigned char* s)  mi_attr_noexcept {
  return (unsigned char*)mi_strdup((const char*)s);
}

int mi_dupenv_s(char** buf, size_t* size, const char* name) mi_attr_noexcept {
  if (buf==NULL || name==NULL) return EINVAL;
  if (size != NULL) *size = 0;
  char* p = getenv(name);        // mscver warning 4996
  if (p==NULL) {
    *buf = NULL;
  }
  else {
    *buf = mi_strdup(p);
    if (*buf==NULL) return ENOMEM;
    if (size != NULL) *size = _mi_strlen(p);
  }
  return 0;
}

int mi_wdupenv_s(unsigned short** buf, size_t* size, const unsigned short* name) mi_attr_noexcept {
  if (buf==NULL || name==NULL) return EINVAL;
  if (size != NULL) *size = 0;
#if !defined(_WIN32) || (defined(WINAPI_FAMILY) && (WINAPI_FAMILY != WINAPI_FAMILY_DESKTOP_APP))
  // not supported
  *buf = NULL;
  return EINVAL;
#else
  unsigned short* p = (unsigned short*)_wgetenv((const wchar_t*)name);  // msvc warning 4996
  if (p==NULL) {
    *buf = NULL;
  }
  else {
    *buf = mi_wcsdup(p);
    if (*buf==NULL) return ENOMEM;
    if (size != NULL) *size = wcslen((const wchar_t*)p);
  }
  return 0;
#endif // 6402
}

mi_decl_nodiscard void* mi_aligned_offset_recalloc(void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept { // Microsoft
  return mi_recalloc_aligned_at(p, newcount, size, alignment, offset);
}

mi_decl_nodiscard void* mi_aligned_recalloc(void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept { // Microsoft
  return mi_recalloc_aligned(p, newcount, size, alignment);
}
// --- Amalgamation END --- mimalloc/src/alloc-posix.c


// --- Amalgamation START --- mimalloc/src/arena.c
/* ----------------------------------------------------------------------------
Copyright (c) 2019-2024, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------
"Arenas" are fixed area's of OS memory from which we can allocate
large blocks (>= MI_ARENA_MIN_BLOCK_SIZE, 4MiB).
In contrast to the rest of mimalloc, the arenas are shared between
threads and need to be accessed using atomic operations.

Arenas are also used to for huge OS page (1GiB) reservations or for reserving
OS memory upfront which can be improve performance or is sometimes needed
on embedded devices. We can also employ this with WASI or `sbrk` systems
to reserve large arenas upfront and be able to reuse the memory more effectively.

The arena allocation needs to be thread safe and we use an atomic bitmap to allocate.
-----------------------------------------------------------------------------*/


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


// --- Amalgamation START --- mimalloc/src/bitmap.h
/* ----------------------------------------------------------------------------
Copyright (c) 2019-2023 Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------
Concurrent bitmap that can set/reset sequences of bits atomically,
represented as an array of fields where each field is a machine word (`size_t`)

There are two api's; the standard one cannot have sequences that cross
between the bitmap fields (and a sequence must be <= MI_BITMAP_FIELD_BITS).
(this is used in region allocation)

The `_across` postfixed functions do allow sequences that can cross over
between the fields. (This is used in arena allocation)
---------------------------------------------------------------------------- */
#ifndef MI_BITMAP_H
#define MI_BITMAP_H

/* -----------------------------------------------------------
  Bitmap definition
----------------------------------------------------------- */

#define MI_BITMAP_FIELD_BITS   (8*MI_SIZE_SIZE)
#define MI_BITMAP_FIELD_FULL   (~((size_t)0))   // all bits set

// An atomic bitmap of `size_t` fields
typedef _Atomic(size_t)  mi_bitmap_field_t;
typedef mi_bitmap_field_t*  mi_bitmap_t;

// A bitmap index is the index of the bit in a bitmap.
typedef size_t mi_bitmap_index_t;

// Create a bit index.
static inline mi_bitmap_index_t mi_bitmap_index_create_ex(size_t idx, size_t bitidx) {
  mi_assert_internal(bitidx <= MI_BITMAP_FIELD_BITS);
  return (idx*MI_BITMAP_FIELD_BITS) + bitidx;
}
static inline mi_bitmap_index_t mi_bitmap_index_create(size_t idx, size_t bitidx) {
  mi_assert_internal(bitidx < MI_BITMAP_FIELD_BITS);
  return mi_bitmap_index_create_ex(idx,bitidx);
}

// Get the field index from a bit index.
static inline size_t mi_bitmap_index_field(mi_bitmap_index_t bitmap_idx) {
  return (bitmap_idx / MI_BITMAP_FIELD_BITS);
}

// Get the bit index in a bitmap field
static inline size_t mi_bitmap_index_bit_in_field(mi_bitmap_index_t bitmap_idx) {
  return (bitmap_idx % MI_BITMAP_FIELD_BITS);
}

// Get the full bit index
static inline size_t mi_bitmap_index_bit(mi_bitmap_index_t bitmap_idx) {
  return bitmap_idx;
}

/* -----------------------------------------------------------
  Claim a bit sequence atomically
----------------------------------------------------------- */

// Try to atomically claim a sequence of `count` bits in a single
// field at `idx` in `bitmap`. Returns `true` on success.
bool _mi_bitmap_try_find_claim_field(mi_bitmap_t bitmap, size_t idx, const size_t count, mi_bitmap_index_t* bitmap_idx);

// Starts at idx, and wraps around to search in all `bitmap_fields` fields.
// For now, `count` can be at most MI_BITMAP_FIELD_BITS and will never cross fields.
bool _mi_bitmap_try_find_from_claim(mi_bitmap_t bitmap, const size_t bitmap_fields, const size_t start_field_idx, const size_t count, mi_bitmap_index_t* bitmap_idx);

// Set `count` bits at `bitmap_idx` to 0 atomically
// Returns `true` if all `count` bits were 1 previously.
bool _mi_bitmap_unclaim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);

// Try to set `count` bits at `bitmap_idx` from 0 to 1 atomically. 
// Returns `true` if successful when all previous `count` bits were 0.
bool _mi_bitmap_try_claim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);

// Set `count` bits at `bitmap_idx` to 1 atomically
// Returns `true` if all `count` bits were 0 previously. `any_zero` is `true` if there was at least one zero bit.
bool _mi_bitmap_claim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* any_zero);

bool _mi_bitmap_is_claimed(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);
bool _mi_bitmap_is_any_claimed(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);


//--------------------------------------------------------------------------
// the `_across` functions work on bitmaps where sequences can cross over
// between the fields. This is used in arena allocation
//--------------------------------------------------------------------------

// Find `count` bits of zeros and set them to 1 atomically; returns `true` on success.
// Starts at idx, and wraps around to search in all `bitmap_fields` fields.
bool _mi_bitmap_try_find_from_claim_across(mi_bitmap_t bitmap, const size_t bitmap_fields, const size_t start_field_idx, const size_t count, mi_bitmap_index_t* bitmap_idx, mi_stats_t* stats);

// Set `count` bits at `bitmap_idx` to 0 atomically
// Returns `true` if all `count` bits were 1 previously.
bool _mi_bitmap_unclaim_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);

// Set `count` bits at `bitmap_idx` to 1 atomically
// Returns `true` if all `count` bits were 0 previously. `any_zero` is `true` if there was at least one zero bit.
bool _mi_bitmap_claim_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* pany_zero);

bool _mi_bitmap_is_claimed_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);
bool _mi_bitmap_is_any_claimed_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);

#endif // 6484
// --- Amalgamation END --- mimalloc/src/bitmap.h



/* -----------------------------------------------------------
  Arena allocation
----------------------------------------------------------- */

// A memory arena descriptor
typedef struct mi_arena_s {
  mi_arena_id_t       id;                   // arena id; 0 for non-specific
  mi_memid_t          memid;                // memid of the memory area
  _Atomic(uint8_t*)start;                // the start of the memory area
  size_t              block_count;          // size of the area in arena blocks (of `MI_ARENA_BLOCK_SIZE`)
  size_t              field_count;          // number of bitmap fields (where `field_count * MI_BITMAP_FIELD_BITS >= block_count`)
  size_t              meta_size;            // size of the arena structure itself (including its bitmaps)
  mi_memid_t          meta_memid;           // memid of the arena structure itself (OS or static allocation)
  int                 numa_node;            // associated NUMA node
  bool                exclusive;            // only allow allocations if specifically for this arena
  bool                is_large;             // memory area consists of large- or huge OS pages (always committed)
  mi_lock_t           abandoned_visit_lock; // lock is only used when abandoned segments are being visited
  _Atomic(size_t)search_idx;           // optimization to start the search for free blocks
  _Atomic(mi_msecs_t)purge_expire;         // expiration time when blocks should be decommitted from `blocks_decommit`.
  mi_bitmap_field_t* blocks_dirty;         // are the blocks potentially non-zero?
  mi_bitmap_field_t* blocks_committed;     // are the blocks committed? (can be NULL for memory that cannot be decommitted)
  mi_bitmap_field_t* blocks_purge;         // blocks that can be (reset) decommitted. (can be NULL for memory that cannot be (reset) decommitted)
  mi_bitmap_field_t* blocks_abandoned;     // blocks that start with an abandoned segment. (This crosses API's but it is convenient to have here)
  mi_bitmap_field_t   blocks_inuse[1];      // in-place bitmap of in-use blocks (of size `field_count`)
  // do not add further fields here as the dirty, committed, purged, and abandoned bitmaps follow the inuse bitmap fields.
} mi_arena_t;


#define MI_ARENA_BLOCK_SIZE   (MI_SEGMENT_SIZE)        // 64MiB  (must be at least MI_SEGMENT_ALIGN)
#define MI_ARENA_MIN_OBJ_SIZE (MI_ARENA_BLOCK_SIZE/2)  // 32MiB
#define MI_MAX_ARENAS         (132)                    // Limited as the reservation exponentially increases (and takes up .bss)

// The available arenas
static mi_decl_cache_align _Atomic(mi_arena_t*) mi_arenas[MI_MAX_ARENAS];
static mi_decl_cache_align _Atomic(size_t)      mi_arena_count; // = 0

#define MI_IN_ARENA_C

// --- Amalgamation START --- mimalloc/src/arena-abandon.c
/* ----------------------------------------------------------------------------
Copyright (c) 2019-2024, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

#if !defined(MI_IN_ARENA_C)
#error "this file should be included from 'arena.c' (so mi_arena_t is visible)"
// add includes help an IDE

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/src/bitmap.h
// --- Amalgamation END --- mimalloc/src/bitmap.h

#endif // 6625

// Minimal exports for arena-abandoned.
size_t      mi_arena_id_index(mi_arena_id_t id);
mi_arena_t* mi_arena_from_index(size_t idx);
size_t      mi_arena_get_count(void);
void*       mi_arena_block_start(mi_arena_t* arena, mi_bitmap_index_t bindex);
bool        mi_arena_memid_indices(mi_memid_t memid, size_t* arena_index, mi_bitmap_index_t* bitmap_index);

/* -----------------------------------------------------------
  Abandoned blocks/segments:

  _mi_arena_segment_clear_abandoned
  _mi_arena_segment_mark_abandoned

  This is used to atomically abandon/reclaim segments
  (and crosses the arena API but it is convenient to have here).

  Abandoned segments still have live blocks; they get reclaimed
  when a thread frees a block in it, or when a thread needs a fresh
  segment.

  Abandoned segments are atomically marked in the `block_abandoned`
  bitmap of arenas. Any segments allocated outside arenas are put
  in the sub-process `abandoned_os_list`. This list is accessed
  using locks but this should be uncommon and generally uncontended.
  Reclaim and visiting either scan through the `block_abandoned`
  bitmaps of the arena's, or visit the `abandoned_os_list`

  A potentially nicer design is to use arena's for everything
  and perhaps have virtual arena's to map OS allocated memory
  but this would lack the "density" of our current arena's. TBC.
----------------------------------------------------------- */


// reclaim a specific OS abandoned segment; `true` on success.
// sets the thread_id.
static bool mi_arena_segment_os_clear_abandoned(mi_segment_t* segment, bool take_lock) {
  mi_assert(segment->memid.memkind != MI_MEM_ARENA);
  // not in an arena, remove from list of abandoned os segments
  mi_subproc_t* const subproc = segment->subproc;
  if (take_lock && !mi_lock_try_acquire(&subproc->abandoned_os_lock)) {
    return false;  // failed to acquire the lock, we just give up
  }
  // remove atomically from the abandoned os list (if possible!)
  bool reclaimed = false;
  mi_segment_t* const next = segment->abandoned_os_next;
  mi_segment_t* const prev = segment->abandoned_os_prev;
  if (next != NULL || prev != NULL || subproc->abandoned_os_list == segment) {
    #if MI_DEBUG>3
    // find ourselves in the abandoned list (and check the count)
    bool found = false;
    size_t count = 0;
    for (mi_segment_t* current = subproc->abandoned_os_list; current != NULL; current = current->abandoned_os_next) {
      if (current == segment) { found = true; }
      count++;
    }
    mi_assert_internal(found);
    mi_assert_internal(count == mi_atomic_load_relaxed(&subproc->abandoned_os_list_count));
    #endif // 6689
    // remove (atomically) from the list and reclaim
    if (prev != NULL) { prev->abandoned_os_next = next; }
    else { subproc->abandoned_os_list = next; }
    if (next != NULL) { next->abandoned_os_prev = prev; }
    else { subproc->abandoned_os_list_tail = prev; }
    segment->abandoned_os_next = NULL;
    segment->abandoned_os_prev = NULL;
    mi_atomic_decrement_relaxed(&subproc->abandoned_count);
    mi_atomic_decrement_relaxed(&subproc->abandoned_os_list_count);
    if (take_lock) { // don't reset the thread_id when iterating
      mi_atomic_store_release(&segment->thread_id, _mi_thread_id());
    }
    reclaimed = true;
  }
  if (take_lock) { mi_lock_release(&segment->subproc->abandoned_os_lock); }
  return reclaimed;
}

// reclaim a specific abandoned segment; `true` on success.
// sets the thread_id.
bool _mi_arena_segment_clear_abandoned(mi_segment_t* segment) {
  if mi_unlikely(segment->memid.memkind != MI_MEM_ARENA) {
    return mi_arena_segment_os_clear_abandoned(segment, true /* take lock */);
  }
  // arena segment: use the blocks_abandoned bitmap.
  size_t arena_idx;
  size_t bitmap_idx;
  mi_arena_memid_indices(segment->memid, &arena_idx, &bitmap_idx);
  mi_arena_t* arena = mi_arena_from_index(arena_idx);
  mi_assert_internal(arena != NULL);
  // reclaim atomically
  bool was_marked = _mi_bitmap_unclaim(arena->blocks_abandoned, arena->field_count, 1, bitmap_idx);
  if (was_marked) {
    mi_assert_internal(mi_atomic_load_acquire(&segment->thread_id) == 0);
    mi_atomic_decrement_relaxed(&segment->subproc->abandoned_count);
    mi_atomic_store_release(&segment->thread_id, _mi_thread_id());
  }
  // mi_assert_internal(was_marked);
  mi_assert_internal(!was_marked || _mi_bitmap_is_claimed(arena->blocks_inuse, arena->field_count, 1, bitmap_idx));
  //mi_assert_internal(arena->blocks_committed == NULL || _mi_bitmap_is_claimed(arena->blocks_committed, arena->field_count, 1, bitmap_idx));
  return was_marked;
}


// mark a specific OS segment as abandoned
static void mi_arena_segment_os_mark_abandoned(mi_segment_t* segment) {
  mi_assert(segment->memid.memkind != MI_MEM_ARENA);
  // not in an arena; we use a list of abandoned segments
  mi_subproc_t* const subproc = segment->subproc;
  if (!mi_lock_acquire(&subproc->abandoned_os_lock)) {
    _mi_error_message(EFAULT, "internal error: failed to acquire the abandoned (os) segment lock to mark abandonment");
    // we can continue but cannot visit/reclaim such blocks..
  }
  else {
    // push on the tail of the list (important for the visitor)
    mi_segment_t* prev = subproc->abandoned_os_list_tail;
    mi_assert_internal(prev == NULL || prev->abandoned_os_next == NULL);
    mi_assert_internal(segment->abandoned_os_prev == NULL);
    mi_assert_internal(segment->abandoned_os_next == NULL);
    if (prev != NULL) { prev->abandoned_os_next = segment; }
    else { subproc->abandoned_os_list = segment; }
    subproc->abandoned_os_list_tail = segment;
    segment->abandoned_os_prev = prev;
    segment->abandoned_os_next = NULL;
    mi_atomic_increment_relaxed(&subproc->abandoned_os_list_count);
    mi_atomic_increment_relaxed(&subproc->abandoned_count);
    // and release the lock
    mi_lock_release(&subproc->abandoned_os_lock);
  }
  return;
}

// mark a specific segment as abandoned
// clears the thread_id.
void _mi_arena_segment_mark_abandoned(mi_segment_t* segment)
{
  mi_assert_internal(segment->used == segment->abandoned);
  mi_atomic_store_release(&segment->thread_id, (uintptr_t)0);  // mark as abandoned for multi-thread free's
  if mi_unlikely(segment->memid.memkind != MI_MEM_ARENA) {
    mi_arena_segment_os_mark_abandoned(segment);
    return;
  }
  // segment is in an arena, mark it in the arena `blocks_abandoned` bitmap
  size_t arena_idx;
  size_t bitmap_idx;
  mi_arena_memid_indices(segment->memid, &arena_idx, &bitmap_idx);
  mi_arena_t* arena = mi_arena_from_index(arena_idx);
  mi_assert_internal(arena != NULL);
  // set abandonment atomically
  mi_subproc_t* const subproc = segment->subproc; // don't access the segment after setting it abandoned
  const bool was_unmarked = _mi_bitmap_claim(arena->blocks_abandoned, arena->field_count, 1, bitmap_idx, NULL);
  if (was_unmarked) { mi_atomic_increment_relaxed(&subproc->abandoned_count); }
  mi_assert_internal(was_unmarked);
  mi_assert_internal(_mi_bitmap_is_claimed(arena->blocks_inuse, arena->field_count, 1, bitmap_idx));
}


/* -----------------------------------------------------------
  Iterate through the abandoned blocks/segments using a cursor.
  This is used for reclaiming and abandoned block visiting.
----------------------------------------------------------- */

// start a cursor at a randomized arena
void _mi_arena_field_cursor_init(mi_heap_t* heap, mi_subproc_t* subproc, bool visit_all, mi_arena_field_cursor_t* current) {
  mi_assert_internal(heap == NULL || heap->tld->segments.subproc == subproc);
  current->bitmap_idx = 0;
  current->subproc = subproc;
  current->visit_all = visit_all;
  current->hold_visit_lock = false;
  const size_t abandoned_count = mi_atomic_load_relaxed(&subproc->abandoned_count);
  const size_t abandoned_list_count = mi_atomic_load_relaxed(&subproc->abandoned_os_list_count);
  const size_t max_arena = mi_arena_get_count();
  if (heap != NULL && heap->arena_id != _mi_arena_id_none()) {
    // for a heap that is bound to one arena, only visit that arena
    current->start = mi_arena_id_index(heap->arena_id);
    current->end = current->start + 1;
    current->os_list_count = 0;
  }
  else {
    // otherwise visit all starting at a random location
    if (abandoned_count > abandoned_list_count && max_arena > 0) {
      current->start = (heap == NULL || max_arena == 0 ? 0 : (mi_arena_id_t)(_mi_heap_random_next(heap) % max_arena));
      current->end = current->start + max_arena;
    }
    else {
      current->start = 0;
      current->end = 0;
    }
    current->os_list_count = abandoned_list_count; // max entries to visit in the os abandoned list
  }
  mi_assert_internal(current->start <= max_arena);
}

void _mi_arena_field_cursor_done(mi_arena_field_cursor_t* current) {
  if (current->hold_visit_lock) {
    mi_lock_release(&current->subproc->abandoned_os_visit_lock);
    current->hold_visit_lock = false;
  }
}

static mi_segment_t* mi_arena_segment_clear_abandoned_at(mi_arena_t* arena, mi_subproc_t* subproc, mi_bitmap_index_t bitmap_idx) {
  // try to reclaim an abandoned segment in the arena atomically
  if (!_mi_bitmap_unclaim(arena->blocks_abandoned, arena->field_count, 1, bitmap_idx)) return NULL;
  mi_assert_internal(_mi_bitmap_is_claimed(arena->blocks_inuse, arena->field_count, 1, bitmap_idx));
  mi_segment_t* segment = (mi_segment_t*)mi_arena_block_start(arena, bitmap_idx);
  mi_assert_internal(mi_atomic_load_relaxed(&segment->thread_id) == 0);
  // check that the segment belongs to our sub-process
  // note: this is the reason we need the `abandoned_visit` lock in the case abandoned visiting is enabled.
  //  without the lock an abandoned visit may otherwise fail to visit all abandoned segments in the sub-process.
  //  for regular reclaim it is fine to miss one sometimes so without abandoned visiting we don't need the `abandoned_visit` lock.
  if (segment->subproc != subproc) {
    // it is from another sub-process, re-mark it and continue searching
    const bool was_zero = _mi_bitmap_claim(arena->blocks_abandoned, arena->field_count, 1, bitmap_idx, NULL);
    mi_assert_internal(was_zero); MI_UNUSED(was_zero);
    return NULL;
  }
  else {
    // success, we unabandoned a segment in our sub-process
    mi_atomic_decrement_relaxed(&subproc->abandoned_count);
    return segment;
  }
}

static mi_segment_t* mi_arena_segment_clear_abandoned_next_field(mi_arena_field_cursor_t* previous) {
  const size_t max_arena = mi_arena_get_count();
  size_t field_idx = mi_bitmap_index_field(previous->bitmap_idx);
  size_t bit_idx = mi_bitmap_index_bit_in_field(previous->bitmap_idx);
  // visit arena's (from the previous cursor)
  for (; previous->start < previous->end; previous->start++, field_idx = 0, bit_idx = 0) {
    // index wraps around
    size_t arena_idx = (previous->start >= max_arena ? previous->start % max_arena : previous->start);
    mi_arena_t* arena = mi_arena_from_index(arena_idx);
    if (arena != NULL) {
      bool has_lock = false;
      // visit the abandoned fields (starting at previous_idx)
      for (; field_idx < arena->field_count; field_idx++, bit_idx = 0) {
        size_t field = mi_atomic_load_relaxed(&arena->blocks_abandoned[field_idx]);
        if mi_unlikely(field != 0) { // skip zero fields quickly
          // we only take the arena lock if there are actually abandoned segments present
          if (!has_lock && mi_option_is_enabled(mi_option_visit_abandoned)) {
            has_lock = (previous->visit_all ? mi_lock_acquire(&arena->abandoned_visit_lock) : mi_lock_try_acquire(&arena->abandoned_visit_lock));
            if (!has_lock) {
              if (previous->visit_all) {
                _mi_error_message(EFAULT, "internal error: failed to visit all abandoned segments due to failure to acquire the visitor lock");
              }
              // skip to next arena
              break;
            }
          }
          mi_assert_internal(has_lock || !mi_option_is_enabled(mi_option_visit_abandoned));
          // visit each set bit in the field  (todo: maybe use `ctz` here?)
          for (; bit_idx < MI_BITMAP_FIELD_BITS; bit_idx++) {
            // pre-check if the bit is set
            size_t mask = ((size_t)1 << bit_idx);
            if mi_unlikely((field & mask) == mask) {
              mi_bitmap_index_t bitmap_idx = mi_bitmap_index_create(field_idx, bit_idx);
              mi_segment_t* const segment = mi_arena_segment_clear_abandoned_at(arena, previous->subproc, bitmap_idx);
              if (segment != NULL) {
                //mi_assert_internal(arena->blocks_committed == NULL || _mi_bitmap_is_claimed(arena->blocks_committed, arena->field_count, 1, bitmap_idx));
                if (has_lock) { mi_lock_release(&arena->abandoned_visit_lock); }
                previous->bitmap_idx = mi_bitmap_index_create_ex(field_idx, bit_idx + 1); // start at next one for the next iteration
                return segment;
              }
            }
          }
        }
      }
      if (has_lock) { mi_lock_release(&arena->abandoned_visit_lock); }
    }
  }
  return NULL;
}

static mi_segment_t* mi_arena_segment_clear_abandoned_next_list(mi_arena_field_cursor_t* previous) {
  // go through the abandoned_os_list
  // we only allow one thread per sub-process to do to visit guarded by the `abandoned_os_visit_lock`.
  // The lock is released when the cursor is released.
  if (!previous->hold_visit_lock) {
    previous->hold_visit_lock = (previous->visit_all ? mi_lock_acquire(&previous->subproc->abandoned_os_visit_lock)
      : mi_lock_try_acquire(&previous->subproc->abandoned_os_visit_lock));
    if (!previous->hold_visit_lock) {
      if (previous->visit_all) {
        _mi_error_message(EFAULT, "internal error: failed to visit all abandoned segments due to failure to acquire the OS visitor lock");
      }
      return NULL; // we cannot get the lock, give up
    }
  }
  // One list entry at a time
  while (previous->os_list_count > 0) {
    previous->os_list_count--;
    const bool has_lock = mi_lock_acquire(&previous->subproc->abandoned_os_lock); // this could contend with concurrent OS block abandonment and reclaim from `free`
    if (has_lock) {
      mi_segment_t* segment = previous->subproc->abandoned_os_list;
      // pop from head of the list, a subsequent mark will push at the end (and thus we iterate through os_list_count entries)
      if (segment == NULL || mi_arena_segment_os_clear_abandoned(segment, false /* we already have the lock */)) {
        mi_lock_release(&previous->subproc->abandoned_os_lock);
        return segment;
      }
      // already abandoned, try again
      mi_lock_release(&previous->subproc->abandoned_os_lock);
    }
    else {
      _mi_error_message(EFAULT, "failed to acquire abandoned OS list lock during abandoned block visit\n");
      return NULL;
    }
  }
  // done
  mi_assert_internal(previous->os_list_count == 0);
  return NULL;
}


// reclaim abandoned segments
// this does not set the thread id (so it appears as still abandoned)
mi_segment_t* _mi_arena_segment_clear_abandoned_next(mi_arena_field_cursor_t* previous) {
  if (previous->start < previous->end) {
    // walk the arena
    mi_segment_t* segment = mi_arena_segment_clear_abandoned_next_field(previous);
    if (segment != NULL) { return segment; }
  }
  // no entries in the arena's anymore, walk the abandoned OS list
  mi_assert_internal(previous->start == previous->end);
  return mi_arena_segment_clear_abandoned_next_list(previous);
}


bool mi_abandoned_visit_blocks(mi_subproc_id_t subproc_id, int heap_tag, bool visit_blocks, mi_block_visit_fun* visitor, void* arg) {
  // (unfortunately) the visit_abandoned option must be enabled from the start.
  // This is to avoid taking locks if abandoned list visiting is not required (as for most programs)
  if (!mi_option_is_enabled(mi_option_visit_abandoned)) {
    _mi_error_message(EFAULT, "internal error: can only visit abandoned blocks when MIMALLOC_VISIT_ABANDONED=ON");
    return false;
  }
  mi_arena_field_cursor_t current;
  _mi_arena_field_cursor_init(NULL, _mi_subproc_from_id(subproc_id), true /* visit all (blocking) */, &current);
  mi_segment_t* segment;
  bool ok = true;
  while (ok && (segment = _mi_arena_segment_clear_abandoned_next(&current)) != NULL) {
    ok = _mi_segment_visit_blocks(segment, heap_tag, visit_blocks, visitor, arg);
    _mi_arena_segment_mark_abandoned(segment);
  }
  _mi_arena_field_cursor_done(&current);
  return ok;
}
// --- Amalgamation END --- mimalloc/src/arena-abandon.c

#undef MI_IN_ARENA_C

/* -----------------------------------------------------------
  Arena id's
  id = arena_index + 1
----------------------------------------------------------- */

size_t mi_arena_id_index(mi_arena_id_t id) {
  return (size_t)(id <= 0 ? MI_MAX_ARENAS : id - 1);
}

static mi_arena_id_t mi_arena_id_create(size_t arena_index) {
  mi_assert_internal(arena_index < MI_MAX_ARENAS);
  return (int)arena_index + 1;
}

mi_arena_id_t _mi_arena_id_none(void) {
  return 0;
}

static bool mi_arena_id_is_suitable(mi_arena_id_t arena_id, bool arena_is_exclusive, mi_arena_id_t req_arena_id) {
  return ((!arena_is_exclusive && req_arena_id == _mi_arena_id_none()) ||
          (arena_id == req_arena_id));
}

bool _mi_arena_memid_is_suitable(mi_memid_t memid, mi_arena_id_t request_arena_id) {
  if (memid.memkind == MI_MEM_ARENA) {
    return mi_arena_id_is_suitable(memid.mem.arena.id, memid.mem.arena.is_exclusive, request_arena_id);
  }
  else {
    return mi_arena_id_is_suitable(_mi_arena_id_none(), false, request_arena_id);
  }
}

size_t mi_arena_get_count(void) {
  return mi_atomic_load_relaxed(&mi_arena_count);
}

mi_arena_t* mi_arena_from_index(size_t idx) {
  mi_assert_internal(idx < mi_arena_get_count());
  return mi_atomic_load_ptr_acquire(mi_arena_t, &mi_arenas[idx]);
}


/* -----------------------------------------------------------
  Arena allocations get a (currently) 16-bit memory id where the
  lower 8 bits are the arena id, and the upper bits the block index.
----------------------------------------------------------- */

static size_t mi_block_count_of_size(size_t size) {
  return _mi_divide_up(size, MI_ARENA_BLOCK_SIZE);
}

static size_t mi_arena_block_size(size_t bcount) {
  return (bcount * MI_ARENA_BLOCK_SIZE);
}

static size_t mi_arena_size(mi_arena_t* arena) {
  return mi_arena_block_size(arena->block_count);
}

static mi_memid_t mi_memid_create_arena(mi_arena_id_t id, bool is_exclusive, mi_bitmap_index_t bitmap_index) {
  mi_memid_t memid = _mi_memid_create(MI_MEM_ARENA);
  memid.mem.arena.id = id;
  memid.mem.arena.block_index = bitmap_index;
  memid.mem.arena.is_exclusive = is_exclusive;
  return memid;
}

bool mi_arena_memid_indices(mi_memid_t memid, size_t* arena_index, mi_bitmap_index_t* bitmap_index) {
  mi_assert_internal(memid.memkind == MI_MEM_ARENA);
  *arena_index = mi_arena_id_index(memid.mem.arena.id);
  *bitmap_index = memid.mem.arena.block_index;
  return memid.mem.arena.is_exclusive;
}



/* -----------------------------------------------------------
  Special static area for mimalloc internal structures
  to avoid OS calls (for example, for the arena metadata (~= 256b))
----------------------------------------------------------- */

#define MI_ARENA_STATIC_MAX  ((MI_INTPTR_SIZE/2)*MI_KiB)  // 4 KiB on 64-bit

static mi_decl_cache_align uint8_t mi_arena_static[MI_ARENA_STATIC_MAX];  // must be cache aligned, see issue #895
static mi_decl_cache_align _Atomic(size_t) mi_arena_static_top;

static void* mi_arena_static_zalloc(size_t size, size_t alignment, mi_memid_t* memid) {
  *memid = _mi_memid_none();
  if (size == 0 || size > MI_ARENA_STATIC_MAX) return NULL;
  const size_t toplow = mi_atomic_load_relaxed(&mi_arena_static_top);
  if ((toplow + size) > MI_ARENA_STATIC_MAX) return NULL;

  // try to claim space
  if (alignment < MI_MAX_ALIGN_SIZE) { alignment = MI_MAX_ALIGN_SIZE; }
  const size_t oversize = size + alignment - 1;
  if (toplow + oversize > MI_ARENA_STATIC_MAX) return NULL;
  const size_t oldtop = mi_atomic_add_acq_rel(&mi_arena_static_top, oversize);
  size_t top = oldtop + oversize;
  if (top > MI_ARENA_STATIC_MAX) {
    // try to roll back, ok if this fails
    mi_atomic_cas_strong_acq_rel(&mi_arena_static_top, &top, oldtop);
    return NULL;
  }

  // success
  *memid = _mi_memid_create(MI_MEM_STATIC);
  memid->initially_zero = true;
  const size_t start = _mi_align_up(oldtop, alignment);
  uint8_t* const p = &mi_arena_static[start];
  _mi_memzero_aligned(p, size);
  return p;
}

void* _mi_arena_meta_zalloc(size_t size, mi_memid_t* memid) {
  *memid = _mi_memid_none();

  // try static
  void* p = mi_arena_static_zalloc(size, MI_MAX_ALIGN_SIZE, memid);
  if (p != NULL) return p;

  // or fall back to the OS
  p = _mi_os_alloc(size, memid, &_mi_stats_main);
  if (p == NULL) return NULL;

  // zero the OS memory if needed
  if (!memid->initially_zero) {
    _mi_memzero_aligned(p, size);
    memid->initially_zero = true;
  }
  return p;
}

void _mi_arena_meta_free(void* p, mi_memid_t memid, size_t size) {
  if (mi_memkind_is_os(memid.memkind)) {
    _mi_os_free(p, size, memid, &_mi_stats_main);
  }
  else {
    mi_assert(memid.memkind == MI_MEM_STATIC);
  }
}

void* mi_arena_block_start(mi_arena_t* arena, mi_bitmap_index_t bindex) {
  return (arena->start + mi_arena_block_size(mi_bitmap_index_bit(bindex)));
}


/* -----------------------------------------------------------
  Thread safe allocation in an arena
----------------------------------------------------------- */

// claim the `blocks_inuse` bits
static bool mi_arena_try_claim(mi_arena_t* arena, size_t blocks, mi_bitmap_index_t* bitmap_idx, mi_stats_t* stats)
{
  size_t idx = 0; // mi_atomic_load_relaxed(&arena->search_idx);  // start from last search; ok to be relaxed as the exact start does not matter
  if (_mi_bitmap_try_find_from_claim_across(arena->blocks_inuse, arena->field_count, idx, blocks, bitmap_idx, stats)) {
    mi_atomic_store_relaxed(&arena->search_idx, mi_bitmap_index_field(*bitmap_idx));  // start search from found location next time around
    return true;
  };
  return false;
}


/* -----------------------------------------------------------
  Arena Allocation
----------------------------------------------------------- */

static mi_decl_noinline void* mi_arena_try_alloc_at(mi_arena_t* arena, size_t arena_index, size_t needed_bcount,
                                                    bool commit, mi_memid_t* memid, mi_os_tld_t* tld)
{
  MI_UNUSED(arena_index);
  mi_assert_internal(mi_arena_id_index(arena->id) == arena_index);

  mi_bitmap_index_t bitmap_index;
  if (!mi_arena_try_claim(arena, needed_bcount, &bitmap_index, tld->stats)) return NULL;

  // claimed it!
  void* p = mi_arena_block_start(arena, bitmap_index);
  *memid = mi_memid_create_arena(arena->id, arena->exclusive, bitmap_index);
  memid->is_pinned = arena->memid.is_pinned;

  // none of the claimed blocks should be scheduled for a decommit
  if (arena->blocks_purge != NULL) {
    // this is thread safe as a potential purge only decommits parts that are not yet claimed as used (in `blocks_inuse`).
    _mi_bitmap_unclaim_across(arena->blocks_purge, arena->field_count, needed_bcount, bitmap_index);
  }

  // set the dirty bits (todo: no need for an atomic op here?)
  if (arena->memid.initially_zero && arena->blocks_dirty != NULL) {
    memid->initially_zero = _mi_bitmap_claim_across(arena->blocks_dirty, arena->field_count, needed_bcount, bitmap_index, NULL);
  }

  // set commit state
  if (arena->blocks_committed == NULL) {
    // always committed
    memid->initially_committed = true;
  }
  else if (commit) {
    // commit requested, but the range may not be committed as a whole: ensure it is committed now
    memid->initially_committed = true;
    bool any_uncommitted;
    _mi_bitmap_claim_across(arena->blocks_committed, arena->field_count, needed_bcount, bitmap_index, &any_uncommitted);
    if (any_uncommitted) {
      bool commit_zero = false;
      if (!_mi_os_commit(p, mi_arena_block_size(needed_bcount), &commit_zero, tld->stats)) {
        memid->initially_committed = false;
      }
      else {
        if (commit_zero) { memid->initially_zero = true; }
      }
    }
  }
  else {
    // no need to commit, but check if already fully committed
    memid->initially_committed = _mi_bitmap_is_claimed_across(arena->blocks_committed, arena->field_count, needed_bcount, bitmap_index);
  }

  return p;
}

// allocate in a speficic arena
static void* mi_arena_try_alloc_at_id(mi_arena_id_t arena_id, bool match_numa_node, int numa_node, size_t size, size_t alignment,
                                       bool commit, bool allow_large, mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld )
{
  MI_UNUSED_RELEASE(alignment);
  mi_assert(alignment <= MI_SEGMENT_ALIGN);
  const size_t bcount = mi_block_count_of_size(size);
  const size_t arena_index = mi_arena_id_index(arena_id);
  mi_assert_internal(arena_index < mi_atomic_load_relaxed(&mi_arena_count));
  mi_assert_internal(size <= mi_arena_block_size(bcount));

  // Check arena suitability
  mi_arena_t* arena = mi_arena_from_index(arena_index);
  if (arena == NULL) return NULL;
  if (!allow_large && arena->is_large) return NULL;
  if (!mi_arena_id_is_suitable(arena->id, arena->exclusive, req_arena_id)) return NULL;
  if (req_arena_id == _mi_arena_id_none()) { // in not specific, check numa affinity
    const bool numa_suitable = (numa_node < 0 || arena->numa_node < 0 || arena->numa_node == numa_node);
    if (match_numa_node) { if (!numa_suitable) return NULL; }
                    else { if (numa_suitable) return NULL; }
  }

  // try to allocate
  void* p = mi_arena_try_alloc_at(arena, arena_index, bcount, commit, memid, tld);
  mi_assert_internal(p == NULL || _mi_is_aligned(p, alignment));
  return p;
}


// allocate from an arena with fallback to the OS
static mi_decl_noinline void* mi_arena_try_alloc(int numa_node, size_t size, size_t alignment,
                                                  bool commit, bool allow_large,
                                                  mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld )
{
  MI_UNUSED(alignment);
  mi_assert_internal(alignment <= MI_SEGMENT_ALIGN);
  const size_t max_arena = mi_atomic_load_relaxed(&mi_arena_count);
  if mi_likely(max_arena == 0) return NULL;

  if (req_arena_id != _mi_arena_id_none()) {
    // try a specific arena if requested
    if (mi_arena_id_index(req_arena_id) < max_arena) {
      void* p = mi_arena_try_alloc_at_id(req_arena_id, true, numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
      if (p != NULL) return p;
    }
  }
  else {
    // try numa affine allocation
    for (size_t i = 0; i < max_arena; i++) {
      void* p = mi_arena_try_alloc_at_id(mi_arena_id_create(i), true, numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
      if (p != NULL) return p;
    }

    // try from another numa node instead..
    if (numa_node >= 0) {  // if numa_node was < 0 (no specific affinity requested), all arena's have been tried already
      for (size_t i = 0; i < max_arena; i++) {
        void* p = mi_arena_try_alloc_at_id(mi_arena_id_create(i), false /* only proceed if not numa local */, numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
        if (p != NULL) return p;
      }
    }
  }
  return NULL;
}

// try to reserve a fresh arena space
static bool mi_arena_reserve(size_t req_size, bool allow_large, mi_arena_id_t req_arena_id, mi_arena_id_t *arena_id)
{
  if (_mi_preloading()) return false;  // use OS only while pre loading
  if (req_arena_id != _mi_arena_id_none()) return false;

  const size_t arena_count = mi_atomic_load_acquire(&mi_arena_count);
  if (arena_count > (MI_MAX_ARENAS - 4)) return false;

  size_t arena_reserve = mi_option_get_size(mi_option_arena_reserve);
  if (arena_reserve == 0) return false;

  if (!_mi_os_has_virtual_reserve()) {
    arena_reserve = arena_reserve/4;  // be conservative if virtual reserve is not supported (for WASM for example)
  }
  arena_reserve = _mi_align_up(arena_reserve, MI_ARENA_BLOCK_SIZE);
  arena_reserve = _mi_align_up(arena_reserve, MI_SEGMENT_SIZE);
  if (arena_count >= 8 && arena_count <= 128) {
    // scale up the arena sizes exponentially every 8 entries (128 entries get to 589TiB)
    const size_t multiplier = (size_t)1 << _mi_clamp(arena_count/8, 0, 16 );
    size_t reserve = 0;
    if (!mi_mul_overflow(multiplier, arena_reserve, &reserve)) {
      arena_reserve = reserve;
    }
  }
  if (arena_reserve < req_size) return false;  // should be able to at least handle the current allocation size

  // commit eagerly?
  bool arena_commit = false;
  if (mi_option_get(mi_option_arena_eager_commit) == 2)      { arena_commit = _mi_os_has_overcommit(); }
  else if (mi_option_get(mi_option_arena_eager_commit) == 1) { arena_commit = true; }

  return (mi_reserve_os_memory_ex(arena_reserve, arena_commit, allow_large, false /* exclusive? */, arena_id) == 0);
}


void* _mi_arena_alloc_aligned(size_t size, size_t alignment, size_t align_offset, bool commit, bool allow_large,
                              mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld)
{
  mi_assert_internal(memid != NULL && tld != NULL);
  mi_assert_internal(size > 0);
  *memid = _mi_memid_none();

  const int numa_node = _mi_os_numa_node(tld); // current numa node

  // try to allocate in an arena if the alignment is small enough and the object is not too small (as for heap meta data)
  if (!mi_option_is_enabled(mi_option_disallow_arena_alloc) || req_arena_id != _mi_arena_id_none()) {  // is arena allocation allowed?
    if (size >= MI_ARENA_MIN_OBJ_SIZE && alignment <= MI_SEGMENT_ALIGN && align_offset == 0) {
      void* p = mi_arena_try_alloc(numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
      if (p != NULL) return p;

      // otherwise, try to first eagerly reserve a new arena
      if (req_arena_id == _mi_arena_id_none()) {
        mi_arena_id_t arena_id = 0;
        if (mi_arena_reserve(size, allow_large, req_arena_id, &arena_id)) {
          // and try allocate in there
          mi_assert_internal(req_arena_id == _mi_arena_id_none());
          p = mi_arena_try_alloc_at_id(arena_id, true, numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
          if (p != NULL) return p;
        }
      }
    }
  }

  // if we cannot use OS allocation, return NULL
  if (mi_option_is_enabled(mi_option_disallow_os_alloc) || req_arena_id != _mi_arena_id_none()) {
    errno = ENOMEM;
    return NULL;
  }

  // finally, fall back to the OS
  if (align_offset > 0) {
    return _mi_os_alloc_aligned_at_offset(size, alignment, align_offset, commit, allow_large, memid, tld->stats);
  }
  else {
    return _mi_os_alloc_aligned(size, alignment, commit, allow_large, memid, tld->stats);
  }
}

void* _mi_arena_alloc(size_t size, bool commit, bool allow_large, mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld)
{
  return _mi_arena_alloc_aligned(size, MI_ARENA_BLOCK_SIZE, 0, commit, allow_large, req_arena_id, memid, tld);
}


void* mi_arena_area(mi_arena_id_t arena_id, size_t* size) {
  if (size != NULL) *size = 0;
  size_t arena_index = mi_arena_id_index(arena_id);
  if (arena_index >= MI_MAX_ARENAS) return NULL;
  mi_arena_t* arena = mi_atomic_load_ptr_acquire(mi_arena_t, &mi_arenas[arena_index]);
  if (arena == NULL) return NULL;
  if (size != NULL) { *size = mi_arena_block_size(arena->block_count); }
  return arena->start;
}


/* -----------------------------------------------------------
  Arena purge
----------------------------------------------------------- */

static long mi_arena_purge_delay(void) {
  // <0 = no purging allowed, 0=immediate purging, >0=milli-second delay
  return (mi_option_get(mi_option_purge_delay) * mi_option_get(mi_option_arena_purge_mult));
}

// reset or decommit in an arena and update the committed/decommit bitmaps
// assumes we own the area (i.e. blocks_in_use is claimed by us)
static void mi_arena_purge(mi_arena_t* arena, size_t bitmap_idx, size_t blocks, mi_stats_t* stats) {
  mi_assert_internal(arena->blocks_committed != NULL);
  mi_assert_internal(arena->blocks_purge != NULL);
  mi_assert_internal(!arena->memid.is_pinned);
  const size_t size = mi_arena_block_size(blocks);
  void* const p = mi_arena_block_start(arena, bitmap_idx);
  bool needs_recommit;
  if (_mi_bitmap_is_claimed_across(arena->blocks_committed, arena->field_count, blocks, bitmap_idx)) {
    // all blocks are committed, we can purge freely
    needs_recommit = _mi_os_purge(p, size, stats);
  }
  else {
    // some blocks are not committed -- this can happen when a partially committed block is freed
    // in `_mi_arena_free` and it is conservatively marked as uncommitted but still scheduled for a purge
    // we need to ensure we do not try to reset (as that may be invalid for uncommitted memory),
    // and also undo the decommit stats (as it was already adjusted)
    mi_assert_internal(mi_option_is_enabled(mi_option_purge_decommits));
    needs_recommit = _mi_os_purge_ex(p, size, false /* allow reset? */, stats);
    if (needs_recommit) { _mi_stat_increase(&_mi_stats_main.committed, size); }
  }

  // clear the purged blocks
  _mi_bitmap_unclaim_across(arena->blocks_purge, arena->field_count, blocks, bitmap_idx);
  // update committed bitmap
  if (needs_recommit) {
    _mi_bitmap_unclaim_across(arena->blocks_committed, arena->field_count, blocks, bitmap_idx);
  }
}

// Schedule a purge. This is usually delayed to avoid repeated decommit/commit calls.
// Note: assumes we (still) own the area as we may purge immediately
static void mi_arena_schedule_purge(mi_arena_t* arena, size_t bitmap_idx, size_t blocks, mi_stats_t* stats) {
  mi_assert_internal(arena->blocks_purge != NULL);
  const long delay = mi_arena_purge_delay();
  if (delay < 0) return;  // is purging allowed at all?

  if (_mi_preloading() || delay == 0) {
    // decommit directly
    mi_arena_purge(arena, bitmap_idx, blocks, stats);
  }
  else {
    // schedule decommit
    mi_msecs_t expire = mi_atomic_loadi64_relaxed(&arena->purge_expire);
    if (expire != 0) {
      mi_atomic_addi64_acq_rel(&arena->purge_expire, (mi_msecs_t)(delay/10));  // add smallish extra delay
    }
    else {
      mi_atomic_storei64_release(&arena->purge_expire, _mi_clock_now() + delay);
    }
    _mi_bitmap_claim_across(arena->blocks_purge, arena->field_count, blocks, bitmap_idx, NULL);
  }
}

// purge a range of blocks
// return true if the full range was purged.
// assumes we own the area (i.e. blocks_in_use is claimed by us)
static bool mi_arena_purge_range(mi_arena_t* arena, size_t idx, size_t startidx, size_t bitlen, size_t purge, mi_stats_t* stats) {
  const size_t endidx = startidx + bitlen;
  size_t bitidx = startidx;
  bool all_purged = false;
  while (bitidx < endidx) {
    // count consecutive ones in the purge mask
    size_t count = 0;
    while (bitidx + count < endidx && (purge & ((size_t)1 << (bitidx + count))) != 0) {
      count++;
    }
    if (count > 0) {
      // found range to be purged
      const mi_bitmap_index_t range_idx = mi_bitmap_index_create(idx, bitidx);
      mi_arena_purge(arena, range_idx, count, stats);
      if (count == bitlen) {
        all_purged = true;
      }
    }
    bitidx += (count+1); // +1 to skip the zero bit (or end)
  }
  return all_purged;
}

// returns true if anything was purged
static bool mi_arena_try_purge(mi_arena_t* arena, mi_msecs_t now, bool force, mi_stats_t* stats)
{
  if (arena->memid.is_pinned || arena->blocks_purge == NULL) return false;
  mi_msecs_t expire = mi_atomic_loadi64_relaxed(&arena->purge_expire);
  if (expire == 0) return false;
  if (!force && expire > now) return false;

  // reset expire (if not already set concurrently)
  mi_atomic_casi64_strong_acq_rel(&arena->purge_expire, &expire, (mi_msecs_t)0);

  // potential purges scheduled, walk through the bitmap
  bool any_purged = false;
  bool full_purge = true;
  for (size_t i = 0; i < arena->field_count; i++) {
    size_t purge = mi_atomic_load_relaxed(&arena->blocks_purge[i]);
    if (purge != 0) {
      size_t bitidx = 0;
      while (bitidx < MI_BITMAP_FIELD_BITS) {
        // find consecutive range of ones in the purge mask
        size_t bitlen = 0;
        while (bitidx + bitlen < MI_BITMAP_FIELD_BITS && (purge & ((size_t)1 << (bitidx + bitlen))) != 0) {
          bitlen++;
        }
        // temporarily claim the purge range as "in-use" to be thread-safe with allocation
        // try to claim the longest range of corresponding in_use bits
        const mi_bitmap_index_t bitmap_index = mi_bitmap_index_create(i, bitidx);
        while( bitlen > 0 ) {
          if (_mi_bitmap_try_claim(arena->blocks_inuse, arena->field_count, bitlen, bitmap_index)) {
            break;
          }
          bitlen--;
        }
        // actual claimed bits at `in_use`
        if (bitlen > 0) {
          // read purge again now that we have the in_use bits
          purge = mi_atomic_load_acquire(&arena->blocks_purge[i]);
          if (!mi_arena_purge_range(arena, i, bitidx, bitlen, purge, stats)) {
            full_purge = false;
          }
          any_purged = true;
          // release the claimed `in_use` bits again
          _mi_bitmap_unclaim(arena->blocks_inuse, arena->field_count, bitlen, bitmap_index);
        }
        bitidx += (bitlen+1);  // +1 to skip the zero (or end)
      } // while bitidx
    } // purge != 0
  }
  // if not fully purged, make sure to purge again in the future
  if (!full_purge) {
    const long delay = mi_arena_purge_delay();
    mi_msecs_t expected = 0;
    mi_atomic_casi64_strong_acq_rel(&arena->purge_expire,&expected,_mi_clock_now() + delay);
  }
  return any_purged;
}

static void mi_arenas_try_purge( bool force, bool visit_all, mi_stats_t* stats ) {
  if (_mi_preloading() || mi_arena_purge_delay() <= 0) return;  // nothing will be scheduled

  const size_t max_arena = mi_atomic_load_acquire(&mi_arena_count);
  if (max_arena == 0) return;

  // allow only one thread to purge at a time
  static mi_atomic_guard_t purge_guard;
  mi_atomic_guard(&purge_guard)
  {
    mi_msecs_t now = _mi_clock_now();
    size_t max_purge_count = (visit_all ? max_arena : 1);
    for (size_t i = 0; i < max_arena; i++) {
      mi_arena_t* arena = mi_atomic_load_ptr_acquire(mi_arena_t, &mi_arenas[i]);
      if (arena != NULL) {
        if (mi_arena_try_purge(arena, now, force, stats)) {
          if (max_purge_count <= 1) break;
          max_purge_count--;
        }
      }
    }
  }
}


/* -----------------------------------------------------------
  Arena free
----------------------------------------------------------- */

void _mi_arena_free(void* p, size_t size, size_t committed_size, mi_memid_t memid, mi_stats_t* stats) {
  mi_assert_internal(size > 0 && stats != NULL);
  mi_assert_internal(committed_size <= size);
  if (p==NULL) return;
  if (size==0) return;
  const bool all_committed = (committed_size == size);

  // need to set all memory to undefined as some parts may still be marked as no_access (like padding etc.)
  mi_track_mem_undefined(p,size);

  if (mi_memkind_is_os(memid.memkind)) {
    // was a direct OS allocation, pass through
    if (!all_committed && committed_size > 0) {
      // if partially committed, adjust the committed stats (as `_mi_os_free` will increase decommit by the full size)
      _mi_stat_decrease(&_mi_stats_main.committed, committed_size);
    }
    _mi_os_free(p, size, memid, stats);
  }
  else if (memid.memkind == MI_MEM_ARENA) {
    // allocated in an arena
    size_t arena_idx;
    size_t bitmap_idx;
    mi_arena_memid_indices(memid, &arena_idx, &bitmap_idx);
    mi_assert_internal(arena_idx < MI_MAX_ARENAS);
    mi_arena_t* arena = mi_atomic_load_ptr_acquire(mi_arena_t,&mi_arenas[arena_idx]);
    mi_assert_internal(arena != NULL);
    const size_t blocks = mi_block_count_of_size(size);

    // checks
    if (arena == NULL) {
      _mi_error_message(EINVAL, "trying to free from an invalid arena: %p, size %zu, memid: 0x%zx\n", p, size, memid);
      return;
    }
    mi_assert_internal(arena->field_count > mi_bitmap_index_field(bitmap_idx));
    if (arena->field_count <= mi_bitmap_index_field(bitmap_idx)) {
      _mi_error_message(EINVAL, "trying to free from an invalid arena block: %p, size %zu, memid: 0x%zx\n", p, size, memid);
      return;
    }

    // potentially decommit
    if (arena->memid.is_pinned || arena->blocks_committed == NULL) {
      mi_assert_internal(all_committed);
    }
    else {
      mi_assert_internal(arena->blocks_committed != NULL);
      mi_assert_internal(arena->blocks_purge != NULL);

      if (!all_committed) {
        // mark the entire range as no longer committed (so we recommit the full range when re-using)
        _mi_bitmap_unclaim_across(arena->blocks_committed, arena->field_count, blocks, bitmap_idx);
        mi_track_mem_noaccess(p,size);
        if (committed_size > 0) {
          // if partially committed, adjust the committed stats (is it will be recommitted when re-using)
          // in the delayed purge, we now need to not count a decommit if the range is not marked as committed.
          _mi_stat_decrease(&_mi_stats_main.committed, committed_size);
        }
        // note: if not all committed, it may be that the purge will reset/decommit the entire range
        // that contains already decommitted parts. Since purge consistently uses reset or decommit that
        // works (as we should never reset decommitted parts).
      }
      // (delay) purge the entire range
      mi_arena_schedule_purge(arena, bitmap_idx, blocks, stats);
    }

    // and make it available to others again
    bool all_inuse = _mi_bitmap_unclaim_across(arena->blocks_inuse, arena->field_count, blocks, bitmap_idx);
    if (!all_inuse) {
      _mi_error_message(EAGAIN, "trying to free an already freed arena block: %p, size %zu\n", p, size);
      return;
    };
  }
  else {
    // arena was none, external, or static; nothing to do
    mi_assert_internal(memid.memkind < MI_MEM_OS);
  }

  // purge expired decommits
  mi_arenas_try_purge(false, false, stats);
}

// destroy owned arenas; this is unsafe and should only be done using `mi_option_destroy_on_exit`
// for dynamic libraries that are unloaded and need to release all their allocated memory.
static void mi_arenas_unsafe_destroy(void) {
  const size_t max_arena = mi_atomic_load_relaxed(&mi_arena_count);
  size_t new_max_arena = 0;
  for (size_t i = 0; i < max_arena; i++) {
    mi_arena_t* arena = mi_atomic_load_ptr_acquire(mi_arena_t, &mi_arenas[i]);
    if (arena != NULL) {
      mi_lock_done(&arena->abandoned_visit_lock);
      if (arena->start != NULL && mi_memkind_is_os(arena->memid.memkind)) {
        mi_atomic_store_ptr_release(mi_arena_t, &mi_arenas[i], NULL);
        _mi_os_free(arena->start, mi_arena_size(arena), arena->memid, &_mi_stats_main);
      }
      else {
        new_max_arena = i;
      }
      _mi_arena_meta_free(arena, arena->meta_memid, arena->meta_size);
    }
  }

  // try to lower the max arena.
  size_t expected = max_arena;
  mi_atomic_cas_strong_acq_rel(&mi_arena_count, &expected, new_max_arena);
}

// Purge the arenas; if `force_purge` is true, amenable parts are purged even if not yet expired
void _mi_arenas_collect(bool force_purge, mi_stats_t* stats) {
  mi_arenas_try_purge(force_purge, force_purge /* visit all? */, stats);
}

// destroy owned arenas; this is unsafe and should only be done using `mi_option_destroy_on_exit`
// for dynamic libraries that are unloaded and need to release all their allocated memory.
void _mi_arena_unsafe_destroy_all(mi_stats_t* stats) {
  mi_arenas_unsafe_destroy();
  _mi_arenas_collect(true /* force purge */, stats);  // purge non-owned arenas
}

// Is a pointer inside any of our arenas?
bool _mi_arena_contains(const void* p) {
  const size_t max_arena = mi_atomic_load_relaxed(&mi_arena_count);
  for (size_t i = 0; i < max_arena; i++) {
    mi_arena_t* arena = mi_atomic_load_ptr_relaxed(mi_arena_t, &mi_arenas[i]);
    if (arena != NULL && arena->start <= (const uint8_t*)p && arena->start + mi_arena_block_size(arena->block_count) > (const uint8_t*)p) {
      return true;
    }
  }
  return false;
}

/* -----------------------------------------------------------
  Add an arena.
----------------------------------------------------------- */

static bool mi_arena_add(mi_arena_t* arena, mi_arena_id_t* arena_id, mi_stats_t* stats) {
  mi_assert_internal(arena != NULL);
  mi_assert_internal((uintptr_t)mi_atomic_load_ptr_relaxed(uint8_t,&arena->start) % MI_SEGMENT_ALIGN == 0);
  mi_assert_internal(arena->block_count > 0);
  if (arena_id != NULL) { *arena_id = -1; }

  size_t i = mi_atomic_increment_acq_rel(&mi_arena_count);
  if (i >= MI_MAX_ARENAS) {
    mi_atomic_decrement_acq_rel(&mi_arena_count);
    return false;
  }
  _mi_stat_counter_increase(&stats->arena_count,1);
  arena->id = mi_arena_id_create(i);
  mi_atomic_store_ptr_release(mi_arena_t,&mi_arenas[i], arena);
  if (arena_id != NULL) { *arena_id = arena->id; }
  return true;
}

static bool mi_manage_os_memory_ex2(void* start, size_t size, bool is_large, int numa_node, bool exclusive, mi_memid_t memid, mi_arena_id_t* arena_id) mi_attr_noexcept
{
  if (arena_id != NULL) *arena_id = _mi_arena_id_none();
  if (size < MI_ARENA_BLOCK_SIZE) return false;

  if (is_large) {
    mi_assert_internal(memid.initially_committed && memid.is_pinned);
  }

  const size_t bcount = size / MI_ARENA_BLOCK_SIZE;
  const size_t fields = _mi_divide_up(bcount, MI_BITMAP_FIELD_BITS);
  const size_t bitmaps = (memid.is_pinned ? 3 : 5);
  const size_t asize  = sizeof(mi_arena_t) + (bitmaps*fields*sizeof(mi_bitmap_field_t));
  mi_memid_t meta_memid;
  mi_arena_t* arena   = (mi_arena_t*)_mi_arena_meta_zalloc(asize, &meta_memid);
  if (arena == NULL) return false;

  // already zero'd due to zalloc
  // _mi_memzero(arena, asize);
  arena->id = _mi_arena_id_none();
  arena->memid = memid;
  arena->exclusive = exclusive;
  arena->meta_size = asize;
  arena->meta_memid = meta_memid;
  arena->block_count = bcount;
  arena->field_count = fields;
  arena->start = (uint8_t*)start;
  arena->numa_node    = numa_node; // TODO: or get the current numa node if -1? (now it allows anyone to allocate on -1)
  arena->is_large     = is_large;
  arena->purge_expire = 0;
  arena->search_idx   = 0;
  mi_lock_init(&arena->abandoned_visit_lock);
  // consecutive bitmaps
  arena->blocks_dirty     = &arena->blocks_inuse[fields];     // just after inuse bitmap
  arena->blocks_abandoned = &arena->blocks_inuse[2 * fields]; // just after dirty bitmap
  arena->blocks_committed = (arena->memid.is_pinned ? NULL : &arena->blocks_inuse[3*fields]); // just after abandoned bitmap
  arena->blocks_purge     = (arena->memid.is_pinned ? NULL : &arena->blocks_inuse[4*fields]); // just after committed bitmap
  // initialize committed bitmap?
  if (arena->blocks_committed != NULL && arena->memid.initially_committed) {
    memset((void*)arena->blocks_committed, 0xFF, fields*sizeof(mi_bitmap_field_t)); // cast to void* to avoid atomic warning
  }

  // and claim leftover blocks if needed (so we never allocate there)
  ptrdiff_t post = (fields * MI_BITMAP_FIELD_BITS) - bcount;
  mi_assert_internal(post >= 0);
  if (post > 0) {
    // don't use leftover bits at the end
    mi_bitmap_index_t postidx = mi_bitmap_index_create(fields - 1, MI_BITMAP_FIELD_BITS - post);
    _mi_bitmap_claim(arena->blocks_inuse, fields, post, postidx, NULL);
  }
  return mi_arena_add(arena, arena_id, &_mi_stats_main);

}

bool mi_manage_os_memory_ex(void* start, size_t size, bool is_committed, bool is_large, bool is_zero, int numa_node, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept {
  mi_memid_t memid = _mi_memid_create(MI_MEM_EXTERNAL);
  memid.initially_committed = is_committed;
  memid.initially_zero = is_zero;
  memid.is_pinned = is_large;
  return mi_manage_os_memory_ex2(start,size,is_large,numa_node,exclusive,memid, arena_id);
}

// Reserve a range of regular OS memory
int mi_reserve_os_memory_ex(size_t size, bool commit, bool allow_large, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept {
  if (arena_id != NULL) *arena_id = _mi_arena_id_none();
  size = _mi_align_up(size, MI_ARENA_BLOCK_SIZE); // at least one block
  mi_memid_t memid;
  void* start = _mi_os_alloc_aligned(size, MI_SEGMENT_ALIGN, commit, allow_large, &memid, &_mi_stats_main);
  if (start == NULL) return ENOMEM;
  const bool is_large = memid.is_pinned; // todo: use separate is_large field?
  if (!mi_manage_os_memory_ex2(start, size, is_large, -1 /* numa node */, exclusive, memid, arena_id)) {
    _mi_os_free_ex(start, size, commit, memid, &_mi_stats_main);
    _mi_verbose_message("failed to reserve %zu KiB memory\n", _mi_divide_up(size, 1024));
    return ENOMEM;
  }
  _mi_verbose_message("reserved %zu KiB memory%s\n", _mi_divide_up(size, 1024), is_large ? " (in large os pages)" : "");
  return 0;
}


// Manage a range of regular OS memory
bool mi_manage_os_memory(void* start, size_t size, bool is_committed, bool is_large, bool is_zero, int numa_node) mi_attr_noexcept {
  return mi_manage_os_memory_ex(start, size, is_committed, is_large, is_zero, numa_node, false /* exclusive? */, NULL);
}

// Reserve a range of regular OS memory
int mi_reserve_os_memory(size_t size, bool commit, bool allow_large) mi_attr_noexcept {
  return mi_reserve_os_memory_ex(size, commit, allow_large, false, NULL);
}


/* -----------------------------------------------------------
  Debugging
----------------------------------------------------------- */

static size_t mi_debug_show_bitmap(const char* prefix, const char* header, size_t block_count, mi_bitmap_field_t* fields, size_t field_count ) {
  _mi_verbose_message("%s%s:\n", prefix, header);
  size_t bcount = 0;
  size_t inuse_count = 0;
  for (size_t i = 0; i < field_count; i++) {
    char buf[MI_BITMAP_FIELD_BITS + 1];
    uintptr_t field = mi_atomic_load_relaxed(&fields[i]);
    for (size_t bit = 0; bit < MI_BITMAP_FIELD_BITS; bit++, bcount++) {
      if (bcount < block_count) {
        bool inuse = ((((uintptr_t)1 << bit) & field) != 0);
        if (inuse) inuse_count++;
        buf[bit] = (inuse ? 'x' : '.');
      }
      else {
        buf[bit] = ' ';
      }
    }
    buf[MI_BITMAP_FIELD_BITS] = 0;
    _mi_verbose_message("%s  %s\n", prefix, buf);
  }
  _mi_verbose_message("%s  total ('x'): %zu\n", prefix, inuse_count);
  return inuse_count;
}

void mi_debug_show_arenas(bool show_inuse, bool show_abandoned, bool show_purge) mi_attr_noexcept {
  size_t max_arenas = mi_atomic_load_relaxed(&mi_arena_count);
  size_t inuse_total = 0;
  size_t abandoned_total = 0;
  size_t purge_total = 0;
  for (size_t i = 0; i < max_arenas; i++) {
    mi_arena_t* arena = mi_atomic_load_ptr_relaxed(mi_arena_t, &mi_arenas[i]);
    if (arena == NULL) break;
    _mi_verbose_message("arena %zu: %zu blocks of size %zuMiB (in %zu fields) %s\n", i, arena->block_count, MI_ARENA_BLOCK_SIZE / MI_MiB, arena->field_count, (arena->memid.is_pinned ? ", pinned" : ""));
    if (show_inuse) {
      inuse_total += mi_debug_show_bitmap("  ", "inuse blocks", arena->block_count, arena->blocks_inuse, arena->field_count);
    }
    if (arena->blocks_committed != NULL) {
      mi_debug_show_bitmap("  ", "committed blocks", arena->block_count, arena->blocks_committed, arena->field_count);
    }
    if (show_abandoned) {
      abandoned_total += mi_debug_show_bitmap("  ", "abandoned blocks", arena->block_count, arena->blocks_abandoned, arena->field_count);
    }
    if (show_purge && arena->blocks_purge != NULL) {
      purge_total += mi_debug_show_bitmap("  ", "purgeable blocks", arena->block_count, arena->blocks_purge, arena->field_count);
    }
  }
  if (show_inuse)     _mi_verbose_message("total inuse blocks    : %zu\n", inuse_total);
  if (show_abandoned) _mi_verbose_message("total abandoned blocks: %zu\n", abandoned_total);
  if (show_purge)     _mi_verbose_message("total purgeable blocks: %zu\n", purge_total);
}


/* -----------------------------------------------------------
  Reserve a huge page arena.
----------------------------------------------------------- */
// reserve at a specific numa node
int mi_reserve_huge_os_pages_at_ex(size_t pages, int numa_node, size_t timeout_msecs, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept {
  if (arena_id != NULL) *arena_id = -1;
  if (pages==0) return 0;
  if (numa_node < -1) numa_node = -1;
  if (numa_node >= 0) numa_node = numa_node % _mi_os_numa_node_count();
  size_t hsize = 0;
  size_t pages_reserved = 0;
  mi_memid_t memid;
  void* p = _mi_os_alloc_huge_os_pages(pages, numa_node, timeout_msecs, &pages_reserved, &hsize, &memid);
  if (p==NULL || pages_reserved==0) {
    _mi_warning_message("failed to reserve %zu GiB huge pages\n", pages);
    return ENOMEM;
  }
  _mi_verbose_message("numa node %i: reserved %zu GiB huge pages (of the %zu GiB requested)\n", numa_node, pages_reserved, pages);

  if (!mi_manage_os_memory_ex2(p, hsize, true, numa_node, exclusive, memid, arena_id)) {
    _mi_os_free(p, hsize, memid, &_mi_stats_main);
    return ENOMEM;
  }
  return 0;
}

int mi_reserve_huge_os_pages_at(size_t pages, int numa_node, size_t timeout_msecs) mi_attr_noexcept {
  return mi_reserve_huge_os_pages_at_ex(pages, numa_node, timeout_msecs, false, NULL);
}

// reserve huge pages evenly among the given number of numa nodes (or use the available ones as detected)
int mi_reserve_huge_os_pages_interleave(size_t pages, size_t numa_nodes, size_t timeout_msecs) mi_attr_noexcept {
  if (pages == 0) return 0;

  // pages per numa node
  size_t numa_count = (numa_nodes > 0 ? numa_nodes : _mi_os_numa_node_count());
  if (numa_count <= 0) numa_count = 1;
  const size_t pages_per = pages / numa_count;
  const size_t pages_mod = pages % numa_count;
  const size_t timeout_per = (timeout_msecs==0 ? 0 : (timeout_msecs / numa_count) + 50);

  // reserve evenly among numa nodes
  for (size_t numa_node = 0; numa_node < numa_count && pages > 0; numa_node++) {
    size_t node_pages = pages_per;  // can be 0
    if (numa_node < pages_mod) node_pages++;
    int err = mi_reserve_huge_os_pages_at(node_pages, (int)numa_node, timeout_per);
    if (err) return err;
    if (pages < node_pages) {
      pages = 0;
    }
    else {
      pages -= node_pages;
    }
  }

  return 0;
}

int mi_reserve_huge_os_pages(size_t pages, double max_secs, size_t* pages_reserved) mi_attr_noexcept {
  MI_UNUSED(max_secs);
  _mi_warning_message("mi_reserve_huge_os_pages is deprecated: use mi_reserve_huge_os_pages_interleave/at instead\n");
  if (pages_reserved != NULL) *pages_reserved = 0;
  int err = mi_reserve_huge_os_pages_interleave(pages, 0, (size_t)(max_secs * 1000.0));
  if (err==0 && pages_reserved!=NULL) *pages_reserved = pages;
  return err;
}


// --- Amalgamation END --- mimalloc/src/arena.c


// --- Amalgamation START --- mimalloc/src/bitmap.c
/* ----------------------------------------------------------------------------
Copyright (c) 2019-2023 Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------
Concurrent bitmap that can set/reset sequences of bits atomically,
represented as an array of fields where each field is a machine word (`size_t`)

There are two api's; the standard one cannot have sequences that cross
between the bitmap fields (and a sequence must be <= MI_BITMAP_FIELD_BITS).

The `_across` postfixed functions do allow sequences that can cross over
between the fields. (This is used in arena allocation)
---------------------------------------------------------------------------- */


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/src/bitmap.h
// --- Amalgamation END --- mimalloc/src/bitmap.h


/* -----------------------------------------------------------
  Bitmap definition
----------------------------------------------------------- */

// The bit mask for a given number of blocks at a specified bit index.
static inline size_t mi_bitmap_mask_(size_t count, size_t bitidx) {
  mi_assert_internal(count + bitidx <= MI_BITMAP_FIELD_BITS);
  mi_assert_internal(count > 0);
  if (count >= MI_BITMAP_FIELD_BITS) return MI_BITMAP_FIELD_FULL;
  if (count == 0) return 0;
  return ((((size_t)1 << count) - 1) << bitidx);
}



/* -----------------------------------------------------------
  Claim a bit sequence atomically
----------------------------------------------------------- */

// Try to atomically claim a sequence of `count` bits in a single
// field at `idx` in `bitmap`. Returns `true` on success.
bool _mi_bitmap_try_find_claim_field(mi_bitmap_t bitmap, size_t idx, const size_t count, mi_bitmap_index_t* bitmap_idx)
{
  mi_assert_internal(bitmap_idx != NULL);
  mi_assert_internal(count <= MI_BITMAP_FIELD_BITS);
  mi_bitmap_field_t* field = &bitmap[idx];
  size_t map  = mi_atomic_load_relaxed(field);
  if (map==MI_BITMAP_FIELD_FULL) return false; // short cut

  // search for 0-bit sequence of length count
  const size_t mask = mi_bitmap_mask_(count, 0);
  const size_t bitidx_max = MI_BITMAP_FIELD_BITS - count;

#ifdef MI_HAVE_FAST_BITSCAN
  size_t bitidx = mi_ctz(~map);    // quickly find the first zero bit if possible
#else
  size_t bitidx = 0;               // otherwise start at 0
#endif // 7977
  size_t m = (mask << bitidx);     // invariant: m == mask shifted by bitidx

  // scan linearly for a free range of zero bits
  while (bitidx <= bitidx_max) {
    const size_t mapm = (map & m);
    if (mapm == 0) {  // are the mask bits free at bitidx?
      mi_assert_internal((m >> bitidx) == mask); // no overflow?
      const size_t newmap = (map | m);
      mi_assert_internal((newmap^map) >> bitidx == mask);
      if (!mi_atomic_cas_strong_acq_rel(field, &map, newmap)) {  // TODO: use weak cas here?
        // no success, another thread claimed concurrently.. keep going (with updated `map`)
        continue;
      }
      else {
        // success, we claimed the bits!
        *bitmap_idx = mi_bitmap_index_create(idx, bitidx);
        return true;
      }
    }
    else {
      // on to the next bit range
#ifdef MI_HAVE_FAST_BITSCAN
      mi_assert_internal(mapm != 0);
      const size_t shift = (count == 1 ? 1 : (MI_INTPTR_BITS - mi_clz(mapm) - bitidx));
      mi_assert_internal(shift > 0 && shift <= count);
#else
      const size_t shift = 1;
#endif // 8003
      bitidx += shift;
      m <<= shift;
    }
  }
  // no bits found
  return false;
}


// Starts at idx, and wraps around to search in all `bitmap_fields` fields.
// For now, `count` can be at most MI_BITMAP_FIELD_BITS and will never cross fields.
bool _mi_bitmap_try_find_from_claim(mi_bitmap_t bitmap, const size_t bitmap_fields, const size_t start_field_idx, const size_t count, mi_bitmap_index_t* bitmap_idx) {
  size_t idx = start_field_idx;
  for (size_t visited = 0; visited < bitmap_fields; visited++, idx++) {
    if (idx >= bitmap_fields) { idx = 0; } // wrap
    if (_mi_bitmap_try_find_claim_field(bitmap, idx, count, bitmap_idx)) {
      return true;
    }
  }
  return false;
}


// Set `count` bits at `bitmap_idx` to 0 atomically
// Returns `true` if all `count` bits were 1 previously.
bool _mi_bitmap_unclaim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  const size_t idx = mi_bitmap_index_field(bitmap_idx);
  const size_t bitidx = mi_bitmap_index_bit_in_field(bitmap_idx);
  const size_t mask = mi_bitmap_mask_(count, bitidx);
  mi_assert_internal(bitmap_fields > idx); MI_UNUSED(bitmap_fields);
  // mi_assert_internal((bitmap[idx] & mask) == mask);
  const size_t prev = mi_atomic_and_acq_rel(&bitmap[idx], ~mask);
  return ((prev & mask) == mask);
}


// Set `count` bits at `bitmap_idx` to 1 atomically
// Returns `true` if all `count` bits were 0 previously. `any_zero` is `true` if there was at least one zero bit.
bool _mi_bitmap_claim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* any_zero) {
  const size_t idx = mi_bitmap_index_field(bitmap_idx);
  const size_t bitidx = mi_bitmap_index_bit_in_field(bitmap_idx);
  const size_t mask = mi_bitmap_mask_(count, bitidx);
  mi_assert_internal(bitmap_fields > idx); MI_UNUSED(bitmap_fields);
  //mi_assert_internal(any_zero != NULL || (bitmap[idx] & mask) == 0);
  size_t prev = mi_atomic_or_acq_rel(&bitmap[idx], mask);
  if (any_zero != NULL) { *any_zero = ((prev & mask) != mask); }
  return ((prev & mask) == 0);
}

// Returns `true` if all `count` bits were 1. `any_ones` is `true` if there was at least one bit set to one.
static bool mi_bitmap_is_claimedx(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* any_ones) {
  const size_t idx = mi_bitmap_index_field(bitmap_idx);
  const size_t bitidx = mi_bitmap_index_bit_in_field(bitmap_idx);
  const size_t mask = mi_bitmap_mask_(count, bitidx);
  mi_assert_internal(bitmap_fields > idx); MI_UNUSED(bitmap_fields);
  const size_t field = mi_atomic_load_relaxed(&bitmap[idx]);
  if (any_ones != NULL) { *any_ones = ((field & mask) != 0); }
  return ((field & mask) == mask);
}

// Try to set `count` bits at `bitmap_idx` from 0 to 1 atomically. 
// Returns `true` if successful when all previous `count` bits were 0.
bool _mi_bitmap_try_claim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  const size_t idx = mi_bitmap_index_field(bitmap_idx);
  const size_t bitidx = mi_bitmap_index_bit_in_field(bitmap_idx);
  const size_t mask = mi_bitmap_mask_(count, bitidx);
  mi_assert_internal(bitmap_fields > idx); MI_UNUSED(bitmap_fields);
  size_t expected = mi_atomic_load_relaxed(&bitmap[idx]);
  do  {    
    if ((expected & mask) != 0) return false;
  } 
  while (!mi_atomic_cas_strong_acq_rel(&bitmap[idx], &expected, expected | mask));
  mi_assert_internal((expected & mask) == 0);
  return true;
}


bool _mi_bitmap_is_claimed(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  return mi_bitmap_is_claimedx(bitmap, bitmap_fields, count, bitmap_idx, NULL);
}

bool _mi_bitmap_is_any_claimed(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  bool any_ones;
  mi_bitmap_is_claimedx(bitmap, bitmap_fields, count, bitmap_idx, &any_ones);
  return any_ones;
}


//--------------------------------------------------------------------------
// the `_across` functions work on bitmaps where sequences can cross over
// between the fields. This is used in arena allocation
//--------------------------------------------------------------------------

// Try to atomically claim a sequence of `count` bits starting from the field
// at `idx` in `bitmap` and crossing into subsequent fields. Returns `true` on success.
// Only needs to consider crossing into the next fields (see `mi_bitmap_try_find_from_claim_across`)
static bool mi_bitmap_try_find_claim_field_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t idx, const size_t count, const size_t retries, mi_bitmap_index_t* bitmap_idx, mi_stats_t* stats)
{
  mi_assert_internal(bitmap_idx != NULL);

  // check initial trailing zeros
  mi_bitmap_field_t* field = &bitmap[idx];
  size_t map = mi_atomic_load_relaxed(field);
  const size_t initial = mi_clz(map);  // count of initial zeros starting at idx
  mi_assert_internal(initial <= MI_BITMAP_FIELD_BITS);
  if (initial == 0)     return false;
  if (initial >= count) return _mi_bitmap_try_find_claim_field(bitmap, idx, count, bitmap_idx);    // no need to cross fields (this case won't happen for us)
  if (_mi_divide_up(count - initial, MI_BITMAP_FIELD_BITS) >= (bitmap_fields - idx)) return false; // not enough entries
  
  // scan ahead
  size_t found = initial;
  size_t mask = 0;     // mask bits for the final field
  while(found < count) {
    field++;
    map = mi_atomic_load_relaxed(field);
    const size_t mask_bits = (found + MI_BITMAP_FIELD_BITS <= count ? MI_BITMAP_FIELD_BITS : (count - found));
    mi_assert_internal(mask_bits > 0 && mask_bits <= MI_BITMAP_FIELD_BITS);
    mask = mi_bitmap_mask_(mask_bits, 0);
    if ((map & mask) != 0) return false;  // some part is already claimed
    found += mask_bits;
  }
  mi_assert_internal(field < &bitmap[bitmap_fields]);

  // we found a range of contiguous zeros up to the final field; mask contains mask in the final field
  // now try to claim the range atomically
  mi_bitmap_field_t* const final_field = field;
  const size_t final_mask = mask;
  mi_bitmap_field_t* const initial_field = &bitmap[idx];
  const size_t initial_idx = MI_BITMAP_FIELD_BITS - initial;
  const size_t initial_mask = mi_bitmap_mask_(initial, initial_idx);

  // initial field
  size_t newmap;
  field = initial_field;
  map = mi_atomic_load_relaxed(field);
  do {
    newmap = (map | initial_mask);
    if ((map & initial_mask) != 0) { goto rollback; };
  } while (!mi_atomic_cas_strong_acq_rel(field, &map, newmap));

  // intermediate fields
  while (++field < final_field) {
    newmap = mi_bitmap_mask_(MI_BITMAP_FIELD_BITS, 0);
    map = 0;
    if (!mi_atomic_cas_strong_acq_rel(field, &map, newmap)) { goto rollback; }
  }

  // final field
  mi_assert_internal(field == final_field);
  map = mi_atomic_load_relaxed(field);
  do {
    newmap = (map | final_mask);
    if ((map & final_mask) != 0) { goto rollback; }
  } while (!mi_atomic_cas_strong_acq_rel(field, &map, newmap));

  // claimed!
  mi_stat_counter_increase(stats->arena_crossover_count,1);
  *bitmap_idx = mi_bitmap_index_create(idx, initial_idx);
  return true;

rollback:
  // roll back intermediate fields
  // (we just failed to claim `field` so decrement first)
  while (--field > initial_field) {
    newmap = 0;
    map = mi_bitmap_mask_(MI_BITMAP_FIELD_BITS, 0);
    mi_assert_internal(mi_atomic_load_relaxed(field) == map);
    mi_atomic_store_release(field, newmap);
  }
  if (field == initial_field) {               // (if we failed on the initial field, `field + 1 == initial_field`)
    map = mi_atomic_load_relaxed(field);
    do {
      mi_assert_internal((map & initial_mask) == initial_mask);
      newmap = (map & ~initial_mask);
    } while (!mi_atomic_cas_strong_acq_rel(field, &map, newmap));
  }
  mi_stat_counter_increase(stats->arena_rollback_count,1);
  // retry? (we make a recursive call instead of goto to be able to use const declarations)
  if (retries <= 2) {
    return mi_bitmap_try_find_claim_field_across(bitmap, bitmap_fields, idx, count, retries+1, bitmap_idx, stats);
  }
  else {
    return false;
  }
}


// Find `count` bits of zeros and set them to 1 atomically; returns `true` on success.
// Starts at idx, and wraps around to search in all `bitmap_fields` fields.
bool _mi_bitmap_try_find_from_claim_across(mi_bitmap_t bitmap, const size_t bitmap_fields, const size_t start_field_idx, const size_t count, mi_bitmap_index_t* bitmap_idx, mi_stats_t* stats) {
  mi_assert_internal(count > 0);
  if (count <= 2) {
    // we don't bother with crossover fields for small counts
    return _mi_bitmap_try_find_from_claim(bitmap, bitmap_fields, start_field_idx, count, bitmap_idx);
  }

  // visit the fields
  size_t idx = start_field_idx;
  for (size_t visited = 0; visited < bitmap_fields; visited++, idx++) {
    if (idx >= bitmap_fields) { idx = 0; } // wrap
    // first try to claim inside a field
    /*
    if (count <= MI_BITMAP_FIELD_BITS) {
      if (_mi_bitmap_try_find_claim_field(bitmap, idx, count, bitmap_idx)) {
        return true;
      }
    }
    */
    // if that fails, then try to claim across fields
    if (mi_bitmap_try_find_claim_field_across(bitmap, bitmap_fields, idx, count, 0, bitmap_idx, stats)) {
      return true;
    }
  }
  return false;
}

// Helper for masks across fields; returns the mid count, post_mask may be 0
static size_t mi_bitmap_mask_across(mi_bitmap_index_t bitmap_idx, size_t bitmap_fields, size_t count, size_t* pre_mask, size_t* mid_mask, size_t* post_mask) {
  MI_UNUSED(bitmap_fields);
  const size_t bitidx = mi_bitmap_index_bit_in_field(bitmap_idx);
  if mi_likely(bitidx + count <= MI_BITMAP_FIELD_BITS) {
    *pre_mask = mi_bitmap_mask_(count, bitidx);
    *mid_mask = 0;
    *post_mask = 0;
    mi_assert_internal(mi_bitmap_index_field(bitmap_idx) < bitmap_fields);
    return 0;
  }
  else {
    const size_t pre_bits = MI_BITMAP_FIELD_BITS - bitidx;
    mi_assert_internal(pre_bits < count);
    *pre_mask = mi_bitmap_mask_(pre_bits, bitidx);
    count -= pre_bits;
    const size_t mid_count = (count / MI_BITMAP_FIELD_BITS);
    *mid_mask = MI_BITMAP_FIELD_FULL;
    count %= MI_BITMAP_FIELD_BITS;
    *post_mask = (count==0 ? 0 : mi_bitmap_mask_(count, 0));
    mi_assert_internal(mi_bitmap_index_field(bitmap_idx) + mid_count + (count==0 ? 0 : 1) < bitmap_fields);
    return mid_count;
  }
}

// Set `count` bits at `bitmap_idx` to 0 atomically
// Returns `true` if all `count` bits were 1 previously.
bool _mi_bitmap_unclaim_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  size_t idx = mi_bitmap_index_field(bitmap_idx);
  size_t pre_mask;
  size_t mid_mask;
  size_t post_mask;
  size_t mid_count = mi_bitmap_mask_across(bitmap_idx, bitmap_fields, count, &pre_mask, &mid_mask, &post_mask);
  bool all_one = true;
  mi_bitmap_field_t* field = &bitmap[idx];
  size_t prev = mi_atomic_and_acq_rel(field++, ~pre_mask);   // clear first part
  if ((prev & pre_mask) != pre_mask) all_one = false;
  while(mid_count-- > 0) {
    prev = mi_atomic_and_acq_rel(field++, ~mid_mask);        // clear mid part
    if ((prev & mid_mask) != mid_mask) all_one = false;
  }
  if (post_mask!=0) {
    prev = mi_atomic_and_acq_rel(field, ~post_mask);         // clear end part
    if ((prev & post_mask) != post_mask) all_one = false;
  }
  return all_one;
}

// Set `count` bits at `bitmap_idx` to 1 atomically
// Returns `true` if all `count` bits were 0 previously. `any_zero` is `true` if there was at least one zero bit.
bool _mi_bitmap_claim_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* pany_zero) {
  size_t idx = mi_bitmap_index_field(bitmap_idx);
  size_t pre_mask;
  size_t mid_mask;
  size_t post_mask;
  size_t mid_count = mi_bitmap_mask_across(bitmap_idx, bitmap_fields, count, &pre_mask, &mid_mask, &post_mask);
  bool all_zero = true;
  bool any_zero = false;
  _Atomic(size_t)*field = &bitmap[idx];
  size_t prev = mi_atomic_or_acq_rel(field++, pre_mask);
  if ((prev & pre_mask) != 0) all_zero = false;
  if ((prev & pre_mask) != pre_mask) any_zero = true;
  while (mid_count-- > 0) {
    prev = mi_atomic_or_acq_rel(field++, mid_mask);
    if ((prev & mid_mask) != 0) all_zero = false;
    if ((prev & mid_mask) != mid_mask) any_zero = true;
  }
  if (post_mask!=0) {
    prev = mi_atomic_or_acq_rel(field, post_mask);
    if ((prev & post_mask) != 0) all_zero = false;
    if ((prev & post_mask) != post_mask) any_zero = true;
  }
  if (pany_zero != NULL) { *pany_zero = any_zero; }
  return all_zero;
}


// Returns `true` if all `count` bits were 1.
// `any_ones` is `true` if there was at least one bit set to one.
static bool mi_bitmap_is_claimedx_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* pany_ones) {
  size_t idx = mi_bitmap_index_field(bitmap_idx);
  size_t pre_mask;
  size_t mid_mask;
  size_t post_mask;
  size_t mid_count = mi_bitmap_mask_across(bitmap_idx, bitmap_fields, count, &pre_mask, &mid_mask, &post_mask);
  bool all_ones = true;
  bool any_ones = false;
  mi_bitmap_field_t* field = &bitmap[idx];
  size_t prev = mi_atomic_load_relaxed(field++);
  if ((prev & pre_mask) != pre_mask) all_ones = false;
  if ((prev & pre_mask) != 0) any_ones = true;
  while (mid_count-- > 0) {
    prev = mi_atomic_load_relaxed(field++);
    if ((prev & mid_mask) != mid_mask) all_ones = false;
    if ((prev & mid_mask) != 0) any_ones = true;
  }
  if (post_mask!=0) {
    prev = mi_atomic_load_relaxed(field);
    if ((prev & post_mask) != post_mask) all_ones = false;
    if ((prev & post_mask) != 0) any_ones = true;
  }
  if (pany_ones != NULL) { *pany_ones = any_ones; }
  return all_ones;
}

bool _mi_bitmap_is_claimed_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  return mi_bitmap_is_claimedx_across(bitmap, bitmap_fields, count, bitmap_idx, NULL);
}

bool _mi_bitmap_is_any_claimed_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  bool any_ones;
  mi_bitmap_is_claimedx_across(bitmap, bitmap_fields, count, bitmap_idx, &any_ones);
  return any_ones;
}
// --- Amalgamation END --- mimalloc/src/bitmap.c


// --- Amalgamation START --- mimalloc/src/heap.c
/*----------------------------------------------------------------------------
Copyright (c) 2018-2021, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


#include <string.h>  // memset, memcpy

#if defined(_MSC_VER) && (_MSC_VER < 1920)
#pragma warning(disable:4204)  // non-constant aggregate initializer
#endif // 8370

/* -----------------------------------------------------------
  Helpers
----------------------------------------------------------- */

// return `true` if ok, `false` to break
typedef bool (heap_page_visitor_fun)(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* arg1, void* arg2);

// Visit all pages in a heap; returns `false` if break was called.
static bool mi_heap_visit_pages(mi_heap_t* heap, heap_page_visitor_fun* fn, void* arg1, void* arg2)
{
  if (heap==NULL || heap->page_count==0) return 0;

  // visit all pages
  #if MI_DEBUG>1
  size_t total = heap->page_count;
  size_t count = 0;
  #endif // 8387

  for (size_t i = 0; i <= MI_BIN_FULL; i++) {
    mi_page_queue_t* pq = &heap->pages[i];
    mi_page_t* page = pq->first;
    while(page != NULL) {
      mi_page_t* next = page->next; // save next in case the page gets removed from the queue
      mi_assert_internal(mi_page_heap(page) == heap);
      #if MI_DEBUG>1
      count++;
      #endif // 8398
      if (!fn(heap, pq, page, arg1, arg2)) return false;
      page = next; // and continue
    }
  }
  mi_assert_internal(count == total);
  return true;
}


#if MI_DEBUG>=2
static bool mi_heap_page_is_valid(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* arg1, void* arg2) {
  MI_UNUSED(arg1);
  MI_UNUSED(arg2);
  MI_UNUSED(pq);
  mi_assert_internal(mi_page_heap(page) == heap);
  mi_segment_t* segment = _mi_page_segment(page);
  mi_assert_internal(segment->thread_id == heap->thread_id);
  mi_assert_expensive(_mi_page_is_valid(page));
  return true;
}
#endif // 8410
#if MI_DEBUG>=3
static bool mi_heap_is_valid(mi_heap_t* heap) {
  mi_assert_internal(heap!=NULL);
  mi_heap_visit_pages(heap, &mi_heap_page_is_valid, NULL, NULL);
  return true;
}
#endif // 8422




/* -----------------------------------------------------------
  "Collect" pages by migrating `local_free` and `thread_free`
  lists and freeing empty pages. This is done when a thread
  stops (and in that case abandons pages if there are still
  blocks alive)
----------------------------------------------------------- */

typedef enum mi_collect_e {
  MI_NORMAL,
  MI_FORCE,
  MI_ABANDON
} mi_collect_t;


static bool mi_heap_page_collect(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* arg_collect, void* arg2 ) {
  MI_UNUSED(arg2);
  MI_UNUSED(heap);
  mi_assert_internal(mi_heap_page_is_valid(heap, pq, page, NULL, NULL));
  mi_collect_t collect = *((mi_collect_t*)arg_collect);
  _mi_page_free_collect(page, collect >= MI_FORCE);
  if (mi_page_all_free(page)) {
    // no more used blocks, free the page.
    // note: this will free retired pages as well.
    _mi_page_free(page, pq, collect >= MI_FORCE);
  }
  else if (collect == MI_ABANDON) {
    // still used blocks but the thread is done; abandon the page
    _mi_page_abandon(page, pq);
  }
  return true; // don't break
}

static bool mi_heap_page_never_delayed_free(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* arg1, void* arg2) {
  MI_UNUSED(arg1);
  MI_UNUSED(arg2);
  MI_UNUSED(heap);
  MI_UNUSED(pq);
  _mi_page_use_delayed_free(page, MI_NEVER_DELAYED_FREE, false);
  return true; // don't break
}

static void mi_heap_collect_ex(mi_heap_t* heap, mi_collect_t collect)
{
  if (heap==NULL || !mi_heap_is_initialized(heap)) return;

  const bool force = (collect >= MI_FORCE);
  _mi_deferred_free(heap, force);

  // python/cpython#112532: we may be called from a thread that is not the owner of the heap
  const bool is_main_thread = (_mi_is_main_thread() && heap->thread_id == _mi_thread_id());

  // note: never reclaim on collect but leave it to threads that need storage to reclaim
  if (
  #ifdef NDEBUG
      collect == MI_FORCE
  #else
      collect >= MI_FORCE
  #endif // 8486
    && is_main_thread && mi_heap_is_backing(heap) && !heap->no_reclaim)
  {
    // the main thread is abandoned (end-of-program), try to reclaim all abandoned segments.
    // if all memory is freed by now, all segments should be freed.
    // note: this only collects in the current subprocess
    _mi_abandoned_reclaim_all(heap, &heap->tld->segments);
  }

  // if abandoning, mark all pages to no longer add to delayed_free
  if (collect == MI_ABANDON) {
    mi_heap_visit_pages(heap, &mi_heap_page_never_delayed_free, NULL, NULL);
  }

  // free all current thread delayed blocks.
  // (if abandoning, after this there are no more thread-delayed references into the pages.)
  _mi_heap_delayed_free_all(heap);

  // collect retired pages
  _mi_heap_collect_retired(heap, force);

  // collect all pages owned by this thread
  mi_heap_visit_pages(heap, &mi_heap_page_collect, &collect, NULL);
  mi_assert_internal( collect != MI_ABANDON || mi_atomic_load_ptr_acquire(mi_block_t,&heap->thread_delayed_free) == NULL );

  // collect segments (purge pages, this can be expensive so don't force on abandonment)
  _mi_segments_collect(collect == MI_FORCE, &heap->tld->segments);

  // if forced, collect thread data cache on program-exit (or shared library unload)
  if (force && is_main_thread && mi_heap_is_backing(heap)) {
    _mi_thread_data_collect();  // collect thread data cache
  }

  // collect arenas (this is program wide so don't force purges on abandonment of threads)
  _mi_arenas_collect(collect == MI_FORCE /* force purge? */, &heap->tld->stats);
}

void _mi_heap_collect_abandon(mi_heap_t* heap) {
  mi_heap_collect_ex(heap, MI_ABANDON);
}

void mi_heap_collect(mi_heap_t* heap, bool force) mi_attr_noexcept {
  mi_heap_collect_ex(heap, (force ? MI_FORCE : MI_NORMAL));
}

void mi_collect(bool force) mi_attr_noexcept {
  mi_heap_collect(mi_prim_get_default_heap(), force);
}


/* -----------------------------------------------------------
  Heap new
----------------------------------------------------------- */

mi_heap_t* mi_heap_get_default(void) {
  mi_thread_init();
  return mi_prim_get_default_heap();
}

static bool mi_heap_is_default(const mi_heap_t* heap) {
  return (heap == mi_prim_get_default_heap());
}


mi_heap_t* mi_heap_get_backing(void) {
  mi_heap_t* heap = mi_heap_get_default();
  mi_assert_internal(heap!=NULL);
  mi_heap_t* bheap = heap->tld->heap_backing;
  mi_assert_internal(bheap!=NULL);
  mi_assert_internal(bheap->thread_id == _mi_thread_id());
  return bheap;
}

void _mi_heap_init(mi_heap_t* heap, mi_tld_t* tld, mi_arena_id_t arena_id, bool noreclaim, uint8_t tag) {
  _mi_memcpy_aligned(heap, &_mi_heap_empty, sizeof(mi_heap_t));
  heap->tld = tld;
  heap->thread_id  = _mi_thread_id();
  heap->arena_id   = arena_id;
  heap->no_reclaim = noreclaim;
  heap->tag        = tag;
  if (heap == tld->heap_backing) {
    _mi_random_init(&heap->random);
  }
  else {
    _mi_random_split(&tld->heap_backing->random, &heap->random);
  }
  heap->cookie  = _mi_heap_random_next(heap) | 1;
  heap->keys[0] = _mi_heap_random_next(heap);
  heap->keys[1] = _mi_heap_random_next(heap);
  _mi_heap_guarded_init(heap);
  // push on the thread local heaps list
  heap->next = heap->tld->heaps;
  heap->tld->heaps = heap;
}

mi_decl_nodiscard mi_heap_t* mi_heap_new_ex(int heap_tag, bool allow_destroy, mi_arena_id_t arena_id) {
  mi_heap_t* bheap = mi_heap_get_backing();
  mi_heap_t* heap = mi_heap_malloc_tp(bheap, mi_heap_t);  // todo: OS allocate in secure mode?
  if (heap == NULL) return NULL;
  mi_assert(heap_tag >= 0 && heap_tag < 256);
  _mi_heap_init(heap, bheap->tld, arena_id, allow_destroy /* no reclaim? */, (uint8_t)heap_tag /* heap tag */);
  return heap;
}

mi_decl_nodiscard mi_heap_t* mi_heap_new_in_arena(mi_arena_id_t arena_id) {
  return mi_heap_new_ex(0 /* default heap tag */, false /* don't allow `mi_heap_destroy` */, arena_id);
}

mi_decl_nodiscard mi_heap_t* mi_heap_new(void) {
  // don't reclaim abandoned memory or otherwise destroy is unsafe
  return mi_heap_new_ex(0 /* default heap tag */, true /* no reclaim */, _mi_arena_id_none());
}

bool _mi_heap_memid_is_suitable(mi_heap_t* heap, mi_memid_t memid) {
  return _mi_arena_memid_is_suitable(memid, heap->arena_id);
}

uintptr_t _mi_heap_random_next(mi_heap_t* heap) {
  return _mi_random_next(&heap->random);
}

// zero out the page queues
static void mi_heap_reset_pages(mi_heap_t* heap) {
  mi_assert_internal(heap != NULL);
  mi_assert_internal(mi_heap_is_initialized(heap));
  // TODO: copy full empty heap instead?
  memset(&heap->pages_free_direct, 0, sizeof(heap->pages_free_direct));
  _mi_memcpy_aligned(&heap->pages, &_mi_heap_empty.pages, sizeof(heap->pages));
  heap->thread_delayed_free = NULL;
  heap->page_count = 0;
}

// called from `mi_heap_destroy` and `mi_heap_delete` to free the internal heap resources.
static void mi_heap_free(mi_heap_t* heap) {
  mi_assert(heap != NULL);
  mi_assert_internal(mi_heap_is_initialized(heap));
  if (heap==NULL || !mi_heap_is_initialized(heap)) return;
  if (mi_heap_is_backing(heap)) return; // dont free the backing heap

  // reset default
  if (mi_heap_is_default(heap)) {
    _mi_heap_set_default_direct(heap->tld->heap_backing);
  }

  // remove ourselves from the thread local heaps list
  // linear search but we expect the number of heaps to be relatively small
  mi_heap_t* prev = NULL;
  mi_heap_t* curr = heap->tld->heaps;
  while (curr != heap && curr != NULL) {
    prev = curr;
    curr = curr->next;
  }
  mi_assert_internal(curr == heap);
  if (curr == heap) {
    if (prev != NULL) { prev->next = heap->next; }
                 else { heap->tld->heaps = heap->next; }
  }
  mi_assert_internal(heap->tld->heaps != NULL);

  // and free the used memory
  mi_free(heap);
}

// return a heap on the same thread as `heap` specialized for the specified tag (if it exists)
mi_heap_t* _mi_heap_by_tag(mi_heap_t* heap, uint8_t tag) {
  if (heap->tag == tag) {
    return heap;
  }
  for (mi_heap_t *curr = heap->tld->heaps; curr != NULL; curr = curr->next) {
    if (curr->tag == tag) {
      return curr;
    }
  }
  return NULL;
}

/* -----------------------------------------------------------
  Heap destroy
----------------------------------------------------------- */

static bool _mi_heap_page_destroy(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* arg1, void* arg2) {
  MI_UNUSED(arg1);
  MI_UNUSED(arg2);
  MI_UNUSED(heap);
  MI_UNUSED(pq);

  // ensure no more thread_delayed_free will be added
  _mi_page_use_delayed_free(page, MI_NEVER_DELAYED_FREE, false);

  // stats
  const size_t bsize = mi_page_block_size(page);
  if (bsize > MI_LARGE_OBJ_SIZE_MAX) {
    mi_heap_stat_decrease(heap, huge, bsize);
  }
#if (MI_STAT)
  _mi_page_free_collect(page, false);  // update used count
  const size_t inuse = page->used;
  if (bsize <= MI_LARGE_OBJ_SIZE_MAX) {
    mi_heap_stat_decrease(heap, normal, bsize * inuse);
#if (MI_STAT>1)
    mi_heap_stat_decrease(heap, normal_bins[_mi_bin(bsize)], inuse);
#endif // 8689
  }
  mi_heap_stat_decrease(heap, malloc, bsize * inuse);  // todo: off for aligned blocks...
#endif // 8684

  /// pretend it is all free now
  mi_assert_internal(mi_page_thread_free(page) == NULL);
  page->used = 0;

  // and free the page
  // mi_page_free(page,false);
  page->next = NULL;
  page->prev = NULL;
  _mi_segment_page_free(page,false /* no force? */, &heap->tld->segments);

  return true; // keep going
}

void _mi_heap_destroy_pages(mi_heap_t* heap) {
  mi_heap_visit_pages(heap, &_mi_heap_page_destroy, NULL, NULL);
  mi_heap_reset_pages(heap);
}

#if MI_TRACK_HEAP_DESTROY
static bool mi_cdecl mi_heap_track_block_free(const mi_heap_t* heap, const mi_heap_area_t* area, void* block, size_t block_size, void* arg) {
  MI_UNUSED(heap); MI_UNUSED(area);  MI_UNUSED(arg); MI_UNUSED(block_size);
  mi_track_free_size(block,mi_usable_size(block));
  return true;
}
#endif // 8714

void mi_heap_destroy(mi_heap_t* heap) {
  mi_assert(heap != NULL);
  mi_assert(mi_heap_is_initialized(heap));
  mi_assert(heap->no_reclaim);
  mi_assert_expensive(mi_heap_is_valid(heap));
  if (heap==NULL || !mi_heap_is_initialized(heap)) return;
  #if MI_GUARDED
  // _mi_warning_message("'mi_heap_destroy' called but MI_GUARDED is enabled -- using `mi_heap_delete` instead (heap at %p)\n", heap);
  mi_heap_delete(heap);
  return;
  #else
  if (!heap->no_reclaim) {
    _mi_warning_message("'mi_heap_destroy' called but ignored as the heap was not created with 'allow_destroy' (heap at %p)\n", heap);
    // don't free in case it may contain reclaimed pages
    mi_heap_delete(heap);
  }
  else {
    // track all blocks as freed
    #if MI_TRACK_HEAP_DESTROY
    mi_heap_visit_blocks(heap, true, mi_heap_track_block_free, NULL);
    #endif // 8740
    // free all pages
    _mi_heap_destroy_pages(heap);
    mi_heap_free(heap);
  }
  #endif // 8728
}

// forcefully destroy all heaps in the current thread
void _mi_heap_unsafe_destroy_all(void) {
  mi_heap_t* bheap = mi_heap_get_backing();
  mi_heap_t* curr = bheap->tld->heaps;
  while (curr != NULL) {
    mi_heap_t* next = curr->next;
    if (curr->no_reclaim) {
      mi_heap_destroy(curr);
    }
    else {
      _mi_heap_destroy_pages(curr);
    }
    curr = next;
  }
}

/* -----------------------------------------------------------
  Safe Heap delete
----------------------------------------------------------- */

// Transfer the pages from one heap to the other
static void mi_heap_absorb(mi_heap_t* heap, mi_heap_t* from) {
  mi_assert_internal(heap!=NULL);
  if (from==NULL || from->page_count == 0) return;

  // reduce the size of the delayed frees
  _mi_heap_delayed_free_partial(from);

  // transfer all pages by appending the queues; this will set a new heap field
  // so threads may do delayed frees in either heap for a while.
  // note: appending waits for each page to not be in the `MI_DELAYED_FREEING` state
  // so after this only the new heap will get delayed frees
  for (size_t i = 0; i <= MI_BIN_FULL; i++) {
    mi_page_queue_t* pq = &heap->pages[i];
    mi_page_queue_t* append = &from->pages[i];
    size_t pcount = _mi_page_queue_append(heap, pq, append);
    heap->page_count += pcount;
    from->page_count -= pcount;
  }
  mi_assert_internal(from->page_count == 0);

  // and do outstanding delayed frees in the `from` heap
  // note: be careful here as the `heap` field in all those pages no longer point to `from`,
  // turns out to be ok as `_mi_heap_delayed_free` only visits the list and calls a
  // the regular `_mi_free_delayed_block` which is safe.
  _mi_heap_delayed_free_all(from);
  #if !defined(_MSC_VER) || (_MSC_VER > 1900) // somehow the following line gives an error in VS2015, issue #353
  mi_assert_internal(mi_atomic_load_ptr_relaxed(mi_block_t,&from->thread_delayed_free) == NULL);
  #endif // 8796

  // and reset the `from` heap
  mi_heap_reset_pages(from);
}

// Safe delete a heap without freeing any still allocated blocks in that heap.
void mi_heap_delete(mi_heap_t* heap)
{
  mi_assert(heap != NULL);
  mi_assert(mi_heap_is_initialized(heap));
  mi_assert_expensive(mi_heap_is_valid(heap));
  if (heap==NULL || !mi_heap_is_initialized(heap)) return;

  if (!mi_heap_is_backing(heap)) {
    // transfer still used pages to the backing heap
    mi_heap_absorb(heap->tld->heap_backing, heap);
  }
  else {
    // the backing heap abandons its pages
    _mi_heap_collect_abandon(heap);
  }
  mi_assert_internal(heap->page_count==0);
  mi_heap_free(heap);
}

mi_heap_t* mi_heap_set_default(mi_heap_t* heap) {
  mi_assert(heap != NULL);
  mi_assert(mi_heap_is_initialized(heap));
  if (heap==NULL || !mi_heap_is_initialized(heap)) return NULL;
  mi_assert_expensive(mi_heap_is_valid(heap));
  mi_heap_t* old = mi_prim_get_default_heap();
  _mi_heap_set_default_direct(heap);
  return old;
}




/* -----------------------------------------------------------
  Analysis
----------------------------------------------------------- */

// static since it is not thread safe to access heaps from other threads.
static mi_heap_t* mi_heap_of_block(const void* p) {
  if (p == NULL) return NULL;
  mi_segment_t* segment = _mi_ptr_segment(p);
  bool valid = (_mi_ptr_cookie(segment) == segment->cookie);
  mi_assert_internal(valid);
  if mi_unlikely(!valid) return NULL;
  return mi_page_heap(_mi_segment_page_of(segment,p));
}

bool mi_heap_contains_block(mi_heap_t* heap, const void* p) {
  mi_assert(heap != NULL);
  if (heap==NULL || !mi_heap_is_initialized(heap)) return false;
  return (heap == mi_heap_of_block(p));
}


static bool mi_heap_page_check_owned(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* p, void* vfound) {
  MI_UNUSED(heap);
  MI_UNUSED(pq);
  bool* found = (bool*)vfound;
  void* start = mi_page_start(page);
  void* end   = (uint8_t*)start + (page->capacity * mi_page_block_size(page));
  *found = (p >= start && p < end);
  return (!*found); // continue if not found
}

bool mi_heap_check_owned(mi_heap_t* heap, const void* p) {
  mi_assert(heap != NULL);
  if (heap==NULL || !mi_heap_is_initialized(heap)) return false;
  if (((uintptr_t)p & (MI_INTPTR_SIZE - 1)) != 0) return false;  // only aligned pointers
  bool found = false;
  mi_heap_visit_pages(heap, &mi_heap_page_check_owned, (void*)p, &found);
  return found;
}

bool mi_check_owned(const void* p) {
  return mi_heap_check_owned(mi_prim_get_default_heap(), p);
}

/* -----------------------------------------------------------
  Visit all heap blocks and areas
  Todo: enable visiting abandoned pages, and
        enable visiting all blocks of all heaps across threads
----------------------------------------------------------- */

void _mi_heap_area_init(mi_heap_area_t* area, mi_page_t* page) {
  const size_t bsize = mi_page_block_size(page);
  const size_t ubsize = mi_page_usable_block_size(page);
  area->reserved = page->reserved * bsize;
  area->committed = page->capacity * bsize;
  area->blocks = mi_page_start(page);
  area->used = page->used;   // number of blocks in use (#553)
  area->block_size = ubsize;
  area->full_block_size = bsize;
  area->heap_tag = page->heap_tag;
}


static void mi_get_fast_divisor(size_t divisor, uint64_t* magic, size_t* shift) {
  mi_assert_internal(divisor > 0 && divisor <= UINT32_MAX);
  *shift = MI_INTPTR_BITS - mi_clz(divisor - 1);
  *magic = ((((uint64_t)1 << 32) * (((uint64_t)1 << *shift) - divisor)) / divisor + 1);
}

static size_t mi_fast_divide(size_t n, uint64_t magic, size_t shift) {
  mi_assert_internal(n <= UINT32_MAX);
  const uint64_t hi = ((uint64_t)n * magic) >> 32;
  return (size_t)((hi + n) >> shift);
}

bool _mi_heap_area_visit_blocks(const mi_heap_area_t* area, mi_page_t* page, mi_block_visit_fun* visitor, void* arg) {
  mi_assert(area != NULL);
  if (area==NULL) return true;
  mi_assert(page != NULL);
  if (page == NULL) return true;

  _mi_page_free_collect(page,true);              // collect both thread_delayed and local_free
  mi_assert_internal(page->local_free == NULL);
  if (page->used == 0) return true;

  size_t psize;
  uint8_t* const pstart = _mi_segment_page_start(_mi_page_segment(page), page, &psize);
  mi_heap_t* const heap = mi_page_heap(page);
  const size_t bsize    = mi_page_block_size(page);
  const size_t ubsize   = mi_page_usable_block_size(page); // without padding

  // optimize page with one block
  if (page->capacity == 1) {
    mi_assert_internal(page->used == 1 && page->free == NULL);
    return visitor(mi_page_heap(page), area, pstart, ubsize, arg);
  }
  mi_assert(bsize <= UINT32_MAX);

  // optimize full pages
  if (page->used == page->capacity) {
    uint8_t* block = pstart;
    for (size_t i = 0; i < page->capacity; i++) {
      if (!visitor(heap, area, block, ubsize, arg)) return false;
      block += bsize;
    }
    return true;
  }

  // create a bitmap of free blocks.
  #define MI_MAX_BLOCKS   (MI_SMALL_PAGE_SIZE / sizeof(void*))
  uintptr_t free_map[MI_MAX_BLOCKS / MI_INTPTR_BITS];
  const uintptr_t bmapsize = _mi_divide_up(page->capacity, MI_INTPTR_BITS);
  memset(free_map, 0, bmapsize * sizeof(intptr_t));
  if (page->capacity % MI_INTPTR_BITS != 0) {
    // mark left-over bits at the end as free
    size_t shift   = (page->capacity % MI_INTPTR_BITS);
    uintptr_t mask = (UINTPTR_MAX << shift);
    free_map[bmapsize - 1] = mask;
  }

  // fast repeated division by the block size
  uint64_t magic;
  size_t   shift;
  mi_get_fast_divisor(bsize, &magic, &shift);

  #if MI_DEBUG>1
  size_t free_count = 0;
  #endif // 8962
  for (mi_block_t* block = page->free; block != NULL; block = mi_block_next(page, block)) {
    #if MI_DEBUG>1
    free_count++;
    #endif // 8966
    mi_assert_internal((uint8_t*)block >= pstart && (uint8_t*)block < (pstart + psize));
    size_t offset = (uint8_t*)block - pstart;
    mi_assert_internal(offset % bsize == 0);
    mi_assert_internal(offset <= UINT32_MAX);
    size_t blockidx = mi_fast_divide(offset, magic, shift);
    mi_assert_internal(blockidx == offset / bsize);
    mi_assert_internal(blockidx < MI_MAX_BLOCKS);
    size_t bitidx = (blockidx / MI_INTPTR_BITS);
    size_t bit = blockidx - (bitidx * MI_INTPTR_BITS);
    free_map[bitidx] |= ((uintptr_t)1 << bit);
  }
  mi_assert_internal(page->capacity == (free_count + page->used));

  // walk through all blocks skipping the free ones
  #if MI_DEBUG>1
  size_t used_count = 0;
  #endif // 8983
  uint8_t* block = pstart;
  for (size_t i = 0; i < bmapsize; i++) {
    if (free_map[i] == 0) {
      // every block is in use
      for (size_t j = 0; j < MI_INTPTR_BITS; j++) {
        #if MI_DEBUG>1
        used_count++;
        #endif // 8991
        if (!visitor(heap, area, block, ubsize, arg)) return false;
        block += bsize;
      }
    }
    else {
      // visit the used blocks in the mask
      uintptr_t m = ~free_map[i];
      while (m != 0) {
        #if MI_DEBUG>1
        used_count++;
        #endif // 9002
        size_t bitidx = mi_ctz(m);
        if (!visitor(heap, area, block + (bitidx * bsize), ubsize, arg)) return false;
        m &= m - 1;  // clear least significant bit
      }
      block += bsize * MI_INTPTR_BITS;
    }
  }
  mi_assert_internal(page->used == used_count);
  return true;
}



// Separate struct to keep `mi_page_t` out of the public interface
typedef struct mi_heap_area_ex_s {
  mi_heap_area_t area;
  mi_page_t* page;
} mi_heap_area_ex_t;

typedef bool (mi_heap_area_visit_fun)(const mi_heap_t* heap, const mi_heap_area_ex_t* area, void* arg);

static bool mi_heap_visit_areas_page(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* vfun, void* arg) {
  MI_UNUSED(heap);
  MI_UNUSED(pq);
  mi_heap_area_visit_fun* fun = (mi_heap_area_visit_fun*)vfun;
  mi_heap_area_ex_t xarea;
  xarea.page = page;
  _mi_heap_area_init(&xarea.area, page);
  return fun(heap, &xarea, arg);
}

// Visit all heap pages as areas
static bool mi_heap_visit_areas(const mi_heap_t* heap, mi_heap_area_visit_fun* visitor, void* arg) {
  if (visitor == NULL) return false;
  return mi_heap_visit_pages((mi_heap_t*)heap, &mi_heap_visit_areas_page, (void*)(visitor), arg); // note: function pointer to void* :-{
}

// Just to pass arguments
typedef struct mi_visit_blocks_args_s {
  bool  visit_blocks;
  mi_block_visit_fun* visitor;
  void* arg;
} mi_visit_blocks_args_t;

static bool mi_heap_area_visitor(const mi_heap_t* heap, const mi_heap_area_ex_t* xarea, void* arg) {
  mi_visit_blocks_args_t* args = (mi_visit_blocks_args_t*)arg;
  if (!args->visitor(heap, &xarea->area, NULL, xarea->area.block_size, args->arg)) return false;
  if (args->visit_blocks) {
    return _mi_heap_area_visit_blocks(&xarea->area, xarea->page, args->visitor, args->arg);
  }
  else {
    return true;
  }
}

// Visit all blocks in a heap
bool mi_heap_visit_blocks(const mi_heap_t* heap, bool visit_blocks, mi_block_visit_fun* visitor, void* arg) {
  mi_visit_blocks_args_t args = { visit_blocks, visitor, arg };
  return mi_heap_visit_areas(heap, &mi_heap_area_visitor, &args);
}
// --- Amalgamation END --- mimalloc/src/heap.c


// --- Amalgamation START --- mimalloc/src/init.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2022, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


#include <string.h>  // memcpy, memset
#include <stdlib.h>  // atexit


// Empty page used to initialize the small free pages array
const mi_page_t _mi_page_empty = {
  0,
  false, false, false, false,
  0,       // capacity
  0,       // reserved capacity
  { 0 },   // flags
  false,   // is_zero
  0,       // retire_expire
  NULL,    // free
  NULL,    // local_free
  0,       // used
  0,       // block size shift
  0,       // heap tag
  0,       // block_size
  NULL,    // page_start
  #if (MI_PADDING || MI_ENCODE_FREELIST)
  { 0, 0 },
  #endif // 9108
  MI_ATOMIC_VAR_INIT(0), // xthread_free
  MI_ATOMIC_VAR_INIT(0), // xheap
  NULL, NULL
  #if MI_INTPTR_SIZE==4
  , { NULL }
  #endif // 9114
};

#define MI_PAGE_EMPTY() ((mi_page_t*)&_mi_page_empty)

#if (MI_PADDING>0) && (MI_INTPTR_SIZE >= 8)
#define MI_SMALL_PAGES_EMPTY  { MI_INIT128(MI_PAGE_EMPTY), MI_PAGE_EMPTY(), MI_PAGE_EMPTY() }
#elif (MI_PADDING>0)
#define MI_SMALL_PAGES_EMPTY  { MI_INIT128(MI_PAGE_EMPTY), MI_PAGE_EMPTY(), MI_PAGE_EMPTY(), MI_PAGE_EMPTY() }
#else
#define MI_SMALL_PAGES_EMPTY  { MI_INIT128(MI_PAGE_EMPTY), MI_PAGE_EMPTY() }
#endif // 9121


// Empty page queues for every bin
#define QNULL(sz)  { NULL, NULL, (sz)*sizeof(uintptr_t) }
#define MI_PAGE_QUEUES_EMPTY \
  { QNULL(1), \
    QNULL(     1), QNULL(     2), QNULL(     3), QNULL(     4), QNULL(     5), QNULL(     6), QNULL(     7), QNULL(     8), /* 8 */ \
    QNULL(    10), QNULL(    12), QNULL(    14), QNULL(    16), QNULL(    20), QNULL(    24), QNULL(    28), QNULL(    32), /* 16 */ \
    QNULL(    40), QNULL(    48), QNULL(    56), QNULL(    64), QNULL(    80), QNULL(    96), QNULL(   112), QNULL(   128), /* 24 */ \
    QNULL(   160), QNULL(   192), QNULL(   224), QNULL(   256), QNULL(   320), QNULL(   384), QNULL(   448), QNULL(   512), /* 32 */ \
    QNULL(   640), QNULL(   768), QNULL(   896), QNULL(  1024), QNULL(  1280), QNULL(  1536), QNULL(  1792), QNULL(  2048), /* 40 */ \
    QNULL(  2560), QNULL(  3072), QNULL(  3584), QNULL(  4096), QNULL(  5120), QNULL(  6144), QNULL(  7168), QNULL(  8192), /* 48 */ \
    QNULL( 10240), QNULL( 12288), QNULL( 14336), QNULL( 16384), QNULL( 20480), QNULL( 24576), QNULL( 28672), QNULL( 32768), /* 56 */ \
    QNULL( 40960), QNULL( 49152), QNULL( 57344), QNULL( 65536), QNULL( 81920), QNULL( 98304), QNULL(114688), QNULL(131072), /* 64 */ \
    QNULL(163840), QNULL(196608), QNULL(229376), QNULL(262144), QNULL(327680), QNULL(393216), QNULL(458752), QNULL(524288), /* 72 */ \
    QNULL(MI_LARGE_OBJ_WSIZE_MAX + 1  /* 655360, Huge queue */), \
    QNULL(MI_LARGE_OBJ_WSIZE_MAX + 2) /* Full queue */ }

#define MI_STAT_COUNT_NULL()  {0,0,0,0}

// Empty statistics
#if MI_STAT>1
#define MI_STAT_COUNT_END_NULL()  , { MI_STAT_COUNT_NULL(), MI_INIT32(MI_STAT_COUNT_NULL) }
#else
#define MI_STAT_COUNT_END_NULL()
#endif // 9149

#define MI_STATS_NULL  \
  MI_STAT_COUNT_NULL(), MI_STAT_COUNT_NULL(), \
  MI_STAT_COUNT_NULL(), MI_STAT_COUNT_NULL(), \
  MI_STAT_COUNT_NULL(), MI_STAT_COUNT_NULL(), \
  MI_STAT_COUNT_NULL(), MI_STAT_COUNT_NULL(), \
  MI_STAT_COUNT_NULL(), MI_STAT_COUNT_NULL(), \
  MI_STAT_COUNT_NULL(), MI_STAT_COUNT_NULL(), \
  MI_STAT_COUNT_NULL(), MI_STAT_COUNT_NULL(), \
  MI_STAT_COUNT_NULL(), \
  { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
  { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
  { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, \
  { 0, 0 } \
  MI_STAT_COUNT_END_NULL()

// --------------------------------------------------------
// Statically allocate an empty heap as the initial
// thread local value for the default heap,
// and statically allocate the backing heap for the main
// thread so it can function without doing any allocation
// itself (as accessing a thread local for the first time
// may lead to allocation itself on some platforms)
// --------------------------------------------------------

mi_decl_cache_align const mi_heap_t _mi_heap_empty = {
  NULL,
  MI_ATOMIC_VAR_INIT(NULL),
  0,                // tid
  0,                // cookie
  0,                // arena id
  { 0, 0 },         // keys
  { {0}, {0}, 0, true }, // random
  0,                // page count
  MI_BIN_FULL, 0,   // page retired min/max
  NULL,             // next
  false,            // can reclaim
  0,                // tag
  #if MI_GUARDED
  0, 0, 0, 0, 1,    // count is 1 so we never write to it (see `internal.h:mi_heap_malloc_use_guarded`)
  #endif // 9192
  MI_SMALL_PAGES_EMPTY,
  MI_PAGE_QUEUES_EMPTY
};


mi_threadid_t _mi_thread_id(void) mi_attr_noexcept {
  return _mi_prim_thread_id();
}

// the thread-local default heap for allocation
mi_decl_thread mi_heap_t* _mi_heap_default = (mi_heap_t*)&_mi_heap_empty;

extern mi_heap_t _mi_heap_main;

static mi_decl_cache_align mi_subproc_t mi_subproc_default;

static mi_decl_cache_align mi_tld_t tld_main = {
  0, false,
  &_mi_heap_main, &_mi_heap_main,
  { { NULL, NULL }, {NULL ,NULL}, {NULL ,NULL, 0},
    0, 0, 0, 0, 0, &mi_subproc_default,
    &tld_main.stats, &tld_main.os
  }, // segments
  { 0, &tld_main.stats },  // os
  { MI_STATS_NULL }       // stats
};

mi_decl_cache_align mi_heap_t _mi_heap_main = {
  &tld_main,
  MI_ATOMIC_VAR_INIT(NULL),
  0,                // thread id
  0,                // initial cookie
  0,                // arena id
  { 0, 0 },         // the key of the main heap can be fixed (unlike page keys that need to be secure!)
  { {0x846ca68b}, {0}, 0, true },  // random
  0,                // page count
  MI_BIN_FULL, 0,   // page retired min/max
  NULL,             // next heap
  false,            // can reclaim
  0,                // tag
  #if MI_GUARDED
  0, 0, 0, 0, 0,
  #endif // 9235
  MI_SMALL_PAGES_EMPTY,
  MI_PAGE_QUEUES_EMPTY
};

bool _mi_process_is_initialized = false;  // set to `true` in `mi_process_init`.

mi_stats_t _mi_stats_main = { MI_STATS_NULL };

#if MI_GUARDED
mi_decl_export void mi_heap_guarded_set_sample_rate(mi_heap_t* heap, size_t sample_rate, size_t seed) {
  heap->guarded_sample_seed = seed;
  if (heap->guarded_sample_seed == 0) { 
    heap->guarded_sample_seed = _mi_heap_random_next(heap); 
  }
  heap->guarded_sample_rate  = sample_rate;
  if (heap->guarded_sample_rate >= 1) {
    heap->guarded_sample_seed = heap->guarded_sample_seed % heap->guarded_sample_rate;
  }
  heap->guarded_sample_count = heap->guarded_sample_seed;  // count down samples
}

mi_decl_export void mi_heap_guarded_set_size_bound(mi_heap_t* heap, size_t min, size_t max) {
  heap->guarded_size_min = min;
  heap->guarded_size_max = (min > max ? min : max);
}

void _mi_heap_guarded_init(mi_heap_t* heap) {
  mi_heap_guarded_set_sample_rate(heap,
    (size_t)mi_option_get_clamp(mi_option_guarded_sample_rate, 0, LONG_MAX),
    (size_t)mi_option_get(mi_option_guarded_sample_seed));
  mi_heap_guarded_set_size_bound(heap, 
    (size_t)mi_option_get_clamp(mi_option_guarded_min, 0, LONG_MAX),
    (size_t)mi_option_get_clamp(mi_option_guarded_max, 0, LONG_MAX) );  
}
#else
mi_decl_export void mi_heap_guarded_set_sample_rate(mi_heap_t* heap, size_t sample_rate, size_t seed) {
  MI_UNUSED(heap); MI_UNUSED(sample_rate); MI_UNUSED(seed);
}

mi_decl_export void mi_heap_guarded_set_size_bound(mi_heap_t* heap, size_t min, size_t max) {
  MI_UNUSED(heap); MI_UNUSED(min); MI_UNUSED(max);
}
void _mi_heap_guarded_init(mi_heap_t* heap) {
  MI_UNUSED(heap);
}
#endif // 9246


static void mi_heap_main_init(void) {
  if (_mi_heap_main.cookie == 0) {
    _mi_heap_main.thread_id = _mi_thread_id();
    _mi_heap_main.cookie = 1;
    #if defined(_WIN32) && !defined(MI_SHARED_LIB)
      _mi_random_init_weak(&_mi_heap_main.random);    // prevent allocation failure during bcrypt dll initialization with static linking
    #else
      _mi_random_init(&_mi_heap_main.random);
    #endif // 9290
    _mi_heap_main.cookie  = _mi_heap_random_next(&_mi_heap_main);
    _mi_heap_main.keys[0] = _mi_heap_random_next(&_mi_heap_main);
    _mi_heap_main.keys[1] = _mi_heap_random_next(&_mi_heap_main);
    mi_lock_init(&mi_subproc_default.abandoned_os_lock);
    mi_lock_init(&mi_subproc_default.abandoned_os_visit_lock);
    _mi_heap_guarded_init(&_mi_heap_main);
  }
}

mi_heap_t* _mi_heap_main_get(void) {
  mi_heap_main_init();
  return &_mi_heap_main;
}


/* -----------------------------------------------------------
  Sub process
----------------------------------------------------------- */

mi_subproc_id_t mi_subproc_main(void) {
  return NULL;
}

mi_subproc_id_t mi_subproc_new(void) {
  mi_memid_t memid = _mi_memid_none();
  mi_subproc_t* subproc = (mi_subproc_t*)_mi_arena_meta_zalloc(sizeof(mi_subproc_t), &memid);
  if (subproc == NULL) return NULL;
  subproc->memid = memid;
  subproc->abandoned_os_list = NULL;
  mi_lock_init(&subproc->abandoned_os_lock);
  mi_lock_init(&subproc->abandoned_os_visit_lock);
  return subproc;
}

mi_subproc_t* _mi_subproc_from_id(mi_subproc_id_t subproc_id) {
  return (subproc_id == NULL ? &mi_subproc_default : (mi_subproc_t*)subproc_id);
}

void mi_subproc_delete(mi_subproc_id_t subproc_id) {
  if (subproc_id == NULL) return;
  mi_subproc_t* subproc = _mi_subproc_from_id(subproc_id);
  // check if there are no abandoned segments still..
  bool safe_to_delete = false;
  if (mi_lock_acquire(&subproc->abandoned_os_lock)) {
    if (subproc->abandoned_os_list == NULL) {
      safe_to_delete = true;
    }
    mi_lock_release(&subproc->abandoned_os_lock);
  }
  if (!safe_to_delete) return;
  // safe to release
  // todo: should we refcount subprocesses?
  mi_lock_done(&subproc->abandoned_os_lock);
  mi_lock_done(&subproc->abandoned_os_visit_lock);
  _mi_arena_meta_free(subproc, subproc->memid, sizeof(mi_subproc_t));
}

void mi_subproc_add_current_thread(mi_subproc_id_t subproc_id) {
  mi_heap_t* heap = mi_heap_get_default();
  if (heap == NULL) return;
  mi_assert(heap->tld->segments.subproc == &mi_subproc_default);
  if (heap->tld->segments.subproc != &mi_subproc_default) return;
  heap->tld->segments.subproc = _mi_subproc_from_id(subproc_id);
}



/* -----------------------------------------------------------
  Initialization and freeing of the thread local heaps
----------------------------------------------------------- */

// note: in x64 in release build `sizeof(mi_thread_data_t)` is under 4KiB (= OS page size).
typedef struct mi_thread_data_s {
  mi_heap_t  heap;   // must come first due to cast in `_mi_heap_done`
  mi_tld_t   tld;
  mi_memid_t memid;  // must come last due to zero'ing
} mi_thread_data_t;


// Thread meta-data is allocated directly from the OS. For
// some programs that do not use thread pools and allocate and
// destroy many OS threads, this may causes too much overhead
// per thread so we maintain a small cache of recently freed metadata.

#define TD_CACHE_SIZE (32)
static _Atomic(mi_thread_data_t*) td_cache[TD_CACHE_SIZE];

static mi_thread_data_t* mi_thread_data_zalloc(void) {
  // try to find thread metadata in the cache
  bool is_zero = false;
  mi_thread_data_t* td = NULL;
  for (int i = 0; i < TD_CACHE_SIZE; i++) {
    td = mi_atomic_load_ptr_relaxed(mi_thread_data_t, &td_cache[i]);
    if (td != NULL) {
      // found cached allocation, try use it
      td = mi_atomic_exchange_ptr_acq_rel(mi_thread_data_t, &td_cache[i], NULL);
      if (td != NULL) {
        break;
      }
    }
  }

  // if that fails, allocate as meta data
  if (td == NULL) {
    mi_memid_t memid;
    td = (mi_thread_data_t*)_mi_os_alloc(sizeof(mi_thread_data_t), &memid, &_mi_stats_main);
    if (td == NULL) {
      // if this fails, try once more. (issue #257)
      td = (mi_thread_data_t*)_mi_os_alloc(sizeof(mi_thread_data_t), &memid, &_mi_stats_main);
      if (td == NULL) {
        // really out of memory
        _mi_error_message(ENOMEM, "unable to allocate thread local heap metadata (%zu bytes)\n", sizeof(mi_thread_data_t));
      }
    }
    if (td != NULL) {
      td->memid = memid;
      is_zero = memid.initially_zero;
    }
  }

  if (td != NULL && !is_zero) {
    _mi_memzero_aligned(td, offsetof(mi_thread_data_t,memid));
  }
  return td;
}

static void mi_thread_data_free( mi_thread_data_t* tdfree ) {
  // try to add the thread metadata to the cache
  for (int i = 0; i < TD_CACHE_SIZE; i++) {
    mi_thread_data_t* td = mi_atomic_load_ptr_relaxed(mi_thread_data_t, &td_cache[i]);
    if (td == NULL) {
      mi_thread_data_t* expected = NULL;
      if (mi_atomic_cas_ptr_weak_acq_rel(mi_thread_data_t, &td_cache[i], &expected, tdfree)) {
        return;
      }
    }
  }
  // if that fails, just free it directly
  _mi_os_free(tdfree, sizeof(mi_thread_data_t), tdfree->memid, &_mi_stats_main);
}

void _mi_thread_data_collect(void) {
  // free all thread metadata from the cache
  for (int i = 0; i < TD_CACHE_SIZE; i++) {
    mi_thread_data_t* td = mi_atomic_load_ptr_relaxed(mi_thread_data_t, &td_cache[i]);
    if (td != NULL) {
      td = mi_atomic_exchange_ptr_acq_rel(mi_thread_data_t, &td_cache[i], NULL);
      if (td != NULL) {
        _mi_os_free(td, sizeof(mi_thread_data_t), td->memid, &_mi_stats_main);
      }
    }
  }
}

// Initialize the thread local default heap, called from `mi_thread_init`
static bool _mi_thread_heap_init(void) {
  if (mi_heap_is_initialized(mi_prim_get_default_heap())) return true;
  if (_mi_is_main_thread()) {
    // mi_assert_internal(_mi_heap_main.thread_id != 0);  // can happen on freeBSD where alloc is called before any initialization
    // the main heap is statically allocated
    mi_heap_main_init();
    _mi_heap_set_default_direct(&_mi_heap_main);
    //mi_assert_internal(_mi_heap_default->tld->heap_backing == mi_prim_get_default_heap());
  }
  else {
    // use `_mi_os_alloc` to allocate directly from the OS
    mi_thread_data_t* td = mi_thread_data_zalloc();
    if (td == NULL) return false;

    mi_tld_t*  tld = &td->tld;
    mi_heap_t* heap = &td->heap;
    _mi_tld_init(tld, heap);  // must be before `_mi_heap_init`
    _mi_heap_init(heap, tld, _mi_arena_id_none(), false /* can reclaim */, 0 /* default tag */);
    _mi_heap_set_default_direct(heap);
  }
  return false;
}

// initialize thread local data
void _mi_tld_init(mi_tld_t* tld, mi_heap_t* bheap) {
  _mi_memzero_aligned(tld,sizeof(mi_tld_t));
  tld->heap_backing = bheap;
  tld->heaps = NULL;
  tld->segments.subproc = &mi_subproc_default;
  tld->segments.stats = &tld->stats;
  tld->segments.os = &tld->os;
  tld->os.stats = &tld->stats;
}

// Free the thread local default heap (called from `mi_thread_done`)
static bool _mi_thread_heap_done(mi_heap_t* heap) {
  if (!mi_heap_is_initialized(heap)) return true;

  // reset default heap
  _mi_heap_set_default_direct(_mi_is_main_thread() ? &_mi_heap_main : (mi_heap_t*)&_mi_heap_empty);

  // switch to backing heap
  heap = heap->tld->heap_backing;
  if (!mi_heap_is_initialized(heap)) return false;

  // delete all non-backing heaps in this thread
  mi_heap_t* curr = heap->tld->heaps;
  while (curr != NULL) {
    mi_heap_t* next = curr->next; // save `next` as `curr` will be freed
    if (curr != heap) {
      mi_assert_internal(!mi_heap_is_backing(curr));
      mi_heap_delete(curr);
    }
    curr = next;
  }
  mi_assert_internal(heap->tld->heaps == heap && heap->next == NULL);
  mi_assert_internal(mi_heap_is_backing(heap));

  // collect if not the main thread
  if (heap != &_mi_heap_main) {
    _mi_heap_collect_abandon(heap);
  }

  // merge stats
  _mi_stats_done(&heap->tld->stats);

  // free if not the main thread
  if (heap != &_mi_heap_main) {
    mi_assert_internal(heap->tld->segments.count == 0 || heap->thread_id != _mi_thread_id());
    mi_thread_data_free((mi_thread_data_t*)heap);
  }
  else {
    #if 0
    // never free the main thread even in debug mode; if a dll is linked statically with mimalloc,
    // there may still be delete/free calls after the mi_fls_done is called. Issue #207
    _mi_heap_destroy_pages(heap);
    mi_assert_internal(heap->tld->heap_backing == &_mi_heap_main);
    #endif // 9522
  }
  return false;
}



// --------------------------------------------------------
// Try to run `mi_thread_done()` automatically so any memory
// owned by the thread but not yet released can be abandoned
// and re-owned by another thread.
//
// 1. windows dynamic library:
//     call from DllMain on DLL_THREAD_DETACH
// 2. windows static library:
//     use `FlsAlloc` to call a destructor when the thread is done
// 3. unix, pthreads:
//     use a pthread key to call a destructor when a pthread is done
//
// In the last two cases we also need to call `mi_process_init`
// to set up the thread local keys.
// --------------------------------------------------------

// Set up handlers so `mi_thread_done` is called automatically
static void mi_process_setup_auto_thread_done(void) {
  static bool tls_initialized = false; // fine if it races
  if (tls_initialized) return;
  tls_initialized = true;
  _mi_prim_thread_init_auto_done();
  _mi_heap_set_default_direct(&_mi_heap_main);
}


bool _mi_is_main_thread(void) {
  return (_mi_heap_main.thread_id==0 || _mi_heap_main.thread_id == _mi_thread_id());
}

static _Atomic(size_t) thread_count = MI_ATOMIC_VAR_INIT(1);

size_t  _mi_current_thread_count(void) {
  return mi_atomic_load_relaxed(&thread_count);
}

// This is called from the `mi_malloc_generic`
void mi_thread_init(void) mi_attr_noexcept
{
  // ensure our process has started already
  mi_process_init();

  // initialize the thread local default heap
  // (this will call `_mi_heap_set_default_direct` and thus set the
  //  fiber/pthread key to a non-zero value, ensuring `_mi_thread_done` is called)
  if (_mi_thread_heap_init()) return;  // returns true if already initialized

  _mi_stat_increase(&_mi_stats_main.threads, 1);
  mi_atomic_increment_relaxed(&thread_count);
  //_mi_verbose_message("thread init: 0x%zx\n", _mi_thread_id());
}

void mi_thread_done(void) mi_attr_noexcept {
  _mi_thread_done(NULL);
}

void _mi_thread_done(mi_heap_t* heap)
{
  // calling with NULL implies using the default heap
  if (heap == NULL) {
    heap = mi_prim_get_default_heap();
    if (heap == NULL) return;
  }

  // prevent re-entrancy through heap_done/heap_set_default_direct (issue #699)
  if (!mi_heap_is_initialized(heap)) {
    return;
  }

  // adjust stats
  mi_atomic_decrement_relaxed(&thread_count);
  _mi_stat_decrease(&_mi_stats_main.threads, 1);

  // check thread-id as on Windows shutdown with FLS the main (exit) thread may call this on thread-local heaps...
  if (heap->thread_id != _mi_thread_id()) return;

  // abandon the thread local heap
  if (_mi_thread_heap_done(heap)) return;  // returns true if already ran
}

void _mi_heap_set_default_direct(mi_heap_t* heap)  {
  mi_assert_internal(heap != NULL);
  #if defined(MI_TLS_SLOT)
  mi_prim_tls_slot_set(MI_TLS_SLOT,heap);
  #elif defined(MI_TLS_PTHREAD_SLOT_OFS)
  *mi_prim_tls_pthread_heap_slot() = heap;
  #elif defined(MI_TLS_PTHREAD)
  // we use _mi_heap_default_key
  #else
  _mi_heap_default = heap;
  #endif // 9616

  // ensure the default heap is passed to `_mi_thread_done`
  // setting to a non-NULL value also ensures `mi_thread_done` is called.
  _mi_prim_thread_associate_default_heap(heap);
}


// --------------------------------------------------------
// Run functions on process init/done, and thread init/done
// --------------------------------------------------------
static bool os_preloading = true;    // true until this module is initialized

// Returns true if this module has not been initialized; Don't use C runtime routines until it returns false.
bool mi_decl_noinline _mi_preloading(void) {
  return os_preloading;
}

// Called once by the process loader from `src/prim/prim.c`
void _mi_process_load(void) {
  mi_heap_main_init();
  #if defined(__APPLE__) || defined(MI_TLS_RECURSE_GUARD)
  volatile mi_heap_t* dummy = _mi_heap_default; // access TLS to allocate it before setting tls_initialized to true;
  if (dummy == NULL) return;                    // use dummy or otherwise the access may get optimized away (issue #697)
  #endif // 9645
  os_preloading = false;
  mi_assert_internal(_mi_is_main_thread());
  _mi_options_init();
  mi_process_setup_auto_thread_done();
  mi_process_init();
  if (_mi_is_redirected()) _mi_verbose_message("malloc is redirected.\n");

  // show message from the redirector (if present)
  const char* msg = NULL;
  _mi_allocator_init(&msg);
  if (msg != NULL && (mi_option_is_enabled(mi_option_verbose) || mi_option_is_enabled(mi_option_show_errors))) {
    _mi_fputs(NULL,NULL,NULL,msg);
  }

  // reseed random
  _mi_random_reinit_if_weak(&_mi_heap_main.random);
}

#if defined(_WIN32) && (defined(_M_IX86) || defined(_M_X64))
#include <intrin.h>
mi_decl_cache_align bool _mi_cpu_has_fsrm = false;
mi_decl_cache_align bool _mi_cpu_has_erms = false;

static void mi_detect_cpu_features(void) {
  // FSRM for fast short rep movsb/stosb support (AMD Zen3+ (~2020) or Intel Ice Lake+ (~2017))
  // EMRS for fast enhanced rep movsb/stosb support
  int32_t cpu_info[4];
  __cpuid(cpu_info, 7);
  _mi_cpu_has_fsrm = ((cpu_info[3] & (1 << 4)) != 0); // bit 4 of EDX : see <https://en.wikipedia.org/wiki/CPUID#EAX=7,_ECX=0:_Extended_Features>
  _mi_cpu_has_erms = ((cpu_info[2] & (1 << 9)) != 0); // bit 9 of ECX : see <https://en.wikipedia.org/wiki/CPUID#EAX=7,_ECX=0:_Extended_Features>
}
#else
static void mi_detect_cpu_features(void) {
  // nothing 
}
#endif // 9667

// Initialize the process; called by thread_init or the process loader
void mi_process_init(void) mi_attr_noexcept {
  // ensure we are called once
  static mi_atomic_once_t process_init;
	#if _MSC_VER < 1920
	mi_heap_main_init(); // vs2017 can dynamically re-initialize _mi_heap_main
	#endif // 9690
  if (!mi_atomic_once(&process_init)) return;
  _mi_process_is_initialized = true;
  _mi_verbose_message("process init: 0x%zx\n", _mi_thread_id());
  mi_process_setup_auto_thread_done();

  mi_detect_cpu_features();
  _mi_os_init();
  mi_heap_main_init();
  #if MI_DEBUG
  _mi_verbose_message("debug level : %d\n", MI_DEBUG);
  #endif // 9701
  _mi_verbose_message("secure level: %d\n", MI_SECURE);
  _mi_verbose_message("mem tracking: %s\n", MI_TRACK_TOOL);
  #if MI_TSAN
  _mi_verbose_message("thread santizer enabled\n");
  #endif // 9706
  mi_thread_init();

  #if defined(_WIN32)
  // On windows, when building as a static lib the FLS cleanup happens to early for the main thread.
  // To avoid this, set the FLS value for the main thread to NULL so the fls cleanup
  // will not call _mi_thread_done on the (still executing) main thread. See issue #508.
  _mi_prim_thread_associate_default_heap(NULL);
  #endif // 9711

  mi_stats_reset();  // only call stat reset *after* thread init (or the heap tld == NULL)
  mi_track_init();

  if (mi_option_is_enabled(mi_option_reserve_huge_os_pages)) {
    size_t pages = mi_option_get_clamp(mi_option_reserve_huge_os_pages, 0, 128*1024);
    long reserve_at = mi_option_get(mi_option_reserve_huge_os_pages_at);
    if (reserve_at != -1) {
      mi_reserve_huge_os_pages_at(pages, reserve_at, pages*500);
    } else {
      mi_reserve_huge_os_pages_interleave(pages, 0, pages*500);
    }
  }
  if (mi_option_is_enabled(mi_option_reserve_os_memory)) {
    long ksize = mi_option_get(mi_option_reserve_os_memory);
    if (ksize > 0) {
      mi_reserve_os_memory((size_t)ksize*MI_KiB, true, true);
    }
  }
}

// Called when the process is done (through `at_exit`)
void mi_cdecl _mi_process_done(void) {
  // only shutdown if we were initialized
  if (!_mi_process_is_initialized) return;
  // ensure we are called once
  static bool process_done = false;
  if (process_done) return;
  process_done = true;

  // release any thread specific resources and ensure _mi_thread_done is called on all but the main thread
  _mi_prim_thread_done_auto_done();

  #ifndef MI_SKIP_COLLECT_ON_EXIT
    #if (MI_DEBUG || !defined(MI_SHARED_LIB))
    // free all memory if possible on process exit. This is not needed for a stand-alone process
    // but should be done if mimalloc is statically linked into another shared library which
    // is repeatedly loaded/unloaded, see issue #281.
    mi_collect(true /* force */ );
    #endif // 9751
  #endif // 9750

  // Forcefully release all retained memory; this can be dangerous in general if overriding regular malloc/free
  // since after process_done there might still be other code running that calls `free` (like at_exit routines,
  // or C-runtime termination code.
  if (mi_option_is_enabled(mi_option_destroy_on_exit)) {
    mi_collect(true /* force */);
    _mi_heap_unsafe_destroy_all();     // forcefully release all memory held by all heaps (of this thread only!)
    _mi_arena_unsafe_destroy_all(& _mi_heap_main_get()->tld->stats);
  }

  if (mi_option_is_enabled(mi_option_show_stats) || mi_option_is_enabled(mi_option_verbose)) {
    mi_stats_print(NULL);
  }
  _mi_allocator_done();
  _mi_verbose_message("process done: 0x%zx\n", _mi_heap_main.thread_id);
  os_preloading = true; // don't call the C runtime anymore
}

// --- Amalgamation END --- mimalloc/src/init.c


// --- Amalgamation START --- mimalloc/src/libc.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// --------------------------------------------------------
// This module defines various std libc functions to reduce
// the dependency on libc, and also prevent errors caused 
// by some libc implementations when called before `main`
// executes (due to malloc redirection)
// --------------------------------------------------------


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


char _mi_toupper(char c) {
  if (c >= 'a' && c <= 'z') return (c - 'a' + 'A');
                       else return c;
}

int _mi_strnicmp(const char* s, const char* t, size_t n) {
  if (n == 0) return 0;
  for (; *s != 0 && *t != 0 && n > 0; s++, t++, n--) {
    if (_mi_toupper(*s) != _mi_toupper(*t)) break;
  }
  return (n == 0 ? 0 : *s - *t);
}

void _mi_strlcpy(char* dest, const char* src, size_t dest_size) {
  if (dest==NULL || src==NULL || dest_size == 0) return;
  // copy until end of src, or when dest is (almost) full
  while (*src != 0 && dest_size > 1) {
    *dest++ = *src++;
    dest_size--;
  }
  // always zero terminate
  *dest = 0;
}

void _mi_strlcat(char* dest, const char* src, size_t dest_size) {
  if (dest==NULL || src==NULL || dest_size == 0) return;
  // find end of string in the dest buffer
  while (*dest != 0 && dest_size > 1) {
    dest++;
    dest_size--;
  }
  // and catenate
  _mi_strlcpy(dest, src, dest_size);
}

size_t _mi_strlen(const char* s) {
  if (s==NULL) return 0;
  size_t len = 0;
  while(s[len] != 0) { len++; }
  return len;
}

size_t _mi_strnlen(const char* s, size_t max_len) {
  if (s==NULL) return 0;
  size_t len = 0;
  while(s[len] != 0 && len < max_len) { len++; }
  return len;
}

#ifdef MI_NO_GETENV
bool _mi_getenv(const char* name, char* result, size_t result_size) {
  MI_UNUSED(name);
  MI_UNUSED(result);
  MI_UNUSED(result_size);
  return false;
}
#else
bool _mi_getenv(const char* name, char* result, size_t result_size) {
  if (name==NULL || result == NULL || result_size < 64) return false;
  return _mi_prim_getenv(name,result,result_size);
}
#endif // 9856

// --------------------------------------------------------
// Define our own limited `_mi_vsnprintf` and `_mi_snprintf`
// This is mostly to avoid calling these when libc is not yet
// initialized (and to reduce dependencies)
// 
// format:      d i, p x u, s
// prec:        z l ll L
// width:       10
// align-left:  -
// fill:        0
// plus:        +
// --------------------------------------------------------

static void mi_outc(char c, char** out, char* end) {
  char* p = *out;
  if (p >= end) return;
  *p = c;
  *out = p + 1;
}

static void mi_outs(const char* s, char** out, char* end) {
  if (s == NULL) return;
  char* p = *out;
  while (*s != 0 && p < end) {
    *p++ = *s++;
  }
  *out = p;
}

static void mi_out_fill(char fill, size_t len, char** out, char* end) {
  char* p = *out;
  for (size_t i = 0; i < len && p < end; i++) {
    *p++ = fill;
  }
  *out = p;
}

static void mi_out_alignright(char fill, char* start, size_t len, size_t extra, char* end) {
  if (len == 0 || extra == 0) return;
  if (start + len + extra >= end) return;
  // move `len` characters to the right (in reverse since it can overlap)
  for (size_t i = 1; i <= len; i++) {
    start[len + extra - i] = start[len - i];
  }
  // and fill the start
  for (size_t i = 0; i < extra; i++) {
    start[i] = fill;
  }
}


static void mi_out_num(uintmax_t x, size_t base, char prefix, char** out, char* end) 
{
  if (x == 0 || base == 0 || base > 16) {
    if (prefix != 0) { mi_outc(prefix, out, end); }
    mi_outc('0',out,end);
  }
  else {
    // output digits in reverse
    char* start = *out;
    while (x > 0) {
      char digit = (char)(x % base);
      mi_outc((digit <= 9 ? '0' + digit : 'A' + digit - 10),out,end);
      x = x / base;
    }
    if (prefix != 0) { 
      mi_outc(prefix, out, end); 
    }
    size_t len = *out - start;
    // and reverse in-place
    for (size_t i = 0; i < (len / 2); i++) {
      char c = start[len - i - 1];
      start[len - i - 1] = start[i];
      start[i] = c;
    }
  }
}


#define MI_NEXTC()  c = *in; if (c==0) break; in++;

void _mi_vsnprintf(char* buf, size_t bufsize, const char* fmt, va_list args) {
  if (buf == NULL || bufsize == 0 || fmt == NULL) return;
  buf[bufsize - 1] = 0;
  char* const end = buf + (bufsize - 1);
  const char* in = fmt;
  char* out = buf;
  while (true) {
    if (out >= end) break;
    char c;
    MI_NEXTC();
    if (c != '%') {
      if ((c >= ' ' && c <= '~') || c=='\n' || c=='\r' || c=='\t') { // output visible ascii or standard control only
        mi_outc(c, &out, end);
      }
    }
    else {
      MI_NEXTC();
      char   fill = ' ';
      size_t width = 0;
      char   numtype = 'd';
      char   numplus = 0;
      bool   alignright = true; 
      if (c == '+' || c == ' ') { numplus = c; MI_NEXTC(); }
      if (c == '-') { alignright = false; MI_NEXTC(); }
      if (c == '0') { fill = '0'; MI_NEXTC(); }
      if (c >= '1' && c <= '9') {
        width = (c - '0'); MI_NEXTC();
        while (c >= '0' && c <= '9') {
          width = (10 * width) + (c - '0'); MI_NEXTC();
        }
        if (c == 0) break;  // extra check due to while
      }      
      if (c == 'z' || c == 't' || c == 'L') { numtype = c; MI_NEXTC(); }
      else if (c == 'l') {
        numtype = c; MI_NEXTC();
        if (c == 'l') { numtype = 'L'; MI_NEXTC(); }
      }

      char* start = out;
      if (c == 's') {
        // string
        const char* s = va_arg(args, const char*);
        mi_outs(s, &out, end);
      }
      else if (c == 'p' || c == 'x' || c == 'u') {
        // unsigned
        uintmax_t x = 0;
        if (c == 'x' || c == 'u') {
          if (numtype == 'z')       x = va_arg(args, size_t);
          else if (numtype == 't')  x = va_arg(args, uintptr_t); // unsigned ptrdiff_t
          else if (numtype == 'L')  x = va_arg(args, unsigned long long);
          else if (numtype == 'l')  x = va_arg(args, unsigned long);
                               else x = va_arg(args, unsigned int);
        }
        else if (c == 'p') {
          x = va_arg(args, uintptr_t);
          mi_outs("0x", &out, end);
          start = out;
          width = (width >= 2 ? width - 2 : 0);
        }
        if (width == 0 && (c == 'x' || c == 'p')) {
          if (c == 'p')   { width = 2 * (x <= UINT32_MAX ? 4 : ((x >> 16) <= UINT32_MAX ? 6 : sizeof(void*))); }
          if (width == 0) { width = 2; }
          fill = '0';
        }
        mi_out_num(x, (c == 'x' || c == 'p' ? 16 : 10), numplus, &out, end);
      }
      else if (c == 'i' || c == 'd') {
        // signed
        intmax_t x = 0;
        if (numtype == 'z')       x = va_arg(args, intptr_t );
        else if (numtype == 't')  x = va_arg(args, ptrdiff_t);
        else if (numtype == 'L')  x = va_arg(args, long long);
        else if (numtype == 'l')  x = va_arg(args, long);
                             else x = va_arg(args, int);
        char pre = 0;
        if (x < 0) {
          pre = '-';
          if (x > INTMAX_MIN) { x = -x; }
        }
        else if (numplus != 0) {
          pre = numplus;
        }
        mi_out_num((uintmax_t)x, 10, pre, &out, end);
      }
      else if (c >= ' ' && c <= '~') {
        // unknown format
        mi_outc('%', &out, end);
        mi_outc(c, &out, end);
      }

      // fill & align
      mi_assert_internal(out <= end);
      mi_assert_internal(out >= start);
      const size_t len = out - start;
      if (len < width) {
        mi_out_fill(fill, width - len, &out, end);
        if (alignright && out <= end) {
          mi_out_alignright(fill, start, len, width - len, end);
        }
      }
    }
  }
  mi_assert_internal(out <= end);
  *out = 0;
}

void _mi_snprintf(char* buf, size_t buflen, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  _mi_vsnprintf(buf, buflen, fmt, args);
  va_end(args);
}
// --- Amalgamation END --- mimalloc/src/libc.c


// --- Amalgamation START --- mimalloc/src/options.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2021, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


#include <stdio.h>      // stdin/stdout
#include <stdlib.h>     // abort



static long mi_max_error_count   = 16; // stop outputting errors after this (use < 0 for no limit)
static long mi_max_warning_count = 16; // stop outputting warnings after this (use < 0 for no limit)

static void mi_add_stderr_output(void);

int mi_version(void) mi_attr_noexcept {
  return MI_MALLOC_VERSION;
}


// --------------------------------------------------------
// Options
// These can be accessed by multiple threads and may be
// concurrently initialized, but an initializing data race
// is ok since they resolve to the same value.
// --------------------------------------------------------
typedef enum mi_init_e {
  UNINIT,       // not yet initialized
  DEFAULTED,    // not found in the environment, use default value
  INITIALIZED   // found in environment or set explicitly
} mi_init_t;

typedef struct mi_option_desc_s {
  long        value;  // the value
  mi_init_t   init;   // is it initialized yet? (from the environment)
  mi_option_t option; // for debugging: the option index should match the option
  const char* name;   // option name without `mimalloc_` prefix
  const char* legacy_name; // potential legacy option name
} mi_option_desc_t;

#define MI_OPTION(opt)                  mi_option_##opt, #opt, NULL
#define MI_OPTION_LEGACY(opt,legacy)    mi_option_##opt, #opt, #legacy

// Some options can be set at build time for statically linked libraries
// (use `-DMI_EXTRA_CPPDEFS="opt1=val1;opt2=val2"`)
//
// This is useful if we cannot pass them as environment variables
// (and setting them programmatically would be too late)

#ifndef MI_DEFAULT_VERBOSE
#define MI_DEFAULT_VERBOSE 0
#endif // 10135

#ifndef MI_DEFAULT_EAGER_COMMIT
#define MI_DEFAULT_EAGER_COMMIT 1
#endif // 10139

#ifndef MI_DEFAULT_ARENA_EAGER_COMMIT
#define MI_DEFAULT_ARENA_EAGER_COMMIT 2
#endif // 10143

// in KiB
#ifndef MI_DEFAULT_ARENA_RESERVE
 #if (MI_INTPTR_SIZE>4)
  #define MI_DEFAULT_ARENA_RESERVE 1024L*1024L
 #else
  #define MI_DEFAULT_ARENA_RESERVE 128L*1024L
 #endif // 10149
#endif // 10148

#ifndef MI_DEFAULT_DISALLOW_ARENA_ALLOC
#define MI_DEFAULT_DISALLOW_ARENA_ALLOC 0
#endif // 10156

#ifndef MI_DEFAULT_ALLOW_LARGE_OS_PAGES
#define MI_DEFAULT_ALLOW_LARGE_OS_PAGES 0
#endif // 10160

#ifndef MI_DEFAULT_RESERVE_HUGE_OS_PAGES
#define MI_DEFAULT_RESERVE_HUGE_OS_PAGES 0
#endif // 10164

#ifndef MI_DEFAULT_RESERVE_OS_MEMORY
#define MI_DEFAULT_RESERVE_OS_MEMORY 0
#endif // 10168

#ifndef MI_DEFAULT_GUARDED_SAMPLE_RATE
#if MI_GUARDED
#define MI_DEFAULT_GUARDED_SAMPLE_RATE 4000
#else
#define MI_DEFAULT_GUARDED_SAMPLE_RATE 0
#endif // 10173
#endif // 10172


static mi_option_desc_t options[_mi_option_last] =
{
  // stable options
#if MI_DEBUG || defined(MI_SHOW_ERRORS)
  { 1, UNINIT, MI_OPTION(show_errors) },
#else
  { 0, UNINIT, MI_OPTION(show_errors) },
#endif // 10184
  { 0, UNINIT, MI_OPTION(show_stats) },
  { MI_DEFAULT_VERBOSE, UNINIT, MI_OPTION(verbose) },

  // some of the following options are experimental and not all combinations are allowed.
  { MI_DEFAULT_EAGER_COMMIT,
       UNINIT, MI_OPTION(eager_commit) },               // commit per segment directly (4MiB)  (but see also `eager_commit_delay`)
  { MI_DEFAULT_ARENA_EAGER_COMMIT,
       UNINIT, MI_OPTION_LEGACY(arena_eager_commit,eager_region_commit) }, // eager commit arena's? 2 is used to enable this only on an OS that has overcommit (i.e. linux)
  { 1, UNINIT, MI_OPTION_LEGACY(purge_decommits,reset_decommits) },        // purge decommits memory (instead of reset) (note: on linux this uses MADV_DONTNEED for decommit)
  { MI_DEFAULT_ALLOW_LARGE_OS_PAGES,
       UNINIT, MI_OPTION_LEGACY(allow_large_os_pages,large_os_pages) },    // use large OS pages, use only with eager commit to prevent fragmentation of VMA's
  { MI_DEFAULT_RESERVE_HUGE_OS_PAGES,
       UNINIT, MI_OPTION(reserve_huge_os_pages) },      // per 1GiB huge pages
  {-1, UNINIT, MI_OPTION(reserve_huge_os_pages_at) },   // reserve huge pages at node N
  { MI_DEFAULT_RESERVE_OS_MEMORY,
       UNINIT, MI_OPTION(reserve_os_memory)     },      // reserve N KiB OS memory in advance (use `option_get_size`)
  { 0, UNINIT, MI_OPTION(deprecated_segment_cache) },   // cache N segments per thread
  { 0, UNINIT, MI_OPTION(deprecated_page_reset) },      // reset page memory on free
  { 0, UNINIT, MI_OPTION(abandoned_page_purge) },       // purge free page memory when a thread terminates
  { 0, UNINIT, MI_OPTION(deprecated_segment_reset) },   // reset segment memory on free (needs eager commit)
#if defined(__NetBSD__)
  { 0, UNINIT, MI_OPTION(eager_commit_delay) },         // the first N segments per thread are not eagerly committed
#else
  { 1, UNINIT, MI_OPTION(eager_commit_delay) },         // the first N segments per thread are not eagerly committed (but per page in the segment on demand)
#endif // 10209
  { 10,  UNINIT, MI_OPTION_LEGACY(purge_delay,reset_delay) },  // purge delay in milli-seconds
  { 0,   UNINIT, MI_OPTION(use_numa_nodes) },           // 0 = use available numa nodes, otherwise use at most N nodes.
  { 0,   UNINIT, MI_OPTION_LEGACY(disallow_os_alloc,limit_os_alloc) },           // 1 = do not use OS memory for allocation (but only reserved arenas)
  { 100, UNINIT, MI_OPTION(os_tag) },                   // only apple specific for now but might serve more or less related purpose
  { 32,  UNINIT, MI_OPTION(max_errors) },               // maximum errors that are output
  { 32,  UNINIT, MI_OPTION(max_warnings) },             // maximum warnings that are output
  { 10,  UNINIT, MI_OPTION(max_segment_reclaim)},       // max. percentage of the abandoned segments to be reclaimed per try.
  { 0,   UNINIT, MI_OPTION(destroy_on_exit)},           // release all OS memory on process exit; careful with dangling pointer or after-exit frees!
  { MI_DEFAULT_ARENA_RESERVE, UNINIT, MI_OPTION(arena_reserve) }, // reserve memory N KiB at a time (=1GiB) (use `option_get_size`)
  { 10,  UNINIT, MI_OPTION(arena_purge_mult) },         // purge delay multiplier for arena's
  { 1,   UNINIT, MI_OPTION_LEGACY(purge_extend_delay, decommit_extend_delay) },
  { 1,   UNINIT, MI_OPTION(abandoned_reclaim_on_free) },// reclaim an abandoned segment on a free
  { MI_DEFAULT_DISALLOW_ARENA_ALLOC,   UNINIT, MI_OPTION(disallow_arena_alloc) }, // 1 = do not use arena's for allocation (except if using specific arena id's)
  { 400, UNINIT, MI_OPTION(retry_on_oom) },             // windows only: retry on out-of-memory for N milli seconds (=400), set to 0 to disable retries.
#if defined(MI_VISIT_ABANDONED)
  { 1,   INITIALIZED, MI_OPTION(visit_abandoned) },     // allow visiting heap blocks in abandonded segments; requires taking locks during reclaim.
#else
  { 0,   UNINIT, MI_OPTION(visit_abandoned) },
#endif // 10228
  { 0,   UNINIT, MI_OPTION(guarded_min) },              // only used when building with MI_GUARDED: minimal rounded object size for guarded objects
  { MI_GiB, UNINIT, MI_OPTION(guarded_max) },           // only used when building with MI_GUARDED: maximal rounded object size for guarded objects
  { 0,   UNINIT, MI_OPTION(guarded_precise) },          // disregard minimal alignment requirement to always place guarded blocks exactly in front of a guard page (=0)
  { MI_DEFAULT_GUARDED_SAMPLE_RATE,
         UNINIT, MI_OPTION(guarded_sample_rate)},       // 1 out of N allocations in the min/max range will be guarded (=4000)
  { 0,   UNINIT, MI_OPTION(guarded_sample_seed)},
  { 0,   UNINIT, MI_OPTION(target_segments_per_thread) }, // abandon segments beyond this point, or 0 to disable.
};

static void mi_option_init(mi_option_desc_t* desc);

static bool mi_option_has_size_in_kib(mi_option_t option) {
  return (option == mi_option_reserve_os_memory || option == mi_option_arena_reserve);
}

void _mi_options_init(void) {
  // called on process load
  mi_add_stderr_output(); // now it safe to use stderr for output
  for(int i = 0; i < _mi_option_last; i++ ) {
    mi_option_t option = (mi_option_t)i;
    long l = mi_option_get(option); MI_UNUSED(l); // initialize
    // if (option != mi_option_verbose)
    {
      mi_option_desc_t* desc = &options[option];
      _mi_verbose_message("option '%s': %ld %s\n", desc->name, desc->value, (mi_option_has_size_in_kib(option) ? "KiB" : ""));
    }
  }
  mi_max_error_count = mi_option_get(mi_option_max_errors);
  mi_max_warning_count = mi_option_get(mi_option_max_warnings);
  #if MI_GUARDED
  if (mi_option_get(mi_option_guarded_sample_rate) > 0) {
    if (mi_option_is_enabled(mi_option_allow_large_os_pages)) {
      mi_option_disable(mi_option_allow_large_os_pages);
      _mi_warning_message("option 'allow_large_os_pages' is disabled to allow for guarded objects\n");
    }
  }
  _mi_verbose_message("guarded build: %s\n", mi_option_get(mi_option_guarded_sample_rate) != 0 ? "enabled" : "disabled");
  #endif // 10262
}

long _mi_option_get_fast(mi_option_t option) {
  mi_assert(option >= 0 && option < _mi_option_last);
  mi_option_desc_t* desc = &options[option];
  mi_assert(desc->option == option);  // index should match the option
  //mi_assert(desc->init != UNINIT);
  return desc->value;
}


mi_decl_nodiscard long mi_option_get(mi_option_t option) {
  mi_assert(option >= 0 && option < _mi_option_last);
  if (option < 0 || option >= _mi_option_last) return 0;
  mi_option_desc_t* desc = &options[option];
  mi_assert(desc->option == option);  // index should match the option
  if mi_unlikely(desc->init == UNINIT) {
    mi_option_init(desc);
  }
  return desc->value;
}

mi_decl_nodiscard long mi_option_get_clamp(mi_option_t option, long min, long max) {
  long x = mi_option_get(option);
  return (x < min ? min : (x > max ? max : x));
}

mi_decl_nodiscard size_t mi_option_get_size(mi_option_t option) {
  const long x = mi_option_get(option);
  size_t size = (x < 0 ? 0 : (size_t)x);
  if (mi_option_has_size_in_kib(option)) {
    size *= MI_KiB;
  }
  return size;
}

void mi_option_set(mi_option_t option, long value) {
  mi_assert(option >= 0 && option < _mi_option_last);
  if (option < 0 || option >= _mi_option_last) return;
  mi_option_desc_t* desc = &options[option];
  mi_assert(desc->option == option);  // index should match the option
  desc->value = value;
  desc->init = INITIALIZED;
  // ensure min/max range; be careful to not recurse.
  if (desc->option == mi_option_guarded_min && _mi_option_get_fast(mi_option_guarded_max) < value) {
    mi_option_set(mi_option_guarded_max, value);
  }
  else if (desc->option == mi_option_guarded_max && _mi_option_get_fast(mi_option_guarded_min) > value) {
    mi_option_set(mi_option_guarded_min, value);
  }
}

void mi_option_set_default(mi_option_t option, long value) {
  mi_assert(option >= 0 && option < _mi_option_last);
  if (option < 0 || option >= _mi_option_last) return;
  mi_option_desc_t* desc = &options[option];
  if (desc->init != INITIALIZED) {
    desc->value = value;
  }
}

mi_decl_nodiscard bool mi_option_is_enabled(mi_option_t option) {
  return (mi_option_get(option) != 0);
}

void mi_option_set_enabled(mi_option_t option, bool enable) {
  mi_option_set(option, (enable ? 1 : 0));
}

void mi_option_set_enabled_default(mi_option_t option, bool enable) {
  mi_option_set_default(option, (enable ? 1 : 0));
}

void mi_option_enable(mi_option_t option) {
  mi_option_set_enabled(option,true);
}

void mi_option_disable(mi_option_t option) {
  mi_option_set_enabled(option,false);
}

static void mi_cdecl mi_out_stderr(const char* msg, void* arg) {
  MI_UNUSED(arg);
  if (msg != NULL && msg[0] != 0) {
    _mi_prim_out_stderr(msg);
  }
}

// Since an output function can be registered earliest in the `main`
// function we also buffer output that happens earlier. When
// an output function is registered it is called immediately with
// the output up to that point.
#ifndef MI_MAX_DELAY_OUTPUT
#define MI_MAX_DELAY_OUTPUT ((size_t)(16*1024))
#endif // 10363
static char out_buf[MI_MAX_DELAY_OUTPUT+1];
static _Atomic(size_t) out_len;

static void mi_cdecl mi_out_buf(const char* msg, void* arg) {
  MI_UNUSED(arg);
  if (msg==NULL) return;
  if (mi_atomic_load_relaxed(&out_len)>=MI_MAX_DELAY_OUTPUT) return;
  size_t n = _mi_strlen(msg);
  if (n==0) return;
  // claim space
  size_t start = mi_atomic_add_acq_rel(&out_len, n);
  if (start >= MI_MAX_DELAY_OUTPUT) return;
  // check bound
  if (start+n >= MI_MAX_DELAY_OUTPUT) {
    n = MI_MAX_DELAY_OUTPUT-start-1;
  }
  _mi_memcpy(&out_buf[start], msg, n);
}

static void mi_out_buf_flush(mi_output_fun* out, bool no_more_buf, void* arg) {
  if (out==NULL) return;
  // claim (if `no_more_buf == true`, no more output will be added after this point)
  size_t count = mi_atomic_add_acq_rel(&out_len, (no_more_buf ? MI_MAX_DELAY_OUTPUT : 1));
  // and output the current contents
  if (count>MI_MAX_DELAY_OUTPUT) count = MI_MAX_DELAY_OUTPUT;
  out_buf[count] = 0;
  out(out_buf,arg);
  if (!no_more_buf) {
    out_buf[count] = '\n'; // if continue with the buffer, insert a newline
  }
}


// Once this module is loaded, switch to this routine
// which outputs to stderr and the delayed output buffer.
static void mi_cdecl mi_out_buf_stderr(const char* msg, void* arg) {
  mi_out_stderr(msg,arg);
  mi_out_buf(msg,arg);
}



// --------------------------------------------------------
// Default output handler
// --------------------------------------------------------

// Should be atomic but gives errors on many platforms as generally we cannot cast a function pointer to a uintptr_t.
// For now, don't register output from multiple threads.
static mi_output_fun* volatile mi_out_default; // = NULL
static _Atomic(void*) mi_out_arg; // = NULL

static mi_output_fun* mi_out_get_default(void** parg) {
  if (parg != NULL) { *parg = mi_atomic_load_ptr_acquire(void,&mi_out_arg); }
  mi_output_fun* out = mi_out_default;
  return (out == NULL ? &mi_out_buf : out);
}

void mi_register_output(mi_output_fun* out, void* arg) mi_attr_noexcept {
  mi_out_default = (out == NULL ? &mi_out_stderr : out); // stop using the delayed output buffer
  mi_atomic_store_ptr_release(void,&mi_out_arg, arg);
  if (out!=NULL) mi_out_buf_flush(out,true,arg);         // output all the delayed output now
}

// add stderr to the delayed output after the module is loaded
static void mi_add_stderr_output(void) {
  mi_assert_internal(mi_out_default == NULL);
  mi_out_buf_flush(&mi_out_stderr, false, NULL); // flush current contents to stderr
  mi_out_default = &mi_out_buf_stderr;           // and add stderr to the delayed output
}

// --------------------------------------------------------
// Messages, all end up calling `_mi_fputs`.
// --------------------------------------------------------
static _Atomic(size_t) error_count;   // = 0;  // when >= max_error_count stop emitting errors
static _Atomic(size_t) warning_count; // = 0;  // when >= max_warning_count stop emitting warnings

// When overriding malloc, we may recurse into mi_vfprintf if an allocation
// inside the C runtime causes another message.
// In some cases (like on macOS) the loader already allocates which
// calls into mimalloc; if we then access thread locals (like `recurse`)
// this may crash as the access may call _tlv_bootstrap that tries to
// (recursively) invoke malloc again to allocate space for the thread local
// variables on demand. This is why we use a _mi_preloading test on such
// platforms. However, C code generator may move the initial thread local address
// load before the `if` and we therefore split it out in a separate funcion.
static mi_decl_thread bool recurse = false;

static mi_decl_noinline bool mi_recurse_enter_prim(void) {
  if (recurse) return false;
  recurse = true;
  return true;
}

static mi_decl_noinline void mi_recurse_exit_prim(void) {
  recurse = false;
}

static bool mi_recurse_enter(void) {
  #if defined(__APPLE__) || defined(MI_TLS_RECURSE_GUARD)
  if (_mi_preloading()) return false;
  #endif // 10464
  return mi_recurse_enter_prim();
}

static void mi_recurse_exit(void) {
  #if defined(__APPLE__) || defined(MI_TLS_RECURSE_GUARD)
  if (_mi_preloading()) return;
  #endif // 10471
  mi_recurse_exit_prim();
}

void _mi_fputs(mi_output_fun* out, void* arg, const char* prefix, const char* message) {
  if (out==NULL || (void*)out==(void*)stdout || (void*)out==(void*)stderr) { // TODO: use mi_out_stderr for stderr?
    if (!mi_recurse_enter()) return;
    out = mi_out_get_default(&arg);
    if (prefix != NULL) out(prefix, arg);
    out(message, arg);
    mi_recurse_exit();
  }
  else {
    if (prefix != NULL) out(prefix, arg);
    out(message, arg);
  }
}

// Define our own limited `fprintf` that avoids memory allocation.
// We do this using `_mi_vsnprintf` with a limited buffer.
static void mi_vfprintf( mi_output_fun* out, void* arg, const char* prefix, const char* fmt, va_list args ) {
  char buf[512];
  if (fmt==NULL) return;
  if (!mi_recurse_enter()) return;
  _mi_vsnprintf(buf, sizeof(buf)-1, fmt, args);
  mi_recurse_exit();
  _mi_fputs(out,arg,prefix,buf);
}

void _mi_fprintf( mi_output_fun* out, void* arg, const char* fmt, ... ) {
  va_list args;
  va_start(args,fmt);
  mi_vfprintf(out,arg,NULL,fmt,args);
  va_end(args);
}

static void mi_vfprintf_thread(mi_output_fun* out, void* arg, const char* prefix, const char* fmt, va_list args) {
  if (prefix != NULL && _mi_strnlen(prefix,33) <= 32 && !_mi_is_main_thread()) {
    char tprefix[64];
    _mi_snprintf(tprefix, sizeof(tprefix), "%sthread 0x%tx: ", prefix, (uintptr_t)_mi_thread_id());
    mi_vfprintf(out, arg, tprefix, fmt, args);
  }
  else {
    mi_vfprintf(out, arg, prefix, fmt, args);
  }
}

void _mi_trace_message(const char* fmt, ...) {
  if (mi_option_get(mi_option_verbose) <= 1) return;  // only with verbose level 2 or higher
  va_list args;
  va_start(args, fmt);
  mi_vfprintf_thread(NULL, NULL, "mimalloc: ", fmt, args);
  va_end(args);
}

void _mi_verbose_message(const char* fmt, ...) {
  if (!mi_option_is_enabled(mi_option_verbose)) return;
  va_list args;
  va_start(args,fmt);
  mi_vfprintf(NULL, NULL, "mimalloc: ", fmt, args);
  va_end(args);
}

static void mi_show_error_message(const char* fmt, va_list args) {
  if (!mi_option_is_enabled(mi_option_verbose)) {
    if (!mi_option_is_enabled(mi_option_show_errors)) return;
    if (mi_max_error_count >= 0 && (long)mi_atomic_increment_acq_rel(&error_count) > mi_max_error_count) return;
  }
  mi_vfprintf_thread(NULL, NULL, "mimalloc: error: ", fmt, args);
}

void _mi_warning_message(const char* fmt, ...) {
  if (!mi_option_is_enabled(mi_option_verbose)) {
    if (!mi_option_is_enabled(mi_option_show_errors)) return;
    if (mi_max_warning_count >= 0 && (long)mi_atomic_increment_acq_rel(&warning_count) > mi_max_warning_count) return;
  }
  va_list args;
  va_start(args,fmt);
  mi_vfprintf_thread(NULL, NULL, "mimalloc: warning: ", fmt, args);
  va_end(args);
}


#if MI_DEBUG
void _mi_assert_fail(const char* assertion, const char* fname, unsigned line, const char* func ) {
  _mi_fprintf(NULL, NULL, "mimalloc: assertion failed: at \"%s\":%u, %s\n  assertion: \"%s\"\n", fname, line, (func==NULL?"":func), assertion);
  abort();
}
#endif // 10556

// --------------------------------------------------------
// Errors
// --------------------------------------------------------

static mi_error_fun* volatile  mi_error_handler; // = NULL
static _Atomic(void*) mi_error_arg;     // = NULL

static void mi_error_default(int err) {
  MI_UNUSED(err);
#if (MI_DEBUG>0)
  if (err==EFAULT) {
#if 0 //     #ifdef _MSC_VER
//     __debugbreak();
    #endif // 10574
    abort();
  }
#endif // 10572
#if (MI_SECURE>0)
  if (err==EFAULT) {  // abort on serious errors in secure mode (corrupted meta-data)
    abort();
  }
#endif // 10580
#if defined(MI_XMALLOC)
  if (err==ENOMEM || err==EOVERFLOW) { // abort on memory allocation fails in xmalloc mode
    abort();
  }
#endif // 10585
}

void mi_register_error(mi_error_fun* fun, void* arg) {
  mi_error_handler = fun;  // can be NULL
  mi_atomic_store_ptr_release(void,&mi_error_arg, arg);
}

void _mi_error_message(int err, const char* fmt, ...) {
  // show detailed error message
  va_list args;
  va_start(args, fmt);
  mi_show_error_message(fmt, args);
  va_end(args);
  // and call the error handler which may abort (or return normally)
  if (mi_error_handler != NULL) {
    mi_error_handler(err, mi_atomic_load_ptr_acquire(void,&mi_error_arg));
  }
  else {
    mi_error_default(err);
  }
}

// --------------------------------------------------------
// Initialize options by checking the environment
// --------------------------------------------------------

// TODO: implement ourselves to reduce dependencies on the C runtime
#include <stdlib.h> // strtol
#include <string.h> // strstr


static void mi_option_init(mi_option_desc_t* desc) {
  // Read option value from the environment
  char s[64 + 1];
  char buf[64+1];
  _mi_strlcpy(buf, "mimalloc_", sizeof(buf));
  _mi_strlcat(buf, desc->name, sizeof(buf));
  bool found = _mi_getenv(buf, s, sizeof(s));
  if (!found && desc->legacy_name != NULL) {
    _mi_strlcpy(buf, "mimalloc_", sizeof(buf));
    _mi_strlcat(buf, desc->legacy_name, sizeof(buf));
    found = _mi_getenv(buf, s, sizeof(s));
    if (found) {
      _mi_warning_message("environment option \"mimalloc_%s\" is deprecated -- use \"mimalloc_%s\" instead.\n", desc->legacy_name, desc->name);
    }
  }

  if (found) {
    size_t len = _mi_strnlen(s, sizeof(buf) - 1);
    for (size_t i = 0; i < len; i++) {
      buf[i] = _mi_toupper(s[i]);
    }
    buf[len] = 0;
    if (buf[0] == 0 || strstr("1;TRUE;YES;ON", buf) != NULL) {
      desc->value = 1;
      desc->init = INITIALIZED;
    }
    else if (strstr("0;FALSE;NO;OFF", buf) != NULL) {
      desc->value = 0;
      desc->init = INITIALIZED;
    }
    else {
      char* end = buf;
      long value = strtol(buf, &end, 10);
      if (mi_option_has_size_in_kib(desc->option)) {
        // this option is interpreted in KiB to prevent overflow of `long` for large allocations
        // (long is 32-bit on 64-bit windows, which allows for 4TiB max.)
        size_t size = (value < 0 ? 0 : (size_t)value);
        bool overflow = false;
        if (*end == 'K') { end++; }
        else if (*end == 'M') { overflow = mi_mul_overflow(size,MI_KiB,&size); end++; }
        else if (*end == 'G') { overflow = mi_mul_overflow(size,MI_MiB,&size); end++; }
        else if (*end == 'T') { overflow = mi_mul_overflow(size,MI_GiB,&size); end++; }
        else { size = (size + MI_KiB - 1) / MI_KiB; }
        if (end[0] == 'I' && end[1] == 'B') { end += 2; } // KiB, MiB, GiB, TiB
        else if (*end == 'B') { end++; }                  // Kb, Mb, Gb, Tb
        if (overflow || size > MI_MAX_ALLOC_SIZE) { size = (MI_MAX_ALLOC_SIZE / MI_KiB); }
        value = (size > LONG_MAX ? LONG_MAX : (long)size);
      }
      if (*end == 0) {
        mi_option_set(desc->option, value);
      }
      else {
        // set `init` first to avoid recursion through _mi_warning_message on mimalloc_verbose.
        desc->init = DEFAULTED;
        if (desc->option == mi_option_verbose && desc->value == 0) {
          // if the 'mimalloc_verbose' env var has a bogus value we'd never know
          // (since the value defaults to 'off') so in that case briefly enable verbose
          desc->value = 1;
          _mi_warning_message("environment option mimalloc_%s has an invalid value.\n", desc->name);
          desc->value = 0;
        }
        else {
          _mi_warning_message("environment option mimalloc_%s has an invalid value.\n", desc->name);
        }
      }
    }
    mi_assert_internal(desc->init != UNINIT);
  }
  else if (!_mi_preloading()) {
    desc->init = DEFAULTED;
  }
}
// --- Amalgamation END --- mimalloc/src/options.c


// --- Amalgamation START --- mimalloc/src/os.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h



/* -----------------------------------------------------------
  Initialization.
----------------------------------------------------------- */
#ifndef MI_DEFAULT_VIRTUAL_ADDRESS_BITS
#if MI_INTPTR_SIZE < 8
#define MI_DEFAULT_VIRTUAL_ADDRESS_BITS   32
#else
#define MI_DEFAULT_VIRTUAL_ADDRESS_BITS   48
#endif // 10725
#endif // 10724

#ifndef MI_DEFAULT_PHYSICAL_MEMORY
#if MI_INTPTR_SIZE < 8
#define MI_DEFAULT_PHYSICAL_MEMORY    4*MI_GiB
#else
#define MI_DEFAULT_PHYSICAL_MEMORY    32*MI_GiB
#endif // 10733
#endif // 10732

static mi_os_mem_config_t mi_os_mem_config = {
  4096,     // page size
  0,        // large page size (usually 2MiB)
  4096,     // allocation granularity
  MI_DEFAULT_PHYSICAL_MEMORY,
  MI_DEFAULT_VIRTUAL_ADDRESS_BITS,
  true,     // has overcommit?  (if true we use MAP_NORESERVE on mmap systems)
  false,    // can we partially free allocated blocks? (on mmap systems we can free anywhere in a mapped range, but on Windows we must free the entire span)
  true      // has virtual reserve? (if true we can reserve virtual address space without using commit or physical memory)
};

bool _mi_os_has_overcommit(void) {
  return mi_os_mem_config.has_overcommit;
}

bool _mi_os_has_virtual_reserve(void) {
  return mi_os_mem_config.has_virtual_reserve;
}


// OS (small) page size
size_t _mi_os_page_size(void) {
  return mi_os_mem_config.page_size;
}

// if large OS pages are supported (2 or 4MiB), then return the size, otherwise return the small page size (4KiB)
size_t _mi_os_large_page_size(void) {
  return (mi_os_mem_config.large_page_size != 0 ? mi_os_mem_config.large_page_size : _mi_os_page_size());
}

bool _mi_os_use_large_page(size_t size, size_t alignment) {
  // if we have access, check the size and alignment requirements
  if (mi_os_mem_config.large_page_size == 0 || !mi_option_is_enabled(mi_option_allow_large_os_pages)) return false;
  return ((size % mi_os_mem_config.large_page_size) == 0 && (alignment % mi_os_mem_config.large_page_size) == 0);
}

// round to a good OS allocation size (bounded by max 12.5% waste)
size_t _mi_os_good_alloc_size(size_t size) {
  size_t align_size;
  if (size < 512*MI_KiB) align_size = _mi_os_page_size();
  else if (size < 2*MI_MiB) align_size = 64*MI_KiB;
  else if (size < 8*MI_MiB) align_size = 256*MI_KiB;
  else if (size < 32*MI_MiB) align_size = 1*MI_MiB;
  else align_size = 4*MI_MiB;
  if mi_unlikely(size >= (SIZE_MAX - align_size)) return size; // possible overflow?
  return _mi_align_up(size, align_size);
}

void _mi_os_init(void) {
  _mi_prim_mem_init(&mi_os_mem_config);
}


/* -----------------------------------------------------------
  Util
-------------------------------------------------------------- */
bool _mi_os_decommit(void* addr, size_t size, mi_stats_t* stats);
bool _mi_os_commit(void* addr, size_t size, bool* is_zero, mi_stats_t* tld_stats);

static inline uintptr_t _mi_align_down(uintptr_t sz, size_t alignment) {
  mi_assert_internal(alignment != 0);
  uintptr_t mask = alignment - 1;
  if ((alignment & mask) == 0) { // power of two?
    return (sz & ~mask);
  }
  else {
    return ((sz / alignment) * alignment);
  }
}

static void* mi_align_down_ptr(void* p, size_t alignment) {
  return (void*)_mi_align_down((uintptr_t)p, alignment);
}


/* -----------------------------------------------------------
  aligned hinting
-------------------------------------------------------------- */

// On systems with enough virtual address bits, we can do efficient aligned allocation by using
// the 2TiB to 30TiB area to allocate those. If we have at least 46 bits of virtual address
// space (64TiB) we use this technique. (but see issue #939)
#if (MI_INTPTR_SIZE >= 8) && !defined(MI_NO_ALIGNED_HINT)
static mi_decl_cache_align _Atomic(uintptr_t)aligned_base;

// Return a MI_SEGMENT_SIZE aligned address that is probably available.
// If this returns NULL, the OS will determine the address but on some OS's that may not be
// properly aligned which can be more costly as it needs to be adjusted afterwards.
// For a size > 1GiB this always returns NULL in order to guarantee good ASLR randomization;
// (otherwise an initial large allocation of say 2TiB has a 50% chance to include (known) addresses
//  in the middle of the 2TiB - 6TiB address range (see issue #372))

#define MI_HINT_BASE ((uintptr_t)2 << 40)  // 2TiB start
#define MI_HINT_AREA ((uintptr_t)4 << 40)  // upto 6TiB   (since before win8 there is "only" 8TiB available to processes)
#define MI_HINT_MAX  ((uintptr_t)30 << 40) // wrap after 30TiB (area after 32TiB is used for huge OS pages)

void* _mi_os_get_aligned_hint(size_t try_alignment, size_t size)
{
  if (try_alignment <= 1 || try_alignment > MI_SEGMENT_SIZE) return NULL;
  if (mi_os_mem_config.virtual_address_bits < 46) return NULL;  // < 64TiB virtual address space
  size = _mi_align_up(size, MI_SEGMENT_SIZE);
  if (size > 1*MI_GiB) return NULL;  // guarantee the chance of fixed valid address is at most 1/(MI_HINT_AREA / 1<<30) = 1/4096.
  #if (MI_SECURE>0)
  size += MI_SEGMENT_SIZE;        // put in `MI_SEGMENT_SIZE` virtual gaps between hinted blocks; this splits VLA's but increases guarded areas.
  #endif // 10842

  uintptr_t hint = mi_atomic_add_acq_rel(&aligned_base, size);
  if (hint == 0 || hint > MI_HINT_MAX) {   // wrap or initialize
    uintptr_t init = MI_HINT_BASE;
    #if (MI_SECURE>0 || MI_DEBUG==0)       // security: randomize start of aligned allocations unless in debug mode
    uintptr_t r = _mi_heap_random_next(mi_prim_get_default_heap());
    init = init + ((MI_SEGMENT_SIZE * ((r>>17) & 0xFFFFF)) % MI_HINT_AREA);  // (randomly 20 bits)*4MiB == 0 to 4TiB
    #endif // 10849
    uintptr_t expected = hint + size;
    mi_atomic_cas_strong_acq_rel(&aligned_base, &expected, init);
    hint = mi_atomic_add_acq_rel(&aligned_base, size); // this may still give 0 or > MI_HINT_MAX but that is ok, it is a hint after all
  }
  if (hint%try_alignment != 0) return NULL;
  return (void*)hint;
}
#else
void* _mi_os_get_aligned_hint(size_t try_alignment, size_t size) {
  MI_UNUSED(try_alignment); MI_UNUSED(size);
  return NULL;
}
#endif // 10822


/* -----------------------------------------------------------
  Free memory
-------------------------------------------------------------- */

static void mi_os_free_huge_os_pages(void* p, size_t size, mi_stats_t* stats);

static void mi_os_prim_free(void* addr, size_t size, bool still_committed, mi_stats_t* tld_stats) {
  MI_UNUSED(tld_stats);
  mi_stats_t* stats = &_mi_stats_main;
  mi_assert_internal((size % _mi_os_page_size()) == 0);
  if (addr == NULL || size == 0) return; // || _mi_os_is_huge_reserved(addr)
  int err = _mi_prim_free(addr, size);
  if (err != 0) {
    _mi_warning_message("unable to free OS memory (error: %d (0x%x), size: 0x%zx bytes, address: %p)\n", err, err, size, addr);
  }
  if (still_committed) { _mi_stat_decrease(&stats->committed, size); }
  _mi_stat_decrease(&stats->reserved, size);
}

void _mi_os_free_ex(void* addr, size_t size, bool still_committed, mi_memid_t memid, mi_stats_t* stats) {
  if (stats == NULL) stats = &_mi_stats_main;
  if (mi_memkind_is_os(memid.memkind)) {
    size_t csize = _mi_os_good_alloc_size(size);
    void* base = addr;
    // different base? (due to alignment)
    if (memid.mem.os.base != NULL) {
      mi_assert(memid.mem.os.base <= addr);
      mi_assert((uint8_t*)memid.mem.os.base + memid.mem.os.alignment >= (uint8_t*)addr);
      base = memid.mem.os.base;
      csize += ((uint8_t*)addr - (uint8_t*)memid.mem.os.base);
    }
    // free it
    if (memid.memkind == MI_MEM_OS_HUGE) {
      mi_assert(memid.is_pinned);
      mi_os_free_huge_os_pages(base, csize, stats);
    }
    else {
      mi_os_prim_free(base, csize, still_committed, stats);
    }
  }
  else {
    // nothing to do
    mi_assert(memid.memkind < MI_MEM_OS);
  }
}

void  _mi_os_free(void* p, size_t size, mi_memid_t memid, mi_stats_t* stats) {
  if (stats == NULL) stats = &_mi_stats_main;
  _mi_os_free_ex(p, size, true, memid, stats);
}


/* -----------------------------------------------------------
   Primitive allocation from the OS.
-------------------------------------------------------------- */

// Note: the `try_alignment` is just a hint and the returned pointer is not guaranteed to be aligned.
// Also `hint_addr` is a hint and may be ignored.
static void* mi_os_prim_alloc_at(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, mi_stats_t* tld_stats) {
  mi_assert_internal(size > 0 && (size % _mi_os_page_size()) == 0);
  mi_assert_internal(is_zero != NULL);
  mi_assert_internal(is_large != NULL);
  if (size == 0) return NULL;
  if (!commit) { allow_large = false; }
  if (try_alignment == 0) { try_alignment = 1; } // avoid 0 to ensure there will be no divide by zero when aligning
  *is_zero = false;
  void* p = NULL;
  int err = _mi_prim_alloc(hint_addr, size, try_alignment, commit, allow_large, is_large, is_zero, &p);
  if (err != 0) {
    _mi_warning_message("unable to allocate OS memory (error: %d (0x%x), addr: %p, size: 0x%zx bytes, align: 0x%zx, commit: %d, allow large: %d)\n", err, err, hint_addr, size, try_alignment, commit, allow_large);
  }

  MI_UNUSED(tld_stats);
  mi_stats_t* stats = &_mi_stats_main;
  mi_stat_counter_increase(stats->mmap_calls, 1);
  if (p != NULL) {
    _mi_stat_increase(&stats->reserved, size);
    if (commit) {
      _mi_stat_increase(&stats->committed, size);
      // seems needed for asan (or `mimalloc-test-api` fails)
      #ifdef MI_TRACK_ASAN
      if (*is_zero) { mi_track_mem_defined(p,size); }
               else { mi_track_mem_undefined(p,size); }
      #endif // 10948
    }
  }
  return p;
}

static void* mi_os_prim_alloc(size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, mi_stats_t* tld_stats) {
  return mi_os_prim_alloc_at(NULL, size, try_alignment, commit, allow_large, is_large, is_zero, tld_stats);
}


// Primitive aligned allocation from the OS.
// This function guarantees the allocated memory is aligned.
static void* mi_os_prim_alloc_aligned(size_t size, size_t alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** base, mi_stats_t* stats) {
  mi_assert_internal(alignment >= _mi_os_page_size() && ((alignment & (alignment - 1)) == 0));
  mi_assert_internal(size > 0 && (size % _mi_os_page_size()) == 0);
  mi_assert_internal(is_large != NULL);
  mi_assert_internal(is_zero != NULL);
  mi_assert_internal(base != NULL);
  if (!commit) allow_large = false;
  if (!(alignment >= _mi_os_page_size() && ((alignment & (alignment - 1)) == 0))) return NULL;
  size = _mi_align_up(size, _mi_os_page_size());

  // try first with a requested alignment hint (this will usually be aligned directly on Win 10+ or BSD)
  void* p = mi_os_prim_alloc(size, alignment, commit, allow_large, is_large, is_zero, stats);
  if (p == NULL) return NULL;

  // aligned already?
  if (((uintptr_t)p % alignment) == 0) {
    *base = p;
  }
  else {
    // if not aligned, free it, overallocate, and unmap around it
    #if !MI_TRACK_ASAN
    _mi_warning_message("unable to allocate aligned OS memory directly, fall back to over-allocation (size: 0x%zx bytes, address: %p, alignment: 0x%zx, commit: %d)\n", size, p, alignment, commit);
    #endif // 10984
    mi_os_prim_free(p, size, commit, stats);
    if (size >= (SIZE_MAX - alignment)) return NULL; // overflow
    const size_t over_size = size + alignment;

    if (!mi_os_mem_config.has_partial_free) {  // win32 virtualAlloc cannot free parts of an allocated block
      // over-allocate uncommitted (virtual) memory
      p = mi_os_prim_alloc(over_size, 1 /*alignment*/, false /* commit? */, false /* allow_large */, is_large, is_zero, stats);
      if (p == NULL) return NULL;

      // set p to the aligned part in the full region
      // note: this is dangerous on Windows as VirtualFree needs the actual base pointer
      // this is handled though by having the `base` field in the memid's
      *base = p; // remember the base
      p = mi_align_up_ptr(p, alignment);

      // explicitly commit only the aligned part
      if (commit) {
        _mi_os_commit(p, size, NULL, stats);
      }
    }
    else  { // mmap can free inside an allocation
      // overallocate...
      p = mi_os_prim_alloc(over_size, 1, commit, false, is_large, is_zero, stats);
      if (p == NULL) return NULL;

      // and selectively unmap parts around the over-allocated area.
      void* aligned_p = mi_align_up_ptr(p, alignment);
      size_t pre_size = (uint8_t*)aligned_p - (uint8_t*)p;
      size_t mid_size = _mi_align_up(size, _mi_os_page_size());
      size_t post_size = over_size - pre_size - mid_size;
      mi_assert_internal(pre_size < over_size&& post_size < over_size&& mid_size >= size);
      if (pre_size > 0)  { mi_os_prim_free(p, pre_size, commit, stats); }
      if (post_size > 0) { mi_os_prim_free((uint8_t*)aligned_p + mid_size, post_size, commit, stats); }
      // we can return the aligned pointer on `mmap` systems
      p = aligned_p;
      *base = aligned_p; // since we freed the pre part, `*base == p`.
    }
  }

  mi_assert_internal(p == NULL || (p != NULL && *base != NULL && ((uintptr_t)p % alignment) == 0));
  return p;
}


/* -----------------------------------------------------------
  OS API: alloc and alloc_aligned
----------------------------------------------------------- */

void* _mi_os_alloc(size_t size, mi_memid_t* memid, mi_stats_t* stats) {
  *memid = _mi_memid_none();
  if (size == 0) return NULL;
  if (stats == NULL) stats = &_mi_stats_main;
  size = _mi_os_good_alloc_size(size);
  bool os_is_large = false;
  bool os_is_zero  = false;
  void* p = mi_os_prim_alloc(size, 0, true, false, &os_is_large, &os_is_zero, stats);
  if (p != NULL) {
    *memid = _mi_memid_create_os(true, os_is_zero, os_is_large);
  }
  return p;
}

void* _mi_os_alloc_aligned(size_t size, size_t alignment, bool commit, bool allow_large, mi_memid_t* memid, mi_stats_t* stats)
{
  MI_UNUSED(&_mi_os_get_aligned_hint); // suppress unused warnings
  *memid = _mi_memid_none();
  if (size == 0) return NULL;
  if (stats == NULL) stats = &_mi_stats_main;
  size = _mi_os_good_alloc_size(size);
  alignment = _mi_align_up(alignment, _mi_os_page_size());

  bool os_is_large = false;
  bool os_is_zero  = false;
  void* os_base = NULL;
  void* p = mi_os_prim_alloc_aligned(size, alignment, commit, allow_large, &os_is_large, &os_is_zero, &os_base, stats );
  if (p != NULL) {
    *memid = _mi_memid_create_os(commit, os_is_zero, os_is_large);
    memid->mem.os.base = os_base;
    memid->mem.os.alignment = alignment;
  }
  return p;
}

/* -----------------------------------------------------------
  OS aligned allocation with an offset. This is used
  for large alignments > MI_BLOCK_ALIGNMENT_MAX. We use a large mimalloc
  page where the object can be aligned at an offset from the start of the segment.
  As we may need to overallocate, we need to free such pointers using `mi_free_aligned`
  to use the actual start of the memory region.
----------------------------------------------------------- */

void* _mi_os_alloc_aligned_at_offset(size_t size, size_t alignment, size_t offset, bool commit, bool allow_large, mi_memid_t* memid, mi_stats_t* stats) {
  mi_assert(offset <= MI_SEGMENT_SIZE);
  mi_assert(offset <= size);
  mi_assert((alignment % _mi_os_page_size()) == 0);
  *memid = _mi_memid_none();
  if (stats == NULL) stats = &_mi_stats_main;
  if (offset > MI_SEGMENT_SIZE) return NULL;
  if (offset == 0) {
    // regular aligned allocation
    return _mi_os_alloc_aligned(size, alignment, commit, allow_large, memid, stats);
  }
  else {
    // overallocate to align at an offset
    const size_t extra = _mi_align_up(offset, alignment) - offset;
    const size_t oversize = size + extra;
    void* const start = _mi_os_alloc_aligned(oversize, alignment, commit, allow_large, memid, stats);
    if (start == NULL) return NULL;

    void* const p = (uint8_t*)start + extra;
    mi_assert(_mi_is_aligned((uint8_t*)p + offset, alignment));
    // decommit the overallocation at the start
    if (commit && extra > _mi_os_page_size()) {
      _mi_os_decommit(start, extra, stats);
    }
    return p;
  }
}

/* -----------------------------------------------------------
  OS memory API: reset, commit, decommit, protect, unprotect.
----------------------------------------------------------- */

// OS page align within a given area, either conservative (pages inside the area only),
// or not (straddling pages outside the area is possible)
static void* mi_os_page_align_areax(bool conservative, void* addr, size_t size, size_t* newsize) {
  mi_assert(addr != NULL && size > 0);
  if (newsize != NULL) *newsize = 0;
  if (size == 0 || addr == NULL) return NULL;

  // page align conservatively within the range
  void* start = (conservative ? mi_align_up_ptr(addr, _mi_os_page_size())
    : mi_align_down_ptr(addr, _mi_os_page_size()));
  void* end = (conservative ? mi_align_down_ptr((uint8_t*)addr + size, _mi_os_page_size())
    : mi_align_up_ptr((uint8_t*)addr + size, _mi_os_page_size()));
  ptrdiff_t diff = (uint8_t*)end - (uint8_t*)start;
  if (diff <= 0) return NULL;

  mi_assert_internal((conservative && (size_t)diff <= size) || (!conservative && (size_t)diff >= size));
  if (newsize != NULL) *newsize = (size_t)diff;
  return start;
}

static void* mi_os_page_align_area_conservative(void* addr, size_t size, size_t* newsize) {
  return mi_os_page_align_areax(true, addr, size, newsize);
}

bool _mi_os_commit(void* addr, size_t size, bool* is_zero, mi_stats_t* tld_stats) {
  MI_UNUSED(tld_stats);
  mi_stats_t* stats = &_mi_stats_main;
  if (is_zero != NULL) { *is_zero = false; }
  _mi_stat_increase(&stats->committed, size);  // use size for precise commit vs. decommit
  _mi_stat_counter_increase(&stats->commit_calls, 1);

  // page align range
  size_t csize;
  void* start = mi_os_page_align_areax(false /* conservative? */, addr, size, &csize);
  if (csize == 0) return true;

  // commit
  bool os_is_zero = false;
  int err = _mi_prim_commit(start, csize, &os_is_zero);
  if (err != 0) {
    _mi_warning_message("cannot commit OS memory (error: %d (0x%x), address: %p, size: 0x%zx bytes)\n", err, err, start, csize);
    return false;
  }
  if (os_is_zero && is_zero != NULL) {
    *is_zero = true;
    mi_assert_expensive(mi_mem_is_zero(start, csize));
  }
  // note: the following seems required for asan (otherwise `mimalloc-test-stress` fails)
  #ifdef MI_TRACK_ASAN
  if (os_is_zero) { mi_track_mem_defined(start,csize); }
             else { mi_track_mem_undefined(start,csize); }
  #endif // 11158
  return true;
}

static bool mi_os_decommit_ex(void* addr, size_t size, bool* needs_recommit, mi_stats_t* tld_stats) {
  MI_UNUSED(tld_stats);
  mi_stats_t* stats = &_mi_stats_main;
  mi_assert_internal(needs_recommit!=NULL);
  _mi_stat_decrease(&stats->committed, size);

  // page align
  size_t csize;
  void* start = mi_os_page_align_area_conservative(addr, size, &csize);
  if (csize == 0) return true;

  // decommit
  *needs_recommit = true;
  int err = _mi_prim_decommit(start,csize,needs_recommit);
  if (err != 0) {
    _mi_warning_message("cannot decommit OS memory (error: %d (0x%x), address: %p, size: 0x%zx bytes)\n", err, err, start, csize);
  }
  mi_assert_internal(err == 0);
  return (err == 0);
}

bool _mi_os_decommit(void* addr, size_t size, mi_stats_t* tld_stats) {
  bool needs_recommit;
  return mi_os_decommit_ex(addr, size, &needs_recommit, tld_stats);
}


// Signal to the OS that the address range is no longer in use
// but may be used later again. This will release physical memory
// pages and reduce swapping while keeping the memory committed.
// We page align to a conservative area inside the range to reset.
bool _mi_os_reset(void* addr, size_t size, mi_stats_t* stats) {
  // page align conservatively within the range
  size_t csize;
  void* start = mi_os_page_align_area_conservative(addr, size, &csize);
  if (csize == 0) return true;  // || _mi_os_is_huge_reserved(addr)
  _mi_stat_increase(&stats->reset, csize);
  _mi_stat_counter_increase(&stats->reset_calls, 1);

  #if (MI_DEBUG>1) && !MI_SECURE && !MI_TRACK_ENABLED // && !MI_TSAN
  memset(start, 0, csize); // pretend it is eagerly reset
  #endif // 11204

  int err = _mi_prim_reset(start, csize);
  if (err != 0) {
    _mi_warning_message("cannot reset OS memory (error: %d (0x%x), address: %p, size: 0x%zx bytes)\n", err, err, start, csize);
  }
  return (err == 0);
}


// either resets or decommits memory, returns true if the memory needs
// to be recommitted if it is to be re-used later on.
bool _mi_os_purge_ex(void* p, size_t size, bool allow_reset, mi_stats_t* stats)
{
  if (mi_option_get(mi_option_purge_delay) < 0) return false;  // is purging allowed?
  _mi_stat_counter_increase(&stats->purge_calls, 1);
  _mi_stat_increase(&stats->purged, size);

  if (mi_option_is_enabled(mi_option_purge_decommits) &&   // should decommit?
    !_mi_preloading())                                     // don't decommit during preloading (unsafe)
  {
    bool needs_recommit = true;
    mi_os_decommit_ex(p, size, &needs_recommit, stats);
    return needs_recommit;
  }
  else {
    if (allow_reset) {  // this can sometimes be not allowed if the range is not fully committed
      _mi_os_reset(p, size, stats);
    }
    return false;  // needs no recommit
  }
}

// either resets or decommits memory, returns true if the memory needs
// to be recommitted if it is to be re-used later on.
bool _mi_os_purge(void* p, size_t size, mi_stats_t * stats) {
  return _mi_os_purge_ex(p, size, true, stats);
}


// Protect a region in memory to be not accessible.
static  bool mi_os_protectx(void* addr, size_t size, bool protect) {
  // page align conservatively within the range
  size_t csize = 0;
  void* start = mi_os_page_align_area_conservative(addr, size, &csize);
  if (csize == 0) return false;
  /*
  if (_mi_os_is_huge_reserved(addr)) {
	  _mi_warning_message("cannot mprotect memory allocated in huge OS pages\n");
  }
  */
  int err = _mi_prim_protect(start,csize,protect);
  if (err != 0) {
    _mi_warning_message("cannot %s OS memory (error: %d (0x%x), address: %p, size: 0x%zx bytes)\n", (protect ? "protect" : "unprotect"), err, err, start, csize);
  }
  return (err == 0);
}

bool _mi_os_protect(void* addr, size_t size) {
  return mi_os_protectx(addr, size, true);
}

bool _mi_os_unprotect(void* addr, size_t size) {
  return mi_os_protectx(addr, size, false);
}



/* ----------------------------------------------------------------------------
Support for allocating huge OS pages (1Gib) that are reserved up-front
and possibly associated with a specific NUMA node. (use `numa_node>=0`)
-----------------------------------------------------------------------------*/
#define MI_HUGE_OS_PAGE_SIZE  (MI_GiB)


#if (MI_INTPTR_SIZE >= 8)
// To ensure proper alignment, use our own area for huge OS pages
static mi_decl_cache_align _Atomic(uintptr_t)  mi_huge_start; // = 0

// Claim an aligned address range for huge pages
static uint8_t* mi_os_claim_huge_pages(size_t pages, size_t* total_size) {
  if (total_size != NULL) *total_size = 0;
  const size_t size = pages * MI_HUGE_OS_PAGE_SIZE;

  uintptr_t start = 0;
  uintptr_t end = 0;
  uintptr_t huge_start = mi_atomic_load_relaxed(&mi_huge_start);
  do {
    start = huge_start;
    if (start == 0) {
      // Initialize the start address after the 32TiB area
      start = ((uintptr_t)32 << 40);  // 32TiB virtual start address
    #if (MI_SECURE>0 || MI_DEBUG==0)      // security: randomize start of huge pages unless in debug mode
      uintptr_t r = _mi_heap_random_next(mi_prim_get_default_heap());
      start = start + ((uintptr_t)MI_HUGE_OS_PAGE_SIZE * ((r>>17) & 0x0FFF));  // (randomly 12bits)*1GiB == between 0 to 4TiB
    #endif // 11298
    }
    end = start + size;
    mi_assert_internal(end % MI_SEGMENT_SIZE == 0);
  } while (!mi_atomic_cas_strong_acq_rel(&mi_huge_start, &huge_start, end));

  if (total_size != NULL) *total_size = size;
  return (uint8_t*)start;
}
#else
static uint8_t* mi_os_claim_huge_pages(size_t pages, size_t* total_size) {
  MI_UNUSED(pages);
  if (total_size != NULL) *total_size = 0;
  return NULL;
}
#endif // 11281

// Allocate MI_SEGMENT_SIZE aligned huge pages
void* _mi_os_alloc_huge_os_pages(size_t pages, int numa_node, mi_msecs_t max_msecs, size_t* pages_reserved, size_t* psize, mi_memid_t* memid) {
  *memid = _mi_memid_none();
  if (psize != NULL) *psize = 0;
  if (pages_reserved != NULL) *pages_reserved = 0;
  size_t size = 0;
  uint8_t* start = mi_os_claim_huge_pages(pages, &size);
  if (start == NULL) return NULL; // or 32-bit systems

  // Allocate one page at the time but try to place them contiguously
  // We allocate one page at the time to be able to abort if it takes too long
  // or to at least allocate as many as available on the system.
  mi_msecs_t start_t = _mi_clock_start();
  size_t page = 0;
  bool all_zero = true;
  while (page < pages) {
    // allocate a page
    bool is_zero = false;
    void* addr = start + (page * MI_HUGE_OS_PAGE_SIZE);
    void* p = NULL;
    int err = _mi_prim_alloc_huge_os_pages(addr, MI_HUGE_OS_PAGE_SIZE, numa_node, &is_zero, &p);
    if (!is_zero) { all_zero = false;  }
    if (err != 0) {
      _mi_warning_message("unable to allocate huge OS page (error: %d (0x%x), address: %p, size: %zx bytes)\n", err, err, addr, MI_HUGE_OS_PAGE_SIZE);
      break;
    }

    // Did we succeed at a contiguous address?
    if (p != addr) {
      // no success, issue a warning and break
      if (p != NULL) {
        _mi_warning_message("could not allocate contiguous huge OS page %zu at %p\n", page, addr);
        mi_os_prim_free(p, MI_HUGE_OS_PAGE_SIZE, true, &_mi_stats_main);
      }
      break;
    }

    // success, record it
    page++;  // increase before timeout check (see issue #711)
    _mi_stat_increase(&_mi_stats_main.committed, MI_HUGE_OS_PAGE_SIZE);
    _mi_stat_increase(&_mi_stats_main.reserved, MI_HUGE_OS_PAGE_SIZE);

    // check for timeout
    if (max_msecs > 0) {
      mi_msecs_t elapsed = _mi_clock_end(start_t);
      if (page >= 1) {
        mi_msecs_t estimate = ((elapsed / (page+1)) * pages);
        if (estimate > 2*max_msecs) { // seems like we are going to timeout, break
          elapsed = max_msecs + 1;
        }
      }
      if (elapsed > max_msecs) {
        _mi_warning_message("huge OS page allocation timed out (after allocating %zu page(s))\n", page);
        break;
      }
    }
  }
  mi_assert_internal(page*MI_HUGE_OS_PAGE_SIZE <= size);
  if (pages_reserved != NULL) { *pages_reserved = page; }
  if (psize != NULL) { *psize = page * MI_HUGE_OS_PAGE_SIZE; }
  if (page != 0) {
    mi_assert(start != NULL);
    *memid = _mi_memid_create_os(true /* is committed */, all_zero, true /* is_large */);
    memid->memkind = MI_MEM_OS_HUGE;
    mi_assert(memid->is_pinned);
    #ifdef MI_TRACK_ASAN
    if (all_zero) { mi_track_mem_defined(start,size); }
    #endif // 11383
  }
  return (page == 0 ? NULL : start);
}

// free every huge page in a range individually (as we allocated per page)
// note: needed with VirtualAlloc but could potentially be done in one go on mmap'd systems.
static void mi_os_free_huge_os_pages(void* p, size_t size, mi_stats_t* stats) {
  if (p==NULL || size==0) return;
  uint8_t* base = (uint8_t*)p;
  while (size >= MI_HUGE_OS_PAGE_SIZE) {
    mi_os_prim_free(base, MI_HUGE_OS_PAGE_SIZE, true, stats);
    size -= MI_HUGE_OS_PAGE_SIZE;
    base += MI_HUGE_OS_PAGE_SIZE;
  }
}

/* ----------------------------------------------------------------------------
Support NUMA aware allocation
-----------------------------------------------------------------------------*/

_Atomic(size_t)  _mi_numa_node_count; // = 0   // cache the node count

size_t _mi_os_numa_node_count_get(void) {
  size_t count = mi_atomic_load_acquire(&_mi_numa_node_count);
  if (count <= 0) {
    long ncount = mi_option_get(mi_option_use_numa_nodes); // given explicitly?
    if (ncount > 0) {
      count = (size_t)ncount;
    }
    else {
      count = _mi_prim_numa_node_count(); // or detect dynamically
      if (count == 0) count = 1;
    }
    mi_atomic_store_release(&_mi_numa_node_count, count); // save it
    _mi_verbose_message("using %zd numa regions\n", count);
  }
  return count;
}

int _mi_os_numa_node_get(mi_os_tld_t* tld) {
  MI_UNUSED(tld);
  size_t numa_count = _mi_os_numa_node_count();
  if (numa_count<=1) return 0; // optimize on single numa node systems: always node 0
  // never more than the node count and >= 0
  size_t numa_node = _mi_prim_numa_node();
  if (numa_node >= numa_count) { numa_node = numa_node % numa_count; }
  return (int)numa_node;
}
// --- Amalgamation END --- mimalloc/src/os.c


// --- Amalgamation START --- mimalloc/src/page.c
/*----------------------------------------------------------------------------
Copyright (c) 2018-2024, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

/* -----------------------------------------------------------
  The core of the allocator. Every segment contains
  pages of a certain block size. The main function
  exported is `mi_malloc_generic`.
----------------------------------------------------------- */


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


/* -----------------------------------------------------------
  Definition of page queues for each block size
----------------------------------------------------------- */

#define MI_IN_PAGE_C

// --- Amalgamation START --- mimalloc/src/page-queue.c
/*----------------------------------------------------------------------------
Copyright (c) 2018-2024, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

/* -----------------------------------------------------------
  Definition of page queues for each block size
----------------------------------------------------------- */

#ifndef MI_IN_PAGE_C
#error "this file should be included from 'page.c'"
// include to help an IDE

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h

#endif // 11482

/* -----------------------------------------------------------
  Minimal alignment in machine words (i.e. `sizeof(void*)`)
----------------------------------------------------------- */

#if (MI_MAX_ALIGN_SIZE > 4*MI_INTPTR_SIZE)
  #error "define alignment for more than 4x word size for this platform"
#elif (MI_MAX_ALIGN_SIZE > 2*MI_INTPTR_SIZE)
  #define MI_ALIGN4W   // 4 machine words minimal alignment
#elif (MI_MAX_ALIGN_SIZE > MI_INTPTR_SIZE)
  #define MI_ALIGN2W   // 2 machine words minimal alignment
#else
  // ok, default alignment is 1 word
#endif // 11503


/* -----------------------------------------------------------
  Queue query
----------------------------------------------------------- */


static inline bool mi_page_queue_is_huge(const mi_page_queue_t* pq) {
  return (pq->block_size == (MI_LARGE_OBJ_SIZE_MAX+sizeof(uintptr_t)));
}

static inline bool mi_page_queue_is_full(const mi_page_queue_t* pq) {
  return (pq->block_size == (MI_LARGE_OBJ_SIZE_MAX+(2*sizeof(uintptr_t))));
}

static inline bool mi_page_queue_is_special(const mi_page_queue_t* pq) {
  return (pq->block_size > MI_LARGE_OBJ_SIZE_MAX);
}

/* -----------------------------------------------------------
  Bins
----------------------------------------------------------- */

// Return the bin for a given field size.
// Returns MI_BIN_HUGE if the size is too large.
// We use `wsize` for the size in "machine word sizes",
// i.e. byte size == `wsize*sizeof(void*)`.
static inline uint8_t mi_bin(size_t size) {
  size_t wsize = _mi_wsize_from_size(size);
  uint8_t bin;
  if (wsize <= 1) {
    bin = 1;
  }
  #if defined(MI_ALIGN4W)
  else if (wsize <= 4) {
    bin = (uint8_t)((wsize+1)&~1); // round to double word sizes
  }
  #elif defined(MI_ALIGN2W)
  else if (wsize <= 8) {
    bin = (uint8_t)((wsize+1)&~1); // round to double word sizes
  }
  #else
  else if (wsize <= 8) {
    bin = (uint8_t)wsize;
  }
  #endif // 11545
  else if (wsize > MI_LARGE_OBJ_WSIZE_MAX) {
    bin = MI_BIN_HUGE;
  }
  else {
    #if defined(MI_ALIGN4W)
    if (wsize <= 16) { wsize = (wsize+3)&~3; } // round to 4x word sizes
    #endif // 11562
    wsize--;
    // find the highest bit
    uint8_t b = (uint8_t)mi_bsr(wsize);  // note: wsize != 0
    // and use the top 3 bits to determine the bin (~12.5% worst internal fragmentation).
    // - adjust with 3 because we use do not round the first 8 sizes
    //   which each get an exact bin
    bin = ((b << 2) + (uint8_t)((wsize >> (b - 2)) & 0x03)) - 3;
    mi_assert_internal(bin < MI_BIN_HUGE);
  }
  mi_assert_internal(bin > 0 && bin <= MI_BIN_HUGE);
  return bin;
}



/* -----------------------------------------------------------
  Queue of pages with free blocks
----------------------------------------------------------- */

uint8_t _mi_bin(size_t size) {
  return mi_bin(size);
}

size_t _mi_bin_size(uint8_t bin) {
  return _mi_heap_empty.pages[bin].block_size;
}

// Good size for allocation
size_t mi_good_size(size_t size) mi_attr_noexcept {
  if (size <= MI_LARGE_OBJ_SIZE_MAX) {
    return _mi_bin_size(mi_bin(size + MI_PADDING_SIZE));
  }
  else {
    return _mi_align_up(size + MI_PADDING_SIZE,_mi_os_page_size());
  }
}

#if (MI_DEBUG>1)
static bool mi_page_queue_contains(mi_page_queue_t* queue, const mi_page_t* page) {
  mi_assert_internal(page != NULL);
  mi_page_t* list = queue->first;
  while (list != NULL) {
    mi_assert_internal(list->next == NULL || list->next->prev == list);
    mi_assert_internal(list->prev == NULL || list->prev->next == list);
    if (list == page) break;
    list = list->next;
  }
  return (list == page);
}

#endif // 11602

#if (MI_DEBUG>1)
static bool mi_heap_contains_queue(const mi_heap_t* heap, const mi_page_queue_t* pq) {
  return (pq >= &heap->pages[0] && pq <= &heap->pages[MI_BIN_FULL]);
}
#endif // 11617

static mi_page_queue_t* mi_heap_page_queue_of(mi_heap_t* heap, const mi_page_t* page) {
  mi_assert_internal(heap!=NULL);
  uint8_t bin = (mi_page_is_in_full(page) ? MI_BIN_FULL : (mi_page_is_huge(page) ? MI_BIN_HUGE : mi_bin(mi_page_block_size(page))));
  mi_assert_internal(bin <= MI_BIN_FULL);
  mi_page_queue_t* pq = &heap->pages[bin];
  mi_assert_internal((mi_page_block_size(page) == pq->block_size) ||
                       (mi_page_is_huge(page) && mi_page_queue_is_huge(pq)) ||
                         (mi_page_is_in_full(page) && mi_page_queue_is_full(pq)));
  return pq;
}

static mi_page_queue_t* mi_page_queue_of(const mi_page_t* page) {
  mi_heap_t* heap = mi_page_heap(page);
  mi_page_queue_t* pq = mi_heap_page_queue_of(heap, page);
  mi_assert_expensive(mi_page_queue_contains(pq, page));
  return pq;
}

// The current small page array is for efficiency and for each
// small size (up to 256) it points directly to the page for that
// size without having to compute the bin. This means when the
// current free page queue is updated for a small bin, we need to update a
// range of entries in `_mi_page_small_free`.
static inline void mi_heap_queue_first_update(mi_heap_t* heap, const mi_page_queue_t* pq) {
  mi_assert_internal(mi_heap_contains_queue(heap,pq));
  size_t size = pq->block_size;
  if (size > MI_SMALL_SIZE_MAX) return;

  mi_page_t* page = pq->first;
  if (pq->first == NULL) page = (mi_page_t*)&_mi_page_empty;

  // find index in the right direct page array
  size_t start;
  size_t idx = _mi_wsize_from_size(size);
  mi_page_t** pages_free = heap->pages_free_direct;

  if (pages_free[idx] == page) return;  // already set

  // find start slot
  if (idx<=1) {
    start = 0;
  }
  else {
    // find previous size; due to minimal alignment upto 3 previous bins may need to be skipped
    uint8_t bin = mi_bin(size);
    const mi_page_queue_t* prev = pq - 1;
    while( bin == mi_bin(prev->block_size) && prev > &heap->pages[0]) {
      prev--;
    }
    start = 1 + _mi_wsize_from_size(prev->block_size);
    if (start > idx) start = idx;
  }

  // set size range to the right page
  mi_assert(start <= idx);
  for (size_t sz = start; sz <= idx; sz++) {
    pages_free[sz] = page;
  }
}

/*
static bool mi_page_queue_is_empty(mi_page_queue_t* queue) {
  return (queue->first == NULL);
}
*/

static void mi_page_queue_remove(mi_page_queue_t* queue, mi_page_t* page) {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(mi_page_queue_contains(queue, page));
  mi_assert_internal(mi_page_block_size(page) == queue->block_size || 
                      (mi_page_is_huge(page) && mi_page_queue_is_huge(queue)) || 
                        (mi_page_is_in_full(page) && mi_page_queue_is_full(queue)));
  mi_heap_t* heap = mi_page_heap(page);
  if (page->prev != NULL) page->prev->next = page->next;
  if (page->next != NULL) page->next->prev = page->prev;
  if (page == queue->last)  queue->last = page->prev;
  if (page == queue->first) {
    queue->first = page->next;
    // update first
    mi_assert_internal(mi_heap_contains_queue(heap, queue));
    mi_heap_queue_first_update(heap,queue);
  }
  heap->page_count--;
  page->next = NULL;
  page->prev = NULL;
  // mi_atomic_store_ptr_release(mi_atomic_cast(void*, &page->heap), NULL);
  mi_page_set_in_full(page,false);
}


static void mi_page_queue_push(mi_heap_t* heap, mi_page_queue_t* queue, mi_page_t* page) {
  mi_assert_internal(mi_page_heap(page) == heap);
  mi_assert_internal(!mi_page_queue_contains(queue, page));
  #if MI_HUGE_PAGE_ABANDON
  mi_assert_internal(_mi_page_segment(page)->page_kind != MI_PAGE_HUGE);
  #endif // 11716
  mi_assert_internal(mi_page_block_size(page) == queue->block_size ||
                      (mi_page_is_huge(page) && mi_page_queue_is_huge(queue)) ||
                        (mi_page_is_in_full(page) && mi_page_queue_is_full(queue)));

  mi_page_set_in_full(page, mi_page_queue_is_full(queue));
  // mi_atomic_store_ptr_release(mi_atomic_cast(void*, &page->heap), heap);
  page->next = queue->first;
  page->prev = NULL;
  if (queue->first != NULL) {
    mi_assert_internal(queue->first->prev == NULL);
    queue->first->prev = page;
    queue->first = page;
  }
  else {
    queue->first = queue->last = page;
  }

  // update direct
  mi_heap_queue_first_update(heap, queue);
  heap->page_count++;
}

static void mi_page_queue_move_to_front(mi_heap_t* heap, mi_page_queue_t* queue, mi_page_t* page) {
  mi_assert_internal(mi_page_heap(page) == heap);
  mi_assert_internal(mi_page_queue_contains(queue, page));
  if (queue->first == page) return;
  mi_page_queue_remove(queue, page);
  mi_page_queue_push(heap, queue, page);
  mi_assert_internal(queue->first == page);
}

static void mi_page_queue_enqueue_from_ex(mi_page_queue_t* to, mi_page_queue_t* from, bool enqueue_at_end, mi_page_t* page) {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(mi_page_queue_contains(from, page));
  mi_assert_expensive(!mi_page_queue_contains(to, page));
  const size_t bsize = mi_page_block_size(page);
  MI_UNUSED(bsize);
  mi_assert_internal((bsize == to->block_size && bsize == from->block_size) ||
                     (bsize == to->block_size && mi_page_queue_is_full(from)) ||
                     (bsize == from->block_size && mi_page_queue_is_full(to)) ||
                     (mi_page_is_huge(page) && mi_page_queue_is_huge(to)) ||
                     (mi_page_is_huge(page) && mi_page_queue_is_full(to)));

  mi_heap_t* heap = mi_page_heap(page);

  // delete from `from`
  if (page->prev != NULL) page->prev->next = page->next;
  if (page->next != NULL) page->next->prev = page->prev;
  if (page == from->last)  from->last = page->prev;
  if (page == from->first) {
    from->first = page->next;
    // update first
    mi_assert_internal(mi_heap_contains_queue(heap, from));
    mi_heap_queue_first_update(heap, from);
  }

  // insert into `to`
  if (enqueue_at_end) {
    // enqueue at the end
    page->prev = to->last;
    page->next = NULL;
    if (to->last != NULL) {
      mi_assert_internal(heap == mi_page_heap(to->last));
      to->last->next = page;
      to->last = page;
    }
    else {
      to->first = page;
      to->last = page;
      mi_heap_queue_first_update(heap, to);
    }
  }
  else {
    if (to->first != NULL) {
      // enqueue at 2nd place
      mi_assert_internal(heap == mi_page_heap(to->first));
      mi_page_t* next = to->first->next;
      page->prev = to->first;
      page->next = next;
      to->first->next = page;
      if (next != NULL) { 
        next->prev = page; 
      }
      else {
        to->last = page;
      }
    }
    else {
      // enqueue at the head (singleton list)
      page->prev = NULL;
      page->next = NULL;
      to->first = page;
      to->last = page;
      mi_heap_queue_first_update(heap, to);
    }
  }

  mi_page_set_in_full(page, mi_page_queue_is_full(to));
}

static void mi_page_queue_enqueue_from(mi_page_queue_t* to, mi_page_queue_t* from, mi_page_t* page) {
  mi_page_queue_enqueue_from_ex(to, from, true /* enqueue at the end */, page);
}

static void mi_page_queue_enqueue_from_full(mi_page_queue_t* to, mi_page_queue_t* from, mi_page_t* page) {
  // note: we could insert at the front to increase reuse, but it slows down certain benchmarks (like `alloc-test`)
  mi_page_queue_enqueue_from_ex(to, from, false /* enqueue at the end of the `to` queue? */, page);
}

// Only called from `mi_heap_absorb`.
size_t _mi_page_queue_append(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_queue_t* append) {
  mi_assert_internal(mi_heap_contains_queue(heap,pq));
  mi_assert_internal(pq->block_size == append->block_size);

  if (append->first==NULL) return 0;

  // set append pages to new heap and count
  size_t count = 0;
  for (mi_page_t* page = append->first; page != NULL; page = page->next) {
    // inline `mi_page_set_heap` to avoid wrong assertion during absorption;
    // in this case it is ok to be delayed freeing since both "to" and "from" heap are still alive.
    mi_atomic_store_release(&page->xheap, (uintptr_t)heap);
    // set the flag to delayed free (not overriding NEVER_DELAYED_FREE) which has as a
    // side effect that it spins until any DELAYED_FREEING is finished. This ensures
    // that after appending only the new heap will be used for delayed free operations.
    _mi_page_use_delayed_free(page, MI_USE_DELAYED_FREE, false);
    count++;
  }

  if (pq->last==NULL) {
    // take over afresh
    mi_assert_internal(pq->first==NULL);
    pq->first = append->first;
    pq->last = append->last;
    mi_heap_queue_first_update(heap, pq);
  }
  else {
    // append to end
    mi_assert_internal(pq->last!=NULL);
    mi_assert_internal(append->first!=NULL);
    pq->last->next = append->first;
    append->first->prev = pq->last;
    pq->last = append->last;
  }
  return count;
}
// --- Amalgamation END --- mimalloc/src/page-queue.c

#undef MI_IN_PAGE_C


/* -----------------------------------------------------------
  Page helpers
----------------------------------------------------------- */

// Index a block in a page
static inline mi_block_t* mi_page_block_at(const mi_page_t* page, void* page_start, size_t block_size, size_t i) {
  MI_UNUSED(page);
  mi_assert_internal(page != NULL);
  mi_assert_internal(i <= page->reserved);
  return (mi_block_t*)((uint8_t*)page_start + (i * block_size));
}

static void mi_page_init(mi_heap_t* heap, mi_page_t* page, size_t size, mi_tld_t* tld);
static void mi_page_extend_free(mi_heap_t* heap, mi_page_t* page, mi_tld_t* tld);

#if (MI_DEBUG>=3)
static size_t mi_page_list_count(mi_page_t* page, mi_block_t* head) {
  size_t count = 0;
  while (head != NULL) {
    mi_assert_internal(page == _mi_ptr_page(head));
    count++;
    head = mi_block_next(page, head);
  }
  return count;
}

/*
// Start of the page available memory
static inline uint8_t* mi_page_area(const mi_page_t* page) {
  return _mi_page_start(_mi_page_segment(page), page, NULL);
}
*/

static bool mi_page_list_is_valid(mi_page_t* page, mi_block_t* p) {
  size_t psize;
  uint8_t* page_area = _mi_segment_page_start(_mi_page_segment(page), page, &psize);
  mi_block_t* start = (mi_block_t*)page_area;
  mi_block_t* end   = (mi_block_t*)(page_area + psize);
  while(p != NULL) {
    if (p < start || p >= end) return false;
    p = mi_block_next(page, p);
  }
#if MI_DEBUG>3 // generally too expensive to check this
  if (page->free_is_zero) {
    const size_t ubsize = mi_page_usable_block_size(page);
    for (mi_block_t* block = page->free; block != NULL; block = mi_block_next(page, block)) {
      mi_assert_expensive(mi_mem_is_zero(block + 1, ubsize - sizeof(mi_block_t)));
    }
  }
#endif // 11912
  return true;
}

static bool mi_page_is_valid_init(mi_page_t* page) {
  mi_assert_internal(mi_page_block_size(page) > 0);
  mi_assert_internal(page->used <= page->capacity);
  mi_assert_internal(page->capacity <= page->reserved);

  // const size_t bsize = mi_page_block_size(page);
  mi_segment_t* segment = _mi_page_segment(page);
  uint8_t* start = mi_page_start(page);
  mi_assert_internal(start == _mi_segment_page_start(segment,page,NULL));
  mi_assert_internal(page->is_huge == (segment->page_kind == MI_PAGE_HUGE));
  //mi_assert_internal(start + page->capacity*page->block_size == page->top);

  mi_assert_internal(mi_page_list_is_valid(page,page->free));
  mi_assert_internal(mi_page_list_is_valid(page,page->local_free));

  #if MI_DEBUG>3 // generally too expensive to check this
  if (page->free_is_zero) {
    const size_t ubsize = mi_page_usable_block_size(page);
    for(mi_block_t* block = page->free; block != NULL; block = mi_block_next(page,block)) {
      mi_assert_expensive(mi_mem_is_zero(block + 1, ubsize - sizeof(mi_block_t)));
    }
  }
  #endif // 11938

  #if !MI_TRACK_ENABLED && !MI_TSAN
  mi_block_t* tfree = mi_page_thread_free(page);
  mi_assert_internal(mi_page_list_is_valid(page, tfree));
  //size_t tfree_count = mi_page_list_count(page, tfree);
  //mi_assert_internal(tfree_count <= page->thread_freed + 1);
  #endif // 11947

  size_t free_count = mi_page_list_count(page, page->free) + mi_page_list_count(page, page->local_free);
  mi_assert_internal(page->used + free_count == page->capacity);

  return true;
}

extern bool _mi_process_is_initialized;             // has mi_process_init been called?

bool _mi_page_is_valid(mi_page_t* page) {
  mi_assert_internal(mi_page_is_valid_init(page));
  #if MI_SECURE
  mi_assert_internal(page->keys[0] != 0);
  #endif // 11964
  if (mi_page_heap(page)!=NULL) {
    mi_segment_t* segment = _mi_page_segment(page);
    mi_assert_internal(!_mi_process_is_initialized || segment->thread_id == mi_page_heap(page)->thread_id || segment->thread_id==0);
    #if MI_HUGE_PAGE_ABANDON
    if (segment->page_kind != MI_PAGE_HUGE)
    #endif // 11970
    {
      mi_page_queue_t* pq = mi_page_queue_of(page);
      mi_assert_internal(mi_page_queue_contains(pq, page));
      mi_assert_internal(pq->block_size==mi_page_block_size(page) || mi_page_block_size(page) > MI_LARGE_OBJ_SIZE_MAX || mi_page_is_in_full(page));
      mi_assert_internal(mi_heap_contains_queue(mi_page_heap(page),pq));
    }
  }
  return true;
}
#endif // 11885

void _mi_page_use_delayed_free(mi_page_t* page, mi_delayed_t delay, bool override_never) {
  while (!_mi_page_try_use_delayed_free(page, delay, override_never)) {
    mi_atomic_yield();
  }
}

bool _mi_page_try_use_delayed_free(mi_page_t* page, mi_delayed_t delay, bool override_never) {
  mi_thread_free_t tfreex;
  mi_delayed_t     old_delay;
  mi_thread_free_t tfree;
  size_t yield_count = 0;
  do {
    tfree = mi_atomic_load_acquire(&page->xthread_free); // note: must acquire as we can break/repeat this loop and not do a CAS;
    tfreex = mi_tf_set_delayed(tfree, delay);
    old_delay = mi_tf_delayed(tfree);
    if mi_unlikely(old_delay == MI_DELAYED_FREEING) {
      if (yield_count >= 4) return false;  // give up after 4 tries
      yield_count++;
      mi_atomic_yield(); // delay until outstanding MI_DELAYED_FREEING are done.
      // tfree = mi_tf_set_delayed(tfree, MI_NO_DELAYED_FREE); // will cause CAS to busy fail
    }
    else if (delay == old_delay) {
      break; // avoid atomic operation if already equal
    }
    else if (!override_never && old_delay == MI_NEVER_DELAYED_FREE) {
      break; // leave never-delayed flag set
    }
  } while ((old_delay == MI_DELAYED_FREEING) ||
           !mi_atomic_cas_weak_release(&page->xthread_free, &tfree, tfreex));

  return true; // success
}

/* -----------------------------------------------------------
  Page collect the `local_free` and `thread_free` lists
----------------------------------------------------------- */

// Collect the local `thread_free` list using an atomic exchange.
// Note: The exchange must be done atomically as this is used right after
// moving to the full list in `mi_page_collect_ex` and we need to
// ensure that there was no race where the page became unfull just before the move.
static void _mi_page_thread_free_collect(mi_page_t* page)
{
  mi_block_t* head;
  mi_thread_free_t tfreex;
  mi_thread_free_t tfree = mi_atomic_load_relaxed(&page->xthread_free);
  do {
    head = mi_tf_block(tfree);
    tfreex = mi_tf_set_block(tfree,NULL);
  } while (!mi_atomic_cas_weak_acq_rel(&page->xthread_free, &tfree, tfreex));

  // return if the list is empty
  if (head == NULL) return;

  // find the tail -- also to get a proper count (without data races)
  size_t max_count = page->capacity; // cannot collect more than capacity
  size_t count = 1;
  mi_block_t* tail = head;
  mi_block_t* next;
  while ((next = mi_block_next(page,tail)) != NULL && count <= max_count) {
    count++;
    tail = next;
  }
  // if `count > max_count` there was a memory corruption (possibly infinite list due to double multi-threaded free)
  if (count > max_count) {
    _mi_error_message(EFAULT, "corrupted thread-free list\n");
    return; // the thread-free items cannot be freed
  }

  // and append the current local free list
  mi_block_set_next(page,tail, page->local_free);
  page->local_free = head;

  // update counts now
  page->used -= (uint16_t)count;
}

void _mi_page_free_collect(mi_page_t* page, bool force) {
  mi_assert_internal(page!=NULL);

  // collect the thread free list
  if (force || mi_page_thread_free(page) != NULL) {  // quick test to avoid an atomic operation
    _mi_page_thread_free_collect(page);
  }

  // and the local free list
  if (page->local_free != NULL) {
    if mi_likely(page->free == NULL) {
      // usual case
      page->free = page->local_free;
      page->local_free = NULL;
      page->free_is_zero = false;
    }
    else if (force) {
      // append -- only on shutdown (force) as this is a linear operation
      mi_block_t* tail = page->local_free;
      mi_block_t* next;
      while ((next = mi_block_next(page, tail)) != NULL) {
        tail = next;
      }
      mi_block_set_next(page, tail, page->free);
      page->free = page->local_free;
      page->local_free = NULL;
      page->free_is_zero = false;
    }
  }

  mi_assert_internal(!force || page->local_free == NULL);
}



/* -----------------------------------------------------------
  Page fresh and retire
----------------------------------------------------------- */

// called from segments when reclaiming abandoned pages
void _mi_page_reclaim(mi_heap_t* heap, mi_page_t* page) {
  mi_assert_expensive(mi_page_is_valid_init(page));
  mi_assert_internal(mi_page_heap(page) == heap);
  mi_assert_internal(mi_page_thread_free_flag(page) != MI_NEVER_DELAYED_FREE);
  #if MI_HUGE_PAGE_ABANDON
  mi_assert_internal(_mi_page_segment(page)->page_kind != MI_PAGE_HUGE);
  #endif // 12105

  // TODO: push on full queue immediately if it is full?
  mi_page_queue_t* pq = mi_page_queue(heap, mi_page_block_size(page));
  mi_page_queue_push(heap, pq, page);
  mi_assert_expensive(_mi_page_is_valid(page));
}

// allocate a fresh page from a segment
static mi_page_t* mi_page_fresh_alloc(mi_heap_t* heap, mi_page_queue_t* pq, size_t block_size, size_t page_alignment) {
  #if !MI_HUGE_PAGE_ABANDON
  mi_assert_internal(pq != NULL);
  mi_assert_internal(mi_heap_contains_queue(heap, pq));
  mi_assert_internal(page_alignment > 0 || block_size > MI_LARGE_OBJ_SIZE_MAX || block_size == pq->block_size);
  #endif // 12117
  mi_page_t* page = _mi_segment_page_alloc(heap, block_size, page_alignment, &heap->tld->segments, &heap->tld->os);
  if (page == NULL) {
    // this may be out-of-memory, or an abandoned page was reclaimed (and in our queue)
    return NULL;
  }
  #if MI_HUGE_PAGE_ABANDON
  mi_assert_internal(pq==NULL || _mi_page_segment(page)->page_kind != MI_PAGE_HUGE);
  #endif // 12127
  mi_assert_internal(pq!=NULL || mi_page_block_size(page) >= block_size);
  // a fresh page was found, initialize it
  const size_t full_block_size = (pq == NULL || mi_page_is_huge(page) ? mi_page_block_size(page) : block_size); // see also: mi_segment_huge_page_alloc
  mi_assert_internal(full_block_size >= block_size);
  mi_page_init(heap, page, full_block_size, heap->tld);
  mi_heap_stat_increase(heap, pages, 1);
  if (pq != NULL) { mi_page_queue_push(heap, pq, page); }
  mi_assert_expensive(_mi_page_is_valid(page));
  return page;
}

// Get a fresh page to use
static mi_page_t* mi_page_fresh(mi_heap_t* heap, mi_page_queue_t* pq) {
  mi_assert_internal(mi_heap_contains_queue(heap, pq));
  mi_page_t* page = mi_page_fresh_alloc(heap, pq, pq->block_size, 0);
  if (page==NULL) return NULL;
  mi_assert_internal(pq->block_size==mi_page_block_size(page));
  mi_assert_internal(pq==mi_page_queue(heap, mi_page_block_size(page)));
  return page;
}

/* -----------------------------------------------------------
   Do any delayed frees
   (put there by other threads if they deallocated in a full page)
----------------------------------------------------------- */
void _mi_heap_delayed_free_all(mi_heap_t* heap) {
  while (!_mi_heap_delayed_free_partial(heap)) {
    mi_atomic_yield();
  }
}

// returns true if all delayed frees were processed
bool _mi_heap_delayed_free_partial(mi_heap_t* heap) {
  // take over the list (note: no atomic exchange since it is often NULL)
  mi_block_t* block = mi_atomic_load_ptr_relaxed(mi_block_t, &heap->thread_delayed_free);
  while (block != NULL && !mi_atomic_cas_ptr_weak_acq_rel(mi_block_t, &heap->thread_delayed_free, &block, NULL)) { /* nothing */ };
  bool all_freed = true;

  // and free them all
  while(block != NULL) {
    mi_block_t* next = mi_block_nextx(heap,block, heap->keys);
    // use internal free instead of regular one to keep stats etc correct
    if (!_mi_free_delayed_block(block)) {
      // we might already start delayed freeing while another thread has not yet
      // reset the delayed_freeing flag; in that case delay it further by reinserting the current block
      // into the delayed free list
      all_freed = false;
      mi_block_t* dfree = mi_atomic_load_ptr_relaxed(mi_block_t, &heap->thread_delayed_free);
      do {
        mi_block_set_nextx(heap, block, dfree, heap->keys);
      } while (!mi_atomic_cas_ptr_weak_release(mi_block_t,&heap->thread_delayed_free, &dfree, block));
    }
    block = next;
  }
  return all_freed;
}

/* -----------------------------------------------------------
  Unfull, abandon, free and retire
----------------------------------------------------------- */

// Move a page from the full list back to a regular list
void _mi_page_unfull(mi_page_t* page) {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(_mi_page_is_valid(page));
  mi_assert_internal(mi_page_is_in_full(page));
  if (!mi_page_is_in_full(page)) return;

  mi_heap_t* heap = mi_page_heap(page);
  mi_page_queue_t* pqfull = &heap->pages[MI_BIN_FULL];
  mi_page_set_in_full(page, false); // to get the right queue
  mi_page_queue_t* pq = mi_heap_page_queue_of(heap, page);
  mi_page_set_in_full(page, true);
  mi_page_queue_enqueue_from_full(pq, pqfull, page);
}

static void mi_page_to_full(mi_page_t* page, mi_page_queue_t* pq) {
  mi_assert_internal(pq == mi_page_queue_of(page));
  mi_assert_internal(!mi_page_immediate_available(page));
  mi_assert_internal(!mi_page_is_in_full(page));

  if (mi_page_is_in_full(page)) return;
  mi_page_queue_enqueue_from(&mi_page_heap(page)->pages[MI_BIN_FULL], pq, page);
  _mi_page_free_collect(page,false);  // try to collect right away in case another thread freed just before MI_USE_DELAYED_FREE was set
}


// Abandon a page with used blocks at the end of a thread.
// Note: only call if it is ensured that no references exist from
// the `page->heap->thread_delayed_free` into this page.
// Currently only called through `mi_heap_collect_ex` which ensures this.
void _mi_page_abandon(mi_page_t* page, mi_page_queue_t* pq) {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(_mi_page_is_valid(page));
  mi_assert_internal(pq == mi_page_queue_of(page));
  mi_assert_internal(mi_page_heap(page) != NULL);

  mi_heap_t* pheap = mi_page_heap(page);

  // remove from our page list
  mi_segments_tld_t* segments_tld = &pheap->tld->segments;
  mi_page_queue_remove(pq, page);

  // page is no longer associated with our heap
  mi_assert_internal(mi_page_thread_free_flag(page)==MI_NEVER_DELAYED_FREE);
  mi_page_set_heap(page, NULL);

#if (MI_DEBUG>1) && !MI_TRACK_ENABLED
  // check there are no references left..
  for (mi_block_t* block = (mi_block_t*)pheap->thread_delayed_free; block != NULL; block = mi_block_nextx(pheap, block, pheap->keys)) {
    mi_assert_internal(_mi_ptr_page(block) != page);
  }
#endif // 12237

  // and abandon it
  mi_assert_internal(mi_page_heap(page) == NULL);
  _mi_segment_page_abandon(page,segments_tld);
}

// force abandon a page
void _mi_page_force_abandon(mi_page_t* page) {
  mi_heap_t* heap = mi_page_heap(page);
  // mark page as not using delayed free
  _mi_page_use_delayed_free(page, MI_NEVER_DELAYED_FREE, false);

  // ensure this page is no longer in the heap delayed free list
  _mi_heap_delayed_free_all(heap);
  // We can still access the page meta-info even if it is freed as we ensure 
  // in `mi_segment_force_abandon` that the segment is not freed (yet)
  if (page->capacity == 0) return; // it may have been freed now

  // and now unlink it from the page queue and abandon (or free)
  mi_page_queue_t* pq = mi_heap_page_queue_of(heap, page);
  if (mi_page_all_free(page)) {
    _mi_page_free(page, pq, false);
  }
  else {
    _mi_page_abandon(page, pq);
  }
}

// Free a page with no more free blocks
void _mi_page_free(mi_page_t* page, mi_page_queue_t* pq, bool force) {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(_mi_page_is_valid(page));
  mi_assert_internal(pq == mi_page_queue_of(page));
  mi_assert_internal(mi_page_all_free(page));
  mi_assert_internal(mi_page_thread_free_flag(page)!=MI_DELAYED_FREEING);

  // no more aligned blocks in here
  mi_page_set_has_aligned(page, false);

  // remove from the page list
  // (no need to do _mi_heap_delayed_free first as all blocks are already free)
  mi_segments_tld_t* segments_tld = &mi_page_heap(page)->tld->segments;
  mi_page_queue_remove(pq, page);

  // and free it
  mi_page_set_heap(page,NULL);
  _mi_segment_page_free(page, force, segments_tld);
}

#define MI_MAX_RETIRE_SIZE    MI_LARGE_OBJ_SIZE_MAX   // should be less than size for MI_BIN_HUGE
#define MI_RETIRE_CYCLES      (16)

// Retire a page with no more used blocks
// Important to not retire too quickly though as new
// allocations might coming.
// Note: called from `mi_free` and benchmarks often
// trigger this due to freeing everything and then
// allocating again so careful when changing this.
void _mi_page_retire(mi_page_t* page) mi_attr_noexcept {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(_mi_page_is_valid(page));
  mi_assert_internal(mi_page_all_free(page));

  mi_page_set_has_aligned(page, false);

  // don't retire too often..
  // (or we end up retiring and re-allocating most of the time)
  // NOTE: refine this more: we should not retire if this
  // is the only page left with free blocks. It is not clear
  // how to check this efficiently though...
  // for now, we don't retire if it is the only page left of this size class.
  mi_page_queue_t* pq = mi_page_queue_of(page);
  #if MI_RETIRE_CYCLES > 0
  const size_t bsize = mi_page_block_size(page);
  if mi_likely( /* bsize < MI_MAX_RETIRE_SIZE && */ !mi_page_queue_is_special(pq)) {  // not full or huge queue?
    if (pq->last==page && pq->first==page) { // the only page in the queue?
      mi_stat_counter_increase(_mi_stats_main.page_no_retire,1);
      page->retire_expire = (bsize <= MI_SMALL_OBJ_SIZE_MAX ? MI_RETIRE_CYCLES : MI_RETIRE_CYCLES/4);
      mi_heap_t* heap = mi_page_heap(page);
      mi_assert_internal(pq >= heap->pages);
      const size_t index = pq - heap->pages;
      mi_assert_internal(index < MI_BIN_FULL && index < MI_BIN_HUGE);
      if (index < heap->page_retired_min) heap->page_retired_min = index;
      if (index > heap->page_retired_max) heap->page_retired_max = index;
      mi_assert_internal(mi_page_all_free(page));
      return; // don't free after all
    }
  }
  #endif // 12315
  _mi_page_free(page, pq, false);
}

// free retired pages: we don't need to look at the entire queues
// since we only retire pages that are at the head position in a queue.
void _mi_heap_collect_retired(mi_heap_t* heap, bool force) {
  size_t min = MI_BIN_FULL;
  size_t max = 0;
  for(size_t bin = heap->page_retired_min; bin <= heap->page_retired_max; bin++) {
    mi_page_queue_t* pq   = &heap->pages[bin];
    mi_page_t*       page = pq->first;
    if (page != NULL && page->retire_expire != 0) {
      if (mi_page_all_free(page)) {
        page->retire_expire--;
        if (force || page->retire_expire == 0) {
          _mi_page_free(pq->first, pq, force);
        }
        else {
          // keep retired, update min/max
          if (bin < min) min = bin;
          if (bin > max) max = bin;
        }
      }
      else {
        page->retire_expire = 0;
      }
    }
  }
  heap->page_retired_min = min;
  heap->page_retired_max = max;
}


/* -----------------------------------------------------------
  Initialize the initial free list in a page.
  In secure mode we initialize a randomized list by
  alternating between slices.
----------------------------------------------------------- */

#define MI_MAX_SLICE_SHIFT  (6)   // at most 64 slices
#define MI_MAX_SLICES       (1UL << MI_MAX_SLICE_SHIFT)
#define MI_MIN_SLICES       (2)

static void mi_page_free_list_extend_secure(mi_heap_t* const heap, mi_page_t* const page, const size_t bsize, const size_t extend, mi_stats_t* const stats) {
  MI_UNUSED(stats);
  #if (MI_SECURE<=2)
  mi_assert_internal(page->free == NULL);
  mi_assert_internal(page->local_free == NULL);
  #endif // 12377
  mi_assert_internal(page->capacity + extend <= page->reserved);
  mi_assert_internal(bsize == mi_page_block_size(page));
  void* const page_area = mi_page_start(page);

  // initialize a randomized free list
  // set up `slice_count` slices to alternate between
  size_t shift = MI_MAX_SLICE_SHIFT;
  while ((extend >> shift) == 0) {
    shift--;
  }
  const size_t slice_count = (size_t)1U << shift;
  const size_t slice_extend = extend / slice_count;
  mi_assert_internal(slice_extend >= 1);
  mi_block_t* blocks[MI_MAX_SLICES];   // current start of the slice
  size_t      counts[MI_MAX_SLICES];   // available objects in the slice
  for (size_t i = 0; i < slice_count; i++) {
    blocks[i] = mi_page_block_at(page, page_area, bsize, page->capacity + i*slice_extend);
    counts[i] = slice_extend;
  }
  counts[slice_count-1] += (extend % slice_count);  // final slice holds the modulus too (todo: distribute evenly?)

  // and initialize the free list by randomly threading through them
  // set up first element
  const uintptr_t r = _mi_heap_random_next(heap);
  size_t current = r % slice_count;
  counts[current]--;
  mi_block_t* const free_start = blocks[current];
  // and iterate through the rest; use `random_shuffle` for performance
  uintptr_t rnd = _mi_random_shuffle(r|1); // ensure not 0
  for (size_t i = 1; i < extend; i++) {
    // call random_shuffle only every INTPTR_SIZE rounds
    const size_t round = i%MI_INTPTR_SIZE;
    if (round == 0) rnd = _mi_random_shuffle(rnd);
    // select a random next slice index
    size_t next = ((rnd >> 8*round) & (slice_count-1));
    while (counts[next]==0) {                            // ensure it still has space
      next++;
      if (next==slice_count) next = 0;
    }
    // and link the current block to it
    counts[next]--;
    mi_block_t* const block = blocks[current];
    blocks[current] = (mi_block_t*)((uint8_t*)block + bsize);  // bump to the following block
    mi_block_set_next(page, block, blocks[next]);   // and set next; note: we may have `current == next`
    current = next;
  }
  // prepend to the free list (usually NULL)
  mi_block_set_next(page, blocks[current], page->free);  // end of the list
  page->free = free_start;
}

static mi_decl_noinline void mi_page_free_list_extend( mi_page_t* const page, const size_t bsize, const size_t extend, mi_stats_t* const stats)
{
  MI_UNUSED(stats);
  #if (MI_SECURE <= 2)
  mi_assert_internal(page->free == NULL);
  mi_assert_internal(page->local_free == NULL);
  #endif // 12435
  mi_assert_internal(page->capacity + extend <= page->reserved);
  mi_assert_internal(bsize == mi_page_block_size(page));
  void* const page_area = mi_page_start(page);

  mi_block_t* const start = mi_page_block_at(page, page_area, bsize, page->capacity);

  // initialize a sequential free list
  mi_block_t* const last = mi_page_block_at(page, page_area, bsize, page->capacity + extend - 1);
  mi_block_t* block = start;
  while(block <= last) {
    mi_block_t* next = (mi_block_t*)((uint8_t*)block + bsize);
    mi_block_set_next(page,block,next);
    block = next;
  }
  // prepend to free list (usually `NULL`)
  mi_block_set_next(page, last, page->free);
  page->free = start;
}

/* -----------------------------------------------------------
  Page initialize and extend the capacity
----------------------------------------------------------- */

#define MI_MAX_EXTEND_SIZE    (4*1024)      // heuristic, one OS page seems to work well.
#if (MI_SECURE>0)
#define MI_MIN_EXTEND         (8*MI_SECURE) // extend at least by this many
#else
#define MI_MIN_EXTEND         (1)
#endif // 12463

// Extend the capacity (up to reserved) by initializing a free list
// We do at most `MI_MAX_EXTEND` to avoid touching too much memory
// Note: we also experimented with "bump" allocation on the first
// allocations but this did not speed up any benchmark (due to an
// extra test in malloc? or cache effects?)
static void mi_page_extend_free(mi_heap_t* heap, mi_page_t* page, mi_tld_t* tld) {
  mi_assert_expensive(mi_page_is_valid_init(page));
  #if (MI_SECURE<=2)
  mi_assert(page->free == NULL);
  mi_assert(page->local_free == NULL);
  if (page->free != NULL) return;
  #endif // 12476
  if (page->capacity >= page->reserved) return;

  size_t page_size;
  //uint8_t* page_start =
  _mi_segment_page_start(_mi_page_segment(page), page, &page_size);
  mi_stat_counter_increase(tld->stats.pages_extended, 1);

  // calculate the extend count
  const size_t bsize = mi_page_block_size(page);
  size_t extend = page->reserved - page->capacity;
  mi_assert_internal(extend > 0);

  size_t max_extend = (bsize >= MI_MAX_EXTEND_SIZE ? MI_MIN_EXTEND : MI_MAX_EXTEND_SIZE/bsize);
  if (max_extend < MI_MIN_EXTEND) { max_extend = MI_MIN_EXTEND; }
  mi_assert_internal(max_extend > 0);

  if (extend > max_extend) {
    // ensure we don't touch memory beyond the page to reduce page commit.
    // the `lean` benchmark tests this. Going from 1 to 8 increases rss by 50%.
    extend = max_extend;
  }

  mi_assert_internal(extend > 0 && extend + page->capacity <= page->reserved);
  mi_assert_internal(extend < (1UL<<16));

  // and append the extend the free list
  if (extend < MI_MIN_SLICES || MI_SECURE==0) { //!mi_option_is_enabled(mi_option_secure)) {
    mi_page_free_list_extend(page, bsize, extend, &tld->stats );
  }
  else {
    mi_page_free_list_extend_secure(heap, page, bsize, extend, &tld->stats);
  }
  // enable the new free list
  page->capacity += (uint16_t)extend;
  mi_stat_increase(tld->stats.page_committed, extend * bsize);
  mi_assert_expensive(mi_page_is_valid_init(page));
}

// Initialize a fresh page
static void mi_page_init(mi_heap_t* heap, mi_page_t* page, size_t block_size, mi_tld_t* tld) {
  mi_assert(page != NULL);
  mi_segment_t* segment = _mi_page_segment(page);
  mi_assert(segment != NULL);
  mi_assert_internal(block_size > 0);
  // set fields
  mi_page_set_heap(page, heap);
  page->block_size = block_size;
  size_t page_size;
  page->page_start = _mi_segment_page_start(segment, page, &page_size);
  mi_track_mem_noaccess(page->page_start,page_size);
  mi_assert_internal(page_size / block_size < (1L<<16));
  page->reserved = (uint16_t)(page_size / block_size);
  mi_assert_internal(page->reserved > 0);
  #if (MI_PADDING || MI_ENCODE_FREELIST)
  page->keys[0] = _mi_heap_random_next(heap);
  page->keys[1] = _mi_heap_random_next(heap);
  #endif // 12534
  page->free_is_zero = page->is_zero_init;
  #if MI_DEBUG>2
  if (page->is_zero_init) {
    mi_track_mem_defined(page->page_start, page_size);
    mi_assert_expensive(mi_mem_is_zero(page->page_start, page_size));
  }
  #endif // 12539
  if (block_size > 0 && _mi_is_power_of_two(block_size)) {
    page->block_size_shift = (uint8_t)(mi_ctz((uintptr_t)block_size));
  }
  else {
    page->block_size_shift = 0;
  }

  mi_assert_internal(page->capacity == 0);
  mi_assert_internal(page->free == NULL);
  mi_assert_internal(page->used == 0);
  mi_assert_internal(page->xthread_free == 0);
  mi_assert_internal(page->next == NULL);
  mi_assert_internal(page->prev == NULL);
  mi_assert_internal(page->retire_expire == 0);
  mi_assert_internal(!mi_page_has_aligned(page));
  #if (MI_PADDING || MI_ENCODE_FREELIST)
  mi_assert_internal(page->keys[0] != 0);
  mi_assert_internal(page->keys[1] != 0);
  #endif // 12560
  mi_assert_internal(page->block_size_shift == 0 || (block_size == ((size_t)1 << page->block_size_shift)));
  mi_assert_expensive(mi_page_is_valid_init(page));

  // initialize an initial free list
  mi_page_extend_free(heap,page,tld);
  mi_assert(mi_page_immediate_available(page));
}


/* -----------------------------------------------------------
  Find pages with free blocks
-------------------------------------------------------------*/

// search for a best next page to use for at most N pages (often cut short if immediate blocks are available)
#define MI_MAX_CANDIDATE_SEARCH  (8)

// is the page not yet used up to its reserved space?
static bool mi_page_is_expandable(const mi_page_t* page) {
  mi_assert_internal(page != NULL);
  mi_assert_internal(page->capacity <= page->reserved);
  return (page->capacity < page->reserved);
}


// Find a page with free blocks of `page->block_size`.
static mi_page_t* mi_page_queue_find_free_ex(mi_heap_t* heap, mi_page_queue_t* pq, bool first_try)
{
  // search through the pages in "next fit" order
  #if MI_STAT
  size_t count = 0;
  #endif // 12592
  size_t candidate_count = 0;        // we reset this on the first candidate to limit the search
  mi_page_t* page_candidate = NULL;  // a page with free space
  mi_page_t* page = pq->first;

  while (page != NULL)
  {
    mi_page_t* next = page->next; // remember next
    #if MI_STAT
    count++;
    #endif // 12602
    candidate_count++;

    // collect freed blocks by us and other threads
    _mi_page_free_collect(page, false);

  #if MI_MAX_CANDIDATE_SEARCH > 1
    // search up to N pages for a best candidate

    // is the local free list non-empty?
    const bool immediate_available = mi_page_immediate_available(page);

    // if the page is completely full, move it to the `mi_pages_full`
    // queue so we don't visit long-lived pages too often.
    if (!immediate_available && !mi_page_is_expandable(page)) {
      mi_assert_internal(!mi_page_is_in_full(page) && !mi_page_immediate_available(page));
      mi_page_to_full(page, pq);
    }
    else {
      // the page has free space, make it a candidate
      // we prefer non-expandable pages with high usage as candidates (to reduce commit, and increase chances of free-ing up pages)
      if (page_candidate == NULL) {
        page_candidate = page;
        candidate_count = 0;
      }
      else if (/* !mi_page_is_expandable(page) && */ page->used >= page_candidate->used) {
        page_candidate = page;
      }
      // if we find a non-expandable candidate, or searched for N pages, return with the best candidate
      if (immediate_available || candidate_count > MI_MAX_CANDIDATE_SEARCH) {
        mi_assert_internal(page_candidate!=NULL);
        break;
      }
    }
  #else
    // first-fit algorithm
    // If the page contains free blocks, we are done
    if (mi_page_immediate_available(page) || mi_page_is_expandable(page)) {
      break;  // pick this one
    }

    // If the page is completely full, move it to the `mi_pages_full`
    // queue so we don't visit long-lived pages too often.
    mi_assert_internal(!mi_page_is_in_full(page) && !mi_page_immediate_available(page));
    mi_page_to_full(page, pq);
  #endif // 12610

    page = next;
  } // for each page

  mi_heap_stat_counter_increase(heap, searches, count);

  // set the page to the best candidate
  if (page_candidate != NULL) {
    page = page_candidate;
  }
  if (page != NULL && !mi_page_immediate_available(page)) {
    mi_assert_internal(mi_page_is_expandable(page));
    mi_page_extend_free(heap, page, heap->tld);
  }

  if (page == NULL) {
    _mi_heap_collect_retired(heap, false); // perhaps make a page available
    page = mi_page_fresh(heap, pq);
    if (page == NULL && first_try) {
      // out-of-memory _or_ an abandoned page with free blocks was reclaimed, try once again
      page = mi_page_queue_find_free_ex(heap, pq, false);
    }
  }
  else {
    // move the page to the front of the queue
    mi_page_queue_move_to_front(heap, pq, page);
    page->retire_expire = 0;
    // _mi_heap_collect_retired(heap, false); // update retire counts; note: increases rss on MemoryLoad bench so don't do this
  }
  mi_assert_internal(page == NULL || mi_page_immediate_available(page));


  return page;
}



// Find a page with free blocks of `size`.
static inline mi_page_t* mi_find_free_page(mi_heap_t* heap, size_t size) {
  mi_page_queue_t* pq = mi_page_queue(heap, size);

  // check the first page: we even do this with candidate search or otherwise we re-search every time
  mi_page_t* page = pq->first;
  if (page != NULL) {
   #if (MI_SECURE>=3) // in secure mode, we extend half the time to increase randomness
    if (page->capacity < page->reserved && ((_mi_heap_random_next(heap) & 1) == 1)) {
      mi_page_extend_free(heap, page, heap->tld);
      mi_assert_internal(mi_page_immediate_available(page));
    }
    else
   #endif // 12694
    {
      _mi_page_free_collect(page,false);
    }

    if (mi_page_immediate_available(page)) {
      page->retire_expire = 0;
      return page; // fast path
    }
  }

  return mi_page_queue_find_free_ex(heap, pq, true);
}


/* -----------------------------------------------------------
  Users can register a deferred free function called
  when the `free` list is empty. Since the `local_free`
  is separate this is deterministically called after
  a certain number of allocations.
----------------------------------------------------------- */

static mi_deferred_free_fun* volatile deferred_free = NULL;
static _Atomic(void*) deferred_arg; // = NULL

void _mi_deferred_free(mi_heap_t* heap, bool force) {
  heap->tld->heartbeat++;
  if (deferred_free != NULL && !heap->tld->recurse) {
    heap->tld->recurse = true;
    deferred_free(force, heap->tld->heartbeat, mi_atomic_load_ptr_relaxed(void,&deferred_arg));
    heap->tld->recurse = false;
  }
}

void mi_register_deferred_free(mi_deferred_free_fun* fn, void* arg) mi_attr_noexcept {
  deferred_free = fn;
  mi_atomic_store_ptr_release(void,&deferred_arg, arg);
}


/* -----------------------------------------------------------
  General allocation
----------------------------------------------------------- */

// Huge pages contain just one block, and the segment contains just that page.
// Huge pages are also use if the requested alignment is very large (> MI_BLOCK_ALIGNMENT_MAX)
// so their size is not always `> MI_LARGE_OBJ_SIZE_MAX`.
static mi_page_t* mi_huge_page_alloc(mi_heap_t* heap, size_t size, size_t page_alignment) {
  size_t block_size = _mi_os_good_alloc_size(size);
  mi_assert_internal(mi_bin(block_size) == MI_BIN_HUGE || page_alignment > 0);
  #if MI_HUGE_PAGE_ABANDON
  mi_page_queue_t* pq = NULL;
  #else
  mi_page_queue_t* pq = mi_page_queue(heap, MI_LARGE_OBJ_SIZE_MAX+1);  // always in the huge queue regardless of the block size
  mi_assert_internal(mi_page_queue_is_huge(pq));
  #endif // 12750
  mi_page_t* page = mi_page_fresh_alloc(heap, pq, block_size, page_alignment);
  if (page != NULL) {
    mi_assert_internal(mi_page_block_size(page) >= size);
    mi_assert_internal(mi_page_immediate_available(page));
    mi_assert_internal(mi_page_is_huge(page));
    mi_assert_internal(_mi_page_segment(page)->page_kind == MI_PAGE_HUGE);
    mi_assert_internal(_mi_page_segment(page)->used==1);
    #if MI_HUGE_PAGE_ABANDON
    mi_assert_internal(_mi_page_segment(page)->thread_id==0); // abandoned, not in the huge queue
    mi_page_set_heap(page, NULL);
    #endif // 12763
    mi_heap_stat_increase(heap, huge, mi_page_block_size(page));
    mi_heap_stat_counter_increase(heap, huge_count, 1);
  }
  return page;
}


// Allocate a page
// Note: in debug mode the size includes MI_PADDING_SIZE and might have overflowed.
static mi_page_t* mi_find_page(mi_heap_t* heap, size_t size, size_t huge_alignment) mi_attr_noexcept {
  // huge allocation?
  const size_t req_size = size - MI_PADDING_SIZE;  // correct for padding_size in case of an overflow on `size`
  if mi_unlikely(req_size > (MI_LARGE_OBJ_SIZE_MAX - MI_PADDING_SIZE) || huge_alignment > 0) {
    if mi_unlikely(req_size > MI_MAX_ALLOC_SIZE) {
      _mi_error_message(EOVERFLOW, "allocation request is too large (%zu bytes)\n", req_size);
      return NULL;
    }
    else {
      return mi_huge_page_alloc(heap,size,huge_alignment);
    }
  }
  else {
    // otherwise find a page with free blocks in our size segregated queues
    #if MI_PADDING
    mi_assert_internal(size >= MI_PADDING_SIZE);
    #endif // 12790
    return mi_find_free_page(heap, size);
  }
}

// Generic allocation routine if the fast path (`alloc.c:mi_page_malloc`) does not succeed.
// Note: in debug mode the size includes MI_PADDING_SIZE and might have overflowed.
// The `huge_alignment` is normally 0 but is set to a multiple of MI_SEGMENT_SIZE for
// very large requested alignments in which case we use a huge segment.
void* _mi_malloc_generic(mi_heap_t* heap, size_t size, bool zero, size_t huge_alignment) mi_attr_noexcept
{
  mi_assert_internal(heap != NULL);

  // initialize if necessary
  if mi_unlikely(!mi_heap_is_initialized(heap)) {
    heap = mi_heap_get_default(); // calls mi_thread_init
    if mi_unlikely(!mi_heap_is_initialized(heap)) { return NULL; }
  }
  mi_assert_internal(mi_heap_is_initialized(heap));

  // call potential deferred free routines
  _mi_deferred_free(heap, false);

  // free delayed frees from other threads (but skip contended ones)
  _mi_heap_delayed_free_partial(heap);

  // find (or allocate) a page of the right size
  mi_page_t* page = mi_find_page(heap, size, huge_alignment);
  if mi_unlikely(page == NULL) { // first time out of memory, try to collect and retry the allocation once more
    mi_heap_collect(heap, true /* force */);
    page = mi_find_page(heap, size, huge_alignment);
  }

  if mi_unlikely(page == NULL) { // out of memory
    const size_t req_size = size - MI_PADDING_SIZE;  // correct for padding_size in case of an overflow on `size`
    _mi_error_message(ENOMEM, "unable to allocate memory (%zu bytes)\n", req_size);
    return NULL;
  }

  mi_assert_internal(mi_page_immediate_available(page));
  mi_assert_internal(mi_page_block_size(page) >= size);

  // and try again, this time succeeding! (i.e. this should never recurse through _mi_page_malloc)
  if mi_unlikely(zero && mi_page_is_huge(page)) {
    // note: we cannot call _mi_page_malloc with zeroing for huge blocks; we zero it afterwards in that case.
    void* p = _mi_page_malloc(heap, page, size);
    mi_assert_internal(p != NULL);
    _mi_memzero_aligned(p, mi_page_usable_block_size(page));
    return p;
  }
  else {
    return _mi_page_malloc_zero(heap, page, size, zero);
  }
}
// --- Amalgamation END --- mimalloc/src/page.c


// --- Amalgamation START --- mimalloc/src/random.c
/* ----------------------------------------------------------------------------
Copyright (c) 2019-2021, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h

#include <string.h>       // memset

/* ----------------------------------------------------------------------------
We use our own PRNG to keep predictable performance of random number generation
and to avoid implementations that use a lock. We only use the OS provided
random source to initialize the initial seeds. Since we do not need ultimate
performance but we do rely on the security (for secret cookies in secure mode)
we use a cryptographically secure generator (chacha20).
-----------------------------------------------------------------------------*/

#define MI_CHACHA_ROUNDS (20)   // perhaps use 12 for better performance?


/* ----------------------------------------------------------------------------
Chacha20 implementation as the original algorithm with a 64-bit nonce
and counter: https://en.wikipedia.org/wiki/Salsa20
The input matrix has sixteen 32-bit values:
Position  0 to  3: constant key
Position  4 to 11: the key
Position 12 to 13: the counter.
Position 14 to 15: the nonce.

The implementation uses regular C code which compiles very well on modern compilers.
(gcc x64 has no register spills, and clang 6+ uses SSE instructions)
-----------------------------------------------------------------------------*/

static inline uint32_t rotl(uint32_t x, uint32_t shift) {
  return (x << shift) | (x >> (32 - shift));
}

static inline void qround(uint32_t x[16], size_t a, size_t b, size_t c, size_t d) {
  x[a] += x[b]; x[d] = rotl(x[d] ^ x[a], 16);
  x[c] += x[d]; x[b] = rotl(x[b] ^ x[c], 12);
  x[a] += x[b]; x[d] = rotl(x[d] ^ x[a], 8);
  x[c] += x[d]; x[b] = rotl(x[b] ^ x[c], 7);
}

static void chacha_block(mi_random_ctx_t* ctx)
{
  // scramble into `x`
  uint32_t x[16];
  for (size_t i = 0; i < 16; i++) {
    x[i] = ctx->input[i];
  }
  for (size_t i = 0; i < MI_CHACHA_ROUNDS; i += 2) {
    qround(x, 0, 4,  8, 12);
    qround(x, 1, 5,  9, 13);
    qround(x, 2, 6, 10, 14);
    qround(x, 3, 7, 11, 15);
    qround(x, 0, 5, 10, 15);
    qround(x, 1, 6, 11, 12);
    qround(x, 2, 7,  8, 13);
    qround(x, 3, 4,  9, 14);
  }

  // add scrambled data to the initial state
  for (size_t i = 0; i < 16; i++) {
    ctx->output[i] = x[i] + ctx->input[i];
  }
  ctx->output_available = 16;

  // increment the counter for the next round
  ctx->input[12] += 1;
  if (ctx->input[12] == 0) {
    ctx->input[13] += 1;
    if (ctx->input[13] == 0) {  // and keep increasing into the nonce
      ctx->input[14] += 1;
    }
  }
}

static uint32_t chacha_next32(mi_random_ctx_t* ctx) {
  if (ctx->output_available <= 0) {
    chacha_block(ctx);
    ctx->output_available = 16; // (assign again to suppress static analysis warning)
  }
  const uint32_t x = ctx->output[16 - ctx->output_available];
  ctx->output[16 - ctx->output_available] = 0; // reset once the data is handed out
  ctx->output_available--;
  return x;
}

static inline uint32_t read32(const uint8_t* p, size_t idx32) {
  const size_t i = 4*idx32;
  return ((uint32_t)p[i+0] | (uint32_t)p[i+1] << 8 | (uint32_t)p[i+2] << 16 | (uint32_t)p[i+3] << 24);
}

static void chacha_init(mi_random_ctx_t* ctx, const uint8_t key[32], uint64_t nonce)
{
  // since we only use chacha for randomness (and not encryption) we
  // do not _need_ to read 32-bit values as little endian but we do anyways
  // just for being compatible :-)
  memset(ctx, 0, sizeof(*ctx));
  for (size_t i = 0; i < 4; i++) {
    const uint8_t* sigma = (uint8_t*)"expand 32-byte k";
    ctx->input[i] = read32(sigma,i);
  }
  for (size_t i = 0; i < 8; i++) {
    ctx->input[i + 4] = read32(key,i);
  }
  ctx->input[12] = 0;
  ctx->input[13] = 0;
  ctx->input[14] = (uint32_t)nonce;
  ctx->input[15] = (uint32_t)(nonce >> 32);
}

static void chacha_split(mi_random_ctx_t* ctx, uint64_t nonce, mi_random_ctx_t* ctx_new) {
  memset(ctx_new, 0, sizeof(*ctx_new));
  _mi_memcpy(ctx_new->input, ctx->input, sizeof(ctx_new->input));
  ctx_new->input[12] = 0;
  ctx_new->input[13] = 0;
  ctx_new->input[14] = (uint32_t)nonce;
  ctx_new->input[15] = (uint32_t)(nonce >> 32);
  mi_assert_internal(ctx->input[14] != ctx_new->input[14] || ctx->input[15] != ctx_new->input[15]); // do not reuse nonces!
  chacha_block(ctx_new);
}


/* ----------------------------------------------------------------------------
Random interface
-----------------------------------------------------------------------------*/

#if MI_DEBUG>1
static bool mi_random_is_initialized(mi_random_ctx_t* ctx) {
  return (ctx != NULL && ctx->input[0] != 0);
}
#endif // 12990

void _mi_random_split(mi_random_ctx_t* ctx, mi_random_ctx_t* ctx_new) {
  mi_assert_internal(mi_random_is_initialized(ctx));
  mi_assert_internal(ctx != ctx_new);
  chacha_split(ctx, (uintptr_t)ctx_new /*nonce*/, ctx_new);
}

uintptr_t _mi_random_next(mi_random_ctx_t* ctx) {
  mi_assert_internal(mi_random_is_initialized(ctx));
  #if MI_INTPTR_SIZE <= 4
    return chacha_next32(ctx);
  #elif MI_INTPTR_SIZE == 8
    return (((uintptr_t)chacha_next32(ctx) << 32) | chacha_next32(ctx));
  #else
  # error "define mi_random_next for this platform"
  #endif // 13004
}


/* ----------------------------------------------------------------------------
To initialize a fresh random context.
If we cannot get good randomness, we fall back to weak randomness based on a timer and ASLR.
-----------------------------------------------------------------------------*/

uintptr_t _mi_os_random_weak(uintptr_t extra_seed) {
  uintptr_t x = (uintptr_t)&_mi_os_random_weak ^ extra_seed; // ASLR makes the address random
  x ^= _mi_prim_clock_now();  
  // and do a few randomization steps
  uintptr_t max = ((x ^ (x >> 17)) & 0x0F) + 1;
  for (uintptr_t i = 0; i < max; i++) {
    x = _mi_random_shuffle(x);
  }
  mi_assert_internal(x != 0);
  return x;
}

static void mi_random_init_ex(mi_random_ctx_t* ctx, bool use_weak) {
  uint8_t key[32];
  if (use_weak || !_mi_prim_random_buf(key, sizeof(key))) {
    // if we fail to get random data from the OS, we fall back to a
    // weak random source based on the current time
    #if !defined(__wasi__)
    if (!use_weak) { _mi_warning_message("unable to use secure randomness\n"); }
    #endif // 13036
    uintptr_t x = _mi_os_random_weak(0);
    for (size_t i = 0; i < 8; i++) {  // key is eight 32-bit words.
      x = _mi_random_shuffle(x);
      ((uint32_t*)key)[i] = (uint32_t)x;
    }
    ctx->weak = true;
  }
  else {
    ctx->weak = false;
  }
  chacha_init(ctx, key, (uintptr_t)ctx /*nonce*/ );
}

void _mi_random_init(mi_random_ctx_t* ctx) {
  mi_random_init_ex(ctx, false);
}

void _mi_random_init_weak(mi_random_ctx_t * ctx) {
  mi_random_init_ex(ctx, true);
}

void _mi_random_reinit_if_weak(mi_random_ctx_t * ctx) {
  if (ctx->weak) {
    _mi_random_init(ctx);
  }
}

/* --------------------------------------------------------
test vectors from <https://tools.ietf.org/html/rfc8439>
----------------------------------------------------------- */
/*
static bool array_equals(uint32_t* x, uint32_t* y, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (x[i] != y[i]) return false;
  }
  return true;
}
static void chacha_test(void)
{
  uint32_t x[4] = { 0x11111111, 0x01020304, 0x9b8d6f43, 0x01234567 };
  uint32_t x_out[4] = { 0xea2a92f4, 0xcb1cf8ce, 0x4581472e, 0x5881c4bb };
  qround(x, 0, 1, 2, 3);
  mi_assert_internal(array_equals(x, x_out, 4));

  uint32_t y[16] = {
       0x879531e0,  0xc5ecf37d,  0x516461b1,  0xc9a62f8a,
       0x44c20ef3,  0x3390af7f,  0xd9fc690b,  0x2a5f714c,
       0x53372767,  0xb00a5631,  0x974c541a,  0x359e9963,
       0x5c971061,  0x3d631689,  0x2098d9d6,  0x91dbd320 };
  uint32_t y_out[16] = {
       0x879531e0,  0xc5ecf37d,  0xbdb886dc,  0xc9a62f8a,
       0x44c20ef3,  0x3390af7f,  0xd9fc690b,  0xcfacafd2,
       0xe46bea80,  0xb00a5631,  0x974c541a,  0x359e9963,
       0x5c971061,  0xccc07c79,  0x2098d9d6,  0x91dbd320 };
  qround(y, 2, 7, 8, 13);
  mi_assert_internal(array_equals(y, y_out, 16));

  mi_random_ctx_t r = {
    { 0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
      0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
      0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c,
      0x00000001, 0x09000000, 0x4a000000, 0x00000000 },
    {0},
    0
  };
  uint32_t r_out[16] = {
       0xe4e7f110, 0x15593bd1, 0x1fdd0f50, 0xc47120a3,
       0xc7f4d1c7, 0x0368c033, 0x9aaa2204, 0x4e6cd4c3,
       0x466482d2, 0x09aa9f07, 0x05d7c214, 0xa2028bd9,
       0xd19c12b5, 0xb94e16de, 0xe883d0cb, 0x4e3c50a2 };
  chacha_block(&r);
  mi_assert_internal(array_equals(r.output, r_out, 16));
}
*/
// --- Amalgamation END --- mimalloc/src/random.c


// --- Amalgamation START --- mimalloc/src/segment.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2024, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


#include <string.h>  // memset
#include <stdio.h>

#define MI_PAGE_HUGE_ALIGN  (256*1024)

static uint8_t* mi_segment_raw_page_start(const mi_segment_t* segment, const mi_page_t* page, size_t* page_size);

/* --------------------------------------------------------------------------------
  Segment allocation
  We allocate pages inside bigger "segments" (4MiB on 64-bit). This is to avoid
  splitting VMA's on Linux and reduce fragmentation on other OS's.
  Each thread owns its own segments.

  Currently we have:
  - small pages (64KiB), 64 in one segment
  - medium pages (512KiB), 8 in one segment
  - large pages (4MiB), 1 in one segment
  - huge segments have 1 page in one segment that can be larger than `MI_SEGMENT_SIZE`.
    it is used for blocks `> MI_LARGE_OBJ_SIZE_MAX` or with alignment `> MI_BLOCK_ALIGNMENT_MAX`.

  The memory for a segment is usually committed on demand.
  (i.e. we are careful to not touch the memory until we actually allocate a block there)

  If a  thread ends, it "abandons" pages that still contain live blocks.
  Such segments are abondoned and these can be reclaimed by still running threads,
  (much like work-stealing).
-------------------------------------------------------------------------------- */


/* -----------------------------------------------------------
  Queue of segments containing free pages
----------------------------------------------------------- */

#if (MI_DEBUG>=3)
static bool mi_segment_queue_contains(const mi_segment_queue_t* queue, const mi_segment_t* segment) {
  mi_assert_internal(segment != NULL);
  mi_segment_t* list = queue->first;
  while (list != NULL) {
    if (list == segment) break;
    mi_assert_internal(list->next==NULL || list->next->prev == list);
    mi_assert_internal(list->prev==NULL || list->prev->next == list);
    list = list->next;
  }
  return (list == segment);
}
#endif // 13169

/*
static bool mi_segment_queue_is_empty(const mi_segment_queue_t* queue) {
  return (queue->first == NULL);
}
*/

static void mi_segment_queue_remove(mi_segment_queue_t* queue, mi_segment_t* segment) {
  mi_assert_expensive(mi_segment_queue_contains(queue, segment));
  if (segment->prev != NULL) segment->prev->next = segment->next;
  if (segment->next != NULL) segment->next->prev = segment->prev;
  if (segment == queue->first) queue->first = segment->next;
  if (segment == queue->last)  queue->last = segment->prev;
  segment->next = NULL;
  segment->prev = NULL;
}

static void mi_segment_enqueue(mi_segment_queue_t* queue, mi_segment_t* segment) {
  mi_assert_expensive(!mi_segment_queue_contains(queue, segment));
  segment->next = NULL;
  segment->prev = queue->last;
  if (queue->last != NULL) {
    mi_assert_internal(queue->last->next == NULL);
    queue->last->next = segment;
    queue->last = segment;
  }
  else {
    queue->last = queue->first = segment;
  }
}

static mi_segment_queue_t* mi_segment_free_queue_of_kind(mi_page_kind_t kind, mi_segments_tld_t* tld) {
  if (kind == MI_PAGE_SMALL) return &tld->small_free;
  else if (kind == MI_PAGE_MEDIUM) return &tld->medium_free;
  else return NULL;
}

static mi_segment_queue_t* mi_segment_free_queue(const mi_segment_t* segment, mi_segments_tld_t* tld) {
  return mi_segment_free_queue_of_kind(segment->page_kind, tld);
}

// remove from free queue if it is in one
static void mi_segment_remove_from_free_queue(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_segment_queue_t* queue = mi_segment_free_queue(segment, tld); // may be NULL
  bool in_queue = (queue!=NULL && (segment->next != NULL || segment->prev != NULL || queue->first == segment));
  if (in_queue) {
    mi_segment_queue_remove(queue, segment);
  }
}

static void mi_segment_insert_in_free_queue(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_segment_enqueue(mi_segment_free_queue(segment, tld), segment);
}


/* -----------------------------------------------------------
 Invariant checking
----------------------------------------------------------- */

#if (MI_DEBUG >= 2) || (MI_SECURE >= 2)
static size_t mi_segment_page_size(const mi_segment_t* segment) {
  if (segment->capacity > 1) {
    mi_assert_internal(segment->page_kind <= MI_PAGE_MEDIUM);
    return ((size_t)1 << segment->page_shift);
  }
  else {
    mi_assert_internal(segment->page_kind >= MI_PAGE_LARGE);
    return segment->segment_size;
  }
}
#endif // 13241

#if (MI_DEBUG>=2)
static bool mi_pages_purge_contains(const mi_page_t* page, mi_segments_tld_t* tld) {
  mi_page_t* p = tld->pages_purge.first;
  while (p != NULL) {
    if (p == page) return true;
    p = p->next;
  }
  return false;
}
#endif // 13254

#if (MI_DEBUG>=3)
static bool mi_segment_is_valid(const mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_assert_internal(segment != NULL);
  mi_assert_internal(_mi_ptr_cookie(segment) == segment->cookie);
  mi_assert_internal(segment->used <= segment->capacity);
  mi_assert_internal(segment->abandoned <= segment->used);
  mi_assert_internal(segment->page_kind <= MI_PAGE_MEDIUM || segment->capacity == 1); // one large or huge page per segment
  size_t nfree = 0;
  for (size_t i = 0; i < segment->capacity; i++) {
    const mi_page_t* const page = &segment->pages[i];
    if (!page->segment_in_use) {
      nfree++;
    }
    if (page->segment_in_use) {
      mi_assert_expensive(!mi_pages_purge_contains(page, tld));
    }
    mi_assert_internal(page->is_huge == (segment->page_kind == MI_PAGE_HUGE));
  }
  mi_assert_internal(nfree + segment->used == segment->capacity);
  // mi_assert_internal(segment->thread_id == _mi_thread_id() || (segment->thread_id==0)); // or 0
  mi_assert_internal(segment->page_kind == MI_PAGE_HUGE ||
                     (mi_segment_page_size(segment) * segment->capacity == segment->segment_size));
  return true;
}
#endif // 13265

static bool mi_page_not_in_queue(const mi_page_t* page, mi_segments_tld_t* tld) {
  mi_assert_internal(page != NULL);
  if (page->next != NULL || page->prev != NULL) {
    mi_assert_internal(mi_pages_purge_contains(page, tld));
    return false;
  }
  else {
    // both next and prev are NULL, check for singleton list
    return (tld->pages_purge.first != page && tld->pages_purge.last != page);
  }
}


/* -----------------------------------------------------------
  Guard pages
----------------------------------------------------------- */

static void mi_segment_protect_range(void* p, size_t size, bool protect) {
  if (protect) {
    _mi_os_protect(p, size);
  }
  else {
    _mi_os_unprotect(p, size);
  }
}

static void mi_segment_protect(mi_segment_t* segment, bool protect, mi_os_tld_t* tld) {
  // add/remove guard pages
  if (MI_SECURE != 0) {
    // in secure mode, we set up a protected page in between the segment info and the page data
    const size_t os_psize = _mi_os_page_size();
    mi_assert_internal((segment->segment_info_size - os_psize) >= (sizeof(mi_segment_t) + ((segment->capacity - 1) * sizeof(mi_page_t))));
    mi_assert_internal(((uintptr_t)segment + segment->segment_info_size) % os_psize == 0);
    mi_segment_protect_range((uint8_t*)segment + segment->segment_info_size - os_psize, os_psize, protect);
    #if (MI_SECURE >= 2)
    if (segment->capacity == 1)
    #endif // 13325
    {
      // and protect the last (or only) page too
      mi_assert_internal(MI_SECURE <= 1 || segment->page_kind >= MI_PAGE_LARGE);
      uint8_t* start = (uint8_t*)segment + segment->segment_size - os_psize;
      if (protect && !segment->memid.initially_committed) {
        if (protect) {
          // ensure secure page is committed
          if (_mi_os_commit(start, os_psize, NULL, tld->stats)) {  // if this fails that is ok (as it is an unaccessible page)
            mi_segment_protect_range(start, os_psize, protect);
          }
        }
      }
      else {
        mi_segment_protect_range(start, os_psize, protect);
      }
    }
    #if (MI_SECURE >= 2)
    else {
      // or protect every page
      const size_t page_size = mi_segment_page_size(segment);
      for (size_t i = 0; i < segment->capacity; i++) {
        if (segment->pages[i].is_committed) {
          mi_segment_protect_range((uint8_t*)segment + (i+1)*page_size - os_psize, os_psize, protect);
        }
      }
    }
    #endif // 13344
  }
}

/* -----------------------------------------------------------
  Page reset
----------------------------------------------------------- */

static void mi_page_purge(mi_segment_t* segment, mi_page_t* page, mi_segments_tld_t* tld) {
  // todo: should we purge the guard page as well when MI_SECURE>=2 ?
  mi_assert_internal(page->is_committed);
  mi_assert_internal(!page->segment_in_use);
  if (!segment->allow_purge) return;
  mi_assert_internal(page->used == 0);
  mi_assert_internal(page->free == NULL);
  mi_assert_expensive(!mi_pages_purge_contains(page, tld));
  size_t psize;
  void* start = mi_segment_raw_page_start(segment, page, &psize);
  const bool needs_recommit = _mi_os_purge(start, psize, tld->stats);
  if (needs_recommit) { page->is_committed = false; }
}

static bool mi_page_ensure_committed(mi_segment_t* segment, mi_page_t* page, mi_segments_tld_t* tld) {
  if (page->is_committed) return true;
  mi_assert_internal(segment->allow_decommit);
  mi_assert_expensive(!mi_pages_purge_contains(page, tld));

  size_t psize;
  uint8_t* start = mi_segment_raw_page_start(segment, page, &psize);
  bool is_zero = false;
  const size_t gsize = (MI_SECURE >= 2 ? _mi_os_page_size() : 0);
  bool ok = _mi_os_commit(start, psize + gsize, &is_zero, tld->stats);
  if (!ok) return false; // failed to commit!
  page->is_committed = true;
  page->used = 0;
  page->free = NULL;
  page->is_zero_init = is_zero;
  if (gsize > 0) {
    mi_segment_protect_range(start + psize, gsize, true);
  }
  return true;
}


/* -----------------------------------------------------------
  The free page queue
----------------------------------------------------------- */

// we re-use the `free` field for the expiration counter. Since this is a
// a pointer size field while the clock is always 64-bit we need to guard
// against overflow, we use substraction to check for expiry which works
// as long as the reset delay is under (2^30 - 1) milliseconds (~12 days)
static uint32_t mi_page_get_expire( mi_page_t* page ) {
  return (uint32_t)((uintptr_t)page->free);
}

static void mi_page_set_expire( mi_page_t* page, uint32_t expire ) {
  page->free = (mi_block_t*)((uintptr_t)expire);
}

static void mi_page_purge_set_expire(mi_page_t* page) {
  mi_assert_internal(mi_page_get_expire(page)==0);
  uint32_t expire = (uint32_t)_mi_clock_now() + mi_option_get(mi_option_purge_delay);
  mi_page_set_expire(page, expire);
}

// we re-use the `free` field for the expiration counter. Since this is a
// a pointer size field while the clock is always 64-bit we need to guard
// against overflow, we use substraction to check for expiry which work
// as long as the reset delay is under (2^30 - 1) milliseconds (~12 days)
static bool mi_page_purge_is_expired(mi_page_t* page, mi_msecs_t now) {
  int32_t expire = (int32_t)mi_page_get_expire(page);
  return (((int32_t)now - expire) >= 0);
}

static void mi_segment_schedule_purge(mi_segment_t* segment, mi_page_t* page, mi_segments_tld_t* tld) {
  mi_assert_internal(!page->segment_in_use);
  mi_assert_internal(mi_page_not_in_queue(page,tld));
  mi_assert_expensive(!mi_pages_purge_contains(page, tld));
  mi_assert_internal(_mi_page_segment(page)==segment);
  if (!segment->allow_purge) return;

  if (mi_option_get(mi_option_purge_delay) == 0) {
    // purge immediately?
    mi_page_purge(segment, page, tld);
  }
  else if (mi_option_get(mi_option_purge_delay) > 0) {   // no purging if the delay is negative
    // otherwise push on the delayed page reset queue
    mi_page_queue_t* pq = &tld->pages_purge;
    // push on top
    mi_page_purge_set_expire(page);
    page->next = pq->first;
    page->prev = NULL;
    if (pq->first == NULL) {
      mi_assert_internal(pq->last == NULL);
      pq->first = pq->last = page;
    }
    else {
      pq->first->prev = page;
      pq->first = page;
    }
  }
}

static void mi_page_purge_remove(mi_page_t* page, mi_segments_tld_t* tld) {
  if (mi_page_not_in_queue(page,tld)) return;

  mi_page_queue_t* pq = &tld->pages_purge;
  mi_assert_internal(pq!=NULL);
  mi_assert_internal(!page->segment_in_use);
  mi_assert_internal(mi_page_get_expire(page) != 0);
  mi_assert_internal(mi_pages_purge_contains(page, tld));
  if (page->prev != NULL) page->prev->next = page->next;
  if (page->next != NULL) page->next->prev = page->prev;
  if (page == pq->last)  pq->last = page->prev;
  if (page == pq->first) pq->first = page->next;
  page->next = page->prev = NULL;
  mi_page_set_expire(page,0);
}

static void mi_segment_remove_all_purges(mi_segment_t* segment, bool force_purge, mi_segments_tld_t* tld) {
  if (segment->memid.is_pinned) return; // never reset in huge OS pages
  for (size_t i = 0; i < segment->capacity; i++) {
    mi_page_t* page = &segment->pages[i];
    if (!page->segment_in_use) {
      mi_page_purge_remove(page, tld);
      if (force_purge && page->is_committed) {
        mi_page_purge(segment, page, tld);
      }
    }
    else {
      mi_assert_internal(mi_page_not_in_queue(page,tld));
    }
  }
}

static void mi_pages_try_purge(bool force, mi_segments_tld_t* tld) {
  if (mi_option_get(mi_option_purge_delay) < 0) return;  // purging is not allowed

  mi_msecs_t now = _mi_clock_now();
  mi_page_queue_t* pq = &tld->pages_purge;
  // from oldest up to the first that has not expired yet
  mi_page_t* page = pq->last;
  while (page != NULL && (force || mi_page_purge_is_expired(page,now))) {
    mi_page_t* const prev = page->prev; // save previous field
    mi_page_purge_remove(page, tld);    // remove from the list to maintain invariant for mi_page_purge
    mi_page_purge(_mi_page_segment(page), page, tld);
    page = prev;
  }
  // discard the reset pages from the queue
  pq->last = page;
  if (page != NULL){
    page->next = NULL;
  }
  else {
    pq->first = NULL;
  }
}


/* -----------------------------------------------------------
 Segment size calculations
----------------------------------------------------------- */

static size_t mi_segment_raw_page_size(const mi_segment_t* segment) {
  return (segment->page_kind == MI_PAGE_HUGE ? segment->segment_size : (size_t)1 << segment->page_shift);
}

// Raw start of the page available memory; can be used on uninitialized pages (only `segment_idx` must be set)
// The raw start is not taking aligned block allocation into consideration.
static uint8_t* mi_segment_raw_page_start(const mi_segment_t* segment, const mi_page_t* page, size_t* page_size) {
  size_t   psize = mi_segment_raw_page_size(segment);
  uint8_t* p = (uint8_t*)segment + page->segment_idx * psize;

  if (page->segment_idx == 0) {
    // the first page starts after the segment info (and possible guard page)
    p += segment->segment_info_size;
    psize -= segment->segment_info_size;
  }

#if (MI_SECURE > 1)  // every page has an os guard page
  psize -= _mi_os_page_size();
#elif (MI_SECURE==1) // the last page has an os guard page at the end
  if (page->segment_idx == segment->capacity - 1) {
    psize -= _mi_os_page_size();
  }
#endif // 13534

  if (page_size != NULL) *page_size = psize;
  mi_assert_internal(page->block_size == 0 || _mi_ptr_page(p) == page);
  mi_assert_internal(_mi_ptr_segment(p) == segment);
  return p;
}

// Start of the page available memory; can be used on uninitialized pages (only `segment_idx` must be set)
uint8_t* _mi_segment_page_start(const mi_segment_t* segment, const mi_page_t* page, size_t* page_size)
{
  size_t   psize;
  uint8_t* p = mi_segment_raw_page_start(segment, page, &psize);
  const size_t block_size = mi_page_block_size(page);
  if (/*page->segment_idx == 0 &&*/ block_size > 0 && block_size <= MI_MAX_ALIGN_GUARANTEE) {
    // for small and medium objects, ensure the page start is aligned with the block size (PR#66 by kickunderscore)
    mi_assert_internal(segment->page_kind <= MI_PAGE_MEDIUM);
    size_t adjust = block_size - ((uintptr_t)p % block_size);
    if (adjust < block_size && psize >= block_size + adjust) {
      p += adjust;
      psize -= adjust;
      mi_assert_internal((uintptr_t)p % block_size == 0);
    }
  }
  mi_assert_internal(_mi_is_aligned(p, MI_MAX_ALIGN_SIZE));
  mi_assert_internal(block_size == 0 || block_size > MI_MAX_ALIGN_GUARANTEE || _mi_is_aligned(p,block_size));

  if (page_size != NULL) *page_size = psize;
  mi_assert_internal(_mi_ptr_page(p) == page);
  mi_assert_internal(_mi_ptr_segment(p) == segment);
  return p;
}


static size_t mi_segment_calculate_sizes(size_t capacity, size_t required, size_t* pre_size, size_t* info_size)
{
  const size_t minsize = sizeof(mi_segment_t) + ((capacity - 1) * sizeof(mi_page_t)) + 16 /* padding */;
  size_t guardsize = 0;
  size_t isize     = 0;


  if (MI_SECURE == 0) {
    // normally no guard pages
    #if MI_GUARDED
    isize = _mi_align_up(minsize, _mi_os_page_size());
    #else
    isize = _mi_align_up(minsize, 16 * MI_MAX_ALIGN_SIZE);
    #endif // 13583
  }
  else {
    // in secure mode, we set up a protected page in between the segment info
    // and the page data (and one at the end of the segment)
    const size_t page_size = _mi_os_page_size();
    isize = _mi_align_up(minsize, page_size);
    guardsize = page_size;
    //required = _mi_align_up(required, isize + guardsize);
  }

  if (info_size != NULL) *info_size = isize;
  if (pre_size != NULL)  *pre_size  = isize + guardsize;
  return (required==0 ? MI_SEGMENT_SIZE : _mi_align_up( required + isize + 2*guardsize, MI_PAGE_HUGE_ALIGN) );
}


/* ----------------------------------------------------------------------------
Segment caches
We keep a small segment cache per thread to increase local
reuse and avoid setting/clearing guard pages in secure mode.
------------------------------------------------------------------------------- */

static void mi_segments_track_size(long segment_size, mi_segments_tld_t* tld) {
  if (segment_size>=0) _mi_stat_increase(&tld->stats->segments,1);
                  else _mi_stat_decrease(&tld->stats->segments,1);
  tld->count += (segment_size >= 0 ? 1 : -1);
  if (tld->count > tld->peak_count) tld->peak_count = tld->count;
  tld->current_size += segment_size;
  if (tld->current_size > tld->peak_size) tld->peak_size = tld->current_size;
}

static void mi_segment_os_free(mi_segment_t* segment, size_t segment_size, mi_segments_tld_t* tld) {
  segment->thread_id = 0;
  _mi_segment_map_freed_at(segment);
  mi_segments_track_size(-((long)segment_size),tld);
  if (segment->was_reclaimed) {
    tld->reclaim_count--;
    segment->was_reclaimed = false;
  }

  if (MI_SECURE != 0) {
    mi_assert_internal(!segment->memid.is_pinned);
    mi_segment_protect(segment, false, tld->os); // ensure no more guard pages are set
  }

  bool fully_committed = true;
  size_t committed_size = 0;
  const size_t page_size = mi_segment_raw_page_size(segment);
  for (size_t i = 0; i < segment->capacity; i++) {
    mi_page_t* page = &segment->pages[i];
    if (page->is_committed)  { committed_size += page_size;  }
    if (!page->is_committed) { fully_committed = false; }
  }
  MI_UNUSED(fully_committed);
  mi_assert_internal((fully_committed && committed_size == segment_size) || (!fully_committed && committed_size < segment_size));

  _mi_arena_free(segment, segment_size, committed_size, segment->memid, tld->stats);
}

// called from `heap_collect`.
void _mi_segments_collect(bool force, mi_segments_tld_t* tld) {
  mi_pages_try_purge(force,tld);
  #if MI_DEBUG>=2
  if (!_mi_is_main_thread()) {
    mi_assert_internal(tld->pages_purge.first == NULL);
    mi_assert_internal(tld->pages_purge.last == NULL);
  }
  #endif // 13650
}


/* -----------------------------------------------------------
   Segment allocation
----------------------------------------------------------- */

static mi_segment_t* mi_segment_os_alloc(bool eager_delayed, size_t page_alignment, mi_arena_id_t req_arena_id,
                                         size_t pre_size, size_t info_size, bool commit, size_t segment_size,
                                         mi_segments_tld_t* tld, mi_os_tld_t* tld_os)
{
  mi_memid_t memid;
  bool   allow_large = (!eager_delayed && (MI_SECURE == 0)); // only allow large OS pages once we are no longer lazy
  size_t align_offset = 0;
  size_t alignment = MI_SEGMENT_SIZE;
  if (page_alignment > 0) {
    alignment = page_alignment;
    align_offset = _mi_align_up(pre_size, MI_SEGMENT_SIZE);
    segment_size = segment_size + (align_offset - pre_size);  // adjust the segment size
  }

  mi_segment_t* segment = (mi_segment_t*)_mi_arena_alloc_aligned(segment_size, alignment, align_offset, commit, allow_large, req_arena_id, &memid, tld_os);
  if (segment == NULL) {
    return NULL;  // failed to allocate
  }

  if (!memid.initially_committed) {
    // ensure the initial info is committed
    mi_assert_internal(!memid.is_pinned);
    bool ok = _mi_os_commit(segment, pre_size, NULL, tld_os->stats);
    if (!ok) {
      // commit failed; we cannot touch the memory: free the segment directly and return `NULL`
      _mi_arena_free(segment, segment_size, 0, memid, tld_os->stats);
      return NULL;
    }
  }

  MI_UNUSED(info_size);
  segment->memid = memid;
  segment->allow_decommit = !memid.is_pinned;
  segment->allow_purge = segment->allow_decommit && (mi_option_get(mi_option_purge_delay) >= 0);
  segment->segment_size = segment_size;
  segment->subproc = tld->subproc;
  mi_segments_track_size((long)(segment_size), tld);
  _mi_segment_map_allocated_at(segment);
  return segment;
}

// Allocate a segment from the OS aligned to `MI_SEGMENT_SIZE` .
static mi_segment_t* mi_segment_alloc(size_t required, mi_page_kind_t page_kind, size_t page_shift, size_t page_alignment,
                                      mi_arena_id_t req_arena_id, mi_segments_tld_t* tld, mi_os_tld_t* os_tld)
{
  // required is only > 0 for huge page allocations
  mi_assert_internal((required > 0 && page_kind > MI_PAGE_LARGE)|| (required==0 && page_kind <= MI_PAGE_LARGE));

  // calculate needed sizes first
  size_t capacity;
  if (page_kind == MI_PAGE_HUGE) {
    mi_assert_internal(page_shift == MI_SEGMENT_SHIFT + 1 && required > 0);
    capacity = 1;
  }
  else {
    mi_assert_internal(required == 0 && page_alignment == 0);
    size_t page_size = (size_t)1 << page_shift;
    capacity = MI_SEGMENT_SIZE / page_size;
    mi_assert_internal(MI_SEGMENT_SIZE % page_size == 0);
    mi_assert_internal(capacity >= 1 && capacity <= MI_SMALL_PAGES_PER_SEGMENT);
  }
  size_t info_size;
  size_t pre_size;
  const size_t init_segment_size = mi_segment_calculate_sizes(capacity, required, &pre_size, &info_size);
  mi_assert_internal(init_segment_size >= required);

  // Initialize parameters
  const bool eager_delayed = (page_kind <= MI_PAGE_MEDIUM &&          // don't delay for large objects
                              // !_mi_os_has_overcommit() &&          // never delay on overcommit systems
                              _mi_current_thread_count() > 1 &&       // do not delay for the first N threads
                              tld->peak_count < (size_t)mi_option_get(mi_option_eager_commit_delay));
  const bool eager  = !eager_delayed && mi_option_is_enabled(mi_option_eager_commit);
  const bool init_commit = eager; // || (page_kind >= MI_PAGE_LARGE);

  // Allocate the segment from the OS (segment_size can change due to alignment)
  mi_segment_t* segment = mi_segment_os_alloc(eager_delayed, page_alignment, req_arena_id, pre_size, info_size, init_commit, init_segment_size, tld, os_tld);
  if (segment == NULL) return NULL;
  mi_assert_internal(segment != NULL && (uintptr_t)segment % MI_SEGMENT_SIZE == 0);
  mi_assert_internal(segment->memid.is_pinned ? segment->memid.initially_committed : true);

  // zero the segment info (but not the `mem` fields)
  ptrdiff_t ofs = offsetof(mi_segment_t, next);
  _mi_memzero((uint8_t*)segment + ofs, info_size - ofs);

  // initialize pages info
  const bool is_huge = (page_kind == MI_PAGE_HUGE);
  for (size_t i = 0; i < capacity; i++) {
    mi_assert_internal(i <= 255);
    segment->pages[i].segment_idx = (uint8_t)i;
    segment->pages[i].is_committed = segment->memid.initially_committed;
    segment->pages[i].is_zero_init = segment->memid.initially_zero;
    segment->pages[i].is_huge = is_huge;
  }

  // initialize
  segment->page_kind  = page_kind;
  segment->capacity   = capacity;
  segment->page_shift = page_shift;
  segment->segment_info_size = pre_size;
  segment->thread_id  = _mi_thread_id();
  segment->cookie     = _mi_ptr_cookie(segment);

  // set protection
  mi_segment_protect(segment, true, tld->os);

  // insert in free lists for small and medium pages
  if (page_kind <= MI_PAGE_MEDIUM) {
    mi_segment_insert_in_free_queue(segment, tld);
  }

  return segment;
}


static void mi_segment_free(mi_segment_t* segment, bool force, mi_segments_tld_t* tld) {
  MI_UNUSED(force);
  mi_assert(segment != NULL);
  
  // in `mi_segment_force_abandon` we set this to true to ensure the segment's memory stays valid
  if (segment->dont_free) return;

  // don't purge as we are freeing now
  mi_segment_remove_all_purges(segment, false /* don't force as we are about to free */, tld);
  mi_segment_remove_from_free_queue(segment, tld);

  mi_assert_expensive(!mi_segment_queue_contains(&tld->small_free, segment));
  mi_assert_expensive(!mi_segment_queue_contains(&tld->medium_free, segment));
  mi_assert(segment->next == NULL);
  mi_assert(segment->prev == NULL);
  _mi_stat_decrease(&tld->stats->page_committed, segment->segment_info_size);

  // return it to the OS
  mi_segment_os_free(segment, segment->segment_size, tld);
}

/* -----------------------------------------------------------
  Free page management inside a segment
----------------------------------------------------------- */


static bool mi_segment_has_free(const mi_segment_t* segment) {
  return (segment->used < segment->capacity);
}

static bool mi_segment_page_claim(mi_segment_t* segment, mi_page_t* page, mi_segments_tld_t* tld) {
  mi_assert_internal(_mi_page_segment(page) == segment);
  mi_assert_internal(!page->segment_in_use);
  mi_page_purge_remove(page, tld);

  // check commit
  if (!mi_page_ensure_committed(segment, page, tld)) return false;

  // set in-use before doing unreset to prevent delayed reset
  page->segment_in_use = true;
  segment->used++;
  mi_assert_internal(page->segment_in_use && page->is_committed && page->used==0 && !mi_pages_purge_contains(page,tld));
  mi_assert_internal(segment->used <= segment->capacity);
  if (segment->used == segment->capacity && segment->page_kind <= MI_PAGE_MEDIUM) {
    // if no more free pages, remove from the queue
    mi_assert_internal(!mi_segment_has_free(segment));
    mi_segment_remove_from_free_queue(segment, tld);
  }
  return true;
}


/* -----------------------------------------------------------
   Free
----------------------------------------------------------- */

static void mi_segment_abandon(mi_segment_t* segment, mi_segments_tld_t* tld);

// clear page data; can be called on abandoned segments
static void mi_segment_page_clear(mi_segment_t* segment, mi_page_t* page, mi_segments_tld_t* tld)
{
  mi_assert_internal(page->segment_in_use);
  mi_assert_internal(mi_page_all_free(page));
  mi_assert_internal(page->is_committed);
  mi_assert_internal(mi_page_not_in_queue(page, tld));

  size_t inuse = page->capacity * mi_page_block_size(page);
  _mi_stat_decrease(&tld->stats->page_committed, inuse);
  _mi_stat_decrease(&tld->stats->pages, 1);

  page->is_zero_init = false;
  page->segment_in_use = false;

  // zero the page data, but not the segment fields and capacity, page start, and block_size (for page size calculations)
  size_t block_size = page->block_size;
  uint8_t block_size_shift = page->block_size_shift;
  uint8_t heap_tag = page->heap_tag;
  uint8_t* page_start = page->page_start;
  uint16_t capacity = page->capacity;
  uint16_t reserved = page->reserved;
  ptrdiff_t ofs = offsetof(mi_page_t,capacity);
  _mi_memzero((uint8_t*)page + ofs, sizeof(*page) - ofs);
  page->capacity = capacity;
  page->reserved = reserved;
  page->block_size = block_size;
  page->block_size_shift = block_size_shift;
  page->heap_tag = heap_tag;
  page->page_start = page_start;
  segment->used--;

  // schedule purge
  mi_segment_schedule_purge(segment, page, tld);

  page->capacity = 0;  // after purge these can be zero'd now
  page->reserved = 0;
}

void _mi_segment_page_free(mi_page_t* page, bool force, mi_segments_tld_t* tld)
{
  mi_assert(page != NULL);
  mi_segment_t* segment = _mi_page_segment(page);
  mi_assert_expensive(mi_segment_is_valid(segment,tld));
  mi_pages_try_purge(false /*force?*/, tld);

  // mark it as free now
  mi_segment_page_clear(segment, page, tld);

  if (segment->used == 0) {
    // no more used pages; remove from the free list and free the segment
    mi_segment_free(segment, force, tld);
  }
  else {
    if (segment->used == segment->abandoned) {
      // only abandoned pages; remove from free list and abandon
      mi_segment_abandon(segment,tld);
    }
    else if (segment->used + 1 == segment->capacity) {
      mi_assert_internal(segment->page_kind <= MI_PAGE_MEDIUM); // large and huge pages are always the single page in a segment
      if (segment->page_kind <= MI_PAGE_MEDIUM) {
        // move back to segments  free list
        mi_segment_insert_in_free_queue(segment,tld);
      }
    }
  }
}


/* -----------------------------------------------------------
Abandonment

When threads terminate, they can leave segments with
live blocks (reached through other threads). Such segments
are "abandoned" and will be reclaimed by other threads to
reuse their pages and/or free them eventually. The
`thread_id` of such segments is 0.

When a block is freed in an abandoned segment, the segment
is reclaimed into that thread.

Moreover, if threads are looking for a fresh segment, they
will first consider abondoned segments -- these can be found
by scanning the arena memory
(segments outside arena memoryare only reclaimed by a free).
----------------------------------------------------------- */

/* -----------------------------------------------------------
   Abandon segment/page
----------------------------------------------------------- */

static void mi_segment_abandon(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_assert_internal(segment->used == segment->abandoned);
  mi_assert_internal(segment->used > 0);
  mi_assert_expensive(mi_segment_is_valid(segment, tld));

  // Potentially force purge. Only abandoned segments in arena memory can be
  // reclaimed without a free so if a segment is not from an arena we force purge here to be conservative.
  mi_pages_try_purge(false /*force?*/,tld);
  const bool force_purge = (segment->memid.memkind != MI_MEM_ARENA) ||  mi_option_is_enabled(mi_option_abandoned_page_purge);
  mi_segment_remove_all_purges(segment, force_purge, tld);

  // remove the segment from the free page queue if needed
  mi_segment_remove_from_free_queue(segment, tld);
  mi_assert_internal(segment->next == NULL && segment->prev == NULL);

  // all pages in the segment are abandoned; add it to the abandoned list
  _mi_stat_increase(&tld->stats->segments_abandoned, 1);
  mi_segments_track_size(-((long)segment->segment_size), tld);
  segment->abandoned_visits = 0;
  if (segment->was_reclaimed) {
    tld->reclaim_count--;
    segment->was_reclaimed = false;
  }
  _mi_arena_segment_mark_abandoned(segment);
}

void _mi_segment_page_abandon(mi_page_t* page, mi_segments_tld_t* tld) {
  mi_assert(page != NULL);
  mi_assert_internal(mi_page_thread_free_flag(page)==MI_NEVER_DELAYED_FREE);
  mi_assert_internal(mi_page_heap(page) == NULL);
  mi_segment_t* segment = _mi_page_segment(page);
  mi_assert_expensive(!mi_pages_purge_contains(page, tld));
  mi_assert_expensive(mi_segment_is_valid(segment, tld));
  segment->abandoned++;
  _mi_stat_increase(&tld->stats->pages_abandoned, 1);
  mi_assert_internal(segment->abandoned <= segment->used);
  if (segment->used == segment->abandoned) {
    // all pages are abandoned, abandon the entire segment
    mi_segment_abandon(segment, tld);
  }
}

/* -----------------------------------------------------------
  Reclaim abandoned pages
----------------------------------------------------------- */

// Possibly clear pages and check if free space is available
static bool mi_segment_check_free(mi_segment_t* segment, size_t block_size, bool* all_pages_free)
{
  mi_assert_internal(mi_atomic_load_relaxed(&segment->thread_id) == 0);
  bool has_page = false;
  size_t pages_used = 0;
  size_t pages_used_empty = 0;
  for (size_t i = 0; i < segment->capacity; i++) {
    mi_page_t* page = &segment->pages[i];
    if (page->segment_in_use) {
      pages_used++;
      // ensure used count is up to date and collect potential concurrent frees
      _mi_page_free_collect(page, false);
      if (mi_page_all_free(page)) {
        // if everything free already, page can be reused for some block size
        // note: don't clear the page yet as we can only OS reset it once it is reclaimed
        pages_used_empty++;
        has_page = true;
      }
      else if (mi_page_block_size(page) == block_size && mi_page_has_any_available(page)) {
        // a page has available free blocks of the right size
        has_page = true;
      }
    }
    else {
      // whole empty page
      has_page = true;
    }
  }
  mi_assert_internal(pages_used == segment->used && pages_used >= pages_used_empty);
  if (all_pages_free != NULL) {
    *all_pages_free = ((pages_used - pages_used_empty) == 0);
  }
  return has_page;
}


// Reclaim a segment; returns NULL if the segment was freed
// set `right_page_reclaimed` to `true` if it reclaimed a page of the right `block_size` that was not full.
static mi_segment_t* mi_segment_reclaim(mi_segment_t* segment, mi_heap_t* heap, size_t requested_block_size, bool* right_page_reclaimed, mi_segments_tld_t* tld) {
  if (right_page_reclaimed != NULL) { *right_page_reclaimed = false; }
  // can be 0 still with abandoned_next, or already a thread id for segments outside an arena that are reclaimed on a free.
  mi_assert_internal(mi_atomic_load_relaxed(&segment->thread_id) == 0 || mi_atomic_load_relaxed(&segment->thread_id) == _mi_thread_id());
  mi_assert_internal(segment->subproc == heap->tld->segments.subproc); // only reclaim within the same subprocess
  mi_atomic_store_release(&segment->thread_id, _mi_thread_id());
  segment->abandoned_visits = 0;
  segment->was_reclaimed = true;
  tld->reclaim_count++;
  mi_segments_track_size((long)segment->segment_size, tld);
  mi_assert_internal(segment->next == NULL && segment->prev == NULL);
  mi_assert_expensive(mi_segment_is_valid(segment, tld));
  _mi_stat_decrease(&tld->stats->segments_abandoned, 1);

  for (size_t i = 0; i < segment->capacity; i++) {
    mi_page_t* page = &segment->pages[i];
    if (page->segment_in_use) {
      mi_assert_internal(page->is_committed);
      mi_assert_internal(mi_page_not_in_queue(page, tld));
      mi_assert_internal(mi_page_thread_free_flag(page)==MI_NEVER_DELAYED_FREE);
      mi_assert_internal(mi_page_heap(page) == NULL);
      segment->abandoned--;
      mi_assert(page->next == NULL);
      _mi_stat_decrease(&tld->stats->pages_abandoned, 1);
      // get the target heap for this thread which has a matching heap tag (so we reclaim into a matching heap)
      mi_heap_t* target_heap = _mi_heap_by_tag(heap, page->heap_tag);  // allow custom heaps to separate objects
      if (target_heap == NULL) {
        target_heap = heap;
        _mi_error_message(EFAULT, "page with tag %u cannot be reclaimed by a heap with the same tag (using heap tag %u instead)\n", page->heap_tag, heap->tag );
      }
      // associate the heap with this page, and allow heap thread delayed free again.
      mi_page_set_heap(page, target_heap);
      _mi_page_use_delayed_free(page, MI_USE_DELAYED_FREE, true); // override never (after heap is set)
      _mi_page_free_collect(page, false); // ensure used count is up to date
      if (mi_page_all_free(page)) {
        // if everything free already, clear the page directly
        mi_segment_page_clear(segment, page, tld);  // reset is ok now
      }
      else {
        // otherwise reclaim it into the heap
        _mi_page_reclaim(target_heap, page);
        if (requested_block_size == mi_page_block_size(page) && mi_page_has_any_available(page) && heap == target_heap) {
          if (right_page_reclaimed != NULL) { *right_page_reclaimed = true; }
        }
      }
    }
    /* expired
    else if (page->is_committed) {  // not in-use, and not reset yet
      // note: do not reset as this includes pages that were not touched before
      // mi_pages_purge_add(segment, page, tld);
    }
    */
  }
  mi_assert_internal(segment->abandoned == 0);
  if (segment->used == 0) {
    mi_assert_internal(right_page_reclaimed == NULL || !(*right_page_reclaimed));
    mi_segment_free(segment, false, tld);
    return NULL;
  }
  else {
    if (segment->page_kind <= MI_PAGE_MEDIUM && mi_segment_has_free(segment)) {
      mi_segment_insert_in_free_queue(segment, tld);
    }
    return segment;
  }
}


// attempt to reclaim a particular segment (called from multi threaded free `alloc.c:mi_free_block_mt`)
bool _mi_segment_attempt_reclaim(mi_heap_t* heap, mi_segment_t* segment) {
  if (mi_atomic_load_relaxed(&segment->thread_id) != 0) return false;  // it is not abandoned
  if (segment->subproc != heap->tld->segments.subproc)  return false;  // only reclaim within the same subprocess
  if (!_mi_heap_memid_is_suitable(heap,segment->memid)) return false;  // don't reclaim between exclusive and non-exclusive arena's
  const long target = _mi_option_get_fast(mi_option_target_segments_per_thread);
  if (target > 0 && (size_t)target <= heap->tld->segments.count) return false; // don't reclaim if going above the target count

  // don't reclaim more from a `free` call than half the current segments
  // this is to prevent a pure free-ing thread to start owning too many segments
  // (but not for out-of-arena segments as that is the main way to be reclaimed for those)
  if (segment->memid.memkind == MI_MEM_ARENA && heap->tld->segments.reclaim_count * 2 > heap->tld->segments.count) {
    return false;
  }
  if (_mi_arena_segment_clear_abandoned(segment)) {  // atomically unabandon
    mi_segment_t* res = mi_segment_reclaim(segment, heap, 0, NULL, &heap->tld->segments);
    mi_assert_internal(res == segment);
    return (res != NULL);
  }
  return false;
}

void _mi_abandoned_reclaim_all(mi_heap_t* heap, mi_segments_tld_t* tld) {
  mi_segment_t* segment;
  mi_arena_field_cursor_t current;
  _mi_arena_field_cursor_init(heap, tld->subproc, true /* visit all, blocking */, &current);
  while ((segment = _mi_arena_segment_clear_abandoned_next(&current)) != NULL) {
    mi_segment_reclaim(segment, heap, 0, NULL, tld);
  }
  _mi_arena_field_cursor_done(&current);
}


static bool segment_count_is_within_target(mi_segments_tld_t* tld, size_t* ptarget) {
  const size_t target = (size_t)mi_option_get_clamp(mi_option_target_segments_per_thread, 0, 1024);
  if (ptarget != NULL) { *ptarget = target; }
  return (target == 0 || tld->count < target);
}

static long mi_segment_get_reclaim_tries(mi_segments_tld_t* tld) {
  // limit the tries to 10% (default) of the abandoned segments with at least 8 and at most 1024 tries.
  const size_t perc = (size_t)mi_option_get_clamp(mi_option_max_segment_reclaim, 0, 100);
  if (perc <= 0) return 0;
  const size_t total_count = mi_atomic_load_relaxed(&tld->subproc->abandoned_count);
  if (total_count == 0) return 0;
  const size_t relative_count = (total_count > 10000 ? (total_count / 100) * perc : (total_count * perc) / 100); // avoid overflow
  long max_tries = (long)(relative_count <= 1 ? 1 : (relative_count > 1024 ? 1024 : relative_count));
  if (max_tries < 8 && total_count > 8) { max_tries = 8;  }
  return max_tries;
}

static mi_segment_t* mi_segment_try_reclaim(mi_heap_t* heap, size_t block_size, mi_page_kind_t page_kind, bool* reclaimed, mi_segments_tld_t* tld)
{
  *reclaimed = false;
  long max_tries = mi_segment_get_reclaim_tries(tld);
  if (max_tries <= 0) return NULL;

  mi_segment_t* result = NULL;
  mi_segment_t* segment = NULL;
  mi_arena_field_cursor_t current;
  _mi_arena_field_cursor_init(heap, tld->subproc, false /* non-blocking */, &current);
  while (segment_count_is_within_target(tld,NULL) && (max_tries-- > 0) && ((segment = _mi_arena_segment_clear_abandoned_next(&current)) != NULL))
  {
    mi_assert(segment->subproc == heap->tld->segments.subproc); // cursor only visits segments in our sub-process
    segment->abandoned_visits++;
    // todo: should we respect numa affinity for abondoned reclaim? perhaps only for the first visit?
    // todo: an arena exclusive heap will potentially visit many abandoned unsuitable segments and use many tries
    // Perhaps we can skip non-suitable ones in a better way?
    bool is_suitable = _mi_heap_memid_is_suitable(heap, segment->memid);
    bool all_pages_free;
    bool has_page = mi_segment_check_free(segment,block_size,&all_pages_free); // try to free up pages (due to concurrent frees)
    if (all_pages_free) {
      // free the segment (by forced reclaim) to make it available to other threads.
      // note1: we prefer to free a segment as that might lead to reclaiming another
      // segment that is still partially used.
      // note2: we could in principle optimize this by skipping reclaim and directly
      // freeing but that would violate some invariants temporarily)
      mi_segment_reclaim(segment, heap, 0, NULL, tld);
    }
    else if (has_page && segment->page_kind == page_kind && is_suitable) {
      // found a free page of the right kind, or page of the right block_size with free space
      // we return the result of reclaim (which is usually `segment`) as it might free
      // the segment due to concurrent frees (in which case `NULL` is returned).
      result = mi_segment_reclaim(segment, heap, block_size, reclaimed, tld);
      break;
    }
    else if (segment->abandoned_visits > 3 && is_suitable) {
      // always reclaim on 3rd visit to limit the abandoned segment count.
      mi_segment_reclaim(segment, heap, 0, NULL, tld);
    }
    else {
      // otherwise, mark it back as abandoned
      // todo: reset delayed pages in the segment?
      _mi_arena_segment_mark_abandoned(segment);
    }
  }
  _mi_arena_field_cursor_done(&current);
  return result;
}


/* -----------------------------------------------------------
  Force abandon a segment that is in use by our thread
----------------------------------------------------------- */

// force abandon a segment
static void mi_segment_force_abandon(mi_segment_t* segment, mi_segments_tld_t* tld)
{
  mi_assert_internal(segment->abandoned < segment->used);
  mi_assert_internal(!segment->dont_free);
  
  // ensure the segment does not get free'd underneath us (so we can check if a page has been freed in `mi_page_force_abandon`)
  segment->dont_free = true;

  // for all pages
  for (size_t i = 0; i < segment->capacity; i++) {
    mi_page_t* page = &segment->pages[i];
    if (page->segment_in_use) {
      // abandon the page if it is still in-use (this will free the page if possible as well (but not our segment))
      mi_assert_internal(segment->used > 0);
      if (segment->used == segment->abandoned+1) {
        // the last page.. abandon and return as the segment will be abandoned after this
        // and we should no longer access it.
        segment->dont_free = false;
        _mi_page_force_abandon(page);
        return;
      }
      else {
        // abandon and continue
        _mi_page_force_abandon(page);
      }
    }
  }
  segment->dont_free = false;
  mi_assert(segment->used == segment->abandoned);
  mi_assert(segment->used == 0);
  if (segment->used == 0) {  // paranoia
    // all free now
    mi_segment_free(segment, false, tld);
  }
  else {
    // perform delayed purges
    mi_pages_try_purge(false /* force? */, tld);
  }
}


// try abandon segments.
// this should be called from `reclaim_or_alloc` so we know all segments are (about) fully in use.
static void mi_segments_try_abandon_to_target(mi_heap_t* heap, size_t target, mi_segments_tld_t* tld) {
  if (target <= 1) return;
  const size_t min_target = (target > 4 ? (target*3)/4 : target);  // 75%
  // todo: we should maintain a list of segments per thread; for now, only consider segments from the heap full pages
  for (int i = 0; i < 64 && tld->count >= min_target; i++) {
    mi_page_t* page = heap->pages[MI_BIN_FULL].first;
    while (page != NULL && mi_page_is_huge(page)) {
      page = page->next;
    }
    if (page==NULL) {
      break;
    }
    mi_segment_t* segment = _mi_page_segment(page);
    mi_segment_force_abandon(segment, tld);
    mi_assert_internal(page != heap->pages[MI_BIN_FULL].first); // as it is just abandoned
  }
}

// try abandon segments.
// this should be called from `reclaim_or_alloc` so we know all segments are (about) fully in use.
static void mi_segments_try_abandon(mi_heap_t* heap, mi_segments_tld_t* tld) {
  // we call this when we are about to add a fresh segment so we should be under our target segment count.
  size_t target = 0;
  if (segment_count_is_within_target(tld, &target)) return;
  mi_segments_try_abandon_to_target(heap, target, tld);
}

void mi_collect_reduce(size_t target_size) mi_attr_noexcept {
  mi_collect(true);
  mi_heap_t* heap = mi_heap_get_default();
  mi_segments_tld_t* tld = &heap->tld->segments;
  size_t target = target_size / MI_SEGMENT_SIZE;
  if (target == 0) {
    target = (size_t)mi_option_get_clamp(mi_option_target_segments_per_thread, 1, 1024);
  }
  mi_segments_try_abandon_to_target(heap, target, tld);
}

/* -----------------------------------------------------------
   Reclaim or allocate
----------------------------------------------------------- */

static mi_segment_t* mi_segment_reclaim_or_alloc(mi_heap_t* heap, size_t block_size, mi_page_kind_t page_kind, size_t page_shift, mi_segments_tld_t* tld, mi_os_tld_t* os_tld)
{
  mi_assert_internal(page_kind <= MI_PAGE_LARGE);
  mi_assert_internal(block_size <= MI_LARGE_OBJ_SIZE_MAX);

  // try to abandon some segments to increase reuse between threads
  mi_segments_try_abandon(heap,tld);

  // 1. try to reclaim an abandoned segment
  bool reclaimed;
  mi_segment_t* segment = mi_segment_try_reclaim(heap, block_size, page_kind, &reclaimed, tld);
  mi_assert_internal(segment == NULL || _mi_arena_memid_is_suitable(segment->memid, heap->arena_id));
  if (reclaimed) {
    // reclaimed the right page right into the heap
    mi_assert_internal(segment != NULL && segment->page_kind == page_kind && page_kind <= MI_PAGE_LARGE);
    return NULL; // pretend out-of-memory as the page will be in the page queue of the heap with available blocks
  }
  else if (segment != NULL) {
    // reclaimed a segment with empty pages (of `page_kind`) in it
    return segment;
  }
  // 2. otherwise allocate a fresh segment
  return mi_segment_alloc(0, page_kind, page_shift, 0, heap->arena_id, tld, os_tld);
}


/* -----------------------------------------------------------
   Small page allocation
----------------------------------------------------------- */

static mi_page_t* mi_segment_find_free(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_assert_internal(mi_segment_has_free(segment));
  mi_assert_expensive(mi_segment_is_valid(segment, tld));
  for (size_t i = 0; i < segment->capacity; i++) {  // TODO: use a bitmap instead of search?
    mi_page_t* page = &segment->pages[i];
    if (!page->segment_in_use) {
      bool ok = mi_segment_page_claim(segment, page, tld);
      if (ok) return page;
    }
  }
  mi_assert(false);
  return NULL;
}

// Allocate a page inside a segment. Requires that the page has free pages
static mi_page_t* mi_segment_page_alloc_in(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_assert_internal(mi_segment_has_free(segment));
  return mi_segment_find_free(segment, tld);
}

static mi_page_t* mi_segment_page_try_alloc_in_queue(mi_heap_t* heap, mi_page_kind_t kind, mi_segments_tld_t* tld) {
  // find an available segment the segment free queue
  mi_segment_queue_t* const free_queue = mi_segment_free_queue_of_kind(kind, tld);
  for (mi_segment_t* segment = free_queue->first; segment != NULL; segment = segment->next) {
    if (_mi_arena_memid_is_suitable(segment->memid, heap->arena_id) && mi_segment_has_free(segment)) {
      return mi_segment_page_alloc_in(segment, tld);
    }
  }
  return NULL;
}

static mi_page_t* mi_segment_page_alloc(mi_heap_t* heap, size_t block_size, mi_page_kind_t kind, size_t page_shift, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  mi_page_t* page = mi_segment_page_try_alloc_in_queue(heap, kind, tld);
  if (page == NULL) {
    // possibly allocate or reclaim a fresh segment
    mi_segment_t* const segment = mi_segment_reclaim_or_alloc(heap, block_size, kind, page_shift, tld, os_tld);
    if (segment == NULL) return NULL;  // return NULL if out-of-memory (or reclaimed)
    mi_assert_internal(segment->page_kind==kind);
    mi_assert_internal(segment->used < segment->capacity);
    mi_assert_internal(_mi_arena_memid_is_suitable(segment->memid, heap->arena_id));
    page = mi_segment_page_try_alloc_in_queue(heap, kind, tld);  // this should now succeed
  }
  mi_assert_internal(page != NULL);
  #if MI_DEBUG>=2 && !MI_TRACK_ENABLED // && !MI_TSAN
  // verify it is committed
  mi_segment_raw_page_start(_mi_page_segment(page), page, NULL)[0] = 0;
  #endif // 14343
  return page;
}

static mi_page_t* mi_segment_small_page_alloc(mi_heap_t* heap, size_t block_size, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  return mi_segment_page_alloc(heap, block_size, MI_PAGE_SMALL,MI_SMALL_PAGE_SHIFT,tld,os_tld);
}

static mi_page_t* mi_segment_medium_page_alloc(mi_heap_t* heap, size_t block_size, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  return mi_segment_page_alloc(heap, block_size, MI_PAGE_MEDIUM, MI_MEDIUM_PAGE_SHIFT, tld, os_tld);
}

/* -----------------------------------------------------------
   large page allocation
----------------------------------------------------------- */

static mi_page_t* mi_segment_large_page_alloc(mi_heap_t* heap, size_t block_size, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  mi_segment_t* segment = mi_segment_reclaim_or_alloc(heap,block_size,MI_PAGE_LARGE,MI_LARGE_PAGE_SHIFT,tld,os_tld);
  if (segment == NULL) return NULL;
  mi_page_t* page = mi_segment_find_free(segment, tld);
  mi_assert_internal(page != NULL);
#if MI_DEBUG>=2 && !MI_TRACK_ENABLED // && !MI_TSAN
  mi_segment_raw_page_start(segment, page, NULL)[0] = 0;
#endif // 14367
  return page;
}

static mi_page_t* mi_segment_huge_page_alloc(size_t size, size_t page_alignment, mi_arena_id_t req_arena_id, mi_segments_tld_t* tld, mi_os_tld_t* os_tld)
{
  mi_segment_t* segment = mi_segment_alloc(size, MI_PAGE_HUGE, MI_SEGMENT_SHIFT + 1, page_alignment, req_arena_id, tld, os_tld);
  if (segment == NULL) return NULL;
  mi_assert_internal(mi_segment_page_size(segment) - segment->segment_info_size - (2*(MI_SECURE == 0 ? 0 : _mi_os_page_size())) >= size);
  #if MI_HUGE_PAGE_ABANDON
  segment->thread_id = 0; // huge pages are immediately abandoned
  mi_segments_track_size(-(long)segment->segment_size, tld);
  #endif // 14378
  mi_page_t* page = mi_segment_find_free(segment, tld);
  mi_assert_internal(page != NULL);
  mi_assert_internal(page->is_huge);

  // for huge pages we initialize the block_size as we may
  // overallocate to accommodate large alignments.
  size_t psize;
  uint8_t* start = mi_segment_raw_page_start(segment, page, &psize);
  page->block_size = psize;

  // reset the part of the page that will not be used; this can be quite large (close to MI_SEGMENT_SIZE)
  if (page_alignment > 0 && segment->allow_decommit && page->is_committed) {
    uint8_t* aligned_p = (uint8_t*)_mi_align_up((uintptr_t)start, page_alignment);
    mi_assert_internal(_mi_is_aligned(aligned_p, page_alignment));
    mi_assert_internal(psize - (aligned_p - start) >= size);
    uint8_t* decommit_start = start + sizeof(mi_block_t); // for the free list
    ptrdiff_t decommit_size = aligned_p - decommit_start;
    _mi_os_reset(decommit_start, decommit_size, os_tld->stats);  // do not decommit as it may be in a region
  }

  return page;
}

#if MI_HUGE_PAGE_ABANDON
// free huge block from another thread
void _mi_segment_huge_page_free(mi_segment_t* segment, mi_page_t* page, mi_block_t* block) {
  // huge page segments are always abandoned and can be freed immediately by any thread
  mi_assert_internal(segment->page_kind==MI_PAGE_HUGE);
  mi_assert_internal(segment == _mi_page_segment(page));
  mi_assert_internal(mi_atomic_load_relaxed(&segment->thread_id)==0);

  // claim it and free
  mi_heap_t* heap = mi_heap_get_default(); // issue #221; don't use the internal get_default_heap as we need to ensure the thread is initialized.
  // paranoia: if this it the last reference, the cas should always succeed
  size_t expected_tid = 0;
  if (mi_atomic_cas_strong_acq_rel(&segment->thread_id, &expected_tid, heap->thread_id)) {
    mi_block_set_next(page, block, page->free);
    page->free = block;
    page->used--;
    page->is_zero_init = false;
    mi_assert(page->used == 0);
    mi_tld_t* tld = heap->tld;
    mi_segments_track_size((long)segment->segment_size, &tld->segments);
    _mi_segment_page_free(page, true, &tld->segments);
  }
#if (MI_DEBUG!=0)
  else {
    mi_assert_internal(false);
  }
#endif // 14427
}

#else
// reset memory of a huge block from another thread
void _mi_segment_huge_page_reset(mi_segment_t* segment, mi_page_t* page, mi_block_t* block) {
  mi_assert_internal(segment->page_kind == MI_PAGE_HUGE);
  mi_assert_internal(segment == _mi_page_segment(page));
  mi_assert_internal(page->used == 1); // this is called just before the free
  mi_assert_internal(page->free == NULL);
  if (segment->allow_decommit && page->is_committed) {
    size_t usize = mi_usable_size(block);
    if (usize > sizeof(mi_block_t)) {
      usize = usize - sizeof(mi_block_t);
      uint8_t* p = (uint8_t*)block + sizeof(mi_block_t);
      _mi_os_reset(p, usize, &_mi_stats_main);
    }
  }
}
#endif // 14405

/* -----------------------------------------------------------
   Page allocation
----------------------------------------------------------- */

mi_page_t* _mi_segment_page_alloc(mi_heap_t* heap, size_t block_size, size_t page_alignment, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  mi_page_t* page;
  if mi_unlikely(page_alignment > MI_BLOCK_ALIGNMENT_MAX) {
    mi_assert_internal(_mi_is_power_of_two(page_alignment));
    mi_assert_internal(page_alignment >= MI_SEGMENT_SIZE);
    //mi_assert_internal((MI_SEGMENT_SIZE % page_alignment) == 0);
    if (page_alignment < MI_SEGMENT_SIZE) { page_alignment = MI_SEGMENT_SIZE; }
    page = mi_segment_huge_page_alloc(block_size, page_alignment, heap->arena_id, tld, os_tld);
  }
  else if (block_size <= MI_SMALL_OBJ_SIZE_MAX) {
    page = mi_segment_small_page_alloc(heap, block_size, tld, os_tld);
  }
  else if (block_size <= MI_MEDIUM_OBJ_SIZE_MAX) {
    page = mi_segment_medium_page_alloc(heap, block_size, tld, os_tld);
  }
  else if (block_size <= MI_LARGE_OBJ_SIZE_MAX /* || mi_is_good_fit(block_size, MI_LARGE_PAGE_SIZE - sizeof(mi_segment_t)) */ ) {
    page = mi_segment_large_page_alloc(heap, block_size, tld, os_tld);
  }
  else {
    page = mi_segment_huge_page_alloc(block_size, page_alignment, heap->arena_id, tld, os_tld);
  }
  mi_assert_expensive(page == NULL || mi_segment_is_valid(_mi_page_segment(page),tld));
  mi_assert_internal(page == NULL || (mi_segment_page_size(_mi_page_segment(page)) - (MI_SECURE == 0 ? 0 : _mi_os_page_size())) >= block_size);
  // mi_segment_try_purge(tld);
  mi_assert_internal(page == NULL || mi_page_not_in_queue(page, tld));
  mi_assert_internal(page == NULL || _mi_page_segment(page)->subproc == tld->subproc);
  return page;
}


/* -----------------------------------------------------------
   Visit blocks in a segment (only used for abandoned segments)
----------------------------------------------------------- */

static bool mi_segment_visit_page(mi_page_t* page, bool visit_blocks, mi_block_visit_fun* visitor, void* arg) {
  mi_heap_area_t area;
  _mi_heap_area_init(&area, page);
  if (!visitor(NULL, &area, NULL, area.block_size, arg)) return false;
  if (visit_blocks) {
    return _mi_heap_area_visit_blocks(&area, page, visitor, arg);
  }
  else {
    return true;
  }
}

bool _mi_segment_visit_blocks(mi_segment_t* segment, int heap_tag, bool visit_blocks, mi_block_visit_fun* visitor, void* arg) {
  for (size_t i = 0; i < segment->capacity; i++) {
    mi_page_t* const page = &segment->pages[i];
    if (page->segment_in_use) {
      if (heap_tag < 0 || (int)page->heap_tag == heap_tag) {
        if (!mi_segment_visit_page(page, visit_blocks, visitor, arg)) return false;
      }
    }
  }
  return true;
}
// --- Amalgamation END --- mimalloc/src/segment.c


// --- Amalgamation START --- mimalloc/src/segment-map.c
/* ----------------------------------------------------------------------------
Copyright (c) 2019-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

/* -----------------------------------------------------------
  The following functions are to reliably find the segment or
  block that encompasses any pointer p (or NULL if it is not
  in any of our segments).
  We maintain a bitmap of all memory with 1 bit per MI_SEGMENT_SIZE (64MiB)
  set to 1 if it contains the segment meta data.
----------------------------------------------------------- */

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


// Reduce total address space to reduce .bss  (due to the `mi_segment_map`)
#if (MI_INTPTR_SIZE > 4) && MI_TRACK_ASAN
#define MI_SEGMENT_MAP_MAX_ADDRESS    (128*1024ULL*MI_GiB)  // 128 TiB  (see issue #881)
#elif (MI_INTPTR_SIZE > 4)
#define MI_SEGMENT_MAP_MAX_ADDRESS    (48*1024ULL*MI_GiB)   // 48 TiB
#else
#define MI_SEGMENT_MAP_MAX_ADDRESS    (UINT32_MAX)
#endif // 14545

#define MI_SEGMENT_MAP_PART_SIZE      (MI_INTPTR_SIZE*MI_KiB - 128)      // 128 > sizeof(mi_memid_t) ! 
#define MI_SEGMENT_MAP_PART_BITS      (8*MI_SEGMENT_MAP_PART_SIZE)
#define MI_SEGMENT_MAP_PART_ENTRIES   (MI_SEGMENT_MAP_PART_SIZE / MI_INTPTR_SIZE)
#define MI_SEGMENT_MAP_PART_BIT_SPAN  (MI_SEGMENT_ALIGN)
#define MI_SEGMENT_MAP_PART_SPAN      (MI_SEGMENT_MAP_PART_BITS * MI_SEGMENT_MAP_PART_BIT_SPAN)
#define MI_SEGMENT_MAP_MAX_PARTS      ((MI_SEGMENT_MAP_MAX_ADDRESS / MI_SEGMENT_MAP_PART_SPAN) + 1)

// A part of the segment map.
typedef struct mi_segmap_part_s {
  mi_memid_t memid;
  _Atomic(uintptr_t) map[MI_SEGMENT_MAP_PART_ENTRIES];
} mi_segmap_part_t;

// Allocate parts on-demand to reduce .bss footprint
static _Atomic(mi_segmap_part_t*) mi_segment_map[MI_SEGMENT_MAP_MAX_PARTS]; // = { NULL, .. }

static mi_segmap_part_t* mi_segment_map_index_of(const mi_segment_t* segment, bool create_on_demand, size_t* idx, size_t* bitidx) {
  // note: segment can be invalid or NULL.
  mi_assert_internal(_mi_ptr_segment(segment + 1) == segment); // is it aligned on MI_SEGMENT_SIZE?
  *idx = 0;
  *bitidx = 0;  
  if ((uintptr_t)segment >= MI_SEGMENT_MAP_MAX_ADDRESS) return NULL;
  const uintptr_t segindex = ((uintptr_t)segment) / MI_SEGMENT_MAP_PART_SPAN;
  if (segindex >= MI_SEGMENT_MAP_MAX_PARTS) return NULL;
  mi_segmap_part_t* part = mi_atomic_load_ptr_relaxed(mi_segmap_part_t, &mi_segment_map[segindex]);

  // allocate on demand to reduce .bss footprint
  if (part == NULL) {
    if (!create_on_demand) return NULL;
    mi_memid_t memid;
    part = (mi_segmap_part_t*)_mi_os_alloc(sizeof(mi_segmap_part_t), &memid, NULL);
    if (part == NULL) return NULL;
    mi_segmap_part_t* expected = NULL;
    if (!mi_atomic_cas_ptr_strong_release(mi_segmap_part_t, &mi_segment_map[segindex], &expected, part)) {
      _mi_os_free(part, sizeof(mi_segmap_part_t), memid, NULL);
      part = expected;
      if (part == NULL) return NULL;
    }
  }
  mi_assert(part != NULL);
  const uintptr_t offset = ((uintptr_t)segment) % MI_SEGMENT_MAP_PART_SPAN;
  const uintptr_t bitofs = offset / MI_SEGMENT_MAP_PART_BIT_SPAN;
  *idx = bitofs / MI_INTPTR_BITS;
  *bitidx = bitofs % MI_INTPTR_BITS;
  return part;
}

void _mi_segment_map_allocated_at(const mi_segment_t* segment) {
  if (segment->memid.memkind == MI_MEM_ARENA) return; // we lookup segments first in the arena's and don't need the segment map
  size_t index;
  size_t bitidx;
  mi_segmap_part_t* part = mi_segment_map_index_of(segment, true /* alloc map if needed */, &index, &bitidx);
  if (part == NULL) return; // outside our address range..
  uintptr_t mask = mi_atomic_load_relaxed(&part->map[index]);
  uintptr_t newmask;
  do {
    newmask = (mask | ((uintptr_t)1 << bitidx));
  } while (!mi_atomic_cas_weak_release(&part->map[index], &mask, newmask));
}

void _mi_segment_map_freed_at(const mi_segment_t* segment) {
  if (segment->memid.memkind == MI_MEM_ARENA) return;
  size_t index;
  size_t bitidx;
  mi_segmap_part_t* part = mi_segment_map_index_of(segment, false /* don't alloc if not present */, &index, &bitidx);
  if (part == NULL) return; // outside our address range..
  uintptr_t mask = mi_atomic_load_relaxed(&part->map[index]);
  uintptr_t newmask;
  do {
    newmask = (mask & ~((uintptr_t)1 << bitidx));
  } while (!mi_atomic_cas_weak_release(&part->map[index], &mask, newmask));
}

// Determine the segment belonging to a pointer or NULL if it is not in a valid segment.
static mi_segment_t* _mi_segment_of(const void* p) {
  if (p == NULL) return NULL;
  mi_segment_t* segment = _mi_ptr_segment(p);  // segment can be NULL  
  size_t index;
  size_t bitidx;
  mi_segmap_part_t* part = mi_segment_map_index_of(segment, false /* dont alloc if not present */, &index, &bitidx);
  if (part == NULL) return NULL;  
  const uintptr_t mask = mi_atomic_load_relaxed(&part->map[index]);
  if mi_likely((mask & ((uintptr_t)1 << bitidx)) != 0) {
    bool cookie_ok = (_mi_ptr_cookie(segment) == segment->cookie);
    mi_assert_internal(cookie_ok); MI_UNUSED(cookie_ok);
    return segment; // yes, allocated by us
  }
  return NULL;
}

// Is this a valid pointer in our heap?
static bool mi_is_valid_pointer(const void* p) {
  // first check if it is in an arena, then check if it is OS allocated
  return (_mi_arena_contains(p) || _mi_segment_of(p) != NULL);
}

mi_decl_nodiscard mi_decl_export bool mi_is_in_heap_region(const void* p) mi_attr_noexcept {
  return mi_is_valid_pointer(p);
}
// --- Amalgamation END --- mimalloc/src/segment-map.c


// --- Amalgamation START --- mimalloc/src/stats.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2021, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


#include <string.h> // memset

#if defined(_MSC_VER) && (_MSC_VER < 1920)
#pragma warning(disable:4204)  // non-constant aggregate initializer
#endif // 14681

/* -----------------------------------------------------------
  Statistics operations
----------------------------------------------------------- */

static bool mi_is_in_main(void* stat) {
  return ((uint8_t*)stat >= (uint8_t*)&_mi_stats_main
         && (uint8_t*)stat < ((uint8_t*)&_mi_stats_main + sizeof(mi_stats_t)));
}

static void mi_stat_update(mi_stat_count_t* stat, int64_t amount) {
  if (amount == 0) return;
  if (mi_is_in_main(stat))
  {
    // add atomically (for abandoned pages)
    int64_t current = mi_atomic_addi64_relaxed(&stat->current, amount);
    mi_atomic_maxi64_relaxed(&stat->peak, current + amount);
    if (amount > 0) {
      mi_atomic_addi64_relaxed(&stat->allocated,amount);
    }
    else {
      mi_atomic_addi64_relaxed(&stat->freed, -amount);
    }
  }
  else {
    // add thread local
    stat->current += amount;
    if (stat->current > stat->peak) stat->peak = stat->current;
    if (amount > 0) {
      stat->allocated += amount;
    }
    else {
      stat->freed += -amount;
    }
  }
}

void _mi_stat_counter_increase(mi_stat_counter_t* stat, size_t amount) {
  if (mi_is_in_main(stat)) {
    mi_atomic_addi64_relaxed( &stat->count, 1 );
    mi_atomic_addi64_relaxed( &stat->total, (int64_t)amount );
  }
  else {
    stat->count++;
    stat->total += amount;
  }
}

void _mi_stat_increase(mi_stat_count_t* stat, size_t amount) {
  mi_stat_update(stat, (int64_t)amount);
}

void _mi_stat_decrease(mi_stat_count_t* stat, size_t amount) {
  mi_stat_update(stat, -((int64_t)amount));
}

// must be thread safe as it is called from stats_merge
static void mi_stat_add(mi_stat_count_t* stat, const mi_stat_count_t* src, int64_t unit) {
  if (stat==src) return;
  if (src->allocated==0 && src->freed==0) return;
  mi_atomic_addi64_relaxed( &stat->allocated, src->allocated * unit);
  mi_atomic_addi64_relaxed( &stat->current, src->current * unit);
  mi_atomic_addi64_relaxed( &stat->freed, src->freed * unit);
  // peak scores do not work across threads..
  mi_atomic_addi64_relaxed( &stat->peak, src->peak * unit);
}

static void mi_stat_counter_add(mi_stat_counter_t* stat, const mi_stat_counter_t* src, int64_t unit) {
  if (stat==src) return;
  mi_atomic_addi64_relaxed( &stat->total, src->total * unit);
  mi_atomic_addi64_relaxed( &stat->count, src->count * unit);
}

// must be thread safe as it is called from stats_merge
static void mi_stats_add(mi_stats_t* stats, const mi_stats_t* src) {
  if (stats==src) return;
  mi_stat_add(&stats->segments, &src->segments,1);
  mi_stat_add(&stats->pages, &src->pages,1);
  mi_stat_add(&stats->reserved, &src->reserved, 1);
  mi_stat_add(&stats->committed, &src->committed, 1);
  mi_stat_add(&stats->reset, &src->reset, 1);
  mi_stat_add(&stats->purged, &src->purged, 1);
  mi_stat_add(&stats->page_committed, &src->page_committed, 1);

  mi_stat_add(&stats->pages_abandoned, &src->pages_abandoned, 1);
  mi_stat_add(&stats->segments_abandoned, &src->segments_abandoned, 1);
  mi_stat_add(&stats->threads, &src->threads, 1);

  mi_stat_add(&stats->malloc, &src->malloc, 1);
  mi_stat_add(&stats->segments_cache, &src->segments_cache, 1);
  mi_stat_add(&stats->normal, &src->normal, 1);
  mi_stat_add(&stats->huge, &src->huge, 1);
  mi_stat_add(&stats->giant, &src->giant, 1);

  mi_stat_counter_add(&stats->pages_extended, &src->pages_extended, 1);
  mi_stat_counter_add(&stats->mmap_calls, &src->mmap_calls, 1);
  mi_stat_counter_add(&stats->commit_calls, &src->commit_calls, 1);
  mi_stat_counter_add(&stats->reset_calls, &src->reset_calls, 1);
  mi_stat_counter_add(&stats->purge_calls, &src->purge_calls, 1);

  mi_stat_counter_add(&stats->page_no_retire, &src->page_no_retire, 1);
  mi_stat_counter_add(&stats->searches, &src->searches, 1);
  mi_stat_counter_add(&stats->normal_count, &src->normal_count, 1);
  mi_stat_counter_add(&stats->huge_count, &src->huge_count, 1);  
  mi_stat_counter_add(&stats->guarded_alloc_count, &src->guarded_alloc_count, 1);
#if MI_STAT>1
  for (size_t i = 0; i <= MI_BIN_HUGE; i++) {
    if (src->normal_bins[i].allocated > 0 || src->normal_bins[i].freed > 0) {
      mi_stat_add(&stats->normal_bins[i], &src->normal_bins[i], 1);
    }
  }
#endif // 14789
}

/* -----------------------------------------------------------
  Display statistics
----------------------------------------------------------- */

// unit > 0 : size in binary bytes
// unit == 0: count as decimal
// unit < 0 : count in binary
static void mi_printf_amount(int64_t n, int64_t unit, mi_output_fun* out, void* arg, const char* fmt) {
  char buf[32]; buf[0] = 0;
  int  len = 32;
  const char* suffix = (unit <= 0 ? " " : "B");
  const int64_t base = (unit == 0 ? 1000 : 1024);
  if (unit>0) n *= unit;

  const int64_t pos = (n < 0 ? -n : n);
  if (pos < base) {
    if (n!=1 || suffix[0] != 'B') {  // skip printing 1 B for the unit column
      _mi_snprintf(buf, len, "%lld   %-3s", (long long)n, (n==0 ? "" : suffix));
    }
  }
  else {
    int64_t divider = base;
    const char* magnitude = "K";
    if (pos >= divider*base) { divider *= base; magnitude = "M"; }
    if (pos >= divider*base) { divider *= base; magnitude = "G"; }
    const int64_t tens = (n / (divider/10));
    const long whole = (long)(tens/10);
    const long frac1 = (long)(tens%10);
    char unitdesc[8];
    _mi_snprintf(unitdesc, 8, "%s%s%s", magnitude, (base==1024 ? "i" : ""), suffix);
    _mi_snprintf(buf, len, "%ld.%ld %-3s", whole, (frac1 < 0 ? -frac1 : frac1), unitdesc);
  }
  _mi_fprintf(out, arg, (fmt==NULL ? "%12s" : fmt), buf);
}


static void mi_print_amount(int64_t n, int64_t unit, mi_output_fun* out, void* arg) {
  mi_printf_amount(n,unit,out,arg,NULL);
}

static void mi_print_count(int64_t n, int64_t unit, mi_output_fun* out, void* arg) {
  if (unit==1) _mi_fprintf(out, arg, "%12s"," ");
          else mi_print_amount(n,0,out,arg);
}

static void mi_stat_print_ex(const mi_stat_count_t* stat, const char* msg, int64_t unit, mi_output_fun* out, void* arg, const char* notok ) {
  _mi_fprintf(out, arg,"%10s:", msg);
  if (unit != 0) {
    if (unit > 0) {
      mi_print_amount(stat->peak, unit, out, arg);
      mi_print_amount(stat->allocated, unit, out, arg);
      mi_print_amount(stat->freed, unit, out, arg);
      mi_print_amount(stat->current, unit, out, arg);
      mi_print_amount(unit, 1, out, arg);
      mi_print_count(stat->allocated, unit, out, arg);
    }
    else {
      mi_print_amount(stat->peak, -1, out, arg);
      mi_print_amount(stat->allocated, -1, out, arg);
      mi_print_amount(stat->freed, -1, out, arg);
      mi_print_amount(stat->current, -1, out, arg);
      if (unit == -1) {
        _mi_fprintf(out, arg, "%24s", "");
      }
      else {
        mi_print_amount(-unit, 1, out, arg);
        mi_print_count((stat->allocated / -unit), 0, out, arg);
      }
    }
    if (stat->allocated > stat->freed) {
      _mi_fprintf(out, arg, "  ");
      _mi_fprintf(out, arg, (notok == NULL ? "not all freed" : notok));
      _mi_fprintf(out, arg, "\n");
    }
    else {
      _mi_fprintf(out, arg, "  ok\n");
    }
  }
  else {
    mi_print_amount(stat->peak, 1, out, arg);
    mi_print_amount(stat->allocated, 1, out, arg);
    _mi_fprintf(out, arg, "%11s", " ");  // no freed
    mi_print_amount(stat->current, 1, out, arg);
    _mi_fprintf(out, arg, "\n");
  }
}

static void mi_stat_print(const mi_stat_count_t* stat, const char* msg, int64_t unit, mi_output_fun* out, void* arg) {
  mi_stat_print_ex(stat, msg, unit, out, arg, NULL);
}

static void mi_stat_peak_print(const mi_stat_count_t* stat, const char* msg, int64_t unit, mi_output_fun* out, void* arg) {
  _mi_fprintf(out, arg, "%10s:", msg);
  mi_print_amount(stat->peak, unit, out, arg);
  _mi_fprintf(out, arg, "\n");
}

static void mi_stat_counter_print(const mi_stat_counter_t* stat, const char* msg, mi_output_fun* out, void* arg ) {
  _mi_fprintf(out, arg, "%10s:", msg);
  mi_print_amount(stat->total, -1, out, arg);
  _mi_fprintf(out, arg, "\n");
}


static void mi_stat_counter_print_avg(const mi_stat_counter_t* stat, const char* msg, mi_output_fun* out, void* arg) {
  const int64_t avg_tens = (stat->count == 0 ? 0 : (stat->total*10 / stat->count));
  const long avg_whole = (long)(avg_tens/10);
  const long avg_frac1 = (long)(avg_tens%10);
  _mi_fprintf(out, arg, "%10s: %5ld.%ld avg\n", msg, avg_whole, avg_frac1);
}


static void mi_print_header(mi_output_fun* out, void* arg ) {
  _mi_fprintf(out, arg, "%10s: %11s %11s %11s %11s %11s %11s\n", "heap stats", "peak   ", "total   ", "freed   ", "current   ", "unit   ", "count   ");
}

#if MI_STAT>1
static void mi_stats_print_bins(const mi_stat_count_t* bins, size_t max, const char* fmt, mi_output_fun* out, void* arg) {
  bool found = false;
  char buf[64];
  for (size_t i = 0; i <= max; i++) {
    if (bins[i].allocated > 0) {
      found = true;
      int64_t unit = _mi_bin_size((uint8_t)i);
      _mi_snprintf(buf, 64, "%s %3lu", fmt, (long)i);
      mi_stat_print(&bins[i], buf, unit, out, arg);
    }
  }
  if (found) {
    _mi_fprintf(out, arg, "\n");
    mi_print_header(out, arg);
  }
}
#endif // 14914



//------------------------------------------------------------
// Use an output wrapper for line-buffered output
// (which is nice when using loggers etc.)
//------------------------------------------------------------
typedef struct buffered_s {
  mi_output_fun* out;   // original output function
  void*          arg;   // and state
  char*          buf;   // local buffer of at least size `count+1`
  size_t         used;  // currently used chars `used <= count`
  size_t         count; // total chars available for output
} buffered_t;

static void mi_buffered_flush(buffered_t* buf) {
  buf->buf[buf->used] = 0;
  _mi_fputs(buf->out, buf->arg, NULL, buf->buf);
  buf->used = 0;
}

static void mi_cdecl mi_buffered_out(const char* msg, void* arg) {
  buffered_t* buf = (buffered_t*)arg;
  if (msg==NULL || buf==NULL) return;
  for (const char* src = msg; *src != 0; src++) {
    char c = *src;
    if (buf->used >= buf->count) mi_buffered_flush(buf);
    mi_assert_internal(buf->used < buf->count);
    buf->buf[buf->used++] = c;
    if (c == '\n') mi_buffered_flush(buf);
  }
}

//------------------------------------------------------------
// Print statistics
//------------------------------------------------------------

static void _mi_stats_print(mi_stats_t* stats, mi_output_fun* out0, void* arg0) mi_attr_noexcept {
  // wrap the output function to be line buffered
  char buf[256];
  buffered_t buffer = { out0, arg0, NULL, 0, 255 };
  buffer.buf = buf;
  mi_output_fun* out = &mi_buffered_out;
  void* arg = &buffer;

  // and print using that
  mi_print_header(out,arg);
  #if MI_STAT>1
  mi_stats_print_bins(stats->normal_bins, MI_BIN_HUGE, "normal",out,arg);
  #endif // 14979
  #if MI_STAT
  mi_stat_print(&stats->normal, "normal", (stats->normal_count.count == 0 ? 1 : -(stats->normal.allocated / stats->normal_count.count)), out, arg);
  mi_stat_print(&stats->huge, "huge", (stats->huge_count.count == 0 ? 1 : -(stats->huge.allocated / stats->huge_count.count)), out, arg);  
  mi_stat_count_t total = { 0,0,0,0 };
  mi_stat_add(&total, &stats->normal, 1);
  mi_stat_add(&total, &stats->huge, 1);
  mi_stat_print(&total, "total", 1, out, arg);
  #endif // 14982
  #if MI_STAT>1
  mi_stat_print(&stats->malloc, "malloc req", 1, out, arg);
  _mi_fprintf(out, arg, "\n");
  #endif // 14990
  mi_stat_print_ex(&stats->reserved, "reserved", 1, out, arg, "");
  mi_stat_print_ex(&stats->committed, "committed", 1, out, arg, "");
  mi_stat_peak_print(&stats->reset, "reset", 1, out, arg );
  mi_stat_peak_print(&stats->purged, "purged", 1, out, arg );
  mi_stat_print(&stats->page_committed, "touched", 1, out, arg);
  mi_stat_print(&stats->segments, "segments", -1, out, arg);
  mi_stat_print(&stats->segments_abandoned, "-abandoned", -1, out, arg);
  mi_stat_print(&stats->segments_cache, "-cached", -1, out, arg);
  mi_stat_print(&stats->pages, "pages", -1, out, arg);
  mi_stat_print(&stats->pages_abandoned, "-abandoned", -1, out, arg);
  mi_stat_counter_print(&stats->pages_extended, "-extended", out, arg);
  mi_stat_counter_print(&stats->page_no_retire, "-noretire", out, arg);
  mi_stat_counter_print(&stats->arena_count, "arenas", out, arg);
  mi_stat_counter_print(&stats->arena_crossover_count, "-crossover", out, arg);
  mi_stat_counter_print(&stats->arena_rollback_count, "-rollback", out, arg);
  mi_stat_counter_print(&stats->mmap_calls, "mmaps", out, arg);
  mi_stat_counter_print(&stats->commit_calls, "commits", out, arg);
  mi_stat_counter_print(&stats->reset_calls, "resets", out, arg);
  mi_stat_counter_print(&stats->purge_calls, "purges", out, arg);
  mi_stat_counter_print(&stats->guarded_alloc_count, "guarded", out, arg);
  mi_stat_print(&stats->threads, "threads", -1, out, arg);
  mi_stat_counter_print_avg(&stats->searches, "searches", out, arg);
  _mi_fprintf(out, arg, "%10s: %5zu\n", "numa nodes", _mi_os_numa_node_count());

  size_t elapsed;
  size_t user_time;
  size_t sys_time;
  size_t current_rss;
  size_t peak_rss;
  size_t current_commit;
  size_t peak_commit;
  size_t page_faults;
  mi_process_info(&elapsed, &user_time, &sys_time, &current_rss, &peak_rss, &current_commit, &peak_commit, &page_faults);
  _mi_fprintf(out, arg, "%10s: %5ld.%03ld s\n", "elapsed", elapsed/1000, elapsed%1000);
  _mi_fprintf(out, arg, "%10s: user: %ld.%03ld s, system: %ld.%03ld s, faults: %lu, rss: ", "process",
              user_time/1000, user_time%1000, sys_time/1000, sys_time%1000, (unsigned long)page_faults );
  mi_printf_amount((int64_t)peak_rss, 1, out, arg, "%s");
  if (peak_commit > 0) {
    _mi_fprintf(out, arg, ", commit: ");
    mi_printf_amount((int64_t)peak_commit, 1, out, arg, "%s");
  }
  _mi_fprintf(out, arg, "\n");
}

static mi_msecs_t mi_process_start; // = 0

static mi_stats_t* mi_stats_get_default(void) {
  mi_heap_t* heap = mi_heap_get_default();
  return &heap->tld->stats;
}

static void mi_stats_merge_from(mi_stats_t* stats) {
  if (stats != &_mi_stats_main) {
    mi_stats_add(&_mi_stats_main, stats);
    memset(stats, 0, sizeof(mi_stats_t));
  }
}

void mi_stats_reset(void) mi_attr_noexcept {
  mi_stats_t* stats = mi_stats_get_default();
  if (stats != &_mi_stats_main) { memset(stats, 0, sizeof(mi_stats_t)); }
  memset(&_mi_stats_main, 0, sizeof(mi_stats_t));
  if (mi_process_start == 0) { mi_process_start = _mi_clock_start(); };
}

void mi_stats_merge(void) mi_attr_noexcept {
  mi_stats_merge_from( mi_stats_get_default() );
}

void _mi_stats_done(mi_stats_t* stats) {  // called from `mi_thread_done`
  mi_stats_merge_from(stats);
}

void mi_stats_print_out(mi_output_fun* out, void* arg) mi_attr_noexcept {
  mi_stats_merge_from(mi_stats_get_default());
  _mi_stats_print(&_mi_stats_main, out, arg);
}

void mi_stats_print(void* out) mi_attr_noexcept {
  // for compatibility there is an `out` parameter (which can be `stdout` or `stderr`)
  mi_stats_print_out((mi_output_fun*)out, NULL);
}

void mi_thread_stats_print_out(mi_output_fun* out, void* arg) mi_attr_noexcept {
  _mi_stats_print(mi_stats_get_default(), out, arg);
}


// ----------------------------------------------------------------
// Basic timer for convenience; use milli-seconds to avoid doubles
// ----------------------------------------------------------------

static mi_msecs_t mi_clock_diff;

mi_msecs_t _mi_clock_now(void) {
  return _mi_prim_clock_now();
}

mi_msecs_t _mi_clock_start(void) {
  if (mi_clock_diff == 0.0) {
    mi_msecs_t t0 = _mi_clock_now();
    mi_clock_diff = _mi_clock_now() - t0;
  }
  return _mi_clock_now();
}

mi_msecs_t _mi_clock_end(mi_msecs_t start) {
  mi_msecs_t end = _mi_clock_now();
  return (end - start - mi_clock_diff);
}


// --------------------------------------------------------
// Basic process statistics
// --------------------------------------------------------

mi_decl_export void mi_process_info(size_t* elapsed_msecs, size_t* user_msecs, size_t* system_msecs, size_t* current_rss, size_t* peak_rss, size_t* current_commit, size_t* peak_commit, size_t* page_faults) mi_attr_noexcept
{
  mi_process_info_t pinfo;
  _mi_memzero_var(pinfo);
  pinfo.elapsed        = _mi_clock_end(mi_process_start);
  pinfo.current_commit = (size_t)(mi_atomic_loadi64_relaxed((_Atomic(int64_t)*)&_mi_stats_main.committed.current));
  pinfo.peak_commit    = (size_t)(mi_atomic_loadi64_relaxed((_Atomic(int64_t)*)&_mi_stats_main.committed.peak));
  pinfo.current_rss    = pinfo.current_commit;
  pinfo.peak_rss       = pinfo.peak_commit;
  pinfo.utime          = 0;
  pinfo.stime          = 0;
  pinfo.page_faults    = 0;

  _mi_prim_process_info(&pinfo);
  
  if (elapsed_msecs!=NULL)  *elapsed_msecs  = (pinfo.elapsed < 0 ? 0 : (pinfo.elapsed < (mi_msecs_t)PTRDIFF_MAX ? (size_t)pinfo.elapsed : PTRDIFF_MAX));
  if (user_msecs!=NULL)     *user_msecs     = (pinfo.utime < 0 ? 0 : (pinfo.utime < (mi_msecs_t)PTRDIFF_MAX ? (size_t)pinfo.utime : PTRDIFF_MAX));
  if (system_msecs!=NULL)   *system_msecs   = (pinfo.stime < 0 ? 0 : (pinfo.stime < (mi_msecs_t)PTRDIFF_MAX ? (size_t)pinfo.stime : PTRDIFF_MAX));
  if (current_rss!=NULL)    *current_rss    = pinfo.current_rss;
  if (peak_rss!=NULL)       *peak_rss       = pinfo.peak_rss;
  if (current_commit!=NULL) *current_commit = pinfo.current_commit;
  if (peak_commit!=NULL)    *peak_commit    = pinfo.peak_commit;
  if (page_faults!=NULL)    *page_faults    = pinfo.page_faults;
}
// --- Amalgamation END --- mimalloc/src/stats.c


// --- Amalgamation START --- mimalloc/src/prim/prim.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// Select the implementation of the primitives
// depending on the OS.

#if defined(_WIN32)

// --- Amalgamation START --- mimalloc/src/prim/windows/prim.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// This file is included in `src/prim/prim.c`


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h

#include <stdio.h>   // fputs, stderr


//---------------------------------------------
// Dynamically bind Windows API points for portability
//---------------------------------------------

// We use VirtualAlloc2 for aligned allocation, but it is only supported on Windows 10 and Windows Server 2016.
// So, we need to look it up dynamically to run on older systems. (use __stdcall for 32-bit compatibility)
// NtAllocateVirtualAllocEx is used for huge OS page allocation (1GiB)
// We define a minimal MEM_EXTENDED_PARAMETER ourselves in order to be able to compile with older SDK's.
typedef enum MI_MEM_EXTENDED_PARAMETER_TYPE_E {
  MiMemExtendedParameterInvalidType = 0,
  MiMemExtendedParameterAddressRequirements,
  MiMemExtendedParameterNumaNode,
  MiMemExtendedParameterPartitionHandle,
  MiMemExtendedParameterUserPhysicalHandle,
  MiMemExtendedParameterAttributeFlags,
  MiMemExtendedParameterMax
} MI_MEM_EXTENDED_PARAMETER_TYPE;

typedef struct DECLSPEC_ALIGN(8) MI_MEM_EXTENDED_PARAMETER_S {
  struct { DWORD64 Type : 8; DWORD64 Reserved : 56; } Type;
  union  { DWORD64 ULong64; PVOID Pointer; SIZE_T Size; HANDLE Handle; DWORD ULong; } Arg;
} MI_MEM_EXTENDED_PARAMETER;

typedef struct MI_MEM_ADDRESS_REQUIREMENTS_S {
  PVOID  LowestStartingAddress;
  PVOID  HighestEndingAddress;
  SIZE_T Alignment;
} MI_MEM_ADDRESS_REQUIREMENTS;

#define MI_MEM_EXTENDED_PARAMETER_NONPAGED_HUGE   0x00000010

#include <winternl.h>
typedef PVOID    (__stdcall *PVirtualAlloc2)(HANDLE, PVOID, SIZE_T, ULONG, ULONG, MI_MEM_EXTENDED_PARAMETER*, ULONG);
typedef NTSTATUS (__stdcall *PNtAllocateVirtualMemoryEx)(HANDLE, PVOID*, SIZE_T*, ULONG, ULONG, MI_MEM_EXTENDED_PARAMETER*, ULONG);
static PVirtualAlloc2 pVirtualAlloc2 = NULL;
static PNtAllocateVirtualMemoryEx pNtAllocateVirtualMemoryEx = NULL;

// Similarly, GetNumaProcesorNodeEx is only supported since Windows 7
typedef struct MI_PROCESSOR_NUMBER_S { WORD Group; BYTE Number; BYTE Reserved; } MI_PROCESSOR_NUMBER;

typedef VOID (__stdcall *PGetCurrentProcessorNumberEx)(MI_PROCESSOR_NUMBER* ProcNumber);
typedef BOOL (__stdcall *PGetNumaProcessorNodeEx)(MI_PROCESSOR_NUMBER* Processor, PUSHORT NodeNumber);
typedef BOOL (__stdcall* PGetNumaNodeProcessorMaskEx)(USHORT Node, PGROUP_AFFINITY ProcessorMask);
typedef BOOL (__stdcall *PGetNumaProcessorNode)(UCHAR Processor, PUCHAR NodeNumber);
static PGetCurrentProcessorNumberEx pGetCurrentProcessorNumberEx = NULL;
static PGetNumaProcessorNodeEx      pGetNumaProcessorNodeEx = NULL;
static PGetNumaNodeProcessorMaskEx  pGetNumaNodeProcessorMaskEx = NULL;
static PGetNumaProcessorNode        pGetNumaProcessorNode = NULL;

//---------------------------------------------
// Enable large page support dynamically (if possible)
//---------------------------------------------

static bool win_enable_large_os_pages(size_t* large_page_size)
{
  static bool large_initialized = false;
  if (large_initialized) return (_mi_os_large_page_size() > 0);
  large_initialized = true;

  // Try to see if large OS pages are supported
  // To use large pages on Windows, we first need access permission
  // Set "Lock pages in memory" permission in the group policy editor
  // <https://devblogs.microsoft.com/oldnewthing/20110128-00/?p=11643>
  unsigned long err = 0;
  HANDLE token = NULL;
  BOOL ok = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token);
  if (ok) {
    TOKEN_PRIVILEGES tp;
    ok = LookupPrivilegeValue(NULL, TEXT("SeLockMemoryPrivilege"), &tp.Privileges[0].Luid);
    if (ok) {
      tp.PrivilegeCount = 1;
      tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      ok = AdjustTokenPrivileges(token, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
      if (ok) {
        err = GetLastError();
        ok = (err == ERROR_SUCCESS);
        if (ok && large_page_size != NULL) {
          *large_page_size = GetLargePageMinimum();
        }
      }
    }
    CloseHandle(token);
  }
  if (!ok) {
    if (err == 0) err = GetLastError();
    _mi_warning_message("cannot enable large OS page support, error %lu\n", err);
  }
  return (ok!=0);
}


//---------------------------------------------
// Initialize
//---------------------------------------------

void _mi_prim_mem_init( mi_os_mem_config_t* config )
{
  config->has_overcommit = false;
  config->has_partial_free = false;
  config->has_virtual_reserve = true;
  // get the page size
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  if (si.dwPageSize > 0) { config->page_size = si.dwPageSize; }
  if (si.dwAllocationGranularity > 0) { config->alloc_granularity = si.dwAllocationGranularity; }
  // get virtual address bits
  if ((uintptr_t)si.lpMaximumApplicationAddress > 0) {
    const size_t vbits = MI_INTPTR_BITS - mi_clz((uintptr_t)si.lpMaximumApplicationAddress);
    config->virtual_address_bits = vbits;
  }
  // get physical memory
  ULONGLONG memInKiB = 0;
  if (GetPhysicallyInstalledSystemMemory(&memInKiB)) {
    if (memInKiB > 0 && memInKiB < (SIZE_MAX / MI_KiB)) {
      config->physical_memory = memInKiB * MI_KiB;
    }
  }
  // get the VirtualAlloc2 function
  HINSTANCE  hDll;
  hDll = LoadLibrary(TEXT("kernelbase.dll"));
  if (hDll != NULL) {
    // use VirtualAlloc2FromApp if possible as it is available to Windows store apps
    pVirtualAlloc2 = (PVirtualAlloc2)(void (*)(void))GetProcAddress(hDll, "VirtualAlloc2FromApp");
    if (pVirtualAlloc2==NULL) pVirtualAlloc2 = (PVirtualAlloc2)(void (*)(void))GetProcAddress(hDll, "VirtualAlloc2");
    FreeLibrary(hDll);
  }
  // NtAllocateVirtualMemoryEx is used for huge page allocation
  hDll = LoadLibrary(TEXT("ntdll.dll"));
  if (hDll != NULL) {
    pNtAllocateVirtualMemoryEx = (PNtAllocateVirtualMemoryEx)(void (*)(void))GetProcAddress(hDll, "NtAllocateVirtualMemoryEx");
    FreeLibrary(hDll);
  }
  // Try to use Win7+ numa API
  hDll = LoadLibrary(TEXT("kernel32.dll"));
  if (hDll != NULL) {
    pGetCurrentProcessorNumberEx = (PGetCurrentProcessorNumberEx)(void (*)(void))GetProcAddress(hDll, "GetCurrentProcessorNumberEx");
    pGetNumaProcessorNodeEx = (PGetNumaProcessorNodeEx)(void (*)(void))GetProcAddress(hDll, "GetNumaProcessorNodeEx");
    pGetNumaNodeProcessorMaskEx = (PGetNumaNodeProcessorMaskEx)(void (*)(void))GetProcAddress(hDll, "GetNumaNodeProcessorMaskEx");
    pGetNumaProcessorNode = (PGetNumaProcessorNode)(void (*)(void))GetProcAddress(hDll, "GetNumaProcessorNode");
    FreeLibrary(hDll);
  }
  if (mi_option_is_enabled(mi_option_allow_large_os_pages) || mi_option_is_enabled(mi_option_reserve_huge_os_pages)) {
    win_enable_large_os_pages(&config->large_page_size);
  }
}


//---------------------------------------------
// Free
//---------------------------------------------

int _mi_prim_free(void* addr, size_t size ) {
  MI_UNUSED(size);
  DWORD errcode = 0;
  bool err = (VirtualFree(addr, 0, MEM_RELEASE) == 0);
  if (err) { errcode = GetLastError(); }
  if (errcode == ERROR_INVALID_ADDRESS) {
    // In mi_os_mem_alloc_aligned the fallback path may have returned a pointer inside
    // the memory region returned by VirtualAlloc; in that case we need to free using
    // the start of the region.
    MEMORY_BASIC_INFORMATION info = { 0 };
    VirtualQuery(addr, &info, sizeof(info));
    if (info.AllocationBase < addr && ((uint8_t*)addr - (uint8_t*)info.AllocationBase) < (ptrdiff_t)MI_SEGMENT_SIZE) {
      errcode = 0;
      err = (VirtualFree(info.AllocationBase, 0, MEM_RELEASE) == 0);
      if (err) { errcode = GetLastError(); }
    }
  }
  return (int)errcode;
}


//---------------------------------------------
// VirtualAlloc
//---------------------------------------------

static void* win_virtual_alloc_prim_once(void* addr, size_t size, size_t try_alignment, DWORD flags) {
  #if (MI_INTPTR_SIZE >= 8)
  // on 64-bit systems, try to use the virtual address area after 2TiB for 4MiB aligned allocations
  if (addr == NULL) {
    void* hint = _mi_os_get_aligned_hint(try_alignment,size);
    if (hint != NULL) {
      void* p = VirtualAlloc(hint, size, flags, PAGE_READWRITE);
      if (p != NULL) return p;
      _mi_verbose_message("warning: unable to allocate hinted aligned OS memory (%zu bytes, error code: 0x%x, address: %p, alignment: %zu, flags: 0x%x)\n", size, GetLastError(), hint, try_alignment, flags);
      // fall through on error
    }
  }
  #endif // 15352
  // on modern Windows try use VirtualAlloc2 for aligned allocation
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0 && pVirtualAlloc2 != NULL) {
    MI_MEM_ADDRESS_REQUIREMENTS reqs = { 0, 0, 0 };
    reqs.Alignment = try_alignment;
    MI_MEM_EXTENDED_PARAMETER param = { {0, 0}, {0} };
    param.Type.Type = MiMemExtendedParameterAddressRequirements;
    param.Arg.Pointer = &reqs;
    void* p = (*pVirtualAlloc2)(GetCurrentProcess(), addr, size, flags, PAGE_READWRITE, &param, 1);
    if (p != NULL) return p;
    _mi_warning_message("unable to allocate aligned OS memory (0x%zx bytes, error code: 0x%x, address: %p, alignment: 0x%zx, flags: 0x%x)\n", size, GetLastError(), addr, try_alignment, flags);
    // fall through on error
  }
  // last resort
  return VirtualAlloc(addr, size, flags, PAGE_READWRITE);
}

static bool win_is_out_of_memory_error(DWORD err) {
  switch (err) {
    case ERROR_COMMITMENT_MINIMUM:
    case ERROR_COMMITMENT_LIMIT:
    case ERROR_PAGEFILE_QUOTA:
    case ERROR_NOT_ENOUGH_MEMORY:
      return true;
    default:
      return false;
  }
}

static void* win_virtual_alloc_prim(void* addr, size_t size, size_t try_alignment, DWORD flags) {
  long max_retry_msecs = mi_option_get_clamp(mi_option_retry_on_oom, 0, 2000);  // at most 2 seconds
  if (max_retry_msecs == 1) { max_retry_msecs = 100; }  // if one sets the option to "true"
  for (long tries = 1; tries <= 10; tries++) {          // try at most 10 times (=2200ms)
    void* p = win_virtual_alloc_prim_once(addr, size, try_alignment, flags);
    if (p != NULL) {
      // success, return the address
      return p;
    }
    else if (max_retry_msecs > 0 && (try_alignment <= 2*MI_SEGMENT_ALIGN) &&
              (flags&MEM_COMMIT) != 0 && (flags&MEM_LARGE_PAGES) == 0 &&
              win_is_out_of_memory_error(GetLastError())) {
      // if committing regular memory and being out-of-memory,
      // keep trying for a bit in case memory frees up after all. See issue #894
      _mi_warning_message("out-of-memory on OS allocation, try again... (attempt %lu, 0x%zx bytes, error code: 0x%x, address: %p, alignment: 0x%zx, flags: 0x%x)\n", tries, size, GetLastError(), addr, try_alignment, flags);
      long sleep_msecs = tries*40;  // increasing waits
      if (sleep_msecs > max_retry_msecs) { sleep_msecs = max_retry_msecs; }
      max_retry_msecs -= sleep_msecs;
      Sleep(sleep_msecs);
    }
    else {
      // otherwise return with an error
      break;
    }
  }
  return NULL;
}

static void* win_virtual_alloc(void* addr, size_t size, size_t try_alignment, DWORD flags, bool large_only, bool allow_large, bool* is_large) {
  mi_assert_internal(!(large_only && !allow_large));
  static _Atomic(size_t) large_page_try_ok; // = 0;
  void* p = NULL;
  // Try to allocate large OS pages (2MiB) if allowed or required.
  if ((large_only || _mi_os_use_large_page(size, try_alignment))
      && allow_large && (flags&MEM_COMMIT)!=0 && (flags&MEM_RESERVE)!=0) {
    size_t try_ok = mi_atomic_load_acquire(&large_page_try_ok);
    if (!large_only && try_ok > 0) {
      // if a large page allocation fails, it seems the calls to VirtualAlloc get very expensive.
      // therefore, once a large page allocation failed, we don't try again for `large_page_try_ok` times.
      mi_atomic_cas_strong_acq_rel(&large_page_try_ok, &try_ok, try_ok - 1);
    }
    else {
      // large OS pages must always reserve and commit.
      *is_large = true;
      p = win_virtual_alloc_prim(addr, size, try_alignment, flags | MEM_LARGE_PAGES);
      if (large_only) return p;
      // fall back to non-large page allocation on error (`p == NULL`).
      if (p == NULL) {
        mi_atomic_store_release(&large_page_try_ok,10UL);  // on error, don't try again for the next N allocations
      }
    }
  }
  // Fall back to regular page allocation
  if (p == NULL) {
    *is_large = ((flags&MEM_LARGE_PAGES) != 0);
    p = win_virtual_alloc_prim(addr, size, try_alignment, flags);
  }
  //if (p == NULL) { _mi_warning_message("unable to allocate OS memory (%zu bytes, error code: 0x%x, address: %p, alignment: %zu, flags: 0x%x, large only: %d, allow large: %d)\n", size, GetLastError(), addr, try_alignment, flags, large_only, allow_large); }
  return p;
}

int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr) {
  mi_assert_internal(size > 0 && (size % _mi_os_page_size()) == 0);
  mi_assert_internal(commit || !allow_large);
  mi_assert_internal(try_alignment > 0);
  *is_zero = true;
  int flags = MEM_RESERVE;
  if (commit) { flags |= MEM_COMMIT; }
  *addr = win_virtual_alloc(hint_addr, size, try_alignment, flags, false, allow_large, is_large);
  return (*addr != NULL ? 0 : (int)GetLastError());
}


//---------------------------------------------
// Commit/Reset/Protect
//---------------------------------------------
#if 0 // #ifdef _MSC_VER
// #pragma warning(disable:6250)   // suppress warning calling VirtualFree without MEM_RELEASE (for decommit)
#endif // 15468

int _mi_prim_commit(void* addr, size_t size, bool* is_zero) {
  *is_zero = false;
  /*
  // zero'ing only happens on an initial commit... but checking upfront seems expensive..
  _MEMORY_BASIC_INFORMATION meminfo; _mi_memzero_var(meminfo);
  if (VirtualQuery(addr, &meminfo, size) > 0) {
    if ((meminfo.State & MEM_COMMIT) == 0) {
      *is_zero = true;
    }
  }
  */
  // commit
  void* p = VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE);
  if (p == NULL) return (int)GetLastError();
  return 0;
}

int _mi_prim_decommit(void* addr, size_t size, bool* needs_recommit) {
  BOOL ok = VirtualFree(addr, size, MEM_DECOMMIT);
  *needs_recommit = true;  // for safety, assume always decommitted even in the case of an error.
  return (ok ? 0 : (int)GetLastError());
}

int _mi_prim_reset(void* addr, size_t size) {
  void* p = VirtualAlloc(addr, size, MEM_RESET, PAGE_READWRITE);
  mi_assert_internal(p == addr);
  #if 0
  if (p != NULL) {
    VirtualUnlock(addr,size); // VirtualUnlock after MEM_RESET removes the memory directly from the working set
  }
  #endif // 15498
  return (p != NULL ? 0 : (int)GetLastError());
}

int _mi_prim_protect(void* addr, size_t size, bool protect) {
  DWORD oldprotect = 0;
  BOOL ok = VirtualProtect(addr, size, protect ? PAGE_NOACCESS : PAGE_READWRITE, &oldprotect);
  return (ok ? 0 : (int)GetLastError());
}


//---------------------------------------------
// Huge page allocation
//---------------------------------------------

static void* _mi_prim_alloc_huge_os_pagesx(void* hint_addr, size_t size, int numa_node)
{
  const DWORD flags = MEM_LARGE_PAGES | MEM_COMMIT | MEM_RESERVE;

  win_enable_large_os_pages(NULL);

  MI_MEM_EXTENDED_PARAMETER params[3] = { {{0,0},{0}},{{0,0},{0}},{{0,0},{0}} };
  // on modern Windows try use NtAllocateVirtualMemoryEx for 1GiB huge pages
  static bool mi_huge_pages_available = true;
  if (pNtAllocateVirtualMemoryEx != NULL && mi_huge_pages_available) {
    params[0].Type.Type = MiMemExtendedParameterAttributeFlags;
    params[0].Arg.ULong64 = MI_MEM_EXTENDED_PARAMETER_NONPAGED_HUGE;
    ULONG param_count = 1;
    if (numa_node >= 0) {
      param_count++;
      params[1].Type.Type = MiMemExtendedParameterNumaNode;
      params[1].Arg.ULong = (unsigned)numa_node;
    }
    SIZE_T psize = size;
    void* base = hint_addr;
    NTSTATUS err = (*pNtAllocateVirtualMemoryEx)(GetCurrentProcess(), &base, &psize, flags, PAGE_READWRITE, params, param_count);
    if (err == 0 && base != NULL) {
      return base;
    }
    else {
      // fall back to regular large pages
      mi_huge_pages_available = false; // don't try further huge pages
      _mi_warning_message("unable to allocate using huge (1GiB) pages, trying large (2MiB) pages instead (status 0x%lx)\n", err);
    }
  }
  // on modern Windows try use VirtualAlloc2 for numa aware large OS page allocation
  if (pVirtualAlloc2 != NULL && numa_node >= 0) {
    params[0].Type.Type = MiMemExtendedParameterNumaNode;
    params[0].Arg.ULong = (unsigned)numa_node;
    return (*pVirtualAlloc2)(GetCurrentProcess(), hint_addr, size, flags, PAGE_READWRITE, params, 1);
  }

  // otherwise use regular virtual alloc on older windows
  return VirtualAlloc(hint_addr, size, flags, PAGE_READWRITE);
}

int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  *is_zero = true;
  *addr = _mi_prim_alloc_huge_os_pagesx(hint_addr,size,numa_node);
  return (*addr != NULL ? 0 : (int)GetLastError());
}


//---------------------------------------------
// Numa nodes
//---------------------------------------------

size_t _mi_prim_numa_node(void) {
  USHORT numa_node = 0;
  if (pGetCurrentProcessorNumberEx != NULL && pGetNumaProcessorNodeEx != NULL) {
    // Extended API is supported
    MI_PROCESSOR_NUMBER pnum;
    (*pGetCurrentProcessorNumberEx)(&pnum);
    USHORT nnode = 0;
    BOOL ok = (*pGetNumaProcessorNodeEx)(&pnum, &nnode);
    if (ok) { numa_node = nnode; }
  }
  else if (pGetNumaProcessorNode != NULL) {
    // Vista or earlier, use older API that is limited to 64 processors. Issue #277
    DWORD pnum = GetCurrentProcessorNumber();
    UCHAR nnode = 0;
    BOOL ok = pGetNumaProcessorNode((UCHAR)pnum, &nnode);
    if (ok) { numa_node = nnode; }
  }
  return numa_node;
}

size_t _mi_prim_numa_node_count(void) {
  ULONG numa_max = 0;
  GetNumaHighestNodeNumber(&numa_max);
  // find the highest node number that has actual processors assigned to it. Issue #282
  while(numa_max > 0) {
    if (pGetNumaNodeProcessorMaskEx != NULL) {
      // Extended API is supported
      GROUP_AFFINITY affinity;
      if ((*pGetNumaNodeProcessorMaskEx)((USHORT)numa_max, &affinity)) {
        if (affinity.Mask != 0) break;  // found the maximum non-empty node
      }
    }
    else {
      // Vista or earlier, use older API that is limited to 64 processors.
      ULONGLONG mask;
      if (GetNumaNodeProcessorMask((UCHAR)numa_max, &mask)) {
        if (mask != 0) break; // found the maximum non-empty node
      };
    }
    // max node was invalid or had no processor assigned, try again
    numa_max--;
  }
  return ((size_t)numa_max + 1);
}


//----------------------------------------------------------------
// Clock
//----------------------------------------------------------------

static mi_msecs_t mi_to_msecs(LARGE_INTEGER t) {
  static LARGE_INTEGER mfreq; // = 0
  if (mfreq.QuadPart == 0LL) {
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    mfreq.QuadPart = f.QuadPart/1000LL;
    if (mfreq.QuadPart == 0) mfreq.QuadPart = 1;
  }
  return (mi_msecs_t)(t.QuadPart / mfreq.QuadPart);
}

mi_msecs_t _mi_prim_clock_now(void) {
  LARGE_INTEGER t;
  QueryPerformanceCounter(&t);
  return mi_to_msecs(t);
}


//----------------------------------------------------------------
// Process Info
//----------------------------------------------------------------

#include <psapi.h>

static mi_msecs_t filetime_msecs(const FILETIME* ftime) {
  ULARGE_INTEGER i;
  i.LowPart = ftime->dwLowDateTime;
  i.HighPart = ftime->dwHighDateTime;
  mi_msecs_t msecs = (i.QuadPart / 10000); // FILETIME is in 100 nano seconds
  return msecs;
}

typedef BOOL (WINAPI *PGetProcessMemoryInfo)(HANDLE, PPROCESS_MEMORY_COUNTERS, DWORD);
static PGetProcessMemoryInfo pGetProcessMemoryInfo = NULL;

void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  FILETIME ct;
  FILETIME ut;
  FILETIME st;
  FILETIME et;
  GetProcessTimes(GetCurrentProcess(), &ct, &et, &st, &ut);
  pinfo->utime = filetime_msecs(&ut);
  pinfo->stime = filetime_msecs(&st);

  // load psapi on demand
  if (pGetProcessMemoryInfo == NULL) {
    HINSTANCE hDll = LoadLibrary(TEXT("psapi.dll"));
    if (hDll != NULL) {
      pGetProcessMemoryInfo = (PGetProcessMemoryInfo)(void (*)(void))GetProcAddress(hDll, "GetProcessMemoryInfo");
    }
  }

  // get process info
  PROCESS_MEMORY_COUNTERS info; _mi_memzero_var(info);
  if (pGetProcessMemoryInfo != NULL) {
    pGetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
  }
  pinfo->current_rss    = (size_t)info.WorkingSetSize;
  pinfo->peak_rss       = (size_t)info.PeakWorkingSetSize;
  pinfo->current_commit = (size_t)info.PagefileUsage;
  pinfo->peak_commit    = (size_t)info.PeakPagefileUsage;
  pinfo->page_faults    = (size_t)info.PageFaultCount;
}

//----------------------------------------------------------------
// Output
//----------------------------------------------------------------

void _mi_prim_out_stderr( const char* msg )
{
  // on windows with redirection, the C runtime cannot handle locale dependent output
  // after the main thread closes so we use direct console output.
  if (!_mi_preloading()) {
    // _cputs(msg);  // _cputs cannot be used as it aborts when failing to lock the console
    static HANDLE hcon = INVALID_HANDLE_VALUE;
    static bool hconIsConsole;
    if (hcon == INVALID_HANDLE_VALUE) {
      CONSOLE_SCREEN_BUFFER_INFO sbi;
      hcon = GetStdHandle(STD_ERROR_HANDLE);
      hconIsConsole = ((hcon != INVALID_HANDLE_VALUE) && GetConsoleScreenBufferInfo(hcon, &sbi));
    }
    const size_t len = _mi_strlen(msg);
    if (len > 0 && len < UINT32_MAX) {
      DWORD written = 0;
      if (hconIsConsole) {
        WriteConsoleA(hcon, msg, (DWORD)len, &written, NULL);
      }
      else if (hcon != INVALID_HANDLE_VALUE) {
        // use direct write if stderr was redirected
        WriteFile(hcon, msg, (DWORD)len, &written, NULL);
      }
      else {
        // finally fall back to fputs after all
        fputs(msg, stderr);
      }
    }
  }
}


//----------------------------------------------------------------
// Environment
//----------------------------------------------------------------

// On Windows use GetEnvironmentVariable instead of getenv to work
// reliably even when this is invoked before the C runtime is initialized.
// i.e. when `_mi_preloading() == true`.
// Note: on windows, environment names are not case sensitive.
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  result[0] = 0;
  size_t len = GetEnvironmentVariableA(name, result, (DWORD)result_size);
  return (len > 0 && len < result_size);
}


//----------------------------------------------------------------
// Random
//----------------------------------------------------------------

#if defined(MI_USE_RTLGENRANDOM) // || defined(__cplusplus)
// We prefer to use BCryptGenRandom instead of (the unofficial) RtlGenRandom but when using
// dynamic overriding, we observed it can raise an exception when compiled with C++, and
// sometimes deadlocks when also running under the VS debugger.
// In contrast, issue #623 implies that on Windows Server 2019 we need to use BCryptGenRandom.
// To be continued..
#pragma comment (lib,"advapi32.lib")
#define RtlGenRandom  SystemFunction036
mi_decl_externc BOOLEAN NTAPI RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  return (RtlGenRandom(buf, (ULONG)buf_len) != 0);
}

#else

#ifndef BCRYPT_USE_SYSTEM_PREFERRED_RNG
#define BCRYPT_USE_SYSTEM_PREFERRED_RNG 0x00000002
#endif // 15755

typedef LONG (NTAPI *PBCryptGenRandom)(HANDLE, PUCHAR, ULONG, ULONG);
static  PBCryptGenRandom pBCryptGenRandom = NULL;

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  if (pBCryptGenRandom == NULL) {
    HINSTANCE hDll = LoadLibrary(TEXT("bcrypt.dll"));
    if (hDll != NULL) {
      pBCryptGenRandom = (PBCryptGenRandom)(void (*)(void))GetProcAddress(hDll, "BCryptGenRandom");
    }
    if (pBCryptGenRandom == NULL) return false;
  }
  return (pBCryptGenRandom(NULL, (PUCHAR)buf, (ULONG)buf_len, BCRYPT_USE_SYSTEM_PREFERRED_RNG) >= 0);
}

#endif  // MI_USE_RTLGENRANDOM // 15739



//----------------------------------------------------------------
// Process & Thread Init/Done
//----------------------------------------------------------------

static void NTAPI mi_win_main(PVOID module, DWORD reason, LPVOID reserved) {
  MI_UNUSED(reserved);
  MI_UNUSED(module);
  if (reason==DLL_PROCESS_ATTACH) {
    _mi_process_load();
  }
  else if (reason==DLL_PROCESS_DETACH) {
    _mi_process_done();
  }
  else if (reason==DLL_THREAD_DETACH && !_mi_is_redirected()) {
    _mi_thread_done(NULL);
  }
}


#if defined(MI_SHARED_LIB)
  #define MI_PRIM_HAS_PROCESS_ATTACH  1

  // Windows DLL: easy to hook into process_init and thread_done
  __declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved) {
    mi_win_main((PVOID)inst,reason,reserved);
    return TRUE;
  }

  // nothing to do since `_mi_thread_done` is handled through the DLL_THREAD_DETACH event.
  void _mi_prim_thread_init_auto_done(void) { }
  void _mi_prim_thread_done_auto_done(void) { }
  void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
    MI_UNUSED(heap);
  }

#elif !defined(MI_WIN_USE_FLS)
  #define MI_PRIM_HAS_PROCESS_ATTACH  1

  static void NTAPI mi_win_main_attach(PVOID module, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH || reason == DLL_THREAD_ATTACH) {
      mi_win_main(module, reason, reserved);
    }
  }
  static void NTAPI mi_win_main_detach(PVOID module, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_DETACH || reason == DLL_THREAD_DETACH) {
      mi_win_main(module, reason, reserved);
    }
  }

  // Set up TLS callbacks in a statically linked library by using special data sections.
  // See <https://stackoverflow.com/questions/14538159/tls-callback-in-windows>
  // We use 2 entries to ensure we call attach events before constructors
  // are called, and detach events after destructors are called.
#if 0 //   #if defined(__cplusplus)
//   extern "C" {
  #endif // 15830

  #if defined(_WIN64)
    #pragma comment(linker, "/INCLUDE:_tls_used")
    #pragma comment(linker, "/INCLUDE:_mi_tls_callback_pre")
    #pragma comment(linker, "/INCLUDE:_mi_tls_callback_post")
    #pragma const_seg(".CRT$XLB")
    extern const PIMAGE_TLS_CALLBACK _mi_tls_callback_pre[];
    const PIMAGE_TLS_CALLBACK _mi_tls_callback_pre[] = { &mi_win_main_attach };
    #pragma const_seg()
    #pragma const_seg(".CRT$XLY")
    extern const PIMAGE_TLS_CALLBACK _mi_tls_callback_post[];
    const PIMAGE_TLS_CALLBACK _mi_tls_callback_post[] = { &mi_win_main_detach };
    #pragma const_seg()
  #else
    #pragma comment(linker, "/INCLUDE:__tls_used")
    #pragma comment(linker, "/INCLUDE:__mi_tls_callback_pre")
    #pragma comment(linker, "/INCLUDE:__mi_tls_callback_post")
    #pragma data_seg(".CRT$XLB")
    PIMAGE_TLS_CALLBACK _mi_tls_callback_pre[] = { &mi_win_main_attach };
    #pragma data_seg()
    #pragma data_seg(".CRT$XLY")
    PIMAGE_TLS_CALLBACK _mi_tls_callback_post[] = { &mi_win_main_detach };
    #pragma data_seg()
  #endif // 15834

#if 0 //   #if defined(__cplusplus)
//   }
  #endif // 15858

  // nothing to do since `_mi_thread_done` is handled through the DLL_THREAD_DETACH event.
  void _mi_prim_thread_init_auto_done(void) { }
  void _mi_prim_thread_done_auto_done(void) { }
  void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
    MI_UNUSED(heap);
  }

#else // deprecated: statically linked, use fiber api

  #if defined(_MSC_VER) // on clang/gcc use the constructor attribute (in `src/prim/prim.c`)
    // MSVC: use data section magic for static libraries
    // See <https://www.codeguru.com/cpp/misc/misc/applicationcontrol/article.php/c6945/Running-Code-Before-and-After-Main.htm>
    #define MI_PRIM_HAS_PROCESS_ATTACH 1

    static int mi_process_attach(void) {
      mi_win_main(NULL,DLL_PROCESS_ATTACH,NULL);
      atexit(&_mi_process_done);
      return 0;
    }
    typedef int(*mi_crt_callback_t)(void);
    #if defined(_WIN64)
      #pragma comment(linker, "/INCLUDE:_mi_tls_callback")
      #pragma section(".CRT$XIU", long, read)
    #else
      #pragma comment(linker, "/INCLUDE:__mi_tls_callback")
    #endif // 15882
    #pragma data_seg(".CRT$XIU")
    mi_decl_externc mi_crt_callback_t _mi_tls_callback[] = { &mi_process_attach };
    #pragma data_seg()
  #endif // 15871

  // use the fiber api for calling `_mi_thread_done`.
  #include <fibersapi.h>
  #if (_WIN32_WINNT < 0x600)  // before Windows Vista
  WINBASEAPI DWORD WINAPI FlsAlloc( _In_opt_ PFLS_CALLBACK_FUNCTION lpCallback );
  WINBASEAPI PVOID WINAPI FlsGetValue( _In_ DWORD dwFlsIndex );
  WINBASEAPI BOOL  WINAPI FlsSetValue( _In_ DWORD dwFlsIndex, _In_opt_ PVOID lpFlsData );
  WINBASEAPI BOOL  WINAPI FlsFree(_In_ DWORD dwFlsIndex);
  #endif // 15895

  static DWORD mi_fls_key = (DWORD)(-1);

  static void NTAPI mi_fls_done(PVOID value) {
    mi_heap_t* heap = (mi_heap_t*)value;
    if (heap != NULL) {
      _mi_thread_done(heap);
      FlsSetValue(mi_fls_key, NULL);  // prevent recursion as _mi_thread_done may set it back to the main heap, issue #672
    }
  }

  void _mi_prim_thread_init_auto_done(void) {
    mi_fls_key = FlsAlloc(&mi_fls_done);
  }

  void _mi_prim_thread_done_auto_done(void) {
    // call thread-done on all threads (except the main thread) to prevent
    // dangling callback pointer if statically linked with a DLL; Issue #208
    FlsFree(mi_fls_key);
  }

  void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
    mi_assert_internal(mi_fls_key != (DWORD)(-1));
    FlsSetValue(mi_fls_key, heap);
  }
#endif // 15796

// ----------------------------------------------------
// Communicate with the redirection module on Windows
// ----------------------------------------------------
#if defined(MI_SHARED_LIB) && !defined(MI_WIN_NOREDIRECT)
  #define MI_PRIM_HAS_ALLOCATOR_INIT 1

  static bool mi_redirected = false;   // true if malloc redirects to mi_malloc

  bool _mi_is_redirected(void) {
    return mi_redirected;
  }

#if 0 //   #ifdef __cplusplus
//   extern "C" {
  #endif // 15940
  mi_decl_export void _mi_redirect_entry(DWORD reason) {
    // called on redirection; careful as this may be called before DllMain
    if (reason == DLL_PROCESS_ATTACH) {
      mi_redirected = true;
    }
    else if (reason == DLL_PROCESS_DETACH) {
      mi_redirected = false;
    }
    else if (reason == DLL_THREAD_DETACH) {
      _mi_thread_done(NULL);
    }
  }
  __declspec(dllimport) bool mi_cdecl mi_allocator_init(const char** message);
  __declspec(dllimport) void mi_cdecl mi_allocator_done(void);
#if 0 //   #ifdef __cplusplus
//   }
  #endif // 15957
  bool _mi_allocator_init(const char** message) {
    return mi_allocator_init(message);
  }
  void _mi_allocator_done(void) {
    mi_allocator_done();
  }
#endif // 15931
// --- Amalgamation END --- mimalloc/src/prim/windows/prim.c


#elif defined(__APPLE__)

// --- Amalgamation START --- mimalloc/src/prim/osx/prim.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// We use the unix/prim.c with the mmap API on macOSX

// --- Amalgamation START --- mimalloc/src/prim/osx/../unix/prim.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// This file is included in `src/prim/prim.c`

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE   // ensure mmap flags and syscall are defined
#endif // 15992

#if 0 // #if defined(__sun)
// // illumos provides new mman.h api when any of these are defined
// // otherwise the old api based on caddr_t which predates the void pointers one.
// // stock solaris provides only the former, chose to atomically to discard those
// // flags only here rather than project wide tough.
// #undef _XOPEN_SOURCE
// #undef _POSIX_C_SOURCE
#endif // 15996


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


#include <sys/mman.h>  // mmap
#include <unistd.h>    // sysconf
#include <fcntl.h>     // open, close, read, access

#if defined(__linux__)
  #include <features.h>
  //#if defined(MI_NO_THP)
  #include <sys/prctl.h>  // THP disable
  //#endif
  #if defined(__GLIBC__)
  #include <linux/mman.h> // linux mmap flags
  #else
  #include <sys/mman.h>
  #endif // 16027
#elif defined(__APPLE__)
  #include <AvailabilityMacros.h>
  #include <TargetConditionals.h>
  #if !defined(TARGET_OS_OSX) || TARGET_OS_OSX   // see issue #879, used to be (!TARGET_IOS_IPHONE && !TARGET_IOS_SIMULATOR)
  #include <mach/vm_statistics.h>    // VM_MAKE_TAG, VM_FLAGS_SUPERPAGE_SIZE_2MB, etc.
  #endif // 16035
  #if !defined(MAC_OS_X_VERSION_10_7)
  #define MAC_OS_X_VERSION_10_7   1070
  #endif // 16038
#elif defined(__FreeBSD__) || defined(__DragonFly__)
  #include <sys/param.h>
  #if __FreeBSD_version >= 1200000
  #include <sys/cpuset.h>
  #include <sys/domainset.h>
  #endif // 16043
  #include <sys/sysctl.h>
#endif // 16022

#if defined(__linux__) || defined(__FreeBSD__)
  #define MI_HAS_SYSCALL_H
  #include <sys/syscall.h>
#endif // 16050


//------------------------------------------------------------------------------------
// Use syscalls for some primitives to allow for libraries that override open/read/close etc.
// and do allocation themselves; using syscalls prevents recursion when mimalloc is
// still initializing (issue #713)
// Declare inline to avoid unused function warnings.
//------------------------------------------------------------------------------------

#if defined(MI_HAS_SYSCALL_H) && defined(SYS_open) && defined(SYS_close) && defined(SYS_read) && defined(SYS_access)

static inline int mi_prim_open(const char* fpath, int open_flags) {
  return syscall(SYS_open,fpath,open_flags,0);
}
static inline ssize_t mi_prim_read(int fd, void* buf, size_t bufsize) {
  return syscall(SYS_read,fd,buf,bufsize);
}
static inline int mi_prim_close(int fd) {
  return syscall(SYS_close,fd);
}
static inline int mi_prim_access(const char *fpath, int mode) {
  return syscall(SYS_access,fpath,mode);
}

#else

static inline int mi_prim_open(const char* fpath, int open_flags) {
  return open(fpath,open_flags);
}
static inline ssize_t mi_prim_read(int fd, void* buf, size_t bufsize) {
  return read(fd,buf,bufsize);
}
static inline int mi_prim_close(int fd) {
  return close(fd);
}
static inline int mi_prim_access(const char *fpath, int mode) {
  return access(fpath,mode);
}

#endif // 16063



//---------------------------------------------
// init
//---------------------------------------------

static bool unix_detect_overcommit(void) {
  bool os_overcommit = true;
#if defined(__linux__)
  int fd = mi_prim_open("/proc/sys/vm/overcommit_memory", O_RDONLY);
	if (fd >= 0) {
    char buf[32];
    ssize_t nread = mi_prim_read(fd, &buf, sizeof(buf));
    mi_prim_close(fd);
    // <https://www.kernel.org/doc/Documentation/vm/overcommit-accounting>
    // 0: heuristic overcommit, 1: always overcommit, 2: never overcommit (ignore NORESERVE)
    if (nread >= 1) {
      os_overcommit = (buf[0] == '0' || buf[0] == '1');
    }
  }
#elif defined(__FreeBSD__)
  int val = 0;
  size_t olen = sizeof(val);
  if (sysctlbyname("vm.overcommit", &val, &olen, NULL, 0) == 0) {
    os_overcommit = (val != 0);
  }
#else
  // default: overcommit is true
#endif // 16103
  return os_overcommit;
}

void _mi_prim_mem_init( mi_os_mem_config_t* config )
{
  long psize = sysconf(_SC_PAGESIZE);
  if (psize > 0) {
    config->page_size = (size_t)psize;
    config->alloc_granularity = (size_t)psize;
    #if defined(_SC_PHYS_PAGES)
    long pphys = sysconf(_SC_PHYS_PAGES);
    if (pphys > 0 && (size_t)pphys < (SIZE_MAX/(size_t)psize)) {
      config->physical_memory = (size_t)pphys * (size_t)psize;
    }
    #endif // 16133
  }
  config->large_page_size = 2*MI_MiB; // TODO: can we query the OS for this?
  config->has_overcommit = unix_detect_overcommit();
  config->has_partial_free = true;    // mmap can free in parts
  config->has_virtual_reserve = true; // todo: check if this true for NetBSD?  (for anonymous mmap with PROT_NONE)

  // disable transparent huge pages for this process?
  #if (defined(__linux__) || defined(__ANDROID__)) && defined(PR_GET_THP_DISABLE)
  #if defined(MI_NO_THP)
  if (true)
  #else
  if (!mi_option_is_enabled(mi_option_allow_large_os_pages)) // disable THP also if large OS pages are not allowed in the options
  #endif // 16147
  {
    int val = 0;
    if (prctl(PR_GET_THP_DISABLE, &val, 0, 0, 0) != 0) {
      // Most likely since distros often come with always/madvise settings.
      val = 1;
      // Disabling only for mimalloc process rather than touching system wide settings
      (void)prctl(PR_SET_THP_DISABLE, &val, 0, 0, 0);
    }
  }
  #endif // 16146
}


//---------------------------------------------
// free
//---------------------------------------------

int _mi_prim_free(void* addr, size_t size ) {
  bool err = (munmap(addr, size) == -1);
  return (err ? errno : 0);
}


//---------------------------------------------
// mmap
//---------------------------------------------

static int unix_madvise(void* addr, size_t size, int advice) {
#if 0 //   #if defined(__sun)
//   int res = madvise((caddr_t)addr, size, advice);  // Solaris needs cast (issue #520)
  #else
  int res = madvise(addr, size, advice);
  #endif // 16180
  return (res==0 ? 0 : errno);
}

static void* unix_mmap_prim(void* addr, size_t size, size_t try_alignment, int protect_flags, int flags, int fd) {
  MI_UNUSED(try_alignment);
  void* p = NULL;
  #if defined(MAP_ALIGNED)  // BSD
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0) {
    size_t n = mi_bsr(try_alignment);
    if (((size_t)1 << n) == try_alignment && n >= 12 && n <= 30) {  // alignment is a power of 2 and 4096 <= alignment <= 1GiB
      p = mmap(addr, size, protect_flags, flags | MAP_ALIGNED(n), fd, 0);
      if (p==MAP_FAILED || !_mi_is_aligned(p,try_alignment)) {
        int err = errno;
        _mi_trace_message("unable to directly request aligned OS memory (error: %d (0x%x), size: 0x%zx bytes, alignment: 0x%zx, hint address: %p)\n", err, err, size, try_alignment, addr);
      }
      if (p!=MAP_FAILED) return p;
      // fall back to regular mmap
    }
  }
  #elif defined(MAP_ALIGN)  // Solaris
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0) {
    p = mmap((void*)try_alignment, size, protect_flags, flags | MAP_ALIGN, fd, 0);  // addr parameter is the required alignment
    if (p!=MAP_FAILED) return p;
    // fall back to regular mmap
  }
  #endif // 16191
  #if (MI_INTPTR_SIZE >= 8) && !defined(MAP_ALIGNED)
  // on 64-bit systems, use the virtual address area after 2TiB for 4MiB aligned allocations
  if (addr == NULL) {
    void* hint = _mi_os_get_aligned_hint(try_alignment, size);
    if (hint != NULL) {
      p = mmap(hint, size, protect_flags, flags, fd, 0);
      if (p==MAP_FAILED || !_mi_is_aligned(p,try_alignment)) {
        #if MI_TRACK_ENABLED  // asan sometimes does not instrument errno correctly?
        int err = 0;
        #else
        int err = errno;
        #endif // 16218
        _mi_trace_message("unable to directly request hinted aligned OS memory (error: %d (0x%x), size: 0x%zx bytes, alignment: 0x%zx, hint address: %p)\n", err, err, size, try_alignment, hint);
      }
      if (p!=MAP_FAILED) return p;
      // fall back to regular mmap
    }
  }
  #endif // 16211
  // regular mmap
  p = mmap(addr, size, protect_flags, flags, fd, 0);
  if (p!=MAP_FAILED) return p;
  // failed to allocate
  return NULL;
}

static int unix_mmap_fd(void) {
  #if defined(VM_MAKE_TAG)
  // macOS: tracking anonymous page with a specific ID. (All up to 98 are taken officially but LLVM sanitizers had taken 99)
  int os_tag = (int)mi_option_get(mi_option_os_tag);
  if (os_tag < 100 || os_tag > 255) { os_tag = 100; }
  return VM_MAKE_TAG(os_tag);
  #else
  return -1;
  #endif // 16238
}

static void* unix_mmap(void* addr, size_t size, size_t try_alignment, int protect_flags, bool large_only, bool allow_large, bool* is_large) {
  #if !defined(MAP_ANONYMOUS)
  #define MAP_ANONYMOUS  MAP_ANON
  #endif // 16249
  #if !defined(MAP_NORESERVE)
  #define MAP_NORESERVE  0
  #endif // 16252
  void* p = NULL;
  const int fd = unix_mmap_fd();
  int flags = MAP_PRIVATE | MAP_ANONYMOUS;
  if (_mi_os_has_overcommit()) {
    flags |= MAP_NORESERVE;
  }
  #if defined(PROT_MAX)
  protect_flags |= PROT_MAX(PROT_READ | PROT_WRITE); // BSD
  #endif // 16261
  // huge page allocation
  if ((large_only || _mi_os_use_large_page(size, try_alignment)) && allow_large) {
    static _Atomic(size_t) large_page_try_ok; // = 0;
    size_t try_ok = mi_atomic_load_acquire(&large_page_try_ok);
    if (!large_only && try_ok > 0) {
      // If the OS is not configured for large OS pages, or the user does not have
      // enough permission, the `mmap` will always fail (but it might also fail for other reasons).
      // Therefore, once a large page allocation failed, we don't try again for `large_page_try_ok` times
      // to avoid too many failing calls to mmap.
      mi_atomic_cas_strong_acq_rel(&large_page_try_ok, &try_ok, try_ok - 1);
    }
    else {
      int lflags = flags & ~MAP_NORESERVE;  // using NORESERVE on huge pages seems to fail on Linux
      int lfd = fd;
      #ifdef MAP_ALIGNED_SUPER
      lflags |= MAP_ALIGNED_SUPER;
      #endif // 16278
      #ifdef MAP_HUGETLB
      lflags |= MAP_HUGETLB;
      #endif // 16281
      #ifdef MAP_HUGE_1GB
      static bool mi_huge_pages_available = true;
      if ((size % MI_GiB) == 0 && mi_huge_pages_available) {
        lflags |= MAP_HUGE_1GB;
      }
      else
      #endif // 16284
      {
        #ifdef MAP_HUGE_2MB
        lflags |= MAP_HUGE_2MB;
        #endif // 16292
      }
      #ifdef VM_FLAGS_SUPERPAGE_SIZE_2MB
      lfd |= VM_FLAGS_SUPERPAGE_SIZE_2MB;
      #endif // 16296
      if (large_only || lflags != flags) {
        // try large OS page allocation
        *is_large = true;
        p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
        #ifdef MAP_HUGE_1GB
        if (p == NULL && (lflags & MAP_HUGE_1GB) == MAP_HUGE_1GB) {
          mi_huge_pages_available = false; // don't try huge 1GiB pages again
          _mi_warning_message("unable to allocate huge (1GiB) page, trying large (2MiB) pages instead (errno: %i)\n", errno);
          lflags = ((lflags & ~MAP_HUGE_1GB) | MAP_HUGE_2MB);
          p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
        }
        #endif // 16303
        if (large_only) return p;
        if (p == NULL) {
          mi_atomic_store_release(&large_page_try_ok, (size_t)8);  // on error, don't try again for the next N allocations
        }
      }
    }
  }
  // regular allocation
  if (p == NULL) {
    *is_large = false;
    p = unix_mmap_prim(addr, size, try_alignment, protect_flags, flags, fd);
    if (p != NULL) {
      #if defined(MADV_HUGEPAGE)
      // Many Linux systems don't allow MAP_HUGETLB but they support instead
      // transparent huge pages (THP). Generally, it is not required to call `madvise` with MADV_HUGE
      // though since properly aligned allocations will already use large pages if available
      // in that case -- in particular for our large regions (in `memory.c`).
      // However, some systems only allow THP if called with explicit `madvise`, so
      // when large OS pages are enabled for mimalloc, we call `madvise` anyways.
      if (allow_large && _mi_os_use_large_page(size, try_alignment)) {
        if (unix_madvise(p, size, MADV_HUGEPAGE) == 0) {
          // *is_large = true; // possibly
        };
      }
      #elif defined(__sun)
      if (allow_large && _mi_os_use_large_page(size, try_alignment)) {
        struct memcntl_mha cmd = {0};
        cmd.mha_pagesize = _mi_os_large_page_size();
        cmd.mha_cmd = MHA_MAPSIZE_VA;
        if (memcntl((caddr_t)p, size, MC_HAT_ADVISE, (caddr_t)&cmd, 0, 0) == 0) {
          // *is_large = true; // possibly
        }
      }
      #endif // 16323
    }
  }
  return p;
}

// Note: the `try_alignment` is just a hint and the returned pointer is not guaranteed to be aligned.
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr) {
  mi_assert_internal(size > 0 && (size % _mi_os_page_size()) == 0);
  mi_assert_internal(commit || !allow_large);
  mi_assert_internal(try_alignment > 0);

  *is_zero = true;
  int protect_flags = (commit ? (PROT_WRITE | PROT_READ) : PROT_NONE);
  *addr = unix_mmap(hint_addr, size, try_alignment, protect_flags, false, allow_large, is_large);
  return (*addr != NULL ? 0 : errno);
}


//---------------------------------------------
// Commit/Reset
//---------------------------------------------

static void unix_mprotect_hint(int err) {
  #if defined(__linux__) && (MI_SECURE>=2) // guard page around every mimalloc page
  if (err == ENOMEM) {
    _mi_warning_message("The next warning may be caused by a low memory map limit.\n"
                        "  On Linux this is controlled by the vm.max_map_count -- maybe increase it?\n"
                        "  For example: sudo sysctl -w vm.max_map_count=262144\n");
  }
  #else
  MI_UNUSED(err);
  #endif // 16368
}





int _mi_prim_commit(void* start, size_t size, bool* is_zero) {
  // commit: ensure we can access the area
  // note: we may think that *is_zero can be true since the memory
  // was either from mmap PROT_NONE, or from decommit MADV_DONTNEED, but
  // we sometimes call commit on a range with still partially committed
  // memory and `mprotect` does not zero the range.
  *is_zero = false;
  int err = mprotect(start, size, (PROT_READ | PROT_WRITE));
  if (err != 0) {
    err = errno;
    unix_mprotect_hint(err);
  }
  return err;
}

int _mi_prim_decommit(void* start, size_t size, bool* needs_recommit) {
  int err = 0;
  // decommit: use MADV_DONTNEED as it decreases rss immediately (unlike MADV_FREE)
  err = unix_madvise(start, size, MADV_DONTNEED);
  #if !MI_DEBUG && !MI_SECURE
    *needs_recommit = false;
  #else
    *needs_recommit = true;
    mprotect(start, size, PROT_NONE);
  #endif // 16402
  /*
  // decommit: use mmap with MAP_FIXED and PROT_NONE to discard the existing memory (and reduce rss)
  *needs_recommit = true;
  const int fd = unix_mmap_fd();
  void* p = mmap(start, size, PROT_NONE, (MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE), fd, 0);
  if (p != start) { err = errno; }
  */
  return err;
}

int _mi_prim_reset(void* start, size_t size) {
  // We try to use `MADV_FREE` as that is the fastest. A drawback though is that it
  // will not reduce the `rss` stats in tools like `top` even though the memory is available
  // to other processes. With the default `MIMALLOC_PURGE_DECOMMITS=1` we ensure that by
  // default `MADV_DONTNEED` is used though.
  #if defined(MADV_FREE)
  static _Atomic(size_t) advice = MI_ATOMIC_VAR_INIT(MADV_FREE);
  int oadvice = (int)mi_atomic_load_relaxed(&advice);
  int err;
  while ((err = unix_madvise(start, size, oadvice)) != 0 && errno == EAGAIN) { errno = 0;  };
  if (err != 0 && errno == EINVAL && oadvice == MADV_FREE) {
    // if MADV_FREE is not supported, fall back to MADV_DONTNEED from now on
    mi_atomic_store_release(&advice, (size_t)MADV_DONTNEED);
    err = unix_madvise(start, size, MADV_DONTNEED);
  }
  #else
  int err = unix_madvise(start, size, MADV_DONTNEED);
  #endif // 16423
  return err;
}

int _mi_prim_protect(void* start, size_t size, bool protect) {
  int err = mprotect(start, size, protect ? PROT_NONE : (PROT_READ | PROT_WRITE));
  if (err != 0) { err = errno; }
  unix_mprotect_hint(err);
  return err;
}



//---------------------------------------------
// Huge page allocation
//---------------------------------------------

#if (MI_INTPTR_SIZE >= 8) && !defined(__HAIKU__) && !defined(__CYGWIN__)

#ifndef MPOL_PREFERRED
#define MPOL_PREFERRED 1
#endif // 16454

#if defined(MI_HAS_SYSCALL_H) && defined(SYS_mbind)
static long mi_prim_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  return syscall(SYS_mbind, start, len, mode, nmask, maxnode, flags);
}
#else
static long mi_prim_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  MI_UNUSED(start); MI_UNUSED(len); MI_UNUSED(mode); MI_UNUSED(nmask); MI_UNUSED(maxnode); MI_UNUSED(flags);
  return 0;
}
#endif // 16458

int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  bool is_large = true;
  *is_zero = true;
  *addr = unix_mmap(hint_addr, size, MI_SEGMENT_SIZE, PROT_READ | PROT_WRITE, true, true, &is_large);
  if (*addr != NULL && numa_node >= 0 && numa_node < 8*MI_INTPTR_SIZE) { // at most 64 nodes
    unsigned long numa_mask = (1UL << numa_node);
    // TODO: does `mbind` work correctly for huge OS pages? should we
    // use `set_mempolicy` before calling mmap instead?
    // see: <https://lkml.org/lkml/2017/2/9/875>
    long err = mi_prim_mbind(*addr, size, MPOL_PREFERRED, &numa_mask, 8*MI_INTPTR_SIZE, 0);
    if (err != 0) {
      err = errno;
      _mi_warning_message("failed to bind huge (1GiB) pages to numa node %d (error: %d (0x%x))\n", numa_node, err, err);
    }
  }
  return (*addr != NULL ? 0 : errno);
}

#else

int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  MI_UNUSED(hint_addr); MI_UNUSED(size); MI_UNUSED(numa_node);
  *is_zero = false;
  *addr = NULL;
  return ENOMEM;
}

#endif // 16452

//---------------------------------------------
// NUMA nodes
//---------------------------------------------

#if defined(__linux__)

size_t _mi_prim_numa_node(void) {
  #if defined(MI_HAS_SYSCALL_H) && defined(SYS_getcpu)
    unsigned long node = 0;
    unsigned long ncpu = 0;
    long err = syscall(SYS_getcpu, &ncpu, &node, NULL);
    if (err != 0) return 0;
    return node;
  #else
    return 0;
  #endif // 16505
}

size_t _mi_prim_numa_node_count(void) {
  char buf[128];
  unsigned node = 0;
  for(node = 0; node < 256; node++) {
    // enumerate node entries -- todo: it there a more efficient way to do this? (but ensure there is no allocation)
    _mi_snprintf(buf, 127, "/sys/devices/system/node/node%u", node + 1);
    if (mi_prim_access(buf,R_OK) != 0) break;
  }
  return (node+1);
}

#elif defined(__FreeBSD__) && __FreeBSD_version >= 1200000

size_t _mi_prim_numa_node(void) {
  domainset_t dom;
  size_t node;
  int policy;
  if (cpuset_getdomain(CPU_LEVEL_CPUSET, CPU_WHICH_PID, -1, sizeof(dom), &dom, &policy) == -1) return 0ul;
  for (node = 0; node < MAXMEMDOM; node++) {
    if (DOMAINSET_ISSET(node, &dom)) return node;
  }
  return 0ul;
}

size_t _mi_prim_numa_node_count(void) {
  size_t ndomains = 0;
  size_t len = sizeof(ndomains);
  if (sysctlbyname("vm.ndomains", &ndomains, &len, NULL, 0) == -1) return 0ul;
  return ndomains;
}

#elif defined(__DragonFly__)

size_t _mi_prim_numa_node(void) {
  // TODO: DragonFly does not seem to provide any userland means to get this information.
  return 0ul;
}

size_t _mi_prim_numa_node_count(void) {
  size_t ncpus = 0, nvirtcoresperphys = 0;
  size_t len = sizeof(size_t);
  if (sysctlbyname("hw.ncpu", &ncpus, &len, NULL, 0) == -1) return 0ul;
  if (sysctlbyname("hw.cpu_topology_ht_ids", &nvirtcoresperphys, &len, NULL, 0) == -1) return 0ul;
  return nvirtcoresperphys * ncpus;
}

#else

size_t _mi_prim_numa_node(void) {
  return 0;
}

size_t _mi_prim_numa_node_count(void) {
  return 1;
}

#endif // 16502

// ----------------------------------------------------------------
// Clock
// ----------------------------------------------------------------

#include <time.h>

#if defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC)

mi_msecs_t _mi_prim_clock_now(void) {
  struct timespec t;
  #ifdef CLOCK_MONOTONIC
  clock_gettime(CLOCK_MONOTONIC, &t);
  #else
  clock_gettime(CLOCK_REALTIME, &t);
  #endif // 16584
  return ((mi_msecs_t)t.tv_sec * 1000) + ((mi_msecs_t)t.tv_nsec / 1000000);
}

#else

// low resolution timer
mi_msecs_t _mi_prim_clock_now(void) {
  #if !defined(CLOCKS_PER_SEC) || (CLOCKS_PER_SEC == 1000) || (CLOCKS_PER_SEC == 0)
  return (mi_msecs_t)clock();
  #elif (CLOCKS_PER_SEC < 1000)
  return (mi_msecs_t)clock() * (1000 / (mi_msecs_t)CLOCKS_PER_SEC);
  #else
  return (mi_msecs_t)clock() / ((mi_msecs_t)CLOCKS_PER_SEC / 1000);
  #endif // 16596
}

#endif // 16580




//----------------------------------------------------------------
// Process info
//----------------------------------------------------------------

#if defined(__unix__) || defined(__unix) || defined(unix) || defined(__APPLE__) || defined(__HAIKU__)
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

#if defined(__APPLE__)
#include <mach/mach.h>
#endif // 16619

#if defined(__HAIKU__)
#include <kernel/OS.h>
#endif // 16623

static mi_msecs_t timeval_secs(const struct timeval* tv) {
  return ((mi_msecs_t)tv->tv_sec * 1000L) + ((mi_msecs_t)tv->tv_usec / 1000L);
}

void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);
  pinfo->utime = timeval_secs(&rusage.ru_utime);
  pinfo->stime = timeval_secs(&rusage.ru_stime);
#if !defined(__HAIKU__)
  pinfo->page_faults = rusage.ru_majflt;
#endif // 16637
#if defined(__HAIKU__)
  // Haiku does not have (yet?) a way to
  // get these stats per process
  thread_info tid;
  area_info mem;
  ssize_t c;
  get_thread_info(find_thread(0), &tid);
  while (get_next_area_info(tid.team, &c, &mem) == B_OK) {
    pinfo->peak_rss += mem.ram_size;
  }
  pinfo->page_faults = 0;
#elif defined(__APPLE__)
  pinfo->peak_rss = rusage.ru_maxrss;         // macos reports in bytes
  #ifdef MACH_TASK_BASIC_INFO
  struct mach_task_basic_info info;
  mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
    pinfo->current_rss = (size_t)info.resident_size;
  }
  #else
  struct task_basic_info info;
  mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
    pinfo->current_rss = (size_t)info.resident_size;
  }
  #endif // 16653
#else
  pinfo->peak_rss = rusage.ru_maxrss * 1024;  // Linux/BSD report in KiB
#endif // 16640
  // use defaults for commit
}

#else

#ifndef __wasi__
// WebAssembly instances are not processes
#pragma message("define a way to get process info")
#endif // 16674

void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  // use defaults
  MI_UNUSED(pinfo);
}

#endif // 16614


//----------------------------------------------------------------
// Output
//----------------------------------------------------------------

void _mi_prim_out_stderr( const char* msg ) {
  fputs(msg,stderr);
}


//----------------------------------------------------------------
// Environment
//----------------------------------------------------------------

#if !defined(MI_USE_ENVIRON) || (MI_USE_ENVIRON!=0)
// On Posix systemsr use `environ` to access environment variables
// even before the C runtime is initialized.
#if defined(__APPLE__) && defined(__has_include) && __has_include(<crt_externs.h>)
#include <crt_externs.h>
static char** mi_get_environ(void) {
  return (*_NSGetEnviron());
}
#else
extern char** environ;
static char** mi_get_environ(void) {
  return environ;
}
#endif // 16704
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  if (name==NULL) return false;
  const size_t len = _mi_strlen(name);
  if (len == 0) return false;
  char** env = mi_get_environ();
  if (env == NULL) return false;
  // compare up to 10000 entries
  for (int i = 0; i < 10000 && env[i] != NULL; i++) {
    const char* s = env[i];
    if (_mi_strnicmp(name, s, len) == 0 && s[len] == '=') { // case insensitive
      // found it
      _mi_strlcpy(result, s + len + 1, result_size);
      return true;
    }
  }
  return false;
}
#else
// fallback: use standard C `getenv` but this cannot be used while initializing the C runtime
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  // cannot call getenv() when still initializing the C runtime.
  if (_mi_preloading()) return false;
  const char* s = getenv(name);
  if (s == NULL) {
    // we check the upper case name too.
    char buf[64+1];
    size_t len = _mi_strnlen(name,sizeof(buf)-1);
    for (size_t i = 0; i < len; i++) {
      buf[i] = _mi_toupper(name[i]);
    }
    buf[len] = 0;
    s = getenv(buf);
  }
  if (s == NULL || _mi_strnlen(s,result_size) >= result_size)  return false;
  _mi_strlcpy(result, s, result_size);
  return true;
}
#endif  // !MI_USE_ENVIRON // 16701


//----------------------------------------------------------------
// Random
//----------------------------------------------------------------

#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_15) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_15)
#include <CommonCrypto/CommonCryptoError.h>
#include <CommonCrypto/CommonRandom.h>

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  // We prefere CCRandomGenerateBytes as it returns an error code while arc4random_buf
  // may fail silently on macOS. See PR #390, and <https://opensource.apple.com/source/Libc/Libc-1439.40.11/gen/FreeBSD/arc4random.c.auto.html>
  return (CCRandomGenerateBytes(buf, buf_len) == kCCSuccess);
}

#elif defined(__ANDROID__) || defined(__DragonFly__) || \
      defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
      defined(__sun) || \
      (defined(__APPLE__) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7))

#include <stdlib.h>
bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  arc4random_buf(buf, buf_len);
  return true;
}

#elif defined(__APPLE__) || defined(__linux__) || defined(__HAIKU__)   // also for old apple versions < 10.7 (issue #829)

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  // Modern Linux provides `getrandom` but different distributions either use `sys/random.h` or `linux/random.h`
  // and for the latter the actual `getrandom` call is not always defined.
  // (see <https://stackoverflow.com/questions/45237324/why-doesnt-getrandom-compile>)
  // We therefore use a syscall directly and fall back dynamically to /dev/urandom when needed.
  #if defined(MI_HAS_SYSCALL_H) && defined(SYS_getrandom)
    #ifndef GRND_NONBLOCK
    #define GRND_NONBLOCK (1)
    #endif // 16792
    static _Atomic(uintptr_t) no_getrandom; // = 0
    if (mi_atomic_load_acquire(&no_getrandom)==0) {
      ssize_t ret = syscall(SYS_getrandom, buf, buf_len, GRND_NONBLOCK);
      if (ret >= 0) return (buf_len == (size_t)ret);
      if (errno != ENOSYS) return false;
      mi_atomic_store_release(&no_getrandom, (uintptr_t)1); // don't call again, and fall back to /dev/urandom
    }
  #endif // 16791
  int flags = O_RDONLY;
  #if defined(O_CLOEXEC)
  flags |= O_CLOEXEC;
  #endif // 16804
  int fd = mi_prim_open("/dev/urandom", flags);
  if (fd < 0) return false;
  size_t count = 0;
  while(count < buf_len) {
    ssize_t ret = mi_prim_read(fd, (char*)buf + count, buf_len - count);
    if (ret<=0) {
      if (errno!=EAGAIN && errno!=EINTR) break;
    }
    else {
      count += ret;
    }
  }
  mi_prim_close(fd);
  return (count==buf_len);
}

#else

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  return false;
}

#endif // 16759


//----------------------------------------------------------------
// Thread init/done
//----------------------------------------------------------------

#if defined(MI_USE_PTHREADS)

// use pthread local storage keys to detect thread ending
// (and used with MI_TLS_PTHREADS for the default heap)
pthread_key_t _mi_heap_default_key = (pthread_key_t)(-1);

static void mi_pthread_done(void* value) {
  if (value!=NULL) {
    _mi_thread_done((mi_heap_t*)value);
  }
}

void _mi_prim_thread_init_auto_done(void) {
  mi_assert_internal(_mi_heap_default_key == (pthread_key_t)(-1));
  pthread_key_create(&_mi_heap_default_key, &mi_pthread_done);
}

void _mi_prim_thread_done_auto_done(void) {
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {  // do not leak the key, see issue #809
    pthread_key_delete(_mi_heap_default_key);
  }
}

void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {  // can happen during recursive invocation on freeBSD
    pthread_setspecific(_mi_heap_default_key, heap);
  }
}

#else

void _mi_prim_thread_init_auto_done(void) {
  // nothing
}

void _mi_prim_thread_done_auto_done(void) {
  // nothing
}

void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  MI_UNUSED(heap);
}

#endif // 16836
// --- Amalgamation END --- mimalloc/src/prim/osx/../unix/prim.c

// --- Amalgamation END --- mimalloc/src/prim/osx/prim.c


#elif defined(__wasi__)
#define MI_USE_SBRK

// --- Amalgamation START --- mimalloc/src/prim/wasi/prim.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// This file is included in `src/prim/prim.c`


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


#include <stdio.h>   // fputs
#include <stdlib.h>  // getenv

//---------------------------------------------
// Initialize
//---------------------------------------------

void _mi_prim_mem_init( mi_os_mem_config_t* config ) {
  config->page_size = 64*MI_KiB; // WebAssembly has a fixed page size: 64KiB
  config->alloc_granularity = 16;
  config->has_overcommit = false;
  config->has_partial_free = false;
  config->has_virtual_reserve = false;
}

//---------------------------------------------
// Free
//---------------------------------------------

int _mi_prim_free(void* addr, size_t size ) {
  MI_UNUSED(addr); MI_UNUSED(size);
  // wasi heap cannot be shrunk
  return 0;
}


//---------------------------------------------
// Allocation: sbrk or memory_grow
//---------------------------------------------

#if defined(MI_USE_SBRK)
  #include <unistd.h>  // for sbrk

  static void* mi_memory_grow( size_t size ) {
    void* p = sbrk(size);
    if (p == (void*)(-1)) return NULL;
    #if !defined(__wasi__) // on wasi this is always zero initialized already (?)
    memset(p,0,size);
    #endif // 16947
    return p;
  }
#elif defined(__wasi__)
  static void* mi_memory_grow( size_t size ) {
    size_t base = (size > 0 ? __builtin_wasm_memory_grow(0,_mi_divide_up(size, _mi_os_page_size()))
                            : __builtin_wasm_memory_size(0));
    if (base == SIZE_MAX) return NULL;
    return (void*)(base * _mi_os_page_size());
  }
#endif // 16941

#if defined(MI_USE_PTHREADS)
static pthread_mutex_t mi_heap_grow_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif // 16961

static void* mi_prim_mem_grow(size_t size, size_t try_alignment) {
  void* p = NULL;
  if (try_alignment <= 1) {
    // `sbrk` is not thread safe in general so try to protect it (we could skip this on WASM but leave it in for now)
    #if defined(MI_USE_PTHREADS)
    pthread_mutex_lock(&mi_heap_grow_mutex);
    #endif // 16969
    p = mi_memory_grow(size);
    #if defined(MI_USE_PTHREADS)
    pthread_mutex_unlock(&mi_heap_grow_mutex);
    #endif // 16973
  }
  else {
    void* base = NULL;
    size_t alloc_size = 0;
    // to allocate aligned use a lock to try to avoid thread interaction
    // between getting the current size and actual allocation
    // (also, `sbrk` is not thread safe in general)
    #if defined(MI_USE_PTHREADS)
    pthread_mutex_lock(&mi_heap_grow_mutex);
    #endif // 16983
    {
      void* current = mi_memory_grow(0);  // get current size
      if (current != NULL) {
        void* aligned_current = mi_align_up_ptr(current, try_alignment);  // and align from there to minimize wasted space
        alloc_size = _mi_align_up( ((uint8_t*)aligned_current - (uint8_t*)current) + size, _mi_os_page_size());
        base = mi_memory_grow(alloc_size);
      }
    }
    #if defined(MI_USE_PTHREADS)
    pthread_mutex_unlock(&mi_heap_grow_mutex);
    #endif // 16994
    if (base != NULL) {
      p = mi_align_up_ptr(base, try_alignment);
      if ((uint8_t*)p + size > (uint8_t*)base + alloc_size) {
        // another thread used wasm_memory_grow/sbrk in-between and we do not have enough
        // space after alignment. Give up (and waste the space as we cannot shrink :-( )
        // (in `mi_os_mem_alloc_aligned` this will fall back to overallocation to align)
        p = NULL;
      }
    }
  }
  /*
  if (p == NULL) {
    _mi_warning_message("unable to allocate sbrk/wasm_memory_grow OS memory (%zu bytes, %zu alignment)\n", size, try_alignment);
    errno = ENOMEM;
    return NULL;
  }
  */
  mi_assert_internal( p == NULL || try_alignment == 0 || (uintptr_t)p % try_alignment == 0 );
  return p;
}

// Note: the `try_alignment` is just a hint and the returned pointer is not guaranteed to be aligned.
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr) {
  MI_UNUSED(allow_large); MI_UNUSED(commit); MI_UNUSED(hint_addr);
  *is_large = false;
  *is_zero = false;
  *addr = mi_prim_mem_grow(size, try_alignment);
  return (*addr != NULL ? 0 : ENOMEM);
}


//---------------------------------------------
// Commit/Reset/Protect
//---------------------------------------------

int _mi_prim_commit(void* addr, size_t size, bool* is_zero) {
  MI_UNUSED(addr); MI_UNUSED(size);
  *is_zero = false;
  return 0;
}

int _mi_prim_decommit(void* addr, size_t size, bool* needs_recommit) {
  MI_UNUSED(addr); MI_UNUSED(size);
  *needs_recommit = false;
  return 0;
}

int _mi_prim_reset(void* addr, size_t size) {
  MI_UNUSED(addr); MI_UNUSED(size);
  return 0;
}

int _mi_prim_protect(void* addr, size_t size, bool protect) {
  MI_UNUSED(addr); MI_UNUSED(size); MI_UNUSED(protect);
  return 0;
}


//---------------------------------------------
// Huge pages and NUMA nodes
//---------------------------------------------

int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  MI_UNUSED(hint_addr); MI_UNUSED(size); MI_UNUSED(numa_node);
  *is_zero = true;
  *addr = NULL;
  return ENOSYS;
}

size_t _mi_prim_numa_node(void) {
  return 0;
}

size_t _mi_prim_numa_node_count(void) {
  return 1;
}


//----------------------------------------------------------------
// Clock
//----------------------------------------------------------------

#include <time.h>

#if defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC)

mi_msecs_t _mi_prim_clock_now(void) {
  struct timespec t;
  #ifdef CLOCK_MONOTONIC
  clock_gettime(CLOCK_MONOTONIC, &t);
  #else
  clock_gettime(CLOCK_REALTIME, &t);
  #endif // 17085
  return ((mi_msecs_t)t.tv_sec * 1000) + ((mi_msecs_t)t.tv_nsec / 1000000);
}

#else

// low resolution timer
mi_msecs_t _mi_prim_clock_now(void) {
  #if !defined(CLOCKS_PER_SEC) || (CLOCKS_PER_SEC == 1000) || (CLOCKS_PER_SEC == 0)
  return (mi_msecs_t)clock();
  #elif (CLOCKS_PER_SEC < 1000)
  return (mi_msecs_t)clock() * (1000 / (mi_msecs_t)CLOCKS_PER_SEC);
  #else
  return (mi_msecs_t)clock() / ((mi_msecs_t)CLOCKS_PER_SEC / 1000);
  #endif // 17097
}

#endif // 17081


//----------------------------------------------------------------
// Process info
//----------------------------------------------------------------

void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  // use defaults
  MI_UNUSED(pinfo);
}


//----------------------------------------------------------------
// Output
//----------------------------------------------------------------

void _mi_prim_out_stderr( const char* msg ) {
  fputs(msg,stderr);
}


//----------------------------------------------------------------
// Environment
//----------------------------------------------------------------

bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  // cannot call getenv() when still initializing the C runtime.
  if (_mi_preloading()) return false;
  const char* s = getenv(name);
  if (s == NULL) {
    // we check the upper case name too.
    char buf[64+1];
    size_t len = _mi_strnlen(name,sizeof(buf)-1);
    for (size_t i = 0; i < len; i++) {
      buf[i] = _mi_toupper(name[i]);
    }
    buf[len] = 0;
    s = getenv(buf);
  }
  if (s == NULL || _mi_strnlen(s,result_size) >= result_size)  return false;
  _mi_strlcpy(result, s, result_size);
  return true;
}


//----------------------------------------------------------------
// Random
//----------------------------------------------------------------

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  return false;
}


//----------------------------------------------------------------
// Thread init/done
//----------------------------------------------------------------

void _mi_prim_thread_init_auto_done(void) {
  // nothing
}

void _mi_prim_thread_done_auto_done(void) {
  // nothing
}

void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  MI_UNUSED(heap);
}
// --- Amalgamation END --- mimalloc/src/prim/wasi/prim.c


#elif defined(__EMSCRIPTEN__)

// --- Amalgamation START --- mimalloc/src/prim/emscripten/prim.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen, Alon Zakai
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// This file is included in `src/prim/prim.c`


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/atomic.h
// --- Amalgamation END --- mimalloc/include/mimalloc/atomic.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


// Design
// ======
//
// mimalloc is built on top of emmalloc. emmalloc is a minimal allocator on top
// of sbrk. The reason for having three layers here is that we want mimalloc to
// be able to allocate and release system memory properly, the same way it would
// when using VirtualAlloc on Windows or mmap on POSIX, and sbrk is too limited.
// Specifically, sbrk can only go up and down, and not "skip" over regions, and
// so we end up either never freeing memory to the system, or we can get stuck
// with holes.
//
// Atm wasm generally does *not* free memory back the system: once grown, we do
// not shrink back down (https://github.com/WebAssembly/design/issues/1397).
// However, that is expected to improve
// (https://github.com/WebAssembly/memory-control/blob/main/proposals/memory-control/Overview.md)
// and so we do not want to bake those limitations in here.
//
// Even without that issue, we want our system allocator to handle holes, that
// is, it should merge freed regions and allow allocating new content there of
// the full size, etc., so that we do not waste space. That means that the
// system allocator really does need to handle the general problem of allocating
// and freeing variable-sized chunks of memory in a random order, like malloc/
// free do. And so it makes sense to layer mimalloc on top of such an
// implementation.
//
// emmalloc makes sense for the lower level because it is small and simple while
// still fully handling merging of holes etc. It is not the most efficient
// allocator, but our assumption is that mimalloc needs to be fast while the
// system allocator underneath it is called much less frequently.
//

//---------------------------------------------
// init
//---------------------------------------------

void _mi_prim_mem_init( mi_os_mem_config_t* config) {
  config->page_size = 64*MI_KiB; // WebAssembly has a fixed page size: 64KiB
  config->alloc_granularity = 16;
  config->has_overcommit = false;
  config->has_partial_free = false;
  config->has_virtual_reserve = false;
}

extern void emmalloc_free(void*);

int _mi_prim_free(void* addr, size_t size) {
  MI_UNUSED(size);
  emmalloc_free(addr);
  return 0;
}


//---------------------------------------------
// Allocation
//---------------------------------------------

extern void* emmalloc_memalign(size_t alignment, size_t size);

// Note: the `try_alignment` is just a hint and the returned pointer is not guaranteed to be aligned.
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr) {
  MI_UNUSED(try_alignment); MI_UNUSED(allow_large); MI_UNUSED(commit); MI_UNUSED(hint_addr);
  *is_large = false;
  // TODO: Track the highest address ever seen; first uses of it are zeroes.
  //       That assumes no one else uses sbrk but us (they could go up,
  //       scribble, and then down), but we could assert on that perhaps.
  *is_zero = false;
  // emmalloc has a minimum alignment size.
  #define MIN_EMMALLOC_ALIGN           8
  if (try_alignment < MIN_EMMALLOC_ALIGN) {
    try_alignment = MIN_EMMALLOC_ALIGN;
  }
  void* p = emmalloc_memalign(try_alignment, size);
  *addr = p;
  if (p == 0) {
    return ENOMEM;
  }
  return 0;
}


//---------------------------------------------
// Commit/Reset
//---------------------------------------------

int _mi_prim_commit(void* addr, size_t size, bool* is_zero) {
  MI_UNUSED(addr); MI_UNUSED(size);
  // See TODO above.
  *is_zero = false;
  return 0;
}

int _mi_prim_decommit(void* addr, size_t size, bool* needs_recommit) {
  MI_UNUSED(addr); MI_UNUSED(size);
  *needs_recommit = false;
  return 0;
}

int _mi_prim_reset(void* addr, size_t size) {
  MI_UNUSED(addr); MI_UNUSED(size);
  return 0;
}

int _mi_prim_protect(void* addr, size_t size, bool protect) {
  MI_UNUSED(addr); MI_UNUSED(size); MI_UNUSED(protect);
  return 0;
}


//---------------------------------------------
// Huge pages and NUMA nodes
//---------------------------------------------

int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  MI_UNUSED(hint_addr); MI_UNUSED(size); MI_UNUSED(numa_node);
  *is_zero = true;
  *addr = NULL;
  return ENOSYS;
}

size_t _mi_prim_numa_node(void) {
  return 0;
}

size_t _mi_prim_numa_node_count(void) {
  return 1;
}


//----------------------------------------------------------------
// Clock
//----------------------------------------------------------------

#include <emscripten/html5.h>

mi_msecs_t _mi_prim_clock_now(void) {
  return emscripten_date_now();
}


//----------------------------------------------------------------
// Process info
//----------------------------------------------------------------

void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  // use defaults
  MI_UNUSED(pinfo);
}


//----------------------------------------------------------------
// Output
//----------------------------------------------------------------

#include <emscripten/console.h>

void _mi_prim_out_stderr( const char* msg) {
  emscripten_console_error(msg);
}


//----------------------------------------------------------------
// Environment
//----------------------------------------------------------------

bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  // For code size reasons, do not support environ customization for now.
  MI_UNUSED(name);
  MI_UNUSED(result);
  MI_UNUSED(result_size);
  return false;
}


//----------------------------------------------------------------
// Random
//----------------------------------------------------------------

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  int err = getentropy(buf, buf_len);
  return !err;
}


//----------------------------------------------------------------
// Thread init/done
//----------------------------------------------------------------

#if defined(MI_USE_PTHREADS)

// use pthread local storage keys to detect thread ending
// (and used with MI_TLS_PTHREADS for the default heap)
pthread_key_t _mi_heap_default_key = (pthread_key_t)(-1);

static void mi_pthread_done(void* value) {
  if (value!=NULL) {
    _mi_thread_done((mi_heap_t*)value);
  }
}

void _mi_prim_thread_init_auto_done(void) {
  mi_assert_internal(_mi_heap_default_key == (pthread_key_t)(-1));
  pthread_key_create(&_mi_heap_default_key, &mi_pthread_done);
}

void _mi_prim_thread_done_auto_done(void) {
  // nothing to do
}

void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {  // can happen during recursive invocation on freeBSD
    pthread_setspecific(_mi_heap_default_key, heap);
  }
}

#else

void _mi_prim_thread_init_auto_done(void) {
  // nothing
}

void _mi_prim_thread_done_auto_done(void) {
  // nothing
}

void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  MI_UNUSED(heap);

}
#endif // 17397
// --- Amalgamation END --- mimalloc/src/prim/emscripten/prim.c


#else

// --- Amalgamation START --- mimalloc/src/prim/unix/prim.c
/* ----------------------------------------------------------------------------
Copyright (c) 2018-2023, Microsoft Research, Daan Leijen
This is free software; you can redistribute it and/or modify it under the
terms of the MIT license. A copy of the license can be found in the file
"LICENSE" at the root of this distribution.
-----------------------------------------------------------------------------*/

// This file is included in `src/prim/prim.c`

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE   // ensure mmap flags and syscall are defined
#endif // 17454

#if 0 // #if defined(__sun)
// // illumos provides new mman.h api when any of these are defined
// // otherwise the old api based on caddr_t which predates the void pointers one.
// // stock solaris provides only the former, chose to atomically to discard those
// // flags only here rather than project wide tough.
// #undef _XOPEN_SOURCE
// #undef _POSIX_C_SOURCE
#endif // 17458


// --- Amalgamation START --- mimalloc/include/mimalloc.h
// --- Amalgamation END --- mimalloc/include/mimalloc.h


// --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// --- Amalgamation END --- mimalloc/include/mimalloc/internal.h


// --- Amalgamation START --- mimalloc/include/mimalloc/prim.h
// --- Amalgamation END --- mimalloc/include/mimalloc/prim.h


#include <sys/mman.h>  // mmap
#include <unistd.h>    // sysconf
#include <fcntl.h>     // open, close, read, access

#if defined(__linux__)
  #include <features.h>
  //#if defined(MI_NO_THP)
  #include <sys/prctl.h>  // THP disable
  //#endif
  #if defined(__GLIBC__)
  #include <linux/mman.h> // linux mmap flags
  #else
  #include <sys/mman.h>
  #endif // 17489
#elif defined(__APPLE__)
  #include <AvailabilityMacros.h>
  #include <TargetConditionals.h>
  #if !defined(TARGET_OS_OSX) || TARGET_OS_OSX   // see issue #879, used to be (!TARGET_IOS_IPHONE && !TARGET_IOS_SIMULATOR)
  #include <mach/vm_statistics.h>    // VM_MAKE_TAG, VM_FLAGS_SUPERPAGE_SIZE_2MB, etc.
  #endif // 17497
  #if !defined(MAC_OS_X_VERSION_10_7)
  #define MAC_OS_X_VERSION_10_7   1070
  #endif // 17500
#elif defined(__FreeBSD__) || defined(__DragonFly__)
  #include <sys/param.h>
  #if __FreeBSD_version >= 1200000
  #include <sys/cpuset.h>
  #include <sys/domainset.h>
  #endif // 17505
  #include <sys/sysctl.h>
#endif // 17484

#if defined(__linux__) || defined(__FreeBSD__)
  #define MI_HAS_SYSCALL_H
  #include <sys/syscall.h>
#endif // 17512


//------------------------------------------------------------------------------------
// Use syscalls for some primitives to allow for libraries that override open/read/close etc.
// and do allocation themselves; using syscalls prevents recursion when mimalloc is
// still initializing (issue #713)
// Declare inline to avoid unused function warnings.
//------------------------------------------------------------------------------------

#if defined(MI_HAS_SYSCALL_H) && defined(SYS_open) && defined(SYS_close) && defined(SYS_read) && defined(SYS_access)

static inline int mi_prim_open(const char* fpath, int open_flags) {
  return syscall(SYS_open,fpath,open_flags,0);
}
static inline ssize_t mi_prim_read(int fd, void* buf, size_t bufsize) {
  return syscall(SYS_read,fd,buf,bufsize);
}
static inline int mi_prim_close(int fd) {
  return syscall(SYS_close,fd);
}
static inline int mi_prim_access(const char *fpath, int mode) {
  return syscall(SYS_access,fpath,mode);
}

#else

static inline int mi_prim_open(const char* fpath, int open_flags) {
  return open(fpath,open_flags);
}
static inline ssize_t mi_prim_read(int fd, void* buf, size_t bufsize) {
  return read(fd,buf,bufsize);
}
static inline int mi_prim_close(int fd) {
  return close(fd);
}
static inline int mi_prim_access(const char *fpath, int mode) {
  return access(fpath,mode);
}

#endif // 17525



//---------------------------------------------
// init
//---------------------------------------------

static bool unix_detect_overcommit(void) {
  bool os_overcommit = true;
#if defined(__linux__)
  int fd = mi_prim_open("/proc/sys/vm/overcommit_memory", O_RDONLY);
	if (fd >= 0) {
    char buf[32];
    ssize_t nread = mi_prim_read(fd, &buf, sizeof(buf));
    mi_prim_close(fd);
    // <https://www.kernel.org/doc/Documentation/vm/overcommit-accounting>
    // 0: heuristic overcommit, 1: always overcommit, 2: never overcommit (ignore NORESERVE)
    if (nread >= 1) {
      os_overcommit = (buf[0] == '0' || buf[0] == '1');
    }
  }
#elif defined(__FreeBSD__)
  int val = 0;
  size_t olen = sizeof(val);
  if (sysctlbyname("vm.overcommit", &val, &olen, NULL, 0) == 0) {
    os_overcommit = (val != 0);
  }
#else
  // default: overcommit is true
#endif // 17565
  return os_overcommit;
}

void _mi_prim_mem_init( mi_os_mem_config_t* config )
{
  long psize = sysconf(_SC_PAGESIZE);
  if (psize > 0) {
    config->page_size = (size_t)psize;
    config->alloc_granularity = (size_t)psize;
    #if defined(_SC_PHYS_PAGES)
    long pphys = sysconf(_SC_PHYS_PAGES);
    if (pphys > 0 && (size_t)pphys < (SIZE_MAX/(size_t)psize)) {
      config->physical_memory = (size_t)pphys * (size_t)psize;
    }
    #endif // 17595
  }
  config->large_page_size = 2*MI_MiB; // TODO: can we query the OS for this?
  config->has_overcommit = unix_detect_overcommit();
  config->has_partial_free = true;    // mmap can free in parts
  config->has_virtual_reserve = true; // todo: check if this true for NetBSD?  (for anonymous mmap with PROT_NONE)

  // disable transparent huge pages for this process?
  #if (defined(__linux__) || defined(__ANDROID__)) && defined(PR_GET_THP_DISABLE)
  #if defined(MI_NO_THP)
  if (true)
  #else
  if (!mi_option_is_enabled(mi_option_allow_large_os_pages)) // disable THP also if large OS pages are not allowed in the options
  #endif // 17609
  {
    int val = 0;
    if (prctl(PR_GET_THP_DISABLE, &val, 0, 0, 0) != 0) {
      // Most likely since distros often come with always/madvise settings.
      val = 1;
      // Disabling only for mimalloc process rather than touching system wide settings
      (void)prctl(PR_SET_THP_DISABLE, &val, 0, 0, 0);
    }
  }
  #endif // 17608
}


//---------------------------------------------
// free
//---------------------------------------------

int _mi_prim_free(void* addr, size_t size ) {
  bool err = (munmap(addr, size) == -1);
  return (err ? errno : 0);
}


//---------------------------------------------
// mmap
//---------------------------------------------

static int unix_madvise(void* addr, size_t size, int advice) {
#if 0 //   #if defined(__sun)
//   int res = madvise((caddr_t)addr, size, advice);  // Solaris needs cast (issue #520)
  #else
  int res = madvise(addr, size, advice);
  #endif // 17642
  return (res==0 ? 0 : errno);
}

static void* unix_mmap_prim(void* addr, size_t size, size_t try_alignment, int protect_flags, int flags, int fd) {
  MI_UNUSED(try_alignment);
  void* p = NULL;
  #if defined(MAP_ALIGNED)  // BSD
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0) {
    size_t n = mi_bsr(try_alignment);
    if (((size_t)1 << n) == try_alignment && n >= 12 && n <= 30) {  // alignment is a power of 2 and 4096 <= alignment <= 1GiB
      p = mmap(addr, size, protect_flags, flags | MAP_ALIGNED(n), fd, 0);
      if (p==MAP_FAILED || !_mi_is_aligned(p,try_alignment)) {
        int err = errno;
        _mi_trace_message("unable to directly request aligned OS memory (error: %d (0x%x), size: 0x%zx bytes, alignment: 0x%zx, hint address: %p)\n", err, err, size, try_alignment, addr);
      }
      if (p!=MAP_FAILED) return p;
      // fall back to regular mmap
    }
  }
  #elif defined(MAP_ALIGN)  // Solaris
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0) {
    p = mmap((void*)try_alignment, size, protect_flags, flags | MAP_ALIGN, fd, 0);  // addr parameter is the required alignment
    if (p!=MAP_FAILED) return p;
    // fall back to regular mmap
  }
  #endif // 17653
  #if (MI_INTPTR_SIZE >= 8) && !defined(MAP_ALIGNED)
  // on 64-bit systems, use the virtual address area after 2TiB for 4MiB aligned allocations
  if (addr == NULL) {
    void* hint = _mi_os_get_aligned_hint(try_alignment, size);
    if (hint != NULL) {
      p = mmap(hint, size, protect_flags, flags, fd, 0);
      if (p==MAP_FAILED || !_mi_is_aligned(p,try_alignment)) {
        #if MI_TRACK_ENABLED  // asan sometimes does not instrument errno correctly?
        int err = 0;
        #else
        int err = errno;
        #endif // 17680
        _mi_trace_message("unable to directly request hinted aligned OS memory (error: %d (0x%x), size: 0x%zx bytes, alignment: 0x%zx, hint address: %p)\n", err, err, size, try_alignment, hint);
      }
      if (p!=MAP_FAILED) return p;
      // fall back to regular mmap
    }
  }
  #endif // 17673
  // regular mmap
  p = mmap(addr, size, protect_flags, flags, fd, 0);
  if (p!=MAP_FAILED) return p;
  // failed to allocate
  return NULL;
}

static int unix_mmap_fd(void) {
  #if defined(VM_MAKE_TAG)
  // macOS: tracking anonymous page with a specific ID. (All up to 98 are taken officially but LLVM sanitizers had taken 99)
  int os_tag = (int)mi_option_get(mi_option_os_tag);
  if (os_tag < 100 || os_tag > 255) { os_tag = 100; }
  return VM_MAKE_TAG(os_tag);
  #else
  return -1;
  #endif // 17700
}

static void* unix_mmap(void* addr, size_t size, size_t try_alignment, int protect_flags, bool large_only, bool allow_large, bool* is_large) {
  #if !defined(MAP_ANONYMOUS)
  #define MAP_ANONYMOUS  MAP_ANON
  #endif // 17711
  #if !defined(MAP_NORESERVE)
  #define MAP_NORESERVE  0
  #endif // 17714
  void* p = NULL;
  const int fd = unix_mmap_fd();
  int flags = MAP_PRIVATE | MAP_ANONYMOUS;
  if (_mi_os_has_overcommit()) {
    flags |= MAP_NORESERVE;
  }
  #if defined(PROT_MAX)
  protect_flags |= PROT_MAX(PROT_READ | PROT_WRITE); // BSD
  #endif // 17723
  // huge page allocation
  if ((large_only || _mi_os_use_large_page(size, try_alignment)) && allow_large) {
    static _Atomic(size_t) large_page_try_ok; // = 0;
    size_t try_ok = mi_atomic_load_acquire(&large_page_try_ok);
    if (!large_only && try_ok > 0) {
      // If the OS is not configured for large OS pages, or the user does not have
      // enough permission, the `mmap` will always fail (but it might also fail for other reasons).
      // Therefore, once a large page allocation failed, we don't try again for `large_page_try_ok` times
      // to avoid too many failing calls to mmap.
      mi_atomic_cas_strong_acq_rel(&large_page_try_ok, &try_ok, try_ok - 1);
    }
    else {
      int lflags = flags & ~MAP_NORESERVE;  // using NORESERVE on huge pages seems to fail on Linux
      int lfd = fd;
      #ifdef MAP_ALIGNED_SUPER
      lflags |= MAP_ALIGNED_SUPER;
      #endif // 17740
      #ifdef MAP_HUGETLB
      lflags |= MAP_HUGETLB;
      #endif // 17743
      #ifdef MAP_HUGE_1GB
      static bool mi_huge_pages_available = true;
      if ((size % MI_GiB) == 0 && mi_huge_pages_available) {
        lflags |= MAP_HUGE_1GB;
      }
      else
      #endif // 17746
      {
        #ifdef MAP_HUGE_2MB
        lflags |= MAP_HUGE_2MB;
        #endif // 17754
      }
      #ifdef VM_FLAGS_SUPERPAGE_SIZE_2MB
      lfd |= VM_FLAGS_SUPERPAGE_SIZE_2MB;
      #endif // 17758
      if (large_only || lflags != flags) {
        // try large OS page allocation
        *is_large = true;
        p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
        #ifdef MAP_HUGE_1GB
        if (p == NULL && (lflags & MAP_HUGE_1GB) == MAP_HUGE_1GB) {
          mi_huge_pages_available = false; // don't try huge 1GiB pages again
          _mi_warning_message("unable to allocate huge (1GiB) page, trying large (2MiB) pages instead (errno: %i)\n", errno);
          lflags = ((lflags & ~MAP_HUGE_1GB) | MAP_HUGE_2MB);
          p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
        }
        #endif // 17765
        if (large_only) return p;
        if (p == NULL) {
          mi_atomic_store_release(&large_page_try_ok, (size_t)8);  // on error, don't try again for the next N allocations
        }
      }
    }
  }
  // regular allocation
  if (p == NULL) {
    *is_large = false;
    p = unix_mmap_prim(addr, size, try_alignment, protect_flags, flags, fd);
    if (p != NULL) {
      #if defined(MADV_HUGEPAGE)
      // Many Linux systems don't allow MAP_HUGETLB but they support instead
      // transparent huge pages (THP). Generally, it is not required to call `madvise` with MADV_HUGE
      // though since properly aligned allocations will already use large pages if available
      // in that case -- in particular for our large regions (in `memory.c`).
      // However, some systems only allow THP if called with explicit `madvise`, so
      // when large OS pages are enabled for mimalloc, we call `madvise` anyways.
      if (allow_large && _mi_os_use_large_page(size, try_alignment)) {
        if (unix_madvise(p, size, MADV_HUGEPAGE) == 0) {
          // *is_large = true; // possibly
        };
      }
      #elif defined(__sun)
      if (allow_large && _mi_os_use_large_page(size, try_alignment)) {
        struct memcntl_mha cmd = {0};
        cmd.mha_pagesize = _mi_os_large_page_size();
        cmd.mha_cmd = MHA_MAPSIZE_VA;
        if (memcntl((caddr_t)p, size, MC_HAT_ADVISE, (caddr_t)&cmd, 0, 0) == 0) {
          // *is_large = true; // possibly
        }
      }
      #endif // 17785
    }
  }
  return p;
}

// Note: the `try_alignment` is just a hint and the returned pointer is not guaranteed to be aligned.
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr) {
  mi_assert_internal(size > 0 && (size % _mi_os_page_size()) == 0);
  mi_assert_internal(commit || !allow_large);
  mi_assert_internal(try_alignment > 0);

  *is_zero = true;
  int protect_flags = (commit ? (PROT_WRITE | PROT_READ) : PROT_NONE);
  *addr = unix_mmap(hint_addr, size, try_alignment, protect_flags, false, allow_large, is_large);
  return (*addr != NULL ? 0 : errno);
}


//---------------------------------------------
// Commit/Reset
//---------------------------------------------

static void unix_mprotect_hint(int err) {
  #if defined(__linux__) && (MI_SECURE>=2) // guard page around every mimalloc page
  if (err == ENOMEM) {
    _mi_warning_message("The next warning may be caused by a low memory map limit.\n"
                        "  On Linux this is controlled by the vm.max_map_count -- maybe increase it?\n"
                        "  For example: sudo sysctl -w vm.max_map_count=262144\n");
  }
  #else
  MI_UNUSED(err);
  #endif // 17830
}





int _mi_prim_commit(void* start, size_t size, bool* is_zero) {
  // commit: ensure we can access the area
  // note: we may think that *is_zero can be true since the memory
  // was either from mmap PROT_NONE, or from decommit MADV_DONTNEED, but
  // we sometimes call commit on a range with still partially committed
  // memory and `mprotect` does not zero the range.
  *is_zero = false;
  int err = mprotect(start, size, (PROT_READ | PROT_WRITE));
  if (err != 0) {
    err = errno;
    unix_mprotect_hint(err);
  }
  return err;
}

int _mi_prim_decommit(void* start, size_t size, bool* needs_recommit) {
  int err = 0;
  // decommit: use MADV_DONTNEED as it decreases rss immediately (unlike MADV_FREE)
  err = unix_madvise(start, size, MADV_DONTNEED);
  #if !MI_DEBUG && !MI_SECURE
    *needs_recommit = false;
  #else
    *needs_recommit = true;
    mprotect(start, size, PROT_NONE);
  #endif // 17864
  /*
  // decommit: use mmap with MAP_FIXED and PROT_NONE to discard the existing memory (and reduce rss)
  *needs_recommit = true;
  const int fd = unix_mmap_fd();
  void* p = mmap(start, size, PROT_NONE, (MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE), fd, 0);
  if (p != start) { err = errno; }
  */
  return err;
}

int _mi_prim_reset(void* start, size_t size) {
  // We try to use `MADV_FREE` as that is the fastest. A drawback though is that it
  // will not reduce the `rss` stats in tools like `top` even though the memory is available
  // to other processes. With the default `MIMALLOC_PURGE_DECOMMITS=1` we ensure that by
  // default `MADV_DONTNEED` is used though.
  #if defined(MADV_FREE)
  static _Atomic(size_t) advice = MI_ATOMIC_VAR_INIT(MADV_FREE);
  int oadvice = (int)mi_atomic_load_relaxed(&advice);
  int err;
  while ((err = unix_madvise(start, size, oadvice)) != 0 && errno == EAGAIN) { errno = 0;  };
  if (err != 0 && errno == EINVAL && oadvice == MADV_FREE) {
    // if MADV_FREE is not supported, fall back to MADV_DONTNEED from now on
    mi_atomic_store_release(&advice, (size_t)MADV_DONTNEED);
    err = unix_madvise(start, size, MADV_DONTNEED);
  }
  #else
  int err = unix_madvise(start, size, MADV_DONTNEED);
  #endif // 17885
  return err;
}

int _mi_prim_protect(void* start, size_t size, bool protect) {
  int err = mprotect(start, size, protect ? PROT_NONE : (PROT_READ | PROT_WRITE));
  if (err != 0) { err = errno; }
  unix_mprotect_hint(err);
  return err;
}



//---------------------------------------------
// Huge page allocation
//---------------------------------------------

#if (MI_INTPTR_SIZE >= 8) && !defined(__HAIKU__) && !defined(__CYGWIN__)

#ifndef MPOL_PREFERRED
#define MPOL_PREFERRED 1
#endif // 17916

#if defined(MI_HAS_SYSCALL_H) && defined(SYS_mbind)
static long mi_prim_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  return syscall(SYS_mbind, start, len, mode, nmask, maxnode, flags);
}
#else
static long mi_prim_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  MI_UNUSED(start); MI_UNUSED(len); MI_UNUSED(mode); MI_UNUSED(nmask); MI_UNUSED(maxnode); MI_UNUSED(flags);
  return 0;
}
#endif // 17920

int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  bool is_large = true;
  *is_zero = true;
  *addr = unix_mmap(hint_addr, size, MI_SEGMENT_SIZE, PROT_READ | PROT_WRITE, true, true, &is_large);
  if (*addr != NULL && numa_node >= 0 && numa_node < 8*MI_INTPTR_SIZE) { // at most 64 nodes
    unsigned long numa_mask = (1UL << numa_node);
    // TODO: does `mbind` work correctly for huge OS pages? should we
    // use `set_mempolicy` before calling mmap instead?
    // see: <https://lkml.org/lkml/2017/2/9/875>
    long err = mi_prim_mbind(*addr, size, MPOL_PREFERRED, &numa_mask, 8*MI_INTPTR_SIZE, 0);
    if (err != 0) {
      err = errno;
      _mi_warning_message("failed to bind huge (1GiB) pages to numa node %d (error: %d (0x%x))\n", numa_node, err, err);
    }
  }
  return (*addr != NULL ? 0 : errno);
}

#else

int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  MI_UNUSED(hint_addr); MI_UNUSED(size); MI_UNUSED(numa_node);
  *is_zero = false;
  *addr = NULL;
  return ENOMEM;
}

#endif // 17914

//---------------------------------------------
// NUMA nodes
//---------------------------------------------

#if defined(__linux__)

size_t _mi_prim_numa_node(void) {
  #if defined(MI_HAS_SYSCALL_H) && defined(SYS_getcpu)
    unsigned long node = 0;
    unsigned long ncpu = 0;
    long err = syscall(SYS_getcpu, &ncpu, &node, NULL);
    if (err != 0) return 0;
    return node;
  #else
    return 0;
  #endif // 17967
}

size_t _mi_prim_numa_node_count(void) {
  char buf[128];
  unsigned node = 0;
  for(node = 0; node < 256; node++) {
    // enumerate node entries -- todo: it there a more efficient way to do this? (but ensure there is no allocation)
    _mi_snprintf(buf, 127, "/sys/devices/system/node/node%u", node + 1);
    if (mi_prim_access(buf,R_OK) != 0) break;
  }
  return (node+1);
}

#elif defined(__FreeBSD__) && __FreeBSD_version >= 1200000

size_t _mi_prim_numa_node(void) {
  domainset_t dom;
  size_t node;
  int policy;
  if (cpuset_getdomain(CPU_LEVEL_CPUSET, CPU_WHICH_PID, -1, sizeof(dom), &dom, &policy) == -1) return 0ul;
  for (node = 0; node < MAXMEMDOM; node++) {
    if (DOMAINSET_ISSET(node, &dom)) return node;
  }
  return 0ul;
}

size_t _mi_prim_numa_node_count(void) {
  size_t ndomains = 0;
  size_t len = sizeof(ndomains);
  if (sysctlbyname("vm.ndomains", &ndomains, &len, NULL, 0) == -1) return 0ul;
  return ndomains;
}

#elif defined(__DragonFly__)

size_t _mi_prim_numa_node(void) {
  // TODO: DragonFly does not seem to provide any userland means to get this information.
  return 0ul;
}

size_t _mi_prim_numa_node_count(void) {
  size_t ncpus = 0, nvirtcoresperphys = 0;
  size_t len = sizeof(size_t);
  if (sysctlbyname("hw.ncpu", &ncpus, &len, NULL, 0) == -1) return 0ul;
  if (sysctlbyname("hw.cpu_topology_ht_ids", &nvirtcoresperphys, &len, NULL, 0) == -1) return 0ul;
  return nvirtcoresperphys * ncpus;
}

#else

size_t _mi_prim_numa_node(void) {
  return 0;
}

size_t _mi_prim_numa_node_count(void) {
  return 1;
}

#endif // 17964

// ----------------------------------------------------------------
// Clock
// ----------------------------------------------------------------

#include <time.h>

#if defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC)

mi_msecs_t _mi_prim_clock_now(void) {
  struct timespec t;
  #ifdef CLOCK_MONOTONIC
  clock_gettime(CLOCK_MONOTONIC, &t);
  #else
  clock_gettime(CLOCK_REALTIME, &t);
  #endif // 18046
  return ((mi_msecs_t)t.tv_sec * 1000) + ((mi_msecs_t)t.tv_nsec / 1000000);
}

#else

// low resolution timer
mi_msecs_t _mi_prim_clock_now(void) {
  #if !defined(CLOCKS_PER_SEC) || (CLOCKS_PER_SEC == 1000) || (CLOCKS_PER_SEC == 0)
  return (mi_msecs_t)clock();
  #elif (CLOCKS_PER_SEC < 1000)
  return (mi_msecs_t)clock() * (1000 / (mi_msecs_t)CLOCKS_PER_SEC);
  #else
  return (mi_msecs_t)clock() / ((mi_msecs_t)CLOCKS_PER_SEC / 1000);
  #endif // 18058
}

#endif // 18042




//----------------------------------------------------------------
// Process info
//----------------------------------------------------------------

#if defined(__unix__) || defined(__unix) || defined(unix) || defined(__APPLE__) || defined(__HAIKU__)
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

#if defined(__APPLE__)
#include <mach/mach.h>
#endif // 18081

#if defined(__HAIKU__)
#include <kernel/OS.h>
#endif // 18085

static mi_msecs_t timeval_secs(const struct timeval* tv) {
  return ((mi_msecs_t)tv->tv_sec * 1000L) + ((mi_msecs_t)tv->tv_usec / 1000L);
}

void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  struct rusage rusage;
  getrusage(RUSAGE_SELF, &rusage);
  pinfo->utime = timeval_secs(&rusage.ru_utime);
  pinfo->stime = timeval_secs(&rusage.ru_stime);
#if !defined(__HAIKU__)
  pinfo->page_faults = rusage.ru_majflt;
#endif // 18099
#if defined(__HAIKU__)
  // Haiku does not have (yet?) a way to
  // get these stats per process
  thread_info tid;
  area_info mem;
  ssize_t c;
  get_thread_info(find_thread(0), &tid);
  while (get_next_area_info(tid.team, &c, &mem) == B_OK) {
    pinfo->peak_rss += mem.ram_size;
  }
  pinfo->page_faults = 0;
#elif defined(__APPLE__)
  pinfo->peak_rss = rusage.ru_maxrss;         // macos reports in bytes
  #ifdef MACH_TASK_BASIC_INFO
  struct mach_task_basic_info info;
  mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
    pinfo->current_rss = (size_t)info.resident_size;
  }
  #else
  struct task_basic_info info;
  mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
  if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
    pinfo->current_rss = (size_t)info.resident_size;
  }
  #endif // 18115
#else
  pinfo->peak_rss = rusage.ru_maxrss * 1024;  // Linux/BSD report in KiB
#endif // 18102
  // use defaults for commit
}

#else

#ifndef __wasi__
// WebAssembly instances are not processes
#pragma message("define a way to get process info")
#endif // 18136

void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  // use defaults
  MI_UNUSED(pinfo);
}

#endif // 18076


//----------------------------------------------------------------
// Output
//----------------------------------------------------------------

void _mi_prim_out_stderr( const char* msg ) {
  fputs(msg,stderr);
}


//----------------------------------------------------------------
// Environment
//----------------------------------------------------------------

#if !defined(MI_USE_ENVIRON) || (MI_USE_ENVIRON!=0)
// On Posix systemsr use `environ` to access environment variables
// even before the C runtime is initialized.
#if defined(__APPLE__) && defined(__has_include) && __has_include(<crt_externs.h>)
#include <crt_externs.h>
static char** mi_get_environ(void) {
  return (*_NSGetEnviron());
}
#else
extern char** environ;
static char** mi_get_environ(void) {
  return environ;
}
#endif // 18166
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  if (name==NULL) return false;
  const size_t len = _mi_strlen(name);
  if (len == 0) return false;
  char** env = mi_get_environ();
  if (env == NULL) return false;
  // compare up to 10000 entries
  for (int i = 0; i < 10000 && env[i] != NULL; i++) {
    const char* s = env[i];
    if (_mi_strnicmp(name, s, len) == 0 && s[len] == '=') { // case insensitive
      // found it
      _mi_strlcpy(result, s + len + 1, result_size);
      return true;
    }
  }
  return false;
}
#else
// fallback: use standard C `getenv` but this cannot be used while initializing the C runtime
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  // cannot call getenv() when still initializing the C runtime.
  if (_mi_preloading()) return false;
  const char* s = getenv(name);
  if (s == NULL) {
    // we check the upper case name too.
    char buf[64+1];
    size_t len = _mi_strnlen(name,sizeof(buf)-1);
    for (size_t i = 0; i < len; i++) {
      buf[i] = _mi_toupper(name[i]);
    }
    buf[len] = 0;
    s = getenv(buf);
  }
  if (s == NULL || _mi_strnlen(s,result_size) >= result_size)  return false;
  _mi_strlcpy(result, s, result_size);
  return true;
}
#endif  // !MI_USE_ENVIRON // 18163


//----------------------------------------------------------------
// Random
//----------------------------------------------------------------

#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_15) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_15)
#include <CommonCrypto/CommonCryptoError.h>
#include <CommonCrypto/CommonRandom.h>

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  // We prefere CCRandomGenerateBytes as it returns an error code while arc4random_buf
  // may fail silently on macOS. See PR #390, and <https://opensource.apple.com/source/Libc/Libc-1439.40.11/gen/FreeBSD/arc4random.c.auto.html>
  return (CCRandomGenerateBytes(buf, buf_len) == kCCSuccess);
}

#elif defined(__ANDROID__) || defined(__DragonFly__) || \
      defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
      defined(__sun) || \
      (defined(__APPLE__) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7))

#include <stdlib.h>
bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  arc4random_buf(buf, buf_len);
  return true;
}

#elif defined(__APPLE__) || defined(__linux__) || defined(__HAIKU__)   // also for old apple versions < 10.7 (issue #829)

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  // Modern Linux provides `getrandom` but different distributions either use `sys/random.h` or `linux/random.h`
  // and for the latter the actual `getrandom` call is not always defined.
  // (see <https://stackoverflow.com/questions/45237324/why-doesnt-getrandom-compile>)
  // We therefore use a syscall directly and fall back dynamically to /dev/urandom when needed.
  #if defined(MI_HAS_SYSCALL_H) && defined(SYS_getrandom)
    #ifndef GRND_NONBLOCK
    #define GRND_NONBLOCK (1)
    #endif // 18254
    static _Atomic(uintptr_t) no_getrandom; // = 0
    if (mi_atomic_load_acquire(&no_getrandom)==0) {
      ssize_t ret = syscall(SYS_getrandom, buf, buf_len, GRND_NONBLOCK);
      if (ret >= 0) return (buf_len == (size_t)ret);
      if (errno != ENOSYS) return false;
      mi_atomic_store_release(&no_getrandom, (uintptr_t)1); // don't call again, and fall back to /dev/urandom
    }
  #endif // 18253
  int flags = O_RDONLY;
  #if defined(O_CLOEXEC)
  flags |= O_CLOEXEC;
  #endif // 18266
  int fd = mi_prim_open("/dev/urandom", flags);
  if (fd < 0) return false;
  size_t count = 0;
  while(count < buf_len) {
    ssize_t ret = mi_prim_read(fd, (char*)buf + count, buf_len - count);
    if (ret<=0) {
      if (errno!=EAGAIN && errno!=EINTR) break;
    }
    else {
      count += ret;
    }
  }
  mi_prim_close(fd);
  return (count==buf_len);
}

#else

bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  return false;
}

#endif // 18221


//----------------------------------------------------------------
// Thread init/done
//----------------------------------------------------------------

#if defined(MI_USE_PTHREADS)

// use pthread local storage keys to detect thread ending
// (and used with MI_TLS_PTHREADS for the default heap)
pthread_key_t _mi_heap_default_key = (pthread_key_t)(-1);

static void mi_pthread_done(void* value) {
  if (value!=NULL) {
    _mi_thread_done((mi_heap_t*)value);
  }
}

void _mi_prim_thread_init_auto_done(void) {
  mi_assert_internal(_mi_heap_default_key == (pthread_key_t)(-1));
  pthread_key_create(&_mi_heap_default_key, &mi_pthread_done);
}

void _mi_prim_thread_done_auto_done(void) {
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {  // do not leak the key, see issue #809
    pthread_key_delete(_mi_heap_default_key);
  }
}

void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {  // can happen during recursive invocation on freeBSD
    pthread_setspecific(_mi_heap_default_key, heap);
  }
}

#else

void _mi_prim_thread_init_auto_done(void) {
  // nothing
}

void _mi_prim_thread_done_auto_done(void) {
  // nothing
}

void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  MI_UNUSED(heap);
}

#endif // 18298
// --- Amalgamation END --- mimalloc/src/prim/unix/prim.c


#endif // 15148

// Generic process initialization
#ifndef MI_PRIM_HAS_PROCESS_ATTACH
#if defined(__GNUC__) || defined(__clang__)
  // gcc,clang: use the constructor/destructor attribute
  // which for both seem to run before regular constructors/destructors
  #if defined(__clang__)
    #define mi_attr_constructor __attribute__((constructor(101)))
    #define mi_attr_destructor  __attribute__((destructor(101)))
  #else
    #define mi_attr_constructor __attribute__((constructor))
    #define mi_attr_destructor  __attribute__((destructor))
  #endif // 18352
  static void mi_attr_constructor mi_process_attach(void) {
    _mi_process_load();
  }
  static void mi_attr_destructor mi_process_detach(void) {
    _mi_process_done();
  }
#elif 0 // #elif defined(__cplusplus)
//   // C++: use static initialization to detect process start/end
//   // This is not guaranteed to be first/last but the best we can generally do?
//   struct mi_init_done_t {
//     mi_init_done_t() {
//       _mi_process_load();
//     }
//     ~mi_init_done_t() {
//       _mi_process_done();
//     }
//   };
//   static mi_init_done_t mi_init_done;
 #else
  #pragma message("define a way to call _mi_process_load/done on your platform")
#endif // 18349
#endif // 18348

// Generic allocator init/done callback 
#ifndef MI_PRIM_HAS_ALLOCATOR_INIT
bool _mi_is_redirected(void) {
  return false;
}
bool _mi_allocator_init(const char** message) {
  if (message != NULL) { *message = NULL; }
  return true;
}
void _mi_allocator_done(void) {
  // nothing to do
}
#endif // 18383
// --- Amalgamation END --- mimalloc/src/prim/prim.c

#if 0 // #if MI_OSX_ZONE
// 
// // --- Amalgamation START --- mimalloc/src/prim/osx/alloc-override-zone.c
// /* ----------------------------------------------------------------------------
// Copyright (c) 2018-2022, Microsoft Research, Daan Leijen
// This is free software; you can redistribute it and/or modify it under the
// terms of the MIT license. A copy of the license can be found in the file
// "LICENSE" at the root of this distribution.
// -----------------------------------------------------------------------------*/
// 
// 
// // --- Amalgamation START --- mimalloc/include/mimalloc.h
// // --- Amalgamation END --- mimalloc/include/mimalloc.h
// 
// 
// // --- Amalgamation START --- mimalloc/include/mimalloc/internal.h
// // --- Amalgamation END --- mimalloc/include/mimalloc/internal.h
// 
// 
#if 0 // #if defined(MI_MALLOC_OVERRIDE)
// 
#if 0 // #if !defined(__APPLE__)
// #error "this file should only be included on macOS"
#endif // 18418
// 
// /* ------------------------------------------------------
//    Override system malloc on macOS
//    This is done through the malloc zone interface.
//    It seems to be most robust in combination with interposing
//    though or otherwise we may get zone errors as there are could
//    be allocations done by the time we take over the
//    zone.
// ------------------------------------------------------ */
// 
// #include <AvailabilityMacros.h>
// #include <malloc/malloc.h>
// #include <string.h>  // memset
// #include <stdlib.h>
// 
#if 0 // #ifdef __cplusplus
// extern "C" {
#endif // 18436
// 
#if 0 // #if defined(MAC_OS_X_VERSION_10_6) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6)
// // only available from OSX 10.6
// extern malloc_zone_t* malloc_default_purgeable_zone(void) __attribute__((weak_import));
#endif // 18440
// 
// /* ------------------------------------------------------
//    malloc zone members
// ------------------------------------------------------ */
// 
// static size_t zone_size(malloc_zone_t* zone, const void* p) {
//   MI_UNUSED(zone);
//   if (!mi_is_in_heap_region(p)){ return 0; } // not our pointer, bail out
//   return mi_usable_size(p);
// }
// 
// static void* zone_malloc(malloc_zone_t* zone, size_t size) {
//   MI_UNUSED(zone);
//   return mi_malloc(size);
// }
// 
// static void* zone_calloc(malloc_zone_t* zone, size_t count, size_t size) {
//   MI_UNUSED(zone);
//   return mi_calloc(count, size);
// }
// 
// static void* zone_valloc(malloc_zone_t* zone, size_t size) {
//   MI_UNUSED(zone);
//   return mi_malloc_aligned(size, _mi_os_page_size());
// }
// 
// static void zone_free(malloc_zone_t* zone, void* p) {
//   MI_UNUSED(zone);
//   mi_cfree(p);
// }
// 
// static void* zone_realloc(malloc_zone_t* zone, void* p, size_t newsize) {
//   MI_UNUSED(zone);
//   return mi_realloc(p, newsize);
// }
// 
// static void* zone_memalign(malloc_zone_t* zone, size_t alignment, size_t size) {
//   MI_UNUSED(zone);
//   return mi_malloc_aligned(size,alignment);
// }
// 
// static void zone_destroy(malloc_zone_t* zone) {
//   MI_UNUSED(zone);
//   // todo: ignore for now?
// }
// 
// static unsigned zone_batch_malloc(malloc_zone_t* zone, size_t size, void** ps, unsigned count) {
//   size_t i;
//   for (i = 0; i < count; i++) {
//     ps[i] = zone_malloc(zone, size);
//     if (ps[i] == NULL) break;
//   }
//   return i;
// }
// 
// static void zone_batch_free(malloc_zone_t* zone, void** ps, unsigned count) {
//   for(size_t i = 0; i < count; i++) {
//     zone_free(zone, ps[i]);
//     ps[i] = NULL;
//   }
// }
// 
// static size_t zone_pressure_relief(malloc_zone_t* zone, size_t size) {
//   MI_UNUSED(zone); MI_UNUSED(size);
//   mi_collect(false);
//   return 0;
// }
// 
// static void zone_free_definite_size(malloc_zone_t* zone, void* p, size_t size) {
//   MI_UNUSED(size);
//   zone_free(zone,p);
// }
// 
// static boolean_t zone_claimed_address(malloc_zone_t* zone, void* p) {
//   MI_UNUSED(zone);
//   return mi_is_in_heap_region(p);
// }
// 
// 
// /* ------------------------------------------------------
//    Introspection members
// ------------------------------------------------------ */
// 
// static kern_return_t intro_enumerator(task_t task, void* p,
//                             unsigned type_mask, vm_address_t zone_address,
//                             memory_reader_t reader,
//                             vm_range_recorder_t recorder)
// {
//   // todo: enumerate all memory
//   MI_UNUSED(task); MI_UNUSED(p); MI_UNUSED(type_mask); MI_UNUSED(zone_address);
//   MI_UNUSED(reader); MI_UNUSED(recorder);
//   return KERN_SUCCESS;
// }
// 
// static size_t intro_good_size(malloc_zone_t* zone, size_t size) {
//   MI_UNUSED(zone);
//   return mi_good_size(size);
// }
// 
// static boolean_t intro_check(malloc_zone_t* zone) {
//   MI_UNUSED(zone);
//   return true;
// }
// 
// static void intro_print(malloc_zone_t* zone, boolean_t verbose) {
//   MI_UNUSED(zone); MI_UNUSED(verbose);
//   mi_stats_print(NULL);
// }
// 
// static void intro_log(malloc_zone_t* zone, void* p) {
//   MI_UNUSED(zone); MI_UNUSED(p);
//   // todo?
// }
// 
// static void intro_force_lock(malloc_zone_t* zone) {
//   MI_UNUSED(zone);
//   // todo?
// }
// 
// static void intro_force_unlock(malloc_zone_t* zone) {
//   MI_UNUSED(zone);
//   // todo?
// }
// 
// static void intro_statistics(malloc_zone_t* zone, malloc_statistics_t* stats) {
//   MI_UNUSED(zone);
//   // todo...
//   stats->blocks_in_use = 0;
//   stats->size_in_use = 0;
//   stats->max_size_in_use = 0;
//   stats->size_allocated = 0;
// }
// 
// static boolean_t intro_zone_locked(malloc_zone_t* zone) {
//   MI_UNUSED(zone);
//   return false;
// }
// 
// 
// /* ------------------------------------------------------
//   At process start, override the default allocator
// ------------------------------------------------------ */
// 
#if 0 // #if defined(__GNUC__) && !defined(__clang__)
// #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif // 18587
// 
#if 0 // #if defined(__clang__)
// #pragma clang diagnostic ignored "-Wc99-extensions"
#endif // 18591
// 
// static malloc_introspection_t mi_introspect = {
//   .enumerator = &intro_enumerator,
//   .good_size = &intro_good_size,
//   .check = &intro_check,
//   .print = &intro_print,
//   .log = &intro_log,
//   .force_lock = &intro_force_lock,
//   .force_unlock = &intro_force_unlock,
#if 0 // #if defined(MAC_OS_X_VERSION_10_6) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6) && !defined(__ppc__)
//   .statistics = &intro_statistics,
//   .zone_locked = &intro_zone_locked,
#endif // 18603
// };
// 
// static malloc_zone_t mi_malloc_zone = {
//   // note: even with designators, the order is important for C++ compilation
//   //.reserved1 = NULL,
//   //.reserved2 = NULL,
//   .size = &zone_size,
//   .malloc = &zone_malloc,
//   .calloc = &zone_calloc,
//   .valloc = &zone_valloc,
//   .free = &zone_free,
//   .realloc = &zone_realloc,
//   .destroy = &zone_destroy,
//   .zone_name = "mimalloc",
//   .batch_malloc = &zone_batch_malloc,
//   .batch_free = &zone_batch_free,
//   .introspect = &mi_introspect,
#if 0 // #if defined(MAC_OS_X_VERSION_10_6) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6) && !defined(__ppc__)
#if 0 //   #if defined(MAC_OS_X_VERSION_10_14) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_14)
//   .version = 10,
  #else
//   .version = 9,
  #endif // 18625
//   // switch to version 9+ on OSX 10.6 to support memalign.
//   .memalign = &zone_memalign,
//   .free_definite_size = &zone_free_definite_size,
#if 0 //   #if defined(MAC_OS_X_VERSION_10_7) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_7)
//   .pressure_relief = &zone_pressure_relief,
  #endif // 18633
#if 0 //   #if defined(MAC_OS_X_VERSION_10_14) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_14)
//   .claimed_address = &zone_claimed_address,
  #endif // 18636
#else
//   .version = 4,
#endif // 18624
// };
// 
#if 0 // #ifdef __cplusplus
// }
#endif // 18644
// 
// 
#if 0 // #if defined(MI_OSX_INTERPOSE) && defined(MI_SHARED_LIB_EXPORT)
// 
// // ------------------------------------------------------
// // Override malloc_xxx and malloc_zone_xxx api's to use only
// // our mimalloc zone. Since even the loader uses malloc
// // on macOS, this ensures that all allocations go through
// // mimalloc (as all calls are interposed).
// // The main `malloc`, `free`, etc calls are interposed in `alloc-override.c`,
// // Here, we also override macOS specific API's like
// // `malloc_zone_calloc` etc. see <https://github.com/aosm/libmalloc/blob/master/man/malloc_zone_malloc.3>
// // ------------------------------------------------------
// 
// static inline malloc_zone_t* mi_get_default_zone(void)
// {
//   static bool init;
//   if mi_unlikely(!init) {
//     init = true;
//     malloc_zone_register(&mi_malloc_zone);  // by calling register we avoid a zone error on free (see <http://eatmyrandom.blogspot.com/2010/03/mallocfree-interception-on-mac-os-x.html>)
//   }
//   return &mi_malloc_zone;
// }
// 
// mi_decl_externc int  malloc_jumpstart(uintptr_t cookie);
// mi_decl_externc void _malloc_fork_prepare(void);
// mi_decl_externc void _malloc_fork_parent(void);
// mi_decl_externc void _malloc_fork_child(void);
// 
// 
// static malloc_zone_t* mi_malloc_create_zone(vm_size_t size, unsigned flags) {
//   MI_UNUSED(size); MI_UNUSED(flags);
//   return mi_get_default_zone();
// }
// 
// static malloc_zone_t* mi_malloc_default_zone (void) {
//   return mi_get_default_zone();
// }
// 
// static malloc_zone_t* mi_malloc_default_purgeable_zone(void) {
//   return mi_get_default_zone();
// }
// 
// static void mi_malloc_destroy_zone(malloc_zone_t* zone) {
//   MI_UNUSED(zone);
//   // nothing.
// }
// 
// static kern_return_t mi_malloc_get_all_zones (task_t task, memory_reader_t mr, vm_address_t** addresses, unsigned* count) {
//   MI_UNUSED(task); MI_UNUSED(mr);
//   if (addresses != NULL) *addresses = NULL;
//   if (count != NULL) *count = 0;
//   return KERN_SUCCESS;
// }
// 
// static const char* mi_malloc_get_zone_name(malloc_zone_t* zone) {
//   return (zone == NULL ? mi_malloc_zone.zone_name : zone->zone_name);
// }
// 
// static void mi_malloc_set_zone_name(malloc_zone_t* zone, const char* name) {
//   MI_UNUSED(zone); MI_UNUSED(name);
// }
// 
// static int mi_malloc_jumpstart(uintptr_t cookie) {
//   MI_UNUSED(cookie);
//   return 1; // or 0 for no error?
// }
// 
// static void mi__malloc_fork_prepare(void) {
//   // nothing
// }
// static void mi__malloc_fork_parent(void) {
//   // nothing
// }
// static void mi__malloc_fork_child(void) {
//   // nothing
// }
// 
// static void mi_malloc_printf(const char* fmt, ...) {
//   MI_UNUSED(fmt);
// }
// 
// static bool zone_check(malloc_zone_t* zone) {
//   MI_UNUSED(zone);
//   return true;
// }
// 
// static malloc_zone_t* zone_from_ptr(const void* p) {
//   MI_UNUSED(p);
//   return mi_get_default_zone();
// }
// 
// static void zone_log(malloc_zone_t* zone, void* p) {
//   MI_UNUSED(zone); MI_UNUSED(p);
// }
// 
// static void zone_print(malloc_zone_t* zone, bool b) {
//   MI_UNUSED(zone); MI_UNUSED(b);
// }
// 
// static void zone_print_ptr_info(void* p) {
//   MI_UNUSED(p);
// }
// 
// static void zone_register(malloc_zone_t* zone) {
//   MI_UNUSED(zone);
// }
// 
// static void zone_unregister(malloc_zone_t* zone) {
//   MI_UNUSED(zone);
// }
// 
// // use interposing so `DYLD_INSERT_LIBRARIES` works without `DYLD_FORCE_FLAT_NAMESPACE=1`
// // See: <https://books.google.com/books?id=K8vUkpOXhN4C&pg=PA73>
// struct mi_interpose_s {
//   const void* replacement;
//   const void* target;
// };
// #define MI_INTERPOSE_FUN(oldfun,newfun) { (const void*)&newfun, (const void*)&oldfun }
// #define MI_INTERPOSE_MI(fun)            MI_INTERPOSE_FUN(fun,mi_##fun)
// #define MI_INTERPOSE_ZONE(fun)          MI_INTERPOSE_FUN(malloc_##fun,fun)
// __attribute__((used)) static const struct mi_interpose_s _mi_zone_interposes[]  __attribute__((section("__DATA, __interpose"))) =
// {
// 
//   MI_INTERPOSE_MI(malloc_create_zone),
//   MI_INTERPOSE_MI(malloc_default_purgeable_zone),
//   MI_INTERPOSE_MI(malloc_default_zone),
//   MI_INTERPOSE_MI(malloc_destroy_zone),
//   MI_INTERPOSE_MI(malloc_get_all_zones),
//   MI_INTERPOSE_MI(malloc_get_zone_name),
//   MI_INTERPOSE_MI(malloc_jumpstart),
//   MI_INTERPOSE_MI(malloc_printf),
//   MI_INTERPOSE_MI(malloc_set_zone_name),
//   MI_INTERPOSE_MI(_malloc_fork_child),
//   MI_INTERPOSE_MI(_malloc_fork_parent),
//   MI_INTERPOSE_MI(_malloc_fork_prepare),
// 
//   MI_INTERPOSE_ZONE(zone_batch_free),
//   MI_INTERPOSE_ZONE(zone_batch_malloc),
//   MI_INTERPOSE_ZONE(zone_calloc),
//   MI_INTERPOSE_ZONE(zone_check),
//   MI_INTERPOSE_ZONE(zone_free),
//   MI_INTERPOSE_ZONE(zone_from_ptr),
//   MI_INTERPOSE_ZONE(zone_log),
//   MI_INTERPOSE_ZONE(zone_malloc),
//   MI_INTERPOSE_ZONE(zone_memalign),
//   MI_INTERPOSE_ZONE(zone_print),
//   MI_INTERPOSE_ZONE(zone_print_ptr_info),
//   MI_INTERPOSE_ZONE(zone_realloc),
//   MI_INTERPOSE_ZONE(zone_register),
//   MI_INTERPOSE_ZONE(zone_unregister),
//   MI_INTERPOSE_ZONE(zone_valloc)
// };
// 
// 
#else
// 
// // ------------------------------------------------------
// // hook into the zone api's without interposing
// // This is the official way of adding an allocator but
// // it seems less robust than using interpose.
// // ------------------------------------------------------
// 
// static inline malloc_zone_t* mi_get_default_zone(void)
// {
//   // The first returned zone is the real default
//   malloc_zone_t** zones = NULL;
//   unsigned count = 0;
//   kern_return_t ret = malloc_get_all_zones(0, NULL, (vm_address_t**)&zones, &count);
//   if (ret == KERN_SUCCESS && count > 0) {
//     return zones[0];
//   }
//   else {
//     // fallback
//     return malloc_default_zone();
//   }
// }
// 
#if 0 // #if defined(__clang__)
// __attribute__((constructor(101))) // highest priority
#else
// __attribute__((constructor))      // priority level is not supported by gcc
#endif // 18825
// __attribute__((used))
// static void _mi_macos_override_malloc(void) {
//   malloc_zone_t* purgeable_zone = NULL;
// 
#if 0 //   #if defined(MAC_OS_X_VERSION_10_6) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6)
//   // force the purgeable zone to exist to avoid strange bugs
//   if (malloc_default_purgeable_zone) {
//     purgeable_zone = malloc_default_purgeable_zone();
//   }
  #endif // 18834
// 
//   // Register our zone.
//   // thomcc: I think this is still needed to put us in the zone list.
//   malloc_zone_register(&mi_malloc_zone);
//   // Unregister the default zone, this makes our zone the new default
//   // as that was the last registered.
//   malloc_zone_t *default_zone = mi_get_default_zone();
//   // thomcc: Unsure if the next test is *always* false or just false in the
//   // cases I've tried. I'm also unsure if the code inside is needed. at all
//   if (default_zone != &mi_malloc_zone) {
//     malloc_zone_unregister(default_zone);
// 
//     // Reregister the default zone so free and realloc in that zone keep working.
//     malloc_zone_register(default_zone);
//   }
// 
//   // Unregister, and re-register the purgeable_zone to avoid bugs if it occurs
//   // earlier than the default zone.
//   if (purgeable_zone != NULL) {
//     malloc_zone_unregister(purgeable_zone);
//     malloc_zone_register(purgeable_zone);
//   }
// 
// }
#endif  // MI_OSX_INTERPOSE // 18649
// 
#endif // MI_MALLOC_OVERRIDE // 18416
// // --- Amalgamation END --- mimalloc/src/prim/osx/alloc-override-zone.c
// 
#endif // 18397
