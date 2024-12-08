#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
#if 0
#endif
#define MI_MALLOC_VERSION 188
#if 0
#if 0
  #else
  #endif
#else
  #define mi_attr_noexcept
#endif
#if 0
#elif (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__)
  #define mi_decl_nodiscard __attribute__((warn_unused_result))
#elif defined(_HAS_NODISCARD)
  #define mi_decl_nodiscard _NODISCARD
#elif 0
#else
  #define mi_decl_nodiscard
#endif
#if 0
#if 0
#elif 0
  #else
  #endif
#if 0
  #else
#if 0
    #else
    #endif
  #endif
#elif defined(__GNUC__)
  #if defined(MI_SHARED_LIB) && defined(MI_SHARED_LIB_EXPORT)
    #define mi_decl_export __attribute__((visibility("default")))
  #else
    #define mi_decl_export
  #endif
  #define mi_cdecl
  #define mi_decl_restrict
  #define mi_attr_malloc __attribute__((malloc))
  #if (defined(__clang_major__) && (__clang_major__ < 4)) || (__GNUC__ < 5)
    #define mi_attr_alloc_size(s)
    #define mi_attr_alloc_size2(s1,s2)
    #define mi_attr_alloc_align(p)
  #elif defined(__INTEL_COMPILER)
    #define mi_attr_alloc_size(s) __attribute__((alloc_size(s)))
    #define mi_attr_alloc_size2(s1,s2) __attribute__((alloc_size(s1,s2)))
    #define mi_attr_alloc_align(p)
  #else
    #define mi_attr_alloc_size(s) __attribute__((alloc_size(s)))
    #define mi_attr_alloc_size2(s1,s2) __attribute__((alloc_size(s1,s2)))
    #define mi_attr_alloc_align(p) __attribute__((alloc_align(p)))
  #endif
#else
  #define mi_cdecl
  #define mi_decl_export
  #define mi_decl_restrict
  #define mi_attr_malloc
  #define mi_attr_alloc_size(s)
  #define mi_attr_alloc_size2(s1,s2)
  #define mi_attr_alloc_align(p)
#endif
#include <stddef.h>
#include <stdbool.h>
#if 0
#endif
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_malloc(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_calloc(size_t count, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(1,2);
mi_decl_nodiscard mi_decl_export void* mi_realloc(void* p, size_t newsize) mi_attr_noexcept mi_attr_alloc_size(2);
mi_decl_export void* mi_expand(void* p, size_t newsize) mi_attr_noexcept mi_attr_alloc_size(2);
mi_decl_export void mi_free(void* p) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_strdup(const char* s) mi_attr_noexcept mi_attr_malloc;
mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_strndup(const char* s, size_t n) mi_attr_noexcept mi_attr_malloc;
mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_realpath(const char* fname, char* resolved_name) mi_attr_noexcept mi_attr_malloc;
#define MI_SMALL_WSIZE_MAX (128)
#define MI_SMALL_SIZE_MAX (MI_SMALL_WSIZE_MAX*sizeof(void*))
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_malloc_small(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_zalloc_small(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_zalloc(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_mallocn(size_t count, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(1,2);
mi_decl_nodiscard mi_decl_export void* mi_reallocn(void* p, size_t count, size_t size) mi_attr_noexcept mi_attr_alloc_size2(2,3);
mi_decl_nodiscard mi_decl_export void* mi_reallocf(void* p, size_t newsize) mi_attr_noexcept mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export size_t mi_usable_size(const void* p) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export size_t mi_good_size(size_t size) mi_attr_noexcept;
typedef void (mi_cdecl mi_deferred_free_fun)(bool force, unsigned long long heartbeat, void* arg);
mi_decl_export void mi_register_deferred_free(mi_deferred_free_fun* deferred_free, void* arg) mi_attr_noexcept;
typedef void (mi_cdecl mi_output_fun)(const char* msg, void* arg);
mi_decl_export void mi_register_output(mi_output_fun* out, void* arg) mi_attr_noexcept;
typedef void (mi_cdecl mi_error_fun)(int err, void* arg);
mi_decl_export void mi_register_error(mi_error_fun* fun, void* arg);
mi_decl_export void mi_collect(bool force) mi_attr_noexcept;
mi_decl_export void mi_collect_reduce(size_t target_thread_owned) mi_attr_noexcept;
mi_decl_export int mi_version(void) mi_attr_noexcept;
mi_decl_export void mi_stats_reset(void) mi_attr_noexcept;
mi_decl_export void mi_stats_merge(void) mi_attr_noexcept;
mi_decl_export void mi_stats_print(void* out) mi_attr_noexcept;
mi_decl_export void mi_stats_print_out(mi_output_fun* out, void* arg) mi_attr_noexcept;
mi_decl_export void mi_process_init(void) mi_attr_noexcept;
mi_decl_export void mi_thread_init(void) mi_attr_noexcept;
mi_decl_export void mi_thread_done(void) mi_attr_noexcept;
mi_decl_export void mi_thread_stats_print_out(mi_output_fun* out, void* arg) mi_attr_noexcept;
mi_decl_export void mi_process_info(size_t* elapsed_msecs, size_t* user_msecs, size_t* system_msecs,
                                    size_t* current_rss, size_t* peak_rss,
                                    size_t* current_commit, size_t* peak_commit, size_t* page_faults) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_malloc_aligned(size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_malloc_aligned_at(size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_zalloc_aligned(size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_zalloc_aligned_at(size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_calloc_aligned(size_t count, size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(1,2) mi_attr_alloc_align(3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_calloc_aligned_at(size_t count, size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(1,2);
mi_decl_nodiscard mi_decl_export void* mi_realloc_aligned(void* p, size_t newsize, size_t alignment) mi_attr_noexcept mi_attr_alloc_size(2) mi_attr_alloc_align(3);
mi_decl_nodiscard mi_decl_export void* mi_realloc_aligned_at(void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size(2);
struct mi_heap_s;
typedef struct mi_heap_s mi_heap_t;
mi_decl_nodiscard mi_decl_export mi_heap_t* mi_heap_new(void);
mi_decl_export void mi_heap_delete(mi_heap_t* heap);
mi_decl_export void mi_heap_destroy(mi_heap_t* heap);
mi_decl_export mi_heap_t* mi_heap_set_default(mi_heap_t* heap);
mi_decl_export mi_heap_t* mi_heap_get_default(void);
mi_decl_export mi_heap_t* mi_heap_get_backing(void);
mi_decl_export void mi_heap_collect(mi_heap_t* heap, bool force) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_malloc(mi_heap_t* heap, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_zalloc(mi_heap_t* heap, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_calloc(mi_heap_t* heap, size_t count, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(2, 3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_mallocn(mi_heap_t* heap, size_t count, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size2(2, 3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_malloc_small(mi_heap_t* heap, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export void* mi_heap_realloc(mi_heap_t* heap, void* p, size_t newsize) mi_attr_noexcept mi_attr_alloc_size(3);
mi_decl_nodiscard mi_decl_export void* mi_heap_reallocn(mi_heap_t* heap, void* p, size_t count, size_t size) mi_attr_noexcept mi_attr_alloc_size2(3,4);
mi_decl_nodiscard mi_decl_export void* mi_heap_reallocf(mi_heap_t* heap, void* p, size_t newsize) mi_attr_noexcept mi_attr_alloc_size(3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict char* mi_heap_strdup(mi_heap_t* heap, const char* s) mi_attr_noexcept mi_attr_malloc;
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
mi_decl_nodiscard mi_decl_export void* mi_rezalloc(void* p, size_t newsize) mi_attr_noexcept mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export void* mi_recalloc(void* p, size_t newcount, size_t size) mi_attr_noexcept mi_attr_alloc_size2(2,3);
mi_decl_nodiscard mi_decl_export void* mi_rezalloc_aligned(void* p, size_t newsize, size_t alignment) mi_attr_noexcept mi_attr_alloc_size(2) mi_attr_alloc_align(3);
mi_decl_nodiscard mi_decl_export void* mi_rezalloc_aligned_at(void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export void* mi_recalloc_aligned(void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept mi_attr_alloc_size2(2,3) mi_attr_alloc_align(4);
mi_decl_nodiscard mi_decl_export void* mi_recalloc_aligned_at(void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size2(2,3);
mi_decl_nodiscard mi_decl_export void* mi_heap_rezalloc(mi_heap_t* heap, void* p, size_t newsize) mi_attr_noexcept mi_attr_alloc_size(3);
mi_decl_nodiscard mi_decl_export void* mi_heap_recalloc(mi_heap_t* heap, void* p, size_t newcount, size_t size) mi_attr_noexcept mi_attr_alloc_size2(3,4);
mi_decl_nodiscard mi_decl_export void* mi_heap_rezalloc_aligned(mi_heap_t* heap, void* p, size_t newsize, size_t alignment) mi_attr_noexcept mi_attr_alloc_size(3) mi_attr_alloc_align(4);
mi_decl_nodiscard mi_decl_export void* mi_heap_rezalloc_aligned_at(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size(3);
mi_decl_nodiscard mi_decl_export void* mi_heap_recalloc_aligned(mi_heap_t* heap, void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept mi_attr_alloc_size2(3,4) mi_attr_alloc_align(5);
mi_decl_nodiscard mi_decl_export void* mi_heap_recalloc_aligned_at(mi_heap_t* heap, void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept mi_attr_alloc_size2(3,4);
mi_decl_export bool mi_heap_contains_block(mi_heap_t* heap, const void* p);
mi_decl_export bool mi_heap_check_owned(mi_heap_t* heap, const void* p);
mi_decl_export bool mi_check_owned(const void* p);
typedef struct mi_heap_area_s {
  void* blocks;
  size_t reserved;
  size_t committed;
  size_t used;
  size_t block_size;
  size_t full_block_size;
  int heap_tag;
} mi_heap_area_t;
typedef bool (mi_cdecl mi_block_visit_fun)(const mi_heap_t* heap, const mi_heap_area_t* area, void* block, size_t block_size, void* arg);
mi_decl_export bool mi_heap_visit_blocks(const mi_heap_t* heap, bool visit_blocks, mi_block_visit_fun* visitor, void* arg);
mi_decl_nodiscard mi_decl_export bool mi_is_in_heap_region(const void* p) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export bool mi_is_redirected(void) mi_attr_noexcept;
mi_decl_export int mi_reserve_huge_os_pages_interleave(size_t pages, size_t numa_nodes, size_t timeout_msecs) mi_attr_noexcept;
mi_decl_export int mi_reserve_huge_os_pages_at(size_t pages, int numa_node, size_t timeout_msecs) mi_attr_noexcept;
mi_decl_export int mi_reserve_os_memory(size_t size, bool commit, bool allow_large) mi_attr_noexcept;
mi_decl_export bool mi_manage_os_memory(void* start, size_t size, bool is_committed, bool is_large, bool is_zero, int numa_node) mi_attr_noexcept;
mi_decl_export void mi_debug_show_arenas(bool show_inuse, bool show_abandoned, bool show_purge) mi_attr_noexcept;
typedef int mi_arena_id_t;
mi_decl_export void* mi_arena_area(mi_arena_id_t arena_id, size_t* size);
mi_decl_export int mi_reserve_huge_os_pages_at_ex(size_t pages, int numa_node, size_t timeout_msecs, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept;
mi_decl_export int mi_reserve_os_memory_ex(size_t size, bool commit, bool allow_large, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept;
mi_decl_export bool mi_manage_os_memory_ex(void* start, size_t size, bool is_committed, bool is_large, bool is_zero, int numa_node, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept;
#if 0
#endif
typedef void* mi_subproc_id_t;
mi_decl_export mi_subproc_id_t mi_subproc_main(void);
mi_decl_export mi_subproc_id_t mi_subproc_new(void);
mi_decl_export void mi_subproc_delete(mi_subproc_id_t subproc);
mi_decl_export void mi_subproc_add_current_thread(mi_subproc_id_t subproc);
mi_decl_export bool mi_abandoned_visit_blocks(mi_subproc_id_t subproc_id, int heap_tag, bool visit_blocks, mi_block_visit_fun* visitor, void* arg);
mi_decl_nodiscard mi_decl_export mi_heap_t* mi_heap_new_ex(int heap_tag, bool allow_destroy, mi_arena_id_t arena_id);
mi_decl_export int mi_reserve_huge_os_pages(size_t pages, double max_secs, size_t* pages_reserved) mi_attr_noexcept;
mi_decl_export void mi_heap_guarded_set_sample_rate(mi_heap_t* heap, size_t sample_rate, size_t seed);
mi_decl_export void mi_heap_guarded_set_size_bound(mi_heap_t* heap, size_t min, size_t max);
#define mi_malloc_tp(tp) ((tp*)mi_malloc(sizeof(tp)))
#define mi_zalloc_tp(tp) ((tp*)mi_zalloc(sizeof(tp)))
#define mi_calloc_tp(tp,n) ((tp*)mi_calloc(n,sizeof(tp)))
#define mi_mallocn_tp(tp,n) ((tp*)mi_mallocn(n,sizeof(tp)))
#define mi_reallocn_tp(p,tp,n) ((tp*)mi_reallocn(p,n,sizeof(tp)))
#define mi_recalloc_tp(p,tp,n) ((tp*)mi_recalloc(p,n,sizeof(tp)))
#define mi_heap_malloc_tp(hp,tp) ((tp*)mi_heap_malloc(hp,sizeof(tp)))
#define mi_heap_zalloc_tp(hp,tp) ((tp*)mi_heap_zalloc(hp,sizeof(tp)))
#define mi_heap_calloc_tp(hp,tp,n) ((tp*)mi_heap_calloc(hp,n,sizeof(tp)))
#define mi_heap_mallocn_tp(hp,tp,n) ((tp*)mi_heap_mallocn(hp,n,sizeof(tp)))
#define mi_heap_reallocn_tp(hp,p,tp,n) ((tp*)mi_heap_reallocn(hp,p,n,sizeof(tp)))
#define mi_heap_recalloc_tp(hp,p,tp,n) ((tp*)mi_heap_recalloc(hp,p,n,sizeof(tp)))
typedef enum mi_option_e {
  mi_option_show_errors,
  mi_option_show_stats,
  mi_option_verbose,
  mi_option_eager_commit,
  mi_option_arena_eager_commit,
  mi_option_purge_decommits,
  mi_option_allow_large_os_pages,
  mi_option_reserve_huge_os_pages,
  mi_option_reserve_huge_os_pages_at,
  mi_option_reserve_os_memory,
  mi_option_deprecated_segment_cache,
  mi_option_deprecated_page_reset,
  mi_option_abandoned_page_purge,
  mi_option_deprecated_segment_reset,
  mi_option_eager_commit_delay,
  mi_option_purge_delay,
  mi_option_use_numa_nodes,
  mi_option_disallow_os_alloc,
  mi_option_os_tag,
  mi_option_max_errors,
  mi_option_max_warnings,
  mi_option_max_segment_reclaim,
  mi_option_destroy_on_exit,
  mi_option_arena_reserve,
  mi_option_arena_purge_mult,
  mi_option_purge_extend_delay,
  mi_option_abandoned_reclaim_on_free,
  mi_option_disallow_arena_alloc,
  mi_option_retry_on_oom,
  mi_option_visit_abandoned,
  mi_option_guarded_min,
  mi_option_guarded_max,
  mi_option_guarded_precise,
  mi_option_guarded_sample_rate,
  mi_option_guarded_sample_seed,
  mi_option_target_segments_per_thread,
  _mi_option_last,
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
mi_decl_nodiscard mi_decl_export long mi_option_get(mi_option_t option);
mi_decl_nodiscard mi_decl_export long mi_option_get_clamp(mi_option_t option, long min, long max);
mi_decl_nodiscard mi_decl_export size_t mi_option_get_size(mi_option_t option);
mi_decl_export void mi_option_set(mi_option_t option, long value);
mi_decl_export void mi_option_set_default(mi_option_t option, long value);
mi_decl_export void mi_cfree(void* p) mi_attr_noexcept;
mi_decl_export void* mi__expand(void* p, size_t newsize) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export size_t mi_malloc_size(const void* p) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export size_t mi_malloc_good_size(size_t size) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export size_t mi_malloc_usable_size(const void *p) mi_attr_noexcept;
mi_decl_export int mi_posix_memalign(void** p, size_t alignment, size_t size) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_memalign(size_t alignment, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_valloc(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_pvalloc(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_aligned_alloc(size_t alignment, size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(2) mi_attr_alloc_align(1);
mi_decl_nodiscard mi_decl_export void* mi_reallocarray(void* p, size_t count, size_t size) mi_attr_noexcept mi_attr_alloc_size2(2,3);
mi_decl_nodiscard mi_decl_export int mi_reallocarr(void* p, size_t count, size_t size) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export void* mi_aligned_recalloc(void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export void* mi_aligned_offset_recalloc(void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export mi_decl_restrict unsigned short* mi_wcsdup(const unsigned short* s) mi_attr_noexcept mi_attr_malloc;
mi_decl_nodiscard mi_decl_export mi_decl_restrict unsigned char* mi_mbsdup(const unsigned char* s) mi_attr_noexcept mi_attr_malloc;
mi_decl_export int mi_dupenv_s(char** buf, size_t* size, const char* name) mi_attr_noexcept;
mi_decl_export int mi_wdupenv_s(unsigned short** buf, size_t* size, const unsigned short* name) mi_attr_noexcept;
mi_decl_export void mi_free_size(void* p, size_t size) mi_attr_noexcept;
mi_decl_export void mi_free_size_aligned(void* p, size_t size, size_t alignment) mi_attr_noexcept;
mi_decl_export void mi_free_aligned(void* p, size_t alignment) mi_attr_noexcept;
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new(size_t size) mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new_aligned(size_t size, size_t alignment) mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new_nothrow(size_t size) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new_aligned_nothrow(size_t size, size_t alignment) mi_attr_noexcept mi_attr_malloc mi_attr_alloc_size(1) mi_attr_alloc_align(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_new_n(size_t count, size_t size) mi_attr_malloc mi_attr_alloc_size2(1, 2);
mi_decl_nodiscard mi_decl_export void* mi_new_realloc(void* p, size_t newsize) mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export void* mi_new_reallocn(void* p, size_t newcount, size_t size) mi_attr_alloc_size2(2, 3);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_alloc_new(mi_heap_t* heap, size_t size) mi_attr_malloc mi_attr_alloc_size(2);
mi_decl_nodiscard mi_decl_export mi_decl_restrict void* mi_heap_alloc_new_n(mi_heap_t* heap, size_t count, size_t size) mi_attr_malloc mi_attr_alloc_size2(2, 3);
#if 0
#endif
#if 0
#if 0
#endif
#if 0
  #else
  #endif
#if 0
  #else
  #endif
#if 0
  #endif
#if 0
#if 0
  #else
  #endif
#if 0
  #endif
#endif
#endif
#include <stddef.h>
#include <stdint.h>
#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 
#endif
#include <windows.h>
#elif !defined(__wasi__) && (!defined(__EMSCRIPTEN__) || defined(__EMSCRIPTEN_PTHREADS__))
#define MI_USE_PTHREADS 
#include <pthread.h>
#endif
#if 0
#if 0
#elif 0
#else
#endif
#elif 0
#else
#include <stdatomic.h>
#define mi_atomic(name) atomic_ ##name
#define mi_memory_order(name) memory_order_ ##name
#if (__STDC_VERSION__ >= 201710L)
 #define MI_ATOMIC_VAR_INIT(x) x
#elif !defined(ATOMIC_VAR_INIT)
 #define MI_ATOMIC_VAR_INIT(x) x
#else
 #define MI_ATOMIC_VAR_INIT(x) ATOMIC_VAR_INIT(x)
#endif
#endif
#define mi_atomic_cas_weak(p,expected,desired,mem_success,mem_fail) \
  mi_atomic(compare_exchange_weak_explicit)(p,expected,desired,mem_success,mem_fail)
#define mi_atomic_cas_strong(p,expected,desired,mem_success,mem_fail) \
  mi_atomic(compare_exchange_strong_explicit)(p,expected,desired,mem_success,mem_fail)
#define mi_atomic_load_acquire(p) mi_atomic(load_explicit)(p,mi_memory_order(acquire))
#define mi_atomic_load_relaxed(p) mi_atomic(load_explicit)(p,mi_memory_order(relaxed))
#define mi_atomic_store_release(p,x) mi_atomic(store_explicit)(p,x,mi_memory_order(release))
#define mi_atomic_store_relaxed(p,x) mi_atomic(store_explicit)(p,x,mi_memory_order(relaxed))
#define mi_atomic_exchange_release(p,x) mi_atomic(exchange_explicit)(p,x,mi_memory_order(release))
#define mi_atomic_exchange_acq_rel(p,x) mi_atomic(exchange_explicit)(p,x,mi_memory_order(acq_rel))
#define mi_atomic_cas_weak_release(p,exp,des) mi_atomic_cas_weak(p,exp,des,mi_memory_order(release),mi_memory_order(relaxed))
#define mi_atomic_cas_weak_acq_rel(p,exp,des) mi_atomic_cas_weak(p,exp,des,mi_memory_order(acq_rel),mi_memory_order(acquire))
#define mi_atomic_cas_strong_release(p,exp,des) mi_atomic_cas_strong(p,exp,des,mi_memory_order(release),mi_memory_order(relaxed))
#define mi_atomic_cas_strong_acq_rel(p,exp,des) mi_atomic_cas_strong(p,exp,des,mi_memory_order(acq_rel),mi_memory_order(acquire))
#define mi_atomic_add_relaxed(p,x) mi_atomic(fetch_add_explicit)(p,x,mi_memory_order(relaxed))
#define mi_atomic_sub_relaxed(p,x) mi_atomic(fetch_sub_explicit)(p,x,mi_memory_order(relaxed))
#define mi_atomic_add_acq_rel(p,x) mi_atomic(fetch_add_explicit)(p,x,mi_memory_order(acq_rel))
#define mi_atomic_sub_acq_rel(p,x) mi_atomic(fetch_sub_explicit)(p,x,mi_memory_order(acq_rel))
#define mi_atomic_and_acq_rel(p,x) mi_atomic(fetch_and_explicit)(p,x,mi_memory_order(acq_rel))
#define mi_atomic_or_acq_rel(p,x) mi_atomic(fetch_or_explicit)(p,x,mi_memory_order(acq_rel))
#define mi_atomic_increment_relaxed(p) mi_atomic_add_relaxed(p,(uintptr_t)1)
#define mi_atomic_decrement_relaxed(p) mi_atomic_sub_relaxed(p,(uintptr_t)1)
#define mi_atomic_increment_acq_rel(p) mi_atomic_add_acq_rel(p,(uintptr_t)1)
#define mi_atomic_decrement_acq_rel(p) mi_atomic_sub_acq_rel(p,(uintptr_t)1)
static inline void mi_atomic_yield(void);
static inline intptr_t mi_atomic_addi(_Atomic(intptr_t)*p, intptr_t add);
static inline intptr_t mi_atomic_subi(_Atomic(intptr_t)*p, intptr_t sub);
#if defined(__cplusplus) || !defined(_MSC_VER)
#define mi_atomic_load_ptr_acquire(tp,p) mi_atomic_load_acquire(p)
#define mi_atomic_load_ptr_relaxed(tp,p) mi_atomic_load_relaxed(p)
#if 0
#else
#define mi_atomic_store_ptr_release(tp,p,x) mi_atomic_store_release(p,x)
#define mi_atomic_store_ptr_relaxed(tp,p,x) mi_atomic_store_relaxed(p,x)
#define mi_atomic_cas_ptr_weak_release(tp,p,exp,des) mi_atomic_cas_weak_release(p,exp,des)
#define mi_atomic_cas_ptr_weak_acq_rel(tp,p,exp,des) mi_atomic_cas_weak_acq_rel(p,exp,des)
#define mi_atomic_cas_ptr_strong_release(tp,p,exp,des) mi_atomic_cas_strong_release(p,exp,des)
#define mi_atomic_exchange_ptr_release(tp,p,x) mi_atomic_exchange_release(p,x)
#define mi_atomic_exchange_ptr_acq_rel(tp,p,x) mi_atomic_exchange_acq_rel(p,x)
#endif
static inline int64_t mi_atomic_addi64_relaxed(volatile int64_t* p, int64_t add) {
  return mi_atomic(fetch_add_explicit)((_Atomic(int64_t)*)p, add, mi_memory_order(relaxed));
}
static inline void mi_atomic_maxi64_relaxed(volatile int64_t* p, int64_t x) {
  int64_t current = mi_atomic_load_relaxed((_Atomic(int64_t)*)p);
  while (current < x && !mi_atomic_cas_weak_release((_Atomic(int64_t)*)p, &current, x)) { };
}
#define mi_atomic_loadi64_acquire(p) mi_atomic(load_explicit)(p,mi_memory_order(acquire))
#define mi_atomic_loadi64_relaxed(p) mi_atomic(load_explicit)(p,mi_memory_order(relaxed))
#define mi_atomic_storei64_release(p,x) mi_atomic(store_explicit)(p,x,mi_memory_order(release))
#define mi_atomic_storei64_relaxed(p,x) mi_atomic(store_explicit)(p,x,mi_memory_order(relaxed))
#define mi_atomic_casi64_strong_acq_rel(p,e,d) mi_atomic_cas_strong_acq_rel(p,e,d)
#define mi_atomic_addi64_acq_rel(p,i) mi_atomic_add_acq_rel(p,i)
#elif 0
#if 0
#else
#endif
#if 0
#else
#endif
#if 0
#else
#endif
#if 0
#else
#endif
#if 0
#else
#endif
#if 0
#else
#endif
#endif
static inline intptr_t mi_atomic_addi(_Atomic(intptr_t)*p, intptr_t add) {
  return (intptr_t)mi_atomic_add_acq_rel((_Atomic(uintptr_t)*)p, (uintptr_t)add);
}
static inline intptr_t mi_atomic_subi(_Atomic(intptr_t)*p, intptr_t sub) {
  return (intptr_t)mi_atomic_addi(p, -sub);
}
typedef _Atomic(uintptr_t) mi_atomic_once_t;
static inline bool mi_atomic_once( mi_atomic_once_t* once ) {
  if (mi_atomic_load_relaxed(once) != 0) return false;
  uintptr_t expected = 0;
  return mi_atomic_cas_strong_acq_rel(once, &expected, (uintptr_t)1);
}
typedef _Atomic(uintptr_t) mi_atomic_guard_t;
#define mi_atomic_guard(guard) \
  uintptr_t _mi_guard_expected = 0; \
  for(bool _mi_guard_once = true; \
      _mi_guard_once && mi_atomic_cas_strong_acq_rel(guard,&_mi_guard_expected,(uintptr_t)1); \
      (mi_atomic_store_release(guard,(uintptr_t)0), _mi_guard_once = false) )
#if 0
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
#endif
#elif defined(__armel__) || defined(__ARMEL__)
static inline void mi_atomic_yield(void) {
  __asm__ volatile ("nop" ::: "memory");
}
#endif
#elif defined(__sun)
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
#endif
#if defined(_WIN32)
#define mi_lock_t CRITICAL_SECTION
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
#define mi_lock_t pthread_mutex_t
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
#else
#define mi_lock_t _Atomic(uintptr_t)
static inline bool mi_lock_try_acquire(mi_lock_t* lock) {
  uintptr_t expected = 0;
  return mi_atomic_cas_strong_acq_rel(lock, &expected, (uintptr_t)1);
}
static inline bool mi_lock_acquire(mi_lock_t* lock) {
  for (int i = 0; i < 1000; i++) {
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
#endif
#if 0
#endif
#ifndef MI_MAX_ALIGN_SIZE
#define MI_MAX_ALIGN_SIZE 16
#endif
#if !defined(MI_SECURE)
#define MI_SECURE 0
#endif
#if !defined(MI_DEBUG)
#if !defined(NDEBUG) || defined(_DEBUG)
#define MI_DEBUG 2
#else
#define MI_DEBUG 0
#endif
#endif
#if defined(MI_GUARDED)
#define MI_PADDING 0
#endif
#if !defined(MI_PADDING) && (MI_SECURE>=3 || MI_DEBUG>=1 || (MI_TRACK_VALGRIND || MI_TRACK_ASAN || MI_TRACK_ETW))
#define MI_PADDING 1
#endif
#if !defined(MI_PADDING_CHECK) && MI_PADDING && (MI_SECURE>=3 || MI_DEBUG>=1)
#define MI_PADDING_CHECK 1
#endif
#if (MI_SECURE>=3 || MI_DEBUG>=1)
#define MI_ENCODE_FREELIST 1
#endif
#if INTPTR_MAX > INT64_MAX
#define MI_INTPTR_SHIFT (4)
#elif INTPTR_MAX == INT64_MAX
#define MI_INTPTR_SHIFT (3)
#elif INTPTR_MAX == INT32_MAX
#define MI_INTPTR_SHIFT (2)
#else
#error platform pointers must be 32, 64, or 128 bits
#endif
#if SIZE_MAX == UINT64_MAX
#define MI_SIZE_SHIFT (3)
typedef int64_t mi_ssize_t;
#elif SIZE_MAX == UINT32_MAX
#define MI_SIZE_SHIFT (2)
typedef int32_t mi_ssize_t;
#else
#error platform objects must be 32 or 64 bits
#endif
#if (SIZE_MAX/2) > LONG_MAX
#define MI_ZU(x) x ##ULL
#define MI_ZI(x) x ##LL
#else
#define MI_ZU(x) x ##UL
#define MI_ZI(x) x ##L
#endif
#define MI_INTPTR_SIZE (1<<MI_INTPTR_SHIFT)
#define MI_INTPTR_BITS (MI_INTPTR_SIZE*8)
#define MI_SIZE_SIZE (1<<MI_SIZE_SHIFT)
#define MI_SIZE_BITS (MI_SIZE_SIZE*8)
#define MI_KiB (MI_ZU(1024))
#define MI_MiB (MI_KiB*MI_KiB)
#define MI_GiB (MI_MiB*MI_KiB)
#ifndef MI_SMALL_PAGE_SHIFT
#define MI_SMALL_PAGE_SHIFT (13 + MI_INTPTR_SHIFT)
#endif
#ifndef MI_MEDIUM_PAGE_SHIFT
#define MI_MEDIUM_PAGE_SHIFT ( 3 + MI_SMALL_PAGE_SHIFT)
#endif
#ifndef MI_LARGE_PAGE_SHIFT
#define MI_LARGE_PAGE_SHIFT ( 3 + MI_MEDIUM_PAGE_SHIFT)
#endif
#ifndef MI_SEGMENT_SHIFT
#define MI_SEGMENT_SHIFT ( MI_LARGE_PAGE_SHIFT)
#endif
#define MI_SEGMENT_SIZE (MI_ZU(1)<<MI_SEGMENT_SHIFT)
#define MI_SEGMENT_ALIGN (MI_SEGMENT_SIZE)
#define MI_SEGMENT_MASK ((uintptr_t)(MI_SEGMENT_ALIGN - 1))
#define MI_SMALL_PAGE_SIZE (MI_ZU(1)<<MI_SMALL_PAGE_SHIFT)
#define MI_MEDIUM_PAGE_SIZE (MI_ZU(1)<<MI_MEDIUM_PAGE_SHIFT)
#define MI_LARGE_PAGE_SIZE (MI_ZU(1)<<MI_LARGE_PAGE_SHIFT)
#define MI_SMALL_PAGES_PER_SEGMENT (MI_SEGMENT_SIZE/MI_SMALL_PAGE_SIZE)
#define MI_MEDIUM_PAGES_PER_SEGMENT (MI_SEGMENT_SIZE/MI_MEDIUM_PAGE_SIZE)
#define MI_LARGE_PAGES_PER_SEGMENT (MI_SEGMENT_SIZE/MI_LARGE_PAGE_SIZE)
#define MI_SMALL_OBJ_SIZE_MAX (MI_SMALL_PAGE_SIZE/4)
#define MI_MEDIUM_OBJ_SIZE_MAX (MI_MEDIUM_PAGE_SIZE/4)
#define MI_LARGE_OBJ_SIZE_MAX (MI_LARGE_PAGE_SIZE/2)
#define MI_LARGE_OBJ_WSIZE_MAX (MI_LARGE_OBJ_SIZE_MAX/MI_INTPTR_SIZE)
#define MI_BIN_HUGE (73U)
#if (MI_LARGE_OBJ_WSIZE_MAX >= 655360)
#error "mimalloc internal: define more bins"
#endif
#define MI_MAX_ALIGN_GUARANTEE (MI_MEDIUM_OBJ_SIZE_MAX)
#define MI_BLOCK_ALIGNMENT_MAX (MI_SEGMENT_SIZE >> 1)
#define MI_MAX_ALLOC_SIZE PTRDIFF_MAX
typedef uintptr_t mi_encoded_t;
typedef size_t mi_threadid_t;
typedef struct mi_block_s {
  mi_encoded_t next;
} mi_block_t;
#if MI_GUARDED
#define MI_BLOCK_TAG_ALIGNED ((mi_encoded_t)(0))
#define MI_BLOCK_TAG_GUARDED (~MI_BLOCK_TAG_ALIGNED)
#endif
typedef enum mi_delayed_e {
  MI_USE_DELAYED_FREE = 0,
  MI_DELAYED_FREEING = 1,
  MI_NO_DELAYED_FREE = 2,
  MI_NEVER_DELAYED_FREE = 3
} mi_delayed_t;
#if !MI_TSAN
typedef union mi_page_flags_s {
  uint8_t full_aligned;
  struct {
    uint8_t in_full : 1;
    uint8_t has_aligned : 1;
  } x;
} mi_page_flags_t;
#else
typedef union mi_page_flags_s {
  uint32_t full_aligned;
  struct {
    uint8_t in_full;
    uint8_t has_aligned;
  } x;
} mi_page_flags_t;
#endif
typedef uintptr_t mi_thread_free_t;
typedef struct mi_page_s {
  uint8_t segment_idx;
  uint8_t segment_in_use:1;
  uint8_t is_committed:1;
  uint8_t is_zero_init:1;
  uint8_t is_huge:1;
  uint16_t capacity;
  uint16_t reserved;
  mi_page_flags_t flags;
  uint8_t free_is_zero:1;
  uint8_t retire_expire:7;
  mi_block_t* free;
  mi_block_t* local_free;
  uint16_t used;
  uint8_t block_size_shift;
  uint8_t heap_tag;
  size_t block_size;
  uint8_t* page_start;
  #if (MI_ENCODE_FREELIST || MI_PADDING)
  uintptr_t keys[2];
  #endif
  _Atomic(mi_thread_free_t) xthread_free;
  _Atomic(uintptr_t) xheap;
  struct mi_page_s* next;
  struct mi_page_s* prev;
  #if MI_INTPTR_SIZE==4
  void* padding[1];
  #endif
} mi_page_t;
typedef enum mi_page_kind_e {
  MI_PAGE_SMALL,
  MI_PAGE_MEDIUM,
  MI_PAGE_LARGE,
  MI_PAGE_HUGE
} mi_page_kind_t;
typedef enum mi_memkind_e {
  MI_MEM_NONE,
  MI_MEM_EXTERNAL,
  MI_MEM_STATIC,
  MI_MEM_OS,
  MI_MEM_OS_HUGE,
  MI_MEM_OS_REMAP,
  MI_MEM_ARENA
} mi_memkind_t;
static inline bool mi_memkind_is_os(mi_memkind_t memkind) {
  return (memkind >= MI_MEM_OS && memkind <= MI_MEM_OS_REMAP);
}
typedef struct mi_memid_os_info {
  void* base;
  size_t alignment;
} mi_memid_os_info_t;
typedef struct mi_memid_arena_info {
  size_t block_index;
  mi_arena_id_t id;
  bool is_exclusive;
} mi_memid_arena_info_t;
typedef struct mi_memid_s {
  union {
    mi_memid_os_info_t os;
    mi_memid_arena_info_t arena;
  } mem;
  bool is_pinned;
  bool initially_committed;
  bool initially_zero;
  mi_memkind_t memkind;
} mi_memid_t;
typedef struct mi_subproc_s mi_subproc_t;
typedef struct mi_segment_s {
  mi_memid_t memid;
  bool allow_decommit;
  bool allow_purge;
  size_t segment_size;
  mi_subproc_t* subproc;
  struct mi_segment_s* next;
  struct mi_segment_s* prev;
  bool was_reclaimed;
  bool dont_free;
  size_t abandoned;
  size_t abandoned_visits;
  size_t used;
  size_t capacity;
  size_t segment_info_size;
  uintptr_t cookie;
  struct mi_segment_s* abandoned_os_next;
  struct mi_segment_s* abandoned_os_prev;
  _Atomic(mi_threadid_t) thread_id;
  size_t page_shift;
  mi_page_kind_t page_kind;
  mi_page_t pages[1];
} mi_segment_t;
typedef struct mi_tld_s mi_tld_t;
typedef struct mi_page_queue_s {
  mi_page_t* first;
  mi_page_t* last;
  size_t block_size;
} mi_page_queue_t;
#define MI_BIN_FULL (MI_BIN_HUGE+1)
typedef struct mi_random_cxt_s {
  uint32_t input[16];
  uint32_t output[16];
  int output_available;
  bool weak;
} mi_random_ctx_t;
#if (MI_PADDING)
typedef struct mi_padding_s {
  uint32_t canary;
  uint32_t delta;
} mi_padding_t;
#define MI_PADDING_SIZE (sizeof(mi_padding_t))
#define MI_PADDING_WSIZE ((MI_PADDING_SIZE + MI_INTPTR_SIZE - 1) / MI_INTPTR_SIZE)
#else
#define MI_PADDING_SIZE 0
#define MI_PADDING_WSIZE 0
#endif
#define MI_PAGES_DIRECT (MI_SMALL_WSIZE_MAX + MI_PADDING_WSIZE + 1)
struct mi_heap_s {
  mi_tld_t* tld;
  _Atomic(mi_block_t*) thread_delayed_free;
  mi_threadid_t thread_id;
  mi_arena_id_t arena_id;
  uintptr_t cookie;
  uintptr_t keys[2];
  mi_random_ctx_t random;
  size_t page_count;
  size_t page_retired_min;
  size_t page_retired_max;
  mi_heap_t* next;
  bool no_reclaim;
  uint8_t tag;
  #if MI_GUARDED
  size_t guarded_size_min;
  size_t guarded_size_max;
  size_t guarded_sample_rate;
  size_t guarded_sample_seed;
  size_t guarded_sample_count;
  #endif
  mi_page_t* pages_free_direct[MI_PAGES_DIRECT];
  mi_page_queue_t pages[MI_BIN_FULL + 1];
};
#if !defined(MI_DEBUG_UNINIT)
#define MI_DEBUG_UNINIT (0xD0)
#endif
#if !defined(MI_DEBUG_FREED)
#define MI_DEBUG_FREED (0xDF)
#endif
#if !defined(MI_DEBUG_PADDING)
#define MI_DEBUG_PADDING (0xDE)
#endif
#if (MI_DEBUG)
void _mi_assert_fail(const char* assertion, const char* fname, unsigned int line, const char* func );
#define mi_assert(expr) ((expr) ? (void)0 : _mi_assert_fail(#expr,__FILE__,__LINE__,__func__))
#else
#define mi_assert(x) 
#endif
#if (MI_DEBUG>1)
#define mi_assert_internal mi_assert
#else
#define mi_assert_internal(x) 
#endif
#if (MI_DEBUG>2)
#define mi_assert_expensive mi_assert
#else
#define mi_assert_expensive(x) 
#endif
#ifndef MI_STAT
#if (MI_DEBUG>0)
#define MI_STAT 2
#else
#define MI_STAT 0
#endif
#endif
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
#endif
} mi_stats_t;
void _mi_stat_increase(mi_stat_count_t* stat, size_t amount);
void _mi_stat_decrease(mi_stat_count_t* stat, size_t amount);
void _mi_stat_counter_increase(mi_stat_counter_t* stat, size_t amount);
#if (MI_STAT)
#define mi_stat_increase(stat,amount) _mi_stat_increase( &(stat), amount)
#define mi_stat_decrease(stat,amount) _mi_stat_decrease( &(stat), amount)
#define mi_stat_counter_increase(stat,amount) _mi_stat_counter_increase( &(stat), amount)
#else
#define mi_stat_increase(stat,amount) (void)0
#define mi_stat_decrease(stat,amount) (void)0
#define mi_stat_counter_increase(stat,amount) (void)0
#endif
#define mi_heap_stat_counter_increase(heap,stat,amount) mi_stat_counter_increase( (heap)->tld->stats.stat, amount)
#define mi_heap_stat_increase(heap,stat,amount) mi_stat_increase( (heap)->tld->stats.stat, amount)
#define mi_heap_stat_decrease(heap,stat,amount) mi_stat_decrease( (heap)->tld->stats.stat, amount)
struct mi_subproc_s {
  _Atomic(size_t) abandoned_count;
  _Atomic(size_t) abandoned_os_list_count;
  mi_lock_t abandoned_os_lock;
  mi_lock_t abandoned_os_visit_lock;
  mi_segment_t* abandoned_os_list;
  mi_segment_t* abandoned_os_list_tail;
  mi_memid_t memid;
};
typedef int64_t mi_msecs_t;
typedef struct mi_segment_queue_s {
  mi_segment_t* first;
  mi_segment_t* last;
} mi_segment_queue_t;
typedef struct mi_os_tld_s {
  size_t region_idx;
  mi_stats_t* stats;
} mi_os_tld_t;
typedef struct mi_segments_tld_s {
  mi_segment_queue_t small_free;
  mi_segment_queue_t medium_free;
  mi_page_queue_t pages_purge;
  size_t count;
  size_t peak_count;
  size_t current_size;
  size_t peak_size;
  size_t reclaim_count;
  mi_subproc_t* subproc;
  mi_stats_t* stats;
  mi_os_tld_t* os;
} mi_segments_tld_t;
struct mi_tld_s {
  unsigned long long heartbeat;
  bool recurse;
  mi_heap_t* heap_backing;
  mi_heap_t* heaps;
  mi_segments_tld_t segments;
  mi_os_tld_t os;
  mi_stats_t stats;
};
#if MI_TRACK_VALGRIND
#define MI_TRACK_ENABLED 1
#define MI_TRACK_HEAP_DESTROY 1
#define MI_TRACK_TOOL "valgrind"
#include <valgrind/valgrind.h>
#include <valgrind/memcheck.h>
#define mi_track_malloc_size(p,reqsize,size,zero) VALGRIND_MALLOCLIKE_BLOCK(p,size,MI_PADDING_SIZE ,zero)
#define mi_track_free_size(p,_size) VALGRIND_FREELIKE_BLOCK(p,MI_PADDING_SIZE )
#define mi_track_resize(p,oldsize,newsize) VALGRIND_RESIZEINPLACE_BLOCK(p,oldsize,newsize,MI_PADDING_SIZE )
#define mi_track_mem_defined(p,size) VALGRIND_MAKE_MEM_DEFINED(p,size)
#define mi_track_mem_undefined(p,size) VALGRIND_MAKE_MEM_UNDEFINED(p,size)
#define mi_track_mem_noaccess(p,size) VALGRIND_MAKE_MEM_NOACCESS(p,size)
#elif MI_TRACK_ASAN
#define MI_TRACK_ENABLED 1
#define MI_TRACK_HEAP_DESTROY 0
#define MI_TRACK_TOOL "asan"
#include <sanitizer/asan_interface.h>
#define mi_track_malloc_size(p,reqsize,size,zero) ASAN_UNPOISON_MEMORY_REGION(p,size)
#define mi_track_free_size(p,size) ASAN_POISON_MEMORY_REGION(p,size)
#define mi_track_mem_defined(p,size) ASAN_UNPOISON_MEMORY_REGION(p,size)
#define mi_track_mem_undefined(p,size) ASAN_UNPOISON_MEMORY_REGION(p,size)
#define mi_track_mem_noaccess(p,size) ASAN_POISON_MEMORY_REGION(p,size)
#elif MI_TRACK_ETW
#define MI_TRACK_ENABLED 1
#define MI_TRACK_HEAP_DESTROY 1
#define MI_TRACK_TOOL "ETW"
#include <wmistr.h>
#include <evntrace.h>
#include <evntprov.h>
#ifndef ETW_INLINE
  #ifdef _ETW_KM_
    #define ETW_INLINE DECLSPEC_NOINLINE __inline
  #else
    #define ETW_INLINE __inline
  #endif
#endif
#if 0
#endif
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION
#ifndef MCGEN_USE_KERNEL_MODE_APIS
  #ifdef _ETW_KM_
    #define MCGEN_USE_KERNEL_MODE_APIS 1
  #else
    #define MCGEN_USE_KERNEL_MODE_APIS 0
  #endif
#endif
#ifndef MCGEN_HAVE_EVENTSETINFORMATION
  #ifdef MCGEN_EVENTSETINFORMATION
    #define MCGEN_HAVE_EVENTSETINFORMATION 1
  #elif MCGEN_USE_KERNEL_MODE_APIS
    #if NTDDI_VERSION >= 0x06040000
      #define MCGEN_HAVE_EVENTSETINFORMATION 1
    #else
      #define MCGEN_HAVE_EVENTSETINFORMATION 2
    #endif
  #else
    #if WINVER >= 0x0602 && defined(EVENT_FILTER_TYPE_SCHEMATIZED)
      #define MCGEN_HAVE_EVENTSETINFORMATION 1
    #else
      #define MCGEN_HAVE_EVENTSETINFORMATION 2
    #endif
  #endif
#endif
#ifndef MCGEN_EVENTREGISTER
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define MCGEN_EVENTREGISTER EtwRegister
  #else
    #define MCGEN_EVENTREGISTER EventRegister
  #endif
#endif
#ifndef MCGEN_EVENTUNREGISTER
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define MCGEN_EVENTUNREGISTER EtwUnregister
  #else
    #define MCGEN_EVENTUNREGISTER EventUnregister
  #endif
#endif
#ifndef MCGEN_EVENTSETINFORMATION
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define MCGEN_EVENTSETINFORMATION EtwSetInformation
  #else
    #define MCGEN_EVENTSETINFORMATION EventSetInformation
  #endif
#endif
#ifndef MCGEN_EVENTWRITETRANSFER
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define MCGEN_EVENTWRITETRANSFER EtwWriteTransfer
  #else
    #define MCGEN_EVENTWRITETRANSFER EventWriteTransfer
  #endif
#endif
#ifndef MCGEN_EVENT_ENABLED
#define MCGEN_EVENT_ENABLED(EventName) EventEnabled ##EventName()
#endif
#ifndef MCGEN_EVENT_ENABLED_FORCONTEXT
#define MCGEN_EVENT_ENABLED_FORCONTEXT(pContext,EventName) EventEnabled ##EventName ##_ForContext(pContext)
#endif
#ifndef MCGEN_ENABLE_CHECK
#define MCGEN_ENABLE_CHECK(Context,Descriptor) (Context.IsEnabled && McGenEventEnabled(&Context, &Descriptor))
#endif
#if !defined(MCGEN_TRACE_CONTEXT_DEF)
#define MCGEN_TRACE_CONTEXT_DEF 
typedef struct _MCGEN_TRACE_CONTEXT
{
    TRACEHANDLE RegistrationHandle;
    TRACEHANDLE Logger;
    ULONGLONG MatchAnyKeyword;
    ULONGLONG MatchAllKeyword;
    ULONG Flags;
    ULONG IsEnabled;
    UCHAR Level;
    UCHAR Reserve;
    USHORT EnableBitsCount;
    PULONG EnableBitMask;
    const ULONGLONG* EnableKeyWords;
    const UCHAR* EnableLevel;
} MCGEN_TRACE_CONTEXT, *PMCGEN_TRACE_CONTEXT;
#endif
#if !defined(MCGEN_LEVEL_KEYWORD_ENABLED_DEF)
#define MCGEN_LEVEL_KEYWORD_ENABLED_DEF 
FORCEINLINE
BOOLEAN
McGenLevelKeywordEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ UCHAR Level,
    _In_ ULONGLONG Keyword
    )
{
    if ((Level <= EnableInfo->Level) ||
        (EnableInfo->Level == 0)) {
        if ((Keyword == (ULONGLONG)0) ||
            ((Keyword & EnableInfo->MatchAnyKeyword) &&
             ((Keyword & EnableInfo->MatchAllKeyword) == EnableInfo->MatchAllKeyword))) {
            return TRUE;
        }
    }
    return FALSE;
}
#endif
#if !defined(MCGEN_EVENT_ENABLED_DEF)
#define MCGEN_EVENT_ENABLED_DEF 
FORCEINLINE
BOOLEAN
McGenEventEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{
    return McGenLevelKeywordEnabled(EnableInfo, EventDescriptor->Level, EventDescriptor->Keyword);
}
#endif
#if !defined(MCGEN_CONTROL_CALLBACK)
#define MCGEN_CONTROL_CALLBACK 
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
{
    PMCGEN_TRACE_CONTEXT Ctx = (PMCGEN_TRACE_CONTEXT)CallbackContext;
    ULONG Ix;
#ifndef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    UNREFERENCED_PARAMETER(SourceId);
    UNREFERENCED_PARAMETER(FilterData);
#endif
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
#pragma warning(suppress: 26451)
                RtlZeroMemory(Ctx->EnableBitMask, (((Ctx->EnableBitsCount - 1) / 32) + 1) * sizeof(ULONG));
            }
            break;
        default:
            break;
    }
#ifdef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    MCGEN_PRIVATE_ENABLE_CALLBACK_V2(
        SourceId,
        ControlCode,
        Level,
        MatchAnyKeyword,
        MatchAllKeyword,
        FilterData,
        CallbackContext
        );
#endif
    return;
}
#endif
#ifndef _mcgen_PENABLECALLBACK
  #if MCGEN_USE_KERNEL_MODE_APIS
    #define _mcgen_PENABLECALLBACK PETWENABLECALLBACK
  #else
    #define _mcgen_PENABLECALLBACK PENABLECALLBACK
  #endif
#endif
#if !defined(_mcgen_PASTE2)
#define _mcgen_PASTE2(a,b) _mcgen_PASTE2_imp(a, b)
#define _mcgen_PASTE2_imp(a,b) a ##b
#endif
#if !defined(_mcgen_PASTE3)
#define _mcgen_PASTE3(a,b,c) _mcgen_PASTE3_imp(a, b, c)
#define _mcgen_PASTE3_imp(a,b,c) a ##b ##_ ##c
#endif
struct _mcgen_PASTE2(MCGEN_EVENTREGISTER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTREGISTER);
typedef struct _mcgen_PASTE2(MCGEN_EVENTREGISTER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTREGISTER)
    MCGEN_EVENTREGISTER_must_not_be_redefined_between_headers;
typedef void MCGEN_EVENTREGISTER_must_not_be_a_functionLike_macro_MCGEN_EVENTREGISTER;
typedef int _mcgen_PASTE2(MCGEN_EVENTREGISTER_must_not_be_a_functionLike_macro_, MCGEN_EVENTREGISTER);
struct _mcgen_PASTE2(MCGEN_EVENTUNREGISTER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTUNREGISTER);
typedef struct _mcgen_PASTE2(MCGEN_EVENTUNREGISTER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTUNREGISTER)
    MCGEN_EVENTUNREGISTER_must_not_be_redefined_between_headers;
typedef void MCGEN_EVENTUNREGISTER_must_not_be_a_functionLike_macro_MCGEN_EVENTUNREGISTER;
typedef int _mcgen_PASTE2(MCGEN_EVENTUNREGISTER_must_not_be_a_functionLike_macro_, MCGEN_EVENTUNREGISTER);
struct _mcgen_PASTE2(MCGEN_EVENTSETINFORMATION_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTSETINFORMATION);
typedef struct _mcgen_PASTE2(MCGEN_EVENTSETINFORMATION_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTSETINFORMATION)
    MCGEN_EVENTSETINFORMATION_must_not_be_redefined_between_headers;
typedef void MCGEN_EVENTSETINFORMATION_must_not_be_a_functionLike_macro_MCGEN_EVENTSETINFORMATION;
typedef int _mcgen_PASTE2(MCGEN_EVENTSETINFORMATION_must_not_be_a_functionLike_macro_, MCGEN_EVENTSETINFORMATION);
struct _mcgen_PASTE2(MCGEN_EVENTWRITETRANSFER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTWRITETRANSFER);
typedef struct _mcgen_PASTE2(MCGEN_EVENTWRITETRANSFER_definition_must_be_an_unqualified_identifier_, MCGEN_EVENTWRITETRANSFER)
    MCGEN_EVENTWRITETRANSFER_must_not_be_redefined_between_headers;;
typedef void MCGEN_EVENTWRITETRANSFER_must_not_be_a_functionLike_macro_MCGEN_EVENTWRITETRANSFER;
typedef int _mcgen_PASTE2(MCGEN_EVENTWRITETRANSFER_must_not_be_a_functionLike_macro_, MCGEN_EVENTWRITETRANSFER);
#ifndef McGenEventWrite_def
#define McGenEventWrite_def 
#define McGenEventWrite _mcgen_PASTE2(McGenEventWrite_, MCGEN_EVENTWRITETRANSFER)
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
    UNREFERENCED_PARAMETER(ActivityId);
    Traits = (const USHORT UNALIGNED*)(UINT_PTR)Context->Logger;
    if (Traits == NULL) {
        EventData[0].Ptr = 0;
        EventData[0].Size = 0;
        EventData[0].Reserved = 0;
    } else {
        EventData[0].Ptr = (ULONG_PTR)Traits;
        EventData[0].Size = *Traits;
        EventData[0].Reserved = 2;
    }
    return MCGEN_EVENTWRITETRANSFER(
        Context->RegistrationHandle,
        Descriptor,
        ActivityId,
        NULL,
        EventDataCount,
        EventData);
}
#endif
#if !defined(McGenEventRegisterUnregister)
#define McGenEventRegisterUnregister 
#define McGenEventRegister _mcgen_PASTE2(McGenEventRegister_, MCGEN_EVENTREGISTER)
#pragma warning(push)
#pragma warning(disable:6103)
DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventRegister(
    _In_ LPCGUID ProviderId,
    _In_opt_ _mcgen_PENABLECALLBACK EnableCallback,
    _In_opt_ PVOID CallbackContext,
    _Inout_ PREGHANDLE RegHandle
    )
{
    ULONG Error;
    if (*RegHandle != 0)
    {
        Error = 0;
    }
    else
    {
        Error = MCGEN_EVENTREGISTER(ProviderId, EnableCallback, CallbackContext, RegHandle);
    }
    return Error;
}
#pragma warning(pop)
#define McGenEventUnregister _mcgen_PASTE2(McGenEventUnregister_, MCGEN_EVENTUNREGISTER)
DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventUnregister(_Inout_ PREGHANDLE RegHandle)
{
    ULONG Error;
    if(*RegHandle == 0)
    {
        Error = 0;
    }
    else
    {
        Error = MCGEN_EVENTUNREGISTER(*RegHandle);
        *RegHandle = (REGHANDLE)0;
    }
    return Error;
}
#endif
#ifndef _mcgen_EVENT_BIT_SET
  #if defined(_M_IX86) || defined(_M_X64)
    #define _mcgen_EVENT_BIT_SET(EnableBits, BitPosition) ((((const unsigned char*)EnableBits)[BitPosition >> 3] & (1u << (BitPosition & 7))) != 0)
  #else
    #define _mcgen_EVENT_BIT_SET(EnableBits, BitPosition) ((EnableBits[BitPosition >> 5] & (1u << (BitPosition & 31))) != 0)
  #endif
#endif
#endif
EXTERN_C __declspec(selectany) const GUID ETW_MI_Provider = {0x138f4dbb, 0xee04, 0x4899, {0xaa, 0x0a, 0x57, 0x2a, 0xd4, 0x47, 0x57, 0x79}};
#ifndef ETW_MI_Provider_Traits
#define ETW_MI_Provider_Traits NULL
#endif
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR ETW_MI_ALLOC = {0x64, 0x1, 0x0, 0x4, 0x0, 0x0, 0x0};
#define ETW_MI_ALLOC_value 0x64
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR ETW_MI_FREE = {0x65, 0x1, 0x0, 0x4, 0x0, 0x0, 0x0};
#define ETW_MI_FREE_value 0x65
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION
EXTERN_C __declspec(selectany) DECLSPEC_CACHEALIGN ULONG microsoft_windows_mimallocEnableBits[1];
EXTERN_C __declspec(selectany) const ULONGLONG microsoft_windows_mimallocKeywords[1] = {0x0};
EXTERN_C __declspec(selectany) const unsigned char microsoft_windows_mimallocLevels[1] = {4};
EXTERN_C __declspec(selectany) MCGEN_TRACE_CONTEXT ETW_MI_Provider_Context = {0, (ULONG_PTR)ETW_MI_Provider_Traits, 0, 0, 0, 0, 0, 0, 1, microsoft_windows_mimallocEnableBits, microsoft_windows_mimallocKeywords, microsoft_windows_mimallocLevels};
#define microsoft_windows_mimallocHandle (ETW_MI_Provider_Context.RegistrationHandle)
#define ETW_MI_Provider_EventWriteActivity 0
#ifndef EventRegistermicrosoft_windows_mimalloc
#define EventRegistermicrosoft_windows_mimalloc() McGenEventRegister(&ETW_MI_Provider, McGenControlCallbackV2, &ETW_MI_Provider_Context, &microsoft_windows_mimallocHandle)
#endif
#ifndef EventRegisterByGuidmicrosoft_windows_mimalloc
#define EventRegisterByGuidmicrosoft_windows_mimalloc(Guid) McGenEventRegister(&(Guid), McGenControlCallbackV2, &ETW_MI_Provider_Context, &microsoft_windows_mimallocHandle)
#endif
#ifndef EventUnregistermicrosoft_windows_mimalloc
#define EventUnregistermicrosoft_windows_mimalloc() McGenEventUnregister(&microsoft_windows_mimallocHandle)
#endif
#ifdef MCGEN_ENABLE_FORCONTEXT_CODE_GENERATION
typedef struct tagMcGenContext_microsoft_windows_mimalloc {
    MCGEN_TRACE_CONTEXT Context;
    ULONG EnableBits[1];
} McGenContext_microsoft_windows_mimalloc;
#define EventRegistermicrosoft_windows_mimalloc_ForContext(pContext) _mcgen_PASTE2(_mcgen_RegisterForContext_microsoft_windows_mimalloc_, MCGEN_EVENTREGISTER)(&ETW_MI_Provider, pContext)
#define EventRegisterByGuidmicrosoft_windows_mimalloc_ForContext(Guid,pContext) _mcgen_PASTE2(_mcgen_RegisterForContext_microsoft_windows_mimalloc_, MCGEN_EVENTREGISTER)(&(Guid), pContext)
#define EventUnregistermicrosoft_windows_mimalloc_ForContext(pContext) McGenEventUnregister(&(pContext)->Context.RegistrationHandle)
#define microsoft_windows_mimallocHandle_ForContext(pContext) ((pContext)->Context.RegistrationHandle)
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
FORCEINLINE
_Ret_ McGenContext_microsoft_windows_mimalloc*
_mcgen_CheckContextType_microsoft_windows_mimalloc(_In_ McGenContext_microsoft_windows_mimalloc* pContext)
{
    return pContext;
}
#endif
#define EventEnabledETW_MI_ALLOC() _mcgen_EVENT_BIT_SET(microsoft_windows_mimallocEnableBits, 0)
#define EventEnabledETW_MI_ALLOC_ForContext(pContext) _mcgen_EVENT_BIT_SET(_mcgen_CheckContextType_microsoft_windows_mimalloc(pContext)->EnableBits, 0)
#define EventWriteETW_MI_ALLOC(Address,Size) \
        MCGEN_EVENT_ENABLED(ETW_MI_ALLOC) \
        ? _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC(&ETW_MI_Provider_Context, &ETW_MI_ALLOC, Address, Size) : 0
#define EventWriteETW_MI_ALLOC_AssumeEnabled(Address,Size) \
        _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC(&ETW_MI_Provider_Context, &ETW_MI_ALLOC, Address, Size)
#define EventWriteETW_MI_ALLOC_ForContext(pContext,Address,Size) \
        MCGEN_EVENT_ENABLED_FORCONTEXT(pContext, ETW_MI_ALLOC) \
        ? _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC(&(pContext)->Context, &ETW_MI_ALLOC, Address, Size) : 0
#define EventWriteETW_MI_ALLOC_ForContextAssumeEnabled(pContext,Address,Size) \
        _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC(&_mcgen_CheckContextType_microsoft_windows_mimalloc(pContext)->Context, &ETW_MI_ALLOC, Address, Size)
#define _mcgen_TEMPLATE_FOR_ETW_MI_ALLOC _mcgen_PASTE2(McTemplateU0xx_, MCGEN_EVENTWRITETRANSFER)
#define EventEnabledETW_MI_FREE() _mcgen_EVENT_BIT_SET(microsoft_windows_mimallocEnableBits, 0)
#define EventEnabledETW_MI_FREE_ForContext(pContext) _mcgen_EVENT_BIT_SET(_mcgen_CheckContextType_microsoft_windows_mimalloc(pContext)->EnableBits, 0)
#define EventWriteETW_MI_FREE(Address,Size) \
        MCGEN_EVENT_ENABLED(ETW_MI_FREE) \
        ? _mcgen_TEMPLATE_FOR_ETW_MI_FREE(&ETW_MI_Provider_Context, &ETW_MI_FREE, Address, Size) : 0
#define EventWriteETW_MI_FREE_AssumeEnabled(Address,Size) \
        _mcgen_TEMPLATE_FOR_ETW_MI_FREE(&ETW_MI_Provider_Context, &ETW_MI_FREE, Address, Size)
#define EventWriteETW_MI_FREE_ForContext(pContext,Address,Size) \
        MCGEN_EVENT_ENABLED_FORCONTEXT(pContext, ETW_MI_FREE) \
        ? _mcgen_TEMPLATE_FOR_ETW_MI_FREE(&(pContext)->Context, &ETW_MI_FREE, Address, Size) : 0
#define EventWriteETW_MI_FREE_ForContextAssumeEnabled(pContext,Address,Size) \
        _mcgen_TEMPLATE_FOR_ETW_MI_FREE(&_mcgen_CheckContextType_microsoft_windows_mimalloc(pContext)->Context, &ETW_MI_FREE, Address, Size)
#define _mcgen_TEMPLATE_FOR_ETW_MI_FREE _mcgen_PASTE2(McTemplateU0xx_, MCGEN_EVENTWRITETRANSFER)
#endif
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION
#ifndef McTemplateU0xx_def
#define McTemplateU0xx_def 
ETW_INLINE
ULONG
_mcgen_PASTE2(McTemplateU0xx_, MCGEN_EVENTWRITETRANSFER)(
    _In_ PMCGEN_TRACE_CONTEXT Context,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_ const unsigned __int64 _Arg0,
    _In_ const unsigned __int64 _Arg1
    )
{
#define McTemplateU0xx_ARGCOUNT 2
    EVENT_DATA_DESCRIPTOR EventData[McTemplateU0xx_ARGCOUNT + 1];
    EventDataDescCreate(&EventData[1],&_Arg0, sizeof(const unsigned __int64) );
    EventDataDescCreate(&EventData[2],&_Arg1, sizeof(const unsigned __int64) );
    return McGenEventWrite(Context, Descriptor, NULL, McTemplateU0xx_ARGCOUNT + 1, EventData);
}
#endif
#endif
#if 0
#endif
#define mi_track_init() EventRegistermicrosoft_windows_mimalloc();
#define mi_track_malloc_size(p,reqsize,size,zero) EventWriteETW_MI_ALLOC((UINT64)(p), size)
#define mi_track_free_size(p,size) EventWriteETW_MI_FREE((UINT64)(p), size)
#else
#define MI_TRACK_ENABLED 0
#define MI_TRACK_HEAP_DESTROY 0
#define MI_TRACK_TOOL "none"
#define mi_track_malloc_size(p,reqsize,size,zero) 
#define mi_track_free_size(p,_size) 
#endif
#ifndef mi_track_resize
#define mi_track_resize(p,oldsize,newsize) mi_track_free_size(p,oldsize); mi_track_malloc(p,newsize,false)
#endif
#ifndef mi_track_align
#define mi_track_align(p,alignedp,offset,size) mi_track_mem_noaccess(p,offset)
#endif
#ifndef mi_track_init
#define mi_track_init() 
#endif
#ifndef mi_track_mem_defined
#define mi_track_mem_defined(p,size) 
#endif
#ifndef mi_track_mem_undefined
#define mi_track_mem_undefined(p,size) 
#endif
#ifndef mi_track_mem_noaccess
#define mi_track_mem_noaccess(p,size) 
#endif
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
#endif
#if (MI_DEBUG>0)
#define mi_trace_message(...) _mi_trace_message(__VA_ARGS__)
#else
#define mi_trace_message(...) 
#endif
#define MI_CACHE_LINE 64
#if 0
#elif (defined(__GNUC__) && (__GNUC__ >= 3)) || defined(__clang__)
#define mi_decl_noinline __attribute__((noinline))
#define mi_decl_thread __thread
#define mi_decl_cache_align __attribute__((aligned(MI_CACHE_LINE)))
#define mi_decl_weak __attribute__((weak))
#else
#define mi_decl_noinline 
#define mi_decl_thread __thread
#define mi_decl_cache_align 
#define mi_decl_weak 
#endif
#if defined(__EMSCRIPTEN__) && !defined(__wasi__)
#define __wasi__ 
#endif
#if 0
#else
#define mi_decl_externc 
#endif
void _mi_fputs(mi_output_fun* out, void* arg, const char* prefix, const char* message);
void _mi_fprintf(mi_output_fun* out, void* arg, const char* fmt, ...);
void _mi_warning_message(const char* fmt, ...);
void _mi_verbose_message(const char* fmt, ...);
void _mi_trace_message(const char* fmt, ...);
void _mi_options_init(void);
long _mi_option_get_fast(mi_option_t option);
void _mi_error_message(int err, const char* fmt, ...);
void _mi_random_init(mi_random_ctx_t* ctx);
void _mi_random_init_weak(mi_random_ctx_t* ctx);
void _mi_random_reinit_if_weak(mi_random_ctx_t * ctx);
void _mi_random_split(mi_random_ctx_t* ctx, mi_random_ctx_t* new_ctx);
uintptr_t _mi_random_next(mi_random_ctx_t* ctx);
uintptr_t _mi_heap_random_next(mi_heap_t* heap);
uintptr_t _mi_os_random_weak(uintptr_t extra_seed);
static inline uintptr_t _mi_random_shuffle(uintptr_t x);
extern mi_decl_cache_align mi_stats_t _mi_stats_main;
extern mi_decl_cache_align const mi_page_t _mi_page_empty;
void _mi_process_load(void);
void mi_cdecl _mi_process_done(void);
bool _mi_is_redirected(void);
bool _mi_allocator_init(const char** message);
void _mi_allocator_done(void);
bool _mi_is_main_thread(void);
size_t _mi_current_thread_count(void);
bool _mi_preloading(void);
void _mi_thread_done(mi_heap_t* heap);
void _mi_thread_data_collect(void);
void _mi_tld_init(mi_tld_t* tld, mi_heap_t* bheap);
mi_threadid_t _mi_thread_id(void) mi_attr_noexcept;
mi_heap_t* _mi_heap_main_get(void);
mi_subproc_t* _mi_subproc_from_id(mi_subproc_id_t subproc_id);
void _mi_heap_guarded_init(mi_heap_t* heap);
void _mi_os_init(void);
void* _mi_os_alloc(size_t size, mi_memid_t* memid, mi_stats_t* stats);
void _mi_os_free(void* p, size_t size, mi_memid_t memid, mi_stats_t* stats);
void _mi_os_free_ex(void* p, size_t size, bool still_committed, mi_memid_t memid, mi_stats_t* stats);
size_t _mi_os_page_size(void);
size_t _mi_os_good_alloc_size(size_t size);
bool _mi_os_has_overcommit(void);
bool _mi_os_has_virtual_reserve(void);
bool _mi_os_reset(void* addr, size_t size, mi_stats_t* tld_stats);
bool _mi_os_commit(void* p, size_t size, bool* is_zero, mi_stats_t* stats);
bool _mi_os_decommit(void* addr, size_t size, mi_stats_t* stats);
bool _mi_os_protect(void* addr, size_t size);
bool _mi_os_unprotect(void* addr, size_t size);
bool _mi_os_purge(void* p, size_t size, mi_stats_t* stats);
bool _mi_os_purge_ex(void* p, size_t size, bool allow_reset, mi_stats_t* stats);
void* _mi_os_alloc_aligned(size_t size, size_t alignment, bool commit, bool allow_large, mi_memid_t* memid, mi_stats_t* stats);
void* _mi_os_alloc_aligned_at_offset(size_t size, size_t alignment, size_t align_offset, bool commit, bool allow_large, mi_memid_t* memid, mi_stats_t* tld_stats);
void* _mi_os_get_aligned_hint(size_t try_alignment, size_t size);
bool _mi_os_use_large_page(size_t size, size_t alignment);
size_t _mi_os_large_page_size(void);
void* _mi_os_alloc_huge_os_pages(size_t pages, int numa_node, mi_msecs_t max_secs, size_t* pages_reserved, size_t* psize, mi_memid_t* memid);
mi_arena_id_t _mi_arena_id_none(void);
void _mi_arena_free(void* p, size_t size, size_t still_committed_size, mi_memid_t memid, mi_stats_t* stats);
void* _mi_arena_alloc(size_t size, bool commit, bool allow_large, mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld);
void* _mi_arena_alloc_aligned(size_t size, size_t alignment, size_t align_offset, bool commit, bool allow_large, mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld);
bool _mi_arena_memid_is_suitable(mi_memid_t memid, mi_arena_id_t request_arena_id);
bool _mi_arena_contains(const void* p);
void _mi_arenas_collect(bool force_purge, mi_stats_t* stats);
void _mi_arena_unsafe_destroy_all(mi_stats_t* stats);
bool _mi_arena_segment_clear_abandoned(mi_segment_t* segment);
void _mi_arena_segment_mark_abandoned(mi_segment_t* segment);
void* _mi_arena_meta_zalloc(size_t size, mi_memid_t* memid);
void _mi_arena_meta_free(void* p, mi_memid_t memid, size_t size);
typedef struct mi_arena_field_cursor_s {
  size_t os_list_count;
  size_t start;
  size_t end;
  size_t bitmap_idx;
  mi_subproc_t* subproc;
  bool visit_all;
  bool hold_visit_lock;
} mi_arena_field_cursor_t;
void _mi_arena_field_cursor_init(mi_heap_t* heap, mi_subproc_t* subproc, bool visit_all, mi_arena_field_cursor_t* current);
mi_segment_t* _mi_arena_segment_clear_abandoned_next(mi_arena_field_cursor_t* previous);
void _mi_arena_field_cursor_done(mi_arena_field_cursor_t* current);
void _mi_segment_map_allocated_at(const mi_segment_t* segment);
void _mi_segment_map_freed_at(const mi_segment_t* segment);
mi_page_t* _mi_segment_page_alloc(mi_heap_t* heap, size_t block_size, size_t page_alignment, mi_segments_tld_t* tld, mi_os_tld_t* os_tld);
void _mi_segment_page_free(mi_page_t* page, bool force, mi_segments_tld_t* tld);
void _mi_segment_page_abandon(mi_page_t* page, mi_segments_tld_t* tld);
uint8_t* _mi_segment_page_start(const mi_segment_t* segment, const mi_page_t* page, size_t* page_size);
#if MI_HUGE_PAGE_ABANDON
void _mi_segment_huge_page_free(mi_segment_t* segment, mi_page_t* page, mi_block_t* block);
#else
void _mi_segment_huge_page_reset(mi_segment_t* segment, mi_page_t* page, mi_block_t* block);
#endif
void _mi_segments_collect(bool force, mi_segments_tld_t* tld);
void _mi_abandoned_reclaim_all(mi_heap_t* heap, mi_segments_tld_t* tld);
bool _mi_segment_attempt_reclaim(mi_heap_t* heap, mi_segment_t* segment);
bool _mi_segment_visit_blocks(mi_segment_t* segment, int heap_tag, bool visit_blocks, mi_block_visit_fun* visitor, void* arg);
void* _mi_malloc_generic(mi_heap_t* heap, size_t size, bool zero, size_t huge_alignment) mi_attr_noexcept mi_attr_malloc;
void _mi_page_retire(mi_page_t* page) mi_attr_noexcept;
void _mi_page_unfull(mi_page_t* page);
void _mi_page_free(mi_page_t* page, mi_page_queue_t* pq, bool force);
void _mi_page_abandon(mi_page_t* page, mi_page_queue_t* pq);
void _mi_page_force_abandon(mi_page_t* page);
void _mi_heap_delayed_free_all(mi_heap_t* heap);
bool _mi_heap_delayed_free_partial(mi_heap_t* heap);
void _mi_heap_collect_retired(mi_heap_t* heap, bool force);
void _mi_page_use_delayed_free(mi_page_t* page, mi_delayed_t delay, bool override_never);
bool _mi_page_try_use_delayed_free(mi_page_t* page, mi_delayed_t delay, bool override_never);
size_t _mi_page_queue_append(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_queue_t* append);
void _mi_deferred_free(mi_heap_t* heap, bool force);
void _mi_page_free_collect(mi_page_t* page,bool force);
void _mi_page_reclaim(mi_heap_t* heap, mi_page_t* page);
size_t _mi_bin_size(uint8_t bin);
uint8_t _mi_bin(size_t size);
void _mi_heap_init(mi_heap_t* heap, mi_tld_t* tld, mi_arena_id_t arena_id, bool noreclaim, uint8_t tag);
void _mi_heap_destroy_pages(mi_heap_t* heap);
void _mi_heap_collect_abandon(mi_heap_t* heap);
void _mi_heap_set_default_direct(mi_heap_t* heap);
bool _mi_heap_memid_is_suitable(mi_heap_t* heap, mi_memid_t memid);
void _mi_heap_unsafe_destroy_all(void);
mi_heap_t* _mi_heap_by_tag(mi_heap_t* heap, uint8_t tag);
void _mi_heap_area_init(mi_heap_area_t* area, mi_page_t* page);
bool _mi_heap_area_visit_blocks(const mi_heap_area_t* area, mi_page_t* page, mi_block_visit_fun* visitor, void* arg);
void _mi_stats_done(mi_stats_t* stats);
mi_msecs_t _mi_clock_now(void);
mi_msecs_t _mi_clock_end(mi_msecs_t start);
mi_msecs_t _mi_clock_start(void);
void* _mi_page_malloc_zero(mi_heap_t* heap, mi_page_t* page, size_t size, bool zero) mi_attr_noexcept;
void* _mi_page_malloc(mi_heap_t* heap, mi_page_t* page, size_t size) mi_attr_noexcept;
void* _mi_page_malloc_zeroed(mi_heap_t* heap, mi_page_t* page, size_t size) mi_attr_noexcept;
void* _mi_heap_malloc_zero(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept;
void* _mi_heap_malloc_zero_ex(mi_heap_t* heap, size_t size, bool zero, size_t huge_alignment) mi_attr_noexcept;
void* _mi_heap_realloc_zero(mi_heap_t* heap, void* p, size_t newsize, bool zero) mi_attr_noexcept;
mi_block_t* _mi_page_ptr_unalign(const mi_page_t* page, const void* p);
bool _mi_free_delayed_block(mi_block_t* block);
void _mi_free_generic(mi_segment_t* segment, mi_page_t* page, bool is_local, void* p) mi_attr_noexcept;
void _mi_padding_shrink(const mi_page_t* page, const mi_block_t* block, const size_t min_size);
#include <stdarg.h>
void _mi_vsnprintf(char* buf, size_t bufsize, const char* fmt, va_list args);
void _mi_snprintf(char* buf, size_t buflen, const char* fmt, ...);
char _mi_toupper(char c);
int _mi_strnicmp(const char* s, const char* t, size_t n);
void _mi_strlcpy(char* dest, const char* src, size_t dest_size);
void _mi_strlcat(char* dest, const char* src, size_t dest_size);
size_t _mi_strlen(const char* s);
size_t _mi_strnlen(const char* s, size_t max_len);
bool _mi_getenv(const char* name, char* result, size_t result_size);
#if MI_DEBUG>1
bool _mi_page_is_valid(mi_page_t* page);
#endif
#if defined(__GNUC__) || defined(__clang__)
#define mi_unlikely(x) (__builtin_expect(!!(x),false))
#define mi_likely(x) (__builtin_expect(!!(x),true))
#elif 0
#else
#define mi_unlikely(x) (x)
#define mi_likely(x) (x)
#endif
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif
#include <errno.h>
#ifndef EAGAIN
#define EAGAIN (11)
#endif
#ifndef ENOMEM
#define ENOMEM (12)
#endif
#ifndef EFAULT
#define EFAULT (14)
#endif
#ifndef EINVAL
#define EINVAL (22)
#endif
#ifndef EOVERFLOW
#define EOVERFLOW (75)
#endif
#define MI_UNUSED(x) (void)(x)
#if (MI_DEBUG>0)
#define MI_UNUSED_RELEASE(x) 
#else
#define MI_UNUSED_RELEASE(x) MI_UNUSED(x)
#endif
#define MI_INIT4(x) x(),x(),x(),x()
#define MI_INIT8(x) MI_INIT4(x),MI_INIT4(x)
#define MI_INIT16(x) MI_INIT8(x),MI_INIT8(x)
#define MI_INIT32(x) MI_INIT16(x),MI_INIT16(x)
#define MI_INIT64(x) MI_INIT32(x),MI_INIT32(x)
#define MI_INIT128(x) MI_INIT64(x),MI_INIT64(x)
#define MI_INIT256(x) MI_INIT128(x),MI_INIT128(x)
#include <string.h>
#define _mi_memzero_var(x) memset(&x,0,sizeof(x))
static inline bool _mi_is_power_of_two(uintptr_t x) {
  return ((x & (x - 1)) == 0);
}
static inline bool _mi_is_aligned(void* p, size_t alignment) {
  mi_assert_internal(alignment != 0);
  return (((uintptr_t)p % alignment) == 0);
}
static inline uintptr_t _mi_align_up(uintptr_t sz, size_t alignment) {
  mi_assert_internal(alignment != 0);
  uintptr_t mask = alignment - 1;
  if ((alignment & mask) == 0) {
    return ((sz + mask) & ~mask);
  }
  else {
    return (((sz + mask)/alignment)*alignment);
  }
}
static inline void* mi_align_up_ptr(void* p, size_t alignment) {
  return (void*)_mi_align_up((uintptr_t)p, alignment);
}
static inline uintptr_t _mi_divide_up(uintptr_t size, size_t divider) {
  mi_assert_internal(divider != 0);
  return (divider == 0 ? size : ((size + divider - 1) / divider));
}
static inline size_t _mi_clamp(size_t sz, size_t min, size_t max) {
  if (sz < min) return min;
  else if (sz > max) return max;
  else return sz;
}
static inline bool mi_mem_is_zero(const void* p, size_t size) {
  for (size_t i = 0; i < size; i++) {
    if (((uint8_t*)p)[i] != 0) return false;
  }
  return true;
}
static inline size_t _mi_wsize_from_size(size_t size) {
  mi_assert_internal(size <= SIZE_MAX - sizeof(uintptr_t));
  return (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
}
#if __has_builtin(__builtin_umul_overflow) || (defined(__GNUC__) && (__GNUC__ >= 5))
#include <limits.h>
#if defined(_CLOCK_T)
#undef _CLOCK_T
#endif
static inline bool mi_mul_overflow(size_t count, size_t size, size_t* total) {
  #if (SIZE_MAX == ULONG_MAX)
    return __builtin_umull_overflow(count, size, (unsigned long *)total);
  #elif (SIZE_MAX == UINT_MAX)
    return __builtin_umul_overflow(count, size, (unsigned int *)total);
  #else
    return __builtin_umulll_overflow(count, size, (unsigned long long *)total);
  #endif
}
#else
static inline bool mi_mul_overflow(size_t count, size_t size, size_t* total) {
  #define MI_MUL_COULD_OVERFLOW ((size_t)1 << (4*sizeof(size_t)))
  *total = count * size;
  return ((size >= MI_MUL_COULD_OVERFLOW || count >= MI_MUL_COULD_OVERFLOW) && size > 0 && (SIZE_MAX / size) < count);
}
#endif
static inline bool mi_count_size_overflow(size_t count, size_t size, size_t* total) {
  if (count==1) {
    *total = size;
    return false;
  }
  else if mi_unlikely(mi_mul_overflow(count, size, total)) {
    #if MI_DEBUG > 0
    _mi_error_message(EOVERFLOW, "allocation request is too large (%zu * %zu bytes)\n", count, size);
    #endif
    *total = SIZE_MAX;
    return true;
  }
  else return false;
}
extern const mi_heap_t _mi_heap_empty;
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
static inline mi_page_t* _mi_heap_get_free_small_page(mi_heap_t* heap, size_t size) {
  mi_assert_internal(size <= (MI_SMALL_SIZE_MAX + MI_PADDING_SIZE));
  const size_t idx = _mi_wsize_from_size(size);
  mi_assert_internal(idx < MI_PAGES_DIRECT);
  return heap->pages_free_direct[idx];
}
static inline mi_segment_t* _mi_ptr_segment(const void* p) {
  mi_segment_t* const segment = (mi_segment_t*)(((uintptr_t)p - 1) & ~MI_SEGMENT_MASK);
  #if MI_INTPTR_SIZE <= 4
  return (p==NULL ? NULL : segment);
  #else
  return ((intptr_t)segment <= 0 ? NULL : segment);
  #endif
}
static inline mi_segment_t* _mi_page_segment(const mi_page_t* page) {
  mi_assert_internal(page!=NULL);
  mi_segment_t* segment = _mi_ptr_segment(page);
  mi_assert_internal(segment == NULL || page == &segment->pages[page->segment_idx]);
  return segment;
}
static inline size_t _mi_segment_page_idx_of(const mi_segment_t* segment, const void* p) {
  ptrdiff_t diff = (uint8_t*)p - (uint8_t*)segment;
  mi_assert_internal(diff >= 0 && (size_t)diff <= MI_SEGMENT_SIZE );
  size_t idx = (size_t)diff >> segment->page_shift;
  mi_assert_internal(idx < segment->capacity);
  mi_assert_internal(segment->page_kind <= MI_PAGE_MEDIUM || idx == 0);
  return idx;
}
static inline mi_page_t* _mi_segment_page_of(const mi_segment_t* segment, const void* p) {
  size_t idx = _mi_segment_page_idx_of(segment, p);
  return &((mi_segment_t*)segment)->pages[idx];
}
static inline uint8_t* mi_page_start(const mi_page_t* page) {
  mi_assert_internal(page->page_start != NULL);
  mi_assert_expensive(_mi_segment_page_start(_mi_page_segment(page),page,NULL) == page->page_start);
  return page->page_start;
}
static inline mi_page_t* _mi_ptr_page(void* p) {
  mi_assert_internal(p!=NULL);
  return _mi_segment_page_of(_mi_ptr_segment(p), p);
}
static inline size_t mi_page_block_size(const mi_page_t* page) {
  mi_assert_internal(page->block_size > 0);
  return page->block_size;
}
static inline bool mi_page_is_huge(const mi_page_t* page) {
  mi_assert_internal((page->is_huge && _mi_page_segment(page)->page_kind == MI_PAGE_HUGE) ||
                     (!page->is_huge && _mi_page_segment(page)->page_kind != MI_PAGE_HUGE));
  return page->is_huge;
}
static inline size_t mi_page_usable_block_size(const mi_page_t* page) {
  return mi_page_block_size(page) - MI_PADDING_SIZE;
}
static inline size_t mi_segment_size(mi_segment_t* segment) {
  return segment->segment_size;
}
static inline mi_block_t* mi_page_thread_free(const mi_page_t* page) {
  return (mi_block_t*)(mi_atomic_load_relaxed(&((mi_page_t*)page)->xthread_free) & ~3);
}
static inline mi_delayed_t mi_page_thread_free_flag(const mi_page_t* page) {
  return (mi_delayed_t)(mi_atomic_load_relaxed(&((mi_page_t*)page)->xthread_free) & 3);
}
static inline mi_heap_t* mi_page_heap(const mi_page_t* page) {
  return (mi_heap_t*)(mi_atomic_load_relaxed(&((mi_page_t*)page)->xheap));
}
static inline void mi_page_set_heap(mi_page_t* page, mi_heap_t* heap) {
  mi_assert_internal(mi_page_thread_free_flag(page) != MI_DELAYED_FREEING);
  mi_atomic_store_release(&page->xheap,(uintptr_t)heap);
  if (heap != NULL) { page->heap_tag = heap->tag; }
}
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
static inline bool mi_page_all_free(const mi_page_t* page) {
  mi_assert_internal(page != NULL);
  return (page->used == 0);
}
static inline bool mi_page_has_any_available(const mi_page_t* page) {
  mi_assert_internal(page != NULL && page->reserved > 0);
  return (page->used < page->reserved || (mi_page_thread_free(page) != NULL));
}
static inline bool mi_page_immediate_available(const mi_page_t* page) {
  mi_assert_internal(page != NULL);
  return (page->free != NULL);
}
static inline bool mi_page_mostly_used(const mi_page_t* page) {
  if (page==NULL) return true;
  uint16_t frac = page->reserved / 8U;
  return (page->reserved - page->used <= frac);
}
static inline mi_page_queue_t* mi_page_queue(const mi_heap_t* heap, size_t size) {
  return &((mi_heap_t*)heap)->pages[_mi_bin(size)];
}
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
#if MI_GUARDED
static inline bool mi_block_ptr_is_guarded(const mi_block_t* block, const void* p) {
  const ptrdiff_t offset = (uint8_t*)p - (uint8_t*)block;
  return (offset >= (ptrdiff_t)(sizeof(mi_block_t)) && block->next == MI_BLOCK_TAG_GUARDED);
}
static inline bool mi_heap_malloc_use_guarded(mi_heap_t* heap, size_t size) {
  const size_t count = heap->guarded_sample_count - 1;
  if mi_likely(count != 0) {
    heap->guarded_sample_count = count;
    return false;
  }
  else if (size >= heap->guarded_size_min && size <= heap->guarded_size_max) {
    heap->guarded_sample_count = heap->guarded_sample_rate;
    return (heap->guarded_sample_rate != 0);
  }
  else {
    if (heap->guarded_sample_rate != 0) { heap->guarded_sample_count = 1; }
    return false;
  }
}
mi_decl_restrict void* _mi_heap_malloc_guarded(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept;
#endif
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
  #ifdef MI_BIG_ENDIAN
  return (x & 0x00FFFFFF);
  #else
  return (x & 0xFFFFFF00);
  #endif
}
static inline mi_block_t* mi_block_nextx( const void* null, const mi_block_t* block, const uintptr_t* keys ) {
  mi_track_mem_defined(block,sizeof(mi_block_t));
  mi_block_t* next;
  #ifdef MI_ENCODE_FREELIST
  next = (mi_block_t*)mi_ptr_decode(null, block->next, keys);
  #else
  MI_UNUSED(keys); MI_UNUSED(null);
  next = (mi_block_t*)block->next;
  #endif
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
  #endif
  mi_track_mem_noaccess(block,sizeof(mi_block_t));
}
static inline mi_block_t* mi_block_next(const mi_page_t* page, const mi_block_t* block) {
  #ifdef MI_ENCODE_FREELIST
  mi_block_t* next = mi_block_nextx(page,block,page->keys);
  if mi_unlikely(next!=NULL && !mi_is_in_same_page(block, next)) {
    _mi_error_message(EFAULT, "corrupted free list entry of size %zub at %p: value 0x%zx\n", mi_page_block_size(page), block, (uintptr_t)next);
    next = NULL;
  }
  return next;
  #else
  MI_UNUSED(page);
  return mi_block_nextx(page,block,NULL);
  #endif
}
static inline void mi_block_set_next(const mi_page_t* page, mi_block_t* block, const mi_block_t* next) {
  #ifdef MI_ENCODE_FREELIST
  mi_block_set_nextx(page,block,next, page->keys);
  #else
  MI_UNUSED(page);
  mi_block_set_nextx(page,block,next,NULL);
  #endif
}
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
static inline uintptr_t _mi_random_shuffle(uintptr_t x) {
  if (x==0) { x = 17; }
#if (MI_INTPTR_SIZE>=8)
  x ^= x >> 30;
  x *= 0xbf58476d1ce4e5b9UL;
  x ^= x >> 27;
  x *= 0x94d049bb133111ebUL;
  x ^= x >> 31;
#elif (MI_INTPTR_SIZE==4)
  x ^= x >> 16;
  x *= 0x7feb352dUL;
  x ^= x >> 15;
  x *= 0x846ca68bUL;
  x ^= x >> 16;
#endif
  return x;
}
int _mi_os_numa_node_get(mi_os_tld_t* tld);
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
#if defined(__GNUC__)
#include <limits.h>
#define MI_HAVE_FAST_BITSCAN 
static inline size_t mi_clz(uintptr_t x) {
  if (x==0) return MI_INTPTR_BITS;
#if (INTPTR_MAX == LONG_MAX)
  return __builtin_clzl(x);
#else
  return __builtin_clzll(x);
#endif
}
static inline size_t mi_ctz(uintptr_t x) {
  if (x==0) return MI_INTPTR_BITS;
#if (INTPTR_MAX == LONG_MAX)
  return __builtin_ctzl(x);
#else
  return __builtin_ctzll(x);
#endif
}
#elif 0
#if 0
#else
#endif
#if 0
#else
#endif
#else
static inline size_t mi_ctz32(uint32_t x) {
  static const unsigned char debruijn[32] = {
    0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
  };
  if (x==0) return 32;
  return debruijn[((x & -(int32_t)x) * 0x077CB531UL) >> 27];
}
static inline size_t mi_clz32(uint32_t x) {
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
#endif
}
static inline size_t mi_ctz(uintptr_t x) {
  if (x==0) return MI_INTPTR_BITS;
#if (MI_INTPTR_BITS <= 32)
  return mi_ctz32((uint32_t)x);
#else
  size_t count = mi_ctz32((uint32_t)x);
  if (count < 32) return count;
  return (32 + mi_ctz32((uint32_t)(x>>32)));
#endif
}
#endif
static inline size_t mi_bsr(uintptr_t x) {
  return (x==0 ? MI_INTPTR_BITS : MI_INTPTR_BITS - 1 - mi_clz(x));
}
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
#endif
#if (defined(__GNUC__) && (__GNUC__ >= 4)) || defined(__clang__)
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
static inline void _mi_memcpy_aligned(void* dst, const void* src, size_t n) {
  mi_assert_internal(((uintptr_t)dst % MI_INTPTR_SIZE == 0) && ((uintptr_t)src % MI_INTPTR_SIZE == 0));
  _mi_memcpy(dst, src, n);
}
static inline void _mi_memzero_aligned(void* dst, size_t n) {
  mi_assert_internal((uintptr_t)dst % MI_INTPTR_SIZE == 0);
  _mi_memzero(dst, n);
}
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
typedef struct mi_os_mem_config_s {
  size_t page_size;
  size_t large_page_size;
  size_t alloc_granularity;
  size_t physical_memory;
  size_t virtual_address_bits;
  bool has_overcommit;
  bool has_partial_free;
  bool has_virtual_reserve;
} mi_os_mem_config_t;
void _mi_prim_mem_init( mi_os_mem_config_t* config );
int _mi_prim_free(void* addr, size_t size );
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr);
int _mi_prim_commit(void* addr, size_t size, bool* is_zero);
int _mi_prim_decommit(void* addr, size_t size, bool* needs_recommit);
int _mi_prim_reset(void* addr, size_t size);
int _mi_prim_protect(void* addr, size_t size, bool protect);
int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr);
size_t _mi_prim_numa_node(void);
size_t _mi_prim_numa_node_count(void);
mi_msecs_t _mi_prim_clock_now(void);
typedef struct mi_process_info_s {
  mi_msecs_t elapsed;
  mi_msecs_t utime;
  mi_msecs_t stime;
  size_t current_rss;
  size_t peak_rss;
  size_t current_commit;
  size_t peak_commit;
  size_t page_faults;
} mi_process_info_t;
void _mi_prim_process_info(mi_process_info_t* pinfo);
void _mi_prim_out_stderr( const char* msg );
bool _mi_prim_getenv(const char* name, char* result, size_t result_size);
bool _mi_prim_random_buf(void* buf, size_t buf_len);
void _mi_prim_thread_init_auto_done(void);
void _mi_prim_thread_done_auto_done(void);
void _mi_prim_thread_associate_default_heap(mi_heap_t* heap);
#if defined(__GNUC__) && ( \
           (defined(__GLIBC__) && (defined(__x86_64__) || defined(__i386__) || (defined(__arm__) && __ARM_ARCH >= 7) || defined(__aarch64__))) \
        || (defined(__APPLE__) && (defined(__x86_64__) || defined(__aarch64__) || defined(__POWERPC__))) \
        || (defined(__BIONIC__) && (defined(__x86_64__) || defined(__i386__) || (defined(__arm__) && __ARM_ARCH >= 7) || defined(__aarch64__))) \
        || (defined(__FreeBSD__) && (defined(__x86_64__) || defined(__i386__) || defined(__aarch64__))) \
        || (defined(__OpenBSD__) && (defined(__x86_64__) || defined(__i386__) || defined(__aarch64__))) \
      )
#define MI_HAS_TLS_SLOT 
static inline void* mi_prim_tls_slot(size_t slot) mi_attr_noexcept {
  void* res;
  const size_t ofs = (slot*sizeof(void*));
  #if defined(__i386__)
    __asm__("movl %%gs:%1, %0" : "=r" (res) : "m" (*((void**)ofs)) : );
  #elif defined(__APPLE__) && defined(__x86_64__)
    __asm__("movq %%gs:%1, %0" : "=r" (res) : "m" (*((void**)ofs)) : );
  #elif defined(__x86_64__) && (MI_INTPTR_SIZE==4)
    __asm__("movl %%fs:%1, %0" : "=r" (res) : "m" (*((void**)ofs)) : );
  #elif defined(__x86_64__)
    __asm__("movq %%fs:%1, %0" : "=r" (res) : "m" (*((void**)ofs)) : );
  #elif defined(__arm__)
    void** tcb; MI_UNUSED(ofs);
    __asm__ volatile ("mrc p15, 0, %0, c13, c0, 3\nbic %0, %0, #3" : "=r" (tcb));
    res = tcb[slot];
  #elif defined(__aarch64__)
    void** tcb; MI_UNUSED(ofs);
    #if defined(__APPLE__)
    __asm__ volatile ("mrs %0, tpidrro_el0\nbic %0, %0, #7" : "=r" (tcb));
    #else
    __asm__ volatile ("mrs %0, tpidr_el0" : "=r" (tcb));
    #endif
    res = tcb[slot];
  #elif defined(__APPLE__) && defined(__POWERPC__)
    MI_UNUSED(ofs);
    res = pthread_getspecific(slot);
  #endif
  return res;
}
static inline void mi_prim_tls_slot_set(size_t slot, void* value) mi_attr_noexcept {
  const size_t ofs = (slot*sizeof(void*));
  #if defined(__i386__)
    __asm__("movl %1,%%gs:%0" : "=m" (*((void**)ofs)) : "rn" (value) : );
  #elif defined(__APPLE__) && defined(__x86_64__)
    __asm__("movq %1,%%gs:%0" : "=m" (*((void**)ofs)) : "rn" (value) : );
  #elif defined(__x86_64__) && (MI_INTPTR_SIZE==4)
    __asm__("movl %1,%%fs:%0" : "=m" (*((void**)ofs)) : "rn" (value) : );
  #elif defined(__x86_64__)
    __asm__("movq %1,%%fs:%0" : "=m" (*((void**)ofs)) : "rn" (value) : );
  #elif defined(__arm__)
    void** tcb; MI_UNUSED(ofs);
    __asm__ volatile ("mrc p15, 0, %0, c13, c0, 3\nbic %0, %0, #3" : "=r" (tcb));
    tcb[slot] = value;
  #elif defined(__aarch64__)
    void** tcb; MI_UNUSED(ofs);
    #if defined(__APPLE__)
    __asm__ volatile ("mrs %0, tpidrro_el0\nbic %0, %0, #7" : "=r" (tcb));
    #else
    __asm__ volatile ("mrs %0, tpidr_el0" : "=r" (tcb));
    #endif
    tcb[slot] = value;
  #elif defined(__APPLE__) && defined(__POWERPC__)
    MI_UNUSED(ofs);
    pthread_setspecific(slot, value);
  #endif
}
#endif
#if !defined(__APPLE__) \
    && !defined(MI_LIBC_MUSL) \
    && (!defined(__clang_major__) || __clang_major__ >= 14)
  #if (defined(__GNUC__) && (__GNUC__ >= 7) && defined(__aarch64__)) \
      || (defined(__GNUC__) && (__GNUC__ >= 11) && defined(__x86_64__)) \
      || (defined(__clang_major__) && (__clang_major__ >= 14) && (defined(__aarch64__) || defined(__x86_64__)))
    #define MI_USE_BUILTIN_THREAD_POINTER 1
  #endif
#endif
extern mi_decl_thread mi_heap_t* _mi_heap_default;
extern bool _mi_process_is_initialized;
static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept;
#if defined(MI_PRIM_THREAD_ID)
static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept {
  return MI_PRIM_THREAD_ID();
}
#elif defined(_WIN32)
static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept {
  return (uintptr_t)NtCurrentTeb();
}
#elif 0
#elif defined(MI_HAS_TLS_SLOT)
static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept {
  #if defined(__BIONIC__)
    return (uintptr_t)mi_prim_tls_slot(1);
  #else
    return (uintptr_t)mi_prim_tls_slot(0);
  #endif
}
#else
static inline mi_threadid_t _mi_prim_thread_id(void) mi_attr_noexcept {
  return (uintptr_t)&_mi_heap_default;
}
#endif
static inline mi_heap_t* mi_prim_get_default_heap(void);
#if 0
#if 0
#elif 0
#elif 0
#endif
#endif
#if defined(MI_TLS_SLOT)
# if !defined(MI_HAS_TLS_SLOT)
# error "trying to use a TLS slot for the default heap, but the mi_prim_tls_slot primitives are not defined"
# endif
static inline mi_heap_t* mi_prim_get_default_heap(void) {
  mi_heap_t* heap = (mi_heap_t*)mi_prim_tls_slot(MI_TLS_SLOT);
  if mi_unlikely(heap == NULL) {
    #ifdef __GNUC__
    __asm("");
    #endif
    heap = (mi_heap_t*)&_mi_heap_empty;
  }
  return heap;
}
#elif defined(MI_TLS_PTHREAD_SLOT_OFS)
static inline mi_heap_t** mi_prim_tls_pthread_heap_slot(void) {
  pthread_t self = pthread_self();
  #if defined(__DragonFly__)
  if (self==NULL) return NULL;
  #endif
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
#else
static inline mi_heap_t* mi_prim_get_default_heap(void) {
  #if defined(MI_TLS_RECURSE_GUARD)
  if (mi_unlikely(!_mi_process_is_initialized)) return _mi_heap_main_get();
  #endif
  return _mi_heap_default;
}
#endif
#include <string.h>
#include <stdlib.h>
#define MI_IN_ALLOC_C 
#if !defined(MI_IN_ALLOC_C)
#error "this file should be included from 'alloc.c' (so aliases can work)"
#endif
#if 0
#endif
#if 0
#if 0
#endif
#if 0
#if 0
  #else
  #endif
#else
#endif
#if 0
#if 0
    #endif
#if 0
    #endif
#if 0
    #else
    #endif
#if 0
  #endif
#if 0
  #endif
#elif 0
#else
#if 0
  #endif
#if 0
  #endif
#endif
#if 0
#endif
#if 0
#if 0
#if 0
    #endif
  #endif
#if 0
  #endif
#elif 0
#if 0
#elif 0
  #else
  #endif
#endif
#if 0
#endif
#if 0
#if 0
  #else
  #endif
#if 0
  #endif
#endif
#if 0
#elif 0
#endif
#if 0
#endif
#if 0
#endif
#endif
#if !defined(MI_IN_ALLOC_C)
#error "this file should be included from 'alloc.c' (so aliases can work from alloc-override)"
#endif
static void mi_check_padding(const mi_page_t* page, const mi_block_t* block);
static bool mi_check_is_double_free(const mi_page_t* page, const mi_block_t* block);
static size_t mi_page_usable_size_of(const mi_page_t* page, const mi_block_t* block);
static void mi_stat_free(const mi_page_t* page, const mi_block_t* block);
static mi_decl_noinline void mi_free_block_mt(mi_page_t* page, mi_segment_t* segment, mi_block_t* block);
static inline void mi_free_block_local(mi_page_t* page, mi_block_t* block, bool track_stats, bool check_full)
{
  if mi_unlikely(mi_check_is_double_free(page, block)) return;
  mi_check_padding(page, block);
  if (track_stats) { mi_stat_free(page, block); }
  #if (MI_DEBUG>0) && !MI_TRACK_ENABLED && !MI_TSAN && !MI_GUARDED
  memset(block, MI_DEBUG_FREED, mi_page_block_size(page));
  #endif
  if (track_stats) { mi_track_free_size(block, mi_page_usable_size_of(page, block)); }
  mi_block_set_next(page, block, page->local_free);
  page->local_free = block;
  if mi_unlikely(--page->used == 0) {
    _mi_page_retire(page);
  }
  else if mi_unlikely(check_full && mi_page_is_in_full(page)) {
    _mi_page_unfull(page);
  }
}
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
#if MI_GUARDED
static void mi_block_unguard(mi_page_t* page, mi_block_t* block, void* p);
static inline void mi_block_check_unguard(mi_page_t* page, mi_block_t* block, void* p) {
  if (mi_block_ptr_is_guarded(block, p)) { mi_block_unguard(page, block, p); }
}
#else
static inline void mi_block_check_unguard(mi_page_t* page, mi_block_t* block, void* p) {
  MI_UNUSED(page); MI_UNUSED(block); MI_UNUSED(p);
}
#endif
static void mi_decl_noinline mi_free_generic_local(mi_page_t* page, mi_segment_t* segment, void* p) mi_attr_noexcept {
  MI_UNUSED(segment);
  mi_block_t* const block = (mi_page_has_aligned(page) ? _mi_page_ptr_unalign(page, p) : (mi_block_t*)p);
  mi_block_check_unguard(page, block, p);
  mi_free_block_local(page, block, true , true );
}
static void mi_decl_noinline mi_free_generic_mt(mi_page_t* page, mi_segment_t* segment, void* p) mi_attr_noexcept {
  mi_block_t* const block = _mi_page_ptr_unalign(page, p);
  mi_block_check_unguard(page, block, p);
  mi_free_block_mt(page, segment, block);
}
void mi_decl_noinline _mi_free_generic(mi_segment_t* segment, mi_page_t* page, bool is_local, void* p) mi_attr_noexcept {
  if (is_local) mi_free_generic_local(page,segment,p);
           else mi_free_generic_mt(page,segment,p);
}
static inline mi_segment_t* mi_checked_ptr_segment(const void* p, const char* msg)
{
  MI_UNUSED(msg);
  #if (MI_DEBUG>0)
  if mi_unlikely(((uintptr_t)p & (MI_INTPTR_SIZE - 1)) != 0 && !mi_option_is_enabled(mi_option_guarded_precise)) {
    _mi_error_message(EINVAL, "%s: invalid (unaligned) pointer: %p\n", msg, p);
    return NULL;
  }
  #endif
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
  #endif
  #if (MI_DEBUG>0 || MI_SECURE>=4)
  if mi_unlikely(_mi_ptr_cookie(segment) != segment->cookie) {
    _mi_error_message(EINVAL, "%s: pointer does not point to a valid heap space: %p\n", msg, p);
    return NULL;
  }
  #endif
  return segment;
}
void mi_free(void* p) mi_attr_noexcept
{
  mi_segment_t* const segment = mi_checked_ptr_segment(p,"mi_free");
  if mi_unlikely(segment==NULL) return;
  const bool is_local = (_mi_prim_thread_id() == mi_atomic_load_relaxed(&segment->thread_id));
  mi_page_t* const page = _mi_segment_page_of(segment, p);
  if mi_likely(is_local) {
    if mi_likely(page->flags.full_aligned == 0) {
      mi_block_t* const block = (mi_block_t*)p;
      mi_free_block_local(page, block, true , false );
    }
    else {
      mi_free_generic_local(page, segment, p);
    }
  }
  else {
    mi_free_generic_mt(page, segment, p);
  }
}
bool _mi_free_delayed_block(mi_block_t* block) {
  mi_assert_internal(block!=NULL);
  const mi_segment_t* const segment = _mi_ptr_segment(block);
  mi_assert_internal(_mi_ptr_cookie(segment) == segment->cookie);
  mi_assert_internal(_mi_thread_id() == segment->thread_id);
  mi_page_t* const page = _mi_segment_page_of(segment, block);
  if (!_mi_page_try_use_delayed_free(page, MI_USE_DELAYED_FREE, false )) {
    return false;
  }
  _mi_page_free_collect(page, false);
  mi_free_block_local(page, block, false , true );
  return true;
}
static void mi_decl_noinline mi_free_block_delayed_mt( mi_page_t* page, mi_block_t* block )
{
  mi_thread_free_t tfreex;
  bool use_delayed;
  mi_thread_free_t tfree = mi_atomic_load_relaxed(&page->xthread_free);
  do {
    use_delayed = (mi_tf_delayed(tfree) == MI_USE_DELAYED_FREE);
    if mi_unlikely(use_delayed) {
      tfreex = mi_tf_set_delayed(tfree,MI_DELAYED_FREEING);
    }
    else {
      mi_block_set_next(page, block, mi_tf_block(tfree));
      tfreex = mi_tf_set_block(tfree,block);
    }
  } while (!mi_atomic_cas_weak_release(&page->xthread_free, &tfree, tfreex));
  if mi_unlikely(use_delayed) {
    mi_heap_t* const heap = (mi_heap_t*)(mi_atomic_load_acquire(&page->xheap));
    mi_assert_internal(heap != NULL);
    if (heap != NULL) {
      mi_block_t* dfree = mi_atomic_load_ptr_relaxed(mi_block_t, &heap->thread_delayed_free);
      do {
        mi_block_set_nextx(heap,block,dfree, heap->keys);
      } while (!mi_atomic_cas_ptr_weak_release(mi_block_t,&heap->thread_delayed_free, &dfree, block));
    }
    tfree = mi_atomic_load_relaxed(&page->xthread_free);
    do {
      tfreex = tfree;
      mi_assert_internal(mi_tf_delayed(tfree) == MI_DELAYED_FREEING);
      tfreex = mi_tf_set_delayed(tfree,MI_NO_DELAYED_FREE);
    } while (!mi_atomic_cas_weak_release(&page->xthread_free, &tfree, tfreex));
  }
}
static void mi_decl_noinline mi_free_block_mt(mi_page_t* page, mi_segment_t* segment, mi_block_t* block)
{
  if (_mi_option_get_fast(mi_option_abandoned_reclaim_on_free) != 0 &&
      #if MI_HUGE_PAGE_ABANDON
      segment->page_kind != MI_PAGE_HUGE &&
      #endif
      mi_atomic_load_relaxed(&segment->thread_id) == 0 &&
      mi_prim_get_default_heap() != (mi_heap_t*)&_mi_heap_empty)
  {
    if (_mi_segment_attempt_reclaim(mi_heap_get_default(), segment)) {
      mi_assert_internal(_mi_thread_id() == mi_atomic_load_relaxed(&segment->thread_id));
      mi_assert_internal(mi_heap_get_default()->tld->segments.subproc == segment->subproc);
      mi_free(block);
      return;
    }
  }
  mi_check_padding(page, block);
  mi_stat_free(page, block);
  mi_track_free_size(block, mi_page_usable_size_of(page,block));
  _mi_padding_shrink(page, block, sizeof(mi_block_t));
  if (segment->page_kind == MI_PAGE_HUGE) {
    #if MI_HUGE_PAGE_ABANDON
    _mi_segment_huge_page_free(segment, page, block);
    return;
    #else
    _mi_segment_huge_page_reset(segment, page, block);
    #endif
  }
  else {
    #if (MI_DEBUG>0) && !MI_TRACK_ENABLED && !MI_TSAN
    memset(block, MI_DEBUG_FREED, mi_usable_size(block));
    #endif
  }
  mi_free_block_delayed_mt(page,block);
}
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
  #endif
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
    return mi_page_usable_aligned_size_of(page, p);
  }
}
mi_decl_nodiscard size_t mi_usable_size(const void* p) mi_attr_noexcept {
  return _mi_usable_size(p, "mi_usable_size");
}
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
#if (MI_ENCODE_FREELIST && (MI_SECURE>=4 || MI_DEBUG!=0))
static bool mi_list_contains(const mi_page_t* page, const mi_block_t* list, const mi_block_t* elem) {
  while (list != NULL) {
    if (elem==list) return true;
    list = mi_block_next(page, list);
  }
  return false;
}
static mi_decl_noinline bool mi_check_is_double_freex(const mi_page_t* page, const mi_block_t* block) {
  if (mi_list_contains(page, page->free, block) ||
      mi_list_contains(page, page->local_free, block) ||
      mi_list_contains(page, mi_page_thread_free(page), block))
  {
    _mi_error_message(EAGAIN, "double free detected of block %p with size %zu\n", block, mi_page_block_size(page));
    return true;
  }
  return false;
}
#define mi_track_page(page,access) { size_t psize; void* pstart = _mi_page_start(_mi_page_segment(page),page,&psize); mi_track_mem_ ##access( pstart, psize); }
static inline bool mi_check_is_double_free(const mi_page_t* page, const mi_block_t* block) {
  bool is_double_free = false;
  mi_block_t* n = mi_block_nextx(page, block, page->keys);
  if (((uintptr_t)n & (MI_INTPTR_SIZE-1))==0 &&
      (n==NULL || mi_is_in_same_page(block, n)))
  {
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
#endif
#if MI_PADDING
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
static size_t mi_page_usable_size_of(const mi_page_t* page, const mi_block_t* block) {
  size_t bsize;
  size_t delta;
  bool ok = mi_page_decode_padding(page, block, &delta, &bsize);
  mi_assert_internal(ok); mi_assert_internal(delta <= bsize);
  return (ok ? bsize - delta : 0);
}
void _mi_padding_shrink(const mi_page_t* page, const mi_block_t* block, const size_t min_size) {
  size_t bsize;
  size_t delta;
  bool ok = mi_page_decode_padding(page, block, &delta, &bsize);
  mi_assert_internal(ok);
  if (!ok || (bsize - delta) >= min_size) return;
  mi_assert_internal(bsize >= min_size);
  if (bsize < min_size) return;
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
#endif
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
    const size_t maxpad = (delta > MI_MAX_ALIGN_SIZE ? MI_MAX_ALIGN_SIZE : delta);
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
#endif
#if (MI_STAT>0)
static void mi_stat_free(const mi_page_t* page, const mi_block_t* block) {
#if (MI_STAT < 2)
  MI_UNUSED(block);
#endif
  mi_heap_t* const heap = mi_heap_get_default();
  const size_t bsize = mi_page_usable_block_size(page);
#if (MI_STAT>1)
  const size_t usize = mi_page_usable_size_of(page, block);
  mi_heap_stat_decrease(heap, malloc, usize);
#endif
  if (bsize <= MI_LARGE_OBJ_SIZE_MAX) {
    mi_heap_stat_decrease(heap, normal, bsize);
#if (MI_STAT > 1)
    mi_heap_stat_decrease(heap, normal_bins[_mi_bin(bsize)], 1);
#endif
  }
  else {
    const size_t bpsize = mi_page_block_size(page);
    mi_heap_stat_decrease(heap, huge, bpsize);
  }
}
#else
static void mi_stat_free(const mi_page_t* page, const mi_block_t* block) {
  MI_UNUSED(page); MI_UNUSED(block);
}
#endif
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
#endif
#undef MI_IN_ALLOC_C
extern inline void* _mi_page_malloc_zero(mi_heap_t* heap, mi_page_t* page, size_t size, bool zero) mi_attr_noexcept
{
  mi_assert_internal(page->block_size == 0 || mi_page_block_size(page) >= size);
  mi_block_t* const block = page->free;
  if mi_unlikely(block == NULL) {
    return _mi_malloc_generic(heap, size, zero, 0);
  }
  mi_assert_internal(block != NULL && _mi_ptr_page(block) == page);
  page->free = mi_block_next(page, block);
  page->used++;
  mi_assert_internal(page->free == NULL || _mi_ptr_page(page->free) == page);
  mi_assert_internal(page->block_size < MI_MAX_ALIGN_SIZE || _mi_is_aligned(block, MI_MAX_ALIGN_SIZE));
  #if MI_DEBUG>3
  if (page->free_is_zero && size > sizeof(*block)) {
    mi_assert_expensive(mi_mem_is_zero(block+1,size - sizeof(*block)));
  }
  #endif
  mi_track_mem_undefined(block, mi_page_usable_block_size(page));
  if mi_unlikely(zero) {
    mi_assert_internal(page->block_size != 0);
    mi_assert_internal(!mi_page_is_huge(page));
    #if MI_PADDING
    mi_assert_internal(page->block_size >= MI_PADDING_SIZE);
    #endif
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
  if (!zero) { block->next = 0; }
  #endif
  #if (MI_STAT>0)
  const size_t bsize = mi_page_usable_block_size(page);
  if (bsize <= MI_LARGE_OBJ_SIZE_MAX) {
    mi_heap_stat_increase(heap, normal, bsize);
    mi_heap_stat_counter_increase(heap, normal_count, 1);
    #if (MI_STAT>1)
    const size_t bin = _mi_bin(bsize);
    mi_heap_stat_increase(heap, normal_bins[bin], 1);
    #endif
  }
  #endif
  #if MI_PADDING
    mi_padding_t* const padding = (mi_padding_t*)((uint8_t*)block + mi_page_usable_block_size(page));
    ptrdiff_t delta = ((uint8_t*)padding - (uint8_t*)block - (size - MI_PADDING_SIZE));
    #if (MI_DEBUG>=2)
    mi_assert_internal(delta >= 0 && mi_page_usable_block_size(page) >= (size - MI_PADDING_SIZE + delta));
    #endif
    mi_track_mem_defined(padding,sizeof(mi_padding_t));
    padding->canary = mi_ptr_encode_canary(page,block,page->keys);
    padding->delta = (uint32_t)(delta);
    #if MI_PADDING_CHECK
    if (!mi_page_is_huge(page)) {
      uint8_t* fill = (uint8_t*)padding - delta;
      const size_t maxpad = (delta > MI_MAX_ALIGN_SIZE ? MI_MAX_ALIGN_SIZE : delta);
      for (size_t i = 0; i < maxpad; i++) { fill[i] = MI_DEBUG_PADDING; }
    }
    #endif
  #endif
  return block;
}
extern void* _mi_page_malloc(mi_heap_t* heap, mi_page_t* page, size_t size) mi_attr_noexcept {
  return _mi_page_malloc_zero(heap,page,size,false);
}
extern void* _mi_page_malloc_zeroed(mi_heap_t* heap, mi_page_t* page, size_t size) mi_attr_noexcept {
  return _mi_page_malloc_zero(heap,page,size,true);
}
#if MI_GUARDED
mi_decl_restrict void* _mi_heap_malloc_guarded(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept;
#endif
static inline mi_decl_restrict void* mi_heap_malloc_small_zero(mi_heap_t* heap, size_t size, bool zero) mi_attr_noexcept {
  mi_assert(heap != NULL);
  mi_assert(size <= MI_SMALL_SIZE_MAX);
  #if MI_DEBUG
  const uintptr_t tid = _mi_thread_id();
  mi_assert(heap->thread_id == 0 || heap->thread_id == tid);
  #endif
  #if (MI_PADDING || MI_GUARDED)
  if (size == 0) { size = sizeof(void*); }
  #endif
  #if MI_GUARDED
  if (mi_heap_malloc_use_guarded(heap,size)) {
    return _mi_heap_malloc_guarded(heap, size, zero);
  }
  #endif
  mi_page_t* page = _mi_heap_get_free_small_page(heap, size + MI_PADDING_SIZE);
  void* const p = _mi_page_malloc_zero(heap, page, size + MI_PADDING_SIZE, zero);
  mi_track_malloc(p,size,zero);
  #if MI_STAT>1
  if (p != NULL) {
    if (!mi_heap_is_initialized(heap)) { heap = mi_prim_get_default_heap(); }
    mi_heap_stat_increase(heap, malloc, mi_usable_size(p));
  }
  #endif
  #if MI_DEBUG>3
  if (p != NULL && zero) {
    mi_assert_expensive(mi_mem_is_zero(p, size));
  }
  #endif
  return p;
}
mi_decl_nodiscard extern inline mi_decl_restrict void* mi_heap_malloc_small(mi_heap_t* heap, size_t size) mi_attr_noexcept {
  return mi_heap_malloc_small_zero(heap, size, false);
}
mi_decl_nodiscard extern inline mi_decl_restrict void* mi_malloc_small(size_t size) mi_attr_noexcept {
  return mi_heap_malloc_small(mi_prim_get_default_heap(), size);
}
extern inline void* _mi_heap_malloc_zero_ex(mi_heap_t* heap, size_t size, bool zero, size_t huge_alignment) mi_attr_noexcept {
  if mi_likely(size <= MI_SMALL_SIZE_MAX) {
    mi_assert_internal(huge_alignment == 0);
    return mi_heap_malloc_small_zero(heap, size, zero);
  }
  #if MI_GUARDED
  else if (huge_alignment==0 && mi_heap_malloc_use_guarded(heap,size)) {
    return _mi_heap_malloc_guarded(heap, size, zero);
  }
  #endif
  else {
    mi_assert(heap!=NULL);
    mi_assert(heap->thread_id == 0 || heap->thread_id == _mi_thread_id());
    void* const p = _mi_malloc_generic(heap, size + MI_PADDING_SIZE, zero, huge_alignment);
    mi_track_malloc(p,size,zero);
    #if MI_STAT>1
    if (p != NULL) {
      if (!mi_heap_is_initialized(heap)) { heap = mi_prim_get_default_heap(); }
      mi_heap_stat_increase(heap, malloc, mi_usable_size(p));
    }
    #endif
    #if MI_DEBUG>3
    if (p != NULL && zero) {
      mi_assert_expensive(mi_mem_is_zero(p, size));
    }
    #endif
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
mi_decl_nodiscard extern mi_decl_restrict void* mi_heap_mallocn(mi_heap_t* heap, size_t count, size_t size) mi_attr_noexcept {
  size_t total;
  if (mi_count_size_overflow(count, size, &total)) return NULL;
  return mi_heap_malloc(heap, total);
}
mi_decl_nodiscard mi_decl_restrict void* mi_mallocn(size_t count, size_t size) mi_attr_noexcept {
  return mi_heap_mallocn(mi_prim_get_default_heap(),count,size);
}
void* mi_expand(void* p, size_t newsize) mi_attr_noexcept {
  #if MI_PADDING
  MI_UNUSED(p); MI_UNUSED(newsize);
  return NULL;
  #else
  if (p == NULL) return NULL;
  const size_t size = _mi_usable_size(p,"mi_expand");
  if (newsize > size) return NULL;
  return p;
  #endif
}
void* _mi_heap_realloc_zero(mi_heap_t* heap, void* p, size_t newsize, bool zero) mi_attr_noexcept {
  const size_t size = _mi_usable_size(p,"mi_realloc");
  if mi_unlikely(newsize <= size && newsize >= (size / 2) && newsize > 0) {
    mi_assert_internal(p!=NULL);
    return p;
  }
  void* newp = mi_heap_malloc(heap,newsize);
  if mi_likely(newp != NULL) {
    if (zero && newsize > size) {
      const size_t start = (size >= sizeof(intptr_t) ? size - sizeof(intptr_t) : 0);
      _mi_memzero((uint8_t*)newp + start, newsize - start);
    }
    else if (newsize == 0) {
      ((uint8_t*)newp)[0] = 0;
    }
    if mi_likely(p != NULL) {
      const size_t copysize = (newsize > size ? size : newsize);
      mi_track_mem_defined(p,copysize);
      _mi_memcpy(newp, p, copysize);
      mi_free(p);
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
mi_decl_nodiscard void* mi_reallocf(void* p, size_t newsize) mi_attr_noexcept {
  return mi_heap_reallocf(mi_prim_get_default_heap(),p,newsize);
}
mi_decl_nodiscard void* mi_rezalloc(void* p, size_t newsize) mi_attr_noexcept {
  return mi_heap_rezalloc(mi_prim_get_default_heap(), p, newsize);
}
mi_decl_nodiscard void* mi_recalloc(void* p, size_t count, size_t size) mi_attr_noexcept {
  return mi_heap_recalloc(mi_prim_get_default_heap(), p, count, size);
}
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
mi_decl_nodiscard mi_decl_restrict char* mi_heap_strndup(mi_heap_t* heap, const char* s, size_t n) mi_attr_noexcept {
  if (s == NULL) return NULL;
  const size_t len = _mi_strnlen(s,n);
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
#ifdef _WIN32
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif
mi_decl_nodiscard mi_decl_restrict char* mi_heap_realpath(mi_heap_t* heap, const char* fname, char* resolved_name) mi_attr_noexcept {
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
char* mi_heap_realpath(mi_heap_t* heap, const char* fname, char* resolved_name) mi_attr_noexcept {
  if (resolved_name != NULL) {
    return realpath(fname,resolved_name);
  }
  else {
    char* rname = realpath(fname, NULL);
    if (rname == NULL) return NULL;
    char* result = mi_heap_strdup(heap, rname);
    mi_cfree(rname);
    return result;
  }
}
#endif
mi_decl_nodiscard mi_decl_restrict char* mi_realpath(const char* fname, char* resolved_name) mi_attr_noexcept {
  return mi_heap_realpath(mi_prim_get_default_heap(),fname,resolved_name);
}
#endif
#if 0
#if 0
  #else
  #endif
#if 0
    #else
    #endif
#else
typedef void (*std_new_handler_t)(void);
#if (defined(__GNUC__) || (defined(__clang__) && !defined(_MSC_VER)))
std_new_handler_t __attribute__((weak)) _ZSt15get_new_handlerv(void) {
  return NULL;
}
static std_new_handler_t mi_get_new_handler(void) {
  return _ZSt15get_new_handlerv();
}
#else
static std_new_handler_t mi_get_new_handler() {
  return NULL;
}
#endif
static bool mi_try_new_handler(bool nothrow) {
  std_new_handler_t h = mi_get_new_handler();
  if (h==NULL) {
    _mi_error_message(ENOMEM, "out of memory in 'new'");
    if (!nothrow) {
      abort();
    }
    return false;
  }
  else {
    h();
    return true;
  }
}
#endif
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
    mi_try_new_handler(false);
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
    mi_try_new_handler(false);
    return NULL;
  }
  else {
    return mi_new_realloc(p, total);
  }
}
#if MI_GUARDED
static void* mi_block_ptr_set_guarded(mi_block_t* block, size_t obj_size) {
  mi_page_t* const page = _mi_ptr_page(block);
  mi_page_set_has_aligned(page, true);
  block->next = MI_BLOCK_TAG_GUARDED;
  mi_segment_t* const segment = _mi_page_segment(page);
  const size_t block_size = mi_page_block_size(page);
  const size_t os_page_size = _mi_os_page_size();
  mi_assert_internal(block_size >= obj_size + os_page_size + sizeof(mi_block_t));
  if (block_size < obj_size + os_page_size + sizeof(mi_block_t)) {
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
  size_t offset = block_size - os_page_size - obj_size;
  mi_assert_internal(offset > sizeof(mi_block_t));
  if (offset > MI_BLOCK_ALIGNMENT_MAX) {
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
  #endif
  const size_t os_page_size = _mi_os_page_size();
  const size_t obj_size = (mi_option_is_enabled(mi_option_guarded_precise) ? size : _mi_align_up(size, MI_MAX_ALIGN_SIZE));
  const size_t bsize = _mi_align_up(_mi_align_up(obj_size, MI_MAX_ALIGN_SIZE) + sizeof(mi_block_t), MI_MAX_ALIGN_SIZE);
  const size_t req_size = _mi_align_up(bsize + os_page_size, os_page_size);
  mi_block_t* const block = (mi_block_t*)_mi_malloc_generic(heap, req_size, zero, 0 );
  if (block==NULL) return NULL;
  void* const p = mi_block_ptr_set_guarded(block, obj_size);
  mi_track_malloc(p, size, zero);
  if (p != NULL) {
    if (!mi_heap_is_initialized(heap)) { heap = mi_prim_get_default_heap(); }
    #if MI_STAT>1
    mi_heap_stat_increase(heap, malloc, mi_usable_size(p));
    #endif
    _mi_stat_counter_increase(&heap->tld->stats.guarded_alloc_count, 1);
  }
  #if MI_DEBUG>3
  if (p != NULL && zero) {
    mi_assert_expensive(mi_mem_is_zero(p, size));
  }
  #endif
  return p;
}
#endif
#if 0
#endif
#include <string.h>
static bool mi_malloc_is_naturally_aligned( size_t size, size_t alignment ) {
  mi_assert_internal(_mi_is_power_of_two(alignment) && (alignment > 0));
  if (alignment > size) return false;
  if (alignment <= MI_MAX_ALIGN_SIZE) return true;
  const size_t bsize = mi_good_size(size);
  return (bsize <= MI_MAX_ALIGN_GUARANTEE && (bsize & (alignment-1)) == 0);
}
#if MI_GUARDED
static mi_decl_restrict void* mi_heap_malloc_guarded_aligned(mi_heap_t* heap, size_t size, size_t alignment, bool zero) mi_attr_noexcept {
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
#endif
static mi_decl_noinline void* mi_heap_malloc_zero_aligned_at_overalloc(mi_heap_t* const heap, const size_t size, const size_t alignment, const size_t offset, const bool zero) mi_attr_noexcept
{
  mi_assert_internal(size <= (MI_MAX_ALLOC_SIZE - MI_PADDING_SIZE));
  mi_assert_internal(alignment != 0 && _mi_is_power_of_two(alignment));
  void* p;
  size_t oversize;
  if mi_unlikely(alignment > MI_BLOCK_ALIGNMENT_MAX) {
    if mi_unlikely(offset != 0) {
#if MI_DEBUG > 0
      _mi_error_message(EOVERFLOW, "aligned allocation with a very large alignment cannot be used with an alignment offset (size %zu, alignment %zu, offset %zu)\n", size, alignment, offset);
#endif
      return NULL;
    }
    oversize = (size <= MI_SMALL_SIZE_MAX ? MI_SMALL_SIZE_MAX + 1 : size);
    p = _mi_heap_malloc_zero_ex(heap, oversize, false, alignment);
    if (p == NULL) return NULL;
  }
  else {
    oversize = size + alignment - 1;
    p = mi_heap_malloc_zero_no_guarded(heap, oversize, zero);
    if (p == NULL) return NULL;
  }
  mi_page_t* page = _mi_ptr_page(p);
  const uintptr_t align_mask = alignment - 1;
  const uintptr_t poffset = ((uintptr_t)p + offset) & align_mask;
  const uintptr_t adjust = (poffset == 0 ? 0 : alignment - poffset);
  mi_assert_internal(adjust < alignment);
  void* aligned_p = (void*)((uintptr_t)p + adjust);
  if (aligned_p != p) {
    mi_page_set_has_aligned(page, true);
    #if MI_GUARDED
    if (adjust >= sizeof(mi_block_t)) {
      mi_block_t* const block = (mi_block_t*)p;
      block->next = MI_BLOCK_TAG_ALIGNED;
    }
    #endif
    _mi_padding_shrink(page, (mi_block_t*)p, adjust + size);
  }
  mi_assert_internal(mi_page_usable_block_size(page) >= adjust + size);
  mi_assert_internal(((uintptr_t)aligned_p + offset) % alignment == 0);
  mi_assert_internal(mi_usable_size(aligned_p)>=size);
  mi_assert_internal(mi_usable_size(p) == mi_usable_size(aligned_p)+adjust);
  #if MI_DEBUG > 1
  mi_page_t* const apage = _mi_ptr_page(aligned_p);
  void* unalign_p = _mi_page_ptr_unalign(apage, aligned_p);
  mi_assert_internal(p == unalign_p);
  #endif
  if (alignment > MI_BLOCK_ALIGNMENT_MAX) {
    mi_track_mem_undefined(aligned_p, size);
    if (zero) {
      _mi_memzero_aligned(aligned_p, mi_usable_size(aligned_p));
    }
  }
  if (p != aligned_p) {
    mi_track_align(p,aligned_p,adjust,mi_usable_size(aligned_p));
    #if MI_GUARDED
    mi_track_mem_defined(p, sizeof(mi_block_t));
    #endif
  }
  return aligned_p;
}
static mi_decl_noinline void* mi_heap_malloc_zero_aligned_at_generic(mi_heap_t* const heap, const size_t size, const size_t alignment, const size_t offset, const bool zero) mi_attr_noexcept
{
  mi_assert_internal(alignment != 0 && _mi_is_power_of_two(alignment));
  if mi_unlikely(size > (MI_MAX_ALLOC_SIZE - MI_PADDING_SIZE)) {
    #if MI_DEBUG > 0
    _mi_error_message(EOVERFLOW, "aligned allocation request is too large (size %zu, alignment %zu)\n", size, alignment);
    #endif
    return NULL;
  }
  if (offset == 0 && mi_malloc_is_naturally_aligned(size,alignment)) {
    void* p = mi_heap_malloc_zero_no_guarded(heap, size, zero);
    mi_assert_internal(p == NULL || ((uintptr_t)p % alignment) == 0);
    const bool is_aligned_or_null = (((uintptr_t)p) & (alignment-1))==0;
    if mi_likely(is_aligned_or_null) {
      return p;
    }
    else {
      mi_assert(false);
      mi_free(p);
    }
  }
  return mi_heap_malloc_zero_aligned_at_overalloc(heap,size,alignment,offset,zero);
}
static void* mi_heap_malloc_zero_aligned_at(mi_heap_t* const heap, const size_t size, const size_t alignment, const size_t offset, const bool zero) mi_attr_noexcept
{
  if mi_unlikely(alignment == 0 || !_mi_is_power_of_two(alignment)) {
    #if MI_DEBUG > 0
    _mi_error_message(EOVERFLOW, "aligned allocation requires the alignment to be a power-of-two (size %zu, alignment %zu)\n", size, alignment);
    #endif
    return NULL;
  }
  #if MI_GUARDED
  if (offset==0 && alignment < MI_BLOCK_ALIGNMENT_MAX && mi_heap_malloc_use_guarded(heap,size)) {
    return mi_heap_malloc_guarded_aligned(heap, size, alignment, zero);
  }
  #endif
  if mi_likely(size <= MI_SMALL_SIZE_MAX && alignment <= size) {
    const uintptr_t align_mask = alignment-1;
    const size_t padsize = size + MI_PADDING_SIZE;
    mi_page_t* page = _mi_heap_get_free_small_page(heap, padsize);
    if mi_likely(page->free != NULL) {
      const bool is_aligned = (((uintptr_t)page->free + offset) & align_mask)==0;
      if mi_likely(is_aligned)
      {
        #if MI_STAT>1
        mi_heap_stat_increase(heap, malloc, size);
        #endif
        void* p = (zero ? _mi_page_malloc_zeroed(heap,page,padsize) : _mi_page_malloc(heap,page,padsize));
        mi_assert_internal(p != NULL);
        mi_assert_internal(((uintptr_t)p + offset) % alignment == 0);
        mi_track_malloc(p,size,zero);
        return p;
      }
    }
  }
  return mi_heap_malloc_zero_aligned_at_generic(heap, size, alignment, offset, zero);
}
mi_decl_nodiscard mi_decl_restrict void* mi_heap_malloc_aligned_at(mi_heap_t* heap, size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_heap_malloc_zero_aligned_at(heap, size, alignment, offset, false);
}
mi_decl_nodiscard mi_decl_restrict void* mi_heap_malloc_aligned(mi_heap_t* heap, size_t size, size_t alignment) mi_attr_noexcept {
  return mi_heap_malloc_aligned_at(heap, size, alignment, 0);
}
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
static void* mi_heap_realloc_zero_aligned_at(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, size_t offset, bool zero) mi_attr_noexcept {
  mi_assert(alignment > 0);
  if (alignment <= sizeof(uintptr_t)) return _mi_heap_realloc_zero(heap,p,newsize,zero);
  if (p == NULL) return mi_heap_malloc_zero_aligned_at(heap,newsize,alignment,offset,zero);
  size_t size = mi_usable_size(p);
  if (newsize <= size && newsize >= (size - (size / 2))
      && (((uintptr_t)p + offset) % alignment) == 0) {
    return p;
  }
  else {
    void* newp = mi_heap_malloc_aligned_at(heap,newsize,alignment,offset);
    if (newp != NULL) {
      if (zero && newsize > size) {
        size_t start = (size >= sizeof(intptr_t) ? size - sizeof(intptr_t) : 0);
        _mi_memzero((uint8_t*)newp + start, newsize - start);
      }
      _mi_memcpy_aligned(newp, p, (newsize > size ? size : newsize));
      mi_free(p);
    }
    return newp;
  }
}
static void* mi_heap_realloc_zero_aligned(mi_heap_t* heap, void* p, size_t newsize, size_t alignment, bool zero) mi_attr_noexcept {
  mi_assert(alignment > 0);
  if (alignment <= sizeof(uintptr_t)) return _mi_heap_realloc_zero(heap,p,newsize,zero);
  size_t offset = ((uintptr_t)p % alignment);
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
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#if 0
#endif
#ifndef EINVAL
#define EINVAL 22
#endif
#ifndef ENOMEM
#define ENOMEM 12
#endif
mi_decl_nodiscard size_t mi_malloc_size(const void* p) mi_attr_noexcept {
  return mi_usable_size(p);
}
mi_decl_nodiscard size_t mi_malloc_usable_size(const void *p) mi_attr_noexcept {
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
  if (p == NULL) return EINVAL;
  if ((alignment % sizeof(void*)) != 0) return EINVAL;
  if (alignment==0 || !_mi_is_power_of_two(alignment)) return EINVAL;
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
  if (size >= SIZE_MAX - psize) return NULL;
  size_t asize = _mi_align_up(size, psize);
  return mi_malloc_aligned(asize, psize);
}
mi_decl_nodiscard mi_decl_restrict void* mi_aligned_alloc(size_t alignment, size_t size) mi_attr_noexcept {
  void* p = mi_malloc_aligned(size, alignment);
  mi_assert_internal(((uintptr_t)p % alignment) == 0);
  return p;
}
mi_decl_nodiscard void* mi_reallocarray( void* p, size_t count, size_t size ) mi_attr_noexcept {
  void* newp = mi_reallocn(p,count,size);
  if (newp==NULL) { errno = ENOMEM; }
  return newp;
}
mi_decl_nodiscard int mi_reallocarr( void* p, size_t count, size_t size ) mi_attr_noexcept {
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
void* mi__expand(void* p, size_t newsize) mi_attr_noexcept {
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
mi_decl_nodiscard mi_decl_restrict unsigned char* mi_mbsdup(const unsigned char* s) mi_attr_noexcept {
  return (unsigned char*)mi_strdup((const char*)s);
}
int mi_dupenv_s(char** buf, size_t* size, const char* name) mi_attr_noexcept {
  if (buf==NULL || name==NULL) return EINVAL;
  if (size != NULL) *size = 0;
  char* p = getenv(name);
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
  *buf = NULL;
  return EINVAL;
#else
  unsigned short* p = (unsigned short*)_wgetenv((const wchar_t*)name);
  if (p==NULL) {
    *buf = NULL;
  }
  else {
    *buf = mi_wcsdup(p);
    if (*buf==NULL) return ENOMEM;
    if (size != NULL) *size = wcslen((const wchar_t*)p);
  }
  return 0;
#endif
}
mi_decl_nodiscard void* mi_aligned_offset_recalloc(void* p, size_t newcount, size_t size, size_t alignment, size_t offset) mi_attr_noexcept {
  return mi_recalloc_aligned_at(p, newcount, size, alignment, offset);
}
mi_decl_nodiscard void* mi_aligned_recalloc(void* p, size_t newcount, size_t size, size_t alignment) mi_attr_noexcept {
  return mi_recalloc_aligned(p, newcount, size, alignment);
}
#ifndef MI_BITMAP_H
#define MI_BITMAP_H 
#define MI_BITMAP_FIELD_BITS (8*MI_SIZE_SIZE)
#define MI_BITMAP_FIELD_FULL (~((size_t)0))
typedef _Atomic(size_t) mi_bitmap_field_t;
typedef mi_bitmap_field_t* mi_bitmap_t;
typedef size_t mi_bitmap_index_t;
static inline mi_bitmap_index_t mi_bitmap_index_create_ex(size_t idx, size_t bitidx) {
  mi_assert_internal(bitidx <= MI_BITMAP_FIELD_BITS);
  return (idx*MI_BITMAP_FIELD_BITS) + bitidx;
}
static inline mi_bitmap_index_t mi_bitmap_index_create(size_t idx, size_t bitidx) {
  mi_assert_internal(bitidx < MI_BITMAP_FIELD_BITS);
  return mi_bitmap_index_create_ex(idx,bitidx);
}
static inline size_t mi_bitmap_index_field(mi_bitmap_index_t bitmap_idx) {
  return (bitmap_idx / MI_BITMAP_FIELD_BITS);
}
static inline size_t mi_bitmap_index_bit_in_field(mi_bitmap_index_t bitmap_idx) {
  return (bitmap_idx % MI_BITMAP_FIELD_BITS);
}
static inline size_t mi_bitmap_index_bit(mi_bitmap_index_t bitmap_idx) {
  return bitmap_idx;
}
bool _mi_bitmap_try_find_claim_field(mi_bitmap_t bitmap, size_t idx, const size_t count, mi_bitmap_index_t* bitmap_idx);
bool _mi_bitmap_try_find_from_claim(mi_bitmap_t bitmap, const size_t bitmap_fields, const size_t start_field_idx, const size_t count, mi_bitmap_index_t* bitmap_idx);
bool _mi_bitmap_unclaim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);
bool _mi_bitmap_try_claim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);
bool _mi_bitmap_claim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* any_zero);
bool _mi_bitmap_is_claimed(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);
bool _mi_bitmap_is_any_claimed(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);
bool _mi_bitmap_try_find_from_claim_across(mi_bitmap_t bitmap, const size_t bitmap_fields, const size_t start_field_idx, const size_t count, mi_bitmap_index_t* bitmap_idx, mi_stats_t* stats);
bool _mi_bitmap_unclaim_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);
bool _mi_bitmap_claim_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* pany_zero);
bool _mi_bitmap_is_claimed_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);
bool _mi_bitmap_is_any_claimed_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx);
#endif
typedef struct mi_arena_s {
  mi_arena_id_t id;
  mi_memid_t memid;
  _Atomic(uint8_t*)start;
  size_t block_count;
  size_t field_count;
  size_t meta_size;
  mi_memid_t meta_memid;
  int numa_node;
  bool exclusive;
  bool is_large;
  mi_lock_t abandoned_visit_lock;
  _Atomic(size_t)search_idx;
  _Atomic(mi_msecs_t)purge_expire;
  mi_bitmap_field_t* blocks_dirty;
  mi_bitmap_field_t* blocks_committed;
  mi_bitmap_field_t* blocks_purge;
  mi_bitmap_field_t* blocks_abandoned;
  mi_bitmap_field_t blocks_inuse[1];
} mi_arena_t;
#define MI_ARENA_BLOCK_SIZE (MI_SEGMENT_SIZE)
#define MI_ARENA_MIN_OBJ_SIZE (MI_ARENA_BLOCK_SIZE/2)
#define MI_MAX_ARENAS (132)
static mi_decl_cache_align _Atomic(mi_arena_t*) mi_arenas[MI_MAX_ARENAS];
static mi_decl_cache_align _Atomic(size_t) mi_arena_count;
#define MI_IN_ARENA_C 
#if !defined(MI_IN_ARENA_C)
#error "this file should be included from 'arena.c' (so mi_arena_t is visible)"
#endif
size_t mi_arena_id_index(mi_arena_id_t id);
mi_arena_t* mi_arena_from_index(size_t idx);
size_t mi_arena_get_count(void);
void* mi_arena_block_start(mi_arena_t* arena, mi_bitmap_index_t bindex);
bool mi_arena_memid_indices(mi_memid_t memid, size_t* arena_index, mi_bitmap_index_t* bitmap_index);
static bool mi_arena_segment_os_clear_abandoned(mi_segment_t* segment, bool take_lock) {
  mi_assert(segment->memid.memkind != MI_MEM_ARENA);
  mi_subproc_t* const subproc = segment->subproc;
  if (take_lock && !mi_lock_try_acquire(&subproc->abandoned_os_lock)) {
    return false;
  }
  bool reclaimed = false;
  mi_segment_t* const next = segment->abandoned_os_next;
  mi_segment_t* const prev = segment->abandoned_os_prev;
  if (next != NULL || prev != NULL || subproc->abandoned_os_list == segment) {
    #if MI_DEBUG>3
    bool found = false;
    size_t count = 0;
    for (mi_segment_t* current = subproc->abandoned_os_list; current != NULL; current = current->abandoned_os_next) {
      if (current == segment) { found = true; }
      count++;
    }
    mi_assert_internal(found);
    mi_assert_internal(count == mi_atomic_load_relaxed(&subproc->abandoned_os_list_count));
    #endif
    if (prev != NULL) { prev->abandoned_os_next = next; }
    else { subproc->abandoned_os_list = next; }
    if (next != NULL) { next->abandoned_os_prev = prev; }
    else { subproc->abandoned_os_list_tail = prev; }
    segment->abandoned_os_next = NULL;
    segment->abandoned_os_prev = NULL;
    mi_atomic_decrement_relaxed(&subproc->abandoned_count);
    mi_atomic_decrement_relaxed(&subproc->abandoned_os_list_count);
    if (take_lock) {
      mi_atomic_store_release(&segment->thread_id, _mi_thread_id());
    }
    reclaimed = true;
  }
  if (take_lock) { mi_lock_release(&segment->subproc->abandoned_os_lock); }
  return reclaimed;
}
bool _mi_arena_segment_clear_abandoned(mi_segment_t* segment) {
  if mi_unlikely(segment->memid.memkind != MI_MEM_ARENA) {
    return mi_arena_segment_os_clear_abandoned(segment, true );
  }
  size_t arena_idx;
  size_t bitmap_idx;
  mi_arena_memid_indices(segment->memid, &arena_idx, &bitmap_idx);
  mi_arena_t* arena = mi_arena_from_index(arena_idx);
  mi_assert_internal(arena != NULL);
  bool was_marked = _mi_bitmap_unclaim(arena->blocks_abandoned, arena->field_count, 1, bitmap_idx);
  if (was_marked) {
    mi_assert_internal(mi_atomic_load_acquire(&segment->thread_id) == 0);
    mi_atomic_decrement_relaxed(&segment->subproc->abandoned_count);
    mi_atomic_store_release(&segment->thread_id, _mi_thread_id());
  }
  mi_assert_internal(!was_marked || _mi_bitmap_is_claimed(arena->blocks_inuse, arena->field_count, 1, bitmap_idx));
  return was_marked;
}
static void mi_arena_segment_os_mark_abandoned(mi_segment_t* segment) {
  mi_assert(segment->memid.memkind != MI_MEM_ARENA);
  mi_subproc_t* const subproc = segment->subproc;
  if (!mi_lock_acquire(&subproc->abandoned_os_lock)) {
    _mi_error_message(EFAULT, "internal error: failed to acquire the abandoned (os) segment lock to mark abandonment");
  }
  else {
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
    mi_lock_release(&subproc->abandoned_os_lock);
  }
  return;
}
void _mi_arena_segment_mark_abandoned(mi_segment_t* segment)
{
  mi_assert_internal(segment->used == segment->abandoned);
  mi_atomic_store_release(&segment->thread_id, (uintptr_t)0);
  if mi_unlikely(segment->memid.memkind != MI_MEM_ARENA) {
    mi_arena_segment_os_mark_abandoned(segment);
    return;
  }
  size_t arena_idx;
  size_t bitmap_idx;
  mi_arena_memid_indices(segment->memid, &arena_idx, &bitmap_idx);
  mi_arena_t* arena = mi_arena_from_index(arena_idx);
  mi_assert_internal(arena != NULL);
  mi_subproc_t* const subproc = segment->subproc;
  const bool was_unmarked = _mi_bitmap_claim(arena->blocks_abandoned, arena->field_count, 1, bitmap_idx, NULL);
  if (was_unmarked) { mi_atomic_increment_relaxed(&subproc->abandoned_count); }
  mi_assert_internal(was_unmarked);
  mi_assert_internal(_mi_bitmap_is_claimed(arena->blocks_inuse, arena->field_count, 1, bitmap_idx));
}
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
    current->start = mi_arena_id_index(heap->arena_id);
    current->end = current->start + 1;
    current->os_list_count = 0;
  }
  else {
    if (abandoned_count > abandoned_list_count && max_arena > 0) {
      current->start = (heap == NULL || max_arena == 0 ? 0 : (mi_arena_id_t)(_mi_heap_random_next(heap) % max_arena));
      current->end = current->start + max_arena;
    }
    else {
      current->start = 0;
      current->end = 0;
    }
    current->os_list_count = abandoned_list_count;
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
  if (!_mi_bitmap_unclaim(arena->blocks_abandoned, arena->field_count, 1, bitmap_idx)) return NULL;
  mi_assert_internal(_mi_bitmap_is_claimed(arena->blocks_inuse, arena->field_count, 1, bitmap_idx));
  mi_segment_t* segment = (mi_segment_t*)mi_arena_block_start(arena, bitmap_idx);
  mi_assert_internal(mi_atomic_load_relaxed(&segment->thread_id) == 0);
  if (segment->subproc != subproc) {
    const bool was_zero = _mi_bitmap_claim(arena->blocks_abandoned, arena->field_count, 1, bitmap_idx, NULL);
    mi_assert_internal(was_zero); MI_UNUSED(was_zero);
    return NULL;
  }
  else {
    mi_atomic_decrement_relaxed(&subproc->abandoned_count);
    return segment;
  }
}
static mi_segment_t* mi_arena_segment_clear_abandoned_next_field(mi_arena_field_cursor_t* previous) {
  const size_t max_arena = mi_arena_get_count();
  size_t field_idx = mi_bitmap_index_field(previous->bitmap_idx);
  size_t bit_idx = mi_bitmap_index_bit_in_field(previous->bitmap_idx);
  for (; previous->start < previous->end; previous->start++, field_idx = 0, bit_idx = 0) {
    size_t arena_idx = (previous->start >= max_arena ? previous->start % max_arena : previous->start);
    mi_arena_t* arena = mi_arena_from_index(arena_idx);
    if (arena != NULL) {
      bool has_lock = false;
      for (; field_idx < arena->field_count; field_idx++, bit_idx = 0) {
        size_t field = mi_atomic_load_relaxed(&arena->blocks_abandoned[field_idx]);
        if mi_unlikely(field != 0) {
          if (!has_lock && mi_option_is_enabled(mi_option_visit_abandoned)) {
            has_lock = (previous->visit_all ? mi_lock_acquire(&arena->abandoned_visit_lock) : mi_lock_try_acquire(&arena->abandoned_visit_lock));
            if (!has_lock) {
              if (previous->visit_all) {
                _mi_error_message(EFAULT, "internal error: failed to visit all abandoned segments due to failure to acquire the visitor lock");
              }
              break;
            }
          }
          mi_assert_internal(has_lock || !mi_option_is_enabled(mi_option_visit_abandoned));
          for (; bit_idx < MI_BITMAP_FIELD_BITS; bit_idx++) {
            size_t mask = ((size_t)1 << bit_idx);
            if mi_unlikely((field & mask) == mask) {
              mi_bitmap_index_t bitmap_idx = mi_bitmap_index_create(field_idx, bit_idx);
              mi_segment_t* const segment = mi_arena_segment_clear_abandoned_at(arena, previous->subproc, bitmap_idx);
              if (segment != NULL) {
                if (has_lock) { mi_lock_release(&arena->abandoned_visit_lock); }
                previous->bitmap_idx = mi_bitmap_index_create_ex(field_idx, bit_idx + 1);
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
  if (!previous->hold_visit_lock) {
    previous->hold_visit_lock = (previous->visit_all ? mi_lock_acquire(&previous->subproc->abandoned_os_visit_lock)
      : mi_lock_try_acquire(&previous->subproc->abandoned_os_visit_lock));
    if (!previous->hold_visit_lock) {
      if (previous->visit_all) {
        _mi_error_message(EFAULT, "internal error: failed to visit all abandoned segments due to failure to acquire the OS visitor lock");
      }
      return NULL;
    }
  }
  while (previous->os_list_count > 0) {
    previous->os_list_count--;
    const bool has_lock = mi_lock_acquire(&previous->subproc->abandoned_os_lock);
    if (has_lock) {
      mi_segment_t* segment = previous->subproc->abandoned_os_list;
      if (segment == NULL || mi_arena_segment_os_clear_abandoned(segment, false )) {
        mi_lock_release(&previous->subproc->abandoned_os_lock);
        return segment;
      }
      mi_lock_release(&previous->subproc->abandoned_os_lock);
    }
    else {
      _mi_error_message(EFAULT, "failed to acquire abandoned OS list lock during abandoned block visit\n");
      return NULL;
    }
  }
  mi_assert_internal(previous->os_list_count == 0);
  return NULL;
}
mi_segment_t* _mi_arena_segment_clear_abandoned_next(mi_arena_field_cursor_t* previous) {
  if (previous->start < previous->end) {
    mi_segment_t* segment = mi_arena_segment_clear_abandoned_next_field(previous);
    if (segment != NULL) { return segment; }
  }
  mi_assert_internal(previous->start == previous->end);
  return mi_arena_segment_clear_abandoned_next_list(previous);
}
bool mi_abandoned_visit_blocks(mi_subproc_id_t subproc_id, int heap_tag, bool visit_blocks, mi_block_visit_fun* visitor, void* arg) {
  if (!mi_option_is_enabled(mi_option_visit_abandoned)) {
    _mi_error_message(EFAULT, "internal error: can only visit abandoned blocks when MIMALLOC_VISIT_ABANDONED=ON");
    return false;
  }
  mi_arena_field_cursor_t current;
  _mi_arena_field_cursor_init(NULL, _mi_subproc_from_id(subproc_id), true , &current);
  mi_segment_t* segment;
  bool ok = true;
  while (ok && (segment = _mi_arena_segment_clear_abandoned_next(&current)) != NULL) {
    ok = _mi_segment_visit_blocks(segment, heap_tag, visit_blocks, visitor, arg);
    _mi_arena_segment_mark_abandoned(segment);
  }
  _mi_arena_field_cursor_done(&current);
  return ok;
}
#undef MI_IN_ARENA_C
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
#define MI_ARENA_STATIC_MAX ((MI_INTPTR_SIZE/2)*MI_KiB)
static mi_decl_cache_align uint8_t mi_arena_static[MI_ARENA_STATIC_MAX];
static mi_decl_cache_align _Atomic(size_t) mi_arena_static_top;
static void* mi_arena_static_zalloc(size_t size, size_t alignment, mi_memid_t* memid) {
  *memid = _mi_memid_none();
  if (size == 0 || size > MI_ARENA_STATIC_MAX) return NULL;
  const size_t toplow = mi_atomic_load_relaxed(&mi_arena_static_top);
  if ((toplow + size) > MI_ARENA_STATIC_MAX) return NULL;
  if (alignment < MI_MAX_ALIGN_SIZE) { alignment = MI_MAX_ALIGN_SIZE; }
  const size_t oversize = size + alignment - 1;
  if (toplow + oversize > MI_ARENA_STATIC_MAX) return NULL;
  const size_t oldtop = mi_atomic_add_acq_rel(&mi_arena_static_top, oversize);
  size_t top = oldtop + oversize;
  if (top > MI_ARENA_STATIC_MAX) {
    mi_atomic_cas_strong_acq_rel(&mi_arena_static_top, &top, oldtop);
    return NULL;
  }
  *memid = _mi_memid_create(MI_MEM_STATIC);
  memid->initially_zero = true;
  const size_t start = _mi_align_up(oldtop, alignment);
  uint8_t* const p = &mi_arena_static[start];
  _mi_memzero_aligned(p, size);
  return p;
}
void* _mi_arena_meta_zalloc(size_t size, mi_memid_t* memid) {
  *memid = _mi_memid_none();
  void* p = mi_arena_static_zalloc(size, MI_MAX_ALIGN_SIZE, memid);
  if (p != NULL) return p;
  p = _mi_os_alloc(size, memid, &_mi_stats_main);
  if (p == NULL) return NULL;
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
static bool mi_arena_try_claim(mi_arena_t* arena, size_t blocks, mi_bitmap_index_t* bitmap_idx, mi_stats_t* stats)
{
  size_t idx = 0;
  if (_mi_bitmap_try_find_from_claim_across(arena->blocks_inuse, arena->field_count, idx, blocks, bitmap_idx, stats)) {
    mi_atomic_store_relaxed(&arena->search_idx, mi_bitmap_index_field(*bitmap_idx));
    return true;
  };
  return false;
}
static mi_decl_noinline void* mi_arena_try_alloc_at(mi_arena_t* arena, size_t arena_index, size_t needed_bcount,
                                                    bool commit, mi_memid_t* memid, mi_os_tld_t* tld)
{
  MI_UNUSED(arena_index);
  mi_assert_internal(mi_arena_id_index(arena->id) == arena_index);
  mi_bitmap_index_t bitmap_index;
  if (!mi_arena_try_claim(arena, needed_bcount, &bitmap_index, tld->stats)) return NULL;
  void* p = mi_arena_block_start(arena, bitmap_index);
  *memid = mi_memid_create_arena(arena->id, arena->exclusive, bitmap_index);
  memid->is_pinned = arena->memid.is_pinned;
  if (arena->blocks_purge != NULL) {
    _mi_bitmap_unclaim_across(arena->blocks_purge, arena->field_count, needed_bcount, bitmap_index);
  }
  if (arena->memid.initially_zero && arena->blocks_dirty != NULL) {
    memid->initially_zero = _mi_bitmap_claim_across(arena->blocks_dirty, arena->field_count, needed_bcount, bitmap_index, NULL);
  }
  if (arena->blocks_committed == NULL) {
    memid->initially_committed = true;
  }
  else if (commit) {
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
    memid->initially_committed = _mi_bitmap_is_claimed_across(arena->blocks_committed, arena->field_count, needed_bcount, bitmap_index);
  }
  return p;
}
static void* mi_arena_try_alloc_at_id(mi_arena_id_t arena_id, bool match_numa_node, int numa_node, size_t size, size_t alignment,
                                       bool commit, bool allow_large, mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld )
{
  MI_UNUSED_RELEASE(alignment);
  mi_assert(alignment <= MI_SEGMENT_ALIGN);
  const size_t bcount = mi_block_count_of_size(size);
  const size_t arena_index = mi_arena_id_index(arena_id);
  mi_assert_internal(arena_index < mi_atomic_load_relaxed(&mi_arena_count));
  mi_assert_internal(size <= mi_arena_block_size(bcount));
  mi_arena_t* arena = mi_arena_from_index(arena_index);
  if (arena == NULL) return NULL;
  if (!allow_large && arena->is_large) return NULL;
  if (!mi_arena_id_is_suitable(arena->id, arena->exclusive, req_arena_id)) return NULL;
  if (req_arena_id == _mi_arena_id_none()) {
    const bool numa_suitable = (numa_node < 0 || arena->numa_node < 0 || arena->numa_node == numa_node);
    if (match_numa_node) { if (!numa_suitable) return NULL; }
                    else { if (numa_suitable) return NULL; }
  }
  void* p = mi_arena_try_alloc_at(arena, arena_index, bcount, commit, memid, tld);
  mi_assert_internal(p == NULL || _mi_is_aligned(p, alignment));
  return p;
}
static mi_decl_noinline void* mi_arena_try_alloc(int numa_node, size_t size, size_t alignment,
                                                  bool commit, bool allow_large,
                                                  mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld )
{
  MI_UNUSED(alignment);
  mi_assert_internal(alignment <= MI_SEGMENT_ALIGN);
  const size_t max_arena = mi_atomic_load_relaxed(&mi_arena_count);
  if mi_likely(max_arena == 0) return NULL;
  if (req_arena_id != _mi_arena_id_none()) {
    if (mi_arena_id_index(req_arena_id) < max_arena) {
      void* p = mi_arena_try_alloc_at_id(req_arena_id, true, numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
      if (p != NULL) return p;
    }
  }
  else {
    for (size_t i = 0; i < max_arena; i++) {
      void* p = mi_arena_try_alloc_at_id(mi_arena_id_create(i), true, numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
      if (p != NULL) return p;
    }
    if (numa_node >= 0) {
      for (size_t i = 0; i < max_arena; i++) {
        void* p = mi_arena_try_alloc_at_id(mi_arena_id_create(i), false , numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
        if (p != NULL) return p;
      }
    }
  }
  return NULL;
}
static bool mi_arena_reserve(size_t req_size, bool allow_large, mi_arena_id_t req_arena_id, mi_arena_id_t *arena_id)
{
  if (_mi_preloading()) return false;
  if (req_arena_id != _mi_arena_id_none()) return false;
  const size_t arena_count = mi_atomic_load_acquire(&mi_arena_count);
  if (arena_count > (MI_MAX_ARENAS - 4)) return false;
  size_t arena_reserve = mi_option_get_size(mi_option_arena_reserve);
  if (arena_reserve == 0) return false;
  if (!_mi_os_has_virtual_reserve()) {
    arena_reserve = arena_reserve/4;
  }
  arena_reserve = _mi_align_up(arena_reserve, MI_ARENA_BLOCK_SIZE);
  arena_reserve = _mi_align_up(arena_reserve, MI_SEGMENT_SIZE);
  if (arena_count >= 8 && arena_count <= 128) {
    const size_t multiplier = (size_t)1 << _mi_clamp(arena_count/8, 0, 16 );
    size_t reserve = 0;
    if (!mi_mul_overflow(multiplier, arena_reserve, &reserve)) {
      arena_reserve = reserve;
    }
  }
  if (arena_reserve < req_size) return false;
  bool arena_commit = false;
  if (mi_option_get(mi_option_arena_eager_commit) == 2) { arena_commit = _mi_os_has_overcommit(); }
  else if (mi_option_get(mi_option_arena_eager_commit) == 1) { arena_commit = true; }
  return (mi_reserve_os_memory_ex(arena_reserve, arena_commit, allow_large, false , arena_id) == 0);
}
void* _mi_arena_alloc_aligned(size_t size, size_t alignment, size_t align_offset, bool commit, bool allow_large,
                              mi_arena_id_t req_arena_id, mi_memid_t* memid, mi_os_tld_t* tld)
{
  mi_assert_internal(memid != NULL && tld != NULL);
  mi_assert_internal(size > 0);
  *memid = _mi_memid_none();
  const int numa_node = _mi_os_numa_node(tld);
  if (!mi_option_is_enabled(mi_option_disallow_arena_alloc) || req_arena_id != _mi_arena_id_none()) {
    if (size >= MI_ARENA_MIN_OBJ_SIZE && alignment <= MI_SEGMENT_ALIGN && align_offset == 0) {
      void* p = mi_arena_try_alloc(numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
      if (p != NULL) return p;
      if (req_arena_id == _mi_arena_id_none()) {
        mi_arena_id_t arena_id = 0;
        if (mi_arena_reserve(size, allow_large, req_arena_id, &arena_id)) {
          mi_assert_internal(req_arena_id == _mi_arena_id_none());
          p = mi_arena_try_alloc_at_id(arena_id, true, numa_node, size, alignment, commit, allow_large, req_arena_id, memid, tld);
          if (p != NULL) return p;
        }
      }
    }
  }
  if (mi_option_is_enabled(mi_option_disallow_os_alloc) || req_arena_id != _mi_arena_id_none()) {
    errno = ENOMEM;
    return NULL;
  }
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
static long mi_arena_purge_delay(void) {
  return (mi_option_get(mi_option_purge_delay) * mi_option_get(mi_option_arena_purge_mult));
}
static void mi_arena_purge(mi_arena_t* arena, size_t bitmap_idx, size_t blocks, mi_stats_t* stats) {
  mi_assert_internal(arena->blocks_committed != NULL);
  mi_assert_internal(arena->blocks_purge != NULL);
  mi_assert_internal(!arena->memid.is_pinned);
  const size_t size = mi_arena_block_size(blocks);
  void* const p = mi_arena_block_start(arena, bitmap_idx);
  bool needs_recommit;
  if (_mi_bitmap_is_claimed_across(arena->blocks_committed, arena->field_count, blocks, bitmap_idx)) {
    needs_recommit = _mi_os_purge(p, size, stats);
  }
  else {
    mi_assert_internal(mi_option_is_enabled(mi_option_purge_decommits));
    needs_recommit = _mi_os_purge_ex(p, size, false , stats);
    if (needs_recommit) { _mi_stat_increase(&_mi_stats_main.committed, size); }
  }
  _mi_bitmap_unclaim_across(arena->blocks_purge, arena->field_count, blocks, bitmap_idx);
  if (needs_recommit) {
    _mi_bitmap_unclaim_across(arena->blocks_committed, arena->field_count, blocks, bitmap_idx);
  }
}
static void mi_arena_schedule_purge(mi_arena_t* arena, size_t bitmap_idx, size_t blocks, mi_stats_t* stats) {
  mi_assert_internal(arena->blocks_purge != NULL);
  const long delay = mi_arena_purge_delay();
  if (delay < 0) return;
  if (_mi_preloading() || delay == 0) {
    mi_arena_purge(arena, bitmap_idx, blocks, stats);
  }
  else {
    mi_msecs_t expire = mi_atomic_loadi64_relaxed(&arena->purge_expire);
    if (expire != 0) {
      mi_atomic_addi64_acq_rel(&arena->purge_expire, (mi_msecs_t)(delay/10));
    }
    else {
      mi_atomic_storei64_release(&arena->purge_expire, _mi_clock_now() + delay);
    }
    _mi_bitmap_claim_across(arena->blocks_purge, arena->field_count, blocks, bitmap_idx, NULL);
  }
}
static bool mi_arena_purge_range(mi_arena_t* arena, size_t idx, size_t startidx, size_t bitlen, size_t purge, mi_stats_t* stats) {
  const size_t endidx = startidx + bitlen;
  size_t bitidx = startidx;
  bool all_purged = false;
  while (bitidx < endidx) {
    size_t count = 0;
    while (bitidx + count < endidx && (purge & ((size_t)1 << (bitidx + count))) != 0) {
      count++;
    }
    if (count > 0) {
      const mi_bitmap_index_t range_idx = mi_bitmap_index_create(idx, bitidx);
      mi_arena_purge(arena, range_idx, count, stats);
      if (count == bitlen) {
        all_purged = true;
      }
    }
    bitidx += (count+1);
  }
  return all_purged;
}
static bool mi_arena_try_purge(mi_arena_t* arena, mi_msecs_t now, bool force, mi_stats_t* stats)
{
  if (arena->memid.is_pinned || arena->blocks_purge == NULL) return false;
  mi_msecs_t expire = mi_atomic_loadi64_relaxed(&arena->purge_expire);
  if (expire == 0) return false;
  if (!force && expire > now) return false;
  mi_atomic_casi64_strong_acq_rel(&arena->purge_expire, &expire, (mi_msecs_t)0);
  bool any_purged = false;
  bool full_purge = true;
  for (size_t i = 0; i < arena->field_count; i++) {
    size_t purge = mi_atomic_load_relaxed(&arena->blocks_purge[i]);
    if (purge != 0) {
      size_t bitidx = 0;
      while (bitidx < MI_BITMAP_FIELD_BITS) {
        size_t bitlen = 0;
        while (bitidx + bitlen < MI_BITMAP_FIELD_BITS && (purge & ((size_t)1 << (bitidx + bitlen))) != 0) {
          bitlen++;
        }
        const mi_bitmap_index_t bitmap_index = mi_bitmap_index_create(i, bitidx);
        while( bitlen > 0 ) {
          if (_mi_bitmap_try_claim(arena->blocks_inuse, arena->field_count, bitlen, bitmap_index)) {
            break;
          }
          bitlen--;
        }
        if (bitlen > 0) {
          purge = mi_atomic_load_acquire(&arena->blocks_purge[i]);
          if (!mi_arena_purge_range(arena, i, bitidx, bitlen, purge, stats)) {
            full_purge = false;
          }
          any_purged = true;
          _mi_bitmap_unclaim(arena->blocks_inuse, arena->field_count, bitlen, bitmap_index);
        }
        bitidx += (bitlen+1);
      }
    }
  }
  if (!full_purge) {
    const long delay = mi_arena_purge_delay();
    mi_msecs_t expected = 0;
    mi_atomic_casi64_strong_acq_rel(&arena->purge_expire,&expected,_mi_clock_now() + delay);
  }
  return any_purged;
}
static void mi_arenas_try_purge( bool force, bool visit_all, mi_stats_t* stats ) {
  if (_mi_preloading() || mi_arena_purge_delay() <= 0) return;
  const size_t max_arena = mi_atomic_load_acquire(&mi_arena_count);
  if (max_arena == 0) return;
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
void _mi_arena_free(void* p, size_t size, size_t committed_size, mi_memid_t memid, mi_stats_t* stats) {
  mi_assert_internal(size > 0 && stats != NULL);
  mi_assert_internal(committed_size <= size);
  if (p==NULL) return;
  if (size==0) return;
  const bool all_committed = (committed_size == size);
  mi_track_mem_undefined(p,size);
  if (mi_memkind_is_os(memid.memkind)) {
    if (!all_committed && committed_size > 0) {
      _mi_stat_decrease(&_mi_stats_main.committed, committed_size);
    }
    _mi_os_free(p, size, memid, stats);
  }
  else if (memid.memkind == MI_MEM_ARENA) {
    size_t arena_idx;
    size_t bitmap_idx;
    mi_arena_memid_indices(memid, &arena_idx, &bitmap_idx);
    mi_assert_internal(arena_idx < MI_MAX_ARENAS);
    mi_arena_t* arena = mi_atomic_load_ptr_acquire(mi_arena_t,&mi_arenas[arena_idx]);
    mi_assert_internal(arena != NULL);
    const size_t blocks = mi_block_count_of_size(size);
    if (arena == NULL) {
      _mi_error_message(EINVAL, "trying to free from an invalid arena: %p, size %zu, memid: 0x%zx\n", p, size, memid);
      return;
    }
    mi_assert_internal(arena->field_count > mi_bitmap_index_field(bitmap_idx));
    if (arena->field_count <= mi_bitmap_index_field(bitmap_idx)) {
      _mi_error_message(EINVAL, "trying to free from an invalid arena block: %p, size %zu, memid: 0x%zx\n", p, size, memid);
      return;
    }
    if (arena->memid.is_pinned || arena->blocks_committed == NULL) {
      mi_assert_internal(all_committed);
    }
    else {
      mi_assert_internal(arena->blocks_committed != NULL);
      mi_assert_internal(arena->blocks_purge != NULL);
      if (!all_committed) {
        _mi_bitmap_unclaim_across(arena->blocks_committed, arena->field_count, blocks, bitmap_idx);
        mi_track_mem_noaccess(p,size);
        if (committed_size > 0) {
          _mi_stat_decrease(&_mi_stats_main.committed, committed_size);
        }
      }
      mi_arena_schedule_purge(arena, bitmap_idx, blocks, stats);
    }
    bool all_inuse = _mi_bitmap_unclaim_across(arena->blocks_inuse, arena->field_count, blocks, bitmap_idx);
    if (!all_inuse) {
      _mi_error_message(EAGAIN, "trying to free an already freed arena block: %p, size %zu\n", p, size);
      return;
    };
  }
  else {
    mi_assert_internal(memid.memkind < MI_MEM_OS);
  }
  mi_arenas_try_purge(false, false, stats);
}
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
  size_t expected = max_arena;
  mi_atomic_cas_strong_acq_rel(&mi_arena_count, &expected, new_max_arena);
}
void _mi_arenas_collect(bool force_purge, mi_stats_t* stats) {
  mi_arenas_try_purge(force_purge, force_purge , stats);
}
void _mi_arena_unsafe_destroy_all(mi_stats_t* stats) {
  mi_arenas_unsafe_destroy();
  _mi_arenas_collect(true , stats);
}
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
  const size_t asize = sizeof(mi_arena_t) + (bitmaps*fields*sizeof(mi_bitmap_field_t));
  mi_memid_t meta_memid;
  mi_arena_t* arena = (mi_arena_t*)_mi_arena_meta_zalloc(asize, &meta_memid);
  if (arena == NULL) return false;
  arena->id = _mi_arena_id_none();
  arena->memid = memid;
  arena->exclusive = exclusive;
  arena->meta_size = asize;
  arena->meta_memid = meta_memid;
  arena->block_count = bcount;
  arena->field_count = fields;
  arena->start = (uint8_t*)start;
  arena->numa_node = numa_node;
  arena->is_large = is_large;
  arena->purge_expire = 0;
  arena->search_idx = 0;
  mi_lock_init(&arena->abandoned_visit_lock);
  arena->blocks_dirty = &arena->blocks_inuse[fields];
  arena->blocks_abandoned = &arena->blocks_inuse[2 * fields];
  arena->blocks_committed = (arena->memid.is_pinned ? NULL : &arena->blocks_inuse[3*fields]);
  arena->blocks_purge = (arena->memid.is_pinned ? NULL : &arena->blocks_inuse[4*fields]);
  if (arena->blocks_committed != NULL && arena->memid.initially_committed) {
    memset((void*)arena->blocks_committed, 0xFF, fields*sizeof(mi_bitmap_field_t));
  }
  ptrdiff_t post = (fields * MI_BITMAP_FIELD_BITS) - bcount;
  mi_assert_internal(post >= 0);
  if (post > 0) {
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
int mi_reserve_os_memory_ex(size_t size, bool commit, bool allow_large, bool exclusive, mi_arena_id_t* arena_id) mi_attr_noexcept {
  if (arena_id != NULL) *arena_id = _mi_arena_id_none();
  size = _mi_align_up(size, MI_ARENA_BLOCK_SIZE);
  mi_memid_t memid;
  void* start = _mi_os_alloc_aligned(size, MI_SEGMENT_ALIGN, commit, allow_large, &memid, &_mi_stats_main);
  if (start == NULL) return ENOMEM;
  const bool is_large = memid.is_pinned;
  if (!mi_manage_os_memory_ex2(start, size, is_large, -1 , exclusive, memid, arena_id)) {
    _mi_os_free_ex(start, size, commit, memid, &_mi_stats_main);
    _mi_verbose_message("failed to reserve %zu KiB memory\n", _mi_divide_up(size, 1024));
    return ENOMEM;
  }
  _mi_verbose_message("reserved %zu KiB memory%s\n", _mi_divide_up(size, 1024), is_large ? " (in large os pages)" : "");
  return 0;
}
bool mi_manage_os_memory(void* start, size_t size, bool is_committed, bool is_large, bool is_zero, int numa_node) mi_attr_noexcept {
  return mi_manage_os_memory_ex(start, size, is_committed, is_large, is_zero, numa_node, false , NULL);
}
int mi_reserve_os_memory(size_t size, bool commit, bool allow_large) mi_attr_noexcept {
  return mi_reserve_os_memory_ex(size, commit, allow_large, false, NULL);
}
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
  if (show_inuse) _mi_verbose_message("total inuse blocks    : %zu\n", inuse_total);
  if (show_abandoned) _mi_verbose_message("total abandoned blocks: %zu\n", abandoned_total);
  if (show_purge) _mi_verbose_message("total purgeable blocks: %zu\n", purge_total);
}
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
int mi_reserve_huge_os_pages_interleave(size_t pages, size_t numa_nodes, size_t timeout_msecs) mi_attr_noexcept {
  if (pages == 0) return 0;
  size_t numa_count = (numa_nodes > 0 ? numa_nodes : _mi_os_numa_node_count());
  if (numa_count <= 0) numa_count = 1;
  const size_t pages_per = pages / numa_count;
  const size_t pages_mod = pages % numa_count;
  const size_t timeout_per = (timeout_msecs==0 ? 0 : (timeout_msecs / numa_count) + 50);
  for (size_t numa_node = 0; numa_node < numa_count && pages > 0; numa_node++) {
    size_t node_pages = pages_per;
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
static inline size_t mi_bitmap_mask_(size_t count, size_t bitidx) {
  mi_assert_internal(count + bitidx <= MI_BITMAP_FIELD_BITS);
  mi_assert_internal(count > 0);
  if (count >= MI_BITMAP_FIELD_BITS) return MI_BITMAP_FIELD_FULL;
  if (count == 0) return 0;
  return ((((size_t)1 << count) - 1) << bitidx);
}
bool _mi_bitmap_try_find_claim_field(mi_bitmap_t bitmap, size_t idx, const size_t count, mi_bitmap_index_t* bitmap_idx)
{
  mi_assert_internal(bitmap_idx != NULL);
  mi_assert_internal(count <= MI_BITMAP_FIELD_BITS);
  mi_bitmap_field_t* field = &bitmap[idx];
  size_t map = mi_atomic_load_relaxed(field);
  if (map==MI_BITMAP_FIELD_FULL) return false;
  const size_t mask = mi_bitmap_mask_(count, 0);
  const size_t bitidx_max = MI_BITMAP_FIELD_BITS - count;
#ifdef MI_HAVE_FAST_BITSCAN
  size_t bitidx = mi_ctz(~map);
#else
  size_t bitidx = 0;
#endif
  size_t m = (mask << bitidx);
  while (bitidx <= bitidx_max) {
    const size_t mapm = (map & m);
    if (mapm == 0) {
      mi_assert_internal((m >> bitidx) == mask);
      const size_t newmap = (map | m);
      mi_assert_internal((newmap^map) >> bitidx == mask);
      if (!mi_atomic_cas_strong_acq_rel(field, &map, newmap)) {
        continue;
      }
      else {
        *bitmap_idx = mi_bitmap_index_create(idx, bitidx);
        return true;
      }
    }
    else {
#ifdef MI_HAVE_FAST_BITSCAN
      mi_assert_internal(mapm != 0);
      const size_t shift = (count == 1 ? 1 : (MI_INTPTR_BITS - mi_clz(mapm) - bitidx));
      mi_assert_internal(shift > 0 && shift <= count);
#else
      const size_t shift = 1;
#endif
      bitidx += shift;
      m <<= shift;
    }
  }
  return false;
}
bool _mi_bitmap_try_find_from_claim(mi_bitmap_t bitmap, const size_t bitmap_fields, const size_t start_field_idx, const size_t count, mi_bitmap_index_t* bitmap_idx) {
  size_t idx = start_field_idx;
  for (size_t visited = 0; visited < bitmap_fields; visited++, idx++) {
    if (idx >= bitmap_fields) { idx = 0; }
    if (_mi_bitmap_try_find_claim_field(bitmap, idx, count, bitmap_idx)) {
      return true;
    }
  }
  return false;
}
bool _mi_bitmap_unclaim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  const size_t idx = mi_bitmap_index_field(bitmap_idx);
  const size_t bitidx = mi_bitmap_index_bit_in_field(bitmap_idx);
  const size_t mask = mi_bitmap_mask_(count, bitidx);
  mi_assert_internal(bitmap_fields > idx); MI_UNUSED(bitmap_fields);
  const size_t prev = mi_atomic_and_acq_rel(&bitmap[idx], ~mask);
  return ((prev & mask) == mask);
}
bool _mi_bitmap_claim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* any_zero) {
  const size_t idx = mi_bitmap_index_field(bitmap_idx);
  const size_t bitidx = mi_bitmap_index_bit_in_field(bitmap_idx);
  const size_t mask = mi_bitmap_mask_(count, bitidx);
  mi_assert_internal(bitmap_fields > idx); MI_UNUSED(bitmap_fields);
  size_t prev = mi_atomic_or_acq_rel(&bitmap[idx], mask);
  if (any_zero != NULL) { *any_zero = ((prev & mask) != mask); }
  return ((prev & mask) == 0);
}
static bool mi_bitmap_is_claimedx(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx, bool* any_ones) {
  const size_t idx = mi_bitmap_index_field(bitmap_idx);
  const size_t bitidx = mi_bitmap_index_bit_in_field(bitmap_idx);
  const size_t mask = mi_bitmap_mask_(count, bitidx);
  mi_assert_internal(bitmap_fields > idx); MI_UNUSED(bitmap_fields);
  const size_t field = mi_atomic_load_relaxed(&bitmap[idx]);
  if (any_ones != NULL) { *any_ones = ((field & mask) != 0); }
  return ((field & mask) == mask);
}
bool _mi_bitmap_try_claim(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  const size_t idx = mi_bitmap_index_field(bitmap_idx);
  const size_t bitidx = mi_bitmap_index_bit_in_field(bitmap_idx);
  const size_t mask = mi_bitmap_mask_(count, bitidx);
  mi_assert_internal(bitmap_fields > idx); MI_UNUSED(bitmap_fields);
  size_t expected = mi_atomic_load_relaxed(&bitmap[idx]);
  do {
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
static bool mi_bitmap_try_find_claim_field_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t idx, const size_t count, const size_t retries, mi_bitmap_index_t* bitmap_idx, mi_stats_t* stats)
{
  mi_assert_internal(bitmap_idx != NULL);
  mi_bitmap_field_t* field = &bitmap[idx];
  size_t map = mi_atomic_load_relaxed(field);
  const size_t initial = mi_clz(map);
  mi_assert_internal(initial <= MI_BITMAP_FIELD_BITS);
  if (initial == 0) return false;
  if (initial >= count) return _mi_bitmap_try_find_claim_field(bitmap, idx, count, bitmap_idx);
  if (_mi_divide_up(count - initial, MI_BITMAP_FIELD_BITS) >= (bitmap_fields - idx)) return false;
  size_t found = initial;
  size_t mask = 0;
  while(found < count) {
    field++;
    map = mi_atomic_load_relaxed(field);
    const size_t mask_bits = (found + MI_BITMAP_FIELD_BITS <= count ? MI_BITMAP_FIELD_BITS : (count - found));
    mi_assert_internal(mask_bits > 0 && mask_bits <= MI_BITMAP_FIELD_BITS);
    mask = mi_bitmap_mask_(mask_bits, 0);
    if ((map & mask) != 0) return false;
    found += mask_bits;
  }
  mi_assert_internal(field < &bitmap[bitmap_fields]);
  mi_bitmap_field_t* const final_field = field;
  const size_t final_mask = mask;
  mi_bitmap_field_t* const initial_field = &bitmap[idx];
  const size_t initial_idx = MI_BITMAP_FIELD_BITS - initial;
  const size_t initial_mask = mi_bitmap_mask_(initial, initial_idx);
  size_t newmap;
  field = initial_field;
  map = mi_atomic_load_relaxed(field);
  do {
    newmap = (map | initial_mask);
    if ((map & initial_mask) != 0) { goto rollback; };
  } while (!mi_atomic_cas_strong_acq_rel(field, &map, newmap));
  while (++field < final_field) {
    newmap = mi_bitmap_mask_(MI_BITMAP_FIELD_BITS, 0);
    map = 0;
    if (!mi_atomic_cas_strong_acq_rel(field, &map, newmap)) { goto rollback; }
  }
  mi_assert_internal(field == final_field);
  map = mi_atomic_load_relaxed(field);
  do {
    newmap = (map | final_mask);
    if ((map & final_mask) != 0) { goto rollback; }
  } while (!mi_atomic_cas_strong_acq_rel(field, &map, newmap));
  mi_stat_counter_increase(stats->arena_crossover_count,1);
  *bitmap_idx = mi_bitmap_index_create(idx, initial_idx);
  return true;
rollback:
  while (--field > initial_field) {
    newmap = 0;
    map = mi_bitmap_mask_(MI_BITMAP_FIELD_BITS, 0);
    mi_assert_internal(mi_atomic_load_relaxed(field) == map);
    mi_atomic_store_release(field, newmap);
  }
  if (field == initial_field) {
    map = mi_atomic_load_relaxed(field);
    do {
      mi_assert_internal((map & initial_mask) == initial_mask);
      newmap = (map & ~initial_mask);
    } while (!mi_atomic_cas_strong_acq_rel(field, &map, newmap));
  }
  mi_stat_counter_increase(stats->arena_rollback_count,1);
  if (retries <= 2) {
    return mi_bitmap_try_find_claim_field_across(bitmap, bitmap_fields, idx, count, retries+1, bitmap_idx, stats);
  }
  else {
    return false;
  }
}
bool _mi_bitmap_try_find_from_claim_across(mi_bitmap_t bitmap, const size_t bitmap_fields, const size_t start_field_idx, const size_t count, mi_bitmap_index_t* bitmap_idx, mi_stats_t* stats) {
  mi_assert_internal(count > 0);
  if (count <= 2) {
    return _mi_bitmap_try_find_from_claim(bitmap, bitmap_fields, start_field_idx, count, bitmap_idx);
  }
  size_t idx = start_field_idx;
  for (size_t visited = 0; visited < bitmap_fields; visited++, idx++) {
    if (idx >= bitmap_fields) { idx = 0; }
    if (mi_bitmap_try_find_claim_field_across(bitmap, bitmap_fields, idx, count, 0, bitmap_idx, stats)) {
      return true;
    }
  }
  return false;
}
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
bool _mi_bitmap_unclaim_across(mi_bitmap_t bitmap, size_t bitmap_fields, size_t count, mi_bitmap_index_t bitmap_idx) {
  size_t idx = mi_bitmap_index_field(bitmap_idx);
  size_t pre_mask;
  size_t mid_mask;
  size_t post_mask;
  size_t mid_count = mi_bitmap_mask_across(bitmap_idx, bitmap_fields, count, &pre_mask, &mid_mask, &post_mask);
  bool all_one = true;
  mi_bitmap_field_t* field = &bitmap[idx];
  size_t prev = mi_atomic_and_acq_rel(field++, ~pre_mask);
  if ((prev & pre_mask) != pre_mask) all_one = false;
  while(mid_count-- > 0) {
    prev = mi_atomic_and_acq_rel(field++, ~mid_mask);
    if ((prev & mid_mask) != mid_mask) all_one = false;
  }
  if (post_mask!=0) {
    prev = mi_atomic_and_acq_rel(field, ~post_mask);
    if ((prev & post_mask) != post_mask) all_one = false;
  }
  return all_one;
}
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
#include <string.h>
#if defined(_MSC_VER) && (_MSC_VER < 1920)
#pragma warning(disable:4204)
#endif
typedef bool (heap_page_visitor_fun)(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* arg1, void* arg2);
static bool mi_heap_visit_pages(mi_heap_t* heap, heap_page_visitor_fun* fn, void* arg1, void* arg2)
{
  if (heap==NULL || heap->page_count==0) return 0;
  #if MI_DEBUG>1
  size_t total = heap->page_count;
  size_t count = 0;
  #endif
  for (size_t i = 0; i <= MI_BIN_FULL; i++) {
    mi_page_queue_t* pq = &heap->pages[i];
    mi_page_t* page = pq->first;
    while(page != NULL) {
      mi_page_t* next = page->next;
      mi_assert_internal(mi_page_heap(page) == heap);
      #if MI_DEBUG>1
      count++;
      #endif
      if (!fn(heap, pq, page, arg1, arg2)) return false;
      page = next;
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
#endif
#if MI_DEBUG>=3
static bool mi_heap_is_valid(mi_heap_t* heap) {
  mi_assert_internal(heap!=NULL);
  mi_heap_visit_pages(heap, &mi_heap_page_is_valid, NULL, NULL);
  return true;
}
#endif
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
    _mi_page_free(page, pq, collect >= MI_FORCE);
  }
  else if (collect == MI_ABANDON) {
    _mi_page_abandon(page, pq);
  }
  return true;
}
static bool mi_heap_page_never_delayed_free(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* arg1, void* arg2) {
  MI_UNUSED(arg1);
  MI_UNUSED(arg2);
  MI_UNUSED(heap);
  MI_UNUSED(pq);
  _mi_page_use_delayed_free(page, MI_NEVER_DELAYED_FREE, false);
  return true;
}
static void mi_heap_collect_ex(mi_heap_t* heap, mi_collect_t collect)
{
  if (heap==NULL || !mi_heap_is_initialized(heap)) return;
  const bool force = (collect >= MI_FORCE);
  _mi_deferred_free(heap, force);
  const bool is_main_thread = (_mi_is_main_thread() && heap->thread_id == _mi_thread_id());
  if (
  #ifdef NDEBUG
      collect == MI_FORCE
  #else
      collect >= MI_FORCE
  #endif
    && is_main_thread && mi_heap_is_backing(heap) && !heap->no_reclaim)
  {
    _mi_abandoned_reclaim_all(heap, &heap->tld->segments);
  }
  if (collect == MI_ABANDON) {
    mi_heap_visit_pages(heap, &mi_heap_page_never_delayed_free, NULL, NULL);
  }
  _mi_heap_delayed_free_all(heap);
  _mi_heap_collect_retired(heap, force);
  mi_heap_visit_pages(heap, &mi_heap_page_collect, &collect, NULL);
  mi_assert_internal( collect != MI_ABANDON || mi_atomic_load_ptr_acquire(mi_block_t,&heap->thread_delayed_free) == NULL );
  _mi_segments_collect(collect == MI_FORCE, &heap->tld->segments);
  if (force && is_main_thread && mi_heap_is_backing(heap)) {
    _mi_thread_data_collect();
  }
  _mi_arenas_collect(collect == MI_FORCE , &heap->tld->stats);
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
  heap->thread_id = _mi_thread_id();
  heap->arena_id = arena_id;
  heap->no_reclaim = noreclaim;
  heap->tag = tag;
  if (heap == tld->heap_backing) {
    _mi_random_init(&heap->random);
  }
  else {
    _mi_random_split(&tld->heap_backing->random, &heap->random);
  }
  heap->cookie = _mi_heap_random_next(heap) | 1;
  heap->keys[0] = _mi_heap_random_next(heap);
  heap->keys[1] = _mi_heap_random_next(heap);
  _mi_heap_guarded_init(heap);
  heap->next = heap->tld->heaps;
  heap->tld->heaps = heap;
}
mi_decl_nodiscard mi_heap_t* mi_heap_new_ex(int heap_tag, bool allow_destroy, mi_arena_id_t arena_id) {
  mi_heap_t* bheap = mi_heap_get_backing();
  mi_heap_t* heap = mi_heap_malloc_tp(bheap, mi_heap_t);
  if (heap == NULL) return NULL;
  mi_assert(heap_tag >= 0 && heap_tag < 256);
  _mi_heap_init(heap, bheap->tld, arena_id, allow_destroy , (uint8_t)heap_tag );
  return heap;
}
mi_decl_nodiscard mi_heap_t* mi_heap_new_in_arena(mi_arena_id_t arena_id) {
  return mi_heap_new_ex(0 , false , arena_id);
}
mi_decl_nodiscard mi_heap_t* mi_heap_new(void) {
  return mi_heap_new_ex(0 , true , _mi_arena_id_none());
}
bool _mi_heap_memid_is_suitable(mi_heap_t* heap, mi_memid_t memid) {
  return _mi_arena_memid_is_suitable(memid, heap->arena_id);
}
uintptr_t _mi_heap_random_next(mi_heap_t* heap) {
  return _mi_random_next(&heap->random);
}
static void mi_heap_reset_pages(mi_heap_t* heap) {
  mi_assert_internal(heap != NULL);
  mi_assert_internal(mi_heap_is_initialized(heap));
  memset(&heap->pages_free_direct, 0, sizeof(heap->pages_free_direct));
  _mi_memcpy_aligned(&heap->pages, &_mi_heap_empty.pages, sizeof(heap->pages));
  heap->thread_delayed_free = NULL;
  heap->page_count = 0;
}
static void mi_heap_free(mi_heap_t* heap) {
  mi_assert(heap != NULL);
  mi_assert_internal(mi_heap_is_initialized(heap));
  if (heap==NULL || !mi_heap_is_initialized(heap)) return;
  if (mi_heap_is_backing(heap)) return;
  if (mi_heap_is_default(heap)) {
    _mi_heap_set_default_direct(heap->tld->heap_backing);
  }
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
  mi_free(heap);
}
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
static bool _mi_heap_page_destroy(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_t* page, void* arg1, void* arg2) {
  MI_UNUSED(arg1);
  MI_UNUSED(arg2);
  MI_UNUSED(heap);
  MI_UNUSED(pq);
  _mi_page_use_delayed_free(page, MI_NEVER_DELAYED_FREE, false);
  const size_t bsize = mi_page_block_size(page);
  if (bsize > MI_LARGE_OBJ_SIZE_MAX) {
    mi_heap_stat_decrease(heap, huge, bsize);
  }
#if (MI_STAT)
  _mi_page_free_collect(page, false);
  const size_t inuse = page->used;
  if (bsize <= MI_LARGE_OBJ_SIZE_MAX) {
    mi_heap_stat_decrease(heap, normal, bsize * inuse);
#if (MI_STAT>1)
    mi_heap_stat_decrease(heap, normal_bins[_mi_bin(bsize)], inuse);
#endif
  }
  mi_heap_stat_decrease(heap, malloc, bsize * inuse);
#endif
  mi_assert_internal(mi_page_thread_free(page) == NULL);
  page->used = 0;
  page->next = NULL;
  page->prev = NULL;
  _mi_segment_page_free(page,false , &heap->tld->segments);
  return true;
}
void _mi_heap_destroy_pages(mi_heap_t* heap) {
  mi_heap_visit_pages(heap, &_mi_heap_page_destroy, NULL, NULL);
  mi_heap_reset_pages(heap);
}
#if MI_TRACK_HEAP_DESTROY
static bool mi_cdecl mi_heap_track_block_free(const mi_heap_t* heap, const mi_heap_area_t* area, void* block, size_t block_size, void* arg) {
  MI_UNUSED(heap); MI_UNUSED(area); MI_UNUSED(arg); MI_UNUSED(block_size);
  mi_track_free_size(block,mi_usable_size(block));
  return true;
}
#endif
void mi_heap_destroy(mi_heap_t* heap) {
  mi_assert(heap != NULL);
  mi_assert(mi_heap_is_initialized(heap));
  mi_assert(heap->no_reclaim);
  mi_assert_expensive(mi_heap_is_valid(heap));
  if (heap==NULL || !mi_heap_is_initialized(heap)) return;
  #if MI_GUARDED
  mi_heap_delete(heap);
  return;
  #else
  if (!heap->no_reclaim) {
    _mi_warning_message("'mi_heap_destroy' called but ignored as the heap was not created with 'allow_destroy' (heap at %p)\n", heap);
    mi_heap_delete(heap);
  }
  else {
    #if MI_TRACK_HEAP_DESTROY
    mi_heap_visit_blocks(heap, true, mi_heap_track_block_free, NULL);
    #endif
    _mi_heap_destroy_pages(heap);
    mi_heap_free(heap);
  }
  #endif
}
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
static void mi_heap_absorb(mi_heap_t* heap, mi_heap_t* from) {
  mi_assert_internal(heap!=NULL);
  if (from==NULL || from->page_count == 0) return;
  _mi_heap_delayed_free_partial(from);
  for (size_t i = 0; i <= MI_BIN_FULL; i++) {
    mi_page_queue_t* pq = &heap->pages[i];
    mi_page_queue_t* append = &from->pages[i];
    size_t pcount = _mi_page_queue_append(heap, pq, append);
    heap->page_count += pcount;
    from->page_count -= pcount;
  }
  mi_assert_internal(from->page_count == 0);
  _mi_heap_delayed_free_all(from);
  #if !defined(_MSC_VER) || (_MSC_VER > 1900)
  mi_assert_internal(mi_atomic_load_ptr_relaxed(mi_block_t,&from->thread_delayed_free) == NULL);
  #endif
  mi_heap_reset_pages(from);
}
void mi_heap_delete(mi_heap_t* heap)
{
  mi_assert(heap != NULL);
  mi_assert(mi_heap_is_initialized(heap));
  mi_assert_expensive(mi_heap_is_valid(heap));
  if (heap==NULL || !mi_heap_is_initialized(heap)) return;
  if (!mi_heap_is_backing(heap)) {
    mi_heap_absorb(heap->tld->heap_backing, heap);
  }
  else {
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
  void* end = (uint8_t*)start + (page->capacity * mi_page_block_size(page));
  *found = (p >= start && p < end);
  return (!*found);
}
bool mi_heap_check_owned(mi_heap_t* heap, const void* p) {
  mi_assert(heap != NULL);
  if (heap==NULL || !mi_heap_is_initialized(heap)) return false;
  if (((uintptr_t)p & (MI_INTPTR_SIZE - 1)) != 0) return false;
  bool found = false;
  mi_heap_visit_pages(heap, &mi_heap_page_check_owned, (void*)p, &found);
  return found;
}
bool mi_check_owned(const void* p) {
  return mi_heap_check_owned(mi_prim_get_default_heap(), p);
}
void _mi_heap_area_init(mi_heap_area_t* area, mi_page_t* page) {
  const size_t bsize = mi_page_block_size(page);
  const size_t ubsize = mi_page_usable_block_size(page);
  area->reserved = page->reserved * bsize;
  area->committed = page->capacity * bsize;
  area->blocks = mi_page_start(page);
  area->used = page->used;
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
  _mi_page_free_collect(page,true);
  mi_assert_internal(page->local_free == NULL);
  if (page->used == 0) return true;
  size_t psize;
  uint8_t* const pstart = _mi_segment_page_start(_mi_page_segment(page), page, &psize);
  mi_heap_t* const heap = mi_page_heap(page);
  const size_t bsize = mi_page_block_size(page);
  const size_t ubsize = mi_page_usable_block_size(page);
  if (page->capacity == 1) {
    mi_assert_internal(page->used == 1 && page->free == NULL);
    return visitor(mi_page_heap(page), area, pstart, ubsize, arg);
  }
  mi_assert(bsize <= UINT32_MAX);
  if (page->used == page->capacity) {
    uint8_t* block = pstart;
    for (size_t i = 0; i < page->capacity; i++) {
      if (!visitor(heap, area, block, ubsize, arg)) return false;
      block += bsize;
    }
    return true;
  }
  #define MI_MAX_BLOCKS (MI_SMALL_PAGE_SIZE / sizeof(void*))
  uintptr_t free_map[MI_MAX_BLOCKS / MI_INTPTR_BITS];
  const uintptr_t bmapsize = _mi_divide_up(page->capacity, MI_INTPTR_BITS);
  memset(free_map, 0, bmapsize * sizeof(intptr_t));
  if (page->capacity % MI_INTPTR_BITS != 0) {
    size_t shift = (page->capacity % MI_INTPTR_BITS);
    uintptr_t mask = (UINTPTR_MAX << shift);
    free_map[bmapsize - 1] = mask;
  }
  uint64_t magic;
  size_t shift;
  mi_get_fast_divisor(bsize, &magic, &shift);
  #if MI_DEBUG>1
  size_t free_count = 0;
  #endif
  for (mi_block_t* block = page->free; block != NULL; block = mi_block_next(page, block)) {
    #if MI_DEBUG>1
    free_count++;
    #endif
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
  #if MI_DEBUG>1
  size_t used_count = 0;
  #endif
  uint8_t* block = pstart;
  for (size_t i = 0; i < bmapsize; i++) {
    if (free_map[i] == 0) {
      for (size_t j = 0; j < MI_INTPTR_BITS; j++) {
        #if MI_DEBUG>1
        used_count++;
        #endif
        if (!visitor(heap, area, block, ubsize, arg)) return false;
        block += bsize;
      }
    }
    else {
      uintptr_t m = ~free_map[i];
      while (m != 0) {
        #if MI_DEBUG>1
        used_count++;
        #endif
        size_t bitidx = mi_ctz(m);
        if (!visitor(heap, area, block + (bitidx * bsize), ubsize, arg)) return false;
        m &= m - 1;
      }
      block += bsize * MI_INTPTR_BITS;
    }
  }
  mi_assert_internal(page->used == used_count);
  return true;
}
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
static bool mi_heap_visit_areas(const mi_heap_t* heap, mi_heap_area_visit_fun* visitor, void* arg) {
  if (visitor == NULL) return false;
  return mi_heap_visit_pages((mi_heap_t*)heap, &mi_heap_visit_areas_page, (void*)(visitor), arg);
}
typedef struct mi_visit_blocks_args_s {
  bool visit_blocks;
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
bool mi_heap_visit_blocks(const mi_heap_t* heap, bool visit_blocks, mi_block_visit_fun* visitor, void* arg) {
  mi_visit_blocks_args_t args = { visit_blocks, visitor, arg };
  return mi_heap_visit_areas(heap, &mi_heap_area_visitor, &args);
}
#include <string.h>
#include <stdlib.h>
const mi_page_t _mi_page_empty = {
  0,
  false, false, false, false,
  0,
  0,
  { 0 },
  false,
  0,
  NULL,
  NULL,
  0,
  0,
  0,
  0,
  NULL,
  #if (MI_PADDING || MI_ENCODE_FREELIST)
  { 0, 0 },
  #endif
  MI_ATOMIC_VAR_INIT(0),
  MI_ATOMIC_VAR_INIT(0),
  NULL, NULL
  #if MI_INTPTR_SIZE==4
  , { NULL }
  #endif
};
#define MI_PAGE_EMPTY() ((mi_page_t*)&_mi_page_empty)
#if (MI_PADDING>0) && (MI_INTPTR_SIZE >= 8)
#define MI_SMALL_PAGES_EMPTY { MI_INIT128(MI_PAGE_EMPTY), MI_PAGE_EMPTY(), MI_PAGE_EMPTY() }
#elif (MI_PADDING>0)
#define MI_SMALL_PAGES_EMPTY { MI_INIT128(MI_PAGE_EMPTY), MI_PAGE_EMPTY(), MI_PAGE_EMPTY(), MI_PAGE_EMPTY() }
#else
#define MI_SMALL_PAGES_EMPTY { MI_INIT128(MI_PAGE_EMPTY), MI_PAGE_EMPTY() }
#endif
#define QNULL(sz) { NULL, NULL, (sz)*sizeof(uintptr_t) }
#define MI_PAGE_QUEUES_EMPTY \
  { QNULL(1), \
    QNULL( 1), QNULL( 2), QNULL( 3), QNULL( 4), QNULL( 5), QNULL( 6), QNULL( 7), QNULL( 8), \
    QNULL( 10), QNULL( 12), QNULL( 14), QNULL( 16), QNULL( 20), QNULL( 24), QNULL( 28), QNULL( 32), \
    QNULL( 40), QNULL( 48), QNULL( 56), QNULL( 64), QNULL( 80), QNULL( 96), QNULL( 112), QNULL( 128), \
    QNULL( 160), QNULL( 192), QNULL( 224), QNULL( 256), QNULL( 320), QNULL( 384), QNULL( 448), QNULL( 512), \
    QNULL( 640), QNULL( 768), QNULL( 896), QNULL( 1024), QNULL( 1280), QNULL( 1536), QNULL( 1792), QNULL( 2048), \
    QNULL( 2560), QNULL( 3072), QNULL( 3584), QNULL( 4096), QNULL( 5120), QNULL( 6144), QNULL( 7168), QNULL( 8192), \
    QNULL( 10240), QNULL( 12288), QNULL( 14336), QNULL( 16384), QNULL( 20480), QNULL( 24576), QNULL( 28672), QNULL( 32768), \
    QNULL( 40960), QNULL( 49152), QNULL( 57344), QNULL( 65536), QNULL( 81920), QNULL( 98304), QNULL(114688), QNULL(131072), \
    QNULL(163840), QNULL(196608), QNULL(229376), QNULL(262144), QNULL(327680), QNULL(393216), QNULL(458752), QNULL(524288), \
    QNULL(MI_LARGE_OBJ_WSIZE_MAX + 1 ), \
    QNULL(MI_LARGE_OBJ_WSIZE_MAX + 2) }
#define MI_STAT_COUNT_NULL() {0,0,0,0}
#if MI_STAT>1
#define MI_STAT_COUNT_END_NULL() , { MI_STAT_COUNT_NULL(), MI_INIT32(MI_STAT_COUNT_NULL) }
#else
#define MI_STAT_COUNT_END_NULL() 
#endif
#define MI_STATS_NULL \
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
mi_decl_cache_align const mi_heap_t _mi_heap_empty = {
  NULL,
  MI_ATOMIC_VAR_INIT(NULL),
  0,
  0,
  0,
  { 0, 0 },
  { {0}, {0}, 0, true },
  0,
  MI_BIN_FULL, 0,
  NULL,
  false,
  0,
  #if MI_GUARDED
  0, 0, 0, 0, 1,
  #endif
  MI_SMALL_PAGES_EMPTY,
  MI_PAGE_QUEUES_EMPTY
};
mi_threadid_t _mi_thread_id(void) mi_attr_noexcept {
  return _mi_prim_thread_id();
}
mi_decl_thread mi_heap_t* _mi_heap_default = (mi_heap_t*)&_mi_heap_empty;
extern mi_heap_t _mi_heap_main;
static mi_decl_cache_align mi_subproc_t mi_subproc_default;
static mi_decl_cache_align mi_tld_t tld_main = {
  0, false,
  &_mi_heap_main, &_mi_heap_main,
  { { NULL, NULL }, {NULL ,NULL}, {NULL ,NULL, 0},
    0, 0, 0, 0, 0, &mi_subproc_default,
    &tld_main.stats, &tld_main.os
  },
  { 0, &tld_main.stats },
  { MI_STATS_NULL }
};
mi_decl_cache_align mi_heap_t _mi_heap_main = {
  &tld_main,
  MI_ATOMIC_VAR_INIT(NULL),
  0,
  0,
  0,
  { 0, 0 },
  { {0x846ca68b}, {0}, 0, true },
  0,
  MI_BIN_FULL, 0,
  NULL,
  false,
  0,
  #if MI_GUARDED
  0, 0, 0, 0, 0,
  #endif
  MI_SMALL_PAGES_EMPTY,
  MI_PAGE_QUEUES_EMPTY
};
bool _mi_process_is_initialized = false;
mi_stats_t _mi_stats_main = { MI_STATS_NULL };
#if MI_GUARDED
mi_decl_export void mi_heap_guarded_set_sample_rate(mi_heap_t* heap, size_t sample_rate, size_t seed) {
  heap->guarded_sample_seed = seed;
  if (heap->guarded_sample_seed == 0) {
    heap->guarded_sample_seed = _mi_heap_random_next(heap);
  }
  heap->guarded_sample_rate = sample_rate;
  if (heap->guarded_sample_rate >= 1) {
    heap->guarded_sample_seed = heap->guarded_sample_seed % heap->guarded_sample_rate;
  }
  heap->guarded_sample_count = heap->guarded_sample_seed;
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
#endif
static void mi_heap_main_init(void) {
  if (_mi_heap_main.cookie == 0) {
    _mi_heap_main.thread_id = _mi_thread_id();
    _mi_heap_main.cookie = 1;
    #if defined(_WIN32) && !defined(MI_SHARED_LIB)
      _mi_random_init_weak(&_mi_heap_main.random);
    #else
      _mi_random_init(&_mi_heap_main.random);
    #endif
    _mi_heap_main.cookie = _mi_heap_random_next(&_mi_heap_main);
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
  bool safe_to_delete = false;
  if (mi_lock_acquire(&subproc->abandoned_os_lock)) {
    if (subproc->abandoned_os_list == NULL) {
      safe_to_delete = true;
    }
    mi_lock_release(&subproc->abandoned_os_lock);
  }
  if (!safe_to_delete) return;
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
typedef struct mi_thread_data_s {
  mi_heap_t heap;
  mi_tld_t tld;
  mi_memid_t memid;
} mi_thread_data_t;
#define TD_CACHE_SIZE (32)
static _Atomic(mi_thread_data_t*) td_cache[TD_CACHE_SIZE];
static mi_thread_data_t* mi_thread_data_zalloc(void) {
  bool is_zero = false;
  mi_thread_data_t* td = NULL;
  for (int i = 0; i < TD_CACHE_SIZE; i++) {
    td = mi_atomic_load_ptr_relaxed(mi_thread_data_t, &td_cache[i]);
    if (td != NULL) {
      td = mi_atomic_exchange_ptr_acq_rel(mi_thread_data_t, &td_cache[i], NULL);
      if (td != NULL) {
        break;
      }
    }
  }
  if (td == NULL) {
    mi_memid_t memid;
    td = (mi_thread_data_t*)_mi_os_alloc(sizeof(mi_thread_data_t), &memid, &_mi_stats_main);
    if (td == NULL) {
      td = (mi_thread_data_t*)_mi_os_alloc(sizeof(mi_thread_data_t), &memid, &_mi_stats_main);
      if (td == NULL) {
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
  for (int i = 0; i < TD_CACHE_SIZE; i++) {
    mi_thread_data_t* td = mi_atomic_load_ptr_relaxed(mi_thread_data_t, &td_cache[i]);
    if (td == NULL) {
      mi_thread_data_t* expected = NULL;
      if (mi_atomic_cas_ptr_weak_acq_rel(mi_thread_data_t, &td_cache[i], &expected, tdfree)) {
        return;
      }
    }
  }
  _mi_os_free(tdfree, sizeof(mi_thread_data_t), tdfree->memid, &_mi_stats_main);
}
void _mi_thread_data_collect(void) {
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
static bool _mi_thread_heap_init(void) {
  if (mi_heap_is_initialized(mi_prim_get_default_heap())) return true;
  if (_mi_is_main_thread()) {
    mi_heap_main_init();
    _mi_heap_set_default_direct(&_mi_heap_main);
  }
  else {
    mi_thread_data_t* td = mi_thread_data_zalloc();
    if (td == NULL) return false;
    mi_tld_t* tld = &td->tld;
    mi_heap_t* heap = &td->heap;
    _mi_tld_init(tld, heap);
    _mi_heap_init(heap, tld, _mi_arena_id_none(), false , 0 );
    _mi_heap_set_default_direct(heap);
  }
  return false;
}
void _mi_tld_init(mi_tld_t* tld, mi_heap_t* bheap) {
  _mi_memzero_aligned(tld,sizeof(mi_tld_t));
  tld->heap_backing = bheap;
  tld->heaps = NULL;
  tld->segments.subproc = &mi_subproc_default;
  tld->segments.stats = &tld->stats;
  tld->segments.os = &tld->os;
  tld->os.stats = &tld->stats;
}
static bool _mi_thread_heap_done(mi_heap_t* heap) {
  if (!mi_heap_is_initialized(heap)) return true;
  _mi_heap_set_default_direct(_mi_is_main_thread() ? &_mi_heap_main : (mi_heap_t*)&_mi_heap_empty);
  heap = heap->tld->heap_backing;
  if (!mi_heap_is_initialized(heap)) return false;
  mi_heap_t* curr = heap->tld->heaps;
  while (curr != NULL) {
    mi_heap_t* next = curr->next;
    if (curr != heap) {
      mi_assert_internal(!mi_heap_is_backing(curr));
      mi_heap_delete(curr);
    }
    curr = next;
  }
  mi_assert_internal(heap->tld->heaps == heap && heap->next == NULL);
  mi_assert_internal(mi_heap_is_backing(heap));
  if (heap != &_mi_heap_main) {
    _mi_heap_collect_abandon(heap);
  }
  _mi_stats_done(&heap->tld->stats);
  if (heap != &_mi_heap_main) {
    mi_assert_internal(heap->tld->segments.count == 0 || heap->thread_id != _mi_thread_id());
    mi_thread_data_free((mi_thread_data_t*)heap);
  }
  else {
    #if 0
    _mi_heap_destroy_pages(heap);
    mi_assert_internal(heap->tld->heap_backing == &_mi_heap_main);
    #endif
  }
  return false;
}
static void mi_process_setup_auto_thread_done(void) {
  static bool tls_initialized = false;
  if (tls_initialized) return;
  tls_initialized = true;
  _mi_prim_thread_init_auto_done();
  _mi_heap_set_default_direct(&_mi_heap_main);
}
bool _mi_is_main_thread(void) {
  return (_mi_heap_main.thread_id==0 || _mi_heap_main.thread_id == _mi_thread_id());
}
static _Atomic(size_t) thread_count = MI_ATOMIC_VAR_INIT(1);
size_t _mi_current_thread_count(void) {
  return mi_atomic_load_relaxed(&thread_count);
}
void mi_thread_init(void) mi_attr_noexcept
{
  mi_process_init();
  if (_mi_thread_heap_init()) return;
  _mi_stat_increase(&_mi_stats_main.threads, 1);
  mi_atomic_increment_relaxed(&thread_count);
}
void mi_thread_done(void) mi_attr_noexcept {
  _mi_thread_done(NULL);
}
void _mi_thread_done(mi_heap_t* heap)
{
  if (heap == NULL) {
    heap = mi_prim_get_default_heap();
    if (heap == NULL) return;
  }
  if (!mi_heap_is_initialized(heap)) {
    return;
  }
  mi_atomic_decrement_relaxed(&thread_count);
  _mi_stat_decrease(&_mi_stats_main.threads, 1);
  if (heap->thread_id != _mi_thread_id()) return;
  if (_mi_thread_heap_done(heap)) return;
}
void _mi_heap_set_default_direct(mi_heap_t* heap) {
  mi_assert_internal(heap != NULL);
  #if defined(MI_TLS_SLOT)
  mi_prim_tls_slot_set(MI_TLS_SLOT,heap);
  #elif defined(MI_TLS_PTHREAD_SLOT_OFS)
  *mi_prim_tls_pthread_heap_slot() = heap;
  #elif defined(MI_TLS_PTHREAD)
  #else
  _mi_heap_default = heap;
  #endif
  _mi_prim_thread_associate_default_heap(heap);
}
static bool os_preloading = true;
bool mi_decl_noinline _mi_preloading(void) {
  return os_preloading;
}
void _mi_process_load(void) {
  mi_heap_main_init();
  #if defined(__APPLE__) || defined(MI_TLS_RECURSE_GUARD)
  volatile mi_heap_t* dummy = _mi_heap_default;
  if (dummy == NULL) return;
  #endif
  os_preloading = false;
  mi_assert_internal(_mi_is_main_thread());
  _mi_options_init();
  mi_process_setup_auto_thread_done();
  mi_process_init();
  if (_mi_is_redirected()) _mi_verbose_message("malloc is redirected.\n");
  const char* msg = NULL;
  _mi_allocator_init(&msg);
  if (msg != NULL && (mi_option_is_enabled(mi_option_verbose) || mi_option_is_enabled(mi_option_show_errors))) {
    _mi_fputs(NULL,NULL,NULL,msg);
  }
  _mi_random_reinit_if_weak(&_mi_heap_main.random);
}
#if defined(_WIN32) && (defined(_M_IX86) || defined(_M_X64))
#include <intrin.h>
mi_decl_cache_align bool _mi_cpu_has_fsrm = false;
mi_decl_cache_align bool _mi_cpu_has_erms = false;
static void mi_detect_cpu_features(void) {
  int32_t cpu_info[4];
  __cpuid(cpu_info, 7);
  _mi_cpu_has_fsrm = ((cpu_info[3] & (1 << 4)) != 0);
  _mi_cpu_has_erms = ((cpu_info[2] & (1 << 9)) != 0);
}
#else
static void mi_detect_cpu_features(void) {
}
#endif
void mi_process_init(void) mi_attr_noexcept {
  static mi_atomic_once_t process_init;
 #if _MSC_VER < 1920
 mi_heap_main_init();
 #endif
  if (!mi_atomic_once(&process_init)) return;
  _mi_process_is_initialized = true;
  _mi_verbose_message("process init: 0x%zx\n", _mi_thread_id());
  mi_process_setup_auto_thread_done();
  mi_detect_cpu_features();
  _mi_os_init();
  mi_heap_main_init();
  #if MI_DEBUG
  _mi_verbose_message("debug level : %d\n", MI_DEBUG);
  #endif
  _mi_verbose_message("secure level: %d\n", MI_SECURE);
  _mi_verbose_message("mem tracking: %s\n", MI_TRACK_TOOL);
  #if MI_TSAN
  _mi_verbose_message("thread santizer enabled\n");
  #endif
  mi_thread_init();
  #if defined(_WIN32)
  _mi_prim_thread_associate_default_heap(NULL);
  #endif
  mi_stats_reset();
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
void mi_cdecl _mi_process_done(void) {
  if (!_mi_process_is_initialized) return;
  static bool process_done = false;
  if (process_done) return;
  process_done = true;
  _mi_prim_thread_done_auto_done();
  #ifndef MI_SKIP_COLLECT_ON_EXIT
    #if (MI_DEBUG || !defined(MI_SHARED_LIB))
    mi_collect(true );
    #endif
  #endif
  if (mi_option_is_enabled(mi_option_destroy_on_exit)) {
    mi_collect(true );
    _mi_heap_unsafe_destroy_all();
    _mi_arena_unsafe_destroy_all(& _mi_heap_main_get()->tld->stats);
  }
  if (mi_option_is_enabled(mi_option_show_stats) || mi_option_is_enabled(mi_option_verbose)) {
    mi_stats_print(NULL);
  }
  _mi_allocator_done();
  _mi_verbose_message("process done: 0x%zx\n", _mi_heap_main.thread_id);
  os_preloading = true;
}
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
  while (*src != 0 && dest_size > 1) {
    *dest++ = *src++;
    dest_size--;
  }
  *dest = 0;
}
void _mi_strlcat(char* dest, const char* src, size_t dest_size) {
  if (dest==NULL || src==NULL || dest_size == 0) return;
  while (*dest != 0 && dest_size > 1) {
    dest++;
    dest_size--;
  }
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
#endif
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
  for (size_t i = 1; i <= len; i++) {
    start[len + extra - i] = start[len - i];
  }
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
    for (size_t i = 0; i < (len / 2); i++) {
      char c = start[len - i - 1];
      start[len - i - 1] = start[i];
      start[i] = c;
    }
  }
}
#define MI_NEXTC() c = *in; if (c==0) break; in++;
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
      if ((c >= ' ' && c <= '~') || c=='\n' || c=='\r' || c=='\t') {
        mi_outc(c, &out, end);
      }
    }
    else {
      MI_NEXTC();
      char fill = ' ';
      size_t width = 0;
      char numtype = 'd';
      char numplus = 0;
      bool alignright = true;
      if (c == '+' || c == ' ') { numplus = c; MI_NEXTC(); }
      if (c == '-') { alignright = false; MI_NEXTC(); }
      if (c == '0') { fill = '0'; MI_NEXTC(); }
      if (c >= '1' && c <= '9') {
        width = (c - '0'); MI_NEXTC();
        while (c >= '0' && c <= '9') {
          width = (10 * width) + (c - '0'); MI_NEXTC();
        }
        if (c == 0) break;
      }
      if (c == 'z' || c == 't' || c == 'L') { numtype = c; MI_NEXTC(); }
      else if (c == 'l') {
        numtype = c; MI_NEXTC();
        if (c == 'l') { numtype = 'L'; MI_NEXTC(); }
      }
      char* start = out;
      if (c == 's') {
        const char* s = va_arg(args, const char*);
        mi_outs(s, &out, end);
      }
      else if (c == 'p' || c == 'x' || c == 'u') {
        uintmax_t x = 0;
        if (c == 'x' || c == 'u') {
          if (numtype == 'z') x = va_arg(args, size_t);
          else if (numtype == 't') x = va_arg(args, uintptr_t);
          else if (numtype == 'L') x = va_arg(args, unsigned long long);
          else if (numtype == 'l') x = va_arg(args, unsigned long);
                               else x = va_arg(args, unsigned int);
        }
        else if (c == 'p') {
          x = va_arg(args, uintptr_t);
          mi_outs("0x", &out, end);
          start = out;
          width = (width >= 2 ? width - 2 : 0);
        }
        if (width == 0 && (c == 'x' || c == 'p')) {
          if (c == 'p') { width = 2 * (x <= UINT32_MAX ? 4 : ((x >> 16) <= UINT32_MAX ? 6 : sizeof(void*))); }
          if (width == 0) { width = 2; }
          fill = '0';
        }
        mi_out_num(x, (c == 'x' || c == 'p' ? 16 : 10), numplus, &out, end);
      }
      else if (c == 'i' || c == 'd') {
        intmax_t x = 0;
        if (numtype == 'z') x = va_arg(args, intptr_t );
        else if (numtype == 't') x = va_arg(args, ptrdiff_t);
        else if (numtype == 'L') x = va_arg(args, long long);
        else if (numtype == 'l') x = va_arg(args, long);
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
        mi_outc('%', &out, end);
        mi_outc(c, &out, end);
      }
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
#include <stdio.h>
#include <stdlib.h>
static long mi_max_error_count = 16;
static long mi_max_warning_count = 16;
static void mi_add_stderr_output(void);
int mi_version(void) mi_attr_noexcept {
  return MI_MALLOC_VERSION;
}
typedef enum mi_init_e {
  UNINIT,
  DEFAULTED,
  INITIALIZED
} mi_init_t;
typedef struct mi_option_desc_s {
  long value;
  mi_init_t init;
  mi_option_t option;
  const char* name;
  const char* legacy_name;
} mi_option_desc_t;
#define MI_OPTION(opt) mi_option_ ##opt, #opt, NULL
#define MI_OPTION_LEGACY(opt,legacy) mi_option_ ##opt, #opt, #legacy
#ifndef MI_DEFAULT_VERBOSE
#define MI_DEFAULT_VERBOSE 0
#endif
#ifndef MI_DEFAULT_EAGER_COMMIT
#define MI_DEFAULT_EAGER_COMMIT 1
#endif
#ifndef MI_DEFAULT_ARENA_EAGER_COMMIT
#define MI_DEFAULT_ARENA_EAGER_COMMIT 2
#endif
#ifndef MI_DEFAULT_ARENA_RESERVE
 #if (MI_INTPTR_SIZE>4)
  #define MI_DEFAULT_ARENA_RESERVE 1024L*1024L
 #else
  #define MI_DEFAULT_ARENA_RESERVE 128L*1024L
 #endif
#endif
#ifndef MI_DEFAULT_DISALLOW_ARENA_ALLOC
#define MI_DEFAULT_DISALLOW_ARENA_ALLOC 0
#endif
#ifndef MI_DEFAULT_ALLOW_LARGE_OS_PAGES
#define MI_DEFAULT_ALLOW_LARGE_OS_PAGES 0
#endif
#ifndef MI_DEFAULT_RESERVE_HUGE_OS_PAGES
#define MI_DEFAULT_RESERVE_HUGE_OS_PAGES 0
#endif
#ifndef MI_DEFAULT_RESERVE_OS_MEMORY
#define MI_DEFAULT_RESERVE_OS_MEMORY 0
#endif
#ifndef MI_DEFAULT_GUARDED_SAMPLE_RATE
#if MI_GUARDED
#define MI_DEFAULT_GUARDED_SAMPLE_RATE 4000
#else
#define MI_DEFAULT_GUARDED_SAMPLE_RATE 0
#endif
#endif
static mi_option_desc_t options[_mi_option_last] =
{
#if MI_DEBUG || defined(MI_SHOW_ERRORS)
  { 1, UNINIT, MI_OPTION(show_errors) },
#else
  { 0, UNINIT, MI_OPTION(show_errors) },
#endif
  { 0, UNINIT, MI_OPTION(show_stats) },
  { MI_DEFAULT_VERBOSE, UNINIT, MI_OPTION(verbose) },
  { MI_DEFAULT_EAGER_COMMIT,
       UNINIT, MI_OPTION(eager_commit) },
  { MI_DEFAULT_ARENA_EAGER_COMMIT,
       UNINIT, MI_OPTION_LEGACY(arena_eager_commit,eager_region_commit) },
  { 1, UNINIT, MI_OPTION_LEGACY(purge_decommits,reset_decommits) },
  { MI_DEFAULT_ALLOW_LARGE_OS_PAGES,
       UNINIT, MI_OPTION_LEGACY(allow_large_os_pages,large_os_pages) },
  { MI_DEFAULT_RESERVE_HUGE_OS_PAGES,
       UNINIT, MI_OPTION(reserve_huge_os_pages) },
  {-1, UNINIT, MI_OPTION(reserve_huge_os_pages_at) },
  { MI_DEFAULT_RESERVE_OS_MEMORY,
       UNINIT, MI_OPTION(reserve_os_memory) },
  { 0, UNINIT, MI_OPTION(deprecated_segment_cache) },
  { 0, UNINIT, MI_OPTION(deprecated_page_reset) },
  { 0, UNINIT, MI_OPTION(abandoned_page_purge) },
  { 0, UNINIT, MI_OPTION(deprecated_segment_reset) },
#if defined(__NetBSD__)
  { 0, UNINIT, MI_OPTION(eager_commit_delay) },
#else
  { 1, UNINIT, MI_OPTION(eager_commit_delay) },
#endif
  { 10, UNINIT, MI_OPTION_LEGACY(purge_delay,reset_delay) },
  { 0, UNINIT, MI_OPTION(use_numa_nodes) },
  { 0, UNINIT, MI_OPTION_LEGACY(disallow_os_alloc,limit_os_alloc) },
  { 100, UNINIT, MI_OPTION(os_tag) },
  { 32, UNINIT, MI_OPTION(max_errors) },
  { 32, UNINIT, MI_OPTION(max_warnings) },
  { 10, UNINIT, MI_OPTION(max_segment_reclaim)},
  { 0, UNINIT, MI_OPTION(destroy_on_exit)},
  { MI_DEFAULT_ARENA_RESERVE, UNINIT, MI_OPTION(arena_reserve) },
  { 10, UNINIT, MI_OPTION(arena_purge_mult) },
  { 1, UNINIT, MI_OPTION_LEGACY(purge_extend_delay, decommit_extend_delay) },
  { 1, UNINIT, MI_OPTION(abandoned_reclaim_on_free) },
  { MI_DEFAULT_DISALLOW_ARENA_ALLOC, UNINIT, MI_OPTION(disallow_arena_alloc) },
  { 400, UNINIT, MI_OPTION(retry_on_oom) },
#if defined(MI_VISIT_ABANDONED)
  { 1, INITIALIZED, MI_OPTION(visit_abandoned) },
#else
  { 0, UNINIT, MI_OPTION(visit_abandoned) },
#endif
  { 0, UNINIT, MI_OPTION(guarded_min) },
  { MI_GiB, UNINIT, MI_OPTION(guarded_max) },
  { 0, UNINIT, MI_OPTION(guarded_precise) },
  { MI_DEFAULT_GUARDED_SAMPLE_RATE,
         UNINIT, MI_OPTION(guarded_sample_rate)},
  { 0, UNINIT, MI_OPTION(guarded_sample_seed)},
  { 0, UNINIT, MI_OPTION(target_segments_per_thread) },
};
static void mi_option_init(mi_option_desc_t* desc);
static bool mi_option_has_size_in_kib(mi_option_t option) {
  return (option == mi_option_reserve_os_memory || option == mi_option_arena_reserve);
}
void _mi_options_init(void) {
  mi_add_stderr_output();
  for(int i = 0; i < _mi_option_last; i++ ) {
    mi_option_t option = (mi_option_t)i;
    long l = mi_option_get(option); MI_UNUSED(l);
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
  #endif
}
long _mi_option_get_fast(mi_option_t option) {
  mi_assert(option >= 0 && option < _mi_option_last);
  mi_option_desc_t* desc = &options[option];
  mi_assert(desc->option == option);
  return desc->value;
}
mi_decl_nodiscard long mi_option_get(mi_option_t option) {
  mi_assert(option >= 0 && option < _mi_option_last);
  if (option < 0 || option >= _mi_option_last) return 0;
  mi_option_desc_t* desc = &options[option];
  mi_assert(desc->option == option);
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
  mi_assert(desc->option == option);
  desc->value = value;
  desc->init = INITIALIZED;
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
#ifndef MI_MAX_DELAY_OUTPUT
#define MI_MAX_DELAY_OUTPUT ((size_t)(16*1024))
#endif
static char out_buf[MI_MAX_DELAY_OUTPUT+1];
static _Atomic(size_t) out_len;
static void mi_cdecl mi_out_buf(const char* msg, void* arg) {
  MI_UNUSED(arg);
  if (msg==NULL) return;
  if (mi_atomic_load_relaxed(&out_len)>=MI_MAX_DELAY_OUTPUT) return;
  size_t n = _mi_strlen(msg);
  if (n==0) return;
  size_t start = mi_atomic_add_acq_rel(&out_len, n);
  if (start >= MI_MAX_DELAY_OUTPUT) return;
  if (start+n >= MI_MAX_DELAY_OUTPUT) {
    n = MI_MAX_DELAY_OUTPUT-start-1;
  }
  _mi_memcpy(&out_buf[start], msg, n);
}
static void mi_out_buf_flush(mi_output_fun* out, bool no_more_buf, void* arg) {
  if (out==NULL) return;
  size_t count = mi_atomic_add_acq_rel(&out_len, (no_more_buf ? MI_MAX_DELAY_OUTPUT : 1));
  if (count>MI_MAX_DELAY_OUTPUT) count = MI_MAX_DELAY_OUTPUT;
  out_buf[count] = 0;
  out(out_buf,arg);
  if (!no_more_buf) {
    out_buf[count] = '\n';
  }
}
static void mi_cdecl mi_out_buf_stderr(const char* msg, void* arg) {
  mi_out_stderr(msg,arg);
  mi_out_buf(msg,arg);
}
static mi_output_fun* volatile mi_out_default;
static _Atomic(void*) mi_out_arg;
static mi_output_fun* mi_out_get_default(void** parg) {
  if (parg != NULL) { *parg = mi_atomic_load_ptr_acquire(void,&mi_out_arg); }
  mi_output_fun* out = mi_out_default;
  return (out == NULL ? &mi_out_buf : out);
}
void mi_register_output(mi_output_fun* out, void* arg) mi_attr_noexcept {
  mi_out_default = (out == NULL ? &mi_out_stderr : out);
  mi_atomic_store_ptr_release(void,&mi_out_arg, arg);
  if (out!=NULL) mi_out_buf_flush(out,true,arg);
}
static void mi_add_stderr_output(void) {
  mi_assert_internal(mi_out_default == NULL);
  mi_out_buf_flush(&mi_out_stderr, false, NULL);
  mi_out_default = &mi_out_buf_stderr;
}
static _Atomic(size_t) error_count;
static _Atomic(size_t) warning_count;
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
  #endif
  return mi_recurse_enter_prim();
}
static void mi_recurse_exit(void) {
  #if defined(__APPLE__) || defined(MI_TLS_RECURSE_GUARD)
  if (_mi_preloading()) return;
  #endif
  mi_recurse_exit_prim();
}
void _mi_fputs(mi_output_fun* out, void* arg, const char* prefix, const char* message) {
  if (out==NULL || (void*)out==(void*)stdout || (void*)out==(void*)stderr) {
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
  if (mi_option_get(mi_option_verbose) <= 1) return;
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
#endif
static mi_error_fun* volatile mi_error_handler;
static _Atomic(void*) mi_error_arg;
static void mi_error_default(int err) {
  MI_UNUSED(err);
#if (MI_DEBUG>0)
  if (err==EFAULT) {
#if 0
    #endif
    abort();
  }
#endif
#if (MI_SECURE>0)
  if (err==EFAULT) {
    abort();
  }
#endif
#if defined(MI_XMALLOC)
  if (err==ENOMEM || err==EOVERFLOW) {
    abort();
  }
#endif
}
void mi_register_error(mi_error_fun* fun, void* arg) {
  mi_error_handler = fun;
  mi_atomic_store_ptr_release(void,&mi_error_arg, arg);
}
void _mi_error_message(int err, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  mi_show_error_message(fmt, args);
  va_end(args);
  if (mi_error_handler != NULL) {
    mi_error_handler(err, mi_atomic_load_ptr_acquire(void,&mi_error_arg));
  }
  else {
    mi_error_default(err);
  }
}
#include <stdlib.h>
#include <string.h>
static void mi_option_init(mi_option_desc_t* desc) {
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
        size_t size = (value < 0 ? 0 : (size_t)value);
        bool overflow = false;
        if (*end == 'K') { end++; }
        else if (*end == 'M') { overflow = mi_mul_overflow(size,MI_KiB,&size); end++; }
        else if (*end == 'G') { overflow = mi_mul_overflow(size,MI_MiB,&size); end++; }
        else if (*end == 'T') { overflow = mi_mul_overflow(size,MI_GiB,&size); end++; }
        else { size = (size + MI_KiB - 1) / MI_KiB; }
        if (end[0] == 'I' && end[1] == 'B') { end += 2; }
        else if (*end == 'B') { end++; }
        if (overflow || size > MI_MAX_ALLOC_SIZE) { size = (MI_MAX_ALLOC_SIZE / MI_KiB); }
        value = (size > LONG_MAX ? LONG_MAX : (long)size);
      }
      if (*end == 0) {
        mi_option_set(desc->option, value);
      }
      else {
        desc->init = DEFAULTED;
        if (desc->option == mi_option_verbose && desc->value == 0) {
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
#ifndef MI_DEFAULT_VIRTUAL_ADDRESS_BITS
#if MI_INTPTR_SIZE < 8
#define MI_DEFAULT_VIRTUAL_ADDRESS_BITS 32
#else
#define MI_DEFAULT_VIRTUAL_ADDRESS_BITS 48
#endif
#endif
#ifndef MI_DEFAULT_PHYSICAL_MEMORY
#if MI_INTPTR_SIZE < 8
#define MI_DEFAULT_PHYSICAL_MEMORY 4*MI_GiB
#else
#define MI_DEFAULT_PHYSICAL_MEMORY 32*MI_GiB
#endif
#endif
static mi_os_mem_config_t mi_os_mem_config = {
  4096,
  0,
  4096,
  MI_DEFAULT_PHYSICAL_MEMORY,
  MI_DEFAULT_VIRTUAL_ADDRESS_BITS,
  true,
  false,
  true
};
bool _mi_os_has_overcommit(void) {
  return mi_os_mem_config.has_overcommit;
}
bool _mi_os_has_virtual_reserve(void) {
  return mi_os_mem_config.has_virtual_reserve;
}
size_t _mi_os_page_size(void) {
  return mi_os_mem_config.page_size;
}
size_t _mi_os_large_page_size(void) {
  return (mi_os_mem_config.large_page_size != 0 ? mi_os_mem_config.large_page_size : _mi_os_page_size());
}
bool _mi_os_use_large_page(size_t size, size_t alignment) {
  if (mi_os_mem_config.large_page_size == 0 || !mi_option_is_enabled(mi_option_allow_large_os_pages)) return false;
  return ((size % mi_os_mem_config.large_page_size) == 0 && (alignment % mi_os_mem_config.large_page_size) == 0);
}
size_t _mi_os_good_alloc_size(size_t size) {
  size_t align_size;
  if (size < 512*MI_KiB) align_size = _mi_os_page_size();
  else if (size < 2*MI_MiB) align_size = 64*MI_KiB;
  else if (size < 8*MI_MiB) align_size = 256*MI_KiB;
  else if (size < 32*MI_MiB) align_size = 1*MI_MiB;
  else align_size = 4*MI_MiB;
  if mi_unlikely(size >= (SIZE_MAX - align_size)) return size;
  return _mi_align_up(size, align_size);
}
void _mi_os_init(void) {
  _mi_prim_mem_init(&mi_os_mem_config);
}
bool _mi_os_decommit(void* addr, size_t size, mi_stats_t* stats);
bool _mi_os_commit(void* addr, size_t size, bool* is_zero, mi_stats_t* tld_stats);
static inline uintptr_t _mi_align_down(uintptr_t sz, size_t alignment) {
  mi_assert_internal(alignment != 0);
  uintptr_t mask = alignment - 1;
  if ((alignment & mask) == 0) {
    return (sz & ~mask);
  }
  else {
    return ((sz / alignment) * alignment);
  }
}
static void* mi_align_down_ptr(void* p, size_t alignment) {
  return (void*)_mi_align_down((uintptr_t)p, alignment);
}
#if (MI_INTPTR_SIZE >= 8) && !defined(MI_NO_ALIGNED_HINT)
static mi_decl_cache_align _Atomic(uintptr_t)aligned_base;
#define MI_HINT_BASE ((uintptr_t)2 << 40)
#define MI_HINT_AREA ((uintptr_t)4 << 40)
#define MI_HINT_MAX ((uintptr_t)30 << 40)
void* _mi_os_get_aligned_hint(size_t try_alignment, size_t size)
{
  if (try_alignment <= 1 || try_alignment > MI_SEGMENT_SIZE) return NULL;
  if (mi_os_mem_config.virtual_address_bits < 46) return NULL;
  size = _mi_align_up(size, MI_SEGMENT_SIZE);
  if (size > 1*MI_GiB) return NULL;
  #if (MI_SECURE>0)
  size += MI_SEGMENT_SIZE;
  #endif
  uintptr_t hint = mi_atomic_add_acq_rel(&aligned_base, size);
  if (hint == 0 || hint > MI_HINT_MAX) {
    uintptr_t init = MI_HINT_BASE;
    #if (MI_SECURE>0 || MI_DEBUG==0)
    uintptr_t r = _mi_heap_random_next(mi_prim_get_default_heap());
    init = init + ((MI_SEGMENT_SIZE * ((r>>17) & 0xFFFFF)) % MI_HINT_AREA);
    #endif
    uintptr_t expected = hint + size;
    mi_atomic_cas_strong_acq_rel(&aligned_base, &expected, init);
    hint = mi_atomic_add_acq_rel(&aligned_base, size);
  }
  if (hint%try_alignment != 0) return NULL;
  return (void*)hint;
}
#else
void* _mi_os_get_aligned_hint(size_t try_alignment, size_t size) {
  MI_UNUSED(try_alignment); MI_UNUSED(size);
  return NULL;
}
#endif
static void mi_os_free_huge_os_pages(void* p, size_t size, mi_stats_t* stats);
static void mi_os_prim_free(void* addr, size_t size, bool still_committed, mi_stats_t* tld_stats) {
  MI_UNUSED(tld_stats);
  mi_stats_t* stats = &_mi_stats_main;
  mi_assert_internal((size % _mi_os_page_size()) == 0);
  if (addr == NULL || size == 0) return;
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
    if (memid.mem.os.base != NULL) {
      mi_assert(memid.mem.os.base <= addr);
      mi_assert((uint8_t*)memid.mem.os.base + memid.mem.os.alignment >= (uint8_t*)addr);
      base = memid.mem.os.base;
      csize += ((uint8_t*)addr - (uint8_t*)memid.mem.os.base);
    }
    if (memid.memkind == MI_MEM_OS_HUGE) {
      mi_assert(memid.is_pinned);
      mi_os_free_huge_os_pages(base, csize, stats);
    }
    else {
      mi_os_prim_free(base, csize, still_committed, stats);
    }
  }
  else {
    mi_assert(memid.memkind < MI_MEM_OS);
  }
}
void _mi_os_free(void* p, size_t size, mi_memid_t memid, mi_stats_t* stats) {
  if (stats == NULL) stats = &_mi_stats_main;
  _mi_os_free_ex(p, size, true, memid, stats);
}
static void* mi_os_prim_alloc_at(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, mi_stats_t* tld_stats) {
  mi_assert_internal(size > 0 && (size % _mi_os_page_size()) == 0);
  mi_assert_internal(is_zero != NULL);
  mi_assert_internal(is_large != NULL);
  if (size == 0) return NULL;
  if (!commit) { allow_large = false; }
  if (try_alignment == 0) { try_alignment = 1; }
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
      #ifdef MI_TRACK_ASAN
      if (*is_zero) { mi_track_mem_defined(p,size); }
               else { mi_track_mem_undefined(p,size); }
      #endif
    }
  }
  return p;
}
static void* mi_os_prim_alloc(size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, mi_stats_t* tld_stats) {
  return mi_os_prim_alloc_at(NULL, size, try_alignment, commit, allow_large, is_large, is_zero, tld_stats);
}
static void* mi_os_prim_alloc_aligned(size_t size, size_t alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** base, mi_stats_t* stats) {
  mi_assert_internal(alignment >= _mi_os_page_size() && ((alignment & (alignment - 1)) == 0));
  mi_assert_internal(size > 0 && (size % _mi_os_page_size()) == 0);
  mi_assert_internal(is_large != NULL);
  mi_assert_internal(is_zero != NULL);
  mi_assert_internal(base != NULL);
  if (!commit) allow_large = false;
  if (!(alignment >= _mi_os_page_size() && ((alignment & (alignment - 1)) == 0))) return NULL;
  size = _mi_align_up(size, _mi_os_page_size());
  void* p = mi_os_prim_alloc(size, alignment, commit, allow_large, is_large, is_zero, stats);
  if (p == NULL) return NULL;
  if (((uintptr_t)p % alignment) == 0) {
    *base = p;
  }
  else {
    #if !MI_TRACK_ASAN
    _mi_warning_message("unable to allocate aligned OS memory directly, fall back to over-allocation (size: 0x%zx bytes, address: %p, alignment: 0x%zx, commit: %d)\n", size, p, alignment, commit);
    #endif
    mi_os_prim_free(p, size, commit, stats);
    if (size >= (SIZE_MAX - alignment)) return NULL;
    const size_t over_size = size + alignment;
    if (!mi_os_mem_config.has_partial_free) {
      p = mi_os_prim_alloc(over_size, 1 , false , false , is_large, is_zero, stats);
      if (p == NULL) return NULL;
      *base = p;
      p = mi_align_up_ptr(p, alignment);
      if (commit) {
        _mi_os_commit(p, size, NULL, stats);
      }
    }
    else {
      p = mi_os_prim_alloc(over_size, 1, commit, false, is_large, is_zero, stats);
      if (p == NULL) return NULL;
      void* aligned_p = mi_align_up_ptr(p, alignment);
      size_t pre_size = (uint8_t*)aligned_p - (uint8_t*)p;
      size_t mid_size = _mi_align_up(size, _mi_os_page_size());
      size_t post_size = over_size - pre_size - mid_size;
      mi_assert_internal(pre_size < over_size&& post_size < over_size&& mid_size >= size);
      if (pre_size > 0) { mi_os_prim_free(p, pre_size, commit, stats); }
      if (post_size > 0) { mi_os_prim_free((uint8_t*)aligned_p + mid_size, post_size, commit, stats); }
      p = aligned_p;
      *base = aligned_p;
    }
  }
  mi_assert_internal(p == NULL || (p != NULL && *base != NULL && ((uintptr_t)p % alignment) == 0));
  return p;
}
void* _mi_os_alloc(size_t size, mi_memid_t* memid, mi_stats_t* stats) {
  *memid = _mi_memid_none();
  if (size == 0) return NULL;
  if (stats == NULL) stats = &_mi_stats_main;
  size = _mi_os_good_alloc_size(size);
  bool os_is_large = false;
  bool os_is_zero = false;
  void* p = mi_os_prim_alloc(size, 0, true, false, &os_is_large, &os_is_zero, stats);
  if (p != NULL) {
    *memid = _mi_memid_create_os(true, os_is_zero, os_is_large);
  }
  return p;
}
void* _mi_os_alloc_aligned(size_t size, size_t alignment, bool commit, bool allow_large, mi_memid_t* memid, mi_stats_t* stats)
{
  MI_UNUSED(&_mi_os_get_aligned_hint);
  *memid = _mi_memid_none();
  if (size == 0) return NULL;
  if (stats == NULL) stats = &_mi_stats_main;
  size = _mi_os_good_alloc_size(size);
  alignment = _mi_align_up(alignment, _mi_os_page_size());
  bool os_is_large = false;
  bool os_is_zero = false;
  void* os_base = NULL;
  void* p = mi_os_prim_alloc_aligned(size, alignment, commit, allow_large, &os_is_large, &os_is_zero, &os_base, stats );
  if (p != NULL) {
    *memid = _mi_memid_create_os(commit, os_is_zero, os_is_large);
    memid->mem.os.base = os_base;
    memid->mem.os.alignment = alignment;
  }
  return p;
}
void* _mi_os_alloc_aligned_at_offset(size_t size, size_t alignment, size_t offset, bool commit, bool allow_large, mi_memid_t* memid, mi_stats_t* stats) {
  mi_assert(offset <= MI_SEGMENT_SIZE);
  mi_assert(offset <= size);
  mi_assert((alignment % _mi_os_page_size()) == 0);
  *memid = _mi_memid_none();
  if (stats == NULL) stats = &_mi_stats_main;
  if (offset > MI_SEGMENT_SIZE) return NULL;
  if (offset == 0) {
    return _mi_os_alloc_aligned(size, alignment, commit, allow_large, memid, stats);
  }
  else {
    const size_t extra = _mi_align_up(offset, alignment) - offset;
    const size_t oversize = size + extra;
    void* const start = _mi_os_alloc_aligned(oversize, alignment, commit, allow_large, memid, stats);
    if (start == NULL) return NULL;
    void* const p = (uint8_t*)start + extra;
    mi_assert(_mi_is_aligned((uint8_t*)p + offset, alignment));
    if (commit && extra > _mi_os_page_size()) {
      _mi_os_decommit(start, extra, stats);
    }
    return p;
  }
}
static void* mi_os_page_align_areax(bool conservative, void* addr, size_t size, size_t* newsize) {
  mi_assert(addr != NULL && size > 0);
  if (newsize != NULL) *newsize = 0;
  if (size == 0 || addr == NULL) return NULL;
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
  _mi_stat_increase(&stats->committed, size);
  _mi_stat_counter_increase(&stats->commit_calls, 1);
  size_t csize;
  void* start = mi_os_page_align_areax(false , addr, size, &csize);
  if (csize == 0) return true;
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
  #ifdef MI_TRACK_ASAN
  if (os_is_zero) { mi_track_mem_defined(start,csize); }
             else { mi_track_mem_undefined(start,csize); }
  #endif
  return true;
}
static bool mi_os_decommit_ex(void* addr, size_t size, bool* needs_recommit, mi_stats_t* tld_stats) {
  MI_UNUSED(tld_stats);
  mi_stats_t* stats = &_mi_stats_main;
  mi_assert_internal(needs_recommit!=NULL);
  _mi_stat_decrease(&stats->committed, size);
  size_t csize;
  void* start = mi_os_page_align_area_conservative(addr, size, &csize);
  if (csize == 0) return true;
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
bool _mi_os_reset(void* addr, size_t size, mi_stats_t* stats) {
  size_t csize;
  void* start = mi_os_page_align_area_conservative(addr, size, &csize);
  if (csize == 0) return true;
  _mi_stat_increase(&stats->reset, csize);
  _mi_stat_counter_increase(&stats->reset_calls, 1);
  #if (MI_DEBUG>1) && !MI_SECURE && !MI_TRACK_ENABLED
  memset(start, 0, csize);
  #endif
  int err = _mi_prim_reset(start, csize);
  if (err != 0) {
    _mi_warning_message("cannot reset OS memory (error: %d (0x%x), address: %p, size: 0x%zx bytes)\n", err, err, start, csize);
  }
  return (err == 0);
}
bool _mi_os_purge_ex(void* p, size_t size, bool allow_reset, mi_stats_t* stats)
{
  if (mi_option_get(mi_option_purge_delay) < 0) return false;
  _mi_stat_counter_increase(&stats->purge_calls, 1);
  _mi_stat_increase(&stats->purged, size);
  if (mi_option_is_enabled(mi_option_purge_decommits) &&
    !_mi_preloading())
  {
    bool needs_recommit = true;
    mi_os_decommit_ex(p, size, &needs_recommit, stats);
    return needs_recommit;
  }
  else {
    if (allow_reset) {
      _mi_os_reset(p, size, stats);
    }
    return false;
  }
}
bool _mi_os_purge(void* p, size_t size, mi_stats_t * stats) {
  return _mi_os_purge_ex(p, size, true, stats);
}
static bool mi_os_protectx(void* addr, size_t size, bool protect) {
  size_t csize = 0;
  void* start = mi_os_page_align_area_conservative(addr, size, &csize);
  if (csize == 0) return false;
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
#define MI_HUGE_OS_PAGE_SIZE (MI_GiB)
#if (MI_INTPTR_SIZE >= 8)
static mi_decl_cache_align _Atomic(uintptr_t) mi_huge_start;
static uint8_t* mi_os_claim_huge_pages(size_t pages, size_t* total_size) {
  if (total_size != NULL) *total_size = 0;
  const size_t size = pages * MI_HUGE_OS_PAGE_SIZE;
  uintptr_t start = 0;
  uintptr_t end = 0;
  uintptr_t huge_start = mi_atomic_load_relaxed(&mi_huge_start);
  do {
    start = huge_start;
    if (start == 0) {
      start = ((uintptr_t)32 << 40);
    #if (MI_SECURE>0 || MI_DEBUG==0)
      uintptr_t r = _mi_heap_random_next(mi_prim_get_default_heap());
      start = start + ((uintptr_t)MI_HUGE_OS_PAGE_SIZE * ((r>>17) & 0x0FFF));
    #endif
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
#endif
void* _mi_os_alloc_huge_os_pages(size_t pages, int numa_node, mi_msecs_t max_msecs, size_t* pages_reserved, size_t* psize, mi_memid_t* memid) {
  *memid = _mi_memid_none();
  if (psize != NULL) *psize = 0;
  if (pages_reserved != NULL) *pages_reserved = 0;
  size_t size = 0;
  uint8_t* start = mi_os_claim_huge_pages(pages, &size);
  if (start == NULL) return NULL;
  mi_msecs_t start_t = _mi_clock_start();
  size_t page = 0;
  bool all_zero = true;
  while (page < pages) {
    bool is_zero = false;
    void* addr = start + (page * MI_HUGE_OS_PAGE_SIZE);
    void* p = NULL;
    int err = _mi_prim_alloc_huge_os_pages(addr, MI_HUGE_OS_PAGE_SIZE, numa_node, &is_zero, &p);
    if (!is_zero) { all_zero = false; }
    if (err != 0) {
      _mi_warning_message("unable to allocate huge OS page (error: %d (0x%x), address: %p, size: %zx bytes)\n", err, err, addr, MI_HUGE_OS_PAGE_SIZE);
      break;
    }
    if (p != addr) {
      if (p != NULL) {
        _mi_warning_message("could not allocate contiguous huge OS page %zu at %p\n", page, addr);
        mi_os_prim_free(p, MI_HUGE_OS_PAGE_SIZE, true, &_mi_stats_main);
      }
      break;
    }
    page++;
    _mi_stat_increase(&_mi_stats_main.committed, MI_HUGE_OS_PAGE_SIZE);
    _mi_stat_increase(&_mi_stats_main.reserved, MI_HUGE_OS_PAGE_SIZE);
    if (max_msecs > 0) {
      mi_msecs_t elapsed = _mi_clock_end(start_t);
      if (page >= 1) {
        mi_msecs_t estimate = ((elapsed / (page+1)) * pages);
        if (estimate > 2*max_msecs) {
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
    *memid = _mi_memid_create_os(true , all_zero, true );
    memid->memkind = MI_MEM_OS_HUGE;
    mi_assert(memid->is_pinned);
    #ifdef MI_TRACK_ASAN
    if (all_zero) { mi_track_mem_defined(start,size); }
    #endif
  }
  return (page == 0 ? NULL : start);
}
static void mi_os_free_huge_os_pages(void* p, size_t size, mi_stats_t* stats) {
  if (p==NULL || size==0) return;
  uint8_t* base = (uint8_t*)p;
  while (size >= MI_HUGE_OS_PAGE_SIZE) {
    mi_os_prim_free(base, MI_HUGE_OS_PAGE_SIZE, true, stats);
    size -= MI_HUGE_OS_PAGE_SIZE;
    base += MI_HUGE_OS_PAGE_SIZE;
  }
}
_Atomic(size_t) _mi_numa_node_count;
size_t _mi_os_numa_node_count_get(void) {
  size_t count = mi_atomic_load_acquire(&_mi_numa_node_count);
  if (count <= 0) {
    long ncount = mi_option_get(mi_option_use_numa_nodes);
    if (ncount > 0) {
      count = (size_t)ncount;
    }
    else {
      count = _mi_prim_numa_node_count();
      if (count == 0) count = 1;
    }
    mi_atomic_store_release(&_mi_numa_node_count, count);
    _mi_verbose_message("using %zd numa regions\n", count);
  }
  return count;
}
int _mi_os_numa_node_get(mi_os_tld_t* tld) {
  MI_UNUSED(tld);
  size_t numa_count = _mi_os_numa_node_count();
  if (numa_count<=1) return 0;
  size_t numa_node = _mi_prim_numa_node();
  if (numa_node >= numa_count) { numa_node = numa_node % numa_count; }
  return (int)numa_node;
}
#define MI_IN_PAGE_C 
#ifndef MI_IN_PAGE_C
#error "this file should be included from 'page.c'"
#endif
#if (MI_MAX_ALIGN_SIZE > 4*MI_INTPTR_SIZE)
  #error "define alignment for more than 4x word size for this platform"
#elif (MI_MAX_ALIGN_SIZE > 2*MI_INTPTR_SIZE)
  #define MI_ALIGN4W
#elif (MI_MAX_ALIGN_SIZE > MI_INTPTR_SIZE)
  #define MI_ALIGN2W
#else
#endif
static inline bool mi_page_queue_is_huge(const mi_page_queue_t* pq) {
  return (pq->block_size == (MI_LARGE_OBJ_SIZE_MAX+sizeof(uintptr_t)));
}
static inline bool mi_page_queue_is_full(const mi_page_queue_t* pq) {
  return (pq->block_size == (MI_LARGE_OBJ_SIZE_MAX+(2*sizeof(uintptr_t))));
}
static inline bool mi_page_queue_is_special(const mi_page_queue_t* pq) {
  return (pq->block_size > MI_LARGE_OBJ_SIZE_MAX);
}
static inline uint8_t mi_bin(size_t size) {
  size_t wsize = _mi_wsize_from_size(size);
  uint8_t bin;
  if (wsize <= 1) {
    bin = 1;
  }
  #if defined(MI_ALIGN4W)
  else if (wsize <= 4) {
    bin = (uint8_t)((wsize+1)&~1);
  }
  #elif defined(MI_ALIGN2W)
  else if (wsize <= 8) {
    bin = (uint8_t)((wsize+1)&~1);
  }
  #else
  else if (wsize <= 8) {
    bin = (uint8_t)wsize;
  }
  #endif
  else if (wsize > MI_LARGE_OBJ_WSIZE_MAX) {
    bin = MI_BIN_HUGE;
  }
  else {
    #if defined(MI_ALIGN4W)
    if (wsize <= 16) { wsize = (wsize+3)&~3; }
    #endif
    wsize--;
    uint8_t b = (uint8_t)mi_bsr(wsize);
    bin = ((b << 2) + (uint8_t)((wsize >> (b - 2)) & 0x03)) - 3;
    mi_assert_internal(bin < MI_BIN_HUGE);
  }
  mi_assert_internal(bin > 0 && bin <= MI_BIN_HUGE);
  return bin;
}
uint8_t _mi_bin(size_t size) {
  return mi_bin(size);
}
size_t _mi_bin_size(uint8_t bin) {
  return _mi_heap_empty.pages[bin].block_size;
}
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
#endif
#if (MI_DEBUG>1)
static bool mi_heap_contains_queue(const mi_heap_t* heap, const mi_page_queue_t* pq) {
  return (pq >= &heap->pages[0] && pq <= &heap->pages[MI_BIN_FULL]);
}
#endif
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
static inline void mi_heap_queue_first_update(mi_heap_t* heap, const mi_page_queue_t* pq) {
  mi_assert_internal(mi_heap_contains_queue(heap,pq));
  size_t size = pq->block_size;
  if (size > MI_SMALL_SIZE_MAX) return;
  mi_page_t* page = pq->first;
  if (pq->first == NULL) page = (mi_page_t*)&_mi_page_empty;
  size_t start;
  size_t idx = _mi_wsize_from_size(size);
  mi_page_t** pages_free = heap->pages_free_direct;
  if (pages_free[idx] == page) return;
  if (idx<=1) {
    start = 0;
  }
  else {
    uint8_t bin = mi_bin(size);
    const mi_page_queue_t* prev = pq - 1;
    while( bin == mi_bin(prev->block_size) && prev > &heap->pages[0]) {
      prev--;
    }
    start = 1 + _mi_wsize_from_size(prev->block_size);
    if (start > idx) start = idx;
  }
  mi_assert(start <= idx);
  for (size_t sz = start; sz <= idx; sz++) {
    pages_free[sz] = page;
  }
}
static void mi_page_queue_remove(mi_page_queue_t* queue, mi_page_t* page) {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(mi_page_queue_contains(queue, page));
  mi_assert_internal(mi_page_block_size(page) == queue->block_size ||
                      (mi_page_is_huge(page) && mi_page_queue_is_huge(queue)) ||
                        (mi_page_is_in_full(page) && mi_page_queue_is_full(queue)));
  mi_heap_t* heap = mi_page_heap(page);
  if (page->prev != NULL) page->prev->next = page->next;
  if (page->next != NULL) page->next->prev = page->prev;
  if (page == queue->last) queue->last = page->prev;
  if (page == queue->first) {
    queue->first = page->next;
    mi_assert_internal(mi_heap_contains_queue(heap, queue));
    mi_heap_queue_first_update(heap,queue);
  }
  heap->page_count--;
  page->next = NULL;
  page->prev = NULL;
  mi_page_set_in_full(page,false);
}
static void mi_page_queue_push(mi_heap_t* heap, mi_page_queue_t* queue, mi_page_t* page) {
  mi_assert_internal(mi_page_heap(page) == heap);
  mi_assert_internal(!mi_page_queue_contains(queue, page));
  #if MI_HUGE_PAGE_ABANDON
  mi_assert_internal(_mi_page_segment(page)->page_kind != MI_PAGE_HUGE);
  #endif
  mi_assert_internal(mi_page_block_size(page) == queue->block_size ||
                      (mi_page_is_huge(page) && mi_page_queue_is_huge(queue)) ||
                        (mi_page_is_in_full(page) && mi_page_queue_is_full(queue)));
  mi_page_set_in_full(page, mi_page_queue_is_full(queue));
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
  if (page->prev != NULL) page->prev->next = page->next;
  if (page->next != NULL) page->next->prev = page->prev;
  if (page == from->last) from->last = page->prev;
  if (page == from->first) {
    from->first = page->next;
    mi_assert_internal(mi_heap_contains_queue(heap, from));
    mi_heap_queue_first_update(heap, from);
  }
  if (enqueue_at_end) {
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
  mi_page_queue_enqueue_from_ex(to, from, true , page);
}
static void mi_page_queue_enqueue_from_full(mi_page_queue_t* to, mi_page_queue_t* from, mi_page_t* page) {
  mi_page_queue_enqueue_from_ex(to, from, false , page);
}
size_t _mi_page_queue_append(mi_heap_t* heap, mi_page_queue_t* pq, mi_page_queue_t* append) {
  mi_assert_internal(mi_heap_contains_queue(heap,pq));
  mi_assert_internal(pq->block_size == append->block_size);
  if (append->first==NULL) return 0;
  size_t count = 0;
  for (mi_page_t* page = append->first; page != NULL; page = page->next) {
    mi_atomic_store_release(&page->xheap, (uintptr_t)heap);
    _mi_page_use_delayed_free(page, MI_USE_DELAYED_FREE, false);
    count++;
  }
  if (pq->last==NULL) {
    mi_assert_internal(pq->first==NULL);
    pq->first = append->first;
    pq->last = append->last;
    mi_heap_queue_first_update(heap, pq);
  }
  else {
    mi_assert_internal(pq->last!=NULL);
    mi_assert_internal(append->first!=NULL);
    pq->last->next = append->first;
    append->first->prev = pq->last;
    pq->last = append->last;
  }
  return count;
}
#undef MI_IN_PAGE_C
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
static bool mi_page_list_is_valid(mi_page_t* page, mi_block_t* p) {
  size_t psize;
  uint8_t* page_area = _mi_segment_page_start(_mi_page_segment(page), page, &psize);
  mi_block_t* start = (mi_block_t*)page_area;
  mi_block_t* end = (mi_block_t*)(page_area + psize);
  while(p != NULL) {
    if (p < start || p >= end) return false;
    p = mi_block_next(page, p);
  }
#if MI_DEBUG>3
  if (page->free_is_zero) {
    const size_t ubsize = mi_page_usable_block_size(page);
    for (mi_block_t* block = page->free; block != NULL; block = mi_block_next(page, block)) {
      mi_assert_expensive(mi_mem_is_zero(block + 1, ubsize - sizeof(mi_block_t)));
    }
  }
#endif
  return true;
}
static bool mi_page_is_valid_init(mi_page_t* page) {
  mi_assert_internal(mi_page_block_size(page) > 0);
  mi_assert_internal(page->used <= page->capacity);
  mi_assert_internal(page->capacity <= page->reserved);
  mi_segment_t* segment = _mi_page_segment(page);
  uint8_t* start = mi_page_start(page);
  mi_assert_internal(start == _mi_segment_page_start(segment,page,NULL));
  mi_assert_internal(page->is_huge == (segment->page_kind == MI_PAGE_HUGE));
  mi_assert_internal(mi_page_list_is_valid(page,page->free));
  mi_assert_internal(mi_page_list_is_valid(page,page->local_free));
  #if MI_DEBUG>3
  if (page->free_is_zero) {
    const size_t ubsize = mi_page_usable_block_size(page);
    for(mi_block_t* block = page->free; block != NULL; block = mi_block_next(page,block)) {
      mi_assert_expensive(mi_mem_is_zero(block + 1, ubsize - sizeof(mi_block_t)));
    }
  }
  #endif
  #if !MI_TRACK_ENABLED && !MI_TSAN
  mi_block_t* tfree = mi_page_thread_free(page);
  mi_assert_internal(mi_page_list_is_valid(page, tfree));
  #endif
  size_t free_count = mi_page_list_count(page, page->free) + mi_page_list_count(page, page->local_free);
  mi_assert_internal(page->used + free_count == page->capacity);
  return true;
}
extern bool _mi_process_is_initialized;
bool _mi_page_is_valid(mi_page_t* page) {
  mi_assert_internal(mi_page_is_valid_init(page));
  #if MI_SECURE
  mi_assert_internal(page->keys[0] != 0);
  #endif
  if (mi_page_heap(page)!=NULL) {
    mi_segment_t* segment = _mi_page_segment(page);
    mi_assert_internal(!_mi_process_is_initialized || segment->thread_id == mi_page_heap(page)->thread_id || segment->thread_id==0);
    #if MI_HUGE_PAGE_ABANDON
    if (segment->page_kind != MI_PAGE_HUGE)
    #endif
    {
      mi_page_queue_t* pq = mi_page_queue_of(page);
      mi_assert_internal(mi_page_queue_contains(pq, page));
      mi_assert_internal(pq->block_size==mi_page_block_size(page) || mi_page_block_size(page) > MI_LARGE_OBJ_SIZE_MAX || mi_page_is_in_full(page));
      mi_assert_internal(mi_heap_contains_queue(mi_page_heap(page),pq));
    }
  }
  return true;
}
#endif
void _mi_page_use_delayed_free(mi_page_t* page, mi_delayed_t delay, bool override_never) {
  while (!_mi_page_try_use_delayed_free(page, delay, override_never)) {
    mi_atomic_yield();
  }
}
bool _mi_page_try_use_delayed_free(mi_page_t* page, mi_delayed_t delay, bool override_never) {
  mi_thread_free_t tfreex;
  mi_delayed_t old_delay;
  mi_thread_free_t tfree;
  size_t yield_count = 0;
  do {
    tfree = mi_atomic_load_acquire(&page->xthread_free);
    tfreex = mi_tf_set_delayed(tfree, delay);
    old_delay = mi_tf_delayed(tfree);
    if mi_unlikely(old_delay == MI_DELAYED_FREEING) {
      if (yield_count >= 4) return false;
      yield_count++;
      mi_atomic_yield();
    }
    else if (delay == old_delay) {
      break;
    }
    else if (!override_never && old_delay == MI_NEVER_DELAYED_FREE) {
      break;
    }
  } while ((old_delay == MI_DELAYED_FREEING) ||
           !mi_atomic_cas_weak_release(&page->xthread_free, &tfree, tfreex));
  return true;
}
static void _mi_page_thread_free_collect(mi_page_t* page)
{
  mi_block_t* head;
  mi_thread_free_t tfreex;
  mi_thread_free_t tfree = mi_atomic_load_relaxed(&page->xthread_free);
  do {
    head = mi_tf_block(tfree);
    tfreex = mi_tf_set_block(tfree,NULL);
  } while (!mi_atomic_cas_weak_acq_rel(&page->xthread_free, &tfree, tfreex));
  if (head == NULL) return;
  size_t max_count = page->capacity;
  size_t count = 1;
  mi_block_t* tail = head;
  mi_block_t* next;
  while ((next = mi_block_next(page,tail)) != NULL && count <= max_count) {
    count++;
    tail = next;
  }
  if (count > max_count) {
    _mi_error_message(EFAULT, "corrupted thread-free list\n");
    return;
  }
  mi_block_set_next(page,tail, page->local_free);
  page->local_free = head;
  page->used -= (uint16_t)count;
}
void _mi_page_free_collect(mi_page_t* page, bool force) {
  mi_assert_internal(page!=NULL);
  if (force || mi_page_thread_free(page) != NULL) {
    _mi_page_thread_free_collect(page);
  }
  if (page->local_free != NULL) {
    if mi_likely(page->free == NULL) {
      page->free = page->local_free;
      page->local_free = NULL;
      page->free_is_zero = false;
    }
    else if (force) {
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
void _mi_page_reclaim(mi_heap_t* heap, mi_page_t* page) {
  mi_assert_expensive(mi_page_is_valid_init(page));
  mi_assert_internal(mi_page_heap(page) == heap);
  mi_assert_internal(mi_page_thread_free_flag(page) != MI_NEVER_DELAYED_FREE);
  #if MI_HUGE_PAGE_ABANDON
  mi_assert_internal(_mi_page_segment(page)->page_kind != MI_PAGE_HUGE);
  #endif
  mi_page_queue_t* pq = mi_page_queue(heap, mi_page_block_size(page));
  mi_page_queue_push(heap, pq, page);
  mi_assert_expensive(_mi_page_is_valid(page));
}
static mi_page_t* mi_page_fresh_alloc(mi_heap_t* heap, mi_page_queue_t* pq, size_t block_size, size_t page_alignment) {
  #if !MI_HUGE_PAGE_ABANDON
  mi_assert_internal(pq != NULL);
  mi_assert_internal(mi_heap_contains_queue(heap, pq));
  mi_assert_internal(page_alignment > 0 || block_size > MI_LARGE_OBJ_SIZE_MAX || block_size == pq->block_size);
  #endif
  mi_page_t* page = _mi_segment_page_alloc(heap, block_size, page_alignment, &heap->tld->segments, &heap->tld->os);
  if (page == NULL) {
    return NULL;
  }
  #if MI_HUGE_PAGE_ABANDON
  mi_assert_internal(pq==NULL || _mi_page_segment(page)->page_kind != MI_PAGE_HUGE);
  #endif
  mi_assert_internal(pq!=NULL || mi_page_block_size(page) >= block_size);
  const size_t full_block_size = (pq == NULL || mi_page_is_huge(page) ? mi_page_block_size(page) : block_size);
  mi_assert_internal(full_block_size >= block_size);
  mi_page_init(heap, page, full_block_size, heap->tld);
  mi_heap_stat_increase(heap, pages, 1);
  if (pq != NULL) { mi_page_queue_push(heap, pq, page); }
  mi_assert_expensive(_mi_page_is_valid(page));
  return page;
}
static mi_page_t* mi_page_fresh(mi_heap_t* heap, mi_page_queue_t* pq) {
  mi_assert_internal(mi_heap_contains_queue(heap, pq));
  mi_page_t* page = mi_page_fresh_alloc(heap, pq, pq->block_size, 0);
  if (page==NULL) return NULL;
  mi_assert_internal(pq->block_size==mi_page_block_size(page));
  mi_assert_internal(pq==mi_page_queue(heap, mi_page_block_size(page)));
  return page;
}
void _mi_heap_delayed_free_all(mi_heap_t* heap) {
  while (!_mi_heap_delayed_free_partial(heap)) {
    mi_atomic_yield();
  }
}
bool _mi_heap_delayed_free_partial(mi_heap_t* heap) {
  mi_block_t* block = mi_atomic_load_ptr_relaxed(mi_block_t, &heap->thread_delayed_free);
  while (block != NULL && !mi_atomic_cas_ptr_weak_acq_rel(mi_block_t, &heap->thread_delayed_free, &block, NULL)) { };
  bool all_freed = true;
  while(block != NULL) {
    mi_block_t* next = mi_block_nextx(heap,block, heap->keys);
    if (!_mi_free_delayed_block(block)) {
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
void _mi_page_unfull(mi_page_t* page) {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(_mi_page_is_valid(page));
  mi_assert_internal(mi_page_is_in_full(page));
  if (!mi_page_is_in_full(page)) return;
  mi_heap_t* heap = mi_page_heap(page);
  mi_page_queue_t* pqfull = &heap->pages[MI_BIN_FULL];
  mi_page_set_in_full(page, false);
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
  _mi_page_free_collect(page,false);
}
void _mi_page_abandon(mi_page_t* page, mi_page_queue_t* pq) {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(_mi_page_is_valid(page));
  mi_assert_internal(pq == mi_page_queue_of(page));
  mi_assert_internal(mi_page_heap(page) != NULL);
  mi_heap_t* pheap = mi_page_heap(page);
  mi_segments_tld_t* segments_tld = &pheap->tld->segments;
  mi_page_queue_remove(pq, page);
  mi_assert_internal(mi_page_thread_free_flag(page)==MI_NEVER_DELAYED_FREE);
  mi_page_set_heap(page, NULL);
#if (MI_DEBUG>1) && !MI_TRACK_ENABLED
  for (mi_block_t* block = (mi_block_t*)pheap->thread_delayed_free; block != NULL; block = mi_block_nextx(pheap, block, pheap->keys)) {
    mi_assert_internal(_mi_ptr_page(block) != page);
  }
#endif
  mi_assert_internal(mi_page_heap(page) == NULL);
  _mi_segment_page_abandon(page,segments_tld);
}
void _mi_page_force_abandon(mi_page_t* page) {
  mi_heap_t* heap = mi_page_heap(page);
  _mi_page_use_delayed_free(page, MI_NEVER_DELAYED_FREE, false);
  _mi_heap_delayed_free_all(heap);
  if (page->capacity == 0) return;
  mi_page_queue_t* pq = mi_heap_page_queue_of(heap, page);
  if (mi_page_all_free(page)) {
    _mi_page_free(page, pq, false);
  }
  else {
    _mi_page_abandon(page, pq);
  }
}
void _mi_page_free(mi_page_t* page, mi_page_queue_t* pq, bool force) {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(_mi_page_is_valid(page));
  mi_assert_internal(pq == mi_page_queue_of(page));
  mi_assert_internal(mi_page_all_free(page));
  mi_assert_internal(mi_page_thread_free_flag(page)!=MI_DELAYED_FREEING);
  mi_page_set_has_aligned(page, false);
  mi_segments_tld_t* segments_tld = &mi_page_heap(page)->tld->segments;
  mi_page_queue_remove(pq, page);
  mi_page_set_heap(page,NULL);
  _mi_segment_page_free(page, force, segments_tld);
}
#define MI_MAX_RETIRE_SIZE MI_LARGE_OBJ_SIZE_MAX
#define MI_RETIRE_CYCLES (16)
void _mi_page_retire(mi_page_t* page) mi_attr_noexcept {
  mi_assert_internal(page != NULL);
  mi_assert_expensive(_mi_page_is_valid(page));
  mi_assert_internal(mi_page_all_free(page));
  mi_page_set_has_aligned(page, false);
  mi_page_queue_t* pq = mi_page_queue_of(page);
  #if MI_RETIRE_CYCLES > 0
  const size_t bsize = mi_page_block_size(page);
  if mi_likely( !mi_page_queue_is_special(pq)) {
    if (pq->last==page && pq->first==page) {
      mi_stat_counter_increase(_mi_stats_main.page_no_retire,1);
      page->retire_expire = (bsize <= MI_SMALL_OBJ_SIZE_MAX ? MI_RETIRE_CYCLES : MI_RETIRE_CYCLES/4);
      mi_heap_t* heap = mi_page_heap(page);
      mi_assert_internal(pq >= heap->pages);
      const size_t index = pq - heap->pages;
      mi_assert_internal(index < MI_BIN_FULL && index < MI_BIN_HUGE);
      if (index < heap->page_retired_min) heap->page_retired_min = index;
      if (index > heap->page_retired_max) heap->page_retired_max = index;
      mi_assert_internal(mi_page_all_free(page));
      return;
    }
  }
  #endif
  _mi_page_free(page, pq, false);
}
void _mi_heap_collect_retired(mi_heap_t* heap, bool force) {
  size_t min = MI_BIN_FULL;
  size_t max = 0;
  for(size_t bin = heap->page_retired_min; bin <= heap->page_retired_max; bin++) {
    mi_page_queue_t* pq = &heap->pages[bin];
    mi_page_t* page = pq->first;
    if (page != NULL && page->retire_expire != 0) {
      if (mi_page_all_free(page)) {
        page->retire_expire--;
        if (force || page->retire_expire == 0) {
          _mi_page_free(pq->first, pq, force);
        }
        else {
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
#define MI_MAX_SLICE_SHIFT (6)
#define MI_MAX_SLICES (1UL << MI_MAX_SLICE_SHIFT)
#define MI_MIN_SLICES (2)
static void mi_page_free_list_extend_secure(mi_heap_t* const heap, mi_page_t* const page, const size_t bsize, const size_t extend, mi_stats_t* const stats) {
  MI_UNUSED(stats);
  #if (MI_SECURE<=2)
  mi_assert_internal(page->free == NULL);
  mi_assert_internal(page->local_free == NULL);
  #endif
  mi_assert_internal(page->capacity + extend <= page->reserved);
  mi_assert_internal(bsize == mi_page_block_size(page));
  void* const page_area = mi_page_start(page);
  size_t shift = MI_MAX_SLICE_SHIFT;
  while ((extend >> shift) == 0) {
    shift--;
  }
  const size_t slice_count = (size_t)1U << shift;
  const size_t slice_extend = extend / slice_count;
  mi_assert_internal(slice_extend >= 1);
  mi_block_t* blocks[MI_MAX_SLICES];
  size_t counts[MI_MAX_SLICES];
  for (size_t i = 0; i < slice_count; i++) {
    blocks[i] = mi_page_block_at(page, page_area, bsize, page->capacity + i*slice_extend);
    counts[i] = slice_extend;
  }
  counts[slice_count-1] += (extend % slice_count);
  const uintptr_t r = _mi_heap_random_next(heap);
  size_t current = r % slice_count;
  counts[current]--;
  mi_block_t* const free_start = blocks[current];
  uintptr_t rnd = _mi_random_shuffle(r|1);
  for (size_t i = 1; i < extend; i++) {
    const size_t round = i%MI_INTPTR_SIZE;
    if (round == 0) rnd = _mi_random_shuffle(rnd);
    size_t next = ((rnd >> 8*round) & (slice_count-1));
    while (counts[next]==0) {
      next++;
      if (next==slice_count) next = 0;
    }
    counts[next]--;
    mi_block_t* const block = blocks[current];
    blocks[current] = (mi_block_t*)((uint8_t*)block + bsize);
    mi_block_set_next(page, block, blocks[next]);
    current = next;
  }
  mi_block_set_next(page, blocks[current], page->free);
  page->free = free_start;
}
static mi_decl_noinline void mi_page_free_list_extend( mi_page_t* const page, const size_t bsize, const size_t extend, mi_stats_t* const stats)
{
  MI_UNUSED(stats);
  #if (MI_SECURE <= 2)
  mi_assert_internal(page->free == NULL);
  mi_assert_internal(page->local_free == NULL);
  #endif
  mi_assert_internal(page->capacity + extend <= page->reserved);
  mi_assert_internal(bsize == mi_page_block_size(page));
  void* const page_area = mi_page_start(page);
  mi_block_t* const start = mi_page_block_at(page, page_area, bsize, page->capacity);
  mi_block_t* const last = mi_page_block_at(page, page_area, bsize, page->capacity + extend - 1);
  mi_block_t* block = start;
  while(block <= last) {
    mi_block_t* next = (mi_block_t*)((uint8_t*)block + bsize);
    mi_block_set_next(page,block,next);
    block = next;
  }
  mi_block_set_next(page, last, page->free);
  page->free = start;
}
#define MI_MAX_EXTEND_SIZE (4*1024)
#if (MI_SECURE>0)
#define MI_MIN_EXTEND (8*MI_SECURE)
#else
#define MI_MIN_EXTEND (1)
#endif
static void mi_page_extend_free(mi_heap_t* heap, mi_page_t* page, mi_tld_t* tld) {
  mi_assert_expensive(mi_page_is_valid_init(page));
  #if (MI_SECURE<=2)
  mi_assert(page->free == NULL);
  mi_assert(page->local_free == NULL);
  if (page->free != NULL) return;
  #endif
  if (page->capacity >= page->reserved) return;
  size_t page_size;
  _mi_segment_page_start(_mi_page_segment(page), page, &page_size);
  mi_stat_counter_increase(tld->stats.pages_extended, 1);
  const size_t bsize = mi_page_block_size(page);
  size_t extend = page->reserved - page->capacity;
  mi_assert_internal(extend > 0);
  size_t max_extend = (bsize >= MI_MAX_EXTEND_SIZE ? MI_MIN_EXTEND : MI_MAX_EXTEND_SIZE/bsize);
  if (max_extend < MI_MIN_EXTEND) { max_extend = MI_MIN_EXTEND; }
  mi_assert_internal(max_extend > 0);
  if (extend > max_extend) {
    extend = max_extend;
  }
  mi_assert_internal(extend > 0 && extend + page->capacity <= page->reserved);
  mi_assert_internal(extend < (1UL<<16));
  if (extend < MI_MIN_SLICES || MI_SECURE==0) {
    mi_page_free_list_extend(page, bsize, extend, &tld->stats );
  }
  else {
    mi_page_free_list_extend_secure(heap, page, bsize, extend, &tld->stats);
  }
  page->capacity += (uint16_t)extend;
  mi_stat_increase(tld->stats.page_committed, extend * bsize);
  mi_assert_expensive(mi_page_is_valid_init(page));
}
static void mi_page_init(mi_heap_t* heap, mi_page_t* page, size_t block_size, mi_tld_t* tld) {
  mi_assert(page != NULL);
  mi_segment_t* segment = _mi_page_segment(page);
  mi_assert(segment != NULL);
  mi_assert_internal(block_size > 0);
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
  #endif
  page->free_is_zero = page->is_zero_init;
  #if MI_DEBUG>2
  if (page->is_zero_init) {
    mi_track_mem_defined(page->page_start, page_size);
    mi_assert_expensive(mi_mem_is_zero(page->page_start, page_size));
  }
  #endif
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
  #endif
  mi_assert_internal(page->block_size_shift == 0 || (block_size == ((size_t)1 << page->block_size_shift)));
  mi_assert_expensive(mi_page_is_valid_init(page));
  mi_page_extend_free(heap,page,tld);
  mi_assert(mi_page_immediate_available(page));
}
#define MI_MAX_CANDIDATE_SEARCH (8)
static bool mi_page_is_expandable(const mi_page_t* page) {
  mi_assert_internal(page != NULL);
  mi_assert_internal(page->capacity <= page->reserved);
  return (page->capacity < page->reserved);
}
static mi_page_t* mi_page_queue_find_free_ex(mi_heap_t* heap, mi_page_queue_t* pq, bool first_try)
{
  #if MI_STAT
  size_t count = 0;
  #endif
  size_t candidate_count = 0;
  mi_page_t* page_candidate = NULL;
  mi_page_t* page = pq->first;
  while (page != NULL)
  {
    mi_page_t* next = page->next;
    #if MI_STAT
    count++;
    #endif
    candidate_count++;
    _mi_page_free_collect(page, false);
  #if MI_MAX_CANDIDATE_SEARCH > 1
    const bool immediate_available = mi_page_immediate_available(page);
    if (!immediate_available && !mi_page_is_expandable(page)) {
      mi_assert_internal(!mi_page_is_in_full(page) && !mi_page_immediate_available(page));
      mi_page_to_full(page, pq);
    }
    else {
      if (page_candidate == NULL) {
        page_candidate = page;
        candidate_count = 0;
      }
      else if ( page->used >= page_candidate->used) {
        page_candidate = page;
      }
      if (immediate_available || candidate_count > MI_MAX_CANDIDATE_SEARCH) {
        mi_assert_internal(page_candidate!=NULL);
        break;
      }
    }
  #else
    if (mi_page_immediate_available(page) || mi_page_is_expandable(page)) {
      break;
    }
    mi_assert_internal(!mi_page_is_in_full(page) && !mi_page_immediate_available(page));
    mi_page_to_full(page, pq);
  #endif
    page = next;
  }
  mi_heap_stat_counter_increase(heap, searches, count);
  if (page_candidate != NULL) {
    page = page_candidate;
  }
  if (page != NULL && !mi_page_immediate_available(page)) {
    mi_assert_internal(mi_page_is_expandable(page));
    mi_page_extend_free(heap, page, heap->tld);
  }
  if (page == NULL) {
    _mi_heap_collect_retired(heap, false);
    page = mi_page_fresh(heap, pq);
    if (page == NULL && first_try) {
      page = mi_page_queue_find_free_ex(heap, pq, false);
    }
  }
  else {
    mi_page_queue_move_to_front(heap, pq, page);
    page->retire_expire = 0;
  }
  mi_assert_internal(page == NULL || mi_page_immediate_available(page));
  return page;
}
static inline mi_page_t* mi_find_free_page(mi_heap_t* heap, size_t size) {
  mi_page_queue_t* pq = mi_page_queue(heap, size);
  mi_page_t* page = pq->first;
  if (page != NULL) {
   #if (MI_SECURE>=3)
    if (page->capacity < page->reserved && ((_mi_heap_random_next(heap) & 1) == 1)) {
      mi_page_extend_free(heap, page, heap->tld);
      mi_assert_internal(mi_page_immediate_available(page));
    }
    else
   #endif
    {
      _mi_page_free_collect(page,false);
    }
    if (mi_page_immediate_available(page)) {
      page->retire_expire = 0;
      return page;
    }
  }
  return mi_page_queue_find_free_ex(heap, pq, true);
}
static mi_deferred_free_fun* volatile deferred_free = NULL;
static _Atomic(void*) deferred_arg;
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
static mi_page_t* mi_huge_page_alloc(mi_heap_t* heap, size_t size, size_t page_alignment) {
  size_t block_size = _mi_os_good_alloc_size(size);
  mi_assert_internal(mi_bin(block_size) == MI_BIN_HUGE || page_alignment > 0);
  #if MI_HUGE_PAGE_ABANDON
  mi_page_queue_t* pq = NULL;
  #else
  mi_page_queue_t* pq = mi_page_queue(heap, MI_LARGE_OBJ_SIZE_MAX+1);
  mi_assert_internal(mi_page_queue_is_huge(pq));
  #endif
  mi_page_t* page = mi_page_fresh_alloc(heap, pq, block_size, page_alignment);
  if (page != NULL) {
    mi_assert_internal(mi_page_block_size(page) >= size);
    mi_assert_internal(mi_page_immediate_available(page));
    mi_assert_internal(mi_page_is_huge(page));
    mi_assert_internal(_mi_page_segment(page)->page_kind == MI_PAGE_HUGE);
    mi_assert_internal(_mi_page_segment(page)->used==1);
    #if MI_HUGE_PAGE_ABANDON
    mi_assert_internal(_mi_page_segment(page)->thread_id==0);
    mi_page_set_heap(page, NULL);
    #endif
    mi_heap_stat_increase(heap, huge, mi_page_block_size(page));
    mi_heap_stat_counter_increase(heap, huge_count, 1);
  }
  return page;
}
static mi_page_t* mi_find_page(mi_heap_t* heap, size_t size, size_t huge_alignment) mi_attr_noexcept {
  const size_t req_size = size - MI_PADDING_SIZE;
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
    #if MI_PADDING
    mi_assert_internal(size >= MI_PADDING_SIZE);
    #endif
    return mi_find_free_page(heap, size);
  }
}
void* _mi_malloc_generic(mi_heap_t* heap, size_t size, bool zero, size_t huge_alignment) mi_attr_noexcept
{
  mi_assert_internal(heap != NULL);
  if mi_unlikely(!mi_heap_is_initialized(heap)) {
    heap = mi_heap_get_default();
    if mi_unlikely(!mi_heap_is_initialized(heap)) { return NULL; }
  }
  mi_assert_internal(mi_heap_is_initialized(heap));
  _mi_deferred_free(heap, false);
  _mi_heap_delayed_free_partial(heap);
  mi_page_t* page = mi_find_page(heap, size, huge_alignment);
  if mi_unlikely(page == NULL) {
    mi_heap_collect(heap, true );
    page = mi_find_page(heap, size, huge_alignment);
  }
  if mi_unlikely(page == NULL) {
    const size_t req_size = size - MI_PADDING_SIZE;
    _mi_error_message(ENOMEM, "unable to allocate memory (%zu bytes)\n", req_size);
    return NULL;
  }
  mi_assert_internal(mi_page_immediate_available(page));
  mi_assert_internal(mi_page_block_size(page) >= size);
  if mi_unlikely(zero && mi_page_is_huge(page)) {
    void* p = _mi_page_malloc(heap, page, size);
    mi_assert_internal(p != NULL);
    _mi_memzero_aligned(p, mi_page_usable_block_size(page));
    return p;
  }
  else {
    return _mi_page_malloc_zero(heap, page, size, zero);
  }
}
#include <string.h>
#define MI_CHACHA_ROUNDS (20)
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
  uint32_t x[16];
  for (size_t i = 0; i < 16; i++) {
    x[i] = ctx->input[i];
  }
  for (size_t i = 0; i < MI_CHACHA_ROUNDS; i += 2) {
    qround(x, 0, 4, 8, 12);
    qround(x, 1, 5, 9, 13);
    qround(x, 2, 6, 10, 14);
    qround(x, 3, 7, 11, 15);
    qround(x, 0, 5, 10, 15);
    qround(x, 1, 6, 11, 12);
    qround(x, 2, 7, 8, 13);
    qround(x, 3, 4, 9, 14);
  }
  for (size_t i = 0; i < 16; i++) {
    ctx->output[i] = x[i] + ctx->input[i];
  }
  ctx->output_available = 16;
  ctx->input[12] += 1;
  if (ctx->input[12] == 0) {
    ctx->input[13] += 1;
    if (ctx->input[13] == 0) {
      ctx->input[14] += 1;
    }
  }
}
static uint32_t chacha_next32(mi_random_ctx_t* ctx) {
  if (ctx->output_available <= 0) {
    chacha_block(ctx);
    ctx->output_available = 16;
  }
  const uint32_t x = ctx->output[16 - ctx->output_available];
  ctx->output[16 - ctx->output_available] = 0;
  ctx->output_available--;
  return x;
}
static inline uint32_t read32(const uint8_t* p, size_t idx32) {
  const size_t i = 4*idx32;
  return ((uint32_t)p[i+0] | (uint32_t)p[i+1] << 8 | (uint32_t)p[i+2] << 16 | (uint32_t)p[i+3] << 24);
}
static void chacha_init(mi_random_ctx_t* ctx, const uint8_t key[32], uint64_t nonce)
{
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
  mi_assert_internal(ctx->input[14] != ctx_new->input[14] || ctx->input[15] != ctx_new->input[15]);
  chacha_block(ctx_new);
}
#if MI_DEBUG>1
static bool mi_random_is_initialized(mi_random_ctx_t* ctx) {
  return (ctx != NULL && ctx->input[0] != 0);
}
#endif
void _mi_random_split(mi_random_ctx_t* ctx, mi_random_ctx_t* ctx_new) {
  mi_assert_internal(mi_random_is_initialized(ctx));
  mi_assert_internal(ctx != ctx_new);
  chacha_split(ctx, (uintptr_t)ctx_new , ctx_new);
}
uintptr_t _mi_random_next(mi_random_ctx_t* ctx) {
  mi_assert_internal(mi_random_is_initialized(ctx));
  #if MI_INTPTR_SIZE <= 4
    return chacha_next32(ctx);
  #elif MI_INTPTR_SIZE == 8
    return (((uintptr_t)chacha_next32(ctx) << 32) | chacha_next32(ctx));
  #else
  # error "define mi_random_next for this platform"
  #endif
}
uintptr_t _mi_os_random_weak(uintptr_t extra_seed) {
  uintptr_t x = (uintptr_t)&_mi_os_random_weak ^ extra_seed;
  x ^= _mi_prim_clock_now();
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
    #if !defined(__wasi__)
    if (!use_weak) { _mi_warning_message("unable to use secure randomness\n"); }
    #endif
    uintptr_t x = _mi_os_random_weak(0);
    for (size_t i = 0; i < 8; i++) {
      x = _mi_random_shuffle(x);
      ((uint32_t*)key)[i] = (uint32_t)x;
    }
    ctx->weak = true;
  }
  else {
    ctx->weak = false;
  }
  chacha_init(ctx, key, (uintptr_t)ctx );
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
#include <string.h>
#include <stdio.h>
#define MI_PAGE_HUGE_ALIGN (256*1024)
static uint8_t* mi_segment_raw_page_start(const mi_segment_t* segment, const mi_page_t* page, size_t* page_size);
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
#endif
static void mi_segment_queue_remove(mi_segment_queue_t* queue, mi_segment_t* segment) {
  mi_assert_expensive(mi_segment_queue_contains(queue, segment));
  if (segment->prev != NULL) segment->prev->next = segment->next;
  if (segment->next != NULL) segment->next->prev = segment->prev;
  if (segment == queue->first) queue->first = segment->next;
  if (segment == queue->last) queue->last = segment->prev;
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
static void mi_segment_remove_from_free_queue(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_segment_queue_t* queue = mi_segment_free_queue(segment, tld);
  bool in_queue = (queue!=NULL && (segment->next != NULL || segment->prev != NULL || queue->first == segment));
  if (in_queue) {
    mi_segment_queue_remove(queue, segment);
  }
}
static void mi_segment_insert_in_free_queue(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_segment_enqueue(mi_segment_free_queue(segment, tld), segment);
}
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
#endif
#if (MI_DEBUG>=2)
static bool mi_pages_purge_contains(const mi_page_t* page, mi_segments_tld_t* tld) {
  mi_page_t* p = tld->pages_purge.first;
  while (p != NULL) {
    if (p == page) return true;
    p = p->next;
  }
  return false;
}
#endif
#if (MI_DEBUG>=3)
static bool mi_segment_is_valid(const mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_assert_internal(segment != NULL);
  mi_assert_internal(_mi_ptr_cookie(segment) == segment->cookie);
  mi_assert_internal(segment->used <= segment->capacity);
  mi_assert_internal(segment->abandoned <= segment->used);
  mi_assert_internal(segment->page_kind <= MI_PAGE_MEDIUM || segment->capacity == 1);
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
  mi_assert_internal(segment->page_kind == MI_PAGE_HUGE ||
                     (mi_segment_page_size(segment) * segment->capacity == segment->segment_size));
  return true;
}
#endif
static bool mi_page_not_in_queue(const mi_page_t* page, mi_segments_tld_t* tld) {
  mi_assert_internal(page != NULL);
  if (page->next != NULL || page->prev != NULL) {
    mi_assert_internal(mi_pages_purge_contains(page, tld));
    return false;
  }
  else {
    return (tld->pages_purge.first != page && tld->pages_purge.last != page);
  }
}
static void mi_segment_protect_range(void* p, size_t size, bool protect) {
  if (protect) {
    _mi_os_protect(p, size);
  }
  else {
    _mi_os_unprotect(p, size);
  }
}
static void mi_segment_protect(mi_segment_t* segment, bool protect, mi_os_tld_t* tld) {
  if (MI_SECURE != 0) {
    const size_t os_psize = _mi_os_page_size();
    mi_assert_internal((segment->segment_info_size - os_psize) >= (sizeof(mi_segment_t) + ((segment->capacity - 1) * sizeof(mi_page_t))));
    mi_assert_internal(((uintptr_t)segment + segment->segment_info_size) % os_psize == 0);
    mi_segment_protect_range((uint8_t*)segment + segment->segment_info_size - os_psize, os_psize, protect);
    #if (MI_SECURE >= 2)
    if (segment->capacity == 1)
    #endif
    {
      mi_assert_internal(MI_SECURE <= 1 || segment->page_kind >= MI_PAGE_LARGE);
      uint8_t* start = (uint8_t*)segment + segment->segment_size - os_psize;
      if (protect && !segment->memid.initially_committed) {
        if (protect) {
          if (_mi_os_commit(start, os_psize, NULL, tld->stats)) {
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
      const size_t page_size = mi_segment_page_size(segment);
      for (size_t i = 0; i < segment->capacity; i++) {
        if (segment->pages[i].is_committed) {
          mi_segment_protect_range((uint8_t*)segment + (i+1)*page_size - os_psize, os_psize, protect);
        }
      }
    }
    #endif
  }
}
static void mi_page_purge(mi_segment_t* segment, mi_page_t* page, mi_segments_tld_t* tld) {
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
  if (!ok) return false;
  page->is_committed = true;
  page->used = 0;
  page->free = NULL;
  page->is_zero_init = is_zero;
  if (gsize > 0) {
    mi_segment_protect_range(start + psize, gsize, true);
  }
  return true;
}
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
    mi_page_purge(segment, page, tld);
  }
  else if (mi_option_get(mi_option_purge_delay) > 0) {
    mi_page_queue_t* pq = &tld->pages_purge;
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
  if (page == pq->last) pq->last = page->prev;
  if (page == pq->first) pq->first = page->next;
  page->next = page->prev = NULL;
  mi_page_set_expire(page,0);
}
static void mi_segment_remove_all_purges(mi_segment_t* segment, bool force_purge, mi_segments_tld_t* tld) {
  if (segment->memid.is_pinned) return;
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
  if (mi_option_get(mi_option_purge_delay) < 0) return;
  mi_msecs_t now = _mi_clock_now();
  mi_page_queue_t* pq = &tld->pages_purge;
  mi_page_t* page = pq->last;
  while (page != NULL && (force || mi_page_purge_is_expired(page,now))) {
    mi_page_t* const prev = page->prev;
    mi_page_purge_remove(page, tld);
    mi_page_purge(_mi_page_segment(page), page, tld);
    page = prev;
  }
  pq->last = page;
  if (page != NULL){
    page->next = NULL;
  }
  else {
    pq->first = NULL;
  }
}
static size_t mi_segment_raw_page_size(const mi_segment_t* segment) {
  return (segment->page_kind == MI_PAGE_HUGE ? segment->segment_size : (size_t)1 << segment->page_shift);
}
static uint8_t* mi_segment_raw_page_start(const mi_segment_t* segment, const mi_page_t* page, size_t* page_size) {
  size_t psize = mi_segment_raw_page_size(segment);
  uint8_t* p = (uint8_t*)segment + page->segment_idx * psize;
  if (page->segment_idx == 0) {
    p += segment->segment_info_size;
    psize -= segment->segment_info_size;
  }
#if (MI_SECURE > 1)
  psize -= _mi_os_page_size();
#elif (MI_SECURE==1)
  if (page->segment_idx == segment->capacity - 1) {
    psize -= _mi_os_page_size();
  }
#endif
  if (page_size != NULL) *page_size = psize;
  mi_assert_internal(page->block_size == 0 || _mi_ptr_page(p) == page);
  mi_assert_internal(_mi_ptr_segment(p) == segment);
  return p;
}
uint8_t* _mi_segment_page_start(const mi_segment_t* segment, const mi_page_t* page, size_t* page_size)
{
  size_t psize;
  uint8_t* p = mi_segment_raw_page_start(segment, page, &psize);
  const size_t block_size = mi_page_block_size(page);
  if ( block_size > 0 && block_size <= MI_MAX_ALIGN_GUARANTEE) {
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
  const size_t minsize = sizeof(mi_segment_t) + ((capacity - 1) * sizeof(mi_page_t)) + 16 ;
  size_t guardsize = 0;
  size_t isize = 0;
  if (MI_SECURE == 0) {
    #if MI_GUARDED
    isize = _mi_align_up(minsize, _mi_os_page_size());
    #else
    isize = _mi_align_up(minsize, 16 * MI_MAX_ALIGN_SIZE);
    #endif
  }
  else {
    const size_t page_size = _mi_os_page_size();
    isize = _mi_align_up(minsize, page_size);
    guardsize = page_size;
  }
  if (info_size != NULL) *info_size = isize;
  if (pre_size != NULL) *pre_size = isize + guardsize;
  return (required==0 ? MI_SEGMENT_SIZE : _mi_align_up( required + isize + 2*guardsize, MI_PAGE_HUGE_ALIGN) );
}
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
    mi_segment_protect(segment, false, tld->os);
  }
  bool fully_committed = true;
  size_t committed_size = 0;
  const size_t page_size = mi_segment_raw_page_size(segment);
  for (size_t i = 0; i < segment->capacity; i++) {
    mi_page_t* page = &segment->pages[i];
    if (page->is_committed) { committed_size += page_size; }
    if (!page->is_committed) { fully_committed = false; }
  }
  MI_UNUSED(fully_committed);
  mi_assert_internal((fully_committed && committed_size == segment_size) || (!fully_committed && committed_size < segment_size));
  _mi_arena_free(segment, segment_size, committed_size, segment->memid, tld->stats);
}
void _mi_segments_collect(bool force, mi_segments_tld_t* tld) {
  mi_pages_try_purge(force,tld);
  #if MI_DEBUG>=2
  if (!_mi_is_main_thread()) {
    mi_assert_internal(tld->pages_purge.first == NULL);
    mi_assert_internal(tld->pages_purge.last == NULL);
  }
  #endif
}
static mi_segment_t* mi_segment_os_alloc(bool eager_delayed, size_t page_alignment, mi_arena_id_t req_arena_id,
                                         size_t pre_size, size_t info_size, bool commit, size_t segment_size,
                                         mi_segments_tld_t* tld, mi_os_tld_t* tld_os)
{
  mi_memid_t memid;
  bool allow_large = (!eager_delayed && (MI_SECURE == 0));
  size_t align_offset = 0;
  size_t alignment = MI_SEGMENT_SIZE;
  if (page_alignment > 0) {
    alignment = page_alignment;
    align_offset = _mi_align_up(pre_size, MI_SEGMENT_SIZE);
    segment_size = segment_size + (align_offset - pre_size);
  }
  mi_segment_t* segment = (mi_segment_t*)_mi_arena_alloc_aligned(segment_size, alignment, align_offset, commit, allow_large, req_arena_id, &memid, tld_os);
  if (segment == NULL) {
    return NULL;
  }
  if (!memid.initially_committed) {
    mi_assert_internal(!memid.is_pinned);
    bool ok = _mi_os_commit(segment, pre_size, NULL, tld_os->stats);
    if (!ok) {
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
static mi_segment_t* mi_segment_alloc(size_t required, mi_page_kind_t page_kind, size_t page_shift, size_t page_alignment,
                                      mi_arena_id_t req_arena_id, mi_segments_tld_t* tld, mi_os_tld_t* os_tld)
{
  mi_assert_internal((required > 0 && page_kind > MI_PAGE_LARGE)|| (required==0 && page_kind <= MI_PAGE_LARGE));
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
  const bool eager_delayed = (page_kind <= MI_PAGE_MEDIUM &&
                              _mi_current_thread_count() > 1 &&
                              tld->peak_count < (size_t)mi_option_get(mi_option_eager_commit_delay));
  const bool eager = !eager_delayed && mi_option_is_enabled(mi_option_eager_commit);
  const bool init_commit = eager;
  mi_segment_t* segment = mi_segment_os_alloc(eager_delayed, page_alignment, req_arena_id, pre_size, info_size, init_commit, init_segment_size, tld, os_tld);
  if (segment == NULL) return NULL;
  mi_assert_internal(segment != NULL && (uintptr_t)segment % MI_SEGMENT_SIZE == 0);
  mi_assert_internal(segment->memid.is_pinned ? segment->memid.initially_committed : true);
  ptrdiff_t ofs = offsetof(mi_segment_t, next);
  _mi_memzero((uint8_t*)segment + ofs, info_size - ofs);
  const bool is_huge = (page_kind == MI_PAGE_HUGE);
  for (size_t i = 0; i < capacity; i++) {
    mi_assert_internal(i <= 255);
    segment->pages[i].segment_idx = (uint8_t)i;
    segment->pages[i].is_committed = segment->memid.initially_committed;
    segment->pages[i].is_zero_init = segment->memid.initially_zero;
    segment->pages[i].is_huge = is_huge;
  }
  segment->page_kind = page_kind;
  segment->capacity = capacity;
  segment->page_shift = page_shift;
  segment->segment_info_size = pre_size;
  segment->thread_id = _mi_thread_id();
  segment->cookie = _mi_ptr_cookie(segment);
  mi_segment_protect(segment, true, tld->os);
  if (page_kind <= MI_PAGE_MEDIUM) {
    mi_segment_insert_in_free_queue(segment, tld);
  }
  return segment;
}
static void mi_segment_free(mi_segment_t* segment, bool force, mi_segments_tld_t* tld) {
  MI_UNUSED(force);
  mi_assert(segment != NULL);
  if (segment->dont_free) return;
  mi_segment_remove_all_purges(segment, false , tld);
  mi_segment_remove_from_free_queue(segment, tld);
  mi_assert_expensive(!mi_segment_queue_contains(&tld->small_free, segment));
  mi_assert_expensive(!mi_segment_queue_contains(&tld->medium_free, segment));
  mi_assert(segment->next == NULL);
  mi_assert(segment->prev == NULL);
  _mi_stat_decrease(&tld->stats->page_committed, segment->segment_info_size);
  mi_segment_os_free(segment, segment->segment_size, tld);
}
static bool mi_segment_has_free(const mi_segment_t* segment) {
  return (segment->used < segment->capacity);
}
static bool mi_segment_page_claim(mi_segment_t* segment, mi_page_t* page, mi_segments_tld_t* tld) {
  mi_assert_internal(_mi_page_segment(page) == segment);
  mi_assert_internal(!page->segment_in_use);
  mi_page_purge_remove(page, tld);
  if (!mi_page_ensure_committed(segment, page, tld)) return false;
  page->segment_in_use = true;
  segment->used++;
  mi_assert_internal(page->segment_in_use && page->is_committed && page->used==0 && !mi_pages_purge_contains(page,tld));
  mi_assert_internal(segment->used <= segment->capacity);
  if (segment->used == segment->capacity && segment->page_kind <= MI_PAGE_MEDIUM) {
    mi_assert_internal(!mi_segment_has_free(segment));
    mi_segment_remove_from_free_queue(segment, tld);
  }
  return true;
}
static void mi_segment_abandon(mi_segment_t* segment, mi_segments_tld_t* tld);
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
  mi_segment_schedule_purge(segment, page, tld);
  page->capacity = 0;
  page->reserved = 0;
}
void _mi_segment_page_free(mi_page_t* page, bool force, mi_segments_tld_t* tld)
{
  mi_assert(page != NULL);
  mi_segment_t* segment = _mi_page_segment(page);
  mi_assert_expensive(mi_segment_is_valid(segment,tld));
  mi_pages_try_purge(false , tld);
  mi_segment_page_clear(segment, page, tld);
  if (segment->used == 0) {
    mi_segment_free(segment, force, tld);
  }
  else {
    if (segment->used == segment->abandoned) {
      mi_segment_abandon(segment,tld);
    }
    else if (segment->used + 1 == segment->capacity) {
      mi_assert_internal(segment->page_kind <= MI_PAGE_MEDIUM);
      if (segment->page_kind <= MI_PAGE_MEDIUM) {
        mi_segment_insert_in_free_queue(segment,tld);
      }
    }
  }
}
static void mi_segment_abandon(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_assert_internal(segment->used == segment->abandoned);
  mi_assert_internal(segment->used > 0);
  mi_assert_expensive(mi_segment_is_valid(segment, tld));
  mi_pages_try_purge(false ,tld);
  const bool force_purge = (segment->memid.memkind != MI_MEM_ARENA) || mi_option_is_enabled(mi_option_abandoned_page_purge);
  mi_segment_remove_all_purges(segment, force_purge, tld);
  mi_segment_remove_from_free_queue(segment, tld);
  mi_assert_internal(segment->next == NULL && segment->prev == NULL);
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
    mi_segment_abandon(segment, tld);
  }
}
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
      _mi_page_free_collect(page, false);
      if (mi_page_all_free(page)) {
        pages_used_empty++;
        has_page = true;
      }
      else if (mi_page_block_size(page) == block_size && mi_page_has_any_available(page)) {
        has_page = true;
      }
    }
    else {
      has_page = true;
    }
  }
  mi_assert_internal(pages_used == segment->used && pages_used >= pages_used_empty);
  if (all_pages_free != NULL) {
    *all_pages_free = ((pages_used - pages_used_empty) == 0);
  }
  return has_page;
}
static mi_segment_t* mi_segment_reclaim(mi_segment_t* segment, mi_heap_t* heap, size_t requested_block_size, bool* right_page_reclaimed, mi_segments_tld_t* tld) {
  if (right_page_reclaimed != NULL) { *right_page_reclaimed = false; }
  mi_assert_internal(mi_atomic_load_relaxed(&segment->thread_id) == 0 || mi_atomic_load_relaxed(&segment->thread_id) == _mi_thread_id());
  mi_assert_internal(segment->subproc == heap->tld->segments.subproc);
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
      mi_heap_t* target_heap = _mi_heap_by_tag(heap, page->heap_tag);
      if (target_heap == NULL) {
        target_heap = heap;
        _mi_error_message(EFAULT, "page with tag %u cannot be reclaimed by a heap with the same tag (using heap tag %u instead)\n", page->heap_tag, heap->tag );
      }
      mi_page_set_heap(page, target_heap);
      _mi_page_use_delayed_free(page, MI_USE_DELAYED_FREE, true);
      _mi_page_free_collect(page, false);
      if (mi_page_all_free(page)) {
        mi_segment_page_clear(segment, page, tld);
      }
      else {
        _mi_page_reclaim(target_heap, page);
        if (requested_block_size == mi_page_block_size(page) && mi_page_has_any_available(page) && heap == target_heap) {
          if (right_page_reclaimed != NULL) { *right_page_reclaimed = true; }
        }
      }
    }
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
bool _mi_segment_attempt_reclaim(mi_heap_t* heap, mi_segment_t* segment) {
  if (mi_atomic_load_relaxed(&segment->thread_id) != 0) return false;
  if (segment->subproc != heap->tld->segments.subproc) return false;
  if (!_mi_heap_memid_is_suitable(heap,segment->memid)) return false;
  const long target = _mi_option_get_fast(mi_option_target_segments_per_thread);
  if (target > 0 && (size_t)target <= heap->tld->segments.count) return false;
  if (segment->memid.memkind == MI_MEM_ARENA && heap->tld->segments.reclaim_count * 2 > heap->tld->segments.count) {
    return false;
  }
  if (_mi_arena_segment_clear_abandoned(segment)) {
    mi_segment_t* res = mi_segment_reclaim(segment, heap, 0, NULL, &heap->tld->segments);
    mi_assert_internal(res == segment);
    return (res != NULL);
  }
  return false;
}
void _mi_abandoned_reclaim_all(mi_heap_t* heap, mi_segments_tld_t* tld) {
  mi_segment_t* segment;
  mi_arena_field_cursor_t current;
  _mi_arena_field_cursor_init(heap, tld->subproc, true , &current);
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
  const size_t perc = (size_t)mi_option_get_clamp(mi_option_max_segment_reclaim, 0, 100);
  if (perc <= 0) return 0;
  const size_t total_count = mi_atomic_load_relaxed(&tld->subproc->abandoned_count);
  if (total_count == 0) return 0;
  const size_t relative_count = (total_count > 10000 ? (total_count / 100) * perc : (total_count * perc) / 100);
  long max_tries = (long)(relative_count <= 1 ? 1 : (relative_count > 1024 ? 1024 : relative_count));
  if (max_tries < 8 && total_count > 8) { max_tries = 8; }
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
  _mi_arena_field_cursor_init(heap, tld->subproc, false , &current);
  while (segment_count_is_within_target(tld,NULL) && (max_tries-- > 0) && ((segment = _mi_arena_segment_clear_abandoned_next(&current)) != NULL))
  {
    mi_assert(segment->subproc == heap->tld->segments.subproc);
    segment->abandoned_visits++;
    bool is_suitable = _mi_heap_memid_is_suitable(heap, segment->memid);
    bool all_pages_free;
    bool has_page = mi_segment_check_free(segment,block_size,&all_pages_free);
    if (all_pages_free) {
      mi_segment_reclaim(segment, heap, 0, NULL, tld);
    }
    else if (has_page && segment->page_kind == page_kind && is_suitable) {
      result = mi_segment_reclaim(segment, heap, block_size, reclaimed, tld);
      break;
    }
    else if (segment->abandoned_visits > 3 && is_suitable) {
      mi_segment_reclaim(segment, heap, 0, NULL, tld);
    }
    else {
      _mi_arena_segment_mark_abandoned(segment);
    }
  }
  _mi_arena_field_cursor_done(&current);
  return result;
}
static void mi_segment_force_abandon(mi_segment_t* segment, mi_segments_tld_t* tld)
{
  mi_assert_internal(segment->abandoned < segment->used);
  mi_assert_internal(!segment->dont_free);
  segment->dont_free = true;
  for (size_t i = 0; i < segment->capacity; i++) {
    mi_page_t* page = &segment->pages[i];
    if (page->segment_in_use) {
      mi_assert_internal(segment->used > 0);
      if (segment->used == segment->abandoned+1) {
        segment->dont_free = false;
        _mi_page_force_abandon(page);
        return;
      }
      else {
        _mi_page_force_abandon(page);
      }
    }
  }
  segment->dont_free = false;
  mi_assert(segment->used == segment->abandoned);
  mi_assert(segment->used == 0);
  if (segment->used == 0) {
    mi_segment_free(segment, false, tld);
  }
  else {
    mi_pages_try_purge(false , tld);
  }
}
static void mi_segments_try_abandon_to_target(mi_heap_t* heap, size_t target, mi_segments_tld_t* tld) {
  if (target <= 1) return;
  const size_t min_target = (target > 4 ? (target*3)/4 : target);
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
    mi_assert_internal(page != heap->pages[MI_BIN_FULL].first);
  }
}
static void mi_segments_try_abandon(mi_heap_t* heap, mi_segments_tld_t* tld) {
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
static mi_segment_t* mi_segment_reclaim_or_alloc(mi_heap_t* heap, size_t block_size, mi_page_kind_t page_kind, size_t page_shift, mi_segments_tld_t* tld, mi_os_tld_t* os_tld)
{
  mi_assert_internal(page_kind <= MI_PAGE_LARGE);
  mi_assert_internal(block_size <= MI_LARGE_OBJ_SIZE_MAX);
  mi_segments_try_abandon(heap,tld);
  bool reclaimed;
  mi_segment_t* segment = mi_segment_try_reclaim(heap, block_size, page_kind, &reclaimed, tld);
  mi_assert_internal(segment == NULL || _mi_arena_memid_is_suitable(segment->memid, heap->arena_id));
  if (reclaimed) {
    mi_assert_internal(segment != NULL && segment->page_kind == page_kind && page_kind <= MI_PAGE_LARGE);
    return NULL;
  }
  else if (segment != NULL) {
    return segment;
  }
  return mi_segment_alloc(0, page_kind, page_shift, 0, heap->arena_id, tld, os_tld);
}
static mi_page_t* mi_segment_find_free(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_assert_internal(mi_segment_has_free(segment));
  mi_assert_expensive(mi_segment_is_valid(segment, tld));
  for (size_t i = 0; i < segment->capacity; i++) {
    mi_page_t* page = &segment->pages[i];
    if (!page->segment_in_use) {
      bool ok = mi_segment_page_claim(segment, page, tld);
      if (ok) return page;
    }
  }
  mi_assert(false);
  return NULL;
}
static mi_page_t* mi_segment_page_alloc_in(mi_segment_t* segment, mi_segments_tld_t* tld) {
  mi_assert_internal(mi_segment_has_free(segment));
  return mi_segment_find_free(segment, tld);
}
static mi_page_t* mi_segment_page_try_alloc_in_queue(mi_heap_t* heap, mi_page_kind_t kind, mi_segments_tld_t* tld) {
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
    mi_segment_t* const segment = mi_segment_reclaim_or_alloc(heap, block_size, kind, page_shift, tld, os_tld);
    if (segment == NULL) return NULL;
    mi_assert_internal(segment->page_kind==kind);
    mi_assert_internal(segment->used < segment->capacity);
    mi_assert_internal(_mi_arena_memid_is_suitable(segment->memid, heap->arena_id));
    page = mi_segment_page_try_alloc_in_queue(heap, kind, tld);
  }
  mi_assert_internal(page != NULL);
  #if MI_DEBUG>=2 && !MI_TRACK_ENABLED
  mi_segment_raw_page_start(_mi_page_segment(page), page, NULL)[0] = 0;
  #endif
  return page;
}
static mi_page_t* mi_segment_small_page_alloc(mi_heap_t* heap, size_t block_size, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  return mi_segment_page_alloc(heap, block_size, MI_PAGE_SMALL,MI_SMALL_PAGE_SHIFT,tld,os_tld);
}
static mi_page_t* mi_segment_medium_page_alloc(mi_heap_t* heap, size_t block_size, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  return mi_segment_page_alloc(heap, block_size, MI_PAGE_MEDIUM, MI_MEDIUM_PAGE_SHIFT, tld, os_tld);
}
static mi_page_t* mi_segment_large_page_alloc(mi_heap_t* heap, size_t block_size, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  mi_segment_t* segment = mi_segment_reclaim_or_alloc(heap,block_size,MI_PAGE_LARGE,MI_LARGE_PAGE_SHIFT,tld,os_tld);
  if (segment == NULL) return NULL;
  mi_page_t* page = mi_segment_find_free(segment, tld);
  mi_assert_internal(page != NULL);
#if MI_DEBUG>=2 && !MI_TRACK_ENABLED
  mi_segment_raw_page_start(segment, page, NULL)[0] = 0;
#endif
  return page;
}
static mi_page_t* mi_segment_huge_page_alloc(size_t size, size_t page_alignment, mi_arena_id_t req_arena_id, mi_segments_tld_t* tld, mi_os_tld_t* os_tld)
{
  mi_segment_t* segment = mi_segment_alloc(size, MI_PAGE_HUGE, MI_SEGMENT_SHIFT + 1, page_alignment, req_arena_id, tld, os_tld);
  if (segment == NULL) return NULL;
  mi_assert_internal(mi_segment_page_size(segment) - segment->segment_info_size - (2*(MI_SECURE == 0 ? 0 : _mi_os_page_size())) >= size);
  #if MI_HUGE_PAGE_ABANDON
  segment->thread_id = 0;
  mi_segments_track_size(-(long)segment->segment_size, tld);
  #endif
  mi_page_t* page = mi_segment_find_free(segment, tld);
  mi_assert_internal(page != NULL);
  mi_assert_internal(page->is_huge);
  size_t psize;
  uint8_t* start = mi_segment_raw_page_start(segment, page, &psize);
  page->block_size = psize;
  if (page_alignment > 0 && segment->allow_decommit && page->is_committed) {
    uint8_t* aligned_p = (uint8_t*)_mi_align_up((uintptr_t)start, page_alignment);
    mi_assert_internal(_mi_is_aligned(aligned_p, page_alignment));
    mi_assert_internal(psize - (aligned_p - start) >= size);
    uint8_t* decommit_start = start + sizeof(mi_block_t);
    ptrdiff_t decommit_size = aligned_p - decommit_start;
    _mi_os_reset(decommit_start, decommit_size, os_tld->stats);
  }
  return page;
}
#if MI_HUGE_PAGE_ABANDON
void _mi_segment_huge_page_free(mi_segment_t* segment, mi_page_t* page, mi_block_t* block) {
  mi_assert_internal(segment->page_kind==MI_PAGE_HUGE);
  mi_assert_internal(segment == _mi_page_segment(page));
  mi_assert_internal(mi_atomic_load_relaxed(&segment->thread_id)==0);
  mi_heap_t* heap = mi_heap_get_default();
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
#endif
}
#else
void _mi_segment_huge_page_reset(mi_segment_t* segment, mi_page_t* page, mi_block_t* block) {
  mi_assert_internal(segment->page_kind == MI_PAGE_HUGE);
  mi_assert_internal(segment == _mi_page_segment(page));
  mi_assert_internal(page->used == 1);
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
#endif
mi_page_t* _mi_segment_page_alloc(mi_heap_t* heap, size_t block_size, size_t page_alignment, mi_segments_tld_t* tld, mi_os_tld_t* os_tld) {
  mi_page_t* page;
  if mi_unlikely(page_alignment > MI_BLOCK_ALIGNMENT_MAX) {
    mi_assert_internal(_mi_is_power_of_two(page_alignment));
    mi_assert_internal(page_alignment >= MI_SEGMENT_SIZE);
    if (page_alignment < MI_SEGMENT_SIZE) { page_alignment = MI_SEGMENT_SIZE; }
    page = mi_segment_huge_page_alloc(block_size, page_alignment, heap->arena_id, tld, os_tld);
  }
  else if (block_size <= MI_SMALL_OBJ_SIZE_MAX) {
    page = mi_segment_small_page_alloc(heap, block_size, tld, os_tld);
  }
  else if (block_size <= MI_MEDIUM_OBJ_SIZE_MAX) {
    page = mi_segment_medium_page_alloc(heap, block_size, tld, os_tld);
  }
  else if (block_size <= MI_LARGE_OBJ_SIZE_MAX ) {
    page = mi_segment_large_page_alloc(heap, block_size, tld, os_tld);
  }
  else {
    page = mi_segment_huge_page_alloc(block_size, page_alignment, heap->arena_id, tld, os_tld);
  }
  mi_assert_expensive(page == NULL || mi_segment_is_valid(_mi_page_segment(page),tld));
  mi_assert_internal(page == NULL || (mi_segment_page_size(_mi_page_segment(page)) - (MI_SECURE == 0 ? 0 : _mi_os_page_size())) >= block_size);
  mi_assert_internal(page == NULL || mi_page_not_in_queue(page, tld));
  mi_assert_internal(page == NULL || _mi_page_segment(page)->subproc == tld->subproc);
  return page;
}
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
#if (MI_INTPTR_SIZE > 4) && MI_TRACK_ASAN
#define MI_SEGMENT_MAP_MAX_ADDRESS (128*1024ULL*MI_GiB)
#elif (MI_INTPTR_SIZE > 4)
#define MI_SEGMENT_MAP_MAX_ADDRESS (48*1024ULL*MI_GiB)
#else
#define MI_SEGMENT_MAP_MAX_ADDRESS (UINT32_MAX)
#endif
#define MI_SEGMENT_MAP_PART_SIZE (MI_INTPTR_SIZE*MI_KiB - 128)
#define MI_SEGMENT_MAP_PART_BITS (8*MI_SEGMENT_MAP_PART_SIZE)
#define MI_SEGMENT_MAP_PART_ENTRIES (MI_SEGMENT_MAP_PART_SIZE / MI_INTPTR_SIZE)
#define MI_SEGMENT_MAP_PART_BIT_SPAN (MI_SEGMENT_ALIGN)
#define MI_SEGMENT_MAP_PART_SPAN (MI_SEGMENT_MAP_PART_BITS * MI_SEGMENT_MAP_PART_BIT_SPAN)
#define MI_SEGMENT_MAP_MAX_PARTS ((MI_SEGMENT_MAP_MAX_ADDRESS / MI_SEGMENT_MAP_PART_SPAN) + 1)
typedef struct mi_segmap_part_s {
  mi_memid_t memid;
  _Atomic(uintptr_t) map[MI_SEGMENT_MAP_PART_ENTRIES];
} mi_segmap_part_t;
static _Atomic(mi_segmap_part_t*) mi_segment_map[MI_SEGMENT_MAP_MAX_PARTS];
static mi_segmap_part_t* mi_segment_map_index_of(const mi_segment_t* segment, bool create_on_demand, size_t* idx, size_t* bitidx) {
  mi_assert_internal(_mi_ptr_segment(segment + 1) == segment);
  *idx = 0;
  *bitidx = 0;
  if ((uintptr_t)segment >= MI_SEGMENT_MAP_MAX_ADDRESS) return NULL;
  const uintptr_t segindex = ((uintptr_t)segment) / MI_SEGMENT_MAP_PART_SPAN;
  if (segindex >= MI_SEGMENT_MAP_MAX_PARTS) return NULL;
  mi_segmap_part_t* part = mi_atomic_load_ptr_relaxed(mi_segmap_part_t, &mi_segment_map[segindex]);
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
  if (segment->memid.memkind == MI_MEM_ARENA) return;
  size_t index;
  size_t bitidx;
  mi_segmap_part_t* part = mi_segment_map_index_of(segment, true , &index, &bitidx);
  if (part == NULL) return;
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
  mi_segmap_part_t* part = mi_segment_map_index_of(segment, false , &index, &bitidx);
  if (part == NULL) return;
  uintptr_t mask = mi_atomic_load_relaxed(&part->map[index]);
  uintptr_t newmask;
  do {
    newmask = (mask & ~((uintptr_t)1 << bitidx));
  } while (!mi_atomic_cas_weak_release(&part->map[index], &mask, newmask));
}
static mi_segment_t* _mi_segment_of(const void* p) {
  if (p == NULL) return NULL;
  mi_segment_t* segment = _mi_ptr_segment(p);
  size_t index;
  size_t bitidx;
  mi_segmap_part_t* part = mi_segment_map_index_of(segment, false , &index, &bitidx);
  if (part == NULL) return NULL;
  const uintptr_t mask = mi_atomic_load_relaxed(&part->map[index]);
  if mi_likely((mask & ((uintptr_t)1 << bitidx)) != 0) {
    bool cookie_ok = (_mi_ptr_cookie(segment) == segment->cookie);
    mi_assert_internal(cookie_ok); MI_UNUSED(cookie_ok);
    return segment;
  }
  return NULL;
}
static bool mi_is_valid_pointer(const void* p) {
  return (_mi_arena_contains(p) || _mi_segment_of(p) != NULL);
}
mi_decl_nodiscard mi_decl_export bool mi_is_in_heap_region(const void* p) mi_attr_noexcept {
  return mi_is_valid_pointer(p);
}
#include <string.h>
#if defined(_MSC_VER) && (_MSC_VER < 1920)
#pragma warning(disable:4204)
#endif
static bool mi_is_in_main(void* stat) {
  return ((uint8_t*)stat >= (uint8_t*)&_mi_stats_main
         && (uint8_t*)stat < ((uint8_t*)&_mi_stats_main + sizeof(mi_stats_t)));
}
static void mi_stat_update(mi_stat_count_t* stat, int64_t amount) {
  if (amount == 0) return;
  if (mi_is_in_main(stat))
  {
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
static void mi_stat_add(mi_stat_count_t* stat, const mi_stat_count_t* src, int64_t unit) {
  if (stat==src) return;
  if (src->allocated==0 && src->freed==0) return;
  mi_atomic_addi64_relaxed( &stat->allocated, src->allocated * unit);
  mi_atomic_addi64_relaxed( &stat->current, src->current * unit);
  mi_atomic_addi64_relaxed( &stat->freed, src->freed * unit);
  mi_atomic_addi64_relaxed( &stat->peak, src->peak * unit);
}
static void mi_stat_counter_add(mi_stat_counter_t* stat, const mi_stat_counter_t* src, int64_t unit) {
  if (stat==src) return;
  mi_atomic_addi64_relaxed( &stat->total, src->total * unit);
  mi_atomic_addi64_relaxed( &stat->count, src->count * unit);
}
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
#endif
}
static void mi_printf_amount(int64_t n, int64_t unit, mi_output_fun* out, void* arg, const char* fmt) {
  char buf[32]; buf[0] = 0;
  int len = 32;
  const char* suffix = (unit <= 0 ? " " : "B");
  const int64_t base = (unit == 0 ? 1000 : 1024);
  if (unit>0) n *= unit;
  const int64_t pos = (n < 0 ? -n : n);
  if (pos < base) {
    if (n!=1 || suffix[0] != 'B') {
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
    _mi_fprintf(out, arg, "%11s", " ");
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
#endif
typedef struct buffered_s {
  mi_output_fun* out;
  void* arg;
  char* buf;
  size_t used;
  size_t count;
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
static void _mi_stats_print(mi_stats_t* stats, mi_output_fun* out0, void* arg0) mi_attr_noexcept {
  char buf[256];
  buffered_t buffer = { out0, arg0, NULL, 0, 255 };
  buffer.buf = buf;
  mi_output_fun* out = &mi_buffered_out;
  void* arg = &buffer;
  mi_print_header(out,arg);
  #if MI_STAT>1
  mi_stats_print_bins(stats->normal_bins, MI_BIN_HUGE, "normal",out,arg);
  #endif
  #if MI_STAT
  mi_stat_print(&stats->normal, "normal", (stats->normal_count.count == 0 ? 1 : -(stats->normal.allocated / stats->normal_count.count)), out, arg);
  mi_stat_print(&stats->huge, "huge", (stats->huge_count.count == 0 ? 1 : -(stats->huge.allocated / stats->huge_count.count)), out, arg);
  mi_stat_count_t total = { 0,0,0,0 };
  mi_stat_add(&total, &stats->normal, 1);
  mi_stat_add(&total, &stats->huge, 1);
  mi_stat_print(&total, "total", 1, out, arg);
  #endif
  #if MI_STAT>1
  mi_stat_print(&stats->malloc, "malloc req", 1, out, arg);
  _mi_fprintf(out, arg, "\n");
  #endif
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
static mi_msecs_t mi_process_start;
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
void _mi_stats_done(mi_stats_t* stats) {
  mi_stats_merge_from(stats);
}
void mi_stats_print_out(mi_output_fun* out, void* arg) mi_attr_noexcept {
  mi_stats_merge_from(mi_stats_get_default());
  _mi_stats_print(&_mi_stats_main, out, arg);
}
void mi_stats_print(void* out) mi_attr_noexcept {
  mi_stats_print_out((mi_output_fun*)out, NULL);
}
void mi_thread_stats_print_out(mi_output_fun* out, void* arg) mi_attr_noexcept {
  _mi_stats_print(mi_stats_get_default(), out, arg);
}
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
mi_decl_export void mi_process_info(size_t* elapsed_msecs, size_t* user_msecs, size_t* system_msecs, size_t* current_rss, size_t* peak_rss, size_t* current_commit, size_t* peak_commit, size_t* page_faults) mi_attr_noexcept
{
  mi_process_info_t pinfo;
  _mi_memzero_var(pinfo);
  pinfo.elapsed = _mi_clock_end(mi_process_start);
  pinfo.current_commit = (size_t)(mi_atomic_loadi64_relaxed((_Atomic(int64_t)*)&_mi_stats_main.committed.current));
  pinfo.peak_commit = (size_t)(mi_atomic_loadi64_relaxed((_Atomic(int64_t)*)&_mi_stats_main.committed.peak));
  pinfo.current_rss = pinfo.current_commit;
  pinfo.peak_rss = pinfo.peak_commit;
  pinfo.utime = 0;
  pinfo.stime = 0;
  pinfo.page_faults = 0;
  _mi_prim_process_info(&pinfo);
  if (elapsed_msecs!=NULL) *elapsed_msecs = (pinfo.elapsed < 0 ? 0 : (pinfo.elapsed < (mi_msecs_t)PTRDIFF_MAX ? (size_t)pinfo.elapsed : PTRDIFF_MAX));
  if (user_msecs!=NULL) *user_msecs = (pinfo.utime < 0 ? 0 : (pinfo.utime < (mi_msecs_t)PTRDIFF_MAX ? (size_t)pinfo.utime : PTRDIFF_MAX));
  if (system_msecs!=NULL) *system_msecs = (pinfo.stime < 0 ? 0 : (pinfo.stime < (mi_msecs_t)PTRDIFF_MAX ? (size_t)pinfo.stime : PTRDIFF_MAX));
  if (current_rss!=NULL) *current_rss = pinfo.current_rss;
  if (peak_rss!=NULL) *peak_rss = pinfo.peak_rss;
  if (current_commit!=NULL) *current_commit = pinfo.current_commit;
  if (peak_commit!=NULL) *peak_commit = pinfo.peak_commit;
  if (page_faults!=NULL) *page_faults = pinfo.page_faults;
}
#if defined(_WIN32)
#include <stdio.h>
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
  union { DWORD64 ULong64; PVOID Pointer; SIZE_T Size; HANDLE Handle; DWORD ULong; } Arg;
} MI_MEM_EXTENDED_PARAMETER;
typedef struct MI_MEM_ADDRESS_REQUIREMENTS_S {
  PVOID LowestStartingAddress;
  PVOID HighestEndingAddress;
  SIZE_T Alignment;
} MI_MEM_ADDRESS_REQUIREMENTS;
#define MI_MEM_EXTENDED_PARAMETER_NONPAGED_HUGE 0x00000010
#include <winternl.h>
typedef PVOID (__stdcall *PVirtualAlloc2)(HANDLE, PVOID, SIZE_T, ULONG, ULONG, MI_MEM_EXTENDED_PARAMETER*, ULONG);
typedef NTSTATUS (__stdcall *PNtAllocateVirtualMemoryEx)(HANDLE, PVOID*, SIZE_T*, ULONG, ULONG, MI_MEM_EXTENDED_PARAMETER*, ULONG);
static PVirtualAlloc2 pVirtualAlloc2 = NULL;
static PNtAllocateVirtualMemoryEx pNtAllocateVirtualMemoryEx = NULL;
typedef struct MI_PROCESSOR_NUMBER_S { WORD Group; BYTE Number; BYTE Reserved; } MI_PROCESSOR_NUMBER;
typedef VOID (__stdcall *PGetCurrentProcessorNumberEx)(MI_PROCESSOR_NUMBER* ProcNumber);
typedef BOOL (__stdcall *PGetNumaProcessorNodeEx)(MI_PROCESSOR_NUMBER* Processor, PUSHORT NodeNumber);
typedef BOOL (__stdcall* PGetNumaNodeProcessorMaskEx)(USHORT Node, PGROUP_AFFINITY ProcessorMask);
typedef BOOL (__stdcall *PGetNumaProcessorNode)(UCHAR Processor, PUCHAR NodeNumber);
static PGetCurrentProcessorNumberEx pGetCurrentProcessorNumberEx = NULL;
static PGetNumaProcessorNodeEx pGetNumaProcessorNodeEx = NULL;
static PGetNumaNodeProcessorMaskEx pGetNumaNodeProcessorMaskEx = NULL;
static PGetNumaProcessorNode pGetNumaProcessorNode = NULL;
static bool win_enable_large_os_pages(size_t* large_page_size)
{
  static bool large_initialized = false;
  if (large_initialized) return (_mi_os_large_page_size() > 0);
  large_initialized = true;
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
void _mi_prim_mem_init( mi_os_mem_config_t* config )
{
  config->has_overcommit = false;
  config->has_partial_free = false;
  config->has_virtual_reserve = true;
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  if (si.dwPageSize > 0) { config->page_size = si.dwPageSize; }
  if (si.dwAllocationGranularity > 0) { config->alloc_granularity = si.dwAllocationGranularity; }
  if ((uintptr_t)si.lpMaximumApplicationAddress > 0) {
    const size_t vbits = MI_INTPTR_BITS - mi_clz((uintptr_t)si.lpMaximumApplicationAddress);
    config->virtual_address_bits = vbits;
  }
  ULONGLONG memInKiB = 0;
  if (GetPhysicallyInstalledSystemMemory(&memInKiB)) {
    if (memInKiB > 0 && memInKiB < (SIZE_MAX / MI_KiB)) {
      config->physical_memory = memInKiB * MI_KiB;
    }
  }
  HINSTANCE hDll;
  hDll = LoadLibrary(TEXT("kernelbase.dll"));
  if (hDll != NULL) {
    pVirtualAlloc2 = (PVirtualAlloc2)(void (*)(void))GetProcAddress(hDll, "VirtualAlloc2FromApp");
    if (pVirtualAlloc2==NULL) pVirtualAlloc2 = (PVirtualAlloc2)(void (*)(void))GetProcAddress(hDll, "VirtualAlloc2");
    FreeLibrary(hDll);
  }
  hDll = LoadLibrary(TEXT("ntdll.dll"));
  if (hDll != NULL) {
    pNtAllocateVirtualMemoryEx = (PNtAllocateVirtualMemoryEx)(void (*)(void))GetProcAddress(hDll, "NtAllocateVirtualMemoryEx");
    FreeLibrary(hDll);
  }
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
int _mi_prim_free(void* addr, size_t size ) {
  MI_UNUSED(size);
  DWORD errcode = 0;
  bool err = (VirtualFree(addr, 0, MEM_RELEASE) == 0);
  if (err) { errcode = GetLastError(); }
  if (errcode == ERROR_INVALID_ADDRESS) {
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
static void* win_virtual_alloc_prim_once(void* addr, size_t size, size_t try_alignment, DWORD flags) {
  #if (MI_INTPTR_SIZE >= 8)
  if (addr == NULL) {
    void* hint = _mi_os_get_aligned_hint(try_alignment,size);
    if (hint != NULL) {
      void* p = VirtualAlloc(hint, size, flags, PAGE_READWRITE);
      if (p != NULL) return p;
      _mi_verbose_message("warning: unable to allocate hinted aligned OS memory (%zu bytes, error code: 0x%x, address: %p, alignment: %zu, flags: 0x%x)\n", size, GetLastError(), hint, try_alignment, flags);
    }
  }
  #endif
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0 && pVirtualAlloc2 != NULL) {
    MI_MEM_ADDRESS_REQUIREMENTS reqs = { 0, 0, 0 };
    reqs.Alignment = try_alignment;
    MI_MEM_EXTENDED_PARAMETER param = { {0, 0}, {0} };
    param.Type.Type = MiMemExtendedParameterAddressRequirements;
    param.Arg.Pointer = &reqs;
    void* p = (*pVirtualAlloc2)(GetCurrentProcess(), addr, size, flags, PAGE_READWRITE, &param, 1);
    if (p != NULL) return p;
    _mi_warning_message("unable to allocate aligned OS memory (0x%zx bytes, error code: 0x%x, address: %p, alignment: 0x%zx, flags: 0x%x)\n", size, GetLastError(), addr, try_alignment, flags);
  }
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
  long max_retry_msecs = mi_option_get_clamp(mi_option_retry_on_oom, 0, 2000);
  if (max_retry_msecs == 1) { max_retry_msecs = 100; }
  for (long tries = 1; tries <= 10; tries++) {
    void* p = win_virtual_alloc_prim_once(addr, size, try_alignment, flags);
    if (p != NULL) {
      return p;
    }
    else if (max_retry_msecs > 0 && (try_alignment <= 2*MI_SEGMENT_ALIGN) &&
              (flags&MEM_COMMIT) != 0 && (flags&MEM_LARGE_PAGES) == 0 &&
              win_is_out_of_memory_error(GetLastError())) {
      _mi_warning_message("out-of-memory on OS allocation, try again... (attempt %lu, 0x%zx bytes, error code: 0x%x, address: %p, alignment: 0x%zx, flags: 0x%x)\n", tries, size, GetLastError(), addr, try_alignment, flags);
      long sleep_msecs = tries*40;
      if (sleep_msecs > max_retry_msecs) { sleep_msecs = max_retry_msecs; }
      max_retry_msecs -= sleep_msecs;
      Sleep(sleep_msecs);
    }
    else {
      break;
    }
  }
  return NULL;
}
static void* win_virtual_alloc(void* addr, size_t size, size_t try_alignment, DWORD flags, bool large_only, bool allow_large, bool* is_large) {
  mi_assert_internal(!(large_only && !allow_large));
  static _Atomic(size_t) large_page_try_ok;
  void* p = NULL;
  if ((large_only || _mi_os_use_large_page(size, try_alignment))
      && allow_large && (flags&MEM_COMMIT)!=0 && (flags&MEM_RESERVE)!=0) {
    size_t try_ok = mi_atomic_load_acquire(&large_page_try_ok);
    if (!large_only && try_ok > 0) {
      mi_atomic_cas_strong_acq_rel(&large_page_try_ok, &try_ok, try_ok - 1);
    }
    else {
      *is_large = true;
      p = win_virtual_alloc_prim(addr, size, try_alignment, flags | MEM_LARGE_PAGES);
      if (large_only) return p;
      if (p == NULL) {
        mi_atomic_store_release(&large_page_try_ok,10UL);
      }
    }
  }
  if (p == NULL) {
    *is_large = ((flags&MEM_LARGE_PAGES) != 0);
    p = win_virtual_alloc_prim(addr, size, try_alignment, flags);
  }
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
#if 0
#endif
int _mi_prim_commit(void* addr, size_t size, bool* is_zero) {
  *is_zero = false;
  void* p = VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE);
  if (p == NULL) return (int)GetLastError();
  return 0;
}
int _mi_prim_decommit(void* addr, size_t size, bool* needs_recommit) {
  BOOL ok = VirtualFree(addr, size, MEM_DECOMMIT);
  *needs_recommit = true;
  return (ok ? 0 : (int)GetLastError());
}
int _mi_prim_reset(void* addr, size_t size) {
  void* p = VirtualAlloc(addr, size, MEM_RESET, PAGE_READWRITE);
  mi_assert_internal(p == addr);
  #if 0
  if (p != NULL) {
    VirtualUnlock(addr,size);
  }
  #endif
  return (p != NULL ? 0 : (int)GetLastError());
}
int _mi_prim_protect(void* addr, size_t size, bool protect) {
  DWORD oldprotect = 0;
  BOOL ok = VirtualProtect(addr, size, protect ? PAGE_NOACCESS : PAGE_READWRITE, &oldprotect);
  return (ok ? 0 : (int)GetLastError());
}
static void* _mi_prim_alloc_huge_os_pagesx(void* hint_addr, size_t size, int numa_node)
{
  const DWORD flags = MEM_LARGE_PAGES | MEM_COMMIT | MEM_RESERVE;
  win_enable_large_os_pages(NULL);
  MI_MEM_EXTENDED_PARAMETER params[3] = { {{0,0},{0}},{{0,0},{0}},{{0,0},{0}} };
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
      mi_huge_pages_available = false;
      _mi_warning_message("unable to allocate using huge (1GiB) pages, trying large (2MiB) pages instead (status 0x%lx)\n", err);
    }
  }
  if (pVirtualAlloc2 != NULL && numa_node >= 0) {
    params[0].Type.Type = MiMemExtendedParameterNumaNode;
    params[0].Arg.ULong = (unsigned)numa_node;
    return (*pVirtualAlloc2)(GetCurrentProcess(), hint_addr, size, flags, PAGE_READWRITE, params, 1);
  }
  return VirtualAlloc(hint_addr, size, flags, PAGE_READWRITE);
}
int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  *is_zero = true;
  *addr = _mi_prim_alloc_huge_os_pagesx(hint_addr,size,numa_node);
  return (*addr != NULL ? 0 : (int)GetLastError());
}
size_t _mi_prim_numa_node(void) {
  USHORT numa_node = 0;
  if (pGetCurrentProcessorNumberEx != NULL && pGetNumaProcessorNodeEx != NULL) {
    MI_PROCESSOR_NUMBER pnum;
    (*pGetCurrentProcessorNumberEx)(&pnum);
    USHORT nnode = 0;
    BOOL ok = (*pGetNumaProcessorNodeEx)(&pnum, &nnode);
    if (ok) { numa_node = nnode; }
  }
  else if (pGetNumaProcessorNode != NULL) {
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
  while(numa_max > 0) {
    if (pGetNumaNodeProcessorMaskEx != NULL) {
      GROUP_AFFINITY affinity;
      if ((*pGetNumaNodeProcessorMaskEx)((USHORT)numa_max, &affinity)) {
        if (affinity.Mask != 0) break;
      }
    }
    else {
      ULONGLONG mask;
      if (GetNumaNodeProcessorMask((UCHAR)numa_max, &mask)) {
        if (mask != 0) break;
      };
    }
    numa_max--;
  }
  return ((size_t)numa_max + 1);
}
static mi_msecs_t mi_to_msecs(LARGE_INTEGER t) {
  static LARGE_INTEGER mfreq;
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
#include <psapi.h>
static mi_msecs_t filetime_msecs(const FILETIME* ftime) {
  ULARGE_INTEGER i;
  i.LowPart = ftime->dwLowDateTime;
  i.HighPart = ftime->dwHighDateTime;
  mi_msecs_t msecs = (i.QuadPart / 10000);
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
  if (pGetProcessMemoryInfo == NULL) {
    HINSTANCE hDll = LoadLibrary(TEXT("psapi.dll"));
    if (hDll != NULL) {
      pGetProcessMemoryInfo = (PGetProcessMemoryInfo)(void (*)(void))GetProcAddress(hDll, "GetProcessMemoryInfo");
    }
  }
  PROCESS_MEMORY_COUNTERS info; _mi_memzero_var(info);
  if (pGetProcessMemoryInfo != NULL) {
    pGetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
  }
  pinfo->current_rss = (size_t)info.WorkingSetSize;
  pinfo->peak_rss = (size_t)info.PeakWorkingSetSize;
  pinfo->current_commit = (size_t)info.PagefileUsage;
  pinfo->peak_commit = (size_t)info.PeakPagefileUsage;
  pinfo->page_faults = (size_t)info.PageFaultCount;
}
void _mi_prim_out_stderr( const char* msg )
{
  if (!_mi_preloading()) {
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
        WriteFile(hcon, msg, (DWORD)len, &written, NULL);
      }
      else {
        fputs(msg, stderr);
      }
    }
  }
}
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  result[0] = 0;
  size_t len = GetEnvironmentVariableA(name, result, (DWORD)result_size);
  return (len > 0 && len < result_size);
}
#if defined(MI_USE_RTLGENRANDOM)
#pragma comment (lib,"advapi32.lib")
#define RtlGenRandom SystemFunction036
mi_decl_externc BOOLEAN NTAPI RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);
bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  return (RtlGenRandom(buf, (ULONG)buf_len) != 0);
}
#else
#ifndef BCRYPT_USE_SYSTEM_PREFERRED_RNG
#define BCRYPT_USE_SYSTEM_PREFERRED_RNG 0x00000002
#endif
typedef LONG (NTAPI *PBCryptGenRandom)(HANDLE, PUCHAR, ULONG, ULONG);
static PBCryptGenRandom pBCryptGenRandom = NULL;
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
#endif
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
  #define MI_PRIM_HAS_PROCESS_ATTACH 1
  __declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved) {
    mi_win_main((PVOID)inst,reason,reserved);
    return TRUE;
  }
  void _mi_prim_thread_init_auto_done(void) { }
  void _mi_prim_thread_done_auto_done(void) { }
  void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
    MI_UNUSED(heap);
  }
#elif !defined(MI_WIN_USE_FLS)
  #define MI_PRIM_HAS_PROCESS_ATTACH 1
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
#if 0
  #endif
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
  #endif
#if 0
  #endif
  void _mi_prim_thread_init_auto_done(void) { }
  void _mi_prim_thread_done_auto_done(void) { }
  void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
    MI_UNUSED(heap);
  }
#else
  #if defined(_MSC_VER)
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
    #endif
    #pragma data_seg(".CRT$XIU")
    mi_decl_externc mi_crt_callback_t _mi_tls_callback[] = { &mi_process_attach };
    #pragma data_seg()
  #endif
  #include <fibersapi.h>
  #if (_WIN32_WINNT < 0x600)
  WINBASEAPI DWORD WINAPI FlsAlloc( _In_opt_ PFLS_CALLBACK_FUNCTION lpCallback );
  WINBASEAPI PVOID WINAPI FlsGetValue( _In_ DWORD dwFlsIndex );
  WINBASEAPI BOOL WINAPI FlsSetValue( _In_ DWORD dwFlsIndex, _In_opt_ PVOID lpFlsData );
  WINBASEAPI BOOL WINAPI FlsFree(_In_ DWORD dwFlsIndex);
  #endif
  static DWORD mi_fls_key = (DWORD)(-1);
  static void NTAPI mi_fls_done(PVOID value) {
    mi_heap_t* heap = (mi_heap_t*)value;
    if (heap != NULL) {
      _mi_thread_done(heap);
      FlsSetValue(mi_fls_key, NULL);
    }
  }
  void _mi_prim_thread_init_auto_done(void) {
    mi_fls_key = FlsAlloc(&mi_fls_done);
  }
  void _mi_prim_thread_done_auto_done(void) {
    FlsFree(mi_fls_key);
  }
  void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
    mi_assert_internal(mi_fls_key != (DWORD)(-1));
    FlsSetValue(mi_fls_key, heap);
  }
#endif
#if defined(MI_SHARED_LIB) && !defined(MI_WIN_NOREDIRECT)
  #define MI_PRIM_HAS_ALLOCATOR_INIT 1
  static bool mi_redirected = false;
  bool _mi_is_redirected(void) {
    return mi_redirected;
  }
#if 0
  #endif
  mi_decl_export void _mi_redirect_entry(DWORD reason) {
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
#if 0
  #endif
  bool _mi_allocator_init(const char** message) {
    return mi_allocator_init(message);
  }
  void _mi_allocator_done(void) {
    mi_allocator_done();
  }
#endif
#elif defined(__APPLE__)
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
#if 0
#endif
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#if defined(__linux__)
  #include <features.h>
  #include <sys/prctl.h>
  #if defined(__GLIBC__)
  #include <linux/mman.h>
  #else
  #include <sys/mman.h>
  #endif
#elif defined(__APPLE__)
  #include <AvailabilityMacros.h>
  #include <TargetConditionals.h>
  #if !defined(TARGET_OS_OSX) || TARGET_OS_OSX
  #include <mach/vm_statistics.h>
  #endif
  #if !defined(MAC_OS_X_VERSION_10_7)
  #define MAC_OS_X_VERSION_10_7 1070
  #endif
#elif defined(__FreeBSD__) || defined(__DragonFly__)
  #include <sys/param.h>
  #if __FreeBSD_version >= 1200000
  #include <sys/cpuset.h>
  #include <sys/domainset.h>
  #endif
  #include <sys/sysctl.h>
#endif
#if defined(__linux__) || defined(__FreeBSD__)
  #define MI_HAS_SYSCALL_H
  #include <sys/syscall.h>
#endif
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
#endif
static bool unix_detect_overcommit(void) {
  bool os_overcommit = true;
#if defined(__linux__)
  int fd = mi_prim_open("/proc/sys/vm/overcommit_memory", O_RDONLY);
 if (fd >= 0) {
    char buf[32];
    ssize_t nread = mi_prim_read(fd, &buf, sizeof(buf));
    mi_prim_close(fd);
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
#endif
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
    #endif
  }
  config->large_page_size = 2*MI_MiB;
  config->has_overcommit = unix_detect_overcommit();
  config->has_partial_free = true;
  config->has_virtual_reserve = true;
  #if (defined(__linux__) || defined(__ANDROID__)) && defined(PR_GET_THP_DISABLE)
  #if defined(MI_NO_THP)
  if (true)
  #else
  if (!mi_option_is_enabled(mi_option_allow_large_os_pages))
  #endif
  {
    int val = 0;
    if (prctl(PR_GET_THP_DISABLE, &val, 0, 0, 0) != 0) {
      val = 1;
      (void)prctl(PR_SET_THP_DISABLE, &val, 0, 0, 0);
    }
  }
  #endif
}
int _mi_prim_free(void* addr, size_t size ) {
  bool err = (munmap(addr, size) == -1);
  return (err ? errno : 0);
}
static int unix_madvise(void* addr, size_t size, int advice) {
#if 0
  #else
  int res = madvise(addr, size, advice);
  #endif
  return (res==0 ? 0 : errno);
}
static void* unix_mmap_prim(void* addr, size_t size, size_t try_alignment, int protect_flags, int flags, int fd) {
  MI_UNUSED(try_alignment);
  void* p = NULL;
  #if defined(MAP_ALIGNED)
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0) {
    size_t n = mi_bsr(try_alignment);
    if (((size_t)1 << n) == try_alignment && n >= 12 && n <= 30) {
      p = mmap(addr, size, protect_flags, flags | MAP_ALIGNED(n), fd, 0);
      if (p==MAP_FAILED || !_mi_is_aligned(p,try_alignment)) {
        int err = errno;
        _mi_trace_message("unable to directly request aligned OS memory (error: %d (0x%x), size: 0x%zx bytes, alignment: 0x%zx, hint address: %p)\n", err, err, size, try_alignment, addr);
      }
      if (p!=MAP_FAILED) return p;
    }
  }
  #elif defined(MAP_ALIGN)
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0) {
    p = mmap((void*)try_alignment, size, protect_flags, flags | MAP_ALIGN, fd, 0);
    if (p!=MAP_FAILED) return p;
  }
  #endif
  #if (MI_INTPTR_SIZE >= 8) && !defined(MAP_ALIGNED)
  if (addr == NULL) {
    void* hint = _mi_os_get_aligned_hint(try_alignment, size);
    if (hint != NULL) {
      p = mmap(hint, size, protect_flags, flags, fd, 0);
      if (p==MAP_FAILED || !_mi_is_aligned(p,try_alignment)) {
        #if MI_TRACK_ENABLED
        int err = 0;
        #else
        int err = errno;
        #endif
        _mi_trace_message("unable to directly request hinted aligned OS memory (error: %d (0x%x), size: 0x%zx bytes, alignment: 0x%zx, hint address: %p)\n", err, err, size, try_alignment, hint);
      }
      if (p!=MAP_FAILED) return p;
    }
  }
  #endif
  p = mmap(addr, size, protect_flags, flags, fd, 0);
  if (p!=MAP_FAILED) return p;
  return NULL;
}
static int unix_mmap_fd(void) {
  #if defined(VM_MAKE_TAG)
  int os_tag = (int)mi_option_get(mi_option_os_tag);
  if (os_tag < 100 || os_tag > 255) { os_tag = 100; }
  return VM_MAKE_TAG(os_tag);
  #else
  return -1;
  #endif
}
static void* unix_mmap(void* addr, size_t size, size_t try_alignment, int protect_flags, bool large_only, bool allow_large, bool* is_large) {
  #if !defined(MAP_ANONYMOUS)
  #define MAP_ANONYMOUS MAP_ANON
  #endif
  #if !defined(MAP_NORESERVE)
  #define MAP_NORESERVE 0
  #endif
  void* p = NULL;
  const int fd = unix_mmap_fd();
  int flags = MAP_PRIVATE | MAP_ANONYMOUS;
  if (_mi_os_has_overcommit()) {
    flags |= MAP_NORESERVE;
  }
  #if defined(PROT_MAX)
  protect_flags |= PROT_MAX(PROT_READ | PROT_WRITE);
  #endif
  if ((large_only || _mi_os_use_large_page(size, try_alignment)) && allow_large) {
    static _Atomic(size_t) large_page_try_ok;
    size_t try_ok = mi_atomic_load_acquire(&large_page_try_ok);
    if (!large_only && try_ok > 0) {
      mi_atomic_cas_strong_acq_rel(&large_page_try_ok, &try_ok, try_ok - 1);
    }
    else {
      int lflags = flags & ~MAP_NORESERVE;
      int lfd = fd;
      #ifdef MAP_ALIGNED_SUPER
      lflags |= MAP_ALIGNED_SUPER;
      #endif
      #ifdef MAP_HUGETLB
      lflags |= MAP_HUGETLB;
      #endif
      #ifdef MAP_HUGE_1GB
      static bool mi_huge_pages_available = true;
      if ((size % MI_GiB) == 0 && mi_huge_pages_available) {
        lflags |= MAP_HUGE_1GB;
      }
      else
      #endif
      {
        #ifdef MAP_HUGE_2MB
        lflags |= MAP_HUGE_2MB;
        #endif
      }
      #ifdef VM_FLAGS_SUPERPAGE_SIZE_2MB
      lfd |= VM_FLAGS_SUPERPAGE_SIZE_2MB;
      #endif
      if (large_only || lflags != flags) {
        *is_large = true;
        p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
        #ifdef MAP_HUGE_1GB
        if (p == NULL && (lflags & MAP_HUGE_1GB) == MAP_HUGE_1GB) {
          mi_huge_pages_available = false;
          _mi_warning_message("unable to allocate huge (1GiB) page, trying large (2MiB) pages instead (errno: %i)\n", errno);
          lflags = ((lflags & ~MAP_HUGE_1GB) | MAP_HUGE_2MB);
          p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
        }
        #endif
        if (large_only) return p;
        if (p == NULL) {
          mi_atomic_store_release(&large_page_try_ok, (size_t)8);
        }
      }
    }
  }
  if (p == NULL) {
    *is_large = false;
    p = unix_mmap_prim(addr, size, try_alignment, protect_flags, flags, fd);
    if (p != NULL) {
      #if defined(MADV_HUGEPAGE)
      if (allow_large && _mi_os_use_large_page(size, try_alignment)) {
        if (unix_madvise(p, size, MADV_HUGEPAGE) == 0) {
        };
      }
      #elif defined(__sun)
      if (allow_large && _mi_os_use_large_page(size, try_alignment)) {
        struct memcntl_mha cmd = {0};
        cmd.mha_pagesize = _mi_os_large_page_size();
        cmd.mha_cmd = MHA_MAPSIZE_VA;
        if (memcntl((caddr_t)p, size, MC_HAT_ADVISE, (caddr_t)&cmd, 0, 0) == 0) {
        }
      }
      #endif
    }
  }
  return p;
}
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr) {
  mi_assert_internal(size > 0 && (size % _mi_os_page_size()) == 0);
  mi_assert_internal(commit || !allow_large);
  mi_assert_internal(try_alignment > 0);
  *is_zero = true;
  int protect_flags = (commit ? (PROT_WRITE | PROT_READ) : PROT_NONE);
  *addr = unix_mmap(hint_addr, size, try_alignment, protect_flags, false, allow_large, is_large);
  return (*addr != NULL ? 0 : errno);
}
static void unix_mprotect_hint(int err) {
  #if defined(__linux__) && (MI_SECURE>=2)
  if (err == ENOMEM) {
    _mi_warning_message("The next warning may be caused by a low memory map limit.\n"
                        "  On Linux this is controlled by the vm.max_map_count -- maybe increase it?\n"
                        "  For example: sudo sysctl -w vm.max_map_count=262144\n");
  }
  #else
  MI_UNUSED(err);
  #endif
}
int _mi_prim_commit(void* start, size_t size, bool* is_zero) {
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
  err = unix_madvise(start, size, MADV_DONTNEED);
  #if !MI_DEBUG && !MI_SECURE
    *needs_recommit = false;
  #else
    *needs_recommit = true;
    mprotect(start, size, PROT_NONE);
  #endif
  return err;
}
int _mi_prim_reset(void* start, size_t size) {
  #if defined(MADV_FREE)
  static _Atomic(size_t) advice = MI_ATOMIC_VAR_INIT(MADV_FREE);
  int oadvice = (int)mi_atomic_load_relaxed(&advice);
  int err;
  while ((err = unix_madvise(start, size, oadvice)) != 0 && errno == EAGAIN) { errno = 0; };
  if (err != 0 && errno == EINVAL && oadvice == MADV_FREE) {
    mi_atomic_store_release(&advice, (size_t)MADV_DONTNEED);
    err = unix_madvise(start, size, MADV_DONTNEED);
  }
  #else
  int err = unix_madvise(start, size, MADV_DONTNEED);
  #endif
  return err;
}
int _mi_prim_protect(void* start, size_t size, bool protect) {
  int err = mprotect(start, size, protect ? PROT_NONE : (PROT_READ | PROT_WRITE));
  if (err != 0) { err = errno; }
  unix_mprotect_hint(err);
  return err;
}
#if (MI_INTPTR_SIZE >= 8) && !defined(__HAIKU__) && !defined(__CYGWIN__)
#ifndef MPOL_PREFERRED
#define MPOL_PREFERRED 1
#endif
#if defined(MI_HAS_SYSCALL_H) && defined(SYS_mbind)
static long mi_prim_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  return syscall(SYS_mbind, start, len, mode, nmask, maxnode, flags);
}
#else
static long mi_prim_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  MI_UNUSED(start); MI_UNUSED(len); MI_UNUSED(mode); MI_UNUSED(nmask); MI_UNUSED(maxnode); MI_UNUSED(flags);
  return 0;
}
#endif
int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  bool is_large = true;
  *is_zero = true;
  *addr = unix_mmap(hint_addr, size, MI_SEGMENT_SIZE, PROT_READ | PROT_WRITE, true, true, &is_large);
  if (*addr != NULL && numa_node >= 0 && numa_node < 8*MI_INTPTR_SIZE) {
    unsigned long numa_mask = (1UL << numa_node);
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
#endif
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
  #endif
}
size_t _mi_prim_numa_node_count(void) {
  char buf[128];
  unsigned node = 0;
  for(node = 0; node < 256; node++) {
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
#endif
#include <time.h>
#if defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC)
mi_msecs_t _mi_prim_clock_now(void) {
  struct timespec t;
  #ifdef CLOCK_MONOTONIC
  clock_gettime(CLOCK_MONOTONIC, &t);
  #else
  clock_gettime(CLOCK_REALTIME, &t);
  #endif
  return ((mi_msecs_t)t.tv_sec * 1000) + ((mi_msecs_t)t.tv_nsec / 1000000);
}
#else
mi_msecs_t _mi_prim_clock_now(void) {
  #if !defined(CLOCKS_PER_SEC) || (CLOCKS_PER_SEC == 1000) || (CLOCKS_PER_SEC == 0)
  return (mi_msecs_t)clock();
  #elif (CLOCKS_PER_SEC < 1000)
  return (mi_msecs_t)clock() * (1000 / (mi_msecs_t)CLOCKS_PER_SEC);
  #else
  return (mi_msecs_t)clock() / ((mi_msecs_t)CLOCKS_PER_SEC / 1000);
  #endif
}
#endif
#if defined(__unix__) || defined(__unix) || defined(unix) || defined(__APPLE__) || defined(__HAIKU__)
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#if defined(__APPLE__)
#include <mach/mach.h>
#endif
#if defined(__HAIKU__)
#include <kernel/OS.h>
#endif
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
#endif
#if defined(__HAIKU__)
  thread_info tid;
  area_info mem;
  ssize_t c;
  get_thread_info(find_thread(0), &tid);
  while (get_next_area_info(tid.team, &c, &mem) == B_OK) {
    pinfo->peak_rss += mem.ram_size;
  }
  pinfo->page_faults = 0;
#elif defined(__APPLE__)
  pinfo->peak_rss = rusage.ru_maxrss;
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
  #endif
#else
  pinfo->peak_rss = rusage.ru_maxrss * 1024;
#endif
}
#else
#ifndef __wasi__
        
#pragma message("define a way to get process info")
#endif
void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  MI_UNUSED(pinfo);
}
#endif
void _mi_prim_out_stderr( const char* msg ) {
  fputs(msg,stderr);
}
#if !defined(MI_USE_ENVIRON) || (MI_USE_ENVIRON!=0)
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
#endif
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  if (name==NULL) return false;
  const size_t len = _mi_strlen(name);
  if (len == 0) return false;
  char** env = mi_get_environ();
  if (env == NULL) return false;
  for (int i = 0; i < 10000 && env[i] != NULL; i++) {
    const char* s = env[i];
    if (_mi_strnicmp(name, s, len) == 0 && s[len] == '=') {
      _mi_strlcpy(result, s + len + 1, result_size);
      return true;
    }
  }
  return false;
}
#else
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  if (_mi_preloading()) return false;
  const char* s = getenv(name);
  if (s == NULL) {
    char buf[64+1];
    size_t len = _mi_strnlen(name,sizeof(buf)-1);
    for (size_t i = 0; i < len; i++) {
      buf[i] = _mi_toupper(name[i]);
    }
    buf[len] = 0;
    s = getenv(buf);
  }
  if (s == NULL || _mi_strnlen(s,result_size) >= result_size) return false;
  _mi_strlcpy(result, s, result_size);
  return true;
}
#endif
#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_15) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_15)
#include <CommonCrypto/CommonCryptoError.h>
#include <CommonCrypto/CommonRandom.h>
bool _mi_prim_random_buf(void* buf, size_t buf_len) {
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
#elif defined(__APPLE__) || defined(__linux__) || defined(__HAIKU__)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  #if defined(MI_HAS_SYSCALL_H) && defined(SYS_getrandom)
    #ifndef GRND_NONBLOCK
    #define GRND_NONBLOCK (1)
    #endif
    static _Atomic(uintptr_t) no_getrandom;
    if (mi_atomic_load_acquire(&no_getrandom)==0) {
      ssize_t ret = syscall(SYS_getrandom, buf, buf_len, GRND_NONBLOCK);
      if (ret >= 0) return (buf_len == (size_t)ret);
      if (errno != ENOSYS) return false;
      mi_atomic_store_release(&no_getrandom, (uintptr_t)1);
    }
  #endif
  int flags = O_RDONLY;
  #if defined(O_CLOEXEC)
  flags |= O_CLOEXEC;
  #endif
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
#endif
#if defined(MI_USE_PTHREADS)
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
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {
    pthread_key_delete(_mi_heap_default_key);
  }
}
void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {
    pthread_setspecific(_mi_heap_default_key, heap);
  }
}
#else
void _mi_prim_thread_init_auto_done(void) {
}
void _mi_prim_thread_done_auto_done(void) {
}
void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  MI_UNUSED(heap);
}
#endif
#elif defined(__wasi__)
#define MI_USE_SBRK 
#include <stdio.h>
#include <stdlib.h>
void _mi_prim_mem_init( mi_os_mem_config_t* config ) {
  config->page_size = 64*MI_KiB;
  config->alloc_granularity = 16;
  config->has_overcommit = false;
  config->has_partial_free = false;
  config->has_virtual_reserve = false;
}
int _mi_prim_free(void* addr, size_t size ) {
  MI_UNUSED(addr); MI_UNUSED(size);
  return 0;
}
#if defined(MI_USE_SBRK)
  #include <unistd.h>
  static void* mi_memory_grow( size_t size ) {
    void* p = sbrk(size);
    if (p == (void*)(-1)) return NULL;
    #if !defined(__wasi__)
    memset(p,0,size);
    #endif
    return p;
  }
#elif defined(__wasi__)
  static void* mi_memory_grow( size_t size ) {
    size_t base = (size > 0 ? __builtin_wasm_memory_grow(0,_mi_divide_up(size, _mi_os_page_size()))
                            : __builtin_wasm_memory_size(0));
    if (base == SIZE_MAX) return NULL;
    return (void*)(base * _mi_os_page_size());
  }
#endif
#if defined(MI_USE_PTHREADS)
static pthread_mutex_t mi_heap_grow_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif
static void* mi_prim_mem_grow(size_t size, size_t try_alignment) {
  void* p = NULL;
  if (try_alignment <= 1) {
    #if defined(MI_USE_PTHREADS)
    pthread_mutex_lock(&mi_heap_grow_mutex);
    #endif
    p = mi_memory_grow(size);
    #if defined(MI_USE_PTHREADS)
    pthread_mutex_unlock(&mi_heap_grow_mutex);
    #endif
  }
  else {
    void* base = NULL;
    size_t alloc_size = 0;
    #if defined(MI_USE_PTHREADS)
    pthread_mutex_lock(&mi_heap_grow_mutex);
    #endif
    {
      void* current = mi_memory_grow(0);
      if (current != NULL) {
        void* aligned_current = mi_align_up_ptr(current, try_alignment);
        alloc_size = _mi_align_up( ((uint8_t*)aligned_current - (uint8_t*)current) + size, _mi_os_page_size());
        base = mi_memory_grow(alloc_size);
      }
    }
    #if defined(MI_USE_PTHREADS)
    pthread_mutex_unlock(&mi_heap_grow_mutex);
    #endif
    if (base != NULL) {
      p = mi_align_up_ptr(base, try_alignment);
      if ((uint8_t*)p + size > (uint8_t*)base + alloc_size) {
        p = NULL;
      }
    }
  }
  mi_assert_internal( p == NULL || try_alignment == 0 || (uintptr_t)p % try_alignment == 0 );
  return p;
}
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr) {
  MI_UNUSED(allow_large); MI_UNUSED(commit); MI_UNUSED(hint_addr);
  *is_large = false;
  *is_zero = false;
  *addr = mi_prim_mem_grow(size, try_alignment);
  return (*addr != NULL ? 0 : ENOMEM);
}
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
#include <time.h>
#if defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC)
mi_msecs_t _mi_prim_clock_now(void) {
  struct timespec t;
  #ifdef CLOCK_MONOTONIC
  clock_gettime(CLOCK_MONOTONIC, &t);
  #else
  clock_gettime(CLOCK_REALTIME, &t);
  #endif
  return ((mi_msecs_t)t.tv_sec * 1000) + ((mi_msecs_t)t.tv_nsec / 1000000);
}
#else
mi_msecs_t _mi_prim_clock_now(void) {
  #if !defined(CLOCKS_PER_SEC) || (CLOCKS_PER_SEC == 1000) || (CLOCKS_PER_SEC == 0)
  return (mi_msecs_t)clock();
  #elif (CLOCKS_PER_SEC < 1000)
  return (mi_msecs_t)clock() * (1000 / (mi_msecs_t)CLOCKS_PER_SEC);
  #else
  return (mi_msecs_t)clock() / ((mi_msecs_t)CLOCKS_PER_SEC / 1000);
  #endif
}
#endif
void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  MI_UNUSED(pinfo);
}
void _mi_prim_out_stderr( const char* msg ) {
  fputs(msg,stderr);
}
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  if (_mi_preloading()) return false;
  const char* s = getenv(name);
  if (s == NULL) {
    char buf[64+1];
    size_t len = _mi_strnlen(name,sizeof(buf)-1);
    for (size_t i = 0; i < len; i++) {
      buf[i] = _mi_toupper(name[i]);
    }
    buf[len] = 0;
    s = getenv(buf);
  }
  if (s == NULL || _mi_strnlen(s,result_size) >= result_size) return false;
  _mi_strlcpy(result, s, result_size);
  return true;
}
bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  return false;
}
void _mi_prim_thread_init_auto_done(void) {
}
void _mi_prim_thread_done_auto_done(void) {
}
void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  MI_UNUSED(heap);
}
#elif defined(__EMSCRIPTEN__)
void _mi_prim_mem_init( mi_os_mem_config_t* config) {
  config->page_size = 64*MI_KiB;
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
extern void* emmalloc_memalign(size_t alignment, size_t size);
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr) {
  MI_UNUSED(try_alignment); MI_UNUSED(allow_large); MI_UNUSED(commit); MI_UNUSED(hint_addr);
  *is_large = false;
  *is_zero = false;
  #define MIN_EMMALLOC_ALIGN 8
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
#include <emscripten/html5.h>
mi_msecs_t _mi_prim_clock_now(void) {
  return emscripten_date_now();
}
void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  MI_UNUSED(pinfo);
}
#include <emscripten/console.h>
void _mi_prim_out_stderr( const char* msg) {
  emscripten_console_error(msg);
}
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  MI_UNUSED(name);
  MI_UNUSED(result);
  MI_UNUSED(result_size);
  return false;
}
bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  int err = getentropy(buf, buf_len);
  return !err;
}
#if defined(MI_USE_PTHREADS)
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
}
void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {
    pthread_setspecific(_mi_heap_default_key, heap);
  }
}
#else
void _mi_prim_thread_init_auto_done(void) {
}
void _mi_prim_thread_done_auto_done(void) {
}
void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  MI_UNUSED(heap);
}
#endif
#else
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE 
#endif
#if 0
#endif
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#if defined(__linux__)
  #include <features.h>
  #include <sys/prctl.h>
  #if defined(__GLIBC__)
  #include <linux/mman.h>
  #else
  #include <sys/mman.h>
  #endif
#elif defined(__APPLE__)
  #include <AvailabilityMacros.h>
  #include <TargetConditionals.h>
  #if !defined(TARGET_OS_OSX) || TARGET_OS_OSX
  #include <mach/vm_statistics.h>
  #endif
  #if !defined(MAC_OS_X_VERSION_10_7)
  #define MAC_OS_X_VERSION_10_7 1070
  #endif
#elif defined(__FreeBSD__) || defined(__DragonFly__)
  #include <sys/param.h>
  #if __FreeBSD_version >= 1200000
  #include <sys/cpuset.h>
  #include <sys/domainset.h>
  #endif
  #include <sys/sysctl.h>
#endif
#if defined(__linux__) || defined(__FreeBSD__)
  #define MI_HAS_SYSCALL_H
  #include <sys/syscall.h>
#endif
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
#endif
static bool unix_detect_overcommit(void) {
  bool os_overcommit = true;
#if defined(__linux__)
  int fd = mi_prim_open("/proc/sys/vm/overcommit_memory", O_RDONLY);
 if (fd >= 0) {
    char buf[32];
    ssize_t nread = mi_prim_read(fd, &buf, sizeof(buf));
    mi_prim_close(fd);
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
#endif
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
    #endif
  }
  config->large_page_size = 2*MI_MiB;
  config->has_overcommit = unix_detect_overcommit();
  config->has_partial_free = true;
  config->has_virtual_reserve = true;
  #if (defined(__linux__) || defined(__ANDROID__)) && defined(PR_GET_THP_DISABLE)
  #if defined(MI_NO_THP)
  if (true)
  #else
  if (!mi_option_is_enabled(mi_option_allow_large_os_pages))
  #endif
  {
    int val = 0;
    if (prctl(PR_GET_THP_DISABLE, &val, 0, 0, 0) != 0) {
      val = 1;
      (void)prctl(PR_SET_THP_DISABLE, &val, 0, 0, 0);
    }
  }
  #endif
}
int _mi_prim_free(void* addr, size_t size ) {
  bool err = (munmap(addr, size) == -1);
  return (err ? errno : 0);
}
static int unix_madvise(void* addr, size_t size, int advice) {
#if 0
  #else
  int res = madvise(addr, size, advice);
  #endif
  return (res==0 ? 0 : errno);
}
static void* unix_mmap_prim(void* addr, size_t size, size_t try_alignment, int protect_flags, int flags, int fd) {
  MI_UNUSED(try_alignment);
  void* p = NULL;
  #if defined(MAP_ALIGNED)
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0) {
    size_t n = mi_bsr(try_alignment);
    if (((size_t)1 << n) == try_alignment && n >= 12 && n <= 30) {
      p = mmap(addr, size, protect_flags, flags | MAP_ALIGNED(n), fd, 0);
      if (p==MAP_FAILED || !_mi_is_aligned(p,try_alignment)) {
        int err = errno;
        _mi_trace_message("unable to directly request aligned OS memory (error: %d (0x%x), size: 0x%zx bytes, alignment: 0x%zx, hint address: %p)\n", err, err, size, try_alignment, addr);
      }
      if (p!=MAP_FAILED) return p;
    }
  }
  #elif defined(MAP_ALIGN)
  if (addr == NULL && try_alignment > 1 && (try_alignment % _mi_os_page_size()) == 0) {
    p = mmap((void*)try_alignment, size, protect_flags, flags | MAP_ALIGN, fd, 0);
    if (p!=MAP_FAILED) return p;
  }
  #endif
  #if (MI_INTPTR_SIZE >= 8) && !defined(MAP_ALIGNED)
  if (addr == NULL) {
    void* hint = _mi_os_get_aligned_hint(try_alignment, size);
    if (hint != NULL) {
      p = mmap(hint, size, protect_flags, flags, fd, 0);
      if (p==MAP_FAILED || !_mi_is_aligned(p,try_alignment)) {
        #if MI_TRACK_ENABLED
        int err = 0;
        #else
        int err = errno;
        #endif
        _mi_trace_message("unable to directly request hinted aligned OS memory (error: %d (0x%x), size: 0x%zx bytes, alignment: 0x%zx, hint address: %p)\n", err, err, size, try_alignment, hint);
      }
      if (p!=MAP_FAILED) return p;
    }
  }
  #endif
  p = mmap(addr, size, protect_flags, flags, fd, 0);
  if (p!=MAP_FAILED) return p;
  return NULL;
}
static int unix_mmap_fd(void) {
  #if defined(VM_MAKE_TAG)
  int os_tag = (int)mi_option_get(mi_option_os_tag);
  if (os_tag < 100 || os_tag > 255) { os_tag = 100; }
  return VM_MAKE_TAG(os_tag);
  #else
  return -1;
  #endif
}
static void* unix_mmap(void* addr, size_t size, size_t try_alignment, int protect_flags, bool large_only, bool allow_large, bool* is_large) {
  #if !defined(MAP_ANONYMOUS)
  #define MAP_ANONYMOUS MAP_ANON
  #endif
  #if !defined(MAP_NORESERVE)
  #define MAP_NORESERVE 0
  #endif
  void* p = NULL;
  const int fd = unix_mmap_fd();
  int flags = MAP_PRIVATE | MAP_ANONYMOUS;
  if (_mi_os_has_overcommit()) {
    flags |= MAP_NORESERVE;
  }
  #if defined(PROT_MAX)
  protect_flags |= PROT_MAX(PROT_READ | PROT_WRITE);
  #endif
  if ((large_only || _mi_os_use_large_page(size, try_alignment)) && allow_large) {
    static _Atomic(size_t) large_page_try_ok;
    size_t try_ok = mi_atomic_load_acquire(&large_page_try_ok);
    if (!large_only && try_ok > 0) {
      mi_atomic_cas_strong_acq_rel(&large_page_try_ok, &try_ok, try_ok - 1);
    }
    else {
      int lflags = flags & ~MAP_NORESERVE;
      int lfd = fd;
      #ifdef MAP_ALIGNED_SUPER
      lflags |= MAP_ALIGNED_SUPER;
      #endif
      #ifdef MAP_HUGETLB
      lflags |= MAP_HUGETLB;
      #endif
      #ifdef MAP_HUGE_1GB
      static bool mi_huge_pages_available = true;
      if ((size % MI_GiB) == 0 && mi_huge_pages_available) {
        lflags |= MAP_HUGE_1GB;
      }
      else
      #endif
      {
        #ifdef MAP_HUGE_2MB
        lflags |= MAP_HUGE_2MB;
        #endif
      }
      #ifdef VM_FLAGS_SUPERPAGE_SIZE_2MB
      lfd |= VM_FLAGS_SUPERPAGE_SIZE_2MB;
      #endif
      if (large_only || lflags != flags) {
        *is_large = true;
        p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
        #ifdef MAP_HUGE_1GB
        if (p == NULL && (lflags & MAP_HUGE_1GB) == MAP_HUGE_1GB) {
          mi_huge_pages_available = false;
          _mi_warning_message("unable to allocate huge (1GiB) page, trying large (2MiB) pages instead (errno: %i)\n", errno);
          lflags = ((lflags & ~MAP_HUGE_1GB) | MAP_HUGE_2MB);
          p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
        }
        #endif
        if (large_only) return p;
        if (p == NULL) {
          mi_atomic_store_release(&large_page_try_ok, (size_t)8);
        }
      }
    }
  }
  if (p == NULL) {
    *is_large = false;
    p = unix_mmap_prim(addr, size, try_alignment, protect_flags, flags, fd);
    if (p != NULL) {
      #if defined(MADV_HUGEPAGE)
      if (allow_large && _mi_os_use_large_page(size, try_alignment)) {
        if (unix_madvise(p, size, MADV_HUGEPAGE) == 0) {
        };
      }
      #elif defined(__sun)
      if (allow_large && _mi_os_use_large_page(size, try_alignment)) {
        struct memcntl_mha cmd = {0};
        cmd.mha_pagesize = _mi_os_large_page_size();
        cmd.mha_cmd = MHA_MAPSIZE_VA;
        if (memcntl((caddr_t)p, size, MC_HAT_ADVISE, (caddr_t)&cmd, 0, 0) == 0) {
        }
      }
      #endif
    }
  }
  return p;
}
int _mi_prim_alloc(void* hint_addr, size_t size, size_t try_alignment, bool commit, bool allow_large, bool* is_large, bool* is_zero, void** addr) {
  mi_assert_internal(size > 0 && (size % _mi_os_page_size()) == 0);
  mi_assert_internal(commit || !allow_large);
  mi_assert_internal(try_alignment > 0);
  *is_zero = true;
  int protect_flags = (commit ? (PROT_WRITE | PROT_READ) : PROT_NONE);
  *addr = unix_mmap(hint_addr, size, try_alignment, protect_flags, false, allow_large, is_large);
  return (*addr != NULL ? 0 : errno);
}
static void unix_mprotect_hint(int err) {
  #if defined(__linux__) && (MI_SECURE>=2)
  if (err == ENOMEM) {
    _mi_warning_message("The next warning may be caused by a low memory map limit.\n"
                        "  On Linux this is controlled by the vm.max_map_count -- maybe increase it?\n"
                        "  For example: sudo sysctl -w vm.max_map_count=262144\n");
  }
  #else
  MI_UNUSED(err);
  #endif
}
int _mi_prim_commit(void* start, size_t size, bool* is_zero) {
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
  err = unix_madvise(start, size, MADV_DONTNEED);
  #if !MI_DEBUG && !MI_SECURE
    *needs_recommit = false;
  #else
    *needs_recommit = true;
    mprotect(start, size, PROT_NONE);
  #endif
  return err;
}
int _mi_prim_reset(void* start, size_t size) {
  #if defined(MADV_FREE)
  static _Atomic(size_t) advice = MI_ATOMIC_VAR_INIT(MADV_FREE);
  int oadvice = (int)mi_atomic_load_relaxed(&advice);
  int err;
  while ((err = unix_madvise(start, size, oadvice)) != 0 && errno == EAGAIN) { errno = 0; };
  if (err != 0 && errno == EINVAL && oadvice == MADV_FREE) {
    mi_atomic_store_release(&advice, (size_t)MADV_DONTNEED);
    err = unix_madvise(start, size, MADV_DONTNEED);
  }
  #else
  int err = unix_madvise(start, size, MADV_DONTNEED);
  #endif
  return err;
}
int _mi_prim_protect(void* start, size_t size, bool protect) {
  int err = mprotect(start, size, protect ? PROT_NONE : (PROT_READ | PROT_WRITE));
  if (err != 0) { err = errno; }
  unix_mprotect_hint(err);
  return err;
}
#if (MI_INTPTR_SIZE >= 8) && !defined(__HAIKU__) && !defined(__CYGWIN__)
#ifndef MPOL_PREFERRED
#define MPOL_PREFERRED 1
#endif
#if defined(MI_HAS_SYSCALL_H) && defined(SYS_mbind)
static long mi_prim_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  return syscall(SYS_mbind, start, len, mode, nmask, maxnode, flags);
}
#else
static long mi_prim_mbind(void* start, unsigned long len, unsigned long mode, const unsigned long* nmask, unsigned long maxnode, unsigned flags) {
  MI_UNUSED(start); MI_UNUSED(len); MI_UNUSED(mode); MI_UNUSED(nmask); MI_UNUSED(maxnode); MI_UNUSED(flags);
  return 0;
}
#endif
int _mi_prim_alloc_huge_os_pages(void* hint_addr, size_t size, int numa_node, bool* is_zero, void** addr) {
  bool is_large = true;
  *is_zero = true;
  *addr = unix_mmap(hint_addr, size, MI_SEGMENT_SIZE, PROT_READ | PROT_WRITE, true, true, &is_large);
  if (*addr != NULL && numa_node >= 0 && numa_node < 8*MI_INTPTR_SIZE) {
    unsigned long numa_mask = (1UL << numa_node);
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
#endif
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
  #endif
}
size_t _mi_prim_numa_node_count(void) {
  char buf[128];
  unsigned node = 0;
  for(node = 0; node < 256; node++) {
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
#endif
#include <time.h>
#if defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC)
mi_msecs_t _mi_prim_clock_now(void) {
  struct timespec t;
  #ifdef CLOCK_MONOTONIC
  clock_gettime(CLOCK_MONOTONIC, &t);
  #else
  clock_gettime(CLOCK_REALTIME, &t);
  #endif
  return ((mi_msecs_t)t.tv_sec * 1000) + ((mi_msecs_t)t.tv_nsec / 1000000);
}
#else
mi_msecs_t _mi_prim_clock_now(void) {
  #if !defined(CLOCKS_PER_SEC) || (CLOCKS_PER_SEC == 1000) || (CLOCKS_PER_SEC == 0)
  return (mi_msecs_t)clock();
  #elif (CLOCKS_PER_SEC < 1000)
  return (mi_msecs_t)clock() * (1000 / (mi_msecs_t)CLOCKS_PER_SEC);
  #else
  return (mi_msecs_t)clock() / ((mi_msecs_t)CLOCKS_PER_SEC / 1000);
  #endif
}
#endif
#if defined(__unix__) || defined(__unix) || defined(unix) || defined(__APPLE__) || defined(__HAIKU__)
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#if defined(__APPLE__)
#include <mach/mach.h>
#endif
#if defined(__HAIKU__)
#include <kernel/OS.h>
#endif
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
#endif
#if defined(__HAIKU__)
  thread_info tid;
  area_info mem;
  ssize_t c;
  get_thread_info(find_thread(0), &tid);
  while (get_next_area_info(tid.team, &c, &mem) == B_OK) {
    pinfo->peak_rss += mem.ram_size;
  }
  pinfo->page_faults = 0;
#elif defined(__APPLE__)
  pinfo->peak_rss = rusage.ru_maxrss;
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
  #endif
#else
  pinfo->peak_rss = rusage.ru_maxrss * 1024;
#endif
}
#else
#ifndef __wasi__
        
#pragma message("define a way to get process info")
#endif
void _mi_prim_process_info(mi_process_info_t* pinfo)
{
  MI_UNUSED(pinfo);
}
#endif
void _mi_prim_out_stderr( const char* msg ) {
  fputs(msg,stderr);
}
#if !defined(MI_USE_ENVIRON) || (MI_USE_ENVIRON!=0)
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
#endif
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  if (name==NULL) return false;
  const size_t len = _mi_strlen(name);
  if (len == 0) return false;
  char** env = mi_get_environ();
  if (env == NULL) return false;
  for (int i = 0; i < 10000 && env[i] != NULL; i++) {
    const char* s = env[i];
    if (_mi_strnicmp(name, s, len) == 0 && s[len] == '=') {
      _mi_strlcpy(result, s + len + 1, result_size);
      return true;
    }
  }
  return false;
}
#else
bool _mi_prim_getenv(const char* name, char* result, size_t result_size) {
  if (_mi_preloading()) return false;
  const char* s = getenv(name);
  if (s == NULL) {
    char buf[64+1];
    size_t len = _mi_strnlen(name,sizeof(buf)-1);
    for (size_t i = 0; i < len; i++) {
      buf[i] = _mi_toupper(name[i]);
    }
    buf[len] = 0;
    s = getenv(buf);
  }
  if (s == NULL || _mi_strnlen(s,result_size) >= result_size) return false;
  _mi_strlcpy(result, s, result_size);
  return true;
}
#endif
#if defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_15) && (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_15)
#include <CommonCrypto/CommonCryptoError.h>
#include <CommonCrypto/CommonRandom.h>
bool _mi_prim_random_buf(void* buf, size_t buf_len) {
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
#elif defined(__APPLE__) || defined(__linux__) || defined(__HAIKU__)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
bool _mi_prim_random_buf(void* buf, size_t buf_len) {
  #if defined(MI_HAS_SYSCALL_H) && defined(SYS_getrandom)
    #ifndef GRND_NONBLOCK
    #define GRND_NONBLOCK (1)
    #endif
    static _Atomic(uintptr_t) no_getrandom;
    if (mi_atomic_load_acquire(&no_getrandom)==0) {
      ssize_t ret = syscall(SYS_getrandom, buf, buf_len, GRND_NONBLOCK);
      if (ret >= 0) return (buf_len == (size_t)ret);
      if (errno != ENOSYS) return false;
      mi_atomic_store_release(&no_getrandom, (uintptr_t)1);
    }
  #endif
  int flags = O_RDONLY;
  #if defined(O_CLOEXEC)
  flags |= O_CLOEXEC;
  #endif
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
#endif
#if defined(MI_USE_PTHREADS)
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
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {
    pthread_key_delete(_mi_heap_default_key);
  }
}
void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  if (_mi_heap_default_key != (pthread_key_t)(-1)) {
    pthread_setspecific(_mi_heap_default_key, heap);
  }
}
#else
void _mi_prim_thread_init_auto_done(void) {
}
void _mi_prim_thread_done_auto_done(void) {
}
void _mi_prim_thread_associate_default_heap(mi_heap_t* heap) {
  MI_UNUSED(heap);
}
#endif
#endif
#ifndef MI_PRIM_HAS_PROCESS_ATTACH
#if defined(__GNUC__) || defined(__clang__)
  #if defined(__clang__)
    #define mi_attr_constructor __attribute__((constructor(101)))
    #define mi_attr_destructor __attribute__((destructor(101)))
  #else
    #define mi_attr_constructor __attribute__((constructor))
    #define mi_attr_destructor __attribute__((destructor))
  #endif
  static void mi_attr_constructor mi_process_attach(void) {
    _mi_process_load();
  }
  static void mi_attr_destructor mi_process_detach(void) {
    _mi_process_done();
  }
#elif 0
 #else
  #pragma message("define a way to call _mi_process_load/done on your platform")
#endif
#endif
#ifndef MI_PRIM_HAS_ALLOCATOR_INIT
bool _mi_is_redirected(void) {
  return false;
}
bool _mi_allocator_init(const char** message) {
  if (message != NULL) { *message = NULL; }
  return true;
}
void _mi_allocator_done(void) {
}
#endif
#if 0
#if 0
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#if 0
  #else
  #endif
#if 0
  #endif
#if 0
  #endif
#else
#endif
#if 0
#endif
#if 0
#else
#if 0
#else
#endif
#if 0
  #endif
#endif
#endif
#endif
