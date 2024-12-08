#![allow(
    dead_code,
    mutable_transmutes,
    non_camel_case_types,
    non_snake_case,
    non_upper_case_globals,
    unused_assignments,
    unused_mut
)]
#![feature(c_variadic, core_intrinsics, extern_types, thread_local)]
#![allow(internal_features)]

use c2rust_bitfields::BitfieldStruct;

#[cfg(not(all(
    target_arch = "x86_64",
    target_os = "linux",
    target_env = "gnu",
    target_pointer_width = "64"
)))]
compile_error!("mimalloc_oxide only supports x86_64-unknown-linux-gnu now");

include!("./extra.rs");

use core::arch::asm;
#[cfg(target_arch = "x86")]
pub use core::arch::x86::_mm_pause;
#[cfg(target_arch = "x86_64")]
pub use core::arch::x86_64::_mm_pause;
unsafe extern "C" {
    pub type _IO_wide_data;
    pub type _IO_codecvt;
    pub type _IO_marker;
    fn clock_gettime(__clock_id: clockid_t, __tp: *mut timespec) -> libc::c_int;
    fn pthread_mutex_init(
        __mutex: *mut pthread_mutex_t,
        __mutexattr: *const pthread_mutexattr_t,
    ) -> libc::c_int;
    fn pthread_mutex_destroy(__mutex: *mut pthread_mutex_t) -> libc::c_int;
    fn pthread_mutex_trylock(__mutex: *mut pthread_mutex_t) -> libc::c_int;
    fn pthread_mutex_lock(__mutex: *mut pthread_mutex_t) -> libc::c_int;
    fn pthread_mutex_unlock(__mutex: *mut pthread_mutex_t) -> libc::c_int;
    fn pthread_key_create(
        __key: *mut pthread_key_t,
        __destr_function: Option<unsafe extern "C" fn(*mut libc::c_void) -> ()>,
    ) -> libc::c_int;
    fn pthread_key_delete(__key: pthread_key_t) -> libc::c_int;
    fn pthread_setspecific(__key: pthread_key_t, __pointer: *const libc::c_void) -> libc::c_int;
    fn strtol(_: *const libc::c_char, _: *mut *mut libc::c_char, _: libc::c_int) -> libc::c_long;
    fn abort() -> !;
    fn getenv(__name: *const libc::c_char) -> *mut libc::c_char;
    fn realpath(__name: *const libc::c_char, __resolved: *mut libc::c_char) -> *mut libc::c_char;
    fn __errno_location() -> *mut libc::c_int;
    fn memcpy(_: *mut libc::c_void, _: *const libc::c_void, _: libc::c_ulong) -> *mut libc::c_void;
    fn memset(_: *mut libc::c_void, _: libc::c_int, _: libc::c_ulong) -> *mut libc::c_void;
    fn strstr(_: *const libc::c_char, _: *const libc::c_char) -> *mut libc::c_char;
    static mut stdout: *mut FILE;
    static mut stderr: *mut FILE;
    fn fputs(__s: *const libc::c_char, __stream: *mut FILE) -> libc::c_int;
    fn mmap(
        __addr: *mut libc::c_void,
        __len: size_t,
        __prot: libc::c_int,
        __flags: libc::c_int,
        __fd: libc::c_int,
        __offset: __off_t,
    ) -> *mut libc::c_void;
    fn munmap(__addr: *mut libc::c_void, __len: size_t) -> libc::c_int;
    fn mprotect(__addr: *mut libc::c_void, __len: size_t, __prot: libc::c_int) -> libc::c_int;
    fn madvise(__addr: *mut libc::c_void, __len: size_t, __advice: libc::c_int) -> libc::c_int;
    fn sysconf(__name: libc::c_int) -> libc::c_long;
    fn syscall(__sysno: libc::c_long, _: ...) -> libc::c_long;
    fn prctl(__option: libc::c_int, _: ...) -> libc::c_int;
    fn getrusage(__who: __rusage_who_t, __usage: *mut rusage) -> libc::c_int;
    static mut environ: *mut *mut libc::c_char;
}
pub type __builtin_va_list = [__va_list_tag; 1];
#[derive(Copy, Clone)]
#[repr(C)]
pub struct __va_list_tag {
    pub gp_offset: libc::c_uint,
    pub fp_offset: libc::c_uint,
    pub overflow_arg_area: *mut libc::c_void,
    pub reg_save_area: *mut libc::c_void,
}
pub type ptrdiff_t = libc::c_long;
pub type size_t = libc::c_ulong;
pub type mi_heap_t = mi_heap_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_heap_s {
    pub tld: *mut mi_tld_t,
    pub thread_delayed_free: *mut mi_block_t,
    pub thread_id: mi_threadid_t,
    pub arena_id: mi_arena_id_t,
    pub cookie: uintptr_t,
    pub keys: [uintptr_t; 2],
    pub random: mi_random_ctx_t,
    pub page_count: size_t,
    pub page_retired_min: size_t,
    pub page_retired_max: size_t,
    pub next: *mut mi_heap_t,
    pub no_reclaim: bool,
    pub tag: uint8_t,
    pub pages_free_direct: [*mut mi_page_t; 130],
    pub pages: [mi_page_queue_t; 75],
}
pub type mi_page_queue_t = mi_page_queue_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_page_queue_s {
    pub first: *mut mi_page_t,
    pub last: *mut mi_page_t,
    pub block_size: size_t,
}
pub type mi_page_t = mi_page_s;
#[derive(Copy, Clone, BitfieldStruct)]
#[repr(C)]
pub struct mi_page_s {
    pub segment_idx: uint8_t,
    #[bitfield(name = "segment_in_use", ty = "uint8_t", bits = "0..=0")]
    #[bitfield(name = "is_committed", ty = "uint8_t", bits = "1..=1")]
    #[bitfield(name = "is_zero_init", ty = "uint8_t", bits = "2..=2")]
    #[bitfield(name = "is_huge", ty = "uint8_t", bits = "3..=3")]
    pub segment_in_use_is_committed_is_zero_init_is_huge: [u8; 1],
    pub capacity: uint16_t,
    pub reserved: uint16_t,
    pub flags: mi_page_flags_t,
    #[bitfield(name = "free_is_zero", ty = "uint8_t", bits = "0..=0")]
    #[bitfield(name = "retire_expire", ty = "uint8_t", bits = "1..=7")]
    pub free_is_zero_retire_expire: [u8; 1],
    pub free: *mut mi_block_t,
    pub local_free: *mut mi_block_t,
    pub used: uint16_t,
    pub block_size_shift: uint8_t,
    pub heap_tag: uint8_t,
    pub block_size: size_t,
    pub page_start: *mut uint8_t,
    pub keys: [uintptr_t; 2],
    pub xthread_free: mi_thread_free_t,
    pub xheap: uintptr_t,
    pub next: *mut mi_page_s,
    pub prev: *mut mi_page_s,
}
pub type uintptr_t = libc::c_ulong;
pub type mi_thread_free_t = uintptr_t;
pub type uint8_t = __uint8_t;
pub type __uint8_t = libc::c_uchar;
pub type uint16_t = __uint16_t;
pub type __uint16_t = libc::c_ushort;
pub type mi_block_t = mi_block_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_block_s {
    pub next: mi_encoded_t,
}
pub type mi_encoded_t = uintptr_t;
pub type mi_page_flags_t = mi_page_flags_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub union mi_page_flags_s {
    pub full_aligned: uint8_t,
    pub x: C2RustUnnamed,
}
#[derive(Copy, Clone, BitfieldStruct)]
#[repr(C)]
pub struct C2RustUnnamed {
    #[bitfield(name = "in_full", ty = "uint8_t", bits = "0..=0")]
    #[bitfield(name = "has_aligned", ty = "uint8_t", bits = "1..=1")]
    pub in_full_has_aligned: [u8; 1],
}
pub type mi_random_ctx_t = mi_random_cxt_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_random_cxt_s {
    pub input: [uint32_t; 16],
    pub output: [uint32_t; 16],
    pub output_available: libc::c_int,
    pub weak: bool,
}
pub type uint32_t = __uint32_t;
pub type __uint32_t = libc::c_uint;
pub type mi_arena_id_t = libc::c_int;
pub type mi_threadid_t = size_t;
pub type mi_tld_t = mi_tld_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_tld_s {
    pub heartbeat: libc::c_ulonglong,
    pub recurse: bool,
    pub heap_backing: *mut mi_heap_t,
    pub heaps: *mut mi_heap_t,
    pub segments: mi_segments_tld_t,
    pub os: mi_os_tld_t,
    pub stats: mi_stats_t,
}
pub type mi_stats_t = mi_stats_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_stats_s {
    pub segments: mi_stat_count_t,
    pub pages: mi_stat_count_t,
    pub reserved: mi_stat_count_t,
    pub committed: mi_stat_count_t,
    pub reset: mi_stat_count_t,
    pub purged: mi_stat_count_t,
    pub page_committed: mi_stat_count_t,
    pub segments_abandoned: mi_stat_count_t,
    pub pages_abandoned: mi_stat_count_t,
    pub threads: mi_stat_count_t,
    pub normal: mi_stat_count_t,
    pub huge: mi_stat_count_t,
    pub giant: mi_stat_count_t,
    pub malloc: mi_stat_count_t,
    pub segments_cache: mi_stat_count_t,
    pub pages_extended: mi_stat_counter_t,
    pub mmap_calls: mi_stat_counter_t,
    pub commit_calls: mi_stat_counter_t,
    pub reset_calls: mi_stat_counter_t,
    pub purge_calls: mi_stat_counter_t,
    pub page_no_retire: mi_stat_counter_t,
    pub searches: mi_stat_counter_t,
    pub normal_count: mi_stat_counter_t,
    pub huge_count: mi_stat_counter_t,
    pub arena_count: mi_stat_counter_t,
    pub arena_crossover_count: mi_stat_counter_t,
    pub arena_rollback_count: mi_stat_counter_t,
    pub guarded_alloc_count: mi_stat_counter_t,
    pub normal_bins: [mi_stat_count_t; 74],
}
pub type mi_stat_count_t = mi_stat_count_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_stat_count_s {
    pub allocated: int64_t,
    pub freed: int64_t,
    pub peak: int64_t,
    pub current: int64_t,
}
pub type int64_t = __int64_t;
pub type __int64_t = libc::c_long;
pub type mi_stat_counter_t = mi_stat_counter_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_stat_counter_s {
    pub total: int64_t,
    pub count: int64_t,
}
pub type mi_os_tld_t = mi_os_tld_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_os_tld_s {
    pub region_idx: size_t,
    pub stats: *mut mi_stats_t,
}
pub type mi_segments_tld_t = mi_segments_tld_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_segments_tld_s {
    pub small_free: mi_segment_queue_t,
    pub medium_free: mi_segment_queue_t,
    pub pages_purge: mi_page_queue_t,
    pub count: size_t,
    pub peak_count: size_t,
    pub current_size: size_t,
    pub peak_size: size_t,
    pub reclaim_count: size_t,
    pub subproc: *mut mi_subproc_t,
    pub stats: *mut mi_stats_t,
    pub os: *mut mi_os_tld_t,
}
pub type mi_subproc_t = mi_subproc_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_subproc_s {
    pub abandoned_count: size_t,
    pub abandoned_os_list_count: size_t,
    pub abandoned_os_lock: pthread_mutex_t,
    pub abandoned_os_visit_lock: pthread_mutex_t,
    pub abandoned_os_list: *mut mi_segment_t,
    pub abandoned_os_list_tail: *mut mi_segment_t,
    pub memid: mi_memid_t,
}
pub type mi_memid_t = mi_memid_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_memid_s {
    pub mem: C2RustUnnamed_0,
    pub is_pinned: bool,
    pub initially_committed: bool,
    pub initially_zero: bool,
    pub memkind: mi_memkind_t,
}
pub type mi_memkind_t = mi_memkind_e;
pub type mi_memkind_e = libc::c_uint;
pub const MI_MEM_ARENA: mi_memkind_e = 6;
pub const MI_MEM_OS_REMAP: mi_memkind_e = 5;
pub const MI_MEM_OS_HUGE: mi_memkind_e = 4;
pub const MI_MEM_OS: mi_memkind_e = 3;
pub const MI_MEM_STATIC: mi_memkind_e = 2;
pub const MI_MEM_EXTERNAL: mi_memkind_e = 1;
pub const MI_MEM_NONE: mi_memkind_e = 0;
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_0 {
    pub os: mi_memid_os_info_t,
    pub arena: mi_memid_arena_info_t,
}
pub type mi_memid_arena_info_t = mi_memid_arena_info;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_memid_arena_info {
    pub block_index: size_t,
    pub id: mi_arena_id_t,
    pub is_exclusive: bool,
}
pub type mi_memid_os_info_t = mi_memid_os_info;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_memid_os_info {
    pub base: *mut libc::c_void,
    pub alignment: size_t,
}
pub type mi_segment_t = mi_segment_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_segment_s {
    pub memid: mi_memid_t,
    pub allow_decommit: bool,
    pub allow_purge: bool,
    pub segment_size: size_t,
    pub subproc: *mut mi_subproc_t,
    pub next: *mut mi_segment_s,
    pub prev: *mut mi_segment_s,
    pub was_reclaimed: bool,
    pub dont_free: bool,
    pub abandoned: size_t,
    pub abandoned_visits: size_t,
    pub used: size_t,
    pub capacity: size_t,
    pub segment_info_size: size_t,
    pub cookie: uintptr_t,
    pub abandoned_os_next: *mut mi_segment_s,
    pub abandoned_os_prev: *mut mi_segment_s,
    pub thread_id: mi_threadid_t,
    pub page_shift: size_t,
    pub page_kind: mi_page_kind_t,
    pub pages: [mi_page_t; 1],
}
pub type mi_page_kind_t = mi_page_kind_e;
pub type mi_page_kind_e = libc::c_uint;
pub const MI_PAGE_HUGE: mi_page_kind_e = 3;
pub const MI_PAGE_LARGE: mi_page_kind_e = 2;
pub const MI_PAGE_MEDIUM: mi_page_kind_e = 1;
pub const MI_PAGE_SMALL: mi_page_kind_e = 0;
#[derive(Copy, Clone)]
#[repr(C)]
pub union pthread_mutex_t {
    pub __data: __pthread_mutex_s,
    pub __size: [libc::c_char; 40],
    pub __align: libc::c_long,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub struct __pthread_mutex_s {
    pub __lock: libc::c_int,
    pub __count: libc::c_uint,
    pub __owner: libc::c_int,
    pub __nusers: libc::c_uint,
    pub __kind: libc::c_int,
    pub __spins: libc::c_short,
    pub __elision: libc::c_short,
    pub __list: __pthread_list_t,
}
pub type __pthread_list_t = __pthread_internal_list;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct __pthread_internal_list {
    pub __prev: *mut __pthread_internal_list,
    pub __next: *mut __pthread_internal_list,
}
pub type mi_segment_queue_t = mi_segment_queue_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_segment_queue_s {
    pub first: *mut mi_segment_t,
    pub last: *mut mi_segment_t,
}
pub type mi_padding_t = mi_padding_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_padding_s {
    pub canary: uint32_t,
    pub delta: uint32_t,
}
pub type mi_output_fun = unsafe extern "C" fn(*const libc::c_char, *mut libc::c_void) -> ();
pub type va_list = __builtin_va_list;
pub const memory_order_acq_rel: memory_order = 4;
pub const memory_order_relaxed: memory_order = 0;
pub const memory_order_acquire: memory_order = 2;
pub type FILE = _IO_FILE;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct _IO_FILE {
    pub _flags: libc::c_int,
    pub _IO_read_ptr: *mut libc::c_char,
    pub _IO_read_end: *mut libc::c_char,
    pub _IO_read_base: *mut libc::c_char,
    pub _IO_write_base: *mut libc::c_char,
    pub _IO_write_ptr: *mut libc::c_char,
    pub _IO_write_end: *mut libc::c_char,
    pub _IO_buf_base: *mut libc::c_char,
    pub _IO_buf_end: *mut libc::c_char,
    pub _IO_save_base: *mut libc::c_char,
    pub _IO_backup_base: *mut libc::c_char,
    pub _IO_save_end: *mut libc::c_char,
    pub _markers: *mut _IO_marker,
    pub _chain: *mut _IO_FILE,
    pub _fileno: libc::c_int,
    pub _flags2: libc::c_int,
    pub _old_offset: __off_t,
    pub _cur_column: libc::c_ushort,
    pub _vtable_offset: libc::c_schar,
    pub _shortbuf: [libc::c_char; 1],
    pub _lock: *mut libc::c_void,
    pub _offset: __off64_t,
    pub _codecvt: *mut _IO_codecvt,
    pub _wide_data: *mut _IO_wide_data,
    pub _freeres_list: *mut _IO_FILE,
    pub _freeres_buf: *mut libc::c_void,
    pub __pad5: size_t,
    pub _mode: libc::c_int,
    pub _unused2: [libc::c_char; 20],
}
pub type __off64_t = libc::c_long;
pub type _IO_lock_t = ();
pub type __off_t = libc::c_long;
pub type uintmax_t = __uintmax_t;
pub type __uintmax_t = libc::c_ulong;
pub type intmax_t = __intmax_t;
pub type __intmax_t = libc::c_long;
pub type intptr_t = libc::c_long;
pub type mi_os_mem_config_t = mi_os_mem_config_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_os_mem_config_s {
    pub page_size: size_t,
    pub large_page_size: size_t,
    pub alloc_granularity: size_t,
    pub physical_memory: size_t,
    pub virtual_address_bits: size_t,
    pub has_overcommit: bool,
    pub has_partial_free: bool,
    pub has_virtual_reserve: bool,
}
pub type mi_option_t = mi_option_e;
pub type mi_option_e = libc::c_uint;
pub const mi_option_limit_os_alloc: mi_option_e = 17;
pub const mi_option_abandoned_page_reset: mi_option_e = 12;
pub const mi_option_reset_delay: mi_option_e = 15;
pub const mi_option_reset_decommits: mi_option_e = 5;
pub const mi_option_eager_region_commit: mi_option_e = 4;
pub const mi_option_large_os_pages: mi_option_e = 6;
pub const _mi_option_last: mi_option_e = 36;
pub const mi_option_target_segments_per_thread: mi_option_e = 35;
pub const mi_option_guarded_sample_seed: mi_option_e = 34;
pub const mi_option_guarded_sample_rate: mi_option_e = 33;
pub const mi_option_guarded_precise: mi_option_e = 32;
pub const mi_option_guarded_max: mi_option_e = 31;
pub const mi_option_guarded_min: mi_option_e = 30;
pub const mi_option_visit_abandoned: mi_option_e = 29;
pub const mi_option_retry_on_oom: mi_option_e = 28;
pub const mi_option_disallow_arena_alloc: mi_option_e = 27;
pub const mi_option_abandoned_reclaim_on_free: mi_option_e = 26;
pub const mi_option_purge_extend_delay: mi_option_e = 25;
pub const mi_option_arena_purge_mult: mi_option_e = 24;
pub const mi_option_arena_reserve: mi_option_e = 23;
pub const mi_option_destroy_on_exit: mi_option_e = 22;
pub const mi_option_max_segment_reclaim: mi_option_e = 21;
pub const mi_option_max_warnings: mi_option_e = 20;
pub const mi_option_max_errors: mi_option_e = 19;
pub const mi_option_os_tag: mi_option_e = 18;
pub const mi_option_disallow_os_alloc: mi_option_e = 17;
pub const mi_option_use_numa_nodes: mi_option_e = 16;
pub const mi_option_purge_delay: mi_option_e = 15;
pub const mi_option_eager_commit_delay: mi_option_e = 14;
pub const mi_option_deprecated_segment_reset: mi_option_e = 13;
pub const mi_option_abandoned_page_purge: mi_option_e = 12;
pub const mi_option_deprecated_page_reset: mi_option_e = 11;
pub const mi_option_deprecated_segment_cache: mi_option_e = 10;
pub const mi_option_reserve_os_memory: mi_option_e = 9;
pub const mi_option_reserve_huge_os_pages_at: mi_option_e = 8;
pub const mi_option_reserve_huge_os_pages: mi_option_e = 7;
pub const mi_option_allow_large_os_pages: mi_option_e = 6;
pub const mi_option_purge_decommits: mi_option_e = 5;
pub const mi_option_arena_eager_commit: mi_option_e = 4;
pub const mi_option_eager_commit: mi_option_e = 3;
pub const mi_option_verbose: mi_option_e = 2;
pub const mi_option_show_stats: mi_option_e = 1;
pub const mi_option_show_errors: mi_option_e = 0;
pub type mi_option_desc_t = mi_option_desc_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_option_desc_s {
    pub value: libc::c_long,
    pub init: mi_init_t,
    pub option: mi_option_t,
    pub name: *const libc::c_char,
    pub legacy_name: *const libc::c_char,
}
pub type mi_init_t = mi_init_e;
pub type mi_init_e = libc::c_uint;
pub const INITIALIZED: mi_init_e = 2;
pub const DEFAULTED: mi_init_e = 1;
pub const UNINIT: mi_init_e = 0;
pub const memory_order_release: memory_order = 3;
pub type mi_arena_t = mi_arena_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_arena_s {
    pub id: mi_arena_id_t,
    pub memid: mi_memid_t,
    pub start: *mut uint8_t,
    pub block_count: size_t,
    pub field_count: size_t,
    pub meta_size: size_t,
    pub meta_memid: mi_memid_t,
    pub numa_node: libc::c_int,
    pub exclusive: bool,
    pub is_large: bool,
    pub abandoned_visit_lock: pthread_mutex_t,
    pub search_idx: size_t,
    pub purge_expire: mi_msecs_t,
    pub blocks_dirty: *mut mi_bitmap_field_t,
    pub blocks_committed: *mut mi_bitmap_field_t,
    pub blocks_purge: *mut mi_bitmap_field_t,
    pub blocks_abandoned: *mut mi_bitmap_field_t,
    pub blocks_inuse: [mi_bitmap_field_t; 1],
}
pub type mi_bitmap_field_t = size_t;
pub type mi_msecs_t = int64_t;
pub type mi_bitmap_index_t = size_t;
pub type mi_bitmap_t = *mut mi_bitmap_field_t;
#[derive(Copy, Clone)]
#[repr(C)]
pub union pthread_mutexattr_t {
    pub __size: [libc::c_char; 4],
    pub __align: libc::c_int,
}
pub type mi_segmap_part_t = mi_segmap_part_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_segmap_part_s {
    pub memid: mi_memid_t,
    pub map: [uintptr_t; 1008],
}
pub type __syscall_slong_t = libc::c_long;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct timespec {
    pub tv_sec: __time_t,
    pub tv_nsec: __syscall_slong_t,
}
pub type __time_t = libc::c_long;
pub type clockid_t = __clockid_t;
pub type __clockid_t = libc::c_int;
pub type mi_atomic_guard_t = uintptr_t;
pub type mi_error_fun = unsafe extern "C" fn(libc::c_int, *mut libc::c_void) -> ();
pub type mi_arena_field_cursor_t = mi_arena_field_cursor_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_arena_field_cursor_s {
    pub os_list_count: size_t,
    pub start: size_t,
    pub end: size_t,
    pub bitmap_idx: size_t,
    pub subproc: *mut mi_subproc_t,
    pub visit_all: bool,
    pub hold_visit_lock: bool,
}
pub const MI_NEVER_DELAYED_FREE: mi_delayed_e = 3;
pub type mi_delayed_t = mi_delayed_e;
pub type mi_delayed_e = libc::c_uint;
pub const MI_NO_DELAYED_FREE: mi_delayed_e = 2;
pub const MI_DELAYED_FREEING: mi_delayed_e = 1;
pub const MI_USE_DELAYED_FREE: mi_delayed_e = 0;
pub type int32_t = __int32_t;
pub type __int32_t = libc::c_int;
pub type mi_thread_data_t = mi_thread_data_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_thread_data_s {
    pub heap: mi_heap_t,
    pub tld: mi_tld_t,
    pub memid: mi_memid_t,
}
pub type pthread_key_t = libc::c_uint;
pub type uint64_t = __uint64_t;
pub type __uint64_t = libc::c_ulong;
pub type ssize_t = __ssize_t;
pub type __ssize_t = libc::c_long;
pub const _SC_PAGESIZE: C2RustUnnamed_15 = 30;
pub const _SC_PHYS_PAGES: C2RustUnnamed_15 = 85;
pub type mi_collect_t = mi_collect_e;
pub type mi_collect_e = libc::c_uint;
pub const MI_ABANDON: mi_collect_e = 2;
pub const MI_FORCE: mi_collect_e = 1;
pub const MI_NORMAL: mi_collect_e = 0;
pub type heap_page_visitor_fun = unsafe extern "C" fn(
    *mut mi_heap_t,
    *mut mi_page_queue_t,
    *mut mi_page_t,
    *mut libc::c_void,
    *mut libc::c_void,
) -> bool;
pub type mi_deferred_free_fun =
    unsafe extern "C" fn(bool, libc::c_ulonglong, *mut libc::c_void) -> ();
pub type mi_atomic_once_t = uintptr_t;
pub type buffered_t = buffered_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct buffered_s {
    pub out: Option<mi_output_fun>,
    pub arg: *mut libc::c_void,
    pub buf: *mut libc::c_char,
    pub used: size_t,
    pub count: size_t,
}
pub type mi_process_info_t = mi_process_info_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_process_info_s {
    pub elapsed: mi_msecs_t,
    pub utime: mi_msecs_t,
    pub stime: mi_msecs_t,
    pub current_rss: size_t,
    pub peak_rss: size_t,
    pub current_commit: size_t,
    pub peak_commit: size_t,
    pub page_faults: size_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_1 {
    pub ru_maxrss: libc::c_long,
    pub __ru_maxrss_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub struct rusage {
    pub ru_utime: timeval,
    pub ru_stime: timeval,
    pub c2rust_unnamed: C2RustUnnamed_1,
    pub c2rust_unnamed_0: C2RustUnnamed_14,
    pub c2rust_unnamed_1: C2RustUnnamed_13,
    pub c2rust_unnamed_2: C2RustUnnamed_12,
    pub c2rust_unnamed_3: C2RustUnnamed_11,
    pub c2rust_unnamed_4: C2RustUnnamed_10,
    pub c2rust_unnamed_5: C2RustUnnamed_9,
    pub c2rust_unnamed_6: C2RustUnnamed_8,
    pub c2rust_unnamed_7: C2RustUnnamed_7,
    pub c2rust_unnamed_8: C2RustUnnamed_6,
    pub c2rust_unnamed_9: C2RustUnnamed_5,
    pub c2rust_unnamed_10: C2RustUnnamed_4,
    pub c2rust_unnamed_11: C2RustUnnamed_3,
    pub c2rust_unnamed_12: C2RustUnnamed_2,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_2 {
    pub ru_nivcsw: libc::c_long,
    pub __ru_nivcsw_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_3 {
    pub ru_nvcsw: libc::c_long,
    pub __ru_nvcsw_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_4 {
    pub ru_nsignals: libc::c_long,
    pub __ru_nsignals_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_5 {
    pub ru_msgrcv: libc::c_long,
    pub __ru_msgrcv_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_6 {
    pub ru_msgsnd: libc::c_long,
    pub __ru_msgsnd_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_7 {
    pub ru_oublock: libc::c_long,
    pub __ru_oublock_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_8 {
    pub ru_inblock: libc::c_long,
    pub __ru_inblock_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_9 {
    pub ru_nswap: libc::c_long,
    pub __ru_nswap_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_10 {
    pub ru_majflt: libc::c_long,
    pub __ru_majflt_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_11 {
    pub ru_minflt: libc::c_long,
    pub __ru_minflt_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_12 {
    pub ru_isrss: libc::c_long,
    pub __ru_isrss_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_13 {
    pub ru_idrss: libc::c_long,
    pub __ru_idrss_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub union C2RustUnnamed_14 {
    pub ru_ixrss: libc::c_long,
    pub __ru_ixrss_word: __syscall_slong_t,
}
#[derive(Copy, Clone)]
#[repr(C)]
pub struct timeval {
    pub tv_sec: __time_t,
    pub tv_usec: __suseconds_t,
}
pub type __suseconds_t = libc::c_long;
pub const RUSAGE_SELF: __rusage_who = 0;
pub type __rusage_who_t = libc::c_int;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_heap_area_s {
    pub blocks: *mut libc::c_void,
    pub reserved: size_t,
    pub committed: size_t,
    pub used: size_t,
    pub block_size: size_t,
    pub full_block_size: size_t,
    pub heap_tag: libc::c_int,
}
pub type mi_heap_area_t = mi_heap_area_s;
pub type mi_block_visit_fun = unsafe extern "C" fn(
    *const mi_heap_t,
    *const mi_heap_area_t,
    *mut libc::c_void,
    size_t,
    *mut libc::c_void,
) -> bool;
pub type mi_visit_blocks_args_t = mi_visit_blocks_args_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_visit_blocks_args_s {
    pub visit_blocks: bool,
    pub visitor: Option<mi_block_visit_fun>,
    pub arg: *mut libc::c_void,
}
pub type mi_heap_area_ex_t = mi_heap_area_ex_s;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct mi_heap_area_ex_s {
    pub area: mi_heap_area_t,
    pub page: *mut mi_page_t,
}
pub type mi_heap_area_visit_fun =
    unsafe extern "C" fn(*const mi_heap_t, *const mi_heap_area_ex_t, *mut libc::c_void) -> bool;
pub type mi_subproc_id_t = *mut libc::c_void;
pub type std_new_handler_t = Option<unsafe extern "C" fn() -> ()>;
pub type memory_order = libc::c_uint;
pub const memory_order_seq_cst: memory_order = 5;
pub const memory_order_consume: memory_order = 1;
pub type C2RustUnnamed_15 = libc::c_uint;
pub const _SC_SIGSTKSZ: C2RustUnnamed_15 = 250;
pub const _SC_MINSIGSTKSZ: C2RustUnnamed_15 = 249;
pub const _SC_THREAD_ROBUST_PRIO_PROTECT: C2RustUnnamed_15 = 248;
pub const _SC_THREAD_ROBUST_PRIO_INHERIT: C2RustUnnamed_15 = 247;
pub const _SC_XOPEN_STREAMS: C2RustUnnamed_15 = 246;
pub const _SC_TRACE_USER_EVENT_MAX: C2RustUnnamed_15 = 245;
pub const _SC_TRACE_SYS_MAX: C2RustUnnamed_15 = 244;
pub const _SC_TRACE_NAME_MAX: C2RustUnnamed_15 = 243;
pub const _SC_TRACE_EVENT_NAME_MAX: C2RustUnnamed_15 = 242;
pub const _SC_SS_REPL_MAX: C2RustUnnamed_15 = 241;
pub const _SC_V7_LPBIG_OFFBIG: C2RustUnnamed_15 = 240;
pub const _SC_V7_LP64_OFF64: C2RustUnnamed_15 = 239;
pub const _SC_V7_ILP32_OFFBIG: C2RustUnnamed_15 = 238;
pub const _SC_V7_ILP32_OFF32: C2RustUnnamed_15 = 237;
pub const _SC_RAW_SOCKETS: C2RustUnnamed_15 = 236;
pub const _SC_IPV6: C2RustUnnamed_15 = 235;
pub const _SC_LEVEL4_CACHE_LINESIZE: C2RustUnnamed_15 = 199;
pub const _SC_LEVEL4_CACHE_ASSOC: C2RustUnnamed_15 = 198;
pub const _SC_LEVEL4_CACHE_SIZE: C2RustUnnamed_15 = 197;
pub const _SC_LEVEL3_CACHE_LINESIZE: C2RustUnnamed_15 = 196;
pub const _SC_LEVEL3_CACHE_ASSOC: C2RustUnnamed_15 = 195;
pub const _SC_LEVEL3_CACHE_SIZE: C2RustUnnamed_15 = 194;
pub const _SC_LEVEL2_CACHE_LINESIZE: C2RustUnnamed_15 = 193;
pub const _SC_LEVEL2_CACHE_ASSOC: C2RustUnnamed_15 = 192;
pub const _SC_LEVEL2_CACHE_SIZE: C2RustUnnamed_15 = 191;
pub const _SC_LEVEL1_DCACHE_LINESIZE: C2RustUnnamed_15 = 190;
pub const _SC_LEVEL1_DCACHE_ASSOC: C2RustUnnamed_15 = 189;
pub const _SC_LEVEL1_DCACHE_SIZE: C2RustUnnamed_15 = 188;
pub const _SC_LEVEL1_ICACHE_LINESIZE: C2RustUnnamed_15 = 187;
pub const _SC_LEVEL1_ICACHE_ASSOC: C2RustUnnamed_15 = 186;
pub const _SC_LEVEL1_ICACHE_SIZE: C2RustUnnamed_15 = 185;
pub const _SC_TRACE_LOG: C2RustUnnamed_15 = 184;
pub const _SC_TRACE_INHERIT: C2RustUnnamed_15 = 183;
pub const _SC_TRACE_EVENT_FILTER: C2RustUnnamed_15 = 182;
pub const _SC_TRACE: C2RustUnnamed_15 = 181;
pub const _SC_HOST_NAME_MAX: C2RustUnnamed_15 = 180;
pub const _SC_V6_LPBIG_OFFBIG: C2RustUnnamed_15 = 179;
pub const _SC_V6_LP64_OFF64: C2RustUnnamed_15 = 178;
pub const _SC_V6_ILP32_OFFBIG: C2RustUnnamed_15 = 177;
pub const _SC_V6_ILP32_OFF32: C2RustUnnamed_15 = 176;
pub const _SC_2_PBS_CHECKPOINT: C2RustUnnamed_15 = 175;
pub const _SC_STREAMS: C2RustUnnamed_15 = 174;
pub const _SC_SYMLOOP_MAX: C2RustUnnamed_15 = 173;
pub const _SC_2_PBS_TRACK: C2RustUnnamed_15 = 172;
pub const _SC_2_PBS_MESSAGE: C2RustUnnamed_15 = 171;
pub const _SC_2_PBS_LOCATE: C2RustUnnamed_15 = 170;
pub const _SC_2_PBS_ACCOUNTING: C2RustUnnamed_15 = 169;
pub const _SC_2_PBS: C2RustUnnamed_15 = 168;
pub const _SC_USER_GROUPS_R: C2RustUnnamed_15 = 167;
pub const _SC_USER_GROUPS: C2RustUnnamed_15 = 166;
pub const _SC_TYPED_MEMORY_OBJECTS: C2RustUnnamed_15 = 165;
pub const _SC_TIMEOUTS: C2RustUnnamed_15 = 164;
pub const _SC_SYSTEM_DATABASE_R: C2RustUnnamed_15 = 163;
pub const _SC_SYSTEM_DATABASE: C2RustUnnamed_15 = 162;
pub const _SC_THREAD_SPORADIC_SERVER: C2RustUnnamed_15 = 161;
pub const _SC_SPORADIC_SERVER: C2RustUnnamed_15 = 160;
pub const _SC_SPAWN: C2RustUnnamed_15 = 159;
pub const _SC_SIGNALS: C2RustUnnamed_15 = 158;
pub const _SC_SHELL: C2RustUnnamed_15 = 157;
pub const _SC_REGEX_VERSION: C2RustUnnamed_15 = 156;
pub const _SC_REGEXP: C2RustUnnamed_15 = 155;
pub const _SC_SPIN_LOCKS: C2RustUnnamed_15 = 154;
pub const _SC_READER_WRITER_LOCKS: C2RustUnnamed_15 = 153;
pub const _SC_NETWORKING: C2RustUnnamed_15 = 152;
pub const _SC_SINGLE_PROCESS: C2RustUnnamed_15 = 151;
pub const _SC_MULTI_PROCESS: C2RustUnnamed_15 = 150;
pub const _SC_MONOTONIC_CLOCK: C2RustUnnamed_15 = 149;
pub const _SC_FILE_SYSTEM: C2RustUnnamed_15 = 148;
pub const _SC_FILE_LOCKING: C2RustUnnamed_15 = 147;
pub const _SC_FILE_ATTRIBUTES: C2RustUnnamed_15 = 146;
pub const _SC_PIPE: C2RustUnnamed_15 = 145;
pub const _SC_FIFO: C2RustUnnamed_15 = 144;
pub const _SC_FD_MGMT: C2RustUnnamed_15 = 143;
pub const _SC_DEVICE_SPECIFIC_R: C2RustUnnamed_15 = 142;
pub const _SC_DEVICE_SPECIFIC: C2RustUnnamed_15 = 141;
pub const _SC_DEVICE_IO: C2RustUnnamed_15 = 140;
pub const _SC_THREAD_CPUTIME: C2RustUnnamed_15 = 139;
pub const _SC_CPUTIME: C2RustUnnamed_15 = 138;
pub const _SC_CLOCK_SELECTION: C2RustUnnamed_15 = 137;
pub const _SC_C_LANG_SUPPORT_R: C2RustUnnamed_15 = 136;
pub const _SC_C_LANG_SUPPORT: C2RustUnnamed_15 = 135;
pub const _SC_BASE: C2RustUnnamed_15 = 134;
pub const _SC_BARRIERS: C2RustUnnamed_15 = 133;
pub const _SC_ADVISORY_INFO: C2RustUnnamed_15 = 132;
pub const _SC_XOPEN_REALTIME_THREADS: C2RustUnnamed_15 = 131;
pub const _SC_XOPEN_REALTIME: C2RustUnnamed_15 = 130;
pub const _SC_XOPEN_LEGACY: C2RustUnnamed_15 = 129;
pub const _SC_XBS5_LPBIG_OFFBIG: C2RustUnnamed_15 = 128;
pub const _SC_XBS5_LP64_OFF64: C2RustUnnamed_15 = 127;
pub const _SC_XBS5_ILP32_OFFBIG: C2RustUnnamed_15 = 126;
pub const _SC_XBS5_ILP32_OFF32: C2RustUnnamed_15 = 125;
pub const _SC_NL_TEXTMAX: C2RustUnnamed_15 = 124;
pub const _SC_NL_SETMAX: C2RustUnnamed_15 = 123;
pub const _SC_NL_NMAX: C2RustUnnamed_15 = 122;
pub const _SC_NL_MSGMAX: C2RustUnnamed_15 = 121;
pub const _SC_NL_LANGMAX: C2RustUnnamed_15 = 120;
pub const _SC_NL_ARGMAX: C2RustUnnamed_15 = 119;
pub const _SC_USHRT_MAX: C2RustUnnamed_15 = 118;
pub const _SC_ULONG_MAX: C2RustUnnamed_15 = 117;
pub const _SC_UINT_MAX: C2RustUnnamed_15 = 116;
pub const _SC_UCHAR_MAX: C2RustUnnamed_15 = 115;
pub const _SC_SHRT_MIN: C2RustUnnamed_15 = 114;
pub const _SC_SHRT_MAX: C2RustUnnamed_15 = 113;
pub const _SC_SCHAR_MIN: C2RustUnnamed_15 = 112;
pub const _SC_SCHAR_MAX: C2RustUnnamed_15 = 111;
pub const _SC_SSIZE_MAX: C2RustUnnamed_15 = 110;
pub const _SC_NZERO: C2RustUnnamed_15 = 109;
pub const _SC_MB_LEN_MAX: C2RustUnnamed_15 = 108;
pub const _SC_WORD_BIT: C2RustUnnamed_15 = 107;
pub const _SC_LONG_BIT: C2RustUnnamed_15 = 106;
pub const _SC_INT_MIN: C2RustUnnamed_15 = 105;
pub const _SC_INT_MAX: C2RustUnnamed_15 = 104;
pub const _SC_CHAR_MIN: C2RustUnnamed_15 = 103;
pub const _SC_CHAR_MAX: C2RustUnnamed_15 = 102;
pub const _SC_CHAR_BIT: C2RustUnnamed_15 = 101;
pub const _SC_XOPEN_XPG4: C2RustUnnamed_15 = 100;
pub const _SC_XOPEN_XPG3: C2RustUnnamed_15 = 99;
pub const _SC_XOPEN_XPG2: C2RustUnnamed_15 = 98;
pub const _SC_2_UPE: C2RustUnnamed_15 = 97;
pub const _SC_2_C_VERSION: C2RustUnnamed_15 = 96;
pub const _SC_2_CHAR_TERM: C2RustUnnamed_15 = 95;
pub const _SC_XOPEN_SHM: C2RustUnnamed_15 = 94;
pub const _SC_XOPEN_ENH_I18N: C2RustUnnamed_15 = 93;
pub const _SC_XOPEN_CRYPT: C2RustUnnamed_15 = 92;
pub const _SC_XOPEN_UNIX: C2RustUnnamed_15 = 91;
pub const _SC_XOPEN_XCU_VERSION: C2RustUnnamed_15 = 90;
pub const _SC_XOPEN_VERSION: C2RustUnnamed_15 = 89;
pub const _SC_PASS_MAX: C2RustUnnamed_15 = 88;
pub const _SC_ATEXIT_MAX: C2RustUnnamed_15 = 87;
pub const _SC_AVPHYS_PAGES: C2RustUnnamed_15 = 86;
pub const _SC_NPROCESSORS_ONLN: C2RustUnnamed_15 = 84;
pub const _SC_NPROCESSORS_CONF: C2RustUnnamed_15 = 83;
pub const _SC_THREAD_PROCESS_SHARED: C2RustUnnamed_15 = 82;
pub const _SC_THREAD_PRIO_PROTECT: C2RustUnnamed_15 = 81;
pub const _SC_THREAD_PRIO_INHERIT: C2RustUnnamed_15 = 80;
pub const _SC_THREAD_PRIORITY_SCHEDULING: C2RustUnnamed_15 = 79;
pub const _SC_THREAD_ATTR_STACKSIZE: C2RustUnnamed_15 = 78;
pub const _SC_THREAD_ATTR_STACKADDR: C2RustUnnamed_15 = 77;
pub const _SC_THREAD_THREADS_MAX: C2RustUnnamed_15 = 76;
pub const _SC_THREAD_STACK_MIN: C2RustUnnamed_15 = 75;
pub const _SC_THREAD_KEYS_MAX: C2RustUnnamed_15 = 74;
pub const _SC_THREAD_DESTRUCTOR_ITERATIONS: C2RustUnnamed_15 = 73;
pub const _SC_TTY_NAME_MAX: C2RustUnnamed_15 = 72;
pub const _SC_LOGIN_NAME_MAX: C2RustUnnamed_15 = 71;
pub const _SC_GETPW_R_SIZE_MAX: C2RustUnnamed_15 = 70;
pub const _SC_GETGR_R_SIZE_MAX: C2RustUnnamed_15 = 69;
pub const _SC_THREAD_SAFE_FUNCTIONS: C2RustUnnamed_15 = 68;
pub const _SC_THREADS: C2RustUnnamed_15 = 67;
pub const _SC_T_IOV_MAX: C2RustUnnamed_15 = 66;
pub const _SC_PII_OSI_M: C2RustUnnamed_15 = 65;
pub const _SC_PII_OSI_CLTS: C2RustUnnamed_15 = 64;
pub const _SC_PII_OSI_COTS: C2RustUnnamed_15 = 63;
pub const _SC_PII_INTERNET_DGRAM: C2RustUnnamed_15 = 62;
pub const _SC_PII_INTERNET_STREAM: C2RustUnnamed_15 = 61;
pub const _SC_IOV_MAX: C2RustUnnamed_15 = 60;
pub const _SC_UIO_MAXIOV: C2RustUnnamed_15 = 60;
pub const _SC_SELECT: C2RustUnnamed_15 = 59;
pub const _SC_POLL: C2RustUnnamed_15 = 58;
pub const _SC_PII_OSI: C2RustUnnamed_15 = 57;
pub const _SC_PII_INTERNET: C2RustUnnamed_15 = 56;
pub const _SC_PII_SOCKET: C2RustUnnamed_15 = 55;
pub const _SC_PII_XTI: C2RustUnnamed_15 = 54;
pub const _SC_PII: C2RustUnnamed_15 = 53;
pub const _SC_2_LOCALEDEF: C2RustUnnamed_15 = 52;
pub const _SC_2_SW_DEV: C2RustUnnamed_15 = 51;
pub const _SC_2_FORT_RUN: C2RustUnnamed_15 = 50;
pub const _SC_2_FORT_DEV: C2RustUnnamed_15 = 49;
pub const _SC_2_C_DEV: C2RustUnnamed_15 = 48;
pub const _SC_2_C_BIND: C2RustUnnamed_15 = 47;
pub const _SC_2_VERSION: C2RustUnnamed_15 = 46;
pub const _SC_CHARCLASS_NAME_MAX: C2RustUnnamed_15 = 45;
pub const _SC_RE_DUP_MAX: C2RustUnnamed_15 = 44;
pub const _SC_LINE_MAX: C2RustUnnamed_15 = 43;
pub const _SC_EXPR_NEST_MAX: C2RustUnnamed_15 = 42;
pub const _SC_EQUIV_CLASS_MAX: C2RustUnnamed_15 = 41;
pub const _SC_COLL_WEIGHTS_MAX: C2RustUnnamed_15 = 40;
pub const _SC_BC_STRING_MAX: C2RustUnnamed_15 = 39;
pub const _SC_BC_SCALE_MAX: C2RustUnnamed_15 = 38;
pub const _SC_BC_DIM_MAX: C2RustUnnamed_15 = 37;
pub const _SC_BC_BASE_MAX: C2RustUnnamed_15 = 36;
pub const _SC_TIMER_MAX: C2RustUnnamed_15 = 35;
pub const _SC_SIGQUEUE_MAX: C2RustUnnamed_15 = 34;
pub const _SC_SEM_VALUE_MAX: C2RustUnnamed_15 = 33;
pub const _SC_SEM_NSEMS_MAX: C2RustUnnamed_15 = 32;
pub const _SC_RTSIG_MAX: C2RustUnnamed_15 = 31;
pub const _SC_VERSION: C2RustUnnamed_15 = 29;
pub const _SC_MQ_PRIO_MAX: C2RustUnnamed_15 = 28;
pub const _SC_MQ_OPEN_MAX: C2RustUnnamed_15 = 27;
pub const _SC_DELAYTIMER_MAX: C2RustUnnamed_15 = 26;
pub const _SC_AIO_PRIO_DELTA_MAX: C2RustUnnamed_15 = 25;
pub const _SC_AIO_MAX: C2RustUnnamed_15 = 24;
pub const _SC_AIO_LISTIO_MAX: C2RustUnnamed_15 = 23;
pub const _SC_SHARED_MEMORY_OBJECTS: C2RustUnnamed_15 = 22;
pub const _SC_SEMAPHORES: C2RustUnnamed_15 = 21;
pub const _SC_MESSAGE_PASSING: C2RustUnnamed_15 = 20;
pub const _SC_MEMORY_PROTECTION: C2RustUnnamed_15 = 19;
pub const _SC_MEMLOCK_RANGE: C2RustUnnamed_15 = 18;
pub const _SC_MEMLOCK: C2RustUnnamed_15 = 17;
pub const _SC_MAPPED_FILES: C2RustUnnamed_15 = 16;
pub const _SC_FSYNC: C2RustUnnamed_15 = 15;
pub const _SC_SYNCHRONIZED_IO: C2RustUnnamed_15 = 14;
pub const _SC_PRIORITIZED_IO: C2RustUnnamed_15 = 13;
pub const _SC_ASYNCHRONOUS_IO: C2RustUnnamed_15 = 12;
pub const _SC_TIMERS: C2RustUnnamed_15 = 11;
pub const _SC_PRIORITY_SCHEDULING: C2RustUnnamed_15 = 10;
pub const _SC_REALTIME_SIGNALS: C2RustUnnamed_15 = 9;
pub const _SC_SAVED_IDS: C2RustUnnamed_15 = 8;
pub const _SC_JOB_CONTROL: C2RustUnnamed_15 = 7;
pub const _SC_TZNAME_MAX: C2RustUnnamed_15 = 6;
pub const _SC_STREAM_MAX: C2RustUnnamed_15 = 5;
pub const _SC_OPEN_MAX: C2RustUnnamed_15 = 4;
pub const _SC_NGROUPS_MAX: C2RustUnnamed_15 = 3;
pub const _SC_CLK_TCK: C2RustUnnamed_15 = 2;
pub const _SC_CHILD_MAX: C2RustUnnamed_15 = 1;
pub const _SC_ARG_MAX: C2RustUnnamed_15 = 0;
pub type __rusage_who = libc::c_int;
pub const RUSAGE_CHILDREN: __rusage_who = -1;
#[inline]
unsafe extern "C" fn mi_atomic_addi64_relaxed(mut p: *mut int64_t, mut add: int64_t) -> int64_t {
    return ::core::intrinsics::atomic_xadd_relaxed(p as *mut int64_t, add);
}
#[inline]
unsafe extern "C" fn mi_atomic_maxi64_relaxed(mut p: *mut int64_t, mut x: int64_t) {
    let mut current: int64_t = ::core::intrinsics::atomic_load_relaxed(p as *mut int64_t);
    while current < x && {
        let fresh0 = ::core::intrinsics::atomic_cxchgweak_release_relaxed(
            p as *mut int64_t,
            *(&mut current as *mut int64_t),
            x,
        );
        *(&mut current as *mut int64_t) = fresh0.0;
        !fresh0.1
    } {}
}
#[inline]
unsafe extern "C" fn mi_atomic_once(mut once: *mut mi_atomic_once_t) -> bool {
    if ::core::intrinsics::atomic_load_relaxed(once) != 0 as libc::c_int as uintptr_t {
        return 0 as libc::c_int != 0;
    }
    let mut expected: uintptr_t = 0 as libc::c_int as uintptr_t;
    let fresh1 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
        once,
        *&mut expected,
        1 as libc::c_int as uintptr_t,
    );
    *&mut expected = fresh1.0;
    return fresh1.1;
}
#[inline]
unsafe extern "C" fn mi_atomic_yield() {
    _mm_pause();
}
#[inline]
unsafe extern "C" fn mi_lock_try_acquire(mut lock: *mut pthread_mutex_t) -> bool {
    return pthread_mutex_trylock(lock) == 0 as libc::c_int;
}
#[inline]
unsafe extern "C" fn mi_lock_acquire(mut lock: *mut pthread_mutex_t) -> bool {
    return pthread_mutex_lock(lock) == 0 as libc::c_int;
}
#[inline]
unsafe extern "C" fn mi_lock_release(mut lock: *mut pthread_mutex_t) {
    pthread_mutex_unlock(lock);
}
#[inline]
unsafe extern "C" fn mi_lock_init(mut lock: *mut pthread_mutex_t) {
    pthread_mutex_init(lock, 0 as *const pthread_mutexattr_t);
}
#[inline]
unsafe extern "C" fn mi_lock_done(mut lock: *mut pthread_mutex_t) {
    pthread_mutex_destroy(lock);
}
#[inline]
unsafe extern "C" fn mi_memkind_is_os(mut memkind: mi_memkind_t) -> bool {
    return memkind as libc::c_uint >= MI_MEM_OS as libc::c_int as libc::c_uint
        && memkind as libc::c_uint <= MI_MEM_OS_REMAP as libc::c_int as libc::c_uint;
}
#[inline]
unsafe extern "C" fn _mi_is_power_of_two(mut x: uintptr_t) -> bool {
    return x & x.wrapping_sub(1 as libc::c_int as uintptr_t) == 0 as libc::c_int as uintptr_t;
}
#[inline]
unsafe extern "C" fn _mi_is_aligned(mut p: *mut libc::c_void, mut alignment: size_t) -> bool {
    if alignment != 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"alignment != 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1640 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_is_aligned\0"))
                .as_ptr(),
        );
    };
    return (p as uintptr_t).wrapping_rem(alignment) == 0 as libc::c_int as libc::c_ulong;
}
#[inline]
unsafe extern "C" fn _mi_align_up(mut sz: uintptr_t, mut alignment: size_t) -> uintptr_t {
    if alignment != 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"alignment != 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1644 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"_mi_align_up\0")).as_ptr(),
        );
    };
    let mut mask: uintptr_t = alignment.wrapping_sub(1 as libc::c_int as size_t);
    if alignment & mask == 0 as libc::c_int as libc::c_ulong {
        return sz.wrapping_add(mask) & !mask;
    } else {
        return sz
            .wrapping_add(mask)
            .wrapping_div(alignment)
            .wrapping_mul(alignment);
    };
}
#[inline]
unsafe extern "C" fn mi_align_up_ptr(
    mut p: *mut libc::c_void,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return _mi_align_up(p as uintptr_t, alignment) as *mut libc::c_void;
}
#[inline]
unsafe extern "C" fn _mi_divide_up(mut size: uintptr_t, mut divider: size_t) -> uintptr_t {
    if divider != 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"divider != 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1657 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"_mi_divide_up\0"))
                .as_ptr(),
        );
    };
    return if divider == 0 as libc::c_int as size_t {
        size
    } else {
        size.wrapping_add(divider)
            .wrapping_sub(1 as libc::c_int as libc::c_ulong)
            .wrapping_div(divider)
    };
}
#[inline]
unsafe extern "C" fn _mi_clamp(mut sz: size_t, mut min: size_t, mut max: size_t) -> size_t {
    if sz < min {
        return min;
    } else if sz > max {
        return max;
    } else {
        return sz;
    };
}
#[inline]
unsafe extern "C" fn _mi_wsize_from_size(mut size: size_t) -> size_t {
    if size
        <= (18446744073709551615 as libc::c_ulong)
            .wrapping_sub(::core::mem::size_of::<uintptr_t>() as libc::c_ulong)
    {
    } else {
        _mi_assert_fail(
            b"size <= SIZE_MAX - sizeof(uintptr_t)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1672 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"_mi_wsize_from_size\0"))
                .as_ptr(),
        );
    };
    return size
        .wrapping_add(::core::mem::size_of::<uintptr_t>() as libc::c_ulong)
        .wrapping_sub(1 as libc::c_int as libc::c_ulong)
        .wrapping_div(::core::mem::size_of::<uintptr_t>() as libc::c_ulong);
}
#[inline]
unsafe extern "C" fn mi_mul_overflow(
    mut count: size_t,
    mut size: size_t,
    mut total: *mut size_t,
) -> bool {
    let (fresh2, fresh3) = count.overflowing_mul(size);
    *(total as *mut libc::c_ulong) = fresh2;
    return fresh3;
}
#[inline]
unsafe extern "C" fn mi_count_size_overflow(
    mut count: size_t,
    mut size: size_t,
    mut total: *mut size_t,
) -> bool {
    if count == 1 as libc::c_int as size_t {
        *total = size;
        return 0 as libc::c_int != 0;
    } else if mi_mul_overflow(count, size, total) as libc::c_int as libc::c_long != 0 {
        _mi_error_message(
            75 as libc::c_int,
            b"allocation request is too large (%zu * %zu bytes)\n\0" as *const u8
                as *const libc::c_char,
            count,
            size,
        );
        *total = 18446744073709551615 as libc::c_ulong;
        return 1 as libc::c_int != 0;
    } else {
        return 0 as libc::c_int != 0;
    };
}
#[inline]
unsafe extern "C" fn mi_heap_is_backing(mut heap: *const mi_heap_t) -> bool {
    return (*(*heap).tld).heap_backing == heap as *mut mi_heap_t;
}
#[inline]
unsafe extern "C" fn mi_heap_is_initialized(mut heap: *mut mi_heap_t) -> bool {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1715 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"mi_heap_is_initialized\0",
            ))
            .as_ptr(),
        );
    };
    return heap != &raw mut _mi_heap_empty;
}
#[inline]
unsafe extern "C" fn _mi_ptr_cookie(mut p: *const libc::c_void) -> uintptr_t {
    if _mi_heap_main.cookie != 0 as libc::c_int as uintptr_t {
    } else {
        _mi_assert_fail(
            b"_mi_heap_main.cookie != 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1720 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_ptr_cookie\0"))
                .as_ptr(),
        );
    };
    return p as uintptr_t ^ _mi_heap_main.cookie;
}
#[inline]
unsafe extern "C" fn _mi_heap_get_free_small_page(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
) -> *mut mi_page_t {
    if size
        <= (128 as libc::c_int as libc::c_ulong)
            .wrapping_mul(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong)
            .wrapping_add(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong)
    {
    } else {
        _mi_assert_fail(
            b"size <= (MI_SMALL_SIZE_MAX + MI_PADDING_SIZE)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1724 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 29], &[libc::c_char; 29]>(
                b"_mi_heap_get_free_small_page\0",
            ))
            .as_ptr(),
        );
    };
    let idx: size_t = _mi_wsize_from_size(size);
    if idx
        < (128 as libc::c_int as libc::c_ulong)
            .wrapping_add(
                (::core::mem::size_of::<mi_padding_t>() as libc::c_ulong)
                    .wrapping_add(((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulong)
                    .wrapping_sub(1 as libc::c_int as libc::c_ulong)
                    .wrapping_div(((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulong),
            )
            .wrapping_add(1 as libc::c_int as libc::c_ulong)
    {
    } else {
        _mi_assert_fail(
            b"idx < MI_PAGES_DIRECT\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1726 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 29], &[libc::c_char; 29]>(
                b"_mi_heap_get_free_small_page\0",
            ))
            .as_ptr(),
        );
    };
    return (*heap).pages_free_direct[idx as usize];
}
#[inline]
unsafe extern "C" fn _mi_ptr_segment(mut p: *const libc::c_void) -> *mut mi_segment_t {
    let segment: *mut mi_segment_t = ((p as uintptr_t).wrapping_sub(1 as libc::c_int as uintptr_t)
        & !(((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_sub(1 as libc::c_int as libc::c_ulonglong) as uintptr_t))
        as *mut mi_segment_t;
    return if segment as intptr_t <= 0 as libc::c_int as intptr_t {
        0 as *mut mi_segment_t
    } else {
        segment
    };
}
#[inline]
unsafe extern "C" fn _mi_page_segment(mut page: *const mi_page_t) -> *mut mi_segment_t {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1738 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_segment\0"))
                .as_ptr(),
        );
    };
    let mut segment: *mut mi_segment_t = _mi_ptr_segment(page as *const libc::c_void);
    if segment.is_null()
        || page
            == &mut *((*segment).pages)
                .as_mut_ptr()
                .offset((*page).segment_idx as isize) as *mut mi_page_t
                as *const mi_page_t
    {
    } else {
        _mi_assert_fail(
            b"segment == NULL || page == &segment->pages[page->segment_idx]\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1740 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_segment\0"))
                .as_ptr(),
        );
    };
    return segment;
}
#[inline]
unsafe extern "C" fn _mi_segment_page_idx_of(
    mut segment: *const mi_segment_t,
    mut p: *const libc::c_void,
) -> size_t {
    let mut diff: ptrdiff_t =
        (p as *mut uint8_t).offset_from(segment as *mut uint8_t) as libc::c_long;
    if diff >= 0 as libc::c_int as ptrdiff_t
        && diff as size_t as libc::c_ulonglong
            <= (1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
    {
    } else {
        _mi_assert_fail(
            b"diff >= 0 && (size_t)diff <= MI_SEGMENT_SIZE\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1745 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                b"_mi_segment_page_idx_of\0",
            ))
            .as_ptr(),
        );
    };
    let mut idx: size_t = diff as size_t >> (*segment).page_shift;
    if idx < (*segment).capacity {
    } else {
        _mi_assert_fail(
            b"idx < segment->capacity\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1747 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                b"_mi_segment_page_idx_of\0",
            ))
            .as_ptr(),
        );
    };
    if (*segment).page_kind as libc::c_uint <= MI_PAGE_MEDIUM as libc::c_int as libc::c_uint
        || idx == 0 as libc::c_int as size_t
    {
    } else {
        _mi_assert_fail(
            b"segment->page_kind <= MI_PAGE_MEDIUM || idx == 0\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1748 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                b"_mi_segment_page_idx_of\0",
            ))
            .as_ptr(),
        );
    };
    return idx;
}
#[inline]
unsafe extern "C" fn _mi_segment_page_of(
    mut segment: *const mi_segment_t,
    mut p: *const libc::c_void,
) -> *mut mi_page_t {
    let mut idx: size_t = _mi_segment_page_idx_of(segment, p);
    return &mut *((*(segment as *mut mi_segment_t)).pages)
        .as_mut_ptr()
        .offset(idx as isize) as *mut mi_page_t;
}
#[inline]
unsafe extern "C" fn mi_page_start(mut page: *const mi_page_t) -> *mut uint8_t {
    if !((*page).page_start).is_null() {
    } else {
        _mi_assert_fail(
            b"page->page_start != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1756 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_page_start\0"))
                .as_ptr(),
        );
    };
    return (*page).page_start;
}
#[inline]
unsafe extern "C" fn _mi_ptr_page(mut p: *mut libc::c_void) -> *mut mi_page_t {
    if !p.is_null() {
    } else {
        _mi_assert_fail(
            b"p!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1761 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"_mi_ptr_page\0")).as_ptr(),
        );
    };
    return _mi_segment_page_of(_mi_ptr_segment(p), p);
}
#[inline]
unsafe extern "C" fn mi_page_block_size(mut page: *const mi_page_t) -> size_t {
    if (*page).block_size > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"page->block_size > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1765 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_page_block_size\0"))
                .as_ptr(),
        );
    };
    return (*page).block_size;
}
#[inline]
unsafe extern "C" fn mi_page_is_huge(mut page: *const mi_page_t) -> bool {
    if (*page).is_huge() as libc::c_int != 0
        && (*_mi_page_segment(page)).page_kind as libc::c_uint
            == MI_PAGE_HUGE as libc::c_int as libc::c_uint
        || (*page).is_huge() == 0
            && (*_mi_page_segment(page)).page_kind as libc::c_uint
                != MI_PAGE_HUGE as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"(page->is_huge && _mi_page_segment(page)->page_kind == MI_PAGE_HUGE) || (!page->is_huge && _mi_page_segment(page)->page_kind != MI_PAGE_HUGE)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1770 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 16],
                &[libc::c_char; 16],
            >(b"mi_page_is_huge\0"))
                .as_ptr(),
        );
    };
    return (*page).is_huge() != 0;
}
#[inline]
unsafe extern "C" fn mi_page_usable_block_size(mut page: *const mi_page_t) -> size_t {
    return (mi_page_block_size(page))
        .wrapping_sub(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong);
}
#[inline]
unsafe extern "C" fn mi_page_thread_free(mut page: *const mi_page_t) -> *mut mi_block_t {
    return (::core::intrinsics::atomic_load_relaxed(&mut (*(page as *mut mi_page_t)).xthread_free)
        & !(3 as libc::c_int) as mi_thread_free_t) as *mut mi_block_t;
}
#[inline]
unsafe extern "C" fn mi_page_thread_free_flag(mut page: *const mi_page_t) -> mi_delayed_t {
    return (::core::intrinsics::atomic_load_relaxed(&mut (*(page as *mut mi_page_t)).xthread_free)
        & 3 as libc::c_int as mi_thread_free_t) as mi_delayed_t;
}
#[inline]
unsafe extern "C" fn mi_page_heap(mut page: *const mi_page_t) -> *mut mi_heap_t {
    return ::core::intrinsics::atomic_load_relaxed(&mut (*(page as *mut mi_page_t)).xheap)
        as *mut mi_heap_t;
}
#[inline]
unsafe extern "C" fn mi_page_set_heap(mut page: *mut mi_page_t, mut heap: *mut mi_heap_t) {
    if mi_page_thread_free_flag(page) as libc::c_uint
        != MI_DELAYED_FREEING as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"mi_page_thread_free_flag(page) != MI_DELAYED_FREEING\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1789 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_page_set_heap\0"))
                .as_ptr(),
        );
    };
    ::core::intrinsics::atomic_store_release(&mut (*page).xheap, heap as uintptr_t);
    if !heap.is_null() {
        (*page).heap_tag = (*heap).tag;
    }
}
#[inline]
unsafe extern "C" fn mi_tf_block(mut tf: mi_thread_free_t) -> *mut mi_block_t {
    return (tf & !(0x3 as libc::c_int) as mi_thread_free_t) as *mut mi_block_t;
}
#[inline]
unsafe extern "C" fn mi_tf_delayed(mut tf: mi_thread_free_t) -> mi_delayed_t {
    return (tf & 0x3 as libc::c_int as mi_thread_free_t) as mi_delayed_t;
}
#[inline]
unsafe extern "C" fn mi_tf_make(
    mut block: *mut mi_block_t,
    mut delayed: mi_delayed_t,
) -> mi_thread_free_t {
    return block as uintptr_t | delayed as uintptr_t;
}
#[inline]
unsafe extern "C" fn mi_tf_set_delayed(
    mut tf: mi_thread_free_t,
    mut delayed: mi_delayed_t,
) -> mi_thread_free_t {
    return mi_tf_make(mi_tf_block(tf), delayed);
}
#[inline]
unsafe extern "C" fn mi_tf_set_block(
    mut tf: mi_thread_free_t,
    mut block: *mut mi_block_t,
) -> mi_thread_free_t {
    return mi_tf_make(block, mi_tf_delayed(tf));
}
#[inline]
unsafe extern "C" fn mi_page_all_free(mut page: *const mi_page_t) -> bool {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1809 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_page_all_free\0"))
                .as_ptr(),
        );
    };
    return (*page).used as libc::c_int == 0 as libc::c_int;
}
#[inline]
unsafe extern "C" fn mi_page_has_any_available(mut page: *const mi_page_t) -> bool {
    if !page.is_null() && (*page).reserved as libc::c_int > 0 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"page != NULL && page->reserved > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1813 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_page_has_any_available\0",
            ))
            .as_ptr(),
        );
    };
    return ((*page).used as libc::c_int) < (*page).reserved as libc::c_int
        || !(mi_page_thread_free(page)).is_null();
}
#[inline]
unsafe extern "C" fn mi_page_immediate_available(mut page: *const mi_page_t) -> bool {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            1817 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"mi_page_immediate_available\0",
            ))
            .as_ptr(),
        );
    };
    return !((*page).free).is_null();
}
#[inline]
unsafe extern "C" fn mi_page_queue(
    mut heap: *const mi_heap_t,
    mut size: size_t,
) -> *mut mi_page_queue_t {
    return &mut *((*(heap as *mut mi_heap_t)).pages)
        .as_mut_ptr()
        .offset((_mi_bin as unsafe extern "C" fn(size_t) -> uint8_t)(size) as isize)
        as *mut mi_page_queue_t;
}
#[inline]
unsafe extern "C" fn mi_page_is_in_full(mut page: *const mi_page_t) -> bool {
    return ((*page).flags.x).in_full() != 0;
}
#[inline]
unsafe extern "C" fn mi_page_set_in_full(mut page: *mut mi_page_t, mut in_full: bool) {
    ((*page).flags.x).set_in_full(in_full as uint8_t);
}
#[inline]
unsafe extern "C" fn mi_page_has_aligned(mut page: *const mi_page_t) -> bool {
    return ((*page).flags.x).has_aligned() != 0;
}
#[inline]
unsafe extern "C" fn mi_page_set_has_aligned(mut page: *mut mi_page_t, mut has_aligned: bool) {
    ((*page).flags.x).set_has_aligned(has_aligned as uint8_t);
}
#[inline]
unsafe extern "C" fn mi_is_in_same_page(
    mut p: *const libc::c_void,
    mut q: *const libc::c_void,
) -> bool {
    let mut segmentp: *mut mi_segment_t = _mi_ptr_segment(p);
    let mut segmentq: *mut mi_segment_t = _mi_ptr_segment(q);
    if segmentp != segmentq {
        return 0 as libc::c_int != 0;
    }
    let mut idxp: size_t = _mi_segment_page_idx_of(segmentp, p);
    let mut idxq: size_t = _mi_segment_page_idx_of(segmentq, q);
    return idxp == idxq;
}
#[inline]
unsafe extern "C" fn mi_rotl(mut x: uintptr_t, mut shift: uintptr_t) -> uintptr_t {
    shift = shift % (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as uintptr_t;
    return if shift == 0 as libc::c_int as uintptr_t {
        x
    } else {
        x << shift
            | x >> ((((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as uintptr_t)
                .wrapping_sub(shift)
    };
}
#[inline]
unsafe extern "C" fn mi_rotr(mut x: uintptr_t, mut shift: uintptr_t) -> uintptr_t {
    shift = shift % (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as uintptr_t;
    return if shift == 0 as libc::c_int as uintptr_t {
        x
    } else {
        x >> shift
            | x << ((((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as uintptr_t)
                .wrapping_sub(shift)
    };
}
#[inline]
unsafe extern "C" fn mi_ptr_decode(
    mut null: *const libc::c_void,
    x: mi_encoded_t,
    mut keys: *const uintptr_t,
) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = (mi_rotr(
        x.wrapping_sub(*keys.offset(0 as libc::c_int as isize)),
        *keys.offset(0 as libc::c_int as isize),
    ) ^ *keys.offset(1 as libc::c_int as isize))
        as *mut libc::c_void;
    return if p == null as *mut libc::c_void {
        0 as *mut libc::c_void
    } else {
        p
    };
}
#[inline]
unsafe extern "C" fn mi_ptr_encode(
    mut null: *const libc::c_void,
    mut p: *const libc::c_void,
    mut keys: *const uintptr_t,
) -> mi_encoded_t {
    let mut x: uintptr_t = (if p.is_null() { null } else { p }) as uintptr_t;
    return (mi_rotl(
        x ^ *keys.offset(1 as libc::c_int as isize),
        *keys.offset(0 as libc::c_int as isize),
    ))
    .wrapping_add(*keys.offset(0 as libc::c_int as isize));
}
#[inline]
unsafe extern "C" fn mi_ptr_encode_canary(
    mut null: *const libc::c_void,
    mut p: *const libc::c_void,
    mut keys: *const uintptr_t,
) -> uint32_t {
    let x: uint32_t = mi_ptr_encode(null, p, keys) as uint32_t;
    return x & 0xffffff00 as libc::c_uint;
}
#[inline]
unsafe extern "C" fn mi_block_nextx(
    mut null: *const libc::c_void,
    mut block: *const mi_block_t,
    mut keys: *const uintptr_t,
) -> *mut mi_block_t {
    let mut next: *mut mi_block_t = 0 as *mut mi_block_t;
    next = mi_ptr_decode(null, (*block).next, keys) as *mut mi_block_t;
    return next;
}
#[inline]
unsafe extern "C" fn mi_block_set_nextx(
    mut null: *const libc::c_void,
    mut block: *mut mi_block_t,
    mut next: *const mi_block_t,
    mut keys: *const uintptr_t,
) {
    (*block).next = mi_ptr_encode(null, next as *const libc::c_void, keys);
}
#[inline]
unsafe extern "C" fn mi_block_next(
    mut page: *const mi_page_t,
    mut block: *const mi_block_t,
) -> *mut mi_block_t {
    let mut next: *mut mi_block_t =
        mi_block_nextx(page as *const libc::c_void, block, ((*page).keys).as_ptr());
    if (!next.is_null()
        && !mi_is_in_same_page(block as *const libc::c_void, next as *const libc::c_void))
        as libc::c_int as libc::c_long
        != 0
    {
        _mi_error_message(
            14 as libc::c_int,
            b"corrupted free list entry of size %zub at %p: value 0x%zx\n\0" as *const u8
                as *const libc::c_char,
            mi_page_block_size(page),
            block,
            next as uintptr_t,
        );
        next = 0 as *mut mi_block_t;
    }
    return next;
}
#[inline]
unsafe extern "C" fn mi_block_set_next(
    mut page: *const mi_page_t,
    mut block: *mut mi_block_t,
    mut next: *const mi_block_t,
) {
    mi_block_set_nextx(
        page as *const libc::c_void,
        block,
        next,
        ((*page).keys).as_ptr(),
    );
}
#[inline]
unsafe extern "C" fn _mi_memid_create(mut memkind: mi_memkind_t) -> mi_memid_t {
    let mut memid: mi_memid_t = mi_memid_s {
        mem: C2RustUnnamed_0 {
            os: mi_memid_os_info {
                base: 0 as *mut libc::c_void,
                alignment: 0,
            },
        },
        is_pinned: false,
        initially_committed: false,
        initially_zero: false,
        memkind: MI_MEM_NONE,
    };
    memset(
        &mut memid as *mut mi_memid_t as *mut libc::c_void,
        0 as libc::c_int,
        ::core::mem::size_of::<mi_memid_t>() as libc::c_ulong,
    );
    memid.memkind = memkind;
    return memid;
}
#[inline]
unsafe extern "C" fn _mi_memid_none() -> mi_memid_t {
    return _mi_memid_create(MI_MEM_NONE);
}
#[inline]
unsafe extern "C" fn _mi_memid_create_os(
    mut committed: bool,
    mut is_zero: bool,
    mut is_large: bool,
) -> mi_memid_t {
    let mut memid: mi_memid_t = _mi_memid_create(MI_MEM_OS);
    memid.initially_committed = committed;
    memid.initially_zero = is_zero;
    memid.is_pinned = is_large;
    return memid;
}
#[inline]
unsafe extern "C" fn _mi_random_shuffle(mut x: uintptr_t) -> uintptr_t {
    if x == 0 as libc::c_int as uintptr_t {
        x = 17 as libc::c_int as uintptr_t;
    }
    x ^= x >> 30 as libc::c_int;
    x = (x as libc::c_ulong).wrapping_mul(0xbf58476d1ce4e5b9 as libc::c_ulong) as uintptr_t
        as uintptr_t;
    x ^= x >> 27 as libc::c_int;
    x = (x as libc::c_ulong).wrapping_mul(0x94d049bb133111eb as libc::c_ulong) as uintptr_t
        as uintptr_t;
    x ^= x >> 31 as libc::c_int;
    return x;
}
#[inline]
unsafe extern "C" fn _mi_os_numa_node(mut tld: *mut mi_os_tld_t) -> libc::c_int {
    if (::core::intrinsics::atomic_load_relaxed(&mut _mi_numa_node_count as *mut size_t)
        == 1 as libc::c_int as size_t) as libc::c_int as libc::c_long
        != 0
    {
        return 0 as libc::c_int;
    } else {
        return _mi_os_numa_node_get(tld);
    };
}
#[inline]
unsafe extern "C" fn _mi_os_numa_node_count() -> size_t {
    let count: size_t = ::core::intrinsics::atomic_load_relaxed(&mut _mi_numa_node_count);
    if (count > 0 as libc::c_int as size_t) as libc::c_int as libc::c_long != 0 {
        return count;
    } else {
        return _mi_os_numa_node_count_get();
    };
}
#[inline]
unsafe extern "C" fn mi_clz(mut x: uintptr_t) -> size_t {
    if x == 0 as libc::c_int as uintptr_t {
        return (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t;
    }
    return x.leading_zeros() as i32 as size_t;
}
#[inline]
unsafe extern "C" fn mi_ctz(mut x: uintptr_t) -> size_t {
    if x == 0 as libc::c_int as uintptr_t {
        return (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t;
    }
    return x.trailing_zeros() as i32 as size_t;
}
#[inline]
unsafe extern "C" fn mi_bsr(mut x: uintptr_t) -> size_t {
    return if x == 0 as libc::c_int as uintptr_t {
        (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t
    } else {
        ((((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int - 1 as libc::c_int) as size_t)
            .wrapping_sub(mi_clz(x))
    };
}
#[inline]
unsafe extern "C" fn _mi_memcpy(
    mut dst: *mut libc::c_void,
    mut src: *const libc::c_void,
    mut n: size_t,
) {
    memcpy(dst, src, n);
}
#[inline]
unsafe extern "C" fn _mi_memzero(mut dst: *mut libc::c_void, mut n: size_t) {
    memset(dst, 0 as libc::c_int, n);
}
#[inline]
unsafe extern "C" fn _mi_memcpy_aligned(
    mut dst: *mut libc::c_void,
    mut src: *const libc::c_void,
    mut n: size_t,
) {
    if dst as uintptr_t % ((1 as libc::c_int) << 3 as libc::c_int) as uintptr_t
        == 0 as libc::c_int as uintptr_t
        && src as uintptr_t % ((1 as libc::c_int) << 3 as libc::c_int) as uintptr_t
            == 0 as libc::c_int as uintptr_t
    {
    } else {
        _mi_assert_fail(
            b"((uintptr_t)dst % MI_INTPTR_SIZE == 0) && ((uintptr_t)src % MI_INTPTR_SIZE == 0)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2087 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_memcpy_aligned\0"))
                .as_ptr(),
        );
    };
    let mut adst: *mut libc::c_void = dst;
    let mut asrc: *const libc::c_void = src;
    _mi_memcpy(adst, asrc, n);
}
#[inline]
unsafe extern "C" fn _mi_memzero_aligned(mut dst: *mut libc::c_void, mut n: size_t) {
    if dst as uintptr_t % ((1 as libc::c_int) << 3 as libc::c_int) as uintptr_t
        == 0 as libc::c_int as uintptr_t
    {
    } else {
        _mi_assert_fail(
            b"(uintptr_t)dst % MI_INTPTR_SIZE == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2093 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"_mi_memzero_aligned\0"))
                .as_ptr(),
        );
    };
    let mut adst: *mut libc::c_void = dst;
    _mi_memzero(adst, n);
}
#[inline]
unsafe extern "C" fn mi_prim_tls_slot(mut slot: size_t) -> *mut libc::c_void {
    let mut res: *mut libc::c_void = 0 as *mut libc::c_void;
    let ofs: size_t =
        slot.wrapping_mul(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong);
    asm!(
        "movq %fs:[{1}], {0}", lateout(reg) res, in (reg) & * (ofs as * mut * mut
        libc::c_void), options(preserves_flags, pure, readonly, att_syntax)
    );
    return res;
}
#[inline]
unsafe extern "C" fn _mi_prim_thread_id() -> mi_threadid_t {
    return mi_prim_tls_slot(0 as libc::c_int as size_t) as uintptr_t;
}
#[inline]
unsafe extern "C" fn mi_prim_get_default_heap() -> *mut mi_heap_t {
    return _mi_heap_default;
}
#[inline]
unsafe extern "C" fn mi_free_block_local(
    mut page: *mut mi_page_t,
    mut block: *mut mi_block_t,
    mut track_stats: bool,
    mut check_full: bool,
) {
    if mi_check_is_double_free(page, block) as libc::c_int as libc::c_long != 0 {
        return;
    }
    mi_check_padding(page, block);
    if track_stats {
        mi_stat_free(page, block);
    }
    memset(
        block as *mut libc::c_void,
        0xdf as libc::c_int,
        mi_page_block_size(page),
    );
    track_stats;
    mi_block_set_next(page, block, (*page).local_free);
    (*page).local_free = block;
    (*page).used = ((*page).used).wrapping_sub(1);
    if ((*page).used as libc::c_int == 0 as libc::c_int) as libc::c_int as libc::c_long != 0 {
        _mi_page_retire(page);
    } else if (check_full as libc::c_int != 0 && mi_page_is_in_full(page) as libc::c_int != 0)
        as libc::c_int as libc::c_long
        != 0
    {
        _mi_page_unfull(page);
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_ptr_unalign(
    mut page: *const mi_page_t,
    mut p: *const libc::c_void,
) -> *mut mi_block_t {
    if !page.is_null() && !p.is_null() {
    } else {
        _mi_assert_fail(
            b"page!=NULL && p!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2392 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_page_ptr_unalign\0"))
                .as_ptr(),
        );
    };
    let mut diff: size_t =
        (p as *mut uint8_t).offset_from((*page).page_start) as libc::c_long as size_t;
    let mut adjust: size_t = 0;
    if ((*page).block_size_shift as libc::c_int != 0 as libc::c_int) as libc::c_int as libc::c_long
        != 0
    {
        adjust = diff
            & ((1 as libc::c_int as size_t) << (*page).block_size_shift as libc::c_int)
                .wrapping_sub(1 as libc::c_int as size_t);
    } else {
        adjust = diff % mi_page_block_size(page);
    }
    return (p as uintptr_t).wrapping_sub(adjust) as *mut mi_block_t;
}
#[inline]
unsafe extern "C" fn mi_block_check_unguard(
    mut page: *mut mi_page_t,
    mut block: *mut mi_block_t,
    mut p: *mut libc::c_void,
) {
}
#[inline(never)]
unsafe extern "C" fn mi_free_generic_local(
    mut page: *mut mi_page_t,
    mut segment: *mut mi_segment_t,
    mut p: *mut libc::c_void,
) {
    let block: *mut mi_block_t = if mi_page_has_aligned(page) as libc::c_int != 0 {
        _mi_page_ptr_unalign(page, p)
    } else {
        p as *mut mi_block_t
    };
    mi_block_check_unguard(page, block, p);
    mi_free_block_local(page, block, 1 as libc::c_int != 0, 1 as libc::c_int != 0);
}
#[inline(never)]
unsafe extern "C" fn mi_free_generic_mt(
    mut page: *mut mi_page_t,
    mut segment: *mut mi_segment_t,
    mut p: *mut libc::c_void,
) {
    let block: *mut mi_block_t = _mi_page_ptr_unalign(page, p);
    mi_block_check_unguard(page, block, p);
    mi_free_block_mt(page, segment, block);
}
#[unsafe(no_mangle)]
#[inline(never)]
pub unsafe extern "C" fn _mi_free_generic(
    mut segment: *mut mi_segment_t,
    mut page: *mut mi_page_t,
    mut is_local: bool,
    mut p: *mut libc::c_void,
) {
    if is_local {
        mi_free_generic_local(page, segment, p);
    } else {
        mi_free_generic_mt(page, segment, p);
    };
}
#[inline]
unsafe extern "C" fn mi_checked_ptr_segment(
    mut p: *const libc::c_void,
    mut msg: *const libc::c_char,
) -> *mut mi_segment_t {
    if (p as uintptr_t & (((1 as libc::c_int) << 3 as libc::c_int) - 1 as libc::c_int) as uintptr_t
        != 0 as libc::c_int as uintptr_t
        && !mi_option_is_enabled(mi_option_guarded_precise)) as libc::c_int as libc::c_long
        != 0
    {
        _mi_error_message(
            22 as libc::c_int,
            b"%s: invalid (unaligned) pointer: %p\n\0" as *const u8 as *const libc::c_char,
            msg,
            p,
        );
        return 0 as *mut mi_segment_t;
    }
    let segment: *mut mi_segment_t = _mi_ptr_segment(p);
    if segment.is_null() as libc::c_int as libc::c_long != 0 {
        return segment;
    }
    if !mi_is_in_heap_region(p) as libc::c_int as libc::c_long != 0 {
        _mi_warning_message(
            b"%s: pointer might not point to a valid heap region: %p\n(this may still be a valid very large allocation (over 64MiB))\n\0"
                as *const u8 as *const libc::c_char,
            msg,
            p,
        );
        if (_mi_ptr_cookie(segment as *const libc::c_void) == (*segment).cookie) as libc::c_int
            as libc::c_long
            != 0
        {
            _mi_warning_message(
                b"(yes, the previous pointer %p was valid after all)\n\0" as *const u8
                    as *const libc::c_char,
                p,
            );
        }
    }
    if (_mi_ptr_cookie(segment as *const libc::c_void) != (*segment).cookie) as libc::c_int
        as libc::c_long
        != 0
    {
        _mi_error_message(
            22 as libc::c_int,
            b"%s: pointer does not point to a valid heap space: %p\n\0" as *const u8
                as *const libc::c_char,
            msg,
            p,
        );
        return 0 as *mut mi_segment_t;
    }
    return segment;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_free(mut p: *mut libc::c_void) {
    let segment: *mut mi_segment_t =
        mi_checked_ptr_segment(p, b"mi_free\0" as *const u8 as *const libc::c_char);
    if segment.is_null() as libc::c_int as libc::c_long != 0 {
        return;
    }
    let is_local: bool = _mi_prim_thread_id()
        == ::core::intrinsics::atomic_load_relaxed(&mut (*segment).thread_id as *mut mi_threadid_t);
    let page: *mut mi_page_t = _mi_segment_page_of(segment, p);
    if is_local as libc::c_int as libc::c_long != 0 {
        if ((*page).flags.full_aligned as libc::c_int == 0 as libc::c_int) as libc::c_int
            as libc::c_long
            != 0
        {
            let block: *mut mi_block_t = p as *mut mi_block_t;
            mi_free_block_local(page, block, 1 as libc::c_int != 0, 0 as libc::c_int != 0);
        } else {
            mi_free_generic_local(page, segment, p);
        }
    } else {
        mi_free_generic_mt(page, segment, p);
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_free_delayed_block(mut block: *mut mi_block_t) -> bool {
    if !block.is_null() {
    } else {
        _mi_assert_fail(
            b"block!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2476 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"_mi_free_delayed_block\0",
            ))
            .as_ptr(),
        );
    };
    let segment: *const mi_segment_t = _mi_ptr_segment(block as *const libc::c_void);
    if _mi_ptr_cookie(segment as *const libc::c_void) == (*segment).cookie {
    } else {
        _mi_assert_fail(
            b"_mi_ptr_cookie(segment) == segment->cookie\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2478 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"_mi_free_delayed_block\0",
            ))
            .as_ptr(),
        );
    };
    if _mi_thread_id() == (*segment).thread_id {
    } else {
        _mi_assert_fail(
            b"_mi_thread_id() == segment->thread_id\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2479 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"_mi_free_delayed_block\0",
            ))
            .as_ptr(),
        );
    };
    let page: *mut mi_page_t = _mi_segment_page_of(segment, block as *const libc::c_void);
    if !_mi_page_try_use_delayed_free(page, MI_USE_DELAYED_FREE, 0 as libc::c_int != 0) {
        return 0 as libc::c_int != 0;
    }
    _mi_page_free_collect(page, 0 as libc::c_int != 0);
    mi_free_block_local(page, block, 0 as libc::c_int != 0, 1 as libc::c_int != 0);
    return 1 as libc::c_int != 0;
}
#[inline(never)]
unsafe extern "C" fn mi_free_block_delayed_mt(
    mut page: *mut mi_page_t,
    mut block: *mut mi_block_t,
) {
    let mut tfreex: mi_thread_free_t = 0;
    let mut use_delayed: bool = false;
    let mut tfree: mi_thread_free_t =
        ::core::intrinsics::atomic_load_relaxed(&mut (*page).xthread_free);
    loop {
        use_delayed = mi_tf_delayed(tfree) as libc::c_uint
            == MI_USE_DELAYED_FREE as libc::c_int as libc::c_uint;
        if use_delayed as libc::c_int as libc::c_long != 0 {
            tfreex = mi_tf_set_delayed(tfree, MI_DELAYED_FREEING);
        } else {
            mi_block_set_next(page, block, mi_tf_block(tfree));
            tfreex = mi_tf_set_block(tfree, block);
        }
        let fresh4 = ::core::intrinsics::atomic_cxchgweak_release_relaxed(
            &mut (*page).xthread_free as *mut mi_thread_free_t,
            *(&mut tfree as *mut mi_thread_free_t),
            tfreex,
        );
        *(&mut tfree as *mut mi_thread_free_t) = fresh4.0;
        if fresh4.1 {
            break;
        }
    }
    if use_delayed as libc::c_int as libc::c_long != 0 {
        let heap: *mut mi_heap_t =
            ::core::intrinsics::atomic_load_acquire(&mut (*page).xheap) as *mut mi_heap_t;
        if !heap.is_null() {
        } else {
            _mi_assert_fail(
                b"heap != NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                2505 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                    b"mi_free_block_delayed_mt\0",
                ))
                .as_ptr(),
            );
        };
        if !heap.is_null() {
            let mut dfree: *mut mi_block_t =
                ::core::intrinsics::atomic_load_relaxed(&mut (*heap).thread_delayed_free);
            loop {
                mi_block_set_nextx(
                    heap as *const libc::c_void,
                    block,
                    dfree,
                    ((*heap).keys).as_mut_ptr(),
                );
                let fresh5 = ::core::intrinsics::atomic_cxchgweak_release_relaxed(
                    &mut (*heap).thread_delayed_free as *mut *mut mi_block_t,
                    *(&mut dfree as *mut *mut mi_block_t),
                    block,
                );
                *(&mut dfree as *mut *mut mi_block_t) = fresh5.0;
                if fresh5.1 {
                    break;
                }
            }
        }
        tfree = ::core::intrinsics::atomic_load_relaxed(&mut (*page).xthread_free);
        loop {
            tfreex = tfree;
            if mi_tf_delayed(tfree) as libc::c_uint
                == MI_DELAYED_FREEING as libc::c_int as libc::c_uint
            {
            } else {
                _mi_assert_fail(
                    b"mi_tf_delayed(tfree) == MI_DELAYED_FREEING\0" as *const u8
                        as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    2515 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                        b"mi_free_block_delayed_mt\0",
                    ))
                    .as_ptr(),
                );
            };
            tfreex = mi_tf_set_delayed(tfree, MI_NO_DELAYED_FREE);
            let fresh6 = ::core::intrinsics::atomic_cxchgweak_release_relaxed(
                &mut (*page).xthread_free as *mut mi_thread_free_t,
                *(&mut tfree as *mut mi_thread_free_t),
                tfreex,
            );
            *(&mut tfree as *mut mi_thread_free_t) = fresh6.0;
            if fresh6.1 {
                break;
            }
        }
    }
}
#[inline(never)]
unsafe extern "C" fn mi_free_block_mt(
    mut page: *mut mi_page_t,
    mut segment: *mut mi_segment_t,
    mut block: *mut mi_block_t,
) {
    if _mi_option_get_fast(mi_option_abandoned_reclaim_on_free) != 0 as libc::c_int as libc::c_long
        && ::core::intrinsics::atomic_load_relaxed(&mut (*segment).thread_id as *mut mi_threadid_t)
            == 0 as libc::c_int as mi_threadid_t
        && mi_prim_get_default_heap() != &raw mut _mi_heap_empty
    {
        if _mi_segment_attempt_reclaim(mi_heap_get_default(), segment) {
            if _mi_thread_id()
                == ::core::intrinsics::atomic_load_relaxed(
                    &mut (*segment).thread_id as *mut mi_threadid_t,
                )
            {
            } else {
                _mi_assert_fail(
                    b"_mi_thread_id() == mi_atomic_load_relaxed(&segment->thread_id)\0" as *const u8
                        as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    2530 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(
                        b"mi_free_block_mt\0",
                    ))
                    .as_ptr(),
                );
            };
            if (*(*mi_heap_get_default()).tld).segments.subproc == (*segment).subproc {
            } else {
                _mi_assert_fail(
                    b"mi_heap_get_default()->tld->segments.subproc == segment->subproc\0"
                        as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    2531 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(
                        b"mi_free_block_mt\0",
                    ))
                    .as_ptr(),
                );
            };
            mi_free(block as *mut libc::c_void);
            return;
        }
    }
    mi_check_padding(page, block);
    mi_stat_free(page, block);
    _mi_padding_shrink(
        page,
        block,
        ::core::mem::size_of::<mi_block_t>() as libc::c_ulong,
    );
    if (*segment).page_kind as libc::c_uint == MI_PAGE_HUGE as libc::c_int as libc::c_uint {
        _mi_segment_huge_page_reset(segment, page, block);
    } else {
        memset(
            block as *mut libc::c_void,
            0xdf as libc::c_int,
            mi_usable_size(block as *const libc::c_void),
        );
    }
    mi_free_block_delayed_mt(page, block);
}
#[inline(never)]
unsafe extern "C" fn mi_page_usable_aligned_size_of(
    mut page: *const mi_page_t,
    mut p: *const libc::c_void,
) -> size_t {
    let mut block: *const mi_block_t = _mi_page_ptr_unalign(page, p);
    let size: size_t = mi_page_usable_size_of(page, block);
    let adjust: ptrdiff_t = (p as *mut uint8_t).offset_from(block as *mut uint8_t) as libc::c_long;
    if adjust >= 0 as libc::c_int as ptrdiff_t && adjust as size_t <= size {
    } else {
        _mi_assert_fail(
            b"adjust >= 0 && (size_t)adjust <= size\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2559 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                b"mi_page_usable_aligned_size_of\0",
            ))
            .as_ptr(),
        );
    };
    let aligned_size: size_t = size.wrapping_sub(adjust as size_t);
    return aligned_size;
}
#[inline]
unsafe extern "C" fn _mi_usable_size(
    mut p: *const libc::c_void,
    mut msg: *const libc::c_char,
) -> size_t {
    let segment: *const mi_segment_t = mi_checked_ptr_segment(p, msg);
    if segment.is_null() as libc::c_int as libc::c_long != 0 {
        return 0 as libc::c_int as size_t;
    }
    let page: *const mi_page_t = _mi_segment_page_of(segment, p);
    if !mi_page_has_aligned(page) as libc::c_int as libc::c_long != 0 {
        let mut block: *const mi_block_t = p as *const mi_block_t;
        return mi_page_usable_size_of(page, block);
    } else {
        return mi_page_usable_aligned_size_of(page, p);
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_usable_size(mut p: *const libc::c_void) -> size_t {
    return _mi_usable_size(p, b"mi_usable_size\0" as *const u8 as *const libc::c_char);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_free_size(mut p: *mut libc::c_void, mut size: size_t) {
    if p.is_null()
        || size <= _mi_usable_size(p, b"mi_free_size\0" as *const u8 as *const libc::c_char)
    {
    } else {
        _mi_assert_fail(
            b"p == NULL || size <= _mi_usable_size(p,\"mi_free_size\")\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2585 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_free_size\0")).as_ptr(),
        );
    };
    mi_free(p);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_free_size_aligned(
    mut p: *mut libc::c_void,
    mut size: size_t,
    mut alignment: size_t,
) {
    if (p as uintptr_t).wrapping_rem(alignment) == 0 as libc::c_int as libc::c_ulong {
    } else {
        _mi_assert_fail(
            b"((uintptr_t)p % alignment) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2590 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_free_size_aligned\0"))
                .as_ptr(),
        );
    };
    mi_free_size(p, size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_free_aligned(mut p: *mut libc::c_void, mut alignment: size_t) {
    if (p as uintptr_t).wrapping_rem(alignment) == 0 as libc::c_int as libc::c_ulong {
    } else {
        _mi_assert_fail(
            b"((uintptr_t)p % alignment) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2595 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_free_aligned\0"))
                .as_ptr(),
        );
    };
    mi_free(p);
}
unsafe extern "C" fn mi_list_contains(
    mut page: *const mi_page_t,
    mut list: *const mi_block_t,
    mut elem: *const mi_block_t,
) -> bool {
    while !list.is_null() {
        if elem == list {
            return 1 as libc::c_int != 0;
        }
        list = mi_block_next(page, list);
    }
    return 0 as libc::c_int != 0;
}
#[inline(never)]
unsafe extern "C" fn mi_check_is_double_freex(
    mut page: *const mi_page_t,
    mut block: *const mi_block_t,
) -> bool {
    if mi_list_contains(page, (*page).free, block) as libc::c_int != 0
        || mi_list_contains(page, (*page).local_free, block) as libc::c_int != 0
        || mi_list_contains(page, mi_page_thread_free(page), block) as libc::c_int != 0
    {
        _mi_error_message(
            11 as libc::c_int,
            b"double free detected of block %p with size %zu\n\0" as *const u8
                as *const libc::c_char,
            block,
            mi_page_block_size(page),
        );
        return 1 as libc::c_int != 0;
    }
    return 0 as libc::c_int != 0;
}
#[inline]
unsafe extern "C" fn mi_check_is_double_free(
    mut page: *const mi_page_t,
    mut block: *const mi_block_t,
) -> bool {
    let mut is_double_free: bool = 0 as libc::c_int != 0;
    let mut n: *mut mi_block_t =
        mi_block_nextx(page as *const libc::c_void, block, ((*page).keys).as_ptr());
    if n as uintptr_t & (((1 as libc::c_int) << 3 as libc::c_int) - 1 as libc::c_int) as uintptr_t
        == 0 as libc::c_int as uintptr_t
        && (n.is_null()
            || mi_is_in_same_page(block as *const libc::c_void, n as *const libc::c_void)
                as libc::c_int
                != 0)
    {
        is_double_free = mi_check_is_double_freex(page, block);
    }
    return is_double_free;
}
unsafe extern "C" fn mi_page_decode_padding(
    mut page: *const mi_page_t,
    mut block: *const mi_block_t,
    mut delta: *mut size_t,
    mut bsize: *mut size_t,
) -> bool {
    *bsize = mi_page_usable_block_size(page);
    let padding: *const mi_padding_t =
        (block as *mut uint8_t).offset(*bsize as isize) as *mut mi_padding_t;
    *delta = (*padding).delta as size_t;
    let mut canary: uint32_t = (*padding).canary;
    let mut keys: [uintptr_t; 2] = [0; 2];
    keys[0 as libc::c_int as usize] = (*page).keys[0 as libc::c_int as usize];
    keys[1 as libc::c_int as usize] = (*page).keys[1 as libc::c_int as usize];
    let mut ok: bool = mi_ptr_encode_canary(
        page as *const libc::c_void,
        block as *const libc::c_void,
        keys.as_mut_ptr(),
    ) == canary
        && *delta <= *bsize;
    return ok;
}
unsafe extern "C" fn mi_page_usable_size_of(
    mut page: *const mi_page_t,
    mut block: *const mi_block_t,
) -> size_t {
    let mut bsize: size_t = 0;
    let mut delta: size_t = 0;
    let mut ok: bool = mi_page_decode_padding(page, block, &mut delta, &mut bsize);
    if ok as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"ok\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2652 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"mi_page_usable_size_of\0",
            ))
            .as_ptr(),
        );
    };
    if delta <= bsize {
    } else {
        _mi_assert_fail(
            b"delta <= bsize\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2652 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"mi_page_usable_size_of\0",
            ))
            .as_ptr(),
        );
    };
    return if ok as libc::c_int != 0 {
        bsize.wrapping_sub(delta)
    } else {
        0 as libc::c_int as size_t
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_padding_shrink(
    mut page: *const mi_page_t,
    mut block: *const mi_block_t,
    min_size: size_t,
) {
    let mut bsize: size_t = 0;
    let mut delta: size_t = 0;
    let mut ok: bool = mi_page_decode_padding(page, block, &mut delta, &mut bsize);
    if ok as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"ok\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2659 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_padding_shrink\0"))
                .as_ptr(),
        );
    };
    if !ok || bsize.wrapping_sub(delta) >= min_size {
        return;
    }
    if bsize >= min_size {
    } else {
        _mi_assert_fail(
            b"bsize >= min_size\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2661 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_padding_shrink\0"))
                .as_ptr(),
        );
    };
    if bsize < min_size {
        return;
    }
    let mut new_delta: size_t = bsize.wrapping_sub(min_size);
    if new_delta < bsize {
    } else {
        _mi_assert_fail(
            b"new_delta < bsize\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2664 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_padding_shrink\0"))
                .as_ptr(),
        );
    };
    let mut padding: *mut mi_padding_t =
        (block as *mut uint8_t).offset(bsize as isize) as *mut mi_padding_t;
    (*padding).delta = new_delta as uint32_t;
}
unsafe extern "C" fn mi_verify_padding(
    mut page: *const mi_page_t,
    mut block: *const mi_block_t,
    mut size: *mut size_t,
    mut wrong: *mut size_t,
) -> bool {
    let mut bsize: size_t = 0;
    let mut delta: size_t = 0;
    let mut ok: bool = mi_page_decode_padding(page, block, &mut delta, &mut bsize);
    *wrong = bsize;
    *size = *wrong;
    if !ok {
        return 0 as libc::c_int != 0;
    }
    if bsize >= delta {
    } else {
        _mi_assert_fail(
            b"bsize >= delta\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2688 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 18], &[libc::c_char; 18]>(b"mi_verify_padding\0"))
                .as_ptr(),
        );
    };
    *size = bsize.wrapping_sub(delta);
    if !mi_page_is_huge(page) {
        let mut fill: *mut uint8_t = (block as *mut uint8_t)
            .offset(bsize as isize)
            .offset(-(delta as isize));
        let maxpad: size_t = if delta > 16 as libc::c_int as size_t {
            16 as libc::c_int as size_t
        } else {
            delta
        };
        let mut i: size_t = 0 as libc::c_int as size_t;
        while i < maxpad {
            if *fill.offset(i as isize) as libc::c_int != 0xde as libc::c_int {
                *wrong = bsize.wrapping_sub(delta).wrapping_add(i);
                ok = 0 as libc::c_int != 0;
                break;
            } else {
                i = i.wrapping_add(1);
                i;
            }
        }
    }
    return ok;
}
unsafe extern "C" fn mi_check_padding(mut page: *const mi_page_t, mut block: *const mi_block_t) {
    let mut size: size_t = 0;
    let mut wrong: size_t = 0;
    if !mi_verify_padding(page, block, &mut size, &mut wrong) {
        _mi_error_message(
            14 as libc::c_int,
            b"buffer overflow in heap block %p of size %zu: write after %zu bytes\n\0" as *const u8
                as *const libc::c_char,
            block,
            size,
            wrong,
        );
    }
}
unsafe extern "C" fn mi_stat_free(mut page: *const mi_page_t, mut block: *const mi_block_t) {
    let heap: *mut mi_heap_t = mi_heap_get_default();
    let bsize: size_t = mi_page_usable_block_size(page);
    let usize: size_t = mi_page_usable_size_of(page, block);
    _mi_stat_decrease(&mut (*(*heap).tld).stats.malloc, usize);
    if bsize as libc::c_ulonglong
        <= ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
    {
        _mi_stat_decrease(&mut (*(*heap).tld).stats.normal, bsize);
        _mi_stat_decrease(
            &mut *((*(*heap).tld).stats.normal_bins)
                .as_mut_ptr()
                .offset((_mi_bin as unsafe extern "C" fn(size_t) -> uint8_t)(bsize) as isize),
            1 as libc::c_int as size_t,
        );
    } else {
        let bpsize: size_t = mi_page_block_size(page);
        _mi_stat_decrease(&mut (*(*heap).tld).stats.huge, bpsize);
    };
}
#[inline]
unsafe extern "C" fn _mi_page_malloc_zero(
    mut heap: *mut mi_heap_t,
    mut page: *mut mi_page_t,
    mut size: size_t,
    mut zero: bool,
) -> *mut libc::c_void {
    if (*page).block_size == 0 as libc::c_int as size_t || mi_page_block_size(page) >= size {
    } else {
        _mi_assert_fail(
            b"page->block_size == 0 || mi_page_block_size(page) >= size\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2764 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_page_malloc_zero\0"))
                .as_ptr(),
        );
    };
    let block: *mut mi_block_t = (*page).free;
    if block.is_null() as libc::c_int as libc::c_long != 0 {
        return _mi_malloc_generic(heap, size, zero, 0 as libc::c_int as size_t);
    }
    if !block.is_null() && _mi_ptr_page(block as *mut libc::c_void) == page {
    } else {
        _mi_assert_fail(
            b"block != NULL && _mi_ptr_page(block) == page\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2769 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_page_malloc_zero\0"))
                .as_ptr(),
        );
    };
    (*page).free = mi_block_next(page, block);
    (*page).used = ((*page).used).wrapping_add(1);
    (*page).used;
    if ((*page).free).is_null() || _mi_ptr_page((*page).free as *mut libc::c_void) == page {
    } else {
        _mi_assert_fail(
            b"page->free == NULL || _mi_ptr_page(page->free) == page\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2772 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_page_malloc_zero\0"))
                .as_ptr(),
        );
    };
    if (*page).block_size < 16 as libc::c_int as size_t
        || _mi_is_aligned(block as *mut libc::c_void, 16 as libc::c_int as size_t) as libc::c_int
            != 0
    {
    } else {
        _mi_assert_fail(
            b"page->block_size < MI_MAX_ALIGN_SIZE || _mi_is_aligned(block, MI_MAX_ALIGN_SIZE)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2773 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_page_malloc_zero\0"))
                .as_ptr(),
        );
    };
    if zero as libc::c_int as libc::c_long != 0 {
        if (*page).block_size != 0 as libc::c_int as size_t {
        } else {
            _mi_assert_fail(
                b"page->block_size != 0\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                2781 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"_mi_page_malloc_zero\0",
                ))
                .as_ptr(),
            );
        };
        if !mi_page_is_huge(page) {
        } else {
            _mi_assert_fail(
                b"!mi_page_is_huge(page)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                2782 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"_mi_page_malloc_zero\0",
                ))
                .as_ptr(),
            );
        };
        if (*page).block_size >= ::core::mem::size_of::<mi_padding_t>() as libc::c_ulong {
        } else {
            _mi_assert_fail(
                b"page->block_size >= MI_PADDING_SIZE\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                2784 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"_mi_page_malloc_zero\0",
                ))
                .as_ptr(),
            );
        };
        if (*page).free_is_zero() != 0 {
            (*block).next = 0 as libc::c_int as mi_encoded_t;
        } else {
            _mi_memzero_aligned(
                block as *mut libc::c_void,
                ((*page).block_size)
                    .wrapping_sub(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong),
            );
        }
    }
    if !zero && !mi_page_is_huge(page) {
        memset(
            block as *mut libc::c_void,
            0xd0 as libc::c_int,
            mi_page_usable_block_size(page),
        );
    }
    let bsize: size_t = mi_page_usable_block_size(page);
    if bsize as libc::c_ulonglong
        <= ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
    {
        _mi_stat_increase(&mut (*(*heap).tld).stats.normal, bsize);
        _mi_stat_counter_increase(
            &mut (*(*heap).tld).stats.normal_count,
            1 as libc::c_int as size_t,
        );
        let bin: size_t = _mi_bin(bsize) as size_t;
        _mi_stat_increase(
            &mut *((*(*heap).tld).stats.normal_bins)
                .as_mut_ptr()
                .offset(bin as isize),
            1 as libc::c_int as size_t,
        );
    }
    let padding: *mut mi_padding_t = (block as *mut uint8_t)
        .offset(mi_page_usable_block_size(page) as isize)
        as *mut mi_padding_t;
    let mut delta: ptrdiff_t = ((padding as *mut uint8_t).offset_from(block as *mut uint8_t)
        as libc::c_long as libc::c_ulong)
        .wrapping_sub(size.wrapping_sub(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong))
        as ptrdiff_t;
    if delta >= 0 as libc::c_int as ptrdiff_t
        && mi_page_usable_block_size(page)
            >= size
                .wrapping_sub(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong)
                .wrapping_add(delta as libc::c_ulong)
    {
    } else {
        _mi_assert_fail(
            b"delta >= 0 && mi_page_usable_block_size(page) >= (size - MI_PADDING_SIZE + delta)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2816 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_page_malloc_zero\0"))
                .as_ptr(),
        );
    };
    (*padding).canary = mi_ptr_encode_canary(
        page as *const libc::c_void,
        block as *const libc::c_void,
        ((*page).keys).as_mut_ptr(),
    );
    (*padding).delta = delta as uint32_t;
    if !mi_page_is_huge(page) {
        let mut fill: *mut uint8_t = (padding as *mut uint8_t).offset(-(delta as isize));
        let maxpad: size_t = (if delta > 16 as libc::c_int as ptrdiff_t {
            16 as libc::c_int as ptrdiff_t
        } else {
            delta
        }) as size_t;
        let mut i: size_t = 0 as libc::c_int as size_t;
        while i < maxpad {
            *fill.offset(i as isize) = 0xde as libc::c_int as uint8_t;
            i = i.wrapping_add(1);
            i;
        }
    }
    return block as *mut libc::c_void;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_malloc(
    mut heap: *mut mi_heap_t,
    mut page: *mut mi_page_t,
    mut size: size_t,
) -> *mut libc::c_void {
    return _mi_page_malloc_zero(heap, page, size, 0 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_malloc_zeroed(
    mut heap: *mut mi_heap_t,
    mut page: *mut mi_page_t,
    mut size: size_t,
) -> *mut libc::c_void {
    return _mi_page_malloc_zero(heap, page, size, 1 as libc::c_int != 0);
}
#[inline]
unsafe extern "C" fn mi_heap_malloc_small_zero(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut zero: bool,
) -> *mut libc::c_void {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2841 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_heap_malloc_small_zero\0",
            ))
            .as_ptr(),
        );
    };
    if size
        <= (128 as libc::c_int as libc::c_ulong)
            .wrapping_mul(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong)
    {
    } else {
        _mi_assert_fail(
            b"size <= MI_SMALL_SIZE_MAX\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2842 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_heap_malloc_small_zero\0",
            ))
            .as_ptr(),
        );
    };
    let tid: uintptr_t = _mi_thread_id();
    if (*heap).thread_id == 0 as libc::c_int as mi_threadid_t || (*heap).thread_id == tid {
    } else {
        _mi_assert_fail(
            b"heap->thread_id == 0 || heap->thread_id == tid\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            2845 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_heap_malloc_small_zero\0",
            ))
            .as_ptr(),
        );
    };
    if size == 0 as libc::c_int as size_t {
        size = ::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong;
    }
    let mut page: *mut mi_page_t = _mi_heap_get_free_small_page(
        heap,
        size.wrapping_add(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong),
    );
    let p: *mut libc::c_void = _mi_page_malloc_zero(
        heap,
        page,
        size.wrapping_add(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong),
        zero,
    );
    if !p.is_null() {
        if mi_usable_size(p) == size {
        } else {
            _mi_assert_fail(
                b"mi_usable_size(p)==(size)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                2857 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                    b"mi_heap_malloc_small_zero\0",
                ))
                .as_ptr(),
            );
        };
    }
    if !p.is_null() {
        if !mi_heap_is_initialized(heap) {
            heap = mi_prim_get_default_heap();
        }
        _mi_stat_increase(&mut (*(*heap).tld).stats.malloc, mi_usable_size(p));
    }
    return p;
}
#[inline]
unsafe extern "C" fn _mi_heap_malloc_zero_ex(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut zero: bool,
    mut huge_alignment: size_t,
) -> *mut libc::c_void {
    if (size
        <= (128 as libc::c_int as libc::c_ulong)
            .wrapping_mul(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong))
        as libc::c_int as libc::c_long
        != 0
    {
        if huge_alignment == 0 as libc::c_int as size_t {
        } else {
            _mi_assert_fail(
                b"huge_alignment == 0\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                2879 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                    b"_mi_heap_malloc_zero_ex\0",
                ))
                .as_ptr(),
            );
        };
        return mi_heap_malloc_small_zero(heap, size, zero);
    } else {
        if !heap.is_null() {
        } else {
            _mi_assert_fail(
                b"heap!=NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                2888 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                    b"_mi_heap_malloc_zero_ex\0",
                ))
                .as_ptr(),
            );
        };
        if (*heap).thread_id == 0 as libc::c_int as mi_threadid_t
            || (*heap).thread_id == _mi_thread_id()
        {
        } else {
            _mi_assert_fail(
                b"heap->thread_id == 0 || heap->thread_id == _mi_thread_id()\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                2889 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                    b"_mi_heap_malloc_zero_ex\0",
                ))
                .as_ptr(),
            );
        };
        let p: *mut libc::c_void = _mi_malloc_generic(
            heap,
            size.wrapping_add(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong),
            zero,
            huge_alignment,
        );
        if !p.is_null() {
            if mi_usable_size(p) == size {
            } else {
                _mi_assert_fail(
                    b"mi_usable_size(p)==(size)\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    2891 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                        b"_mi_heap_malloc_zero_ex\0",
                    ))
                    .as_ptr(),
                );
            };
        }
        if !p.is_null() {
            if !mi_heap_is_initialized(heap) {
                heap = mi_prim_get_default_heap();
            }
            _mi_stat_increase(&mut (*(*heap).tld).stats.malloc, mi_usable_size(p));
        }
        return p;
    };
}
#[inline]
unsafe extern "C" fn _mi_heap_malloc_zero(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut zero: bool,
) -> *mut libc::c_void {
    return _mi_heap_malloc_zero_ex(heap, size, zero, 0 as libc::c_int as size_t);
}
#[inline]
unsafe extern "C" fn mi_heap_malloc(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
) -> *mut libc::c_void {
    return _mi_heap_malloc_zero(heap, size, 0 as libc::c_int != 0);
}
#[inline]
unsafe extern "C" fn mi_malloc(mut size: size_t) -> *mut libc::c_void {
    return mi_heap_malloc(mi_prim_get_default_heap(), size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_zalloc_small(mut size: size_t) -> *mut libc::c_void {
    return mi_heap_malloc_small_zero(mi_prim_get_default_heap(), size, 1 as libc::c_int != 0);
}
#[inline]
unsafe extern "C" fn mi_heap_zalloc(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
) -> *mut libc::c_void {
    return _mi_heap_malloc_zero(heap, size, 1 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_zalloc(mut size: size_t) -> *mut libc::c_void {
    return mi_heap_zalloc(mi_prim_get_default_heap(), size);
}
#[inline]
unsafe extern "C" fn mi_heap_calloc(
    mut heap: *mut mi_heap_t,
    mut count: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    let mut total: size_t = 0;
    if mi_count_size_overflow(count, size, &mut total) {
        return 0 as *mut libc::c_void;
    }
    return mi_heap_zalloc(heap, total);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_calloc(mut count: size_t, mut size: size_t) -> *mut libc::c_void {
    return mi_heap_calloc(mi_prim_get_default_heap(), count, size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_mallocn(
    mut heap: *mut mi_heap_t,
    mut count: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    let mut total: size_t = 0;
    if mi_count_size_overflow(count, size, &mut total) {
        return 0 as *mut libc::c_void;
    }
    return mi_heap_malloc(heap, total);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_mallocn(mut count: size_t, mut size: size_t) -> *mut libc::c_void {
    return mi_heap_mallocn(mi_prim_get_default_heap(), count, size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_expand(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
) -> *mut libc::c_void {
    return 0 as *mut libc::c_void;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_realloc_zero(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut zero: bool,
) -> *mut libc::c_void {
    let size: size_t = _mi_usable_size(p, b"mi_realloc\0" as *const u8 as *const libc::c_char);
    if (newsize <= size
        && newsize >= size / 2 as libc::c_int as size_t
        && newsize > 0 as libc::c_int as size_t) as libc::c_int as libc::c_long
        != 0
    {
        if !p.is_null() {
        } else {
            _mi_assert_fail(
                b"p!=NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                2954 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"_mi_heap_realloc_zero\0",
                ))
                .as_ptr(),
            );
        };
        return p;
    }
    let mut newp: *mut libc::c_void = mi_heap_malloc(heap, newsize);
    if !newp.is_null() as libc::c_int as libc::c_long != 0 {
        if zero as libc::c_int != 0 && newsize > size {
            let start: size_t = if size >= ::core::mem::size_of::<intptr_t>() as libc::c_ulong {
                size.wrapping_sub(::core::mem::size_of::<intptr_t>() as libc::c_ulong)
            } else {
                0 as libc::c_int as libc::c_ulong
            };
            _mi_memzero(
                (newp as *mut uint8_t).offset(start as isize) as *mut libc::c_void,
                newsize.wrapping_sub(start),
            );
        } else if newsize == 0 as libc::c_int as size_t {
            *(newp as *mut uint8_t).offset(0 as libc::c_int as isize) = 0 as libc::c_int as uint8_t;
        }
        if !p.is_null() as libc::c_int as libc::c_long != 0 {
            let copysize: size_t = if newsize > size { size } else { newsize };
            _mi_memcpy(newp, p, copysize);
            mi_free(p);
        }
    }
    return newp;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_realloc(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
) -> *mut libc::c_void {
    return _mi_heap_realloc_zero(heap, p, newsize, 0 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_reallocn(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut count: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    let mut total: size_t = 0;
    if mi_count_size_overflow(count, size, &mut total) {
        return 0 as *mut libc::c_void;
    }
    return mi_heap_realloc(heap, p, total);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_reallocf(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
) -> *mut libc::c_void {
    let mut newp: *mut libc::c_void = mi_heap_realloc(heap, p, newsize);
    if newp.is_null() && !p.is_null() {
        mi_free(p);
    }
    return newp;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_rezalloc(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
) -> *mut libc::c_void {
    return _mi_heap_realloc_zero(heap, p, newsize, 1 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_recalloc(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut count: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    let mut total: size_t = 0;
    if mi_count_size_overflow(count, size, &mut total) {
        return 0 as *mut libc::c_void;
    }
    return mi_heap_rezalloc(heap, p, total);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_realloc(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
) -> *mut libc::c_void {
    return mi_heap_realloc(mi_prim_get_default_heap(), p, newsize);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reallocn(
    mut p: *mut libc::c_void,
    mut count: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    return mi_heap_reallocn(mi_prim_get_default_heap(), p, count, size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reallocf(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
) -> *mut libc::c_void {
    return mi_heap_reallocf(mi_prim_get_default_heap(), p, newsize);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_rezalloc(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
) -> *mut libc::c_void {
    return mi_heap_rezalloc(mi_prim_get_default_heap(), p, newsize);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_recalloc(
    mut p: *mut libc::c_void,
    mut count: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    return mi_heap_recalloc(mi_prim_get_default_heap(), p, count, size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_strdup(
    mut heap: *mut mi_heap_t,
    mut s: *const libc::c_char,
) -> *mut libc::c_char {
    if s.is_null() {
        return 0 as *mut libc::c_char;
    }
    let mut len: size_t = _mi_strlen(s);
    let mut t: *mut libc::c_char =
        mi_heap_malloc(heap, len.wrapping_add(1 as libc::c_int as size_t)) as *mut libc::c_char;
    if t.is_null() {
        return 0 as *mut libc::c_char;
    }
    _mi_memcpy(t as *mut libc::c_void, s as *const libc::c_void, len);
    *t.offset(len as isize) = 0 as libc::c_int as libc::c_char;
    return t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_strdup(mut s: *const libc::c_char) -> *mut libc::c_char {
    return mi_heap_strdup(mi_prim_get_default_heap(), s);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_strndup(
    mut heap: *mut mi_heap_t,
    mut s: *const libc::c_char,
    mut n: size_t,
) -> *mut libc::c_char {
    if s.is_null() {
        return 0 as *mut libc::c_char;
    }
    let len: size_t = _mi_strnlen(s, n);
    let mut t: *mut libc::c_char =
        mi_heap_malloc(heap, len.wrapping_add(1 as libc::c_int as size_t)) as *mut libc::c_char;
    if t.is_null() {
        return 0 as *mut libc::c_char;
    }
    _mi_memcpy(t as *mut libc::c_void, s as *const libc::c_void, len);
    *t.offset(len as isize) = 0 as libc::c_int as libc::c_char;
    return t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_strndup(
    mut s: *const libc::c_char,
    mut n: size_t,
) -> *mut libc::c_char {
    return mi_heap_strndup(mi_prim_get_default_heap(), s, n);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_realpath(
    mut heap: *mut mi_heap_t,
    mut fname: *const libc::c_char,
    mut resolved_name: *mut libc::c_char,
) -> *mut libc::c_char {
    if !resolved_name.is_null() {
        return realpath(fname, resolved_name);
    } else {
        let mut rname: *mut libc::c_char = realpath(fname, 0 as *mut libc::c_char);
        if rname.is_null() {
            return 0 as *mut libc::c_char;
        }
        let mut result: *mut libc::c_char = mi_heap_strdup(heap, rname);
        mi_cfree(rname as *mut libc::c_void);
        return result;
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_realpath(
    mut fname: *const libc::c_char,
    mut resolved_name: *mut libc::c_char,
) -> *mut libc::c_char {
    return mi_heap_realpath(mi_prim_get_default_heap(), fname, resolved_name);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _ZSt15get_new_handlerv() -> std_new_handler_t {
    return None;
}
unsafe extern "C" fn mi_get_new_handler() -> std_new_handler_t {
    return _ZSt15get_new_handlerv();
}
unsafe extern "C" fn mi_try_new_handler(mut nothrow: bool) -> bool {
    let mut h: std_new_handler_t = mi_get_new_handler();
    if h.is_none() {
        _mi_error_message(
            12 as libc::c_int,
            b"out of memory in 'new'\0" as *const u8 as *const libc::c_char,
        );
        if !nothrow {
            abort();
        }
        return 0 as libc::c_int != 0;
    } else {
        h.expect("non-null function pointer")();
        return 1 as libc::c_int != 0;
    };
}
#[unsafe(no_mangle)]
#[inline(never)]
pub unsafe extern "C" fn mi_heap_try_new(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut nothrow: bool,
) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = 0 as *mut libc::c_void;
    while p.is_null() && mi_try_new_handler(nothrow) as libc::c_int != 0 {
        p = mi_heap_malloc(heap, size);
    }
    return p;
}
#[inline(never)]
unsafe extern "C" fn mi_try_new(mut size: size_t, mut nothrow: bool) -> *mut libc::c_void {
    return mi_heap_try_new(mi_prim_get_default_heap(), size, nothrow);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_alloc_new(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = mi_heap_malloc(heap, size);
    if p.is_null() as libc::c_int as libc::c_long != 0 {
        return mi_heap_try_new(heap, size, 0 as libc::c_int != 0);
    }
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_new(mut size: size_t) -> *mut libc::c_void {
    return mi_heap_alloc_new(mi_prim_get_default_heap(), size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_alloc_new_n(
    mut heap: *mut mi_heap_t,
    mut count: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    let mut total: size_t = 0;
    if mi_count_size_overflow(count, size, &mut total) as libc::c_int as libc::c_long != 0 {
        mi_try_new_handler(0 as libc::c_int != 0);
        return 0 as *mut libc::c_void;
    } else {
        return mi_heap_alloc_new(heap, total);
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_new_n(mut count: size_t, mut size: size_t) -> *mut libc::c_void {
    return mi_heap_alloc_new_n(mi_prim_get_default_heap(), count, size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_new_nothrow(mut size: size_t) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = mi_malloc(size);
    if p.is_null() as libc::c_int as libc::c_long != 0 {
        return mi_try_new(size, 1 as libc::c_int != 0);
    }
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_new_aligned(
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = 0 as *mut libc::c_void;
    loop {
        p = mi_malloc_aligned(size, alignment);
        if !(p.is_null() && mi_try_new_handler(0 as libc::c_int != 0) as libc::c_int != 0) {
            break;
        }
    }
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_new_aligned_nothrow(
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = 0 as *mut libc::c_void;
    loop {
        p = mi_malloc_aligned(size, alignment);
        if !(p.is_null() && mi_try_new_handler(1 as libc::c_int != 0) as libc::c_int != 0) {
            break;
        }
    }
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_new_realloc(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
) -> *mut libc::c_void {
    let mut q: *mut libc::c_void = 0 as *mut libc::c_void;
    loop {
        q = mi_realloc(p, newsize);
        if !(q.is_null() && mi_try_new_handler(0 as libc::c_int != 0) as libc::c_int != 0) {
            break;
        }
    }
    return q;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_new_reallocn(
    mut p: *mut libc::c_void,
    mut newcount: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    let mut total: size_t = 0;
    if mi_count_size_overflow(newcount, size, &mut total) as libc::c_int as libc::c_long != 0 {
        mi_try_new_handler(0 as libc::c_int != 0);
        return 0 as *mut libc::c_void;
    } else {
        return mi_new_realloc(p, total);
    };
}
unsafe extern "C" fn mi_malloc_is_naturally_aligned(
    mut size: size_t,
    mut alignment: size_t,
) -> bool {
    if _mi_is_power_of_two(alignment) as libc::c_int != 0 && alignment > 0 as libc::c_int as size_t
    {
    } else {
        _mi_assert_fail(
            b"_mi_is_power_of_two(alignment) && (alignment > 0)\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3242 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                b"mi_malloc_is_naturally_aligned\0",
            ))
            .as_ptr(),
        );
    };
    if alignment > size {
        return 0 as libc::c_int != 0;
    }
    if alignment <= 16 as libc::c_int as size_t {
        return 1 as libc::c_int != 0;
    }
    let bsize: size_t = mi_good_size(size);
    return bsize as libc::c_ulonglong
        <= ((1 as libc::c_ulonglong) << 3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
            .wrapping_div(4 as libc::c_int as libc::c_ulonglong)
        && bsize & alignment.wrapping_sub(1 as libc::c_int as size_t)
            == 0 as libc::c_int as size_t;
}
unsafe extern "C" fn mi_heap_malloc_zero_no_guarded(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut zero: bool,
) -> *mut libc::c_void {
    return _mi_heap_malloc_zero(heap, size, zero);
}
#[inline(never)]
unsafe extern "C" fn mi_heap_malloc_zero_aligned_at_overalloc(
    heap: *mut mi_heap_t,
    size: size_t,
    alignment: size_t,
    offset: size_t,
    zero: bool,
) -> *mut libc::c_void {
    if size
        <= (9223372036854775807 as libc::c_long as libc::c_ulong)
            .wrapping_sub(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong)
    {
    } else {
        _mi_assert_fail(
            b"size <= (MI_MAX_ALLOC_SIZE - MI_PADDING_SIZE)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3273 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 41], &[libc::c_char; 41]>(
                b"mi_heap_malloc_zero_aligned_at_overalloc\0",
            ))
            .as_ptr(),
        );
    };
    if alignment != 0 as libc::c_int as size_t && _mi_is_power_of_two(alignment) as libc::c_int != 0
    {
    } else {
        _mi_assert_fail(
            b"alignment != 0 && _mi_is_power_of_two(alignment)\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3274 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 41], &[libc::c_char; 41]>(
                b"mi_heap_malloc_zero_aligned_at_overalloc\0",
            ))
            .as_ptr(),
        );
    };
    let mut p: *mut libc::c_void = 0 as *mut libc::c_void;
    let mut oversize: size_t = 0;
    if (alignment as libc::c_ulonglong
        > (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
            >> 1 as libc::c_int) as libc::c_int as libc::c_long
        != 0
    {
        if (offset != 0 as libc::c_int as size_t) as libc::c_int as libc::c_long != 0 {
            _mi_error_message(
                75 as libc::c_int,
                b"aligned allocation with a very large alignment cannot be used with an alignment offset (size %zu, alignment %zu, offset %zu)\n\0"
                    as *const u8 as *const libc::c_char,
                size,
                alignment,
                offset,
            );
            return 0 as *mut libc::c_void;
        }
        oversize = if size
            <= (128 as libc::c_int as libc::c_ulong)
                .wrapping_mul(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong)
        {
            (128 as libc::c_int as libc::c_ulong)
                .wrapping_mul(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong)
                .wrapping_add(1 as libc::c_int as libc::c_ulong)
        } else {
            size
        };
        p = _mi_heap_malloc_zero_ex(heap, oversize, 0 as libc::c_int != 0, alignment);
        if p.is_null() {
            return 0 as *mut libc::c_void;
        }
    } else {
        oversize = size
            .wrapping_add(alignment)
            .wrapping_sub(1 as libc::c_int as size_t);
        p = mi_heap_malloc_zero_no_guarded(heap, oversize, zero);
        if p.is_null() {
            return 0 as *mut libc::c_void;
        }
    }
    let mut page: *mut mi_page_t = _mi_ptr_page(p);
    let align_mask: uintptr_t = alignment.wrapping_sub(1 as libc::c_int as size_t);
    let poffset: uintptr_t = (p as uintptr_t).wrapping_add(offset) & align_mask;
    let adjust: uintptr_t = if poffset == 0 as libc::c_int as uintptr_t {
        0 as libc::c_int as libc::c_ulong
    } else {
        alignment.wrapping_sub(poffset)
    };
    if adjust < alignment {
    } else {
        _mi_assert_fail(
            b"adjust < alignment\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3297 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 41], &[libc::c_char; 41]>(
                b"mi_heap_malloc_zero_aligned_at_overalloc\0",
            ))
            .as_ptr(),
        );
    };
    let mut aligned_p: *mut libc::c_void =
        (p as uintptr_t).wrapping_add(adjust) as *mut libc::c_void;
    if aligned_p != p {
        mi_page_set_has_aligned(page, 1 as libc::c_int != 0);
        _mi_padding_shrink(page, p as *mut mi_block_t, adjust.wrapping_add(size));
    }
    if mi_page_usable_block_size(page) >= adjust.wrapping_add(size) {
    } else {
        _mi_assert_fail(
            b"mi_page_usable_block_size(page) >= adjust + size\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3309 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 41], &[libc::c_char; 41]>(
                b"mi_heap_malloc_zero_aligned_at_overalloc\0",
            ))
            .as_ptr(),
        );
    };
    if (aligned_p as uintptr_t)
        .wrapping_add(offset)
        .wrapping_rem(alignment)
        == 0 as libc::c_int as libc::c_ulong
    {
    } else {
        _mi_assert_fail(
            b"((uintptr_t)aligned_p + offset) % alignment == 0\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3310 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 41], &[libc::c_char; 41]>(
                b"mi_heap_malloc_zero_aligned_at_overalloc\0",
            ))
            .as_ptr(),
        );
    };
    if mi_usable_size(aligned_p) >= size {
    } else {
        _mi_assert_fail(
            b"mi_usable_size(aligned_p)>=size\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3311 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 41], &[libc::c_char; 41]>(
                b"mi_heap_malloc_zero_aligned_at_overalloc\0",
            ))
            .as_ptr(),
        );
    };
    if mi_usable_size(p) == (mi_usable_size(aligned_p)).wrapping_add(adjust) {
    } else {
        _mi_assert_fail(
            b"mi_usable_size(p) == mi_usable_size(aligned_p)+adjust\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3312 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 41], &[libc::c_char; 41]>(
                b"mi_heap_malloc_zero_aligned_at_overalloc\0",
            ))
            .as_ptr(),
        );
    };
    let apage: *mut mi_page_t = _mi_ptr_page(aligned_p);
    let mut unalign_p: *mut libc::c_void =
        _mi_page_ptr_unalign(apage, aligned_p) as *mut libc::c_void;
    if p == unalign_p {
    } else {
        _mi_assert_fail(
            b"p == unalign_p\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3316 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 41], &[libc::c_char; 41]>(
                b"mi_heap_malloc_zero_aligned_at_overalloc\0",
            ))
            .as_ptr(),
        );
    };
    if alignment as libc::c_ulonglong
        > (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
            >> 1 as libc::c_int
    {
        if zero {
            _mi_memzero_aligned(aligned_p, mi_usable_size(aligned_p));
        }
    }
    p != aligned_p;
    return aligned_p;
}
#[inline(never)]
unsafe extern "C" fn mi_heap_malloc_zero_aligned_at_generic(
    heap: *mut mi_heap_t,
    size: size_t,
    alignment: size_t,
    offset: size_t,
    zero: bool,
) -> *mut libc::c_void {
    if alignment != 0 as libc::c_int as size_t && _mi_is_power_of_two(alignment) as libc::c_int != 0
    {
    } else {
        _mi_assert_fail(
            b"alignment != 0 && _mi_is_power_of_two(alignment)\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3334 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 39], &[libc::c_char; 39]>(
                b"mi_heap_malloc_zero_aligned_at_generic\0",
            ))
            .as_ptr(),
        );
    };
    if (size
        > (9223372036854775807 as libc::c_long as libc::c_ulong)
            .wrapping_sub(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong))
        as libc::c_int as libc::c_long
        != 0
    {
        _mi_error_message(
            75 as libc::c_int,
            b"aligned allocation request is too large (size %zu, alignment %zu)\n\0" as *const u8
                as *const libc::c_char,
            size,
            alignment,
        );
        return 0 as *mut libc::c_void;
    }
    if offset == 0 as libc::c_int as size_t
        && mi_malloc_is_naturally_aligned(size, alignment) as libc::c_int != 0
    {
        let mut p: *mut libc::c_void = mi_heap_malloc_zero_no_guarded(heap, size, zero);
        if p.is_null()
            || (p as uintptr_t).wrapping_rem(alignment) == 0 as libc::c_int as libc::c_ulong
        {
        } else {
            _mi_assert_fail(
                b"p == NULL || ((uintptr_t)p % alignment) == 0\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                3343 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 39], &[libc::c_char; 39]>(
                    b"mi_heap_malloc_zero_aligned_at_generic\0",
                ))
                .as_ptr(),
            );
        };
        let is_aligned_or_null: bool = p as uintptr_t
            & alignment.wrapping_sub(1 as libc::c_int as size_t)
            == 0 as libc::c_int as libc::c_ulong;
        if is_aligned_or_null as libc::c_int as libc::c_long != 0 {
            return p;
        } else {
            if 0 as libc::c_int != 0 {
            } else {
                _mi_assert_fail(
                    b"false\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    3349 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 39], &[libc::c_char; 39]>(
                        b"mi_heap_malloc_zero_aligned_at_generic\0",
                    ))
                    .as_ptr(),
                );
            };
            mi_free(p);
        }
    }
    return mi_heap_malloc_zero_aligned_at_overalloc(heap, size, alignment, offset, zero);
}
unsafe extern "C" fn mi_heap_malloc_zero_aligned_at(
    heap: *mut mi_heap_t,
    size: size_t,
    alignment: size_t,
    offset: size_t,
    zero: bool,
) -> *mut libc::c_void {
    if (alignment == 0 as libc::c_int as size_t || !_mi_is_power_of_two(alignment)) as libc::c_int
        as libc::c_long
        != 0
    {
        _mi_error_message(
            75 as libc::c_int,
            b"aligned allocation requires the alignment to be a power-of-two (size %zu, alignment %zu)\n\0"
                as *const u8 as *const libc::c_char,
            size,
            alignment,
        );
        return 0 as *mut libc::c_void;
    }
    if (size
        <= (128 as libc::c_int as libc::c_ulong)
            .wrapping_mul(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong)
        && alignment <= size) as libc::c_int as libc::c_long
        != 0
    {
        let align_mask: uintptr_t = alignment.wrapping_sub(1 as libc::c_int as size_t);
        let padsize: size_t =
            size.wrapping_add(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong);
        let mut page: *mut mi_page_t = _mi_heap_get_free_small_page(heap, padsize);
        if !((*page).free).is_null() as libc::c_int as libc::c_long != 0 {
            let is_aligned: bool = ((*page).free as uintptr_t).wrapping_add(offset) & align_mask
                == 0 as libc::c_int as libc::c_ulong;
            if is_aligned as libc::c_int as libc::c_long != 0 {
                _mi_stat_increase(&mut (*(*heap).tld).stats.malloc, size);
                let mut p: *mut libc::c_void = if zero as libc::c_int != 0 {
                    _mi_page_malloc_zeroed(heap, page, padsize)
                } else {
                    _mi_page_malloc(heap, page, padsize)
                };
                if !p.is_null() {
                } else {
                    _mi_assert_fail(
                        b"p != NULL\0" as *const u8 as *const libc::c_char,
                        b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                        3380 as libc::c_int as libc::c_uint,
                        (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                            b"mi_heap_malloc_zero_aligned_at\0",
                        ))
                        .as_ptr(),
                    );
                };
                if (p as uintptr_t)
                    .wrapping_add(offset)
                    .wrapping_rem(alignment)
                    == 0 as libc::c_int as libc::c_ulong
                {
                } else {
                    _mi_assert_fail(
                        b"((uintptr_t)p + offset) % alignment == 0\0" as *const u8
                            as *const libc::c_char,
                        b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                        3381 as libc::c_int as libc::c_uint,
                        (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                            b"mi_heap_malloc_zero_aligned_at\0",
                        ))
                        .as_ptr(),
                    );
                };
                if !p.is_null() {
                    if mi_usable_size(p) == size {
                    } else {
                        _mi_assert_fail(
                            b"mi_usable_size(p)==(size)\0" as *const u8 as *const libc::c_char,
                            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                            3382 as libc::c_int as libc::c_uint,
                            (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                                b"mi_heap_malloc_zero_aligned_at\0",
                            ))
                            .as_ptr(),
                        );
                    };
                }
                return p;
            }
        }
    }
    return mi_heap_malloc_zero_aligned_at_generic(heap, size, alignment, offset, zero);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_malloc_aligned_at(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_malloc_zero_aligned_at(heap, size, alignment, offset, 0 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_malloc_aligned(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_malloc_aligned_at(heap, size, alignment, 0 as libc::c_int as size_t);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_zalloc_aligned_at(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_malloc_zero_aligned_at(heap, size, alignment, offset, 1 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_zalloc_aligned(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_zalloc_aligned_at(heap, size, alignment, 0 as libc::c_int as size_t);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_calloc_aligned_at(
    mut heap: *mut mi_heap_t,
    mut count: size_t,
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    let mut total: size_t = 0;
    if mi_count_size_overflow(count, size, &mut total) {
        return 0 as *mut libc::c_void;
    }
    return mi_heap_zalloc_aligned_at(heap, total, alignment, offset);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_calloc_aligned(
    mut heap: *mut mi_heap_t,
    mut count: size_t,
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_calloc_aligned_at(heap, count, size, alignment, 0 as libc::c_int as size_t);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_malloc_aligned_at(
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_malloc_aligned_at(mi_prim_get_default_heap(), size, alignment, offset);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_malloc_aligned(
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_malloc_aligned(mi_prim_get_default_heap(), size, alignment);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_zalloc_aligned_at(
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_zalloc_aligned_at(mi_prim_get_default_heap(), size, alignment, offset);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_zalloc_aligned(
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_zalloc_aligned(mi_prim_get_default_heap(), size, alignment);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_calloc_aligned_at(
    mut count: size_t,
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_calloc_aligned_at(mi_prim_get_default_heap(), count, size, alignment, offset);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_calloc_aligned(
    mut count: size_t,
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_calloc_aligned(mi_prim_get_default_heap(), count, size, alignment);
}
unsafe extern "C" fn mi_heap_realloc_zero_aligned_at(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
    mut offset: size_t,
    mut zero: bool,
) -> *mut libc::c_void {
    if alignment > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"alignment > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3428 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                b"mi_heap_realloc_zero_aligned_at\0",
            ))
            .as_ptr(),
        );
    };
    if alignment <= ::core::mem::size_of::<uintptr_t>() as libc::c_ulong {
        return _mi_heap_realloc_zero(heap, p, newsize, zero);
    }
    if p.is_null() {
        return mi_heap_malloc_zero_aligned_at(heap, newsize, alignment, offset, zero);
    }
    let mut size: size_t = mi_usable_size(p);
    if newsize <= size
        && newsize >= size.wrapping_sub(size / 2 as libc::c_int as size_t)
        && (p as uintptr_t)
            .wrapping_add(offset)
            .wrapping_rem(alignment)
            == 0 as libc::c_int as libc::c_ulong
    {
        return p;
    } else {
        let mut newp: *mut libc::c_void =
            mi_heap_malloc_aligned_at(heap, newsize, alignment, offset);
        if !newp.is_null() {
            if zero as libc::c_int != 0 && newsize > size {
                let mut start: size_t =
                    if size >= ::core::mem::size_of::<intptr_t>() as libc::c_ulong {
                        size.wrapping_sub(::core::mem::size_of::<intptr_t>() as libc::c_ulong)
                    } else {
                        0 as libc::c_int as libc::c_ulong
                    };
                _mi_memzero(
                    (newp as *mut uint8_t).offset(start as isize) as *mut libc::c_void,
                    newsize.wrapping_sub(start),
                );
            }
            _mi_memcpy_aligned(newp, p, if newsize > size { size } else { newsize });
            mi_free(p);
        }
        return newp;
    };
}
unsafe extern "C" fn mi_heap_realloc_zero_aligned(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
    mut zero: bool,
) -> *mut libc::c_void {
    if alignment > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"alignment > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3450 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 29], &[libc::c_char; 29]>(
                b"mi_heap_realloc_zero_aligned\0",
            ))
            .as_ptr(),
        );
    };
    if alignment <= ::core::mem::size_of::<uintptr_t>() as libc::c_ulong {
        return _mi_heap_realloc_zero(heap, p, newsize, zero);
    }
    let mut offset: size_t = (p as uintptr_t).wrapping_rem(alignment);
    return mi_heap_realloc_zero_aligned_at(heap, p, newsize, alignment, offset, zero);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_realloc_aligned_at(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_realloc_zero_aligned_at(
        heap,
        p,
        newsize,
        alignment,
        offset,
        0 as libc::c_int != 0,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_realloc_aligned(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_realloc_zero_aligned(heap, p, newsize, alignment, 0 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_rezalloc_aligned_at(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_realloc_zero_aligned_at(
        heap,
        p,
        newsize,
        alignment,
        offset,
        1 as libc::c_int != 0,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_rezalloc_aligned(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_realloc_zero_aligned(heap, p, newsize, alignment, 1 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_recalloc_aligned_at(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newcount: size_t,
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    let mut total: size_t = 0;
    if mi_count_size_overflow(newcount, size, &mut total) {
        return 0 as *mut libc::c_void;
    }
    return mi_heap_rezalloc_aligned_at(heap, p, total, alignment, offset);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_recalloc_aligned(
    mut heap: *mut mi_heap_t,
    mut p: *mut libc::c_void,
    mut newcount: size_t,
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    let mut total: size_t = 0;
    if mi_count_size_overflow(newcount, size, &mut total) {
        return 0 as *mut libc::c_void;
    }
    return mi_heap_rezalloc_aligned(heap, p, total, alignment);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_realloc_aligned_at(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_realloc_aligned_at(mi_prim_get_default_heap(), p, newsize, alignment, offset);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_realloc_aligned(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_realloc_aligned(mi_prim_get_default_heap(), p, newsize, alignment);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_rezalloc_aligned_at(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_rezalloc_aligned_at(mi_prim_get_default_heap(), p, newsize, alignment, offset);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_rezalloc_aligned(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_rezalloc_aligned(mi_prim_get_default_heap(), p, newsize, alignment);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_recalloc_aligned_at(
    mut p: *mut libc::c_void,
    mut newcount: size_t,
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_heap_recalloc_aligned_at(
        mi_prim_get_default_heap(),
        p,
        newcount,
        size,
        alignment,
        offset,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_recalloc_aligned(
    mut p: *mut libc::c_void,
    mut newcount: size_t,
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_heap_recalloc_aligned(mi_prim_get_default_heap(), p, newcount, size, alignment);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_malloc_size(mut p: *const libc::c_void) -> size_t {
    return mi_usable_size(p);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_malloc_usable_size(mut p: *const libc::c_void) -> size_t {
    return mi_usable_size(p);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_malloc_good_size(mut size: size_t) -> size_t {
    return mi_good_size(size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_cfree(mut p: *mut libc::c_void) {
    if mi_is_in_heap_region(p) {
        mi_free(p);
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_posix_memalign(
    mut p: *mut *mut libc::c_void,
    mut alignment: size_t,
    mut size: size_t,
) -> libc::c_int {
    if p.is_null() {
        return 22 as libc::c_int;
    }
    if alignment.wrapping_rem(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong)
        != 0 as libc::c_int as libc::c_ulong
    {
        return 22 as libc::c_int;
    }
    if alignment == 0 as libc::c_int as size_t || !_mi_is_power_of_two(alignment) {
        return 22 as libc::c_int;
    }
    let mut q: *mut libc::c_void = mi_malloc_aligned(size, alignment);
    if q.is_null() && size != 0 as libc::c_int as size_t {
        return 12 as libc::c_int;
    }
    if (q as uintptr_t).wrapping_rem(alignment) == 0 as libc::c_int as libc::c_ulong {
    } else {
        _mi_assert_fail(
            b"((uintptr_t)q % alignment) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3526 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 18], &[libc::c_char; 18]>(b"mi_posix_memalign\0"))
                .as_ptr(),
        );
    };
    *p = q;
    return 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_memalign(mut alignment: size_t, mut size: size_t) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = mi_malloc_aligned(size, alignment);
    if (p as uintptr_t).wrapping_rem(alignment) == 0 as libc::c_int as libc::c_ulong {
    } else {
        _mi_assert_fail(
            b"((uintptr_t)p % alignment) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3532 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 12], &[libc::c_char; 12]>(b"mi_memalign\0")).as_ptr(),
        );
    };
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_valloc(mut size: size_t) -> *mut libc::c_void {
    return mi_memalign(_mi_os_page_size(), size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_pvalloc(mut size: size_t) -> *mut libc::c_void {
    let mut psize: size_t = _mi_os_page_size();
    if size >= (18446744073709551615 as libc::c_ulong).wrapping_sub(psize) {
        return 0 as *mut libc::c_void;
    }
    let mut asize: size_t = _mi_align_up(size, psize);
    return mi_malloc_aligned(asize, psize);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_aligned_alloc(
    mut alignment: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = mi_malloc_aligned(size, alignment);
    if (p as uintptr_t).wrapping_rem(alignment) == 0 as libc::c_int as libc::c_ulong {
    } else {
        _mi_assert_fail(
            b"((uintptr_t)p % alignment) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3546 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_aligned_alloc\0"))
                .as_ptr(),
        );
    };
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reallocarray(
    mut p: *mut libc::c_void,
    mut count: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    let mut newp: *mut libc::c_void = mi_reallocn(p, count, size);
    if newp.is_null() {
        *__errno_location() = 12 as libc::c_int;
    }
    return newp;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reallocarr(
    mut p: *mut libc::c_void,
    mut count: size_t,
    mut size: size_t,
) -> libc::c_int {
    if !p.is_null() {
    } else {
        _mi_assert_fail(
            b"p != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3555 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_reallocarr\0"))
                .as_ptr(),
        );
    };
    if p.is_null() {
        *__errno_location() = 22 as libc::c_int;
        return 22 as libc::c_int;
    }
    let mut op: *mut *mut libc::c_void = p as *mut *mut libc::c_void;
    let mut newp: *mut libc::c_void = mi_reallocarray(*op, count, size);
    if newp.is_null() as libc::c_int as libc::c_long != 0 {
        return *__errno_location();
    }
    *op = newp;
    return 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi__expand(
    mut p: *mut libc::c_void,
    mut newsize: size_t,
) -> *mut libc::c_void {
    let mut res: *mut libc::c_void = mi_expand(p, newsize);
    if res.is_null() {
        *__errno_location() = 12 as libc::c_int;
    }
    return res;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_wcsdup(mut s: *const libc::c_ushort) -> *mut libc::c_ushort {
    if s.is_null() {
        return 0 as *mut libc::c_ushort;
    }
    let mut len: size_t = 0;
    len = 0 as libc::c_int as size_t;
    while *s.offset(len as isize) as libc::c_int != 0 as libc::c_int {
        len = len.wrapping_add(1);
        len;
    }
    let mut size: size_t = len
        .wrapping_add(1 as libc::c_int as size_t)
        .wrapping_mul(::core::mem::size_of::<libc::c_ushort>() as libc::c_ulong);
    let mut p: *mut libc::c_ushort = mi_malloc(size) as *mut libc::c_ushort;
    if !p.is_null() {
        _mi_memcpy(p as *mut libc::c_void, s as *const libc::c_void, size);
    }
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_mbsdup(mut s: *const libc::c_uchar) -> *mut libc::c_uchar {
    return mi_strdup(s as *const libc::c_char) as *mut libc::c_uchar;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_dupenv_s(
    mut buf: *mut *mut libc::c_char,
    mut size: *mut size_t,
    mut name: *const libc::c_char,
) -> libc::c_int {
    if buf.is_null() || name.is_null() {
        return 22 as libc::c_int;
    }
    if !size.is_null() {
        *size = 0 as libc::c_int as size_t;
    }
    let mut p: *mut libc::c_char = getenv(name);
    if p.is_null() {
        *buf = 0 as *mut libc::c_char;
    } else {
        *buf = mi_strdup(p);
        if (*buf).is_null() {
            return 12 as libc::c_int;
        }
        if !size.is_null() {
            *size = _mi_strlen(p);
        }
    }
    return 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_wdupenv_s(
    mut buf: *mut *mut libc::c_ushort,
    mut size: *mut size_t,
    mut name: *const libc::c_ushort,
) -> libc::c_int {
    if buf.is_null() || name.is_null() {
        return 22 as libc::c_int;
    }
    if !size.is_null() {
        *size = 0 as libc::c_int as size_t;
    }
    *buf = 0 as *mut libc::c_ushort;
    return 22 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_aligned_offset_recalloc(
    mut p: *mut libc::c_void,
    mut newcount: size_t,
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
) -> *mut libc::c_void {
    return mi_recalloc_aligned_at(p, newcount, size, alignment, offset);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_aligned_recalloc(
    mut p: *mut libc::c_void,
    mut newcount: size_t,
    mut size: size_t,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return mi_recalloc_aligned(p, newcount, size, alignment);
}
#[inline]
unsafe extern "C" fn mi_bitmap_index_create_ex(
    mut idx: size_t,
    mut bitidx: size_t,
) -> mi_bitmap_index_t {
    if bitidx <= (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t {
    } else {
        _mi_assert_fail(
            b"bitidx <= MI_BITMAP_FIELD_BITS\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3632 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_bitmap_index_create_ex\0",
            ))
            .as_ptr(),
        );
    };
    return (idx * (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t)
        .wrapping_add(bitidx);
}
#[inline]
unsafe extern "C" fn mi_bitmap_index_create(
    mut idx: size_t,
    mut bitidx: size_t,
) -> mi_bitmap_index_t {
    if bitidx < (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t {
    } else {
        _mi_assert_fail(
            b"bitidx < MI_BITMAP_FIELD_BITS\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3636 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"mi_bitmap_index_create\0",
            ))
            .as_ptr(),
        );
    };
    return mi_bitmap_index_create_ex(idx, bitidx);
}
#[inline]
unsafe extern "C" fn mi_bitmap_index_field(mut bitmap_idx: mi_bitmap_index_t) -> size_t {
    return bitmap_idx
        / (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as mi_bitmap_index_t;
}
#[inline]
unsafe extern "C" fn mi_bitmap_index_bit_in_field(mut bitmap_idx: mi_bitmap_index_t) -> size_t {
    return bitmap_idx
        % (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as mi_bitmap_index_t;
}
#[inline]
unsafe extern "C" fn mi_bitmap_index_bit(mut bitmap_idx: mi_bitmap_index_t) -> size_t {
    return bitmap_idx;
}
static mut mi_arenas: [*mut mi_arena_t; 132] = [0 as *const mi_arena_t as *mut mi_arena_t; 132];
static mut mi_arena_count: size_t = 0;
unsafe extern "C" fn mi_arena_segment_os_clear_abandoned(
    mut segment: *mut mi_segment_t,
    mut take_lock: bool,
) -> bool {
    if (*segment).memid.memkind as libc::c_uint != MI_MEM_ARENA as libc::c_int as libc::c_uint {
    } else {
        _mi_assert_fail(
            b"segment->memid.memkind != MI_MEM_ARENA\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3696 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 36], &[libc::c_char; 36]>(
                b"mi_arena_segment_os_clear_abandoned\0",
            ))
            .as_ptr(),
        );
    };
    let subproc: *mut mi_subproc_t = (*segment).subproc;
    if take_lock as libc::c_int != 0 && !mi_lock_try_acquire(&mut (*subproc).abandoned_os_lock) {
        return 0 as libc::c_int != 0;
    }
    let mut reclaimed: bool = 0 as libc::c_int != 0;
    let next: *mut mi_segment_t = (*segment).abandoned_os_next;
    let prev: *mut mi_segment_t = (*segment).abandoned_os_prev;
    if !next.is_null() || !prev.is_null() || (*subproc).abandoned_os_list == segment {
        if !prev.is_null() {
            (*prev).abandoned_os_next = next;
        } else {
            (*subproc).abandoned_os_list = next;
        }
        if !next.is_null() {
            (*next).abandoned_os_prev = prev;
        } else {
            (*subproc).abandoned_os_list_tail = prev;
        }
        (*segment).abandoned_os_next = 0 as *mut mi_segment_s;
        (*segment).abandoned_os_prev = 0 as *mut mi_segment_s;
        ::core::intrinsics::atomic_xsub_relaxed(
            &mut (*subproc).abandoned_count,
            1 as libc::c_int as uintptr_t,
        );
        ::core::intrinsics::atomic_xsub_relaxed(
            &mut (*subproc).abandoned_os_list_count,
            1 as libc::c_int as uintptr_t,
        );
        if take_lock {
            ::core::intrinsics::atomic_store_release(&mut (*segment).thread_id, _mi_thread_id());
        }
        reclaimed = 1 as libc::c_int != 0;
    }
    if take_lock {
        mi_lock_release(&mut (*(*segment).subproc).abandoned_os_lock);
    }
    return reclaimed;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_segment_clear_abandoned(mut segment: *mut mi_segment_t) -> bool {
    if ((*segment).memid.memkind as libc::c_uint != MI_MEM_ARENA as libc::c_int as libc::c_uint)
        as libc::c_int as libc::c_long
        != 0
    {
        return mi_arena_segment_os_clear_abandoned(segment, 1 as libc::c_int != 0);
    }
    let mut arena_idx: size_t = 0;
    let mut bitmap_idx: size_t = 0;
    mi_arena_memid_indices((*segment).memid, &mut arena_idx, &mut bitmap_idx);
    let mut arena: *mut mi_arena_t = mi_arena_from_index(arena_idx);
    if !arena.is_null() {
    } else {
        _mi_assert_fail(
            b"arena != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3739 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 34], &[libc::c_char; 34]>(
                b"_mi_arena_segment_clear_abandoned\0",
            ))
            .as_ptr(),
        );
    };
    let mut was_marked: bool = _mi_bitmap_unclaim(
        (*arena).blocks_abandoned,
        (*arena).field_count,
        1 as libc::c_int as size_t,
        bitmap_idx,
    );
    if was_marked {
        if ::core::intrinsics::atomic_load_acquire(&mut (*segment).thread_id as *mut mi_threadid_t)
            == 0 as libc::c_int as mi_threadid_t
        {
        } else {
            _mi_assert_fail(
                b"mi_atomic_load_acquire(&segment->thread_id) == 0\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                3742 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 34], &[libc::c_char; 34]>(
                    b"_mi_arena_segment_clear_abandoned\0",
                ))
                .as_ptr(),
            );
        };
        ::core::intrinsics::atomic_xsub_relaxed(
            &mut (*(*segment).subproc).abandoned_count,
            1 as libc::c_int as uintptr_t,
        );
        ::core::intrinsics::atomic_store_release(&mut (*segment).thread_id, _mi_thread_id());
    }
    if !was_marked
        || _mi_bitmap_is_claimed(
            ((*arena).blocks_inuse).as_mut_ptr(),
            (*arena).field_count,
            1 as libc::c_int as size_t,
            bitmap_idx,
        ) as libc::c_int
            != 0
    {
    } else {
        _mi_assert_fail(
            b"!was_marked || _mi_bitmap_is_claimed(arena->blocks_inuse, arena->field_count, 1, bitmap_idx)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3746 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 34],
                &[libc::c_char; 34],
            >(b"_mi_arena_segment_clear_abandoned\0"))
                .as_ptr(),
        );
    };
    return was_marked;
}
unsafe extern "C" fn mi_arena_segment_os_mark_abandoned(mut segment: *mut mi_segment_t) {
    if (*segment).memid.memkind as libc::c_uint != MI_MEM_ARENA as libc::c_int as libc::c_uint {
    } else {
        _mi_assert_fail(
            b"segment->memid.memkind != MI_MEM_ARENA\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3750 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 35], &[libc::c_char; 35]>(
                b"mi_arena_segment_os_mark_abandoned\0",
            ))
            .as_ptr(),
        );
    };
    let subproc: *mut mi_subproc_t = (*segment).subproc;
    if !mi_lock_acquire(&mut (*subproc).abandoned_os_lock) {
        _mi_error_message(
            14 as libc::c_int,
            b"internal error: failed to acquire the abandoned (os) segment lock to mark abandonment\0"
                as *const u8 as *const libc::c_char,
        );
    } else {
        let mut prev: *mut mi_segment_t = (*subproc).abandoned_os_list_tail;
        if prev.is_null() || ((*prev).abandoned_os_next).is_null() {
        } else {
            _mi_assert_fail(
                b"prev == NULL || prev->abandoned_os_next == NULL\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                3757 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 35], &[libc::c_char; 35]>(
                    b"mi_arena_segment_os_mark_abandoned\0",
                ))
                .as_ptr(),
            );
        };
        if ((*segment).abandoned_os_prev).is_null() {
        } else {
            _mi_assert_fail(
                b"segment->abandoned_os_prev == NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                3758 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 35], &[libc::c_char; 35]>(
                    b"mi_arena_segment_os_mark_abandoned\0",
                ))
                .as_ptr(),
            );
        };
        if ((*segment).abandoned_os_next).is_null() {
        } else {
            _mi_assert_fail(
                b"segment->abandoned_os_next == NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                3759 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 35], &[libc::c_char; 35]>(
                    b"mi_arena_segment_os_mark_abandoned\0",
                ))
                .as_ptr(),
            );
        };
        if !prev.is_null() {
            (*prev).abandoned_os_next = segment;
        } else {
            (*subproc).abandoned_os_list = segment;
        }
        (*subproc).abandoned_os_list_tail = segment;
        (*segment).abandoned_os_prev = prev;
        (*segment).abandoned_os_next = 0 as *mut mi_segment_s;
        ::core::intrinsics::atomic_xadd_relaxed(
            &mut (*subproc).abandoned_os_list_count,
            1 as libc::c_int as uintptr_t,
        );
        ::core::intrinsics::atomic_xadd_relaxed(
            &mut (*subproc).abandoned_count,
            1 as libc::c_int as uintptr_t,
        );
        mi_lock_release(&mut (*subproc).abandoned_os_lock);
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_segment_mark_abandoned(mut segment: *mut mi_segment_t) {
    if (*segment).used == (*segment).abandoned {
    } else {
        _mi_assert_fail(
            b"segment->used == segment->abandoned\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3773 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 33], &[libc::c_char; 33]>(
                b"_mi_arena_segment_mark_abandoned\0",
            ))
            .as_ptr(),
        );
    };
    ::core::intrinsics::atomic_store_release(
        &mut (*segment).thread_id,
        0 as libc::c_int as uintptr_t,
    );
    if ((*segment).memid.memkind as libc::c_uint != MI_MEM_ARENA as libc::c_int as libc::c_uint)
        as libc::c_int as libc::c_long
        != 0
    {
        mi_arena_segment_os_mark_abandoned(segment);
        return;
    }
    let mut arena_idx: size_t = 0;
    let mut bitmap_idx: size_t = 0;
    mi_arena_memid_indices((*segment).memid, &mut arena_idx, &mut bitmap_idx);
    let mut arena: *mut mi_arena_t = mi_arena_from_index(arena_idx);
    if !arena.is_null() {
    } else {
        _mi_assert_fail(
            b"arena != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3783 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 33], &[libc::c_char; 33]>(
                b"_mi_arena_segment_mark_abandoned\0",
            ))
            .as_ptr(),
        );
    };
    let subproc: *mut mi_subproc_t = (*segment).subproc;
    let was_unmarked: bool = _mi_bitmap_claim(
        (*arena).blocks_abandoned,
        (*arena).field_count,
        1 as libc::c_int as size_t,
        bitmap_idx,
        0 as *mut bool,
    );
    if was_unmarked {
        ::core::intrinsics::atomic_xadd_relaxed(
            &mut (*subproc).abandoned_count,
            1 as libc::c_int as uintptr_t,
        );
    }
    if was_unmarked as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"was_unmarked\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3787 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 33], &[libc::c_char; 33]>(
                b"_mi_arena_segment_mark_abandoned\0",
            ))
            .as_ptr(),
        );
    };
    if _mi_bitmap_is_claimed(
        ((*arena).blocks_inuse).as_mut_ptr(),
        (*arena).field_count,
        1 as libc::c_int as size_t,
        bitmap_idx,
    ) as libc::c_int
        != 0
    {
    } else {
        _mi_assert_fail(
            b"_mi_bitmap_is_claimed(arena->blocks_inuse, arena->field_count, 1, bitmap_idx)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3788 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 33], &[libc::c_char; 33]>(
                b"_mi_arena_segment_mark_abandoned\0",
            ))
            .as_ptr(),
        );
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_field_cursor_init(
    mut heap: *mut mi_heap_t,
    mut subproc: *mut mi_subproc_t,
    mut visit_all: bool,
    mut current: *mut mi_arena_field_cursor_t,
) {
    if heap.is_null() || (*(*heap).tld).segments.subproc == subproc {
    } else {
        _mi_assert_fail(
            b"heap == NULL || heap->tld->segments.subproc == subproc\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3791 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"_mi_arena_field_cursor_init\0",
            ))
            .as_ptr(),
        );
    };
    (*current).bitmap_idx = 0 as libc::c_int as size_t;
    (*current).subproc = subproc;
    (*current).visit_all = visit_all;
    (*current).hold_visit_lock = 0 as libc::c_int != 0;
    let abandoned_count: size_t =
        ::core::intrinsics::atomic_load_relaxed(&mut (*subproc).abandoned_count);
    let abandoned_list_count: size_t =
        ::core::intrinsics::atomic_load_relaxed(&mut (*subproc).abandoned_os_list_count);
    let max_arena: size_t = mi_arena_get_count();
    if !heap.is_null() && (*heap).arena_id != _mi_arena_id_none() {
        (*current).start = mi_arena_id_index((*heap).arena_id);
        (*current).end = ((*current).start).wrapping_add(1 as libc::c_int as size_t);
        (*current).os_list_count = 0 as libc::c_int as size_t;
    } else {
        if abandoned_count > abandoned_list_count && max_arena > 0 as libc::c_int as size_t {
            (*current).start = (if heap.is_null() || max_arena == 0 as libc::c_int as size_t {
                0 as libc::c_int
            } else {
                (_mi_heap_random_next(heap)).wrapping_rem(max_arena) as mi_arena_id_t
            }) as size_t;
            (*current).end = ((*current).start).wrapping_add(max_arena);
        } else {
            (*current).start = 0 as libc::c_int as size_t;
            (*current).end = 0 as libc::c_int as size_t;
        }
        (*current).os_list_count = abandoned_list_count;
    }
    if (*current).start <= max_arena {
    } else {
        _mi_assert_fail(
            b"current->start <= max_arena\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3815 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"_mi_arena_field_cursor_init\0",
            ))
            .as_ptr(),
        );
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_field_cursor_done(mut current: *mut mi_arena_field_cursor_t) {
    if (*current).hold_visit_lock {
        mi_lock_release(&mut (*(*current).subproc).abandoned_os_visit_lock);
        (*current).hold_visit_lock = 0 as libc::c_int != 0;
    }
}
unsafe extern "C" fn mi_arena_segment_clear_abandoned_at(
    mut arena: *mut mi_arena_t,
    mut subproc: *mut mi_subproc_t,
    mut bitmap_idx: mi_bitmap_index_t,
) -> *mut mi_segment_t {
    if !_mi_bitmap_unclaim(
        (*arena).blocks_abandoned,
        (*arena).field_count,
        1 as libc::c_int as size_t,
        bitmap_idx,
    ) {
        return 0 as *mut mi_segment_t;
    }
    if _mi_bitmap_is_claimed(
        ((*arena).blocks_inuse).as_mut_ptr(),
        (*arena).field_count,
        1 as libc::c_int as size_t,
        bitmap_idx,
    ) as libc::c_int
        != 0
    {
    } else {
        _mi_assert_fail(
            b"_mi_bitmap_is_claimed(arena->blocks_inuse, arena->field_count, 1, bitmap_idx)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3825 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 36], &[libc::c_char; 36]>(
                b"mi_arena_segment_clear_abandoned_at\0",
            ))
            .as_ptr(),
        );
    };
    let mut segment: *mut mi_segment_t =
        mi_arena_block_start(arena, bitmap_idx) as *mut mi_segment_t;
    if ::core::intrinsics::atomic_load_relaxed(&mut (*segment).thread_id as *mut mi_threadid_t)
        == 0 as libc::c_int as mi_threadid_t
    {
    } else {
        _mi_assert_fail(
            b"mi_atomic_load_relaxed(&segment->thread_id) == 0\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3827 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 36], &[libc::c_char; 36]>(
                b"mi_arena_segment_clear_abandoned_at\0",
            ))
            .as_ptr(),
        );
    };
    if (*segment).subproc != subproc {
        let was_zero: bool = _mi_bitmap_claim(
            (*arena).blocks_abandoned,
            (*arena).field_count,
            1 as libc::c_int as size_t,
            bitmap_idx,
            0 as *mut bool,
        );
        if was_zero as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"was_zero\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                3830 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 36], &[libc::c_char; 36]>(
                    b"mi_arena_segment_clear_abandoned_at\0",
                ))
                .as_ptr(),
            );
        };
        return 0 as *mut mi_segment_t;
    } else {
        ::core::intrinsics::atomic_xsub_relaxed(
            &mut (*subproc).abandoned_count,
            1 as libc::c_int as uintptr_t,
        );
        return segment;
    };
}
unsafe extern "C" fn mi_arena_segment_clear_abandoned_next_field(
    mut previous: *mut mi_arena_field_cursor_t,
) -> *mut mi_segment_t {
    let max_arena: size_t = mi_arena_get_count();
    let mut field_idx: size_t = mi_bitmap_index_field((*previous).bitmap_idx);
    let mut bit_idx: size_t = mi_bitmap_index_bit_in_field((*previous).bitmap_idx);
    while (*previous).start < (*previous).end {
        let mut arena_idx: size_t = if (*previous).start >= max_arena {
            (*previous).start % max_arena
        } else {
            (*previous).start
        };
        let mut arena: *mut mi_arena_t = mi_arena_from_index(arena_idx);
        if !arena.is_null() {
            let mut has_lock: bool = 0 as libc::c_int != 0;
            while field_idx < (*arena).field_count {
                let mut field: size_t = ::core::intrinsics::atomic_load_relaxed(
                    &mut *((*arena).blocks_abandoned).offset(field_idx as isize)
                        as *mut mi_bitmap_field_t,
                );
                if (field != 0 as libc::c_int as size_t) as libc::c_int as libc::c_long != 0 {
                    if !has_lock
                        && mi_option_is_enabled(mi_option_visit_abandoned) as libc::c_int != 0
                    {
                        has_lock = if (*previous).visit_all as libc::c_int != 0 {
                            mi_lock_acquire(&mut (*arena).abandoned_visit_lock) as libc::c_int
                        } else {
                            mi_lock_try_acquire(&mut (*arena).abandoned_visit_lock) as libc::c_int
                        } != 0;
                        if !has_lock {
                            if (*previous).visit_all {
                                _mi_error_message(
                                    14 as libc::c_int,
                                    b"internal error: failed to visit all abandoned segments due to failure to acquire the visitor lock\0"
                                        as *const u8 as *const libc::c_char,
                                );
                            }
                            break;
                        }
                    }
                    if has_lock as libc::c_int != 0
                        || !mi_option_is_enabled(mi_option_visit_abandoned)
                    {
                    } else {
                        _mi_assert_fail(
                            b"has_lock || !mi_option_is_enabled(mi_option_visit_abandoned)\0"
                                as *const u8 as *const libc::c_char,
                            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                            3859 as libc::c_int as libc::c_uint,
                            (*::core::mem::transmute::<&[u8; 44], &[libc::c_char; 44]>(
                                b"mi_arena_segment_clear_abandoned_next_field\0",
                            ))
                            .as_ptr(),
                        );
                    };
                    while bit_idx
                        < (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t
                    {
                        let mut mask: size_t = (1 as libc::c_int as size_t) << bit_idx;
                        if (field & mask == mask) as libc::c_int as libc::c_long != 0 {
                            let mut bitmap_idx: mi_bitmap_index_t =
                                mi_bitmap_index_create(field_idx, bit_idx);
                            let segment: *mut mi_segment_t = mi_arena_segment_clear_abandoned_at(
                                arena,
                                (*previous).subproc,
                                bitmap_idx,
                            );
                            if !segment.is_null() {
                                if has_lock {
                                    mi_lock_release(&mut (*arena).abandoned_visit_lock);
                                }
                                (*previous).bitmap_idx = mi_bitmap_index_create_ex(
                                    field_idx,
                                    bit_idx.wrapping_add(1 as libc::c_int as size_t),
                                );
                                return segment;
                            }
                        }
                        bit_idx = bit_idx.wrapping_add(1);
                        bit_idx;
                    }
                }
                field_idx = field_idx.wrapping_add(1);
                field_idx;
                bit_idx = 0 as libc::c_int as size_t;
            }
            if has_lock {
                mi_lock_release(&mut (*arena).abandoned_visit_lock);
            }
        }
        (*previous).start = ((*previous).start).wrapping_add(1);
        (*previous).start;
        field_idx = 0 as libc::c_int as size_t;
        bit_idx = 0 as libc::c_int as size_t;
    }
    return 0 as *mut mi_segment_t;
}
unsafe extern "C" fn mi_arena_segment_clear_abandoned_next_list(
    mut previous: *mut mi_arena_field_cursor_t,
) -> *mut mi_segment_t {
    if !(*previous).hold_visit_lock {
        (*previous).hold_visit_lock = if (*previous).visit_all as libc::c_int != 0 {
            mi_lock_acquire(&mut (*(*previous).subproc).abandoned_os_visit_lock) as libc::c_int
        } else {
            mi_lock_try_acquire(&mut (*(*previous).subproc).abandoned_os_visit_lock) as libc::c_int
        } != 0;
        if !(*previous).hold_visit_lock {
            if (*previous).visit_all {
                _mi_error_message(
                    14 as libc::c_int,
                    b"internal error: failed to visit all abandoned segments due to failure to acquire the OS visitor lock\0"
                        as *const u8 as *const libc::c_char,
                );
            }
            return 0 as *mut mi_segment_t;
        }
    }
    while (*previous).os_list_count > 0 as libc::c_int as size_t {
        (*previous).os_list_count = ((*previous).os_list_count).wrapping_sub(1);
        (*previous).os_list_count;
        let has_lock: bool = mi_lock_acquire(&mut (*(*previous).subproc).abandoned_os_lock);
        if has_lock {
            let mut segment: *mut mi_segment_t = (*(*previous).subproc).abandoned_os_list;
            if segment.is_null()
                || mi_arena_segment_os_clear_abandoned(segment, 0 as libc::c_int != 0)
                    as libc::c_int
                    != 0
            {
                mi_lock_release(&mut (*(*previous).subproc).abandoned_os_lock);
                return segment;
            }
            mi_lock_release(&mut (*(*previous).subproc).abandoned_os_lock);
        } else {
            _mi_error_message(
                14 as libc::c_int,
                b"failed to acquire abandoned OS list lock during abandoned block visit\n\0"
                    as *const u8 as *const libc::c_char,
            );
            return 0 as *mut mi_segment_t;
        }
    }
    if (*previous).os_list_count == 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"previous->os_list_count == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3906 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 43], &[libc::c_char; 43]>(
                b"mi_arena_segment_clear_abandoned_next_list\0",
            ))
            .as_ptr(),
        );
    };
    return 0 as *mut mi_segment_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_segment_clear_abandoned_next(
    mut previous: *mut mi_arena_field_cursor_t,
) -> *mut mi_segment_t {
    if (*previous).start < (*previous).end {
        let mut segment: *mut mi_segment_t = mi_arena_segment_clear_abandoned_next_field(previous);
        if !segment.is_null() {
            return segment;
        }
    }
    if (*previous).start == (*previous).end {
    } else {
        _mi_assert_fail(
            b"previous->start == previous->end\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3914 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 39], &[libc::c_char; 39]>(
                b"_mi_arena_segment_clear_abandoned_next\0",
            ))
            .as_ptr(),
        );
    };
    return mi_arena_segment_clear_abandoned_next_list(previous);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_abandoned_visit_blocks(
    mut subproc_id: mi_subproc_id_t,
    mut heap_tag: libc::c_int,
    mut visit_blocks: bool,
    mut visitor: Option<mi_block_visit_fun>,
    mut arg: *mut libc::c_void,
) -> bool {
    if !mi_option_is_enabled(mi_option_visit_abandoned) {
        _mi_error_message(
            14 as libc::c_int,
            b"internal error: can only visit abandoned blocks when MIMALLOC_VISIT_ABANDONED=ON\0"
                as *const u8 as *const libc::c_char,
        );
        return 0 as libc::c_int != 0;
    }
    let mut current: mi_arena_field_cursor_t = mi_arena_field_cursor_s {
        os_list_count: 0,
        start: 0,
        end: 0,
        bitmap_idx: 0,
        subproc: 0 as *mut mi_subproc_t,
        visit_all: false,
        hold_visit_lock: false,
    };
    _mi_arena_field_cursor_init(
        0 as *mut mi_heap_t,
        _mi_subproc_from_id(subproc_id),
        1 as libc::c_int != 0,
        &mut current,
    );
    let mut segment: *mut mi_segment_t = 0 as *mut mi_segment_t;
    let mut ok: bool = 1 as libc::c_int != 0;
    while ok as libc::c_int != 0 && {
        segment = _mi_arena_segment_clear_abandoned_next(&mut current);
        !segment.is_null()
    } {
        ok = _mi_segment_visit_blocks(segment, heap_tag, visit_blocks, visitor, arg);
        _mi_arena_segment_mark_abandoned(segment);
    }
    _mi_arena_field_cursor_done(&mut current);
    return ok;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_arena_id_index(mut id: mi_arena_id_t) -> size_t {
    return (if id <= 0 as libc::c_int {
        132 as libc::c_int
    } else {
        id - 1 as libc::c_int
    }) as size_t;
}
unsafe extern "C" fn mi_arena_id_create(mut arena_index: size_t) -> mi_arena_id_t {
    if arena_index < 132 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"arena_index < MI_MAX_ARENAS\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3938 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_arena_id_create\0"))
                .as_ptr(),
        );
    };
    return arena_index as libc::c_int + 1 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_id_none() -> mi_arena_id_t {
    return 0 as libc::c_int;
}
unsafe extern "C" fn mi_arena_id_is_suitable(
    mut arena_id: mi_arena_id_t,
    mut arena_is_exclusive: bool,
    mut req_arena_id: mi_arena_id_t,
) -> bool {
    return !arena_is_exclusive && req_arena_id == _mi_arena_id_none() || arena_id == req_arena_id;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_memid_is_suitable(
    mut memid: mi_memid_t,
    mut request_arena_id: mi_arena_id_t,
) -> bool {
    if memid.memkind as libc::c_uint == MI_MEM_ARENA as libc::c_int as libc::c_uint {
        return mi_arena_id_is_suitable(
            memid.mem.arena.id,
            memid.mem.arena.is_exclusive,
            request_arena_id,
        );
    } else {
        return mi_arena_id_is_suitable(
            _mi_arena_id_none(),
            0 as libc::c_int != 0,
            request_arena_id,
        );
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_arena_get_count() -> size_t {
    return ::core::intrinsics::atomic_load_relaxed(&mut mi_arena_count);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_arena_from_index(mut idx: size_t) -> *mut mi_arena_t {
    if idx < mi_arena_get_count() {
    } else {
        _mi_assert_fail(
            b"idx < mi_arena_get_count()\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3960 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_arena_from_index\0"))
                .as_ptr(),
        );
    };
    return ::core::intrinsics::atomic_load_acquire(
        &mut *mi_arenas.as_mut_ptr().offset(idx as isize) as *mut *mut mi_arena_t,
    );
}
unsafe extern "C" fn mi_block_count_of_size(mut size: size_t) -> size_t {
    return _mi_divide_up(
        size,
        ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            as size_t,
    );
}
unsafe extern "C" fn mi_arena_block_size(mut bcount: size_t) -> size_t {
    return (bcount as libc::c_ulonglong).wrapping_mul(
        (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
    ) as size_t;
}
unsafe extern "C" fn mi_arena_size(mut arena: *mut mi_arena_t) -> size_t {
    return mi_arena_block_size((*arena).block_count);
}
unsafe extern "C" fn mi_memid_create_arena(
    mut id: mi_arena_id_t,
    mut is_exclusive: bool,
    mut bitmap_index: mi_bitmap_index_t,
) -> mi_memid_t {
    let mut memid: mi_memid_t = _mi_memid_create(MI_MEM_ARENA);
    memid.mem.arena.id = id;
    memid.mem.arena.block_index = bitmap_index;
    memid.mem.arena.is_exclusive = is_exclusive;
    return memid;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_arena_memid_indices(
    mut memid: mi_memid_t,
    mut arena_index: *mut size_t,
    mut bitmap_index: *mut mi_bitmap_index_t,
) -> bool {
    if memid.memkind as libc::c_uint == MI_MEM_ARENA as libc::c_int as libc::c_uint {
    } else {
        _mi_assert_fail(
            b"memid.memkind == MI_MEM_ARENA\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            3980 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"mi_arena_memid_indices\0",
            ))
            .as_ptr(),
        );
    };
    *arena_index = mi_arena_id_index(memid.mem.arena.id);
    *bitmap_index = memid.mem.arena.block_index;
    return memid.mem.arena.is_exclusive;
}
static mut mi_arena_static: [uint8_t; 4096] = [0; 4096];
static mut mi_arena_static_top: size_t = 0;
unsafe extern "C" fn mi_arena_static_zalloc(
    mut size: size_t,
    mut alignment: size_t,
    mut memid: *mut mi_memid_t,
) -> *mut libc::c_void {
    *memid = _mi_memid_none();
    if size == 0 as libc::c_int as size_t
        || size as libc::c_ulonglong
            > ((((1 as libc::c_int) << 3 as libc::c_int) / 2 as libc::c_int) as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong)
    {
        return 0 as *mut libc::c_void;
    }
    let toplow: size_t = ::core::intrinsics::atomic_load_relaxed(&mut mi_arena_static_top);
    if toplow.wrapping_add(size) as libc::c_ulonglong
        > ((((1 as libc::c_int) << 3 as libc::c_int) / 2 as libc::c_int) as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong)
    {
        return 0 as *mut libc::c_void;
    }
    if alignment < 16 as libc::c_int as size_t {
        alignment = 16 as libc::c_int as size_t;
    }
    let oversize: size_t = size
        .wrapping_add(alignment)
        .wrapping_sub(1 as libc::c_int as size_t);
    if toplow.wrapping_add(oversize) as libc::c_ulonglong
        > ((((1 as libc::c_int) << 3 as libc::c_int) / 2 as libc::c_int) as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong)
    {
        return 0 as *mut libc::c_void;
    }
    let oldtop: size_t = ::core::intrinsics::atomic_xadd_acqrel(&mut mi_arena_static_top, oversize);
    let mut top: size_t = oldtop.wrapping_add(oversize);
    if top as libc::c_ulonglong
        > ((((1 as libc::c_int) << 3 as libc::c_int) / 2 as libc::c_int) as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong)
    {
        let fresh7 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
            &mut mi_arena_static_top,
            *&mut top,
            oldtop,
        );
        *&mut top = fresh7.0;
        fresh7.1;
        return 0 as *mut libc::c_void;
    }
    *memid = _mi_memid_create(MI_MEM_STATIC);
    (*memid).initially_zero = 1 as libc::c_int != 0;
    let start: size_t = _mi_align_up(oldtop, alignment);
    let p: *mut uint8_t = &mut *mi_arena_static.as_mut_ptr().offset(start as isize) as *mut uint8_t;
    _mi_memzero_aligned(p as *mut libc::c_void, size);
    return p as *mut libc::c_void;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_meta_zalloc(
    mut size: size_t,
    mut memid: *mut mi_memid_t,
) -> *mut libc::c_void {
    *memid = _mi_memid_none();
    let mut p: *mut libc::c_void = mi_arena_static_zalloc(size, 16 as libc::c_int as size_t, memid);
    if !p.is_null() {
        return p;
    }
    p = _mi_os_alloc(size, memid, &mut _mi_stats_main);
    if p.is_null() {
        return 0 as *mut libc::c_void;
    }
    if !(*memid).initially_zero {
        _mi_memzero_aligned(p, size);
        (*memid).initially_zero = 1 as libc::c_int != 0;
    }
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_meta_free(
    mut p: *mut libc::c_void,
    mut memid: mi_memid_t,
    mut size: size_t,
) {
    if mi_memkind_is_os(memid.memkind) {
        _mi_os_free(p, size, memid, &mut _mi_stats_main);
    } else {
        if memid.memkind as libc::c_uint == MI_MEM_STATIC as libc::c_int as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"memid.memkind == MI_MEM_STATIC\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4026 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(
                    b"_mi_arena_meta_free\0",
                ))
                .as_ptr(),
            );
        };
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_arena_block_start(
    mut arena: *mut mi_arena_t,
    mut bindex: mi_bitmap_index_t,
) -> *mut libc::c_void {
    return ((*arena).start).offset(mi_arena_block_size(mi_bitmap_index_bit(bindex)) as isize)
        as *mut libc::c_void;
}
unsafe extern "C" fn mi_arena_try_claim(
    mut arena: *mut mi_arena_t,
    mut blocks: size_t,
    mut bitmap_idx: *mut mi_bitmap_index_t,
    mut stats: *mut mi_stats_t,
) -> bool {
    let mut idx: size_t = 0 as libc::c_int as size_t;
    if _mi_bitmap_try_find_from_claim_across(
        ((*arena).blocks_inuse).as_mut_ptr(),
        (*arena).field_count,
        idx,
        blocks,
        bitmap_idx,
        stats,
    ) {
        ::core::intrinsics::atomic_store_relaxed(
            &mut (*arena).search_idx,
            mi_bitmap_index_field(*bitmap_idx),
        );
        return 1 as libc::c_int != 0;
    }
    return 0 as libc::c_int != 0;
}
#[inline(never)]
unsafe extern "C" fn mi_arena_try_alloc_at(
    mut arena: *mut mi_arena_t,
    mut arena_index: size_t,
    mut needed_bcount: size_t,
    mut commit: bool,
    mut memid: *mut mi_memid_t,
    mut tld: *mut mi_os_tld_t,
) -> *mut libc::c_void {
    if mi_arena_id_index((*arena).id) == arena_index {
    } else {
        _mi_assert_fail(
            b"mi_arena_id_index(arena->id) == arena_index\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4045 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_arena_try_alloc_at\0"))
                .as_ptr(),
        );
    };
    let mut bitmap_index: mi_bitmap_index_t = 0;
    if !mi_arena_try_claim(arena, needed_bcount, &mut bitmap_index, (*tld).stats) {
        return 0 as *mut libc::c_void;
    }
    let mut p: *mut libc::c_void = mi_arena_block_start(arena, bitmap_index);
    *memid = mi_memid_create_arena((*arena).id, (*arena).exclusive, bitmap_index);
    (*memid).is_pinned = (*arena).memid.is_pinned;
    if !((*arena).blocks_purge).is_null() {
        _mi_bitmap_unclaim_across(
            (*arena).blocks_purge,
            (*arena).field_count,
            needed_bcount,
            bitmap_index,
        );
    }
    if (*arena).memid.initially_zero as libc::c_int != 0 && !((*arena).blocks_dirty).is_null() {
        (*memid).initially_zero = _mi_bitmap_claim_across(
            (*arena).blocks_dirty,
            (*arena).field_count,
            needed_bcount,
            bitmap_index,
            0 as *mut bool,
        );
    }
    if ((*arena).blocks_committed).is_null() {
        (*memid).initially_committed = 1 as libc::c_int != 0;
    } else if commit {
        (*memid).initially_committed = 1 as libc::c_int != 0;
        let mut any_uncommitted: bool = false;
        _mi_bitmap_claim_across(
            (*arena).blocks_committed,
            (*arena).field_count,
            needed_bcount,
            bitmap_index,
            &mut any_uncommitted,
        );
        if any_uncommitted {
            let mut commit_zero: bool = 0 as libc::c_int != 0;
            if !_mi_os_commit(
                p,
                mi_arena_block_size(needed_bcount),
                &mut commit_zero,
                (*tld).stats,
            ) {
                (*memid).initially_committed = 0 as libc::c_int != 0;
            } else if commit_zero {
                (*memid).initially_zero = 1 as libc::c_int != 0;
            }
        }
    } else {
        (*memid).initially_committed = _mi_bitmap_is_claimed_across(
            (*arena).blocks_committed,
            (*arena).field_count,
            needed_bcount,
            bitmap_index,
        );
    }
    return p;
}
unsafe extern "C" fn mi_arena_try_alloc_at_id(
    mut arena_id: mi_arena_id_t,
    mut match_numa_node: bool,
    mut numa_node: libc::c_int,
    mut size: size_t,
    mut alignment: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut req_arena_id: mi_arena_id_t,
    mut memid: *mut mi_memid_t,
    mut tld: *mut mi_os_tld_t,
) -> *mut libc::c_void {
    if alignment as libc::c_ulonglong
        <= (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
    {
    } else {
        _mi_assert_fail(
            b"alignment <= MI_SEGMENT_ALIGN\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4083 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_arena_try_alloc_at_id\0",
            ))
            .as_ptr(),
        );
    };
    let bcount: size_t = mi_block_count_of_size(size);
    let arena_index: size_t = mi_arena_id_index(arena_id);
    if arena_index < ::core::intrinsics::atomic_load_relaxed(&mut mi_arena_count as *mut size_t) {
    } else {
        _mi_assert_fail(
            b"arena_index < mi_atomic_load_relaxed(&mi_arena_count)\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4086 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_arena_try_alloc_at_id\0",
            ))
            .as_ptr(),
        );
    };
    if size <= mi_arena_block_size(bcount) {
    } else {
        _mi_assert_fail(
            b"size <= mi_arena_block_size(bcount)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4087 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_arena_try_alloc_at_id\0",
            ))
            .as_ptr(),
        );
    };
    let mut arena: *mut mi_arena_t = mi_arena_from_index(arena_index);
    if arena.is_null() {
        return 0 as *mut libc::c_void;
    }
    if !allow_large && (*arena).is_large as libc::c_int != 0 {
        return 0 as *mut libc::c_void;
    }
    if !mi_arena_id_is_suitable((*arena).id, (*arena).exclusive, req_arena_id) {
        return 0 as *mut libc::c_void;
    }
    if req_arena_id == _mi_arena_id_none() {
        let numa_suitable: bool = numa_node < 0 as libc::c_int
            || (*arena).numa_node < 0 as libc::c_int
            || (*arena).numa_node == numa_node;
        if match_numa_node {
            if !numa_suitable {
                return 0 as *mut libc::c_void;
            }
        } else if numa_suitable {
            return 0 as *mut libc::c_void;
        }
    }
    let mut p: *mut libc::c_void =
        mi_arena_try_alloc_at(arena, arena_index, bcount, commit, memid, tld);
    if p.is_null() || _mi_is_aligned(p, alignment) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"p == NULL || _mi_is_aligned(p, alignment)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4098 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_arena_try_alloc_at_id\0",
            ))
            .as_ptr(),
        );
    };
    return p;
}
#[inline(never)]
unsafe extern "C" fn mi_arena_try_alloc(
    mut numa_node: libc::c_int,
    mut size: size_t,
    mut alignment: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut req_arena_id: mi_arena_id_t,
    mut memid: *mut mi_memid_t,
    mut tld: *mut mi_os_tld_t,
) -> *mut libc::c_void {
    if alignment as libc::c_ulonglong
        <= (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
    {
    } else {
        _mi_assert_fail(
            b"alignment <= MI_SEGMENT_ALIGN\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4106 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_arena_try_alloc\0"))
                .as_ptr(),
        );
    };
    let max_arena: size_t = ::core::intrinsics::atomic_load_relaxed(&mut mi_arena_count);
    if (max_arena == 0 as libc::c_int as size_t) as libc::c_int as libc::c_long != 0 {
        return 0 as *mut libc::c_void;
    }
    if req_arena_id != _mi_arena_id_none() {
        if mi_arena_id_index(req_arena_id) < max_arena {
            let mut p: *mut libc::c_void = mi_arena_try_alloc_at_id(
                req_arena_id,
                1 as libc::c_int != 0,
                numa_node,
                size,
                alignment,
                commit,
                allow_large,
                req_arena_id,
                memid,
                tld,
            );
            if !p.is_null() {
                return p;
            }
        }
    } else {
        let mut i: size_t = 0 as libc::c_int as size_t;
        while i < max_arena {
            let mut p_0: *mut libc::c_void = mi_arena_try_alloc_at_id(
                mi_arena_id_create(i),
                1 as libc::c_int != 0,
                numa_node,
                size,
                alignment,
                commit,
                allow_large,
                req_arena_id,
                memid,
                tld,
            );
            if !p_0.is_null() {
                return p_0;
            }
            i = i.wrapping_add(1);
            i;
        }
        if numa_node >= 0 as libc::c_int {
            let mut i_0: size_t = 0 as libc::c_int as size_t;
            while i_0 < max_arena {
                let mut p_1: *mut libc::c_void = mi_arena_try_alloc_at_id(
                    mi_arena_id_create(i_0),
                    0 as libc::c_int != 0,
                    numa_node,
                    size,
                    alignment,
                    commit,
                    allow_large,
                    req_arena_id,
                    memid,
                    tld,
                );
                if !p_1.is_null() {
                    return p_1;
                }
                i_0 = i_0.wrapping_add(1);
                i_0;
            }
        }
    }
    return 0 as *mut libc::c_void;
}
unsafe extern "C" fn mi_arena_reserve(
    mut req_size: size_t,
    mut allow_large: bool,
    mut req_arena_id: mi_arena_id_t,
    mut arena_id: *mut mi_arena_id_t,
) -> bool {
    if _mi_preloading() {
        return 0 as libc::c_int != 0;
    }
    if req_arena_id != _mi_arena_id_none() {
        return 0 as libc::c_int != 0;
    }
    let arena_count: size_t = ::core::intrinsics::atomic_load_acquire(&mut mi_arena_count);
    if arena_count > (132 as libc::c_int - 4 as libc::c_int) as size_t {
        return 0 as libc::c_int != 0;
    }
    let mut arena_reserve: size_t = mi_option_get_size(mi_option_arena_reserve);
    if arena_reserve == 0 as libc::c_int as size_t {
        return 0 as libc::c_int != 0;
    }
    if !_mi_os_has_virtual_reserve() {
        arena_reserve = arena_reserve / 4 as libc::c_int as size_t;
    }
    arena_reserve = _mi_align_up(
        arena_reserve,
        ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            as size_t,
    );
    arena_reserve = _mi_align_up(
        arena_reserve,
        ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            as size_t,
    );
    if arena_count >= 8 as libc::c_int as size_t && arena_count <= 128 as libc::c_int as size_t {
        let multiplier: size_t = (1 as libc::c_int as size_t)
            << _mi_clamp(
                arena_count / 8 as libc::c_int as size_t,
                0 as libc::c_int as size_t,
                16 as libc::c_int as size_t,
            );
        let mut reserve: size_t = 0 as libc::c_int as size_t;
        if !mi_mul_overflow(multiplier, arena_reserve, &mut reserve) {
            arena_reserve = reserve;
        }
    }
    if arena_reserve < req_size {
        return 0 as libc::c_int != 0;
    }
    let mut arena_commit: bool = 0 as libc::c_int != 0;
    if mi_option_get(mi_option_arena_eager_commit) == 2 as libc::c_int as libc::c_long {
        arena_commit = _mi_os_has_overcommit();
    } else if mi_option_get(mi_option_arena_eager_commit) == 1 as libc::c_int as libc::c_long {
        arena_commit = 1 as libc::c_int != 0;
    }
    return mi_reserve_os_memory_ex(
        arena_reserve,
        arena_commit,
        allow_large,
        0 as libc::c_int != 0,
        arena_id,
    ) == 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_alloc_aligned(
    mut size: size_t,
    mut alignment: size_t,
    mut align_offset: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut req_arena_id: mi_arena_id_t,
    mut memid: *mut mi_memid_t,
    mut tld: *mut mi_os_tld_t,
) -> *mut libc::c_void {
    if !memid.is_null() && !tld.is_null() {
    } else {
        _mi_assert_fail(
            b"memid != NULL && tld != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4158 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                b"_mi_arena_alloc_aligned\0",
            ))
            .as_ptr(),
        );
    };
    if size > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"size > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4159 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                b"_mi_arena_alloc_aligned\0",
            ))
            .as_ptr(),
        );
    };
    *memid = _mi_memid_none();
    let numa_node: libc::c_int = _mi_os_numa_node(tld);
    if !mi_option_is_enabled(mi_option_disallow_arena_alloc) || req_arena_id != _mi_arena_id_none()
    {
        if size as libc::c_ulonglong
            >= ((1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
            && alignment as libc::c_ulonglong
                <= (1 as libc::c_ulonglong)
                    << 3 as libc::c_int
                        + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
            && align_offset == 0 as libc::c_int as size_t
        {
            let mut p: *mut libc::c_void = mi_arena_try_alloc(
                numa_node,
                size,
                alignment,
                commit,
                allow_large,
                req_arena_id,
                memid,
                tld,
            );
            if !p.is_null() {
                return p;
            }
            if req_arena_id == _mi_arena_id_none() {
                let mut arena_id: mi_arena_id_t = 0 as libc::c_int;
                if mi_arena_reserve(size, allow_large, req_arena_id, &mut arena_id) {
                    if req_arena_id == _mi_arena_id_none() {
                    } else {
                        _mi_assert_fail(
                            b"req_arena_id == _mi_arena_id_none()\0" as *const u8
                                as *const libc::c_char,
                            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                            4169 as libc::c_int as libc::c_uint,
                            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                                b"_mi_arena_alloc_aligned\0",
                            ))
                            .as_ptr(),
                        );
                    };
                    p = mi_arena_try_alloc_at_id(
                        arena_id,
                        1 as libc::c_int != 0,
                        numa_node,
                        size,
                        alignment,
                        commit,
                        allow_large,
                        req_arena_id,
                        memid,
                        tld,
                    );
                    if !p.is_null() {
                        return p;
                    }
                }
            }
        }
    }
    if mi_option_is_enabled(mi_option_disallow_os_alloc) as libc::c_int != 0
        || req_arena_id != _mi_arena_id_none()
    {
        *__errno_location() = 12 as libc::c_int;
        return 0 as *mut libc::c_void;
    }
    if align_offset > 0 as libc::c_int as size_t {
        return _mi_os_alloc_aligned_at_offset(
            size,
            alignment,
            align_offset,
            commit,
            allow_large,
            memid,
            (*tld).stats,
        );
    } else {
        return _mi_os_alloc_aligned(size, alignment, commit, allow_large, memid, (*tld).stats);
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_alloc(
    mut size: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut req_arena_id: mi_arena_id_t,
    mut memid: *mut mi_memid_t,
    mut tld: *mut mi_os_tld_t,
) -> *mut libc::c_void {
    return _mi_arena_alloc_aligned(
        size,
        ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            as size_t,
        0 as libc::c_int as size_t,
        commit,
        allow_large,
        req_arena_id,
        memid,
        tld,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_arena_area(
    mut arena_id: mi_arena_id_t,
    mut size: *mut size_t,
) -> *mut libc::c_void {
    if !size.is_null() {
        *size = 0 as libc::c_int as size_t;
    }
    let mut arena_index: size_t = mi_arena_id_index(arena_id);
    if arena_index >= 132 as libc::c_int as size_t {
        return 0 as *mut libc::c_void;
    }
    let mut arena: *mut mi_arena_t = ::core::intrinsics::atomic_load_acquire(
        &mut *mi_arenas.as_mut_ptr().offset(arena_index as isize) as *mut *mut mi_arena_t,
    );
    if arena.is_null() {
        return 0 as *mut libc::c_void;
    }
    if !size.is_null() {
        *size = mi_arena_block_size((*arena).block_count);
    }
    return (*arena).start as *mut libc::c_void;
}
unsafe extern "C" fn mi_arena_purge_delay() -> libc::c_long {
    return mi_option_get(mi_option_purge_delay) * mi_option_get(mi_option_arena_purge_mult);
}
unsafe extern "C" fn mi_arena_purge(
    mut arena: *mut mi_arena_t,
    mut bitmap_idx: size_t,
    mut blocks: size_t,
    mut stats: *mut mi_stats_t,
) {
    if !((*arena).blocks_committed).is_null() {
    } else {
        _mi_assert_fail(
            b"arena->blocks_committed != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4204 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_arena_purge\0"))
                .as_ptr(),
        );
    };
    if !((*arena).blocks_purge).is_null() {
    } else {
        _mi_assert_fail(
            b"arena->blocks_purge != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4205 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_arena_purge\0"))
                .as_ptr(),
        );
    };
    if !(*arena).memid.is_pinned {
    } else {
        _mi_assert_fail(
            b"!arena->memid.is_pinned\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4206 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_arena_purge\0"))
                .as_ptr(),
        );
    };
    let size: size_t = mi_arena_block_size(blocks);
    let p: *mut libc::c_void = mi_arena_block_start(arena, bitmap_idx);
    let mut needs_recommit: bool = false;
    if _mi_bitmap_is_claimed_across(
        (*arena).blocks_committed,
        (*arena).field_count,
        blocks,
        bitmap_idx,
    ) {
        needs_recommit = _mi_os_purge(p, size, stats);
    } else {
        if mi_option_is_enabled(mi_option_purge_decommits) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"mi_option_is_enabled(mi_option_purge_decommits)\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4214 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_arena_purge\0"))
                    .as_ptr(),
            );
        };
        needs_recommit = _mi_os_purge_ex(p, size, 0 as libc::c_int != 0, stats);
        if needs_recommit {
            _mi_stat_increase(&mut _mi_stats_main.committed, size);
        }
    }
    _mi_bitmap_unclaim_across(
        (*arena).blocks_purge,
        (*arena).field_count,
        blocks,
        bitmap_idx,
    );
    if needs_recommit {
        _mi_bitmap_unclaim_across(
            (*arena).blocks_committed,
            (*arena).field_count,
            blocks,
            bitmap_idx,
        );
    }
}
unsafe extern "C" fn mi_arena_schedule_purge(
    mut arena: *mut mi_arena_t,
    mut bitmap_idx: size_t,
    mut blocks: size_t,
    mut stats: *mut mi_stats_t,
) {
    if !((*arena).blocks_purge).is_null() {
    } else {
        _mi_assert_fail(
            b"arena->blocks_purge != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4224 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                b"mi_arena_schedule_purge\0",
            ))
            .as_ptr(),
        );
    };
    let delay: libc::c_long = mi_arena_purge_delay();
    if delay < 0 as libc::c_int as libc::c_long {
        return;
    }
    if _mi_preloading() as libc::c_int != 0 || delay == 0 as libc::c_int as libc::c_long {
        mi_arena_purge(arena, bitmap_idx, blocks, stats);
    } else {
        let mut expire: mi_msecs_t =
            ::core::intrinsics::atomic_load_relaxed(&mut (*arena).purge_expire);
        if expire != 0 as libc::c_int as mi_msecs_t {
            ::core::intrinsics::atomic_xadd_acqrel(
                &mut (*arena).purge_expire,
                delay / 10 as libc::c_int as libc::c_long,
            );
        } else {
            ::core::intrinsics::atomic_store_release(
                &mut (*arena).purge_expire,
                _mi_clock_now() + delay,
            );
        }
        _mi_bitmap_claim_across(
            (*arena).blocks_purge,
            (*arena).field_count,
            blocks,
            bitmap_idx,
            0 as *mut bool,
        );
    };
}
unsafe extern "C" fn mi_arena_purge_range(
    mut arena: *mut mi_arena_t,
    mut idx: size_t,
    mut startidx: size_t,
    mut bitlen: size_t,
    mut purge: size_t,
    mut stats: *mut mi_stats_t,
) -> bool {
    let endidx: size_t = startidx.wrapping_add(bitlen);
    let mut bitidx: size_t = startidx;
    let mut all_purged: bool = 0 as libc::c_int != 0;
    while bitidx < endidx {
        let mut count: size_t = 0 as libc::c_int as size_t;
        while bitidx.wrapping_add(count) < endidx
            && purge & (1 as libc::c_int as size_t) << bitidx.wrapping_add(count)
                != 0 as libc::c_int as size_t
        {
            count = count.wrapping_add(1);
            count;
        }
        if count > 0 as libc::c_int as size_t {
            let range_idx: mi_bitmap_index_t = mi_bitmap_index_create(idx, bitidx);
            mi_arena_purge(arena, range_idx, count, stats);
            if count == bitlen {
                all_purged = 1 as libc::c_int != 0;
            }
        }
        bitidx = bitidx.wrapping_add(count.wrapping_add(1 as libc::c_int as size_t));
    }
    return all_purged;
}
unsafe extern "C" fn mi_arena_try_purge(
    mut arena: *mut mi_arena_t,
    mut now: mi_msecs_t,
    mut force: bool,
    mut stats: *mut mi_stats_t,
) -> bool {
    if (*arena).memid.is_pinned as libc::c_int != 0 || ((*arena).blocks_purge).is_null() {
        return 0 as libc::c_int != 0;
    }
    let mut expire: mi_msecs_t =
        ::core::intrinsics::atomic_load_relaxed(&mut (*arena).purge_expire);
    if expire == 0 as libc::c_int as mi_msecs_t {
        return 0 as libc::c_int != 0;
    }
    if !force && expire > now {
        return 0 as libc::c_int != 0;
    }
    let fresh8 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
        &mut (*arena).purge_expire,
        *&mut expire,
        0 as libc::c_int as mi_msecs_t,
    );
    *&mut expire = fresh8.0;
    fresh8.1;
    let mut any_purged: bool = 0 as libc::c_int != 0;
    let mut full_purge: bool = 1 as libc::c_int != 0;
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < (*arena).field_count {
        let mut purge: size_t = ::core::intrinsics::atomic_load_relaxed(
            &mut *((*arena).blocks_purge).offset(i as isize) as *mut mi_bitmap_field_t,
        );
        if purge != 0 as libc::c_int as size_t {
            let mut bitidx: size_t = 0 as libc::c_int as size_t;
            while bitidx < (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t {
                let mut bitlen: size_t = 0 as libc::c_int as size_t;
                while bitidx.wrapping_add(bitlen)
                    < (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t
                    && purge & (1 as libc::c_int as size_t) << bitidx.wrapping_add(bitlen)
                        != 0 as libc::c_int as size_t
                {
                    bitlen = bitlen.wrapping_add(1);
                    bitlen;
                }
                let bitmap_index: mi_bitmap_index_t = mi_bitmap_index_create(i, bitidx);
                while bitlen > 0 as libc::c_int as size_t {
                    if _mi_bitmap_try_claim(
                        ((*arena).blocks_inuse).as_mut_ptr(),
                        (*arena).field_count,
                        bitlen,
                        bitmap_index,
                    ) {
                        break;
                    }
                    bitlen = bitlen.wrapping_sub(1);
                    bitlen;
                }
                if bitlen > 0 as libc::c_int as size_t {
                    purge = ::core::intrinsics::atomic_load_acquire(
                        &mut *((*arena).blocks_purge).offset(i as isize) as *mut mi_bitmap_field_t,
                    );
                    if !mi_arena_purge_range(arena, i, bitidx, bitlen, purge, stats) {
                        full_purge = 0 as libc::c_int != 0;
                    }
                    any_purged = 1 as libc::c_int != 0;
                    _mi_bitmap_unclaim(
                        ((*arena).blocks_inuse).as_mut_ptr(),
                        (*arena).field_count,
                        bitlen,
                        bitmap_index,
                    );
                }
                bitidx = bitidx.wrapping_add(bitlen.wrapping_add(1 as libc::c_int as size_t));
            }
        }
        i = i.wrapping_add(1);
        i;
    }
    if !full_purge {
        let delay: libc::c_long = mi_arena_purge_delay();
        let mut expected: mi_msecs_t = 0 as libc::c_int as mi_msecs_t;
        let fresh9 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
            &mut (*arena).purge_expire,
            *&mut expected,
            _mi_clock_now() + delay,
        );
        *&mut expected = fresh9.0;
        fresh9.1;
    }
    return any_purged;
}
unsafe extern "C" fn mi_arenas_try_purge(
    mut force: bool,
    mut visit_all: bool,
    mut stats: *mut mi_stats_t,
) {
    if _mi_preloading() as libc::c_int != 0
        || mi_arena_purge_delay() <= 0 as libc::c_int as libc::c_long
    {
        return;
    }
    let max_arena: size_t = ::core::intrinsics::atomic_load_acquire(&mut mi_arena_count);
    if max_arena == 0 as libc::c_int as size_t {
        return;
    }
    static mut purge_guard: mi_atomic_guard_t = 0;
    let mut _mi_guard_expected: uintptr_t = 0 as libc::c_int as uintptr_t;
    let mut _mi_guard_once: bool = 1 as libc::c_int != 0;
    while _mi_guard_once as libc::c_int != 0 && {
        let fresh10 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
            &mut purge_guard as *mut mi_atomic_guard_t,
            *(&mut _mi_guard_expected as *mut uintptr_t),
            1 as libc::c_int as uintptr_t,
        );
        *(&mut _mi_guard_expected as *mut uintptr_t) = fresh10.0;
        fresh10.1 as libc::c_int != 0
    } {
        let mut now: mi_msecs_t = _mi_clock_now();
        let mut max_purge_count: size_t = if visit_all as libc::c_int != 0 {
            max_arena
        } else {
            1 as libc::c_int as size_t
        };
        let mut i: size_t = 0 as libc::c_int as size_t;
        while i < max_arena {
            let mut arena: *mut mi_arena_t = ::core::intrinsics::atomic_load_acquire(
                &mut *mi_arenas.as_mut_ptr().offset(i as isize) as *mut *mut mi_arena_t,
            );
            if !arena.is_null() {
                if mi_arena_try_purge(arena, now, force, stats) {
                    if max_purge_count <= 1 as libc::c_int as size_t {
                        break;
                    }
                    max_purge_count = max_purge_count.wrapping_sub(1);
                    max_purge_count;
                }
            }
            i = i.wrapping_add(1);
            i;
        }
        ::core::intrinsics::atomic_store_release(&mut purge_guard, 0 as libc::c_int as uintptr_t);
        _mi_guard_once = 0 as libc::c_int != 0;
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_free(
    mut p: *mut libc::c_void,
    mut size: size_t,
    mut committed_size: size_t,
    mut memid: mi_memid_t,
    mut stats: *mut mi_stats_t,
) {
    if size > 0 as libc::c_int as size_t && !stats.is_null() {
    } else {
        _mi_assert_fail(
            b"size > 0 && stats != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4326 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_arena_free\0"))
                .as_ptr(),
        );
    };
    if committed_size <= size {
    } else {
        _mi_assert_fail(
            b"committed_size <= size\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4327 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_arena_free\0"))
                .as_ptr(),
        );
    };
    if p.is_null() {
        return;
    }
    if size == 0 as libc::c_int as size_t {
        return;
    }
    let all_committed: bool = committed_size == size;
    if mi_memkind_is_os(memid.memkind) {
        if !all_committed && committed_size > 0 as libc::c_int as size_t {
            _mi_stat_decrease(&mut _mi_stats_main.committed, committed_size);
        }
        _mi_os_free(p, size, memid, stats);
    } else if memid.memkind as libc::c_uint == MI_MEM_ARENA as libc::c_int as libc::c_uint {
        let mut arena_idx: size_t = 0;
        let mut bitmap_idx: size_t = 0;
        mi_arena_memid_indices(memid, &mut arena_idx, &mut bitmap_idx);
        if arena_idx < 132 as libc::c_int as size_t {
        } else {
            _mi_assert_fail(
                b"arena_idx < MI_MAX_ARENAS\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4342 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_arena_free\0"))
                    .as_ptr(),
            );
        };
        let mut arena: *mut mi_arena_t = ::core::intrinsics::atomic_load_acquire(
            &mut *mi_arenas.as_mut_ptr().offset(arena_idx as isize) as *mut *mut mi_arena_t,
        );
        if !arena.is_null() {
        } else {
            _mi_assert_fail(
                b"arena != NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4344 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_arena_free\0"))
                    .as_ptr(),
            );
        };
        let blocks: size_t = mi_block_count_of_size(size);
        if arena.is_null() {
            _mi_error_message(
                22 as libc::c_int,
                b"trying to free from an invalid arena: %p, size %zu, memid: 0x%zx\n\0" as *const u8
                    as *const libc::c_char,
                p,
                size,
                memid,
            );
            return;
        }
        if (*arena).field_count > mi_bitmap_index_field(bitmap_idx) {
        } else {
            _mi_assert_fail(
                b"arena->field_count > mi_bitmap_index_field(bitmap_idx)\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4350 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_arena_free\0"))
                    .as_ptr(),
            );
        };
        if (*arena).field_count <= mi_bitmap_index_field(bitmap_idx) {
            _mi_error_message(
                22 as libc::c_int,
                b"trying to free from an invalid arena block: %p, size %zu, memid: 0x%zx\n\0"
                    as *const u8 as *const libc::c_char,
                p,
                size,
                memid,
            );
            return;
        }
        if (*arena).memid.is_pinned as libc::c_int != 0 || ((*arena).blocks_committed).is_null() {
            if all_committed as libc::c_int != 0 {
            } else {
                _mi_assert_fail(
                    b"all_committed\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    4356 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(
                        b"_mi_arena_free\0",
                    ))
                    .as_ptr(),
                );
            };
        } else {
            if !((*arena).blocks_committed).is_null() {
            } else {
                _mi_assert_fail(
                    b"arena->blocks_committed != NULL\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    4359 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(
                        b"_mi_arena_free\0",
                    ))
                    .as_ptr(),
                );
            };
            if !((*arena).blocks_purge).is_null() {
            } else {
                _mi_assert_fail(
                    b"arena->blocks_purge != NULL\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    4360 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(
                        b"_mi_arena_free\0",
                    ))
                    .as_ptr(),
                );
            };
            if !all_committed {
                _mi_bitmap_unclaim_across(
                    (*arena).blocks_committed,
                    (*arena).field_count,
                    blocks,
                    bitmap_idx,
                );
                if committed_size > 0 as libc::c_int as size_t {
                    _mi_stat_decrease(&mut _mi_stats_main.committed, committed_size);
                }
            }
            mi_arena_schedule_purge(arena, bitmap_idx, blocks, stats);
        }
        let mut all_inuse: bool = _mi_bitmap_unclaim_across(
            ((*arena).blocks_inuse).as_mut_ptr(),
            (*arena).field_count,
            blocks,
            bitmap_idx,
        );
        if !all_inuse {
            _mi_error_message(
                11 as libc::c_int,
                b"trying to free an already freed arena block: %p, size %zu\n\0" as *const u8
                    as *const libc::c_char,
                p,
                size,
            );
            return;
        }
    } else {
        if (memid.memkind as libc::c_uint) < MI_MEM_OS as libc::c_int as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"memid.memkind < MI_MEM_OS\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4377 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_arena_free\0"))
                    .as_ptr(),
            );
        };
    }
    mi_arenas_try_purge(0 as libc::c_int != 0, 0 as libc::c_int != 0, stats);
}
unsafe extern "C" fn mi_arenas_unsafe_destroy() {
    let max_arena: size_t = ::core::intrinsics::atomic_load_relaxed(&mut mi_arena_count);
    let mut new_max_arena: size_t = 0 as libc::c_int as size_t;
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < max_arena {
        let mut arena: *mut mi_arena_t = ::core::intrinsics::atomic_load_acquire(
            &mut *mi_arenas.as_mut_ptr().offset(i as isize) as *mut *mut mi_arena_t,
        );
        if !arena.is_null() {
            mi_lock_done(&mut (*arena).abandoned_visit_lock);
            if !((*arena).start).is_null()
                && mi_memkind_is_os((*arena).memid.memkind) as libc::c_int != 0
            {
                ::core::intrinsics::atomic_store_release(
                    &mut *mi_arenas.as_mut_ptr().offset(i as isize) as *mut *mut mi_arena_t,
                    0 as *mut mi_arena_t,
                );
                _mi_os_free(
                    (*arena).start as *mut libc::c_void,
                    mi_arena_size(arena),
                    (*arena).memid,
                    &mut _mi_stats_main,
                );
            } else {
                new_max_arena = i;
            }
            _mi_arena_meta_free(
                arena as *mut libc::c_void,
                (*arena).meta_memid,
                (*arena).meta_size,
            );
        }
        i = i.wrapping_add(1);
        i;
    }
    let mut expected: size_t = max_arena;
    let fresh11 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
        &mut mi_arena_count,
        *&mut expected,
        new_max_arena,
    );
    *&mut expected = fresh11.0;
    fresh11.1;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arenas_collect(mut force_purge: bool, mut stats: *mut mi_stats_t) {
    mi_arenas_try_purge(force_purge, force_purge, stats);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_unsafe_destroy_all(mut stats: *mut mi_stats_t) {
    mi_arenas_unsafe_destroy();
    _mi_arenas_collect(1 as libc::c_int != 0, stats);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_arena_contains(mut p: *const libc::c_void) -> bool {
    let max_arena: size_t = ::core::intrinsics::atomic_load_relaxed(&mut mi_arena_count);
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < max_arena {
        let mut arena: *mut mi_arena_t = ::core::intrinsics::atomic_load_relaxed(
            &mut *mi_arenas.as_mut_ptr().offset(i as isize) as *mut *mut mi_arena_t,
        );
        if !arena.is_null()
            && (*arena).start <= p as *const uint8_t as *mut uint8_t
            && ((*arena).start).offset(mi_arena_block_size((*arena).block_count) as isize)
                > p as *const uint8_t as *mut uint8_t
        {
            return 1 as libc::c_int != 0;
        }
        i = i.wrapping_add(1);
        i;
    }
    return 0 as libc::c_int != 0;
}
unsafe extern "C" fn mi_arena_add(
    mut arena: *mut mi_arena_t,
    mut arena_id: *mut mi_arena_id_t,
    mut stats: *mut mi_stats_t,
) -> bool {
    if !arena.is_null() {
    } else {
        _mi_assert_fail(
            b"arena != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4419 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_arena_add\0")).as_ptr(),
        );
    };
    if (::core::intrinsics::atomic_load_relaxed(&mut (*arena).start as *mut *mut uint8_t)
        as uintptr_t as libc::c_ulonglong)
        .wrapping_rem(
            (1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
        )
        == 0 as libc::c_int as libc::c_ulonglong
    {
    } else {
        _mi_assert_fail(
            b"(uintptr_t)mi_atomic_load_ptr_relaxed(uint8_t,&arena->start) % MI_SEGMENT_ALIGN == 0\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4420 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 13],
                &[libc::c_char; 13],
            >(b"mi_arena_add\0"))
                .as_ptr(),
        );
    };
    if (*arena).block_count > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"arena->block_count > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4421 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_arena_add\0")).as_ptr(),
        );
    };
    if !arena_id.is_null() {
        *arena_id = -(1 as libc::c_int);
    }
    let mut i: size_t =
        ::core::intrinsics::atomic_xadd_acqrel(&mut mi_arena_count, 1 as libc::c_int as uintptr_t);
    if i >= 132 as libc::c_int as size_t {
        ::core::intrinsics::atomic_xsub_acqrel(&mut mi_arena_count, 1 as libc::c_int as uintptr_t);
        return 0 as libc::c_int != 0;
    }
    _mi_stat_counter_increase(&mut (*stats).arena_count, 1 as libc::c_int as size_t);
    (*arena).id = mi_arena_id_create(i);
    ::core::intrinsics::atomic_store_release(
        &mut *mi_arenas.as_mut_ptr().offset(i as isize) as *mut *mut mi_arena_t,
        arena,
    );
    if !arena_id.is_null() {
        *arena_id = (*arena).id;
    }
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn mi_manage_os_memory_ex2(
    mut start: *mut libc::c_void,
    mut size: size_t,
    mut is_large: bool,
    mut numa_node: libc::c_int,
    mut exclusive: bool,
    mut memid: mi_memid_t,
    mut arena_id: *mut mi_arena_id_t,
) -> bool {
    if !arena_id.is_null() {
        *arena_id = _mi_arena_id_none();
    }
    if (size as libc::c_ulonglong)
        < (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
    {
        return 0 as libc::c_int != 0;
    }
    if is_large {
        if memid.initially_committed as libc::c_int != 0 && memid.is_pinned as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"memid.initially_committed && memid.is_pinned\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4439 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                    b"mi_manage_os_memory_ex2\0",
                ))
                .as_ptr(),
            );
        };
    }
    let bcount: size_t = (size as libc::c_ulonglong).wrapping_div(
        (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
    ) as size_t;
    let fields: size_t = _mi_divide_up(
        bcount,
        (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t,
    );
    let bitmaps: size_t = (if memid.is_pinned as libc::c_int != 0 {
        3 as libc::c_int
    } else {
        5 as libc::c_int
    }) as size_t;
    let asize: size_t = (::core::mem::size_of::<mi_arena_t>() as libc::c_ulong).wrapping_add(
        (bitmaps * fields)
            .wrapping_mul(::core::mem::size_of::<mi_bitmap_field_t>() as libc::c_ulong),
    );
    let mut meta_memid: mi_memid_t = mi_memid_s {
        mem: C2RustUnnamed_0 {
            os: mi_memid_os_info {
                base: 0 as *mut libc::c_void,
                alignment: 0,
            },
        },
        is_pinned: false,
        initially_committed: false,
        initially_zero: false,
        memkind: MI_MEM_NONE,
    };
    let mut arena: *mut mi_arena_t =
        _mi_arena_meta_zalloc(asize, &mut meta_memid) as *mut mi_arena_t;
    if arena.is_null() {
        return 0 as libc::c_int != 0;
    }
    (*arena).id = _mi_arena_id_none();
    (*arena).memid = memid;
    (*arena).exclusive = exclusive;
    (*arena).meta_size = asize;
    (*arena).meta_memid = meta_memid;
    (*arena).block_count = bcount;
    (*arena).field_count = fields;
    (*arena).start = start as *mut uint8_t;
    (*arena).numa_node = numa_node;
    (*arena).is_large = is_large;
    (*arena).purge_expire = 0 as libc::c_int as libc::c_long;
    (*arena).search_idx = 0 as libc::c_int as libc::c_ulong;
    mi_lock_init(&mut (*arena).abandoned_visit_lock);
    (*arena).blocks_dirty = &mut *((*arena).blocks_inuse).as_mut_ptr().offset(fields as isize)
        as *mut mi_bitmap_field_t;
    (*arena).blocks_abandoned = &mut *((*arena).blocks_inuse)
        .as_mut_ptr()
        .offset((2 as libc::c_int as size_t * fields) as isize)
        as *mut mi_bitmap_field_t;
    (*arena).blocks_committed = if (*arena).memid.is_pinned as libc::c_int != 0 {
        0 as *mut mi_bitmap_field_t
    } else {
        &mut *((*arena).blocks_inuse)
            .as_mut_ptr()
            .offset((3 as libc::c_int as size_t * fields) as isize)
            as *mut mi_bitmap_field_t
    };
    (*arena).blocks_purge = if (*arena).memid.is_pinned as libc::c_int != 0 {
        0 as *mut mi_bitmap_field_t
    } else {
        &mut *((*arena).blocks_inuse)
            .as_mut_ptr()
            .offset((4 as libc::c_int as size_t * fields) as isize)
            as *mut mi_bitmap_field_t
    };
    if !((*arena).blocks_committed).is_null()
        && (*arena).memid.initially_committed as libc::c_int != 0
    {
        memset(
            (*arena).blocks_committed as *mut libc::c_void,
            0xff as libc::c_int,
            fields.wrapping_mul(::core::mem::size_of::<mi_bitmap_field_t>() as libc::c_ulong),
        );
    }
    let mut post: ptrdiff_t = (fields
        * (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t)
        .wrapping_sub(bcount) as ptrdiff_t;
    if post >= 0 as libc::c_int as ptrdiff_t {
    } else {
        _mi_assert_fail(
            b"post >= 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4469 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                b"mi_manage_os_memory_ex2\0",
            ))
            .as_ptr(),
        );
    };
    if post > 0 as libc::c_int as ptrdiff_t {
        let mut postidx: mi_bitmap_index_t = mi_bitmap_index_create(
            fields.wrapping_sub(1 as libc::c_int as size_t),
            ((8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as ptrdiff_t - post)
                as size_t,
        );
        _mi_bitmap_claim(
            ((*arena).blocks_inuse).as_mut_ptr(),
            fields,
            post as size_t,
            postidx,
            0 as *mut bool,
        );
    }
    return mi_arena_add(arena, arena_id, &mut _mi_stats_main);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_manage_os_memory_ex(
    mut start: *mut libc::c_void,
    mut size: size_t,
    mut is_committed: bool,
    mut is_large: bool,
    mut is_zero: bool,
    mut numa_node: libc::c_int,
    mut exclusive: bool,
    mut arena_id: *mut mi_arena_id_t,
) -> bool {
    let mut memid: mi_memid_t = _mi_memid_create(MI_MEM_EXTERNAL);
    memid.initially_committed = is_committed;
    memid.initially_zero = is_zero;
    memid.is_pinned = is_large;
    return mi_manage_os_memory_ex2(start, size, is_large, numa_node, exclusive, memid, arena_id);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reserve_os_memory_ex(
    mut size: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut exclusive: bool,
    mut arena_id: *mut mi_arena_id_t,
) -> libc::c_int {
    if !arena_id.is_null() {
        *arena_id = _mi_arena_id_none();
    }
    size = _mi_align_up(
        size,
        ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            as size_t,
    );
    let mut memid: mi_memid_t = mi_memid_s {
        mem: C2RustUnnamed_0 {
            os: mi_memid_os_info {
                base: 0 as *mut libc::c_void,
                alignment: 0,
            },
        },
        is_pinned: false,
        initially_committed: false,
        initially_zero: false,
        memkind: MI_MEM_NONE,
    };
    let mut start: *mut libc::c_void = _mi_os_alloc_aligned(
        size,
        ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            as size_t,
        commit,
        allow_large,
        &mut memid,
        &mut _mi_stats_main,
    );
    if start.is_null() {
        return 12 as libc::c_int;
    }
    let is_large: bool = memid.is_pinned;
    if !mi_manage_os_memory_ex2(
        start,
        size,
        is_large,
        -(1 as libc::c_int),
        exclusive,
        memid,
        arena_id,
    ) {
        _mi_os_free_ex(start, size, commit, memid, &mut _mi_stats_main);
        _mi_verbose_message(
            b"failed to reserve %zu KiB memory\n\0" as *const u8 as *const libc::c_char,
            _mi_divide_up(size, 1024 as libc::c_int as size_t),
        );
        return 12 as libc::c_int;
    }
    _mi_verbose_message(
        b"reserved %zu KiB memory%s\n\0" as *const u8 as *const libc::c_char,
        _mi_divide_up(size, 1024 as libc::c_int as size_t),
        if is_large as libc::c_int != 0 {
            b" (in large os pages)\0" as *const u8 as *const libc::c_char
        } else {
            b"\0" as *const u8 as *const libc::c_char
        },
    );
    return 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_manage_os_memory(
    mut start: *mut libc::c_void,
    mut size: size_t,
    mut is_committed: bool,
    mut is_large: bool,
    mut is_zero: bool,
    mut numa_node: libc::c_int,
) -> bool {
    return mi_manage_os_memory_ex(
        start,
        size,
        is_committed,
        is_large,
        is_zero,
        numa_node,
        0 as libc::c_int != 0,
        0 as *mut mi_arena_id_t,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reserve_os_memory(
    mut size: size_t,
    mut commit: bool,
    mut allow_large: bool,
) -> libc::c_int {
    return mi_reserve_os_memory_ex(
        size,
        commit,
        allow_large,
        0 as libc::c_int != 0,
        0 as *mut mi_arena_id_t,
    );
}
unsafe extern "C" fn mi_debug_show_bitmap(
    mut prefix: *const libc::c_char,
    mut header: *const libc::c_char,
    mut block_count: size_t,
    mut fields: *mut mi_bitmap_field_t,
    mut field_count: size_t,
) -> size_t {
    _mi_verbose_message(
        b"%s%s:\n\0" as *const u8 as *const libc::c_char,
        prefix,
        header,
    );
    let mut bcount: size_t = 0 as libc::c_int as size_t;
    let mut inuse_count: size_t = 0 as libc::c_int as size_t;
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < field_count {
        let mut buf: [libc::c_char; 65] = [0; 65];
        let mut field: uintptr_t = ::core::intrinsics::atomic_load_relaxed(
            &mut *fields.offset(i as isize) as *mut mi_bitmap_field_t,
        );
        let mut bit: size_t = 0 as libc::c_int as size_t;
        while bit < (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t {
            if bcount < block_count {
                let mut inuse: bool =
                    (1 as libc::c_int as uintptr_t) << bit & field != 0 as libc::c_int as uintptr_t;
                if inuse {
                    inuse_count = inuse_count.wrapping_add(1);
                    inuse_count;
                }
                buf[bit as usize] = (if inuse as libc::c_int != 0 {
                    'x' as i32
                } else {
                    '.' as i32
                }) as libc::c_char;
            } else {
                buf[bit as usize] = ' ' as i32 as libc::c_char;
            }
            bit = bit.wrapping_add(1);
            bit;
            bcount = bcount.wrapping_add(1);
            bcount;
        }
        buf[(8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as usize] =
            0 as libc::c_int as libc::c_char;
        _mi_verbose_message(
            b"%s  %s\n\0" as *const u8 as *const libc::c_char,
            prefix,
            buf.as_mut_ptr(),
        );
        i = i.wrapping_add(1);
        i;
    }
    _mi_verbose_message(
        b"%s  total ('x'): %zu\n\0" as *const u8 as *const libc::c_char,
        prefix,
        inuse_count,
    );
    return inuse_count;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_debug_show_arenas(
    mut show_inuse: bool,
    mut show_abandoned: bool,
    mut show_purge: bool,
) {
    let mut max_arenas: size_t = ::core::intrinsics::atomic_load_relaxed(&mut mi_arena_count);
    let mut inuse_total: size_t = 0 as libc::c_int as size_t;
    let mut abandoned_total: size_t = 0 as libc::c_int as size_t;
    let mut purge_total: size_t = 0 as libc::c_int as size_t;
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < max_arenas {
        let mut arena: *mut mi_arena_t = ::core::intrinsics::atomic_load_relaxed(
            &mut *mi_arenas.as_mut_ptr().offset(i as isize) as *mut *mut mi_arena_t,
        );
        if arena.is_null() {
            break;
        }
        _mi_verbose_message(
            b"arena %zu: %zu blocks of size %zuMiB (in %zu fields) %s\n\0" as *const u8
                as *const libc::c_char,
            i,
            (*arena).block_count,
            ((1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                .wrapping_div((1024 as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong)),
            (*arena).field_count,
            if (*arena).memid.is_pinned as libc::c_int != 0 {
                b", pinned\0" as *const u8 as *const libc::c_char
            } else {
                b"\0" as *const u8 as *const libc::c_char
            },
        );
        if show_inuse {
            inuse_total = inuse_total.wrapping_add(mi_debug_show_bitmap(
                b"  \0" as *const u8 as *const libc::c_char,
                b"inuse blocks\0" as *const u8 as *const libc::c_char,
                (*arena).block_count,
                ((*arena).blocks_inuse).as_mut_ptr(),
                (*arena).field_count,
            ));
        }
        if !((*arena).blocks_committed).is_null() {
            mi_debug_show_bitmap(
                b"  \0" as *const u8 as *const libc::c_char,
                b"committed blocks\0" as *const u8 as *const libc::c_char,
                (*arena).block_count,
                (*arena).blocks_committed,
                (*arena).field_count,
            );
        }
        if show_abandoned {
            abandoned_total = abandoned_total.wrapping_add(mi_debug_show_bitmap(
                b"  \0" as *const u8 as *const libc::c_char,
                b"abandoned blocks\0" as *const u8 as *const libc::c_char,
                (*arena).block_count,
                (*arena).blocks_abandoned,
                (*arena).field_count,
            ));
        }
        if show_purge as libc::c_int != 0 && !((*arena).blocks_purge).is_null() {
            purge_total = purge_total.wrapping_add(mi_debug_show_bitmap(
                b"  \0" as *const u8 as *const libc::c_char,
                b"purgeable blocks\0" as *const u8 as *const libc::c_char,
                (*arena).block_count,
                (*arena).blocks_purge,
                (*arena).field_count,
            ));
        }
        i = i.wrapping_add(1);
        i;
    }
    if show_inuse {
        _mi_verbose_message(
            b"total inuse blocks    : %zu\n\0" as *const u8 as *const libc::c_char,
            inuse_total,
        );
    }
    if show_abandoned {
        _mi_verbose_message(
            b"total abandoned blocks: %zu\n\0" as *const u8 as *const libc::c_char,
            abandoned_total,
        );
    }
    if show_purge {
        _mi_verbose_message(
            b"total purgeable blocks: %zu\n\0" as *const u8 as *const libc::c_char,
            purge_total,
        );
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reserve_huge_os_pages_at_ex(
    mut pages: size_t,
    mut numa_node: libc::c_int,
    mut timeout_msecs: size_t,
    mut exclusive: bool,
    mut arena_id: *mut mi_arena_id_t,
) -> libc::c_int {
    if !arena_id.is_null() {
        *arena_id = -(1 as libc::c_int);
    }
    if pages == 0 as libc::c_int as size_t {
        return 0 as libc::c_int;
    }
    if numa_node < -(1 as libc::c_int) {
        numa_node = -(1 as libc::c_int);
    }
    if numa_node >= 0 as libc::c_int {
        numa_node = (numa_node as size_t % _mi_os_numa_node_count()) as libc::c_int;
    }
    let mut hsize: size_t = 0 as libc::c_int as size_t;
    let mut pages_reserved: size_t = 0 as libc::c_int as size_t;
    let mut memid: mi_memid_t = mi_memid_s {
        mem: C2RustUnnamed_0 {
            os: mi_memid_os_info {
                base: 0 as *mut libc::c_void,
                alignment: 0,
            },
        },
        is_pinned: false,
        initially_committed: false,
        initially_zero: false,
        memkind: MI_MEM_NONE,
    };
    let mut p: *mut libc::c_void = _mi_os_alloc_huge_os_pages(
        pages,
        numa_node,
        timeout_msecs as mi_msecs_t,
        &mut pages_reserved,
        &mut hsize,
        &mut memid,
    );
    if p.is_null() || pages_reserved == 0 as libc::c_int as size_t {
        _mi_warning_message(
            b"failed to reserve %zu GiB huge pages\n\0" as *const u8 as *const libc::c_char,
            pages,
        );
        return 12 as libc::c_int;
    }
    _mi_verbose_message(
        b"numa node %i: reserved %zu GiB huge pages (of the %zu GiB requested)\n\0" as *const u8
            as *const libc::c_char,
        numa_node,
        pages_reserved,
        pages,
    );
    if !mi_manage_os_memory_ex2(
        p,
        hsize,
        1 as libc::c_int != 0,
        numa_node,
        exclusive,
        memid,
        arena_id,
    ) {
        _mi_os_free(p, hsize, memid, &mut _mi_stats_main);
        return 12 as libc::c_int;
    }
    return 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reserve_huge_os_pages_at(
    mut pages: size_t,
    mut numa_node: libc::c_int,
    mut timeout_msecs: size_t,
) -> libc::c_int {
    return mi_reserve_huge_os_pages_at_ex(
        pages,
        numa_node,
        timeout_msecs,
        0 as libc::c_int != 0,
        0 as *mut mi_arena_id_t,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reserve_huge_os_pages_interleave(
    mut pages: size_t,
    mut numa_nodes: size_t,
    mut timeout_msecs: size_t,
) -> libc::c_int {
    if pages == 0 as libc::c_int as size_t {
        return 0 as libc::c_int;
    }
    let mut numa_count: size_t = if numa_nodes > 0 as libc::c_int as size_t {
        numa_nodes
    } else {
        _mi_os_numa_node_count()
    };
    if numa_count <= 0 as libc::c_int as size_t {
        numa_count = 1 as libc::c_int as size_t;
    }
    let pages_per: size_t = pages / numa_count;
    let pages_mod: size_t = pages % numa_count;
    let timeout_per: size_t = if timeout_msecs == 0 as libc::c_int as size_t {
        0 as libc::c_int as size_t
    } else {
        (timeout_msecs / numa_count).wrapping_add(50 as libc::c_int as size_t)
    };
    let mut numa_node: size_t = 0 as libc::c_int as size_t;
    while numa_node < numa_count && pages > 0 as libc::c_int as size_t {
        let mut node_pages: size_t = pages_per;
        if numa_node < pages_mod {
            node_pages = node_pages.wrapping_add(1);
            node_pages;
        }
        let mut err: libc::c_int =
            mi_reserve_huge_os_pages_at(node_pages, numa_node as libc::c_int, timeout_per);
        if err != 0 {
            return err;
        }
        if pages < node_pages {
            pages = 0 as libc::c_int as size_t;
        } else {
            pages = pages.wrapping_sub(node_pages);
        }
        numa_node = numa_node.wrapping_add(1);
        numa_node;
    }
    return 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_reserve_huge_os_pages(
    mut pages: size_t,
    mut max_secs: libc::c_double,
    mut pages_reserved: *mut size_t,
) -> libc::c_int {
    _mi_warning_message(
        b"mi_reserve_huge_os_pages is deprecated: use mi_reserve_huge_os_pages_interleave/at instead\n\0"
            as *const u8 as *const libc::c_char,
    );
    if !pages_reserved.is_null() {
        *pages_reserved = 0 as libc::c_int as size_t;
    }
    let mut err: libc::c_int = mi_reserve_huge_os_pages_interleave(
        pages,
        0 as libc::c_int as size_t,
        (max_secs * 1000.0f64) as size_t,
    );
    if err == 0 as libc::c_int && !pages_reserved.is_null() {
        *pages_reserved = pages;
    }
    return err;
}
#[inline]
unsafe extern "C" fn mi_bitmap_mask_(mut count: size_t, mut bitidx: size_t) -> size_t {
    if count.wrapping_add(bitidx)
        <= (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t
    {
    } else {
        _mi_assert_fail(
            b"count + bitidx <= MI_BITMAP_FIELD_BITS\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4606 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_bitmap_mask_\0"))
                .as_ptr(),
        );
    };
    if count > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"count > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4607 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_bitmap_mask_\0"))
                .as_ptr(),
        );
    };
    if count >= (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t {
        return !(0 as libc::c_int as size_t);
    }
    if count == 0 as libc::c_int as size_t {
        return 0 as libc::c_int as size_t;
    }
    return ((1 as libc::c_int as size_t) << count).wrapping_sub(1 as libc::c_int as size_t)
        << bitidx;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_try_find_claim_field(
    mut bitmap: mi_bitmap_t,
    mut idx: size_t,
    count: size_t,
    mut bitmap_idx: *mut mi_bitmap_index_t,
) -> bool {
    if !bitmap_idx.is_null() {
    } else {
        _mi_assert_fail(
            b"bitmap_idx != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4614 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                b"_mi_bitmap_try_find_claim_field\0",
            ))
            .as_ptr(),
        );
    };
    if count <= (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t {
    } else {
        _mi_assert_fail(
            b"count <= MI_BITMAP_FIELD_BITS\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4615 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                b"_mi_bitmap_try_find_claim_field\0",
            ))
            .as_ptr(),
        );
    };
    let mut field: *mut mi_bitmap_field_t =
        &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t;
    let mut map: size_t = ::core::intrinsics::atomic_load_relaxed(field);
    if map == !(0 as libc::c_int as size_t) {
        return 0 as libc::c_int != 0;
    }
    let mask: size_t = mi_bitmap_mask_(count, 0 as libc::c_int as size_t);
    let bitidx_max: size_t = ((8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int))
        as size_t)
        .wrapping_sub(count);
    let mut bitidx: size_t = mi_ctz(!map);
    let mut m: size_t = mask << bitidx;
    while bitidx <= bitidx_max {
        let mapm: size_t = map & m;
        if mapm == 0 as libc::c_int as size_t {
            if m >> bitidx == mask {
            } else {
                _mi_assert_fail(
                    b"(m >> bitidx) == mask\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    4630 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                        b"_mi_bitmap_try_find_claim_field\0",
                    ))
                    .as_ptr(),
                );
            };
            let newmap: size_t = map | m;
            if (newmap ^ map) >> bitidx == mask {
            } else {
                _mi_assert_fail(
                    b"(newmap^map) >> bitidx == mask\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    4632 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                        b"_mi_bitmap_try_find_claim_field\0",
                    ))
                    .as_ptr(),
                );
            };
            let fresh12 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
                field,
                *(&mut map as *mut size_t),
                newmap,
            );
            *(&mut map as *mut size_t) = fresh12.0;
            if !fresh12.1 {
                continue;
            }
            *bitmap_idx = mi_bitmap_index_create(idx, bitidx);
            return 1 as libc::c_int != 0;
        } else {
            if mapm != 0 as libc::c_int as size_t {
            } else {
                _mi_assert_fail(
                    b"mapm != 0\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    4643 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                        b"_mi_bitmap_try_find_claim_field\0",
                    ))
                    .as_ptr(),
                );
            };
            let shift: size_t = if count == 1 as libc::c_int as size_t {
                1 as libc::c_int as size_t
            } else {
                ((((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t)
                    .wrapping_sub(mi_clz(mapm))
                    .wrapping_sub(bitidx)
            };
            if shift > 0 as libc::c_int as size_t && shift <= count {
            } else {
                _mi_assert_fail(
                    b"shift > 0 && shift <= count\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    4645 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                        b"_mi_bitmap_try_find_claim_field\0",
                    ))
                    .as_ptr(),
                );
            };
            bitidx = bitidx.wrapping_add(shift);
            m <<= shift;
        }
    }
    return 0 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_try_find_from_claim(
    mut bitmap: mi_bitmap_t,
    bitmap_fields: size_t,
    start_field_idx: size_t,
    count: size_t,
    mut bitmap_idx: *mut mi_bitmap_index_t,
) -> bool {
    let mut idx: size_t = start_field_idx;
    let mut visited: size_t = 0 as libc::c_int as size_t;
    while visited < bitmap_fields {
        if idx >= bitmap_fields {
            idx = 0 as libc::c_int as size_t;
        }
        if _mi_bitmap_try_find_claim_field(bitmap, idx, count, bitmap_idx) {
            return 1 as libc::c_int != 0;
        }
        visited = visited.wrapping_add(1);
        visited;
        idx = idx.wrapping_add(1);
        idx;
    }
    return 0 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_unclaim(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
) -> bool {
    let idx: size_t = mi_bitmap_index_field(bitmap_idx);
    let bitidx: size_t = mi_bitmap_index_bit_in_field(bitmap_idx);
    let mask: size_t = mi_bitmap_mask_(count, bitidx);
    if bitmap_fields > idx {
    } else {
        _mi_assert_fail(
            b"bitmap_fields > idx\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4669 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_bitmap_unclaim\0"))
                .as_ptr(),
        );
    };
    let prev: size_t = ::core::intrinsics::atomic_and_acqrel(
        &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t,
        !mask,
    );
    return prev & mask == mask;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_claim(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
    mut any_zero: *mut bool,
) -> bool {
    let idx: size_t = mi_bitmap_index_field(bitmap_idx);
    let bitidx: size_t = mi_bitmap_index_bit_in_field(bitmap_idx);
    let mask: size_t = mi_bitmap_mask_(count, bitidx);
    if bitmap_fields > idx {
    } else {
        _mi_assert_fail(
            b"bitmap_fields > idx\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4677 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_bitmap_claim\0"))
                .as_ptr(),
        );
    };
    let mut prev: size_t = ::core::intrinsics::atomic_or_acqrel(
        &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t,
        mask,
    );
    if !any_zero.is_null() {
        *any_zero = prev & mask != mask;
    }
    return prev & mask == 0 as libc::c_int as size_t;
}
unsafe extern "C" fn mi_bitmap_is_claimedx(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
    mut any_ones: *mut bool,
) -> bool {
    let idx: size_t = mi_bitmap_index_field(bitmap_idx);
    let bitidx: size_t = mi_bitmap_index_bit_in_field(bitmap_idx);
    let mask: size_t = mi_bitmap_mask_(count, bitidx);
    if bitmap_fields > idx {
    } else {
        _mi_assert_fail(
            b"bitmap_fields > idx\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4686 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_bitmap_is_claimedx\0"))
                .as_ptr(),
        );
    };
    let field: size_t = ::core::intrinsics::atomic_load_relaxed(
        &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t
    );
    if !any_ones.is_null() {
        *any_ones = field & mask != 0 as libc::c_int as size_t;
    }
    return field & mask == mask;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_try_claim(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
) -> bool {
    let idx: size_t = mi_bitmap_index_field(bitmap_idx);
    let bitidx: size_t = mi_bitmap_index_bit_in_field(bitmap_idx);
    let mask: size_t = mi_bitmap_mask_(count, bitidx);
    if bitmap_fields > idx {
    } else {
        _mi_assert_fail(
            b"bitmap_fields > idx\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4695 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_bitmap_try_claim\0"))
                .as_ptr(),
        );
    };
    let mut expected: size_t = ::core::intrinsics::atomic_load_relaxed(
        &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t,
    );
    loop {
        if expected & mask != 0 as libc::c_int as size_t {
            return 0 as libc::c_int != 0;
        }
        let fresh13 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
            &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t,
            *(&mut expected as *mut size_t),
            expected | mask,
        );
        *(&mut expected as *mut size_t) = fresh13.0;
        if fresh13.1 {
            break;
        }
    }
    if expected & mask == 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"(expected & mask) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4701 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_bitmap_try_claim\0"))
                .as_ptr(),
        );
    };
    return 1 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_is_claimed(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
) -> bool {
    return mi_bitmap_is_claimedx(bitmap, bitmap_fields, count, bitmap_idx, 0 as *mut bool);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_is_any_claimed(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
) -> bool {
    let mut any_ones: bool = false;
    mi_bitmap_is_claimedx(bitmap, bitmap_fields, count, bitmap_idx, &mut any_ones);
    return any_ones;
}
unsafe extern "C" fn mi_bitmap_try_find_claim_field_across(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut idx: size_t,
    count: size_t,
    retries: size_t,
    mut bitmap_idx: *mut mi_bitmap_index_t,
    mut stats: *mut mi_stats_t,
) -> bool {
    let mut current_block: u64;
    if !bitmap_idx.is_null() {
    } else {
        _mi_assert_fail(
            b"bitmap_idx != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4714 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 38], &[libc::c_char; 38]>(
                b"mi_bitmap_try_find_claim_field_across\0",
            ))
            .as_ptr(),
        );
    };
    let mut field: *mut mi_bitmap_field_t =
        &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t;
    let mut map: size_t = ::core::intrinsics::atomic_load_relaxed(field);
    let initial: size_t = mi_clz(map);
    if initial <= (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t {
    } else {
        _mi_assert_fail(
            b"initial <= MI_BITMAP_FIELD_BITS\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4718 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 38], &[libc::c_char; 38]>(
                b"mi_bitmap_try_find_claim_field_across\0",
            ))
            .as_ptr(),
        );
    };
    if initial == 0 as libc::c_int as size_t {
        return 0 as libc::c_int != 0;
    }
    if initial >= count {
        return _mi_bitmap_try_find_claim_field(bitmap, idx, count, bitmap_idx);
    }
    if _mi_divide_up(
        count.wrapping_sub(initial),
        (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t,
    ) >= bitmap_fields.wrapping_sub(idx)
    {
        return 0 as libc::c_int != 0;
    }
    let mut found: size_t = initial;
    let mut mask: size_t = 0 as libc::c_int as size_t;
    while found < count {
        field = field.offset(1);
        field;
        map = ::core::intrinsics::atomic_load_relaxed(field);
        let mask_bits: size_t = if found
            .wrapping_add((8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t)
            <= count
        {
            (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t
        } else {
            count.wrapping_sub(found)
        };
        if mask_bits > 0 as libc::c_int as size_t
            && mask_bits <= (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t
        {
        } else {
            _mi_assert_fail(
                b"mask_bits > 0 && mask_bits <= MI_BITMAP_FIELD_BITS\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4728 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 38], &[libc::c_char; 38]>(
                    b"mi_bitmap_try_find_claim_field_across\0",
                ))
                .as_ptr(),
            );
        };
        mask = mi_bitmap_mask_(mask_bits, 0 as libc::c_int as size_t);
        if map & mask != 0 as libc::c_int as size_t {
            return 0 as libc::c_int != 0;
        }
        found = found.wrapping_add(mask_bits);
    }
    if field < &mut *bitmap.offset(bitmap_fields as isize) as *mut mi_bitmap_field_t {
    } else {
        _mi_assert_fail(
            b"field < &bitmap[bitmap_fields]\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4733 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 38], &[libc::c_char; 38]>(
                b"mi_bitmap_try_find_claim_field_across\0",
            ))
            .as_ptr(),
        );
    };
    let final_field: *mut mi_bitmap_field_t = field;
    let final_mask: size_t = mask;
    let initial_field: *mut mi_bitmap_field_t =
        &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t;
    let initial_idx: size_t = ((8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int))
        as size_t)
        .wrapping_sub(initial);
    let initial_mask: size_t = mi_bitmap_mask_(initial, initial_idx);
    let mut newmap: size_t = 0;
    field = initial_field;
    map = ::core::intrinsics::atomic_load_relaxed(field);
    loop {
        newmap = map | initial_mask;
        if map & initial_mask != 0 as libc::c_int as size_t {
            current_block = 6638229613705292009;
            break;
        }
        let fresh14 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
            field,
            *(&mut map as *mut size_t),
            newmap,
        );
        *(&mut map as *mut size_t) = fresh14.0;
        if fresh14.1 {
            current_block = 14576567515993809846;
            break;
        }
    }
    's_114: loop {
        match current_block {
            6638229613705292009 => {
                loop {
                    field = field.offset(-1);
                    if !(field > initial_field) {
                        break;
                    }
                    newmap = 0 as libc::c_int as size_t;
                    map = mi_bitmap_mask_(
                        (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t,
                        0 as libc::c_int as size_t,
                    );
                    if ::core::intrinsics::atomic_load_relaxed(field) == map {
                    } else {
                        _mi_assert_fail(
                            b"mi_atomic_load_relaxed(field) == map\0" as *const u8
                                as *const libc::c_char,
                            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                            4764 as libc::c_int as libc::c_uint,
                            (*::core::mem::transmute::<&[u8; 38], &[libc::c_char; 38]>(
                                b"mi_bitmap_try_find_claim_field_across\0",
                            ))
                            .as_ptr(),
                        );
                    };
                    ::core::intrinsics::atomic_store_release(field, newmap);
                }
                if field == initial_field {
                    map = ::core::intrinsics::atomic_load_relaxed(field);
                    loop {
                        if map & initial_mask == initial_mask {
                        } else {
                            _mi_assert_fail(
                                b"(map & initial_mask) == initial_mask\0" as *const u8
                                    as *const libc::c_char,
                                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                                4770 as libc::c_int as libc::c_uint,
                                (*::core::mem::transmute::<&[u8; 38], &[libc::c_char; 38]>(
                                    b"mi_bitmap_try_find_claim_field_across\0",
                                ))
                                .as_ptr(),
                            );
                        };
                        newmap = map & !initial_mask;
                        let fresh17 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
                            field,
                            *(&mut map as *mut size_t),
                            newmap,
                        );
                        *(&mut map as *mut size_t) = fresh17.0;
                        if fresh17.1 {
                            break;
                        }
                    }
                }
                _mi_stat_counter_increase(
                    &mut (*stats).arena_rollback_count,
                    1 as libc::c_int as size_t,
                );
                if retries <= 2 as libc::c_int as size_t {
                    return mi_bitmap_try_find_claim_field_across(
                        bitmap,
                        bitmap_fields,
                        idx,
                        count,
                        retries.wrapping_add(1 as libc::c_int as size_t),
                        bitmap_idx,
                        stats,
                    );
                } else {
                    return 0 as libc::c_int != 0;
                }
            }
            _ => {
                field = field.offset(1);
                if field < final_field {
                    newmap = mi_bitmap_mask_(
                        (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t,
                        0 as libc::c_int as size_t,
                    );
                    map = 0 as libc::c_int as size_t;
                    let fresh15 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
                        field,
                        *(&mut map as *mut size_t),
                        newmap,
                    );
                    *(&mut map as *mut size_t) = fresh15.0;
                    if !fresh15.1 {
                        current_block = 6638229613705292009;
                    } else {
                        current_block = 14576567515993809846;
                    }
                } else {
                    if field == final_field {
                    } else {
                        _mi_assert_fail(
                            b"field == final_field\0" as *const u8 as *const libc::c_char,
                            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                            4751 as libc::c_int as libc::c_uint,
                            (*::core::mem::transmute::<&[u8; 38], &[libc::c_char; 38]>(
                                b"mi_bitmap_try_find_claim_field_across\0",
                            ))
                            .as_ptr(),
                        );
                    };
                    map = ::core::intrinsics::atomic_load_relaxed(field);
                    loop {
                        newmap = map | final_mask;
                        if map & final_mask != 0 as libc::c_int as size_t {
                            current_block = 6638229613705292009;
                            continue 's_114;
                        }
                        let fresh16 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
                            field,
                            *(&mut map as *mut size_t),
                            newmap,
                        );
                        *(&mut map as *mut size_t) = fresh16.0;
                        if fresh16.1 {
                            break;
                        }
                    }
                    _mi_stat_counter_increase(
                        &mut (*stats).arena_crossover_count,
                        1 as libc::c_int as size_t,
                    );
                    *bitmap_idx = mi_bitmap_index_create(idx, initial_idx);
                    return 1 as libc::c_int != 0;
                }
            }
        }
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_try_find_from_claim_across(
    mut bitmap: mi_bitmap_t,
    bitmap_fields: size_t,
    start_field_idx: size_t,
    count: size_t,
    mut bitmap_idx: *mut mi_bitmap_index_t,
    mut stats: *mut mi_stats_t,
) -> bool {
    if count > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"count > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4783 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 38], &[libc::c_char; 38]>(
                b"_mi_bitmap_try_find_from_claim_across\0",
            ))
            .as_ptr(),
        );
    };
    if count <= 2 as libc::c_int as size_t {
        return _mi_bitmap_try_find_from_claim(
            bitmap,
            bitmap_fields,
            start_field_idx,
            count,
            bitmap_idx,
        );
    }
    let mut idx: size_t = start_field_idx;
    let mut visited: size_t = 0 as libc::c_int as size_t;
    while visited < bitmap_fields {
        if idx >= bitmap_fields {
            idx = 0 as libc::c_int as size_t;
        }
        if mi_bitmap_try_find_claim_field_across(
            bitmap,
            bitmap_fields,
            idx,
            count,
            0 as libc::c_int as size_t,
            bitmap_idx,
            stats,
        ) {
            return 1 as libc::c_int != 0;
        }
        visited = visited.wrapping_add(1);
        visited;
        idx = idx.wrapping_add(1);
        idx;
    }
    return 0 as libc::c_int != 0;
}
unsafe extern "C" fn mi_bitmap_mask_across(
    mut bitmap_idx: mi_bitmap_index_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut pre_mask: *mut size_t,
    mut mid_mask: *mut size_t,
    mut post_mask: *mut size_t,
) -> size_t {
    let bitidx: size_t = mi_bitmap_index_bit_in_field(bitmap_idx);
    if (bitidx.wrapping_add(count)
        <= (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t)
        as libc::c_int as libc::c_long
        != 0
    {
        *pre_mask = mi_bitmap_mask_(count, bitidx);
        *mid_mask = 0 as libc::c_int as size_t;
        *post_mask = 0 as libc::c_int as size_t;
        if mi_bitmap_index_field(bitmap_idx) < bitmap_fields {
        } else {
            _mi_assert_fail(
                b"mi_bitmap_index_field(bitmap_idx) < bitmap_fields\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4803 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"mi_bitmap_mask_across\0",
                ))
                .as_ptr(),
            );
        };
        return 0 as libc::c_int as size_t;
    } else {
        let pre_bits: size_t = ((8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int))
            as size_t)
            .wrapping_sub(bitidx);
        if pre_bits < count {
        } else {
            _mi_assert_fail(
                b"pre_bits < count\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4808 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"mi_bitmap_mask_across\0",
                ))
                .as_ptr(),
            );
        };
        *pre_mask = mi_bitmap_mask_(pre_bits, bitidx);
        count = count.wrapping_sub(pre_bits);
        let mid_count: size_t =
            count / (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t;
        *mid_mask = !(0 as libc::c_int as size_t);
        count = count % (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as size_t;
        *post_mask = if count == 0 as libc::c_int as size_t {
            0 as libc::c_int as size_t
        } else {
            mi_bitmap_mask_(count, 0 as libc::c_int as size_t)
        };
        if (mi_bitmap_index_field(bitmap_idx))
            .wrapping_add(mid_count)
            .wrapping_add(
                (if count == 0 as libc::c_int as size_t {
                    0 as libc::c_int
                } else {
                    1 as libc::c_int
                }) as size_t,
            )
            < bitmap_fields
        {
        } else {
            _mi_assert_fail(
                b"mi_bitmap_index_field(bitmap_idx) + mid_count + (count==0 ? 0 : 1) < bitmap_fields\0"
                    as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                4815 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<
                    &[u8; 22],
                    &[libc::c_char; 22],
                >(b"mi_bitmap_mask_across\0"))
                    .as_ptr(),
            );
        };
        return mid_count;
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_unclaim_across(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
) -> bool {
    let mut idx: size_t = mi_bitmap_index_field(bitmap_idx);
    let mut pre_mask: size_t = 0;
    let mut mid_mask: size_t = 0;
    let mut post_mask: size_t = 0;
    let mut mid_count: size_t = mi_bitmap_mask_across(
        bitmap_idx,
        bitmap_fields,
        count,
        &mut pre_mask,
        &mut mid_mask,
        &mut post_mask,
    );
    let mut all_one: bool = 1 as libc::c_int != 0;
    let mut field: *mut mi_bitmap_field_t =
        &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t;
    let fresh18 = field;
    field = field.offset(1);
    let mut prev: size_t = ::core::intrinsics::atomic_and_acqrel(fresh18, !pre_mask);
    if prev & pre_mask != pre_mask {
        all_one = 0 as libc::c_int != 0;
    }
    loop {
        let fresh19 = mid_count;
        mid_count = mid_count.wrapping_sub(1);
        if !(fresh19 > 0 as libc::c_int as size_t) {
            break;
        }
        let fresh20 = field;
        field = field.offset(1);
        prev = ::core::intrinsics::atomic_and_acqrel(fresh20, !mid_mask);
        if prev & mid_mask != mid_mask {
            all_one = 0 as libc::c_int != 0;
        }
    }
    if post_mask != 0 as libc::c_int as size_t {
        prev = ::core::intrinsics::atomic_and_acqrel(field, !post_mask);
        if prev & post_mask != post_mask {
            all_one = 0 as libc::c_int != 0;
        }
    }
    return all_one;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_claim_across(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
    mut pany_zero: *mut bool,
) -> bool {
    let mut idx: size_t = mi_bitmap_index_field(bitmap_idx);
    let mut pre_mask: size_t = 0;
    let mut mid_mask: size_t = 0;
    let mut post_mask: size_t = 0;
    let mut mid_count: size_t = mi_bitmap_mask_across(
        bitmap_idx,
        bitmap_fields,
        count,
        &mut pre_mask,
        &mut mid_mask,
        &mut post_mask,
    );
    let mut all_zero: bool = 1 as libc::c_int != 0;
    let mut any_zero: bool = 0 as libc::c_int != 0;
    let mut field: *mut size_t = &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t;
    let fresh21 = field;
    field = field.offset(1);
    let mut prev: size_t = ::core::intrinsics::atomic_or_acqrel(fresh21, pre_mask);
    if prev & pre_mask != 0 as libc::c_int as size_t {
        all_zero = 0 as libc::c_int != 0;
    }
    if prev & pre_mask != pre_mask {
        any_zero = 1 as libc::c_int != 0;
    }
    loop {
        let fresh22 = mid_count;
        mid_count = mid_count.wrapping_sub(1);
        if !(fresh22 > 0 as libc::c_int as size_t) {
            break;
        }
        let fresh23 = field;
        field = field.offset(1);
        prev = ::core::intrinsics::atomic_or_acqrel(fresh23, mid_mask);
        if prev & mid_mask != 0 as libc::c_int as size_t {
            all_zero = 0 as libc::c_int != 0;
        }
        if prev & mid_mask != mid_mask {
            any_zero = 1 as libc::c_int != 0;
        }
    }
    if post_mask != 0 as libc::c_int as size_t {
        prev = ::core::intrinsics::atomic_or_acqrel(field, post_mask);
        if prev & post_mask != 0 as libc::c_int as size_t {
            all_zero = 0 as libc::c_int != 0;
        }
        if prev & post_mask != post_mask {
            any_zero = 1 as libc::c_int != 0;
        }
    }
    if !pany_zero.is_null() {
        *pany_zero = any_zero;
    }
    return all_zero;
}
unsafe extern "C" fn mi_bitmap_is_claimedx_across(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
    mut pany_ones: *mut bool,
) -> bool {
    let mut idx: size_t = mi_bitmap_index_field(bitmap_idx);
    let mut pre_mask: size_t = 0;
    let mut mid_mask: size_t = 0;
    let mut post_mask: size_t = 0;
    let mut mid_count: size_t = mi_bitmap_mask_across(
        bitmap_idx,
        bitmap_fields,
        count,
        &mut pre_mask,
        &mut mid_mask,
        &mut post_mask,
    );
    let mut all_ones: bool = 1 as libc::c_int != 0;
    let mut any_ones: bool = 0 as libc::c_int != 0;
    let mut field: *mut mi_bitmap_field_t =
        &mut *bitmap.offset(idx as isize) as *mut mi_bitmap_field_t;
    let fresh24 = field;
    field = field.offset(1);
    let mut prev: size_t = ::core::intrinsics::atomic_load_relaxed(fresh24);
    if prev & pre_mask != pre_mask {
        all_ones = 0 as libc::c_int != 0;
    }
    if prev & pre_mask != 0 as libc::c_int as size_t {
        any_ones = 1 as libc::c_int != 0;
    }
    loop {
        let fresh25 = mid_count;
        mid_count = mid_count.wrapping_sub(1);
        if !(fresh25 > 0 as libc::c_int as size_t) {
            break;
        }
        let fresh26 = field;
        field = field.offset(1);
        prev = ::core::intrinsics::atomic_load_relaxed(fresh26);
        if prev & mid_mask != mid_mask {
            all_ones = 0 as libc::c_int != 0;
        }
        if prev & mid_mask != 0 as libc::c_int as size_t {
            any_ones = 1 as libc::c_int != 0;
        }
    }
    if post_mask != 0 as libc::c_int as size_t {
        prev = ::core::intrinsics::atomic_load_relaxed(field);
        if prev & post_mask != post_mask {
            all_ones = 0 as libc::c_int != 0;
        }
        if prev & post_mask != 0 as libc::c_int as size_t {
            any_ones = 1 as libc::c_int != 0;
        }
    }
    if !pany_ones.is_null() {
        *pany_ones = any_ones;
    }
    return all_ones;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_is_claimed_across(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
) -> bool {
    return mi_bitmap_is_claimedx_across(bitmap, bitmap_fields, count, bitmap_idx, 0 as *mut bool);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bitmap_is_any_claimed_across(
    mut bitmap: mi_bitmap_t,
    mut bitmap_fields: size_t,
    mut count: size_t,
    mut bitmap_idx: mi_bitmap_index_t,
) -> bool {
    let mut any_ones: bool = false;
    mi_bitmap_is_claimedx_across(bitmap, bitmap_fields, count, bitmap_idx, &mut any_ones);
    return any_ones;
}
unsafe extern "C" fn mi_heap_visit_pages(
    mut heap: *mut mi_heap_t,
    mut fn_0: Option<heap_page_visitor_fun>,
    mut arg1: *mut libc::c_void,
    mut arg2: *mut libc::c_void,
) -> bool {
    if heap.is_null() || (*heap).page_count == 0 as libc::c_int as size_t {
        return 0 as libc::c_int != 0;
    }
    let mut total: size_t = (*heap).page_count;
    let mut count: size_t = 0 as libc::c_int as size_t;
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i <= (73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) as size_t {
        let mut pq: *mut mi_page_queue_t =
            &mut *((*heap).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_queue_t;
        let mut page: *mut mi_page_t = (*pq).first;
        while !page.is_null() {
            let mut next: *mut mi_page_t = (*page).next;
            if mi_page_heap(page) == heap {
            } else {
                _mi_assert_fail(
                    b"mi_page_heap(page) == heap\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    4914 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(
                        b"mi_heap_visit_pages\0",
                    ))
                    .as_ptr(),
                );
            };
            count = count.wrapping_add(1);
            count;
            if !fn_0.expect("non-null function pointer")(heap, pq, page, arg1, arg2) {
                return 0 as libc::c_int != 0;
            }
            page = next;
        }
        i = i.wrapping_add(1);
        i;
    }
    if count == total {
    } else {
        _mi_assert_fail(
            b"count == total\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4922 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_heap_visit_pages\0"))
                .as_ptr(),
        );
    };
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn mi_heap_page_is_valid(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
    mut arg1: *mut libc::c_void,
    mut arg2: *mut libc::c_void,
) -> bool {
    if mi_page_heap(page) == heap {
    } else {
        _mi_assert_fail(
            b"mi_page_heap(page) == heap\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4930 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_heap_page_is_valid\0"))
                .as_ptr(),
        );
    };
    let mut segment: *mut mi_segment_t = _mi_page_segment(page);
    if (*segment).thread_id == (*heap).thread_id {
    } else {
        _mi_assert_fail(
            b"segment->thread_id == heap->thread_id\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4932 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_heap_page_is_valid\0"))
                .as_ptr(),
        );
    };
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn mi_heap_page_collect(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
    mut arg_collect: *mut libc::c_void,
    mut arg2: *mut libc::c_void,
) -> bool {
    if mi_heap_page_is_valid(
        heap,
        pq,
        page,
        0 as *mut libc::c_void,
        0 as *mut libc::c_void,
    ) as libc::c_int
        != 0
    {
    } else {
        _mi_assert_fail(
            b"mi_heap_page_is_valid(heap, pq, page, NULL, NULL)\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4952 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_heap_page_collect\0"))
                .as_ptr(),
        );
    };
    let mut collect: mi_collect_t = *(arg_collect as *mut mi_collect_t);
    _mi_page_free_collect(
        page,
        collect as libc::c_uint >= MI_FORCE as libc::c_int as libc::c_uint,
    );
    if mi_page_all_free(page) {
        _mi_page_free(
            page,
            pq,
            collect as libc::c_uint >= MI_FORCE as libc::c_int as libc::c_uint,
        );
    } else if collect as libc::c_uint == MI_ABANDON as libc::c_int as libc::c_uint {
        _mi_page_abandon(page, pq);
    }
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn mi_heap_page_never_delayed_free(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
    mut arg1: *mut libc::c_void,
    mut arg2: *mut libc::c_void,
) -> bool {
    _mi_page_use_delayed_free(page, MI_NEVER_DELAYED_FREE, 0 as libc::c_int != 0);
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn mi_heap_collect_ex(mut heap: *mut mi_heap_t, mut collect: mi_collect_t) {
    if heap.is_null() || !mi_heap_is_initialized(heap) {
        return;
    }
    let force: bool = collect as libc::c_uint >= MI_FORCE as libc::c_int as libc::c_uint;
    _mi_deferred_free(heap, force);
    let is_main_thread: bool =
        _mi_is_main_thread() as libc::c_int != 0 && (*heap).thread_id == _mi_thread_id();
    if collect as libc::c_uint >= MI_FORCE as libc::c_int as libc::c_uint
        && is_main_thread as libc::c_int != 0
        && mi_heap_is_backing(heap) as libc::c_int != 0
        && !(*heap).no_reclaim
    {
        _mi_abandoned_reclaim_all(heap, &mut (*(*heap).tld).segments);
    }
    if collect as libc::c_uint == MI_ABANDON as libc::c_int as libc::c_uint {
        mi_heap_visit_pages(
            heap,
            Some(
                mi_heap_page_never_delayed_free
                    as unsafe extern "C" fn(
                        *mut mi_heap_t,
                        *mut mi_page_queue_t,
                        *mut mi_page_t,
                        *mut libc::c_void,
                        *mut libc::c_void,
                    ) -> bool,
            ),
            0 as *mut libc::c_void,
            0 as *mut libc::c_void,
        );
    }
    _mi_heap_delayed_free_all(heap);
    _mi_heap_collect_retired(heap, force);
    mi_heap_visit_pages(
        heap,
        Some(
            mi_heap_page_collect
                as unsafe extern "C" fn(
                    *mut mi_heap_t,
                    *mut mi_page_queue_t,
                    *mut mi_page_t,
                    *mut libc::c_void,
                    *mut libc::c_void,
                ) -> bool,
        ),
        &mut collect as *mut mi_collect_t as *mut libc::c_void,
        0 as *mut libc::c_void,
    );
    if collect as libc::c_uint != MI_ABANDON as libc::c_int as libc::c_uint
        || (::core::intrinsics::atomic_load_acquire(
            &mut (*heap).thread_delayed_free as *mut *mut mi_block_t,
        ))
        .is_null()
    {
    } else {
        _mi_assert_fail(
            b"collect != MI_ABANDON || mi_atomic_load_ptr_acquire(mi_block_t,&heap->thread_delayed_free) == NULL\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            4993 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 19],
                &[libc::c_char; 19],
            >(b"mi_heap_collect_ex\0"))
                .as_ptr(),
        );
    };
    _mi_segments_collect(
        collect as libc::c_uint == MI_FORCE as libc::c_int as libc::c_uint,
        &mut (*(*heap).tld).segments,
    );
    if force as libc::c_int != 0
        && is_main_thread as libc::c_int != 0
        && mi_heap_is_backing(heap) as libc::c_int != 0
    {
        _mi_thread_data_collect();
    }
    _mi_arenas_collect(
        collect as libc::c_uint == MI_FORCE as libc::c_int as libc::c_uint,
        &mut (*(*heap).tld).stats,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_collect_abandon(mut heap: *mut mi_heap_t) {
    mi_heap_collect_ex(heap, MI_ABANDON);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_collect(mut heap: *mut mi_heap_t, mut force: bool) {
    mi_heap_collect_ex(
        heap,
        (if force as libc::c_int != 0 {
            MI_FORCE as libc::c_int
        } else {
            MI_NORMAL as libc::c_int
        }) as mi_collect_t,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_collect(mut force: bool) {
    mi_heap_collect(mi_prim_get_default_heap(), force);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_get_default() -> *mut mi_heap_t {
    mi_thread_init();
    return mi_prim_get_default_heap();
}
unsafe extern "C" fn mi_heap_is_default(mut heap: *const mi_heap_t) -> bool {
    return heap == mi_prim_get_default_heap() as *const mi_heap_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_get_backing() -> *mut mi_heap_t {
    let mut heap: *mut mi_heap_t = mi_heap_get_default();
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5018 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_heap_get_backing\0"))
                .as_ptr(),
        );
    };
    let mut bheap: *mut mi_heap_t = (*(*heap).tld).heap_backing;
    if !bheap.is_null() {
    } else {
        _mi_assert_fail(
            b"bheap!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5020 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_heap_get_backing\0"))
                .as_ptr(),
        );
    };
    if (*bheap).thread_id == _mi_thread_id() {
    } else {
        _mi_assert_fail(
            b"bheap->thread_id == _mi_thread_id()\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5021 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_heap_get_backing\0"))
                .as_ptr(),
        );
    };
    return bheap;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_init(
    mut heap: *mut mi_heap_t,
    mut tld: *mut mi_tld_t,
    mut arena_id: mi_arena_id_t,
    mut noreclaim: bool,
    mut tag: uint8_t,
) {
    _mi_memcpy_aligned(
        heap as *mut libc::c_void,
        &_mi_heap_empty as *const mi_heap_t as *const libc::c_void,
        ::core::mem::size_of::<mi_heap_t>() as libc::c_ulong,
    );
    (*heap).tld = tld;
    (*heap).thread_id = _mi_thread_id();
    (*heap).arena_id = arena_id;
    (*heap).no_reclaim = noreclaim;
    (*heap).tag = tag;
    if heap == (*tld).heap_backing {
        _mi_random_init(&mut (*heap).random);
    } else {
        _mi_random_split(&mut (*(*tld).heap_backing).random, &mut (*heap).random);
    }
    (*heap).cookie = _mi_heap_random_next(heap) | 1 as libc::c_int as uintptr_t;
    (*heap).keys[0 as libc::c_int as usize] = _mi_heap_random_next(heap);
    (*heap).keys[1 as libc::c_int as usize] = _mi_heap_random_next(heap);
    _mi_heap_guarded_init(heap);
    (*heap).next = (*(*heap).tld).heaps;
    (*(*heap).tld).heaps = heap;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_new_ex(
    mut heap_tag: libc::c_int,
    mut allow_destroy: bool,
    mut arena_id: mi_arena_id_t,
) -> *mut mi_heap_t {
    let mut bheap: *mut mi_heap_t = mi_heap_get_backing();
    let mut heap: *mut mi_heap_t =
        mi_heap_malloc(bheap, ::core::mem::size_of::<mi_heap_t>() as libc::c_ulong)
            as *mut mi_heap_t;
    if heap.is_null() {
        return 0 as *mut mi_heap_t;
    }
    if heap_tag >= 0 as libc::c_int && heap_tag < 256 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"heap_tag >= 0 && heap_tag < 256\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5048 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_heap_new_ex\0"))
                .as_ptr(),
        );
    };
    _mi_heap_init(
        heap,
        (*bheap).tld,
        arena_id,
        allow_destroy,
        heap_tag as uint8_t,
    );
    return heap;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_new_in_arena(mut arena_id: mi_arena_id_t) -> *mut mi_heap_t {
    return mi_heap_new_ex(0 as libc::c_int, 0 as libc::c_int != 0, arena_id);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_new() -> *mut mi_heap_t {
    return mi_heap_new_ex(0 as libc::c_int, 1 as libc::c_int != 0, _mi_arena_id_none());
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_memid_is_suitable(
    mut heap: *mut mi_heap_t,
    mut memid: mi_memid_t,
) -> bool {
    return _mi_arena_memid_is_suitable(memid, (*heap).arena_id);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_random_next(mut heap: *mut mi_heap_t) -> uintptr_t {
    return _mi_random_next(&mut (*heap).random);
}
unsafe extern "C" fn mi_heap_reset_pages(mut heap: *mut mi_heap_t) {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5065 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_heap_reset_pages\0"))
                .as_ptr(),
        );
    };
    if mi_heap_is_initialized(heap) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_is_initialized(heap)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5066 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_heap_reset_pages\0"))
                .as_ptr(),
        );
    };
    memset(
        &mut (*heap).pages_free_direct as *mut [*mut mi_page_t; 130] as *mut libc::c_void,
        0 as libc::c_int,
        ::core::mem::size_of::<[*mut mi_page_t; 130]>() as libc::c_ulong,
    );
    _mi_memcpy_aligned(
        &mut (*heap).pages as *mut [mi_page_queue_t; 75] as *mut libc::c_void,
        &_mi_heap_empty.pages as *const [mi_page_queue_t; 75] as *const libc::c_void,
        ::core::mem::size_of::<[mi_page_queue_t; 75]>() as libc::c_ulong,
    );
    (*heap).thread_delayed_free = 0 as *mut mi_block_t;
    (*heap).page_count = 0 as libc::c_int as size_t;
}
unsafe extern "C" fn mi_heap_free(mut heap: *mut mi_heap_t) {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5073 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_heap_free\0")).as_ptr(),
        );
    };
    if mi_heap_is_initialized(heap) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_is_initialized(heap)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5074 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_heap_free\0")).as_ptr(),
        );
    };
    if heap.is_null() || !mi_heap_is_initialized(heap) {
        return;
    }
    if mi_heap_is_backing(heap) {
        return;
    }
    if mi_heap_is_default(heap) {
        _mi_heap_set_default_direct((*(*heap).tld).heap_backing);
    }
    let mut prev: *mut mi_heap_t = 0 as *mut mi_heap_t;
    let mut curr: *mut mi_heap_t = (*(*heap).tld).heaps;
    while curr != heap && !curr.is_null() {
        prev = curr;
        curr = (*curr).next;
    }
    if curr == heap {
    } else {
        _mi_assert_fail(
            b"curr == heap\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5086 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_heap_free\0")).as_ptr(),
        );
    };
    if curr == heap {
        if !prev.is_null() {
            (*prev).next = (*heap).next;
        } else {
            (*(*heap).tld).heaps = (*heap).next;
        }
    }
    if !((*(*heap).tld).heaps).is_null() {
    } else {
        _mi_assert_fail(
            b"heap->tld->heaps != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5091 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_heap_free\0")).as_ptr(),
        );
    };
    mi_free(heap as *mut libc::c_void);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_by_tag(
    mut heap: *mut mi_heap_t,
    mut tag: uint8_t,
) -> *mut mi_heap_t {
    if (*heap).tag as libc::c_int == tag as libc::c_int {
        return heap;
    }
    let mut curr: *mut mi_heap_t = (*(*heap).tld).heaps;
    while !curr.is_null() {
        if (*curr).tag as libc::c_int == tag as libc::c_int {
            return curr;
        }
        curr = (*curr).next;
    }
    return 0 as *mut mi_heap_t;
}
unsafe extern "C" fn _mi_heap_page_destroy(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
    mut arg1: *mut libc::c_void,
    mut arg2: *mut libc::c_void,
) -> bool {
    _mi_page_use_delayed_free(page, MI_NEVER_DELAYED_FREE, 0 as libc::c_int != 0);
    let bsize: size_t = mi_page_block_size(page);
    if bsize as libc::c_ulonglong
        > ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
    {
        _mi_stat_decrease(&mut (*(*heap).tld).stats.huge, bsize);
    }
    _mi_page_free_collect(page, 0 as libc::c_int != 0);
    let inuse: size_t = (*page).used as size_t;
    if bsize as libc::c_ulonglong
        <= ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
    {
        _mi_stat_decrease(&mut (*(*heap).tld).stats.normal, bsize * inuse);
        _mi_stat_decrease(
            &mut *((*(*heap).tld).stats.normal_bins)
                .as_mut_ptr()
                .offset((_mi_bin as unsafe extern "C" fn(size_t) -> uint8_t)(bsize) as isize),
            inuse,
        );
    }
    _mi_stat_decrease(&mut (*(*heap).tld).stats.malloc, bsize * inuse);
    if (mi_page_thread_free(page)).is_null() {
    } else {
        _mi_assert_fail(
            b"mi_page_thread_free(page) == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5126 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"_mi_heap_page_destroy\0"))
                .as_ptr(),
        );
    };
    (*page).used = 0 as libc::c_int as uint16_t;
    (*page).next = 0 as *mut mi_page_s;
    (*page).prev = 0 as *mut mi_page_s;
    _mi_segment_page_free(page, 0 as libc::c_int != 0, &mut (*(*heap).tld).segments);
    return 1 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_destroy_pages(mut heap: *mut mi_heap_t) {
    mi_heap_visit_pages(
        heap,
        Some(
            _mi_heap_page_destroy
                as unsafe extern "C" fn(
                    *mut mi_heap_t,
                    *mut mi_page_queue_t,
                    *mut mi_page_t,
                    *mut libc::c_void,
                    *mut libc::c_void,
                ) -> bool,
        ),
        0 as *mut libc::c_void,
        0 as *mut libc::c_void,
    );
    mi_heap_reset_pages(heap);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_destroy(mut heap: *mut mi_heap_t) {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5145 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_heap_destroy\0"))
                .as_ptr(),
        );
    };
    if mi_heap_is_initialized(heap) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_is_initialized(heap)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5146 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_heap_destroy\0"))
                .as_ptr(),
        );
    };
    if (*heap).no_reclaim as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"heap->no_reclaim\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5147 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_heap_destroy\0"))
                .as_ptr(),
        );
    };
    if heap.is_null() || !mi_heap_is_initialized(heap) {
        return;
    }
    if !(*heap).no_reclaim {
        _mi_warning_message(
            b"'mi_heap_destroy' called but ignored as the heap was not created with 'allow_destroy' (heap at %p)\n\0"
                as *const u8 as *const libc::c_char,
            heap,
        );
        mi_heap_delete(heap);
    } else {
        _mi_heap_destroy_pages(heap);
        mi_heap_free(heap);
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_unsafe_destroy_all() {
    let mut bheap: *mut mi_heap_t = mi_heap_get_backing();
    let mut curr: *mut mi_heap_t = (*(*bheap).tld).heaps;
    while !curr.is_null() {
        let mut next: *mut mi_heap_t = (*curr).next;
        if (*curr).no_reclaim {
            mi_heap_destroy(curr);
        } else {
            _mi_heap_destroy_pages(curr);
        }
        curr = next;
    }
}
unsafe extern "C" fn mi_heap_absorb(mut heap: *mut mi_heap_t, mut from: *mut mi_heap_t) {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5182 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_heap_absorb\0"))
                .as_ptr(),
        );
    };
    if from.is_null() || (*from).page_count == 0 as libc::c_int as size_t {
        return;
    }
    _mi_heap_delayed_free_partial(from);
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i <= (73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) as size_t {
        let mut pq: *mut mi_page_queue_t =
            &mut *((*heap).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_queue_t;
        let mut append: *mut mi_page_queue_t =
            &mut *((*from).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_queue_t;
        let mut pcount: size_t = _mi_page_queue_append(heap, pq, append);
        (*heap).page_count = ((*heap).page_count).wrapping_add(pcount);
        (*from).page_count = ((*from).page_count).wrapping_sub(pcount);
        i = i.wrapping_add(1);
        i;
    }
    if (*from).page_count == 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"from->page_count == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5192 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_heap_absorb\0"))
                .as_ptr(),
        );
    };
    _mi_heap_delayed_free_all(from);
    if (::core::intrinsics::atomic_load_relaxed(
        &mut (*from).thread_delayed_free as *mut *mut mi_block_t,
    ))
    .is_null()
    {
    } else {
        _mi_assert_fail(
            b"mi_atomic_load_ptr_relaxed(mi_block_t,&from->thread_delayed_free) == NULL\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5195 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_heap_absorb\0"))
                .as_ptr(),
        );
    };
    mi_heap_reset_pages(from);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_delete(mut heap: *mut mi_heap_t) {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5201 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_heap_delete\0"))
                .as_ptr(),
        );
    };
    if mi_heap_is_initialized(heap) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_is_initialized(heap)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5202 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_heap_delete\0"))
                .as_ptr(),
        );
    };
    if heap.is_null() || !mi_heap_is_initialized(heap) {
        return;
    }
    if !mi_heap_is_backing(heap) {
        mi_heap_absorb((*(*heap).tld).heap_backing, heap);
    } else {
        _mi_heap_collect_abandon(heap);
    }
    if (*heap).page_count == 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"heap->page_count==0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5211 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_heap_delete\0"))
                .as_ptr(),
        );
    };
    mi_heap_free(heap);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_set_default(mut heap: *mut mi_heap_t) -> *mut mi_heap_t {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5215 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_heap_set_default\0"))
                .as_ptr(),
        );
    };
    if mi_heap_is_initialized(heap) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_is_initialized(heap)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5216 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_heap_set_default\0"))
                .as_ptr(),
        );
    };
    if heap.is_null() || !mi_heap_is_initialized(heap) {
        return 0 as *mut mi_heap_t;
    }
    let mut old: *mut mi_heap_t = mi_prim_get_default_heap();
    _mi_heap_set_default_direct(heap);
    return old;
}
unsafe extern "C" fn mi_heap_of_block(mut p: *const libc::c_void) -> *mut mi_heap_t {
    if p.is_null() {
        return 0 as *mut mi_heap_t;
    }
    let mut segment: *mut mi_segment_t = _mi_ptr_segment(p);
    let mut valid: bool = _mi_ptr_cookie(segment as *const libc::c_void) == (*segment).cookie;
    if valid as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"valid\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5227 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_heap_of_block\0"))
                .as_ptr(),
        );
    };
    if !valid as libc::c_int as libc::c_long != 0 {
        return 0 as *mut mi_heap_t;
    }
    return mi_page_heap(_mi_segment_page_of(segment, p));
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_contains_block(
    mut heap: *mut mi_heap_t,
    mut p: *const libc::c_void,
) -> bool {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5232 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"mi_heap_contains_block\0",
            ))
            .as_ptr(),
        );
    };
    if heap.is_null() || !mi_heap_is_initialized(heap) {
        return 0 as libc::c_int != 0;
    }
    return heap == mi_heap_of_block(p);
}
unsafe extern "C" fn mi_heap_page_check_owned(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
    mut p: *mut libc::c_void,
    mut vfound: *mut libc::c_void,
) -> bool {
    let mut found: *mut bool = vfound as *mut bool;
    let mut start: *mut libc::c_void = mi_page_start(page) as *mut libc::c_void;
    let mut end: *mut libc::c_void = (start as *mut uint8_t)
        .offset(((*page).capacity as size_t * mi_page_block_size(page)) as isize)
        as *mut libc::c_void;
    *found = p >= start && p < end;
    return !*found;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_check_owned(
    mut heap: *mut mi_heap_t,
    mut p: *const libc::c_void,
) -> bool {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5246 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_heap_check_owned\0"))
                .as_ptr(),
        );
    };
    if heap.is_null() || !mi_heap_is_initialized(heap) {
        return 0 as libc::c_int != 0;
    }
    if p as uintptr_t & (((1 as libc::c_int) << 3 as libc::c_int) - 1 as libc::c_int) as uintptr_t
        != 0 as libc::c_int as uintptr_t
    {
        return 0 as libc::c_int != 0;
    }
    let mut found: bool = 0 as libc::c_int != 0;
    mi_heap_visit_pages(
        heap,
        Some(
            mi_heap_page_check_owned
                as unsafe extern "C" fn(
                    *mut mi_heap_t,
                    *mut mi_page_queue_t,
                    *mut mi_page_t,
                    *mut libc::c_void,
                    *mut libc::c_void,
                ) -> bool,
        ),
        p as *mut libc::c_void,
        &mut found as *mut bool as *mut libc::c_void,
    );
    return found;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_check_owned(mut p: *const libc::c_void) -> bool {
    return mi_heap_check_owned(mi_prim_get_default_heap(), p);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_area_init(
    mut area: *mut mi_heap_area_t,
    mut page: *mut mi_page_t,
) {
    let bsize: size_t = mi_page_block_size(page);
    let ubsize: size_t = mi_page_usable_block_size(page);
    (*area).reserved = (*page).reserved as size_t * bsize;
    (*area).committed = (*page).capacity as size_t * bsize;
    (*area).blocks = mi_page_start(page) as *mut libc::c_void;
    (*area).used = (*page).used as size_t;
    (*area).block_size = ubsize;
    (*area).full_block_size = bsize;
    (*area).heap_tag = (*page).heap_tag as libc::c_int;
}
unsafe extern "C" fn mi_get_fast_divisor(
    mut divisor: size_t,
    mut magic: *mut uint64_t,
    mut shift: *mut size_t,
) {
    if divisor > 0 as libc::c_int as size_t && divisor <= 4294967295 as libc::c_uint as size_t {
    } else {
        _mi_assert_fail(
            b"divisor > 0 && divisor <= UINT32_MAX\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5268 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_get_fast_divisor\0"))
                .as_ptr(),
        );
    };
    *shift = ((((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t)
        .wrapping_sub(mi_clz(divisor.wrapping_sub(1 as libc::c_int as size_t)));
    *magic = ((1 as libc::c_int as uint64_t) << 32 as libc::c_int)
        .wrapping_mul(((1 as libc::c_int as uint64_t) << *shift).wrapping_sub(divisor))
        .wrapping_div(divisor)
        .wrapping_add(1 as libc::c_int as libc::c_ulong);
}
unsafe extern "C" fn mi_fast_divide(
    mut n: size_t,
    mut magic: uint64_t,
    mut shift: size_t,
) -> size_t {
    if n <= 4294967295 as libc::c_uint as size_t {
    } else {
        _mi_assert_fail(
            b"n <= UINT32_MAX\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5273 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_fast_divide\0"))
                .as_ptr(),
        );
    };
    let hi: uint64_t = n * magic >> 32 as libc::c_int;
    return hi.wrapping_add(n) >> shift;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_area_visit_blocks(
    mut area: *const mi_heap_area_t,
    mut page: *mut mi_page_t,
    mut visitor: Option<mi_block_visit_fun>,
    mut arg: *mut libc::c_void,
) -> bool {
    if !area.is_null() {
    } else {
        _mi_assert_fail(
            b"area != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5278 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"_mi_heap_area_visit_blocks\0",
            ))
            .as_ptr(),
        );
    };
    if area.is_null() {
        return 1 as libc::c_int != 0;
    }
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5280 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"_mi_heap_area_visit_blocks\0",
            ))
            .as_ptr(),
        );
    };
    if page.is_null() {
        return 1 as libc::c_int != 0;
    }
    _mi_page_free_collect(page, 1 as libc::c_int != 0);
    if ((*page).local_free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->local_free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5283 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"_mi_heap_area_visit_blocks\0",
            ))
            .as_ptr(),
        );
    };
    if (*page).used as libc::c_int == 0 as libc::c_int {
        return 1 as libc::c_int != 0;
    }
    let mut psize: size_t = 0;
    let pstart: *mut uint8_t = _mi_segment_page_start(_mi_page_segment(page), page, &mut psize);
    let heap: *mut mi_heap_t = mi_page_heap(page);
    let bsize: size_t = mi_page_block_size(page);
    let ubsize: size_t = mi_page_usable_block_size(page);
    if (*page).capacity as libc::c_int == 1 as libc::c_int {
        if (*page).used as libc::c_int == 1 as libc::c_int && ((*page).free).is_null() {
        } else {
            _mi_assert_fail(
                b"page->used == 1 && page->free == NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                5291 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"_mi_heap_area_visit_blocks\0",
                ))
                .as_ptr(),
            );
        };
        return visitor.expect("non-null function pointer")(
            mi_page_heap(page),
            area,
            pstart as *mut libc::c_void,
            ubsize,
            arg,
        );
    }
    if bsize <= 4294967295 as libc::c_uint as size_t {
    } else {
        _mi_assert_fail(
            b"bsize <= UINT32_MAX\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5294 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"_mi_heap_area_visit_blocks\0",
            ))
            .as_ptr(),
        );
    };
    if (*page).used as libc::c_int == (*page).capacity as libc::c_int {
        let mut block: *mut uint8_t = pstart;
        let mut i: size_t = 0 as libc::c_int as size_t;
        while i < (*page).capacity as size_t {
            if !visitor.expect("non-null function pointer")(
                heap,
                area,
                block as *mut libc::c_void,
                ubsize,
                arg,
            ) {
                return 0 as libc::c_int != 0;
            }
            block = block.offset(bsize as isize);
            i = i.wrapping_add(1);
            i;
        }
        return 1 as libc::c_int != 0;
    }
    let mut free_map: [uintptr_t; 128] = [0; 128];
    let bmapsize: uintptr_t = _mi_divide_up(
        (*page).capacity as uintptr_t,
        (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t,
    );
    memset(
        free_map.as_mut_ptr() as *mut libc::c_void,
        0 as libc::c_int,
        bmapsize.wrapping_mul(::core::mem::size_of::<intptr_t>() as libc::c_ulong),
    );
    if (*page).capacity as libc::c_int
        % (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int)
        != 0 as libc::c_int
    {
        let mut shift: size_t = ((*page).capacity as libc::c_int
            % (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int))
            as size_t;
        let mut mask: uintptr_t = (18446744073709551615 as libc::c_ulong) << shift;
        free_map[bmapsize.wrapping_sub(1 as libc::c_int as uintptr_t) as usize] = mask;
    }
    let mut magic: uint64_t = 0;
    let mut shift_0: size_t = 0;
    mi_get_fast_divisor(bsize, &mut magic, &mut shift_0);
    let mut free_count: size_t = 0 as libc::c_int as size_t;
    let mut block_0: *mut mi_block_t = (*page).free;
    while !block_0.is_null() {
        free_count = free_count.wrapping_add(1);
        free_count;
        if block_0 as *mut uint8_t >= pstart
            && (block_0 as *mut uint8_t) < pstart.offset(psize as isize)
        {
        } else {
            _mi_assert_fail(
                b"(uint8_t*)block >= pstart && (uint8_t*)block < (pstart + psize)\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                5322 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"_mi_heap_area_visit_blocks\0",
                ))
                .as_ptr(),
            );
        };
        let mut offset: size_t =
            (block_0 as *mut uint8_t).offset_from(pstart) as libc::c_long as size_t;
        if offset % bsize == 0 as libc::c_int as size_t {
        } else {
            _mi_assert_fail(
                b"offset % bsize == 0\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                5324 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"_mi_heap_area_visit_blocks\0",
                ))
                .as_ptr(),
            );
        };
        if offset <= 4294967295 as libc::c_uint as size_t {
        } else {
            _mi_assert_fail(
                b"offset <= UINT32_MAX\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                5325 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"_mi_heap_area_visit_blocks\0",
                ))
                .as_ptr(),
            );
        };
        let mut blockidx: size_t = mi_fast_divide(offset, magic, shift_0);
        if blockidx == offset / bsize {
        } else {
            _mi_assert_fail(
                b"blockidx == offset / bsize\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                5327 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"_mi_heap_area_visit_blocks\0",
                ))
                .as_ptr(),
            );
        };
        if (blockidx as libc::c_ulonglong)
            < ((1 as libc::c_ulonglong) << 13 as libc::c_int + 3 as libc::c_int)
                .wrapping_div(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong
                    as libc::c_ulonglong)
        {
        } else {
            _mi_assert_fail(
                b"blockidx < MI_MAX_BLOCKS\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                5328 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"_mi_heap_area_visit_blocks\0",
                ))
                .as_ptr(),
            );
        };
        let mut bitidx: size_t =
            blockidx / (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t;
        let mut bit: size_t = blockidx.wrapping_sub(
            bitidx * (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t,
        );
        free_map[bitidx as usize] |= (1 as libc::c_int as uintptr_t) << bit;
        block_0 = mi_block_next(page, block_0);
    }
    if (*page).capacity as size_t == free_count.wrapping_add((*page).used as size_t) {
    } else {
        _mi_assert_fail(
            b"page->capacity == (free_count + page->used)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5333 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"_mi_heap_area_visit_blocks\0",
            ))
            .as_ptr(),
        );
    };
    let mut used_count: size_t = 0 as libc::c_int as size_t;
    let mut block_1: *mut uint8_t = pstart;
    let mut i_0: size_t = 0 as libc::c_int as size_t;
    while i_0 < bmapsize {
        if free_map[i_0 as usize] == 0 as libc::c_int as uintptr_t {
            let mut j: size_t = 0 as libc::c_int as size_t;
            while j < (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t {
                used_count = used_count.wrapping_add(1);
                used_count;
                if !visitor.expect("non-null function pointer")(
                    heap,
                    area,
                    block_1 as *mut libc::c_void,
                    ubsize,
                    arg,
                ) {
                    return 0 as libc::c_int != 0;
                }
                block_1 = block_1.offset(bsize as isize);
                j = j.wrapping_add(1);
                j;
            }
        } else {
            let mut m: uintptr_t = !free_map[i_0 as usize];
            while m != 0 as libc::c_int as uintptr_t {
                used_count = used_count.wrapping_add(1);
                used_count;
                let mut bitidx_0: size_t = mi_ctz(m);
                if !visitor.expect("non-null function pointer")(
                    heap,
                    area,
                    block_1.offset((bitidx_0 * bsize) as isize) as *mut libc::c_void,
                    ubsize,
                    arg,
                ) {
                    return 0 as libc::c_int != 0;
                }
                m &= m.wrapping_sub(1 as libc::c_int as uintptr_t);
            }
            block_1 = block_1.offset(
                (bsize * (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as size_t)
                    as isize,
            );
        }
        i_0 = i_0.wrapping_add(1);
        i_0;
    }
    if (*page).used as size_t == used_count {
    } else {
        _mi_assert_fail(
            b"page->used == used_count\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5361 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"_mi_heap_area_visit_blocks\0",
            ))
            .as_ptr(),
        );
    };
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn mi_heap_visit_areas_page(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
    mut vfun: *mut libc::c_void,
    mut arg: *mut libc::c_void,
) -> bool {
    let mut fun: Option<mi_heap_area_visit_fun> =
        ::core::mem::transmute::<*mut libc::c_void, Option<mi_heap_area_visit_fun>>(vfun);
    let mut xarea: mi_heap_area_ex_t = mi_heap_area_ex_s {
        area: mi_heap_area_s {
            blocks: 0 as *mut libc::c_void,
            reserved: 0,
            committed: 0,
            used: 0,
            block_size: 0,
            full_block_size: 0,
            heap_tag: 0,
        },
        page: 0 as *mut mi_page_t,
    };
    xarea.page = page;
    _mi_heap_area_init(&mut xarea.area, page);
    return fun.expect("non-null function pointer")(heap, &mut xarea, arg);
}
unsafe extern "C" fn mi_heap_visit_areas(
    mut heap: *const mi_heap_t,
    mut visitor: Option<mi_heap_area_visit_fun>,
    mut arg: *mut libc::c_void,
) -> bool {
    if visitor.is_none() {
        return 0 as libc::c_int != 0;
    }
    return mi_heap_visit_pages(
        heap as *mut mi_heap_t,
        Some(
            mi_heap_visit_areas_page
                as unsafe extern "C" fn(
                    *mut mi_heap_t,
                    *mut mi_page_queue_t,
                    *mut mi_page_t,
                    *mut libc::c_void,
                    *mut libc::c_void,
                ) -> bool,
        ),
        ::core::mem::transmute::<Option<mi_heap_area_visit_fun>, *mut libc::c_void>(visitor),
        arg,
    );
}
unsafe extern "C" fn mi_heap_area_visitor(
    mut heap: *const mi_heap_t,
    mut xarea: *const mi_heap_area_ex_t,
    mut arg: *mut libc::c_void,
) -> bool {
    let mut args: *mut mi_visit_blocks_args_t = arg as *mut mi_visit_blocks_args_t;
    if !((*args).visitor).expect("non-null function pointer")(
        heap,
        &(*xarea).area,
        0 as *mut libc::c_void,
        (*xarea).area.block_size,
        (*args).arg,
    ) {
        return 0 as libc::c_int != 0;
    }
    if (*args).visit_blocks {
        return _mi_heap_area_visit_blocks(
            &(*xarea).area,
            (*xarea).page,
            (*args).visitor,
            (*args).arg,
        );
    } else {
        return 1 as libc::c_int != 0;
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_visit_blocks(
    mut heap: *const mi_heap_t,
    mut visit_blocks: bool,
    mut visitor: Option<mi_block_visit_fun>,
    mut arg: *mut libc::c_void,
) -> bool {
    let mut args: mi_visit_blocks_args_t = {
        let mut init = mi_visit_blocks_args_s {
            visit_blocks: visit_blocks,
            visitor: visitor,
            arg: arg,
        };
        init
    };
    return mi_heap_visit_areas(
        heap,
        Some(
            mi_heap_area_visitor
                as unsafe extern "C" fn(
                    *const mi_heap_t,
                    *const mi_heap_area_ex_t,
                    *mut libc::c_void,
                ) -> bool,
        ),
        &mut args as *mut mi_visit_blocks_args_t as *mut libc::c_void,
    );
}
#[unsafe(no_mangle)]
pub static mut _mi_page_empty: mi_page_t = mi_page_s {
    segment_idx: 0,
    segment_in_use_is_committed_is_zero_init_is_huge: [0; 1],
    capacity: 0,
    reserved: 0,
    flags: mi_page_flags_s { full_aligned: 0 },
    free_is_zero_retire_expire: [0; 1],
    free: 0 as *const mi_block_t as *mut mi_block_t,
    local_free: 0 as *const mi_block_t as *mut mi_block_t,
    used: 0,
    block_size_shift: 0,
    heap_tag: 0,
    block_size: 0,
    page_start: 0 as *const uint8_t as *mut uint8_t,
    keys: [0; 2],
    xthread_free: 0,
    xheap: 0,
    next: 0 as *const mi_page_s as *mut mi_page_s,
    prev: 0 as *const mi_page_s as *mut mi_page_s,
};
#[unsafe(no_mangle)]
pub static mut _mi_heap_empty: mi_heap_t = mi_heap_s {
    tld: 0 as *const mi_tld_t as *mut mi_tld_t,
    thread_delayed_free: 0 as *const mi_block_t as *mut mi_block_t,
    thread_id: 0,
    arena_id: 0,
    cookie: 0,
    keys: [0; 2],
    random: mi_random_cxt_s {
        input: [0; 16],
        output: [0; 16],
        output_available: 0,
        weak: false,
    },
    page_count: 0,
    page_retired_min: 0,
    page_retired_max: 0,
    next: 0 as *const mi_heap_t as *mut mi_heap_t,
    no_reclaim: false,
    tag: 0,
    pages_free_direct: [0 as *const mi_page_t as *mut mi_page_t; 130],
    pages: [mi_page_queue_s {
        first: 0 as *mut mi_page_t,
        last: 0 as *mut mi_page_t,
        block_size: 0,
    }; 75],
};
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_thread_id() -> mi_threadid_t {
    return _mi_prim_thread_id();
}
#[unsafe(no_mangle)]
#[thread_local]
pub static mut _mi_heap_default: *mut mi_heap_t = unsafe { &raw mut _mi_heap_empty };
static mut mi_subproc_default: mi_subproc_t = mi_subproc_s {
    abandoned_count: 0,
    abandoned_os_list_count: 0,
    abandoned_os_lock: pthread_mutex_t {
        __data: __pthread_mutex_s {
            __lock: 0,
            __count: 0,
            __owner: 0,
            __nusers: 0,
            __kind: 0,
            __spins: 0,
            __elision: 0,
            __list: __pthread_internal_list {
                __prev: 0 as *const __pthread_internal_list as *mut __pthread_internal_list,
                __next: 0 as *const __pthread_internal_list as *mut __pthread_internal_list,
            },
        },
    },
    abandoned_os_visit_lock: pthread_mutex_t {
        __data: __pthread_mutex_s {
            __lock: 0,
            __count: 0,
            __owner: 0,
            __nusers: 0,
            __kind: 0,
            __spins: 0,
            __elision: 0,
            __list: __pthread_internal_list {
                __prev: 0 as *const __pthread_internal_list as *mut __pthread_internal_list,
                __next: 0 as *const __pthread_internal_list as *mut __pthread_internal_list,
            },
        },
    },
    abandoned_os_list: 0 as *const mi_segment_t as *mut mi_segment_t,
    abandoned_os_list_tail: 0 as *const mi_segment_t as *mut mi_segment_t,
    memid: mi_memid_s {
        mem: C2RustUnnamed_0 {
            os: mi_memid_os_info {
                base: 0 as *mut libc::c_void,
                alignment: 0,
            },
        },
        is_pinned: false,
        initially_committed: false,
        initially_zero: false,
        memkind: MI_MEM_NONE,
    },
};
static mut tld_main: mi_tld_t = mi_tld_s {
    heartbeat: 0,
    recurse: false,
    heap_backing: 0 as *const mi_heap_t as *mut mi_heap_t,
    heaps: 0 as *const mi_heap_t as *mut mi_heap_t,
    segments: mi_segments_tld_s {
        small_free: mi_segment_queue_s {
            first: 0 as *const mi_segment_t as *mut mi_segment_t,
            last: 0 as *const mi_segment_t as *mut mi_segment_t,
        },
        medium_free: mi_segment_queue_s {
            first: 0 as *const mi_segment_t as *mut mi_segment_t,
            last: 0 as *const mi_segment_t as *mut mi_segment_t,
        },
        pages_purge: mi_page_queue_s {
            first: 0 as *mut mi_page_t,
            last: 0 as *mut mi_page_t,
            block_size: 0,
        },
        count: 0,
        peak_count: 0,
        current_size: 0,
        peak_size: 0,
        reclaim_count: 0,
        subproc: 0 as *const mi_subproc_t as *mut mi_subproc_t,
        stats: 0 as *const mi_stats_t as *mut mi_stats_t,
        os: 0 as *const mi_os_tld_t as *mut mi_os_tld_t,
    },
    os: mi_os_tld_s {
        region_idx: 0,
        stats: 0 as *const mi_stats_t as *mut mi_stats_t,
    },
    stats: mi_stats_s {
        segments: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        pages: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        reserved: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        committed: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        reset: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        purged: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        page_committed: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        segments_abandoned: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        pages_abandoned: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        threads: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        normal: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        huge: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        giant: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        malloc: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        segments_cache: mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        },
        pages_extended: mi_stat_counter_s { total: 0, count: 0 },
        mmap_calls: mi_stat_counter_s { total: 0, count: 0 },
        commit_calls: mi_stat_counter_s { total: 0, count: 0 },
        reset_calls: mi_stat_counter_s { total: 0, count: 0 },
        purge_calls: mi_stat_counter_s { total: 0, count: 0 },
        page_no_retire: mi_stat_counter_s { total: 0, count: 0 },
        searches: mi_stat_counter_s { total: 0, count: 0 },
        normal_count: mi_stat_counter_s { total: 0, count: 0 },
        huge_count: mi_stat_counter_s { total: 0, count: 0 },
        arena_count: mi_stat_counter_s { total: 0, count: 0 },
        arena_crossover_count: mi_stat_counter_s { total: 0, count: 0 },
        arena_rollback_count: mi_stat_counter_s { total: 0, count: 0 },
        guarded_alloc_count: mi_stat_counter_s { total: 0, count: 0 },
        normal_bins: [mi_stat_count_s {
            allocated: 0,
            freed: 0,
            peak: 0,
            current: 0,
        }; 74],
    },
};
#[unsafe(no_mangle)]
pub static mut _mi_heap_main: mi_heap_t = mi_heap_s {
    tld: 0 as *const mi_tld_t as *mut mi_tld_t,
    thread_delayed_free: 0 as *const mi_block_t as *mut mi_block_t,
    thread_id: 0,
    arena_id: 0,
    cookie: 0,
    keys: [0; 2],
    random: mi_random_cxt_s {
        input: [0; 16],
        output: [0; 16],
        output_available: 0,
        weak: false,
    },
    page_count: 0,
    page_retired_min: 0,
    page_retired_max: 0,
    next: 0 as *const mi_heap_t as *mut mi_heap_t,
    no_reclaim: false,
    tag: 0,
    pages_free_direct: [0 as *const mi_page_t as *mut mi_page_t; 130],
    pages: [mi_page_queue_s {
        first: 0 as *mut mi_page_t,
        last: 0 as *mut mi_page_t,
        block_size: 0,
    }; 75],
};
#[unsafe(no_mangle)]
pub static mut _mi_process_is_initialized: bool = 0 as libc::c_int != 0;
#[unsafe(no_mangle)]
pub static mut _mi_stats_main: mi_stats_t = {
    let mut init = mi_stats_s {
        segments: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        pages: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        reserved: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        committed: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        reset: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        purged: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        page_committed: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        segments_abandoned: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        pages_abandoned: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        threads: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        normal: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        huge: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        giant: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        malloc: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        segments_cache: {
            let mut init = mi_stat_count_s {
                allocated: 0 as libc::c_int as int64_t,
                freed: 0 as libc::c_int as int64_t,
                peak: 0 as libc::c_int as int64_t,
                current: 0 as libc::c_int as int64_t,
            };
            init
        },
        pages_extended: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        mmap_calls: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        commit_calls: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        reset_calls: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        purge_calls: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        page_no_retire: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        searches: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        normal_count: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        huge_count: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        arena_count: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        arena_crossover_count: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        arena_rollback_count: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        guarded_alloc_count: {
            let mut init = mi_stat_counter_s {
                total: 0 as libc::c_int as int64_t,
                count: 0 as libc::c_int as int64_t,
            };
            init
        },
        normal_bins: [
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            {
                let mut init = mi_stat_count_s {
                    allocated: 0 as libc::c_int as int64_t,
                    freed: 0 as libc::c_int as int64_t,
                    peak: 0 as libc::c_int as int64_t,
                    current: 0 as libc::c_int as int64_t,
                };
                init
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
            mi_stat_count_s {
                allocated: 0,
                freed: 0,
                peak: 0,
                current: 0,
            },
        ],
    };
    init
};
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_guarded_set_sample_rate(
    mut heap: *mut mi_heap_t,
    mut sample_rate: size_t,
    mut seed: size_t,
) {
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_heap_guarded_set_size_bound(
    mut heap: *mut mi_heap_t,
    mut min: size_t,
    mut max: size_t,
) {
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_guarded_init(mut heap: *mut mi_heap_t) {}
unsafe extern "C" fn mi_heap_main_init() {
    if _mi_heap_main.cookie == 0 as libc::c_int as uintptr_t {
        _mi_heap_main.thread_id = _mi_thread_id();
        _mi_heap_main.cookie = 1 as libc::c_int as uintptr_t;
        _mi_random_init(&mut _mi_heap_main.random);
        _mi_heap_main.cookie = _mi_heap_random_next(&mut _mi_heap_main);
        _mi_heap_main.keys[0 as libc::c_int as usize] = _mi_heap_random_next(&mut _mi_heap_main);
        _mi_heap_main.keys[1 as libc::c_int as usize] = _mi_heap_random_next(&mut _mi_heap_main);
        mi_lock_init(&mut mi_subproc_default.abandoned_os_lock);
        mi_lock_init(&mut mi_subproc_default.abandoned_os_visit_lock);
        _mi_heap_guarded_init(&mut _mi_heap_main);
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_main_get() -> *mut mi_heap_t {
    mi_heap_main_init();
    return &mut _mi_heap_main;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_subproc_main() -> mi_subproc_id_t {
    return 0 as *mut libc::c_void;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_subproc_new() -> mi_subproc_id_t {
    let mut memid: mi_memid_t = _mi_memid_none();
    let mut subproc: *mut mi_subproc_t = _mi_arena_meta_zalloc(
        ::core::mem::size_of::<mi_subproc_t>() as libc::c_ulong,
        &mut memid,
    ) as *mut mi_subproc_t;
    if subproc.is_null() {
        return 0 as *mut libc::c_void;
    }
    (*subproc).memid = memid;
    (*subproc).abandoned_os_list = 0 as *mut mi_segment_t;
    mi_lock_init(&mut (*subproc).abandoned_os_lock);
    mi_lock_init(&mut (*subproc).abandoned_os_visit_lock);
    return subproc as mi_subproc_id_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_subproc_from_id(mut subproc_id: mi_subproc_id_t) -> *mut mi_subproc_t {
    return if subproc_id.is_null() {
        &mut mi_subproc_default
    } else {
        subproc_id as *mut mi_subproc_t
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_subproc_delete(mut subproc_id: mi_subproc_id_t) {
    if subproc_id.is_null() {
        return;
    }
    let mut subproc: *mut mi_subproc_t = _mi_subproc_from_id(subproc_id);
    let mut safe_to_delete: bool = 0 as libc::c_int != 0;
    if mi_lock_acquire(&mut (*subproc).abandoned_os_lock) {
        if ((*subproc).abandoned_os_list).is_null() {
            safe_to_delete = 1 as libc::c_int != 0;
        }
        mi_lock_release(&mut (*subproc).abandoned_os_lock);
    }
    if !safe_to_delete {
        return;
    }
    mi_lock_done(&mut (*subproc).abandoned_os_lock);
    mi_lock_done(&mut (*subproc).abandoned_os_visit_lock);
    _mi_arena_meta_free(
        subproc as *mut libc::c_void,
        (*subproc).memid,
        ::core::mem::size_of::<mi_subproc_t>() as libc::c_ulong,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_subproc_add_current_thread(mut subproc_id: mi_subproc_id_t) {
    let mut heap: *mut mi_heap_t = mi_heap_get_default();
    if heap.is_null() {
        return;
    }
    if (*(*heap).tld).segments.subproc == &mut mi_subproc_default as *mut mi_subproc_t {
    } else {
        _mi_assert_fail(
            b"heap->tld->segments.subproc == &mi_subproc_default\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5616 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 30], &[libc::c_char; 30]>(
                b"mi_subproc_add_current_thread\0",
            ))
            .as_ptr(),
        );
    };
    if (*(*heap).tld).segments.subproc != &mut mi_subproc_default as *mut mi_subproc_t {
        return;
    }
    (*(*heap).tld).segments.subproc = _mi_subproc_from_id(subproc_id);
}
static mut td_cache: [*mut mi_thread_data_t; 32] =
    [0 as *const mi_thread_data_t as *mut mi_thread_data_t; 32];
unsafe extern "C" fn mi_thread_data_zalloc() -> *mut mi_thread_data_t {
    let mut is_zero: bool = 0 as libc::c_int != 0;
    let mut td: *mut mi_thread_data_t = 0 as *mut mi_thread_data_t;
    let mut i: libc::c_int = 0 as libc::c_int;
    while i < 32 as libc::c_int {
        td = ::core::intrinsics::atomic_load_relaxed(
            &mut *td_cache.as_mut_ptr().offset(i as isize) as *mut *mut mi_thread_data_t,
        );
        if !td.is_null() {
            td = ::core::intrinsics::atomic_xchg_acqrel(
                &mut *td_cache.as_mut_ptr().offset(i as isize) as *mut *mut mi_thread_data_t,
                0 as *mut mi_thread_data_t,
            );
            if !td.is_null() {
                break;
            }
        }
        i += 1;
        i;
    }
    if td.is_null() {
        let mut memid: mi_memid_t = mi_memid_s {
            mem: C2RustUnnamed_0 {
                os: mi_memid_os_info {
                    base: 0 as *mut libc::c_void,
                    alignment: 0,
                },
            },
            is_pinned: false,
            initially_committed: false,
            initially_zero: false,
            memkind: MI_MEM_NONE,
        };
        td = _mi_os_alloc(
            ::core::mem::size_of::<mi_thread_data_t>() as libc::c_ulong,
            &mut memid,
            &mut _mi_stats_main,
        ) as *mut mi_thread_data_t;
        if td.is_null() {
            td = _mi_os_alloc(
                ::core::mem::size_of::<mi_thread_data_t>() as libc::c_ulong,
                &mut memid,
                &mut _mi_stats_main,
            ) as *mut mi_thread_data_t;
            if td.is_null() {
                _mi_error_message(
                    12 as libc::c_int,
                    b"unable to allocate thread local heap metadata (%zu bytes)\n\0" as *const u8
                        as *const libc::c_char,
                    ::core::mem::size_of::<mi_thread_data_t>() as libc::c_ulong,
                );
            }
        }
        if !td.is_null() {
            (*td).memid = memid;
            is_zero = memid.initially_zero;
        }
    }
    if !td.is_null() && !is_zero {
        _mi_memzero_aligned(td as *mut libc::c_void, 6296 as libc::c_ulong);
    }
    return td;
}
unsafe extern "C" fn mi_thread_data_free(mut tdfree: *mut mi_thread_data_t) {
    let mut i: libc::c_int = 0 as libc::c_int;
    while i < 32 as libc::c_int {
        let mut td: *mut mi_thread_data_t = ::core::intrinsics::atomic_load_relaxed(
            &mut *td_cache.as_mut_ptr().offset(i as isize) as *mut *mut mi_thread_data_t,
        );
        if td.is_null() {
            let mut expected: *mut mi_thread_data_t = 0 as *mut mi_thread_data_t;
            let fresh27 = ::core::intrinsics::atomic_cxchgweak_acqrel_acquire(
                &mut *td_cache.as_mut_ptr().offset(i as isize) as *mut *mut mi_thread_data_t,
                *(&mut expected as *mut *mut mi_thread_data_t),
                tdfree,
            );
            *(&mut expected as *mut *mut mi_thread_data_t) = fresh27.0;
            if fresh27.1 {
                return;
            }
        }
        i += 1;
        i;
    }
    _mi_os_free(
        tdfree as *mut libc::c_void,
        ::core::mem::size_of::<mi_thread_data_t>() as libc::c_ulong,
        (*tdfree).memid,
        &mut _mi_stats_main,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_thread_data_collect() {
    let mut i: libc::c_int = 0 as libc::c_int;
    while i < 32 as libc::c_int {
        let mut td: *mut mi_thread_data_t = ::core::intrinsics::atomic_load_relaxed(
            &mut *td_cache.as_mut_ptr().offset(i as isize) as *mut *mut mi_thread_data_t,
        );
        if !td.is_null() {
            td = ::core::intrinsics::atomic_xchg_acqrel(
                &mut *td_cache.as_mut_ptr().offset(i as isize) as *mut *mut mi_thread_data_t,
                0 as *mut mi_thread_data_t,
            );
            if !td.is_null() {
                _mi_os_free(
                    td as *mut libc::c_void,
                    ::core::mem::size_of::<mi_thread_data_t>() as libc::c_ulong,
                    (*td).memid,
                    &mut _mi_stats_main,
                );
            }
        }
        i += 1;
        i;
    }
}
unsafe extern "C" fn _mi_thread_heap_init() -> bool {
    if mi_heap_is_initialized(mi_prim_get_default_heap()) {
        return 1 as libc::c_int != 0;
    }
    if _mi_is_main_thread() {
        mi_heap_main_init();
        _mi_heap_set_default_direct(&mut _mi_heap_main);
    } else {
        let mut td: *mut mi_thread_data_t = mi_thread_data_zalloc();
        if td.is_null() {
            return 0 as libc::c_int != 0;
        }
        let mut tld: *mut mi_tld_t = &mut (*td).tld;
        let mut heap: *mut mi_heap_t = &mut (*td).heap;
        _mi_tld_init(tld, heap);
        _mi_heap_init(
            heap,
            tld,
            _mi_arena_id_none(),
            0 as libc::c_int != 0,
            0 as libc::c_int as uint8_t,
        );
        _mi_heap_set_default_direct(heap);
    }
    return 0 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_tld_init(mut tld: *mut mi_tld_t, mut bheap: *mut mi_heap_t) {
    _mi_memzero_aligned(
        tld as *mut libc::c_void,
        ::core::mem::size_of::<mi_tld_t>() as libc::c_ulong,
    );
    (*tld).heap_backing = bheap;
    (*tld).heaps = 0 as *mut mi_heap_t;
    (*tld).segments.subproc = &mut mi_subproc_default;
    (*tld).segments.stats = &mut (*tld).stats;
    (*tld).segments.os = &mut (*tld).os;
    (*tld).os.stats = &mut (*tld).stats;
}
unsafe extern "C" fn _mi_thread_heap_done(mut heap: *mut mi_heap_t) -> bool {
    if !mi_heap_is_initialized(heap) {
        return 1 as libc::c_int != 0;
    }
    _mi_heap_set_default_direct(if _mi_is_main_thread() as libc::c_int != 0 {
        &mut _mi_heap_main
    } else {
        &raw mut _mi_heap_empty
    });
    heap = (*(*heap).tld).heap_backing;
    if !mi_heap_is_initialized(heap) {
        return 0 as libc::c_int != 0;
    }
    let mut curr: *mut mi_heap_t = (*(*heap).tld).heaps;
    while !curr.is_null() {
        let mut next: *mut mi_heap_t = (*curr).next;
        if curr != heap {
            if !mi_heap_is_backing(curr) {
            } else {
                _mi_assert_fail(
                    b"!mi_heap_is_backing(curr)\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    5716 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                        b"_mi_thread_heap_done\0",
                    ))
                    .as_ptr(),
                );
            };
            mi_heap_delete(curr);
        }
        curr = next;
    }
    if (*(*heap).tld).heaps == heap && ((*heap).next).is_null() {
    } else {
        _mi_assert_fail(
            b"heap->tld->heaps == heap && heap->next == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5721 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_thread_heap_done\0"))
                .as_ptr(),
        );
    };
    if mi_heap_is_backing(heap) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_is_backing(heap)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5722 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"_mi_thread_heap_done\0"))
                .as_ptr(),
        );
    };
    if heap != &mut _mi_heap_main as *mut mi_heap_t {
        _mi_heap_collect_abandon(heap);
    }
    _mi_stats_done(&mut (*(*heap).tld).stats);
    if heap != &mut _mi_heap_main as *mut mi_heap_t {
        if (*(*heap).tld).segments.count == 0 as libc::c_int as size_t
            || (*heap).thread_id != _mi_thread_id()
        {
        } else {
            _mi_assert_fail(
                b"heap->tld->segments.count == 0 || heap->thread_id != _mi_thread_id()\0"
                    as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                5728 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"_mi_thread_heap_done\0",
                ))
                .as_ptr(),
            );
        };
        mi_thread_data_free(heap as *mut mi_thread_data_t);
    }
    return 0 as libc::c_int != 0;
}
unsafe extern "C" fn mi_process_setup_auto_thread_done() {
    static mut tls_initialized: bool = 0 as libc::c_int != 0;
    if tls_initialized {
        return;
    }
    tls_initialized = 1 as libc::c_int != 0;
    _mi_prim_thread_init_auto_done();
    _mi_heap_set_default_direct(&mut _mi_heap_main);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_is_main_thread() -> bool {
    return _mi_heap_main.thread_id == 0 as libc::c_int as mi_threadid_t
        || _mi_heap_main.thread_id == _mi_thread_id();
}
static mut thread_count: size_t = 1 as libc::c_int as libc::c_ulong;
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_current_thread_count() -> size_t {
    return ::core::intrinsics::atomic_load_relaxed(&mut thread_count);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_thread_init() {
    mi_process_init();
    if _mi_thread_heap_init() {
        return;
    }
    _mi_stat_increase(&mut _mi_stats_main.threads, 1 as libc::c_int as size_t);
    ::core::intrinsics::atomic_xadd_relaxed(&mut thread_count, 1 as libc::c_int as uintptr_t);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_thread_done() {
    _mi_thread_done(0 as *mut mi_heap_t);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_thread_done(mut heap: *mut mi_heap_t) {
    if heap.is_null() {
        heap = mi_prim_get_default_heap();
        if heap.is_null() {
            return;
        }
    }
    if !mi_heap_is_initialized(heap) {
        return;
    }
    ::core::intrinsics::atomic_xsub_relaxed(&mut thread_count, 1 as libc::c_int as uintptr_t);
    _mi_stat_decrease(&mut _mi_stats_main.threads, 1 as libc::c_int as size_t);
    if (*heap).thread_id != _mi_thread_id() {
        return;
    }
    if _mi_thread_heap_done(heap) {
        return;
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_set_default_direct(mut heap: *mut mi_heap_t) {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5778 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"_mi_heap_set_default_direct\0",
            ))
            .as_ptr(),
        );
    };
    _mi_heap_default = heap;
    _mi_prim_thread_associate_default_heap(heap);
}
static mut os_preloading: bool = 1 as libc::c_int != 0;
#[unsafe(no_mangle)]
#[inline(never)]
pub unsafe extern "C" fn _mi_preloading() -> bool {
    return os_preloading;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_process_load() {
    mi_heap_main_init();
    os_preloading = 0 as libc::c_int != 0;
    if _mi_is_main_thread() as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"_mi_is_main_thread()\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            5800 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_process_load\0"))
                .as_ptr(),
        );
    };
    _mi_options_init();
    mi_process_setup_auto_thread_done();
    mi_process_init();
    if _mi_is_redirected() {
        _mi_verbose_message(b"malloc is redirected.\n\0" as *const u8 as *const libc::c_char);
    }
    let mut msg: *const libc::c_char = 0 as *const libc::c_char;
    _mi_allocator_init(&mut msg);
    if !msg.is_null()
        && (mi_option_is_enabled(mi_option_verbose) as libc::c_int != 0
            || mi_option_is_enabled(mi_option_show_errors) as libc::c_int != 0)
    {
        _mi_fputs(None, 0 as *mut libc::c_void, 0 as *const libc::c_char, msg);
    }
    _mi_random_reinit_if_weak(&mut _mi_heap_main.random);
}
unsafe extern "C" fn mi_detect_cpu_features() {}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_process_init() {
    static mut process_init: mi_atomic_once_t = 0;
    mi_heap_main_init();
    if !mi_atomic_once(&mut process_init) {
        return;
    }
    _mi_process_is_initialized = 1 as libc::c_int != 0;
    _mi_verbose_message(
        b"process init: 0x%zx\n\0" as *const u8 as *const libc::c_char,
        _mi_thread_id(),
    );
    mi_process_setup_auto_thread_done();
    mi_detect_cpu_features();
    _mi_os_init();
    mi_heap_main_init();
    _mi_verbose_message(
        b"debug level : %d\n\0" as *const u8 as *const libc::c_char,
        2 as libc::c_int,
    );
    _mi_verbose_message(
        b"secure level: %d\n\0" as *const u8 as *const libc::c_char,
        0 as libc::c_int,
    );
    _mi_verbose_message(
        b"mem tracking: %s\n\0" as *const u8 as *const libc::c_char,
        b"none\0" as *const u8 as *const libc::c_char,
    );
    mi_thread_init();
    mi_stats_reset();
    if mi_option_is_enabled(mi_option_reserve_huge_os_pages) {
        let mut pages: size_t = mi_option_get_clamp(
            mi_option_reserve_huge_os_pages,
            0 as libc::c_int as libc::c_long,
            (128 as libc::c_int * 1024 as libc::c_int) as libc::c_long,
        ) as size_t;
        let mut reserve_at: libc::c_long = mi_option_get(mi_option_reserve_huge_os_pages_at);
        if reserve_at != -(1 as libc::c_int) as libc::c_long {
            mi_reserve_huge_os_pages_at(
                pages,
                reserve_at as libc::c_int,
                pages * 500 as libc::c_int as size_t,
            );
        } else {
            mi_reserve_huge_os_pages_interleave(
                pages,
                0 as libc::c_int as size_t,
                pages * 500 as libc::c_int as size_t,
            );
        }
    }
    if mi_option_is_enabled(mi_option_reserve_os_memory) {
        let mut ksize: libc::c_long = mi_option_get(mi_option_reserve_os_memory);
        if ksize > 0 as libc::c_int as libc::c_long {
            mi_reserve_os_memory(
                (ksize as size_t as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong)
                    as size_t,
                1 as libc::c_int != 0,
                1 as libc::c_int != 0,
            );
        }
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_process_done() {
    if !_mi_process_is_initialized {
        return;
    }
    static mut process_done: bool = 0 as libc::c_int != 0;
    if process_done {
        return;
    }
    process_done = 1 as libc::c_int != 0;
    _mi_prim_thread_done_auto_done();
    mi_collect(1 as libc::c_int != 0);
    if mi_option_is_enabled(mi_option_destroy_on_exit) {
        mi_collect(1 as libc::c_int != 0);
        _mi_heap_unsafe_destroy_all();
        _mi_arena_unsafe_destroy_all(
            &mut (*(*(_mi_heap_main_get as unsafe extern "C" fn() -> *mut mi_heap_t)()).tld).stats,
        );
    }
    if mi_option_is_enabled(mi_option_show_stats) as libc::c_int != 0
        || mi_option_is_enabled(mi_option_verbose) as libc::c_int != 0
    {
        mi_stats_print(0 as *mut libc::c_void);
    }
    _mi_allocator_done();
    _mi_verbose_message(
        b"process done: 0x%zx\n\0" as *const u8 as *const libc::c_char,
        _mi_heap_main.thread_id,
    );
    os_preloading = 1 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_toupper(mut c: libc::c_char) -> libc::c_char {
    if c as libc::c_int >= 'a' as i32 && c as libc::c_int <= 'z' as i32 {
        return (c as libc::c_int - 'a' as i32 + 'A' as i32) as libc::c_char;
    } else {
        return c;
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_strnicmp(
    mut s: *const libc::c_char,
    mut t: *const libc::c_char,
    mut n: size_t,
) -> libc::c_int {
    if n == 0 as libc::c_int as size_t {
        return 0 as libc::c_int;
    }
    while *s as libc::c_int != 0 as libc::c_int
        && *t as libc::c_int != 0 as libc::c_int
        && n > 0 as libc::c_int as size_t
    {
        if _mi_toupper(*s) as libc::c_int != _mi_toupper(*t) as libc::c_int {
            break;
        }
        s = s.offset(1);
        s;
        t = t.offset(1);
        t;
        n = n.wrapping_sub(1);
        n;
    }
    return if n == 0 as libc::c_int as size_t {
        0 as libc::c_int
    } else {
        *s as libc::c_int - *t as libc::c_int
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_strlcpy(
    mut dest: *mut libc::c_char,
    mut src: *const libc::c_char,
    mut dest_size: size_t,
) {
    if dest.is_null() || src.is_null() || dest_size == 0 as libc::c_int as size_t {
        return;
    }
    while *src as libc::c_int != 0 as libc::c_int && dest_size > 1 as libc::c_int as size_t {
        let fresh28 = src;
        src = src.offset(1);
        let fresh29 = dest;
        dest = dest.offset(1);
        *fresh29 = *fresh28;
        dest_size = dest_size.wrapping_sub(1);
        dest_size;
    }
    *dest = 0 as libc::c_int as libc::c_char;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_strlcat(
    mut dest: *mut libc::c_char,
    mut src: *const libc::c_char,
    mut dest_size: size_t,
) {
    if dest.is_null() || src.is_null() || dest_size == 0 as libc::c_int as size_t {
        return;
    }
    while *dest as libc::c_int != 0 as libc::c_int && dest_size > 1 as libc::c_int as size_t {
        dest = dest.offset(1);
        dest;
        dest_size = dest_size.wrapping_sub(1);
        dest_size;
    }
    _mi_strlcpy(dest, src, dest_size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_strlen(mut s: *const libc::c_char) -> size_t {
    if s.is_null() {
        return 0 as libc::c_int as size_t;
    }
    let mut len: size_t = 0 as libc::c_int as size_t;
    while *s.offset(len as isize) as libc::c_int != 0 as libc::c_int {
        len = len.wrapping_add(1);
        len;
    }
    return len;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_strnlen(mut s: *const libc::c_char, mut max_len: size_t) -> size_t {
    if s.is_null() {
        return 0 as libc::c_int as size_t;
    }
    let mut len: size_t = 0 as libc::c_int as size_t;
    while *s.offset(len as isize) as libc::c_int != 0 as libc::c_int && len < max_len {
        len = len.wrapping_add(1);
        len;
    }
    return len;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_getenv(
    mut name: *const libc::c_char,
    mut result: *mut libc::c_char,
    mut result_size: size_t,
) -> bool {
    if name.is_null() || result.is_null() || result_size < 64 as libc::c_int as size_t {
        return 0 as libc::c_int != 0;
    }
    return _mi_prim_getenv(name, result, result_size);
}
unsafe extern "C" fn mi_outc(
    mut c: libc::c_char,
    mut out: *mut *mut libc::c_char,
    mut end: *mut libc::c_char,
) {
    let mut p: *mut libc::c_char = *out;
    if p >= end {
        return;
    }
    *p = c;
    *out = p.offset(1 as libc::c_int as isize);
}
unsafe extern "C" fn mi_outs(
    mut s: *const libc::c_char,
    mut out: *mut *mut libc::c_char,
    mut end: *mut libc::c_char,
) {
    if s.is_null() {
        return;
    }
    let mut p: *mut libc::c_char = *out;
    while *s as libc::c_int != 0 as libc::c_int && p < end {
        let fresh30 = s;
        s = s.offset(1);
        let fresh31 = p;
        p = p.offset(1);
        *fresh31 = *fresh30;
    }
    *out = p;
}
unsafe extern "C" fn mi_out_fill(
    mut fill: libc::c_char,
    mut len: size_t,
    mut out: *mut *mut libc::c_char,
    mut end: *mut libc::c_char,
) {
    let mut p: *mut libc::c_char = *out;
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < len && p < end {
        let fresh32 = p;
        p = p.offset(1);
        *fresh32 = fill;
        i = i.wrapping_add(1);
        i;
    }
    *out = p;
}
unsafe extern "C" fn mi_out_alignright(
    mut fill: libc::c_char,
    mut start: *mut libc::c_char,
    mut len: size_t,
    mut extra: size_t,
    mut end: *mut libc::c_char,
) {
    if len == 0 as libc::c_int as size_t || extra == 0 as libc::c_int as size_t {
        return;
    }
    if start.offset(len as isize).offset(extra as isize) >= end {
        return;
    }
    let mut i: size_t = 1 as libc::c_int as size_t;
    while i <= len {
        *start.offset(len.wrapping_add(extra).wrapping_sub(i) as isize) =
            *start.offset(len.wrapping_sub(i) as isize);
        i = i.wrapping_add(1);
        i;
    }
    let mut i_0: size_t = 0 as libc::c_int as size_t;
    while i_0 < extra {
        *start.offset(i_0 as isize) = fill;
        i_0 = i_0.wrapping_add(1);
        i_0;
    }
}
unsafe extern "C" fn mi_out_num(
    mut x: uintmax_t,
    mut base: size_t,
    mut prefix: libc::c_char,
    mut out: *mut *mut libc::c_char,
    mut end: *mut libc::c_char,
) {
    if x == 0 as libc::c_int as uintmax_t
        || base == 0 as libc::c_int as size_t
        || base > 16 as libc::c_int as size_t
    {
        if prefix as libc::c_int != 0 as libc::c_int {
            mi_outc(prefix, out, end);
        }
        mi_outc('0' as i32 as libc::c_char, out, end);
    } else {
        let mut start: *mut libc::c_char = *out;
        while x > 0 as libc::c_int as uintmax_t {
            let mut digit: libc::c_char = x.wrapping_rem(base) as libc::c_char;
            mi_outc(
                (if digit as libc::c_int <= 9 as libc::c_int {
                    '0' as i32 + digit as libc::c_int
                } else {
                    'A' as i32 + digit as libc::c_int - 10 as libc::c_int
                }) as libc::c_char,
                out,
                end,
            );
            x = x.wrapping_div(base);
        }
        if prefix as libc::c_int != 0 as libc::c_int {
            mi_outc(prefix, out, end);
        }
        let mut len: size_t = (*out).offset_from(start) as libc::c_long as size_t;
        let mut i: size_t = 0 as libc::c_int as size_t;
        while i < len / 2 as libc::c_int as size_t {
            let mut c: libc::c_char = *start
                .offset(len.wrapping_sub(i).wrapping_sub(1 as libc::c_int as size_t) as isize);
            *start.offset(len.wrapping_sub(i).wrapping_sub(1 as libc::c_int as size_t) as isize) =
                *start.offset(i as isize);
            *start.offset(i as isize) = c;
            i = i.wrapping_add(1);
            i;
        }
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_vsnprintf(
    mut buf: *mut libc::c_char,
    mut bufsize: size_t,
    mut fmt: *const libc::c_char,
    mut args: ::core::ffi::VaList,
) {
    if buf.is_null() || bufsize == 0 as libc::c_int as size_t || fmt.is_null() {
        return;
    }
    *buf.offset(bufsize.wrapping_sub(1 as libc::c_int as size_t) as isize) =
        0 as libc::c_int as libc::c_char;
    let end: *mut libc::c_char =
        buf.offset(bufsize.wrapping_sub(1 as libc::c_int as size_t) as isize);
    let mut in_0: *const libc::c_char = fmt;
    let mut out: *mut libc::c_char = buf;
    while !(out >= end) {
        let mut c: libc::c_char = 0;
        c = *in_0;
        if c as libc::c_int == 0 as libc::c_int {
            break;
        }
        in_0 = in_0.offset(1);
        in_0;
        if c as libc::c_int != '%' as i32 {
            if c as libc::c_int >= ' ' as i32 && c as libc::c_int <= '~' as i32
                || c as libc::c_int == '\n' as i32
                || c as libc::c_int == '\r' as i32
                || c as libc::c_int == '\t' as i32
            {
                mi_outc(c, &mut out, end);
            }
        } else {
            c = *in_0;
            if c as libc::c_int == 0 as libc::c_int {
                break;
            }
            in_0 = in_0.offset(1);
            in_0;
            let mut fill: libc::c_char = ' ' as i32 as libc::c_char;
            let mut width: size_t = 0 as libc::c_int as size_t;
            let mut numtype: libc::c_char = 'd' as i32 as libc::c_char;
            let mut numplus: libc::c_char = 0 as libc::c_int as libc::c_char;
            let mut alignright: bool = 1 as libc::c_int != 0;
            if c as libc::c_int == '+' as i32 || c as libc::c_int == ' ' as i32 {
                numplus = c;
                c = *in_0;
                if c as libc::c_int == 0 as libc::c_int {
                    break;
                }
                in_0 = in_0.offset(1);
                in_0;
            }
            if c as libc::c_int == '-' as i32 {
                alignright = 0 as libc::c_int != 0;
                c = *in_0;
                if c as libc::c_int == 0 as libc::c_int {
                    break;
                }
                in_0 = in_0.offset(1);
                in_0;
            }
            if c as libc::c_int == '0' as i32 {
                fill = '0' as i32 as libc::c_char;
                c = *in_0;
                if c as libc::c_int == 0 as libc::c_int {
                    break;
                }
                in_0 = in_0.offset(1);
                in_0;
            }
            if c as libc::c_int >= '1' as i32 && c as libc::c_int <= '9' as i32 {
                width = (c as libc::c_int - '0' as i32) as size_t;
                c = *in_0;
                if c as libc::c_int == 0 as libc::c_int {
                    break;
                }
                in_0 = in_0.offset(1);
                in_0;
                while c as libc::c_int >= '0' as i32 && c as libc::c_int <= '9' as i32 {
                    width = (10 as libc::c_int as size_t * width)
                        .wrapping_add((c as libc::c_int - '0' as i32) as size_t);
                    c = *in_0;
                    if c as libc::c_int == 0 as libc::c_int {
                        break;
                    }
                    in_0 = in_0.offset(1);
                    in_0;
                }
                if c as libc::c_int == 0 as libc::c_int {
                    break;
                }
            }
            if c as libc::c_int == 'z' as i32
                || c as libc::c_int == 't' as i32
                || c as libc::c_int == 'L' as i32
            {
                numtype = c;
                c = *in_0;
                if c as libc::c_int == 0 as libc::c_int {
                    break;
                }
                in_0 = in_0.offset(1);
                in_0;
            } else if c as libc::c_int == 'l' as i32 {
                numtype = c;
                c = *in_0;
                if c as libc::c_int == 0 as libc::c_int {
                    break;
                }
                in_0 = in_0.offset(1);
                in_0;
                if c as libc::c_int == 'l' as i32 {
                    numtype = 'L' as i32 as libc::c_char;
                    c = *in_0;
                    if c as libc::c_int == 0 as libc::c_int {
                        break;
                    }
                    in_0 = in_0.offset(1);
                    in_0;
                }
            }
            let mut start: *mut libc::c_char = out;
            if c as libc::c_int == 's' as i32 {
                let mut s: *const libc::c_char = args.arg::<*const libc::c_char>();
                mi_outs(s, &mut out, end);
            } else if c as libc::c_int == 'p' as i32
                || c as libc::c_int == 'x' as i32
                || c as libc::c_int == 'u' as i32
            {
                let mut x: uintmax_t = 0 as libc::c_int as uintmax_t;
                if c as libc::c_int == 'x' as i32 || c as libc::c_int == 'u' as i32 {
                    if numtype as libc::c_int == 'z' as i32 {
                        x = args.arg::<size_t>();
                    } else if numtype as libc::c_int == 't' as i32 {
                        x = args.arg::<uintptr_t>();
                    } else if numtype as libc::c_int == 'L' as i32 {
                        x = args.arg::<libc::c_ulonglong>() as uintmax_t;
                    } else if numtype as libc::c_int == 'l' as i32 {
                        x = args.arg::<libc::c_ulong>();
                    } else {
                        x = args.arg::<libc::c_uint>() as uintmax_t;
                    }
                } else if c as libc::c_int == 'p' as i32 {
                    x = args.arg::<uintptr_t>();
                    mi_outs(b"0x\0" as *const u8 as *const libc::c_char, &mut out, end);
                    start = out;
                    width = if width >= 2 as libc::c_int as size_t {
                        width.wrapping_sub(2 as libc::c_int as size_t)
                    } else {
                        0 as libc::c_int as size_t
                    };
                }
                if width == 0 as libc::c_int as size_t
                    && (c as libc::c_int == 'x' as i32 || c as libc::c_int == 'p' as i32)
                {
                    if c as libc::c_int == 'p' as i32 {
                        width = (2 as libc::c_int as libc::c_ulong).wrapping_mul(
                            (if x <= 4294967295 as libc::c_uint as uintmax_t {
                                4 as libc::c_int as libc::c_ulong
                            } else {
                                (if x >> 16 as libc::c_int
                                    <= 4294967295 as libc::c_uint as uintmax_t
                                {
                                    6 as libc::c_int as libc::c_ulong
                                } else {
                                    ::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong
                                })
                            }),
                        );
                    }
                    if width == 0 as libc::c_int as size_t {
                        width = 2 as libc::c_int as size_t;
                    }
                    fill = '0' as i32 as libc::c_char;
                }
                mi_out_num(
                    x,
                    (if c as libc::c_int == 'x' as i32 || c as libc::c_int == 'p' as i32 {
                        16 as libc::c_int
                    } else {
                        10 as libc::c_int
                    }) as size_t,
                    numplus,
                    &mut out,
                    end,
                );
            } else if c as libc::c_int == 'i' as i32 || c as libc::c_int == 'd' as i32 {
                let mut x_0: intmax_t = 0 as libc::c_int as intmax_t;
                if numtype as libc::c_int == 'z' as i32 {
                    x_0 = args.arg::<intptr_t>();
                } else if numtype as libc::c_int == 't' as i32 {
                    x_0 = args.arg::<ptrdiff_t>();
                } else if numtype as libc::c_int == 'L' as i32 {
                    x_0 = args.arg::<libc::c_longlong>() as intmax_t;
                } else if numtype as libc::c_int == 'l' as i32 {
                    x_0 = args.arg::<libc::c_long>();
                } else {
                    x_0 = args.arg::<libc::c_int>() as intmax_t;
                }
                let mut pre: libc::c_char = 0 as libc::c_int as libc::c_char;
                if x_0 < 0 as libc::c_int as intmax_t {
                    pre = '-' as i32 as libc::c_char;
                    if x_0
                        > -(9223372036854775807 as libc::c_long) - 1 as libc::c_int as libc::c_long
                    {
                        x_0 = -x_0;
                    }
                } else if numplus as libc::c_int != 0 as libc::c_int {
                    pre = numplus;
                }
                mi_out_num(
                    x_0 as uintmax_t,
                    10 as libc::c_int as size_t,
                    pre,
                    &mut out,
                    end,
                );
            } else if c as libc::c_int >= ' ' as i32 && c as libc::c_int <= '~' as i32 {
                mi_outc('%' as i32 as libc::c_char, &mut out, end);
                mi_outc(c, &mut out, end);
            }
            if out <= end {
            } else {
                _mi_assert_fail(
                    b"out <= end\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    6084 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"_mi_vsnprintf\0"))
                        .as_ptr(),
                );
            };
            if out >= start {
            } else {
                _mi_assert_fail(
                    b"out >= start\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    6085 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"_mi_vsnprintf\0"))
                        .as_ptr(),
                );
            };
            let len: size_t = out.offset_from(start) as libc::c_long as size_t;
            if len < width {
                mi_out_fill(fill, width.wrapping_sub(len), &mut out, end);
                if alignright as libc::c_int != 0 && out <= end {
                    mi_out_alignright(fill, start, len, width.wrapping_sub(len), end);
                }
            }
        }
    }
    if out <= end {
    } else {
        _mi_assert_fail(
            b"out <= end\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6095 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"_mi_vsnprintf\0"))
                .as_ptr(),
        );
    };
    *out = 0 as libc::c_int as libc::c_char;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_snprintf(
    mut buf: *mut libc::c_char,
    mut buflen: size_t,
    mut fmt: *const libc::c_char,
    mut args: ...
) {
    let mut args_0: ::core::ffi::VaListImpl;
    args_0 = args.clone();
    _mi_vsnprintf(buf, buflen, fmt, args_0.as_va_list());
}
static mut mi_max_error_count: libc::c_long = 16 as libc::c_int as libc::c_long;
static mut mi_max_warning_count: libc::c_long = 16 as libc::c_int as libc::c_long;
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_version() -> libc::c_int {
    return 188 as libc::c_int;
}
static mut options: [mi_option_desc_t; 36] = [mi_option_desc_s {
    value: 0,
    init: UNINIT,
    option: mi_option_show_errors,
    name: 0 as *const libc::c_char,
    legacy_name: 0 as *const libc::c_char,
}; 36];
unsafe extern "C" fn mi_option_has_size_in_kib(mut option: mi_option_t) -> bool {
    return option as libc::c_uint == mi_option_reserve_os_memory as libc::c_int as libc::c_uint
        || option as libc::c_uint == mi_option_arena_reserve as libc::c_int as libc::c_uint;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_options_init() {
    mi_add_stderr_output();
    let mut i: libc::c_int = 0 as libc::c_int;
    while i < _mi_option_last as libc::c_int {
        let mut option: mi_option_t = i as mi_option_t;
        let mut l: libc::c_long = mi_option_get(option);
        let mut desc: *mut mi_option_desc_t =
            &mut *options.as_mut_ptr().offset(option as isize) as *mut mi_option_desc_t;
        _mi_verbose_message(
            b"option '%s': %ld %s\n\0" as *const u8 as *const libc::c_char,
            (*desc).name,
            (*desc).value,
            if mi_option_has_size_in_kib(option) as libc::c_int != 0 {
                b"KiB\0" as *const u8 as *const libc::c_char
            } else {
                b"\0" as *const u8 as *const libc::c_char
            },
        );
        i += 1;
        i;
    }
    mi_max_error_count = mi_option_get(mi_option_max_errors);
    mi_max_warning_count = mi_option_get(mi_option_max_warnings);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_option_get_fast(mut option: mi_option_t) -> libc::c_long {
    if option as libc::c_uint >= 0 as libc::c_int as libc::c_uint
        && (option as libc::c_uint) < _mi_option_last as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"option >= 0 && option < _mi_option_last\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6245 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"_mi_option_get_fast\0"))
                .as_ptr(),
        );
    };
    let mut desc: *mut mi_option_desc_t =
        &mut *options.as_mut_ptr().offset(option as isize) as *mut mi_option_desc_t;
    if (*desc).option as libc::c_uint == option as libc::c_uint {
    } else {
        _mi_assert_fail(
            b"desc->option == option\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6247 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"_mi_option_get_fast\0"))
                .as_ptr(),
        );
    };
    return (*desc).value;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_get(mut option: mi_option_t) -> libc::c_long {
    if option as libc::c_uint >= 0 as libc::c_int as libc::c_uint
        && (option as libc::c_uint) < _mi_option_last as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"option >= 0 && option < _mi_option_last\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6251 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_option_get\0"))
                .as_ptr(),
        );
    };
    if (option as libc::c_uint) < 0 as libc::c_int as libc::c_uint
        || option as libc::c_uint >= _mi_option_last as libc::c_int as libc::c_uint
    {
        return 0 as libc::c_int as libc::c_long;
    }
    let mut desc: *mut mi_option_desc_t =
        &mut *options.as_mut_ptr().offset(option as isize) as *mut mi_option_desc_t;
    if (*desc).option as libc::c_uint == option as libc::c_uint {
    } else {
        _mi_assert_fail(
            b"desc->option == option\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6254 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_option_get\0"))
                .as_ptr(),
        );
    };
    if ((*desc).init as libc::c_uint == UNINIT as libc::c_int as libc::c_uint) as libc::c_int
        as libc::c_long
        != 0
    {
        mi_option_init(desc);
    }
    return (*desc).value;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_get_clamp(
    mut option: mi_option_t,
    mut min: libc::c_long,
    mut max: libc::c_long,
) -> libc::c_long {
    let mut x: libc::c_long = mi_option_get(option);
    return if x < min {
        min
    } else if x > max {
        max
    } else {
        x
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_get_size(mut option: mi_option_t) -> size_t {
    let x: libc::c_long = mi_option_get(option);
    let mut size: size_t = if x < 0 as libc::c_int as libc::c_long {
        0 as libc::c_int as size_t
    } else {
        x as size_t
    };
    if mi_option_has_size_in_kib(option) {
        size =
            (size as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong) as size_t as size_t;
    }
    return size;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_set(mut option: mi_option_t, mut value: libc::c_long) {
    if option as libc::c_uint >= 0 as libc::c_int as libc::c_uint
        && (option as libc::c_uint) < _mi_option_last as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"option >= 0 && option < _mi_option_last\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6273 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_option_set\0"))
                .as_ptr(),
        );
    };
    if (option as libc::c_uint) < 0 as libc::c_int as libc::c_uint
        || option as libc::c_uint >= _mi_option_last as libc::c_int as libc::c_uint
    {
        return;
    }
    let mut desc: *mut mi_option_desc_t =
        &mut *options.as_mut_ptr().offset(option as isize) as *mut mi_option_desc_t;
    if (*desc).option as libc::c_uint == option as libc::c_uint {
    } else {
        _mi_assert_fail(
            b"desc->option == option\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6276 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_option_set\0"))
                .as_ptr(),
        );
    };
    (*desc).value = value;
    (*desc).init = INITIALIZED;
    if (*desc).option as libc::c_uint == mi_option_guarded_min as libc::c_int as libc::c_uint
        && _mi_option_get_fast(mi_option_guarded_max) < value
    {
        mi_option_set(mi_option_guarded_max, value);
    } else if (*desc).option as libc::c_uint == mi_option_guarded_max as libc::c_int as libc::c_uint
        && _mi_option_get_fast(mi_option_guarded_min) > value
    {
        mi_option_set(mi_option_guarded_min, value);
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_set_default(mut option: mi_option_t, mut value: libc::c_long) {
    if option as libc::c_uint >= 0 as libc::c_int as libc::c_uint
        && (option as libc::c_uint) < _mi_option_last as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"option >= 0 && option < _mi_option_last\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6287 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_option_set_default\0"))
                .as_ptr(),
        );
    };
    if (option as libc::c_uint) < 0 as libc::c_int as libc::c_uint
        || option as libc::c_uint >= _mi_option_last as libc::c_int as libc::c_uint
    {
        return;
    }
    let mut desc: *mut mi_option_desc_t =
        &mut *options.as_mut_ptr().offset(option as isize) as *mut mi_option_desc_t;
    if (*desc).init as libc::c_uint != INITIALIZED as libc::c_int as libc::c_uint {
        (*desc).value = value;
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_is_enabled(mut option: mi_option_t) -> bool {
    return mi_option_get(option) != 0 as libc::c_int as libc::c_long;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_set_enabled(mut option: mi_option_t, mut enable: bool) {
    mi_option_set(
        option,
        (if enable as libc::c_int != 0 {
            1 as libc::c_int
        } else {
            0 as libc::c_int
        }) as libc::c_long,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_set_enabled_default(mut option: mi_option_t, mut enable: bool) {
    mi_option_set_default(
        option,
        (if enable as libc::c_int != 0 {
            1 as libc::c_int
        } else {
            0 as libc::c_int
        }) as libc::c_long,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_enable(mut option: mi_option_t) {
    mi_option_set_enabled(option, 1 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_option_disable(mut option: mi_option_t) {
    mi_option_set_enabled(option, 0 as libc::c_int != 0);
}
unsafe extern "C" fn mi_out_stderr(mut msg: *const libc::c_char, mut arg: *mut libc::c_void) {
    if !msg.is_null() && *msg.offset(0 as libc::c_int as isize) as libc::c_int != 0 as libc::c_int {
        _mi_prim_out_stderr(msg);
    }
}
static mut out_buf: [libc::c_char; 16385] = [0; 16385];
static mut out_len: size_t = 0;
unsafe extern "C" fn mi_out_buf(mut msg: *const libc::c_char, mut arg: *mut libc::c_void) {
    if msg.is_null() {
        return;
    }
    if ::core::intrinsics::atomic_load_relaxed(&mut out_len as *mut size_t)
        >= (16 as libc::c_int * 1024 as libc::c_int) as size_t
    {
        return;
    }
    let mut n: size_t = _mi_strlen(msg);
    if n == 0 as libc::c_int as size_t {
        return;
    }
    let mut start: size_t = ::core::intrinsics::atomic_xadd_acqrel(&mut out_len, n);
    if start >= (16 as libc::c_int * 1024 as libc::c_int) as size_t {
        return;
    }
    if start.wrapping_add(n) >= (16 as libc::c_int * 1024 as libc::c_int) as size_t {
        n = ((16 as libc::c_int * 1024 as libc::c_int) as size_t)
            .wrapping_sub(start)
            .wrapping_sub(1 as libc::c_int as size_t);
    }
    _mi_memcpy(
        &mut *out_buf.as_mut_ptr().offset(start as isize) as *mut libc::c_char as *mut libc::c_void,
        msg as *const libc::c_void,
        n,
    );
}
unsafe extern "C" fn mi_out_buf_flush(
    mut out: Option<mi_output_fun>,
    mut no_more_buf: bool,
    mut arg: *mut libc::c_void,
) {
    if out.is_none() {
        return;
    }
    let mut count: size_t = ::core::intrinsics::atomic_xadd_acqrel(
        &mut out_len,
        if no_more_buf as libc::c_int != 0 {
            (16 as libc::c_int * 1024 as libc::c_int) as size_t
        } else {
            1 as libc::c_int as size_t
        },
    );
    if count > (16 as libc::c_int * 1024 as libc::c_int) as size_t {
        count = (16 as libc::c_int * 1024 as libc::c_int) as size_t;
    }
    out_buf[count as usize] = 0 as libc::c_int as libc::c_char;
    out.expect("non-null function pointer")(out_buf.as_mut_ptr(), arg);
    if !no_more_buf {
        out_buf[count as usize] = '\n' as i32 as libc::c_char;
    }
}
unsafe extern "C" fn mi_out_buf_stderr(mut msg: *const libc::c_char, mut arg: *mut libc::c_void) {
    mi_out_stderr(msg, arg);
    mi_out_buf(msg, arg);
}
static mut mi_out_default: Option<mi_output_fun> = None;
static mut mi_out_arg: *mut libc::c_void = 0 as *const libc::c_void as *mut libc::c_void;
unsafe extern "C" fn mi_out_get_default(mut parg: *mut *mut libc::c_void) -> Option<mi_output_fun> {
    if !parg.is_null() {
        *parg = ::core::intrinsics::atomic_load_acquire(&mut mi_out_arg);
    }
    let mut out: Option<mi_output_fun> = mi_out_default;
    return if out.is_none() {
        Some(mi_out_buf as unsafe extern "C" fn(*const libc::c_char, *mut libc::c_void) -> ())
    } else {
        out
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_register_output(
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    ::core::ptr::write_volatile(
        &mut mi_out_default as *mut Option<mi_output_fun>,
        if out.is_none() {
            Some(
                mi_out_stderr as unsafe extern "C" fn(*const libc::c_char, *mut libc::c_void) -> (),
            )
        } else {
            out
        },
    );
    ::core::intrinsics::atomic_store_release(&mut mi_out_arg, arg);
    if out.is_some() {
        mi_out_buf_flush(out, 1 as libc::c_int != 0, arg);
    }
}
unsafe extern "C" fn mi_add_stderr_output() {
    if mi_out_default.is_none() {
    } else {
        _mi_assert_fail(
            b"mi_out_default == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6360 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_add_stderr_output\0"))
                .as_ptr(),
        );
    };
    mi_out_buf_flush(
        Some(mi_out_stderr as unsafe extern "C" fn(*const libc::c_char, *mut libc::c_void) -> ()),
        0 as libc::c_int != 0,
        0 as *mut libc::c_void,
    );
    ::core::ptr::write_volatile(
        &mut mi_out_default as *mut Option<mi_output_fun>,
        Some(
            mi_out_buf_stderr as unsafe extern "C" fn(*const libc::c_char, *mut libc::c_void) -> (),
        ),
    );
}
static mut error_count: size_t = 0;
static mut warning_count: size_t = 0;
#[thread_local]
static mut recurse: bool = 0 as libc::c_int != 0;
#[inline(never)]
unsafe extern "C" fn mi_recurse_enter_prim() -> bool {
    if recurse {
        return 0 as libc::c_int != 0;
    }
    recurse = 1 as libc::c_int != 0;
    return 1 as libc::c_int != 0;
}
#[inline(never)]
unsafe extern "C" fn mi_recurse_exit_prim() {
    recurse = 0 as libc::c_int != 0;
}
unsafe extern "C" fn mi_recurse_enter() -> bool {
    return mi_recurse_enter_prim();
}
unsafe extern "C" fn mi_recurse_exit() {
    mi_recurse_exit_prim();
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_fputs(
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
    mut prefix: *const libc::c_char,
    mut message: *const libc::c_char,
) {
    if out.is_none()
        || ::core::mem::transmute::<Option<mi_output_fun>, *mut libc::c_void>(out)
            == stdout as *mut libc::c_void
        || ::core::mem::transmute::<Option<mi_output_fun>, *mut libc::c_void>(out)
            == stderr as *mut libc::c_void
    {
        if !mi_recurse_enter() {
            return;
        }
        out = mi_out_get_default(&mut arg);
        if !prefix.is_null() {
            out.expect("non-null function pointer")(prefix, arg);
        }
        out.expect("non-null function pointer")(message, arg);
        mi_recurse_exit();
    } else {
        if !prefix.is_null() {
            out.expect("non-null function pointer")(prefix, arg);
        }
        out.expect("non-null function pointer")(message, arg);
    };
}
unsafe extern "C" fn mi_vfprintf(
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
    mut prefix: *const libc::c_char,
    mut fmt: *const libc::c_char,
    mut args: ::core::ffi::VaList,
) {
    let mut buf: [libc::c_char; 512] = [0; 512];
    if fmt.is_null() {
        return;
    }
    if !mi_recurse_enter() {
        return;
    }
    _mi_vsnprintf(
        buf.as_mut_ptr(),
        (::core::mem::size_of::<[libc::c_char; 512]>() as libc::c_ulong)
            .wrapping_sub(1 as libc::c_int as libc::c_ulong),
        fmt,
        args.as_va_list(),
    );
    mi_recurse_exit();
    _mi_fputs(out, arg, prefix, buf.as_mut_ptr());
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_fprintf(
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
    mut fmt: *const libc::c_char,
    mut args: ...
) {
    let mut args_0: ::core::ffi::VaListImpl;
    args_0 = args.clone();
    mi_vfprintf(out, arg, 0 as *const libc::c_char, fmt, args_0.as_va_list());
}
unsafe extern "C" fn mi_vfprintf_thread(
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
    mut prefix: *const libc::c_char,
    mut fmt: *const libc::c_char,
    mut args: ::core::ffi::VaList,
) {
    if !prefix.is_null()
        && _mi_strnlen(prefix, 33 as libc::c_int as size_t) <= 32 as libc::c_int as size_t
        && !_mi_is_main_thread()
    {
        let mut tprefix: [libc::c_char; 64] = [0; 64];
        _mi_snprintf(
            tprefix.as_mut_ptr(),
            ::core::mem::size_of::<[libc::c_char; 64]>() as libc::c_ulong,
            b"%sthread 0x%tx: \0" as *const u8 as *const libc::c_char,
            prefix,
            _mi_thread_id(),
        );
        mi_vfprintf(out, arg, tprefix.as_mut_ptr(), fmt, args.as_va_list());
    } else {
        mi_vfprintf(out, arg, prefix, fmt, args.as_va_list());
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_trace_message(mut fmt: *const libc::c_char, mut args: ...) {
    if mi_option_get(mi_option_verbose) <= 1 as libc::c_int as libc::c_long {
        return;
    }
    let mut args_0: ::core::ffi::VaListImpl;
    args_0 = args.clone();
    mi_vfprintf_thread(
        None,
        0 as *mut libc::c_void,
        b"mimalloc: \0" as *const u8 as *const libc::c_char,
        fmt,
        args_0.as_va_list(),
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_verbose_message(mut fmt: *const libc::c_char, mut args: ...) {
    if !mi_option_is_enabled(mi_option_verbose) {
        return;
    }
    let mut args_0: ::core::ffi::VaListImpl;
    args_0 = args.clone();
    mi_vfprintf(
        None,
        0 as *mut libc::c_void,
        b"mimalloc: \0" as *const u8 as *const libc::c_char,
        fmt,
        args_0.as_va_list(),
    );
}
unsafe extern "C" fn mi_show_error_message(
    mut fmt: *const libc::c_char,
    mut args: ::core::ffi::VaList,
) {
    if !mi_option_is_enabled(mi_option_verbose) {
        if !mi_option_is_enabled(mi_option_show_errors) {
            return;
        }
        if mi_max_error_count >= 0 as libc::c_int as libc::c_long
            && ::core::intrinsics::atomic_xadd_acqrel(
                &mut error_count as *mut size_t,
                1 as libc::c_int as uintptr_t,
            ) as libc::c_long
                > mi_max_error_count
        {
            return;
        }
    }
    mi_vfprintf_thread(
        None,
        0 as *mut libc::c_void,
        b"mimalloc: error: \0" as *const u8 as *const libc::c_char,
        fmt,
        args.as_va_list(),
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_warning_message(mut fmt: *const libc::c_char, mut args: ...) {
    if !mi_option_is_enabled(mi_option_verbose) {
        if !mi_option_is_enabled(mi_option_show_errors) {
            return;
        }
        if mi_max_warning_count >= 0 as libc::c_int as libc::c_long
            && ::core::intrinsics::atomic_xadd_acqrel(
                &mut warning_count as *mut size_t,
                1 as libc::c_int as uintptr_t,
            ) as libc::c_long
                > mi_max_warning_count
        {
            return;
        }
    }
    let mut args_0: ::core::ffi::VaListImpl;
    args_0 = args.clone();
    mi_vfprintf_thread(
        None,
        0 as *mut libc::c_void,
        b"mimalloc: warning: \0" as *const u8 as *const libc::c_char,
        fmt,
        args_0.as_va_list(),
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_assert_fail(
    mut assertion: *const libc::c_char,
    mut fname: *const libc::c_char,
    mut line: libc::c_uint,
    mut func: *const libc::c_char,
) {
    _mi_fprintf(
        None,
        0 as *mut libc::c_void,
        b"mimalloc: assertion failed: at \"%s\":%u, %s\n  assertion: \"%s\"\n\0" as *const u8
            as *const libc::c_char,
        fname,
        line,
        if func.is_null() {
            b"\0" as *const u8 as *const libc::c_char
        } else {
            func
        },
        assertion,
    );
    abort();
}
static mut mi_error_handler: Option<mi_error_fun> = None;
static mut mi_error_arg: *mut libc::c_void = 0 as *const libc::c_void as *mut libc::c_void;
unsafe extern "C" fn mi_error_default(mut err: libc::c_int) {
    if err == 14 as libc::c_int {
        abort();
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_register_error(
    mut fun: Option<mi_error_fun>,
    mut arg: *mut libc::c_void,
) {
    ::core::ptr::write_volatile(&mut mi_error_handler as *mut Option<mi_error_fun>, fun);
    ::core::intrinsics::atomic_store_release(&mut mi_error_arg, arg);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_error_message(
    mut err: libc::c_int,
    mut fmt: *const libc::c_char,
    mut args: ...
) {
    let mut args_0: ::core::ffi::VaListImpl;
    args_0 = args.clone();
    mi_show_error_message(fmt, args_0.as_va_list());
    if mi_error_handler.is_some() {
        mi_error_handler.expect("non-null function pointer")(
            err,
            ::core::intrinsics::atomic_load_acquire(&mut mi_error_arg),
        );
    } else {
        mi_error_default(err);
    };
}
unsafe extern "C" fn mi_option_init(mut desc: *mut mi_option_desc_t) {
    let mut s: [libc::c_char; 65] = [0; 65];
    let mut buf: [libc::c_char; 65] = [0; 65];
    _mi_strlcpy(
        buf.as_mut_ptr(),
        b"mimalloc_\0" as *const u8 as *const libc::c_char,
        ::core::mem::size_of::<[libc::c_char; 65]>() as libc::c_ulong,
    );
    _mi_strlcat(
        buf.as_mut_ptr(),
        (*desc).name,
        ::core::mem::size_of::<[libc::c_char; 65]>() as libc::c_ulong,
    );
    let mut found: bool = _mi_getenv(
        buf.as_mut_ptr(),
        s.as_mut_ptr(),
        ::core::mem::size_of::<[libc::c_char; 65]>() as libc::c_ulong,
    );
    if !found && !((*desc).legacy_name).is_null() {
        _mi_strlcpy(
            buf.as_mut_ptr(),
            b"mimalloc_\0" as *const u8 as *const libc::c_char,
            ::core::mem::size_of::<[libc::c_char; 65]>() as libc::c_ulong,
        );
        _mi_strlcat(
            buf.as_mut_ptr(),
            (*desc).legacy_name,
            ::core::mem::size_of::<[libc::c_char; 65]>() as libc::c_ulong,
        );
        found = _mi_getenv(
            buf.as_mut_ptr(),
            s.as_mut_ptr(),
            ::core::mem::size_of::<[libc::c_char; 65]>() as libc::c_ulong,
        );
        if found {
            _mi_warning_message(
                b"environment option \"mimalloc_%s\" is deprecated -- use \"mimalloc_%s\" instead.\n\0"
                    as *const u8 as *const libc::c_char,
                (*desc).legacy_name,
                (*desc).name,
            );
        }
    }
    if found {
        let mut len: size_t = _mi_strnlen(
            s.as_mut_ptr(),
            (::core::mem::size_of::<[libc::c_char; 65]>() as libc::c_ulong)
                .wrapping_sub(1 as libc::c_int as libc::c_ulong),
        );
        let mut i: size_t = 0 as libc::c_int as size_t;
        while i < len {
            buf[i as usize] = _mi_toupper(s[i as usize]);
            i = i.wrapping_add(1);
            i;
        }
        buf[len as usize] = 0 as libc::c_int as libc::c_char;
        if buf[0 as libc::c_int as usize] as libc::c_int == 0 as libc::c_int
            || !(strstr(
                b"1;TRUE;YES;ON\0" as *const u8 as *const libc::c_char,
                buf.as_mut_ptr(),
            ))
            .is_null()
        {
            (*desc).value = 1 as libc::c_int as libc::c_long;
            (*desc).init = INITIALIZED;
        } else if !(strstr(
            b"0;FALSE;NO;OFF\0" as *const u8 as *const libc::c_char,
            buf.as_mut_ptr(),
        ))
        .is_null()
        {
            (*desc).value = 0 as libc::c_int as libc::c_long;
            (*desc).init = INITIALIZED;
        } else {
            let mut end: *mut libc::c_char = buf.as_mut_ptr();
            let mut value: libc::c_long = strtol(buf.as_mut_ptr(), &mut end, 10 as libc::c_int);
            if mi_option_has_size_in_kib((*desc).option) {
                let mut size: size_t = if value < 0 as libc::c_int as libc::c_long {
                    0 as libc::c_int as size_t
                } else {
                    value as size_t
                };
                let mut overflow: bool = 0 as libc::c_int != 0;
                if *end as libc::c_int == 'K' as i32 {
                    end = end.offset(1);
                    end;
                } else if *end as libc::c_int == 'M' as i32 {
                    overflow =
                        mi_mul_overflow(size, 1024 as libc::c_ulonglong as size_t, &mut size);
                    end = end.offset(1);
                    end;
                } else if *end as libc::c_int == 'G' as i32 {
                    overflow = mi_mul_overflow(
                        size,
                        (1024 as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong)
                            as size_t,
                        &mut size,
                    );
                    end = end.offset(1);
                    end;
                } else if *end as libc::c_int == 'T' as i32 {
                    overflow = mi_mul_overflow(
                        size,
                        (1024 as libc::c_ulonglong)
                            .wrapping_mul(1024 as libc::c_ulonglong)
                            .wrapping_mul(1024 as libc::c_ulonglong)
                            as size_t,
                        &mut size,
                    );
                    end = end.offset(1);
                    end;
                } else {
                    size = (size as libc::c_ulonglong)
                        .wrapping_add(1024 as libc::c_ulonglong)
                        .wrapping_sub(1 as libc::c_int as libc::c_ulonglong)
                        .wrapping_div(1024 as libc::c_ulonglong)
                        as size_t;
                }
                if *end.offset(0 as libc::c_int as isize) as libc::c_int == 'I' as i32
                    && *end.offset(1 as libc::c_int as isize) as libc::c_int == 'B' as i32
                {
                    end = end.offset(2 as libc::c_int as isize);
                } else if *end as libc::c_int == 'B' as i32 {
                    end = end.offset(1);
                    end;
                }
                if overflow as libc::c_int != 0
                    || size > 9223372036854775807 as libc::c_long as size_t
                {
                    size = (9223372036854775807 as libc::c_long as libc::c_ulonglong)
                        .wrapping_div(1024 as libc::c_ulonglong)
                        as size_t;
                }
                value = if size > 9223372036854775807 as libc::c_long as size_t {
                    9223372036854775807 as libc::c_long
                } else {
                    size as libc::c_long
                };
            }
            if *end as libc::c_int == 0 as libc::c_int {
                mi_option_set((*desc).option, value);
            } else {
                (*desc).init = DEFAULTED;
                if (*desc).option as libc::c_uint
                    == mi_option_verbose as libc::c_int as libc::c_uint
                    && (*desc).value == 0 as libc::c_int as libc::c_long
                {
                    (*desc).value = 1 as libc::c_int as libc::c_long;
                    _mi_warning_message(
                        b"environment option mimalloc_%s has an invalid value.\n\0" as *const u8
                            as *const libc::c_char,
                        (*desc).name,
                    );
                    (*desc).value = 0 as libc::c_int as libc::c_long;
                } else {
                    _mi_warning_message(
                        b"environment option mimalloc_%s has an invalid value.\n\0" as *const u8
                            as *const libc::c_char,
                        (*desc).name,
                    );
                }
            }
        }
        if (*desc).init as libc::c_uint != UNINIT as libc::c_int as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"desc->init != UNINIT\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                6560 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"mi_option_init\0"))
                    .as_ptr(),
            );
        };
    } else if !_mi_preloading() {
        (*desc).init = DEFAULTED;
    }
}
static mut mi_os_mem_config: mi_os_mem_config_t = mi_os_mem_config_s {
    page_size: 0,
    large_page_size: 0,
    alloc_granularity: 0,
    physical_memory: 0,
    virtual_address_bits: 0,
    has_overcommit: false,
    has_partial_free: false,
    has_virtual_reserve: false,
};
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_has_overcommit() -> bool {
    return mi_os_mem_config.has_overcommit;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_has_virtual_reserve() -> bool {
    return mi_os_mem_config.has_virtual_reserve;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_page_size() -> size_t {
    return mi_os_mem_config.page_size;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_large_page_size() -> size_t {
    return if mi_os_mem_config.large_page_size != 0 as libc::c_int as size_t {
        mi_os_mem_config.large_page_size
    } else {
        _mi_os_page_size()
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_use_large_page(mut size: size_t, mut alignment: size_t) -> bool {
    if mi_os_mem_config.large_page_size == 0 as libc::c_int as size_t
        || !mi_option_is_enabled(mi_option_allow_large_os_pages)
    {
        return 0 as libc::c_int != 0;
    }
    return size % mi_os_mem_config.large_page_size == 0 as libc::c_int as size_t
        && alignment % mi_os_mem_config.large_page_size == 0 as libc::c_int as size_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_good_alloc_size(mut size: size_t) -> size_t {
    let mut align_size: size_t = 0;
    if (size as libc::c_ulonglong)
        < (512 as libc::c_int as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong)
    {
        align_size = _mi_os_page_size();
    } else if (size as libc::c_ulonglong)
        < (2 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul((1024 as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong))
    {
        align_size = (64 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong) as size_t;
    } else if (size as libc::c_ulonglong)
        < (8 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul((1024 as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong))
    {
        align_size = (256 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong) as size_t;
    } else if (size as libc::c_ulonglong)
        < (32 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul((1024 as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong))
    {
        align_size = (1 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul((1024 as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong))
            as size_t;
    } else {
        align_size = (4 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul((1024 as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong))
            as size_t;
    }
    if (size >= (18446744073709551615 as libc::c_ulong).wrapping_sub(align_size)) as libc::c_int
        as libc::c_long
        != 0
    {
        return size;
    }
    return _mi_align_up(size, align_size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_init() {
    _mi_prim_mem_init(&mut mi_os_mem_config);
}
#[inline]
unsafe extern "C" fn _mi_align_down(mut sz: uintptr_t, mut alignment: size_t) -> uintptr_t {
    if alignment != 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"alignment != 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6622 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_align_down\0"))
                .as_ptr(),
        );
    };
    let mut mask: uintptr_t = alignment.wrapping_sub(1 as libc::c_int as size_t);
    if alignment & mask == 0 as libc::c_int as libc::c_ulong {
        return sz & !mask;
    } else {
        return sz.wrapping_div(alignment).wrapping_mul(alignment);
    };
}
unsafe extern "C" fn mi_align_down_ptr(
    mut p: *mut libc::c_void,
    mut alignment: size_t,
) -> *mut libc::c_void {
    return _mi_align_down(p as uintptr_t, alignment) as *mut libc::c_void;
}
static mut aligned_base: uintptr_t = 0;
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_get_aligned_hint(
    mut try_alignment: size_t,
    mut size: size_t,
) -> *mut libc::c_void {
    if try_alignment <= 1 as libc::c_int as size_t
        || try_alignment as libc::c_ulonglong
            > (1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
    {
        return 0 as *mut libc::c_void;
    }
    if mi_os_mem_config.virtual_address_bits < 46 as libc::c_int as size_t {
        return 0 as *mut libc::c_void;
    }
    size = _mi_align_up(
        size,
        ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            as size_t,
    );
    if size as libc::c_ulonglong
        > (1 as libc::c_int as libc::c_ulonglong).wrapping_mul(
            (1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong),
        )
    {
        return 0 as *mut libc::c_void;
    }
    let mut hint: uintptr_t = ::core::intrinsics::atomic_xadd_acqrel(&mut aligned_base, size);
    if hint == 0 as libc::c_int as uintptr_t
        || hint > (30 as libc::c_int as uintptr_t) << 40 as libc::c_int
    {
        let mut init: uintptr_t = (2 as libc::c_int as uintptr_t) << 40 as libc::c_int;
        let mut expected: uintptr_t = hint.wrapping_add(size);
        let fresh33 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
            &mut aligned_base,
            *&mut expected,
            init,
        );
        *&mut expected = fresh33.0;
        fresh33.1;
        hint = ::core::intrinsics::atomic_xadd_acqrel(&mut aligned_base, size);
    }
    if hint.wrapping_rem(try_alignment) != 0 as libc::c_int as libc::c_ulong {
        return 0 as *mut libc::c_void;
    }
    return hint as *mut libc::c_void;
}
unsafe extern "C" fn mi_os_prim_free(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut still_committed: bool,
    mut tld_stats: *mut mi_stats_t,
) {
    let mut stats: *mut mi_stats_t = &mut _mi_stats_main;
    if size % _mi_os_page_size() == 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"(size % _mi_os_page_size()) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6672 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_os_prim_free\0"))
                .as_ptr(),
        );
    };
    if addr.is_null() || size == 0 as libc::c_int as size_t {
        return;
    }
    let mut err: libc::c_int = _mi_prim_free(addr, size);
    if err != 0 as libc::c_int {
        _mi_warning_message(
            b"unable to free OS memory (error: %d (0x%x), size: 0x%zx bytes, address: %p)\n\0"
                as *const u8 as *const libc::c_char,
            err,
            err,
            size,
            addr,
        );
    }
    if still_committed {
        _mi_stat_decrease(&mut (*stats).committed, size);
    }
    _mi_stat_decrease(&mut (*stats).reserved, size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_free_ex(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut still_committed: bool,
    mut memid: mi_memid_t,
    mut stats: *mut mi_stats_t,
) {
    if stats.is_null() {
        stats = &mut _mi_stats_main;
    }
    if mi_memkind_is_os(memid.memkind) {
        let mut csize: size_t = _mi_os_good_alloc_size(size);
        let mut base: *mut libc::c_void = addr;
        if !(memid.mem.os.base).is_null() {
            if memid.mem.os.base <= addr {
            } else {
                _mi_assert_fail(
                    b"memid.mem.os.base <= addr\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    6687 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(
                        b"_mi_os_free_ex\0",
                    ))
                    .as_ptr(),
                );
            };
            if (memid.mem.os.base as *mut uint8_t).offset(memid.mem.os.alignment as isize)
                >= addr as *mut uint8_t
            {
            } else {
                _mi_assert_fail(
                    b"(uint8_t*)memid.mem.os.base + memid.mem.os.alignment >= (uint8_t*)addr\0"
                        as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    6688 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(
                        b"_mi_os_free_ex\0",
                    ))
                    .as_ptr(),
                );
            };
            base = memid.mem.os.base;
            csize = csize.wrapping_add(
                (addr as *mut uint8_t).offset_from(memid.mem.os.base as *mut uint8_t)
                    as libc::c_long as size_t,
            );
        }
        if memid.memkind as libc::c_uint == MI_MEM_OS_HUGE as libc::c_int as libc::c_uint {
            if memid.is_pinned as libc::c_int != 0 {
            } else {
                _mi_assert_fail(
                    b"memid.is_pinned\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    6693 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(
                        b"_mi_os_free_ex\0",
                    ))
                    .as_ptr(),
                );
            };
            mi_os_free_huge_os_pages(base, csize, stats);
        } else {
            mi_os_prim_free(base, csize, still_committed, stats);
        }
    } else {
        if (memid.memkind as libc::c_uint) < MI_MEM_OS as libc::c_int as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"memid.memkind < MI_MEM_OS\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                6701 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_os_free_ex\0"))
                    .as_ptr(),
            );
        };
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_free(
    mut p: *mut libc::c_void,
    mut size: size_t,
    mut memid: mi_memid_t,
    mut stats: *mut mi_stats_t,
) {
    if stats.is_null() {
        stats = &mut _mi_stats_main;
    }
    _mi_os_free_ex(p, size, 1 as libc::c_int != 0, memid, stats);
}
unsafe extern "C" fn mi_os_prim_alloc_at(
    mut hint_addr: *mut libc::c_void,
    mut size: size_t,
    mut try_alignment: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut is_large: *mut bool,
    mut is_zero: *mut bool,
    mut tld_stats: *mut mi_stats_t,
) -> *mut libc::c_void {
    if size > 0 as libc::c_int as size_t && size % _mi_os_page_size() == 0 as libc::c_int as size_t
    {
    } else {
        _mi_assert_fail(
            b"size > 0 && (size % _mi_os_page_size()) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6709 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_os_prim_alloc_at\0"))
                .as_ptr(),
        );
    };
    if !is_zero.is_null() {
    } else {
        _mi_assert_fail(
            b"is_zero != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6710 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_os_prim_alloc_at\0"))
                .as_ptr(),
        );
    };
    if !is_large.is_null() {
    } else {
        _mi_assert_fail(
            b"is_large != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6711 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_os_prim_alloc_at\0"))
                .as_ptr(),
        );
    };
    if size == 0 as libc::c_int as size_t {
        return 0 as *mut libc::c_void;
    }
    if !commit {
        allow_large = 0 as libc::c_int != 0;
    }
    if try_alignment == 0 as libc::c_int as size_t {
        try_alignment = 1 as libc::c_int as size_t;
    }
    *is_zero = 0 as libc::c_int != 0;
    let mut p: *mut libc::c_void = 0 as *mut libc::c_void;
    let mut err: libc::c_int = _mi_prim_alloc(
        hint_addr,
        size,
        try_alignment,
        commit,
        allow_large,
        is_large,
        is_zero,
        &mut p,
    );
    if err != 0 as libc::c_int {
        _mi_warning_message(
            b"unable to allocate OS memory (error: %d (0x%x), addr: %p, size: 0x%zx bytes, align: 0x%zx, commit: %d, allow large: %d)\n\0"
                as *const u8 as *const libc::c_char,
            err,
            err,
            hint_addr,
            size,
            try_alignment,
            commit as libc::c_int,
            allow_large as libc::c_int,
        );
    }
    let mut stats: *mut mi_stats_t = &mut _mi_stats_main;
    _mi_stat_counter_increase(&mut (*stats).mmap_calls, 1 as libc::c_int as size_t);
    if !p.is_null() {
        _mi_stat_increase(&mut (*stats).reserved, size);
        if commit {
            _mi_stat_increase(&mut (*stats).committed, size);
        }
    }
    return p;
}
unsafe extern "C" fn mi_os_prim_alloc(
    mut size: size_t,
    mut try_alignment: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut is_large: *mut bool,
    mut is_zero: *mut bool,
    mut tld_stats: *mut mi_stats_t,
) -> *mut libc::c_void {
    return mi_os_prim_alloc_at(
        0 as *mut libc::c_void,
        size,
        try_alignment,
        commit,
        allow_large,
        is_large,
        is_zero,
        tld_stats,
    );
}
unsafe extern "C" fn mi_os_prim_alloc_aligned(
    mut size: size_t,
    mut alignment: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut is_large: *mut bool,
    mut is_zero: *mut bool,
    mut base: *mut *mut libc::c_void,
    mut stats: *mut mi_stats_t,
) -> *mut libc::c_void {
    if alignment >= _mi_os_page_size()
        && alignment & alignment.wrapping_sub(1 as libc::c_int as size_t)
            == 0 as libc::c_int as size_t
    {
    } else {
        _mi_assert_fail(
            b"alignment >= _mi_os_page_size() && ((alignment & (alignment - 1)) == 0)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6740 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_os_prim_alloc_aligned\0",
            ))
            .as_ptr(),
        );
    };
    if size > 0 as libc::c_int as size_t && size % _mi_os_page_size() == 0 as libc::c_int as size_t
    {
    } else {
        _mi_assert_fail(
            b"size > 0 && (size % _mi_os_page_size()) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6741 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_os_prim_alloc_aligned\0",
            ))
            .as_ptr(),
        );
    };
    if !is_large.is_null() {
    } else {
        _mi_assert_fail(
            b"is_large != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6742 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_os_prim_alloc_aligned\0",
            ))
            .as_ptr(),
        );
    };
    if !is_zero.is_null() {
    } else {
        _mi_assert_fail(
            b"is_zero != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6743 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_os_prim_alloc_aligned\0",
            ))
            .as_ptr(),
        );
    };
    if !base.is_null() {
    } else {
        _mi_assert_fail(
            b"base != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6744 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_os_prim_alloc_aligned\0",
            ))
            .as_ptr(),
        );
    };
    if !commit {
        allow_large = 0 as libc::c_int != 0;
    }
    if !(alignment >= _mi_os_page_size()
        && alignment & alignment.wrapping_sub(1 as libc::c_int as size_t)
            == 0 as libc::c_int as size_t)
    {
        return 0 as *mut libc::c_void;
    }
    size = _mi_align_up(size, _mi_os_page_size());
    let mut p: *mut libc::c_void = mi_os_prim_alloc(
        size,
        alignment,
        commit,
        allow_large,
        is_large,
        is_zero,
        stats,
    );
    if p.is_null() {
        return 0 as *mut libc::c_void;
    }
    if (p as uintptr_t).wrapping_rem(alignment) == 0 as libc::c_int as libc::c_ulong {
        *base = p;
    } else {
        _mi_warning_message(
            b"unable to allocate aligned OS memory directly, fall back to over-allocation (size: 0x%zx bytes, address: %p, alignment: 0x%zx, commit: %d)\n\0"
                as *const u8 as *const libc::c_char,
            size,
            p,
            alignment,
            commit as libc::c_int,
        );
        mi_os_prim_free(p, size, commit, stats);
        if size >= (18446744073709551615 as libc::c_ulong).wrapping_sub(alignment) {
            return 0 as *mut libc::c_void;
        }
        let over_size: size_t = size.wrapping_add(alignment);
        if !mi_os_mem_config.has_partial_free {
            p = mi_os_prim_alloc(
                over_size,
                1 as libc::c_int as size_t,
                0 as libc::c_int != 0,
                0 as libc::c_int != 0,
                is_large,
                is_zero,
                stats,
            );
            if p.is_null() {
                return 0 as *mut libc::c_void;
            }
            *base = p;
            p = mi_align_up_ptr(p, alignment);
            if commit {
                _mi_os_commit(p, size, 0 as *mut bool, stats);
            }
        } else {
            p = mi_os_prim_alloc(
                over_size,
                1 as libc::c_int as size_t,
                commit,
                0 as libc::c_int != 0,
                is_large,
                is_zero,
                stats,
            );
            if p.is_null() {
                return 0 as *mut libc::c_void;
            }
            let mut aligned_p: *mut libc::c_void = mi_align_up_ptr(p, alignment);
            let mut pre_size: size_t = (aligned_p as *mut uint8_t).offset_from(p as *mut uint8_t)
                as libc::c_long as size_t;
            let mut mid_size: size_t = _mi_align_up(size, _mi_os_page_size());
            let mut post_size: size_t = over_size.wrapping_sub(pre_size).wrapping_sub(mid_size);
            if pre_size < over_size && post_size < over_size && mid_size >= size {
            } else {
                _mi_assert_fail(
                    b"pre_size < over_size&& post_size < over_size&& mid_size >= size\0"
                        as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    6776 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                        b"mi_os_prim_alloc_aligned\0",
                    ))
                    .as_ptr(),
                );
            };
            if pre_size > 0 as libc::c_int as size_t {
                mi_os_prim_free(p, pre_size, commit, stats);
            }
            if post_size > 0 as libc::c_int as size_t {
                mi_os_prim_free(
                    (aligned_p as *mut uint8_t).offset(mid_size as isize) as *mut libc::c_void,
                    post_size,
                    commit,
                    stats,
                );
            }
            p = aligned_p;
            *base = aligned_p;
        }
    }
    if p.is_null()
        || !p.is_null()
            && !(*base).is_null()
            && (p as uintptr_t).wrapping_rem(alignment) == 0 as libc::c_int as libc::c_ulong
    {
    } else {
        _mi_assert_fail(
            b"p == NULL || (p != NULL && *base != NULL && ((uintptr_t)p % alignment) == 0)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6783 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_os_prim_alloc_aligned\0",
            ))
            .as_ptr(),
        );
    };
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_alloc(
    mut size: size_t,
    mut memid: *mut mi_memid_t,
    mut stats: *mut mi_stats_t,
) -> *mut libc::c_void {
    *memid = _mi_memid_none();
    if size == 0 as libc::c_int as size_t {
        return 0 as *mut libc::c_void;
    }
    if stats.is_null() {
        stats = &mut _mi_stats_main;
    }
    size = _mi_os_good_alloc_size(size);
    let mut os_is_large: bool = 0 as libc::c_int != 0;
    let mut os_is_zero: bool = 0 as libc::c_int != 0;
    let mut p: *mut libc::c_void = mi_os_prim_alloc(
        size,
        0 as libc::c_int as size_t,
        1 as libc::c_int != 0,
        0 as libc::c_int != 0,
        &mut os_is_large,
        &mut os_is_zero,
        stats,
    );
    if !p.is_null() {
        *memid = _mi_memid_create_os(1 as libc::c_int != 0, os_is_zero, os_is_large);
    }
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_alloc_aligned(
    mut size: size_t,
    mut alignment: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut memid: *mut mi_memid_t,
    mut stats: *mut mi_stats_t,
) -> *mut libc::c_void {
    Some(_mi_os_get_aligned_hint as unsafe extern "C" fn(size_t, size_t) -> *mut libc::c_void);
    *memid = _mi_memid_none();
    if size == 0 as libc::c_int as size_t {
        return 0 as *mut libc::c_void;
    }
    if stats.is_null() {
        stats = &mut _mi_stats_main;
    }
    size = _mi_os_good_alloc_size(size);
    alignment = _mi_align_up(alignment, _mi_os_page_size());
    let mut os_is_large: bool = 0 as libc::c_int != 0;
    let mut os_is_zero: bool = 0 as libc::c_int != 0;
    let mut os_base: *mut libc::c_void = 0 as *mut libc::c_void;
    let mut p: *mut libc::c_void = mi_os_prim_alloc_aligned(
        size,
        alignment,
        commit,
        allow_large,
        &mut os_is_large,
        &mut os_is_zero,
        &mut os_base,
        stats,
    );
    if !p.is_null() {
        *memid = _mi_memid_create_os(commit, os_is_zero, os_is_large);
        (*memid).mem.os.base = os_base;
        (*memid).mem.os.alignment = alignment;
    }
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_alloc_aligned_at_offset(
    mut size: size_t,
    mut alignment: size_t,
    mut offset: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut memid: *mut mi_memid_t,
    mut stats: *mut mi_stats_t,
) -> *mut libc::c_void {
    if offset as libc::c_ulonglong
        <= (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
    {
    } else {
        _mi_assert_fail(
            b"offset <= MI_SEGMENT_SIZE\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6819 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                b"_mi_os_alloc_aligned_at_offset\0",
            ))
            .as_ptr(),
        );
    };
    if offset <= size {
    } else {
        _mi_assert_fail(
            b"offset <= size\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6820 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                b"_mi_os_alloc_aligned_at_offset\0",
            ))
            .as_ptr(),
        );
    };
    if alignment % _mi_os_page_size() == 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"(alignment % _mi_os_page_size()) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6821 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                b"_mi_os_alloc_aligned_at_offset\0",
            ))
            .as_ptr(),
        );
    };
    *memid = _mi_memid_none();
    if stats.is_null() {
        stats = &mut _mi_stats_main;
    }
    if offset as libc::c_ulonglong
        > (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
    {
        return 0 as *mut libc::c_void;
    }
    if offset == 0 as libc::c_int as size_t {
        return _mi_os_alloc_aligned(size, alignment, commit, allow_large, memid, stats);
    } else {
        let extra: size_t = (_mi_align_up(offset, alignment)).wrapping_sub(offset);
        let oversize: size_t = size.wrapping_add(extra);
        let start: *mut libc::c_void =
            _mi_os_alloc_aligned(oversize, alignment, commit, allow_large, memid, stats);
        if start.is_null() {
            return 0 as *mut libc::c_void;
        }
        let p: *mut libc::c_void =
            (start as *mut uint8_t).offset(extra as isize) as *mut libc::c_void;
        if _mi_is_aligned(
            (p as *mut uint8_t).offset(offset as isize) as *mut libc::c_void,
            alignment,
        ) as libc::c_int
            != 0
        {
        } else {
            _mi_assert_fail(
                b"_mi_is_aligned((uint8_t*)p + offset, alignment)\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                6834 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                    b"_mi_os_alloc_aligned_at_offset\0",
                ))
                .as_ptr(),
            );
        };
        if commit as libc::c_int != 0 && extra > _mi_os_page_size() {
            _mi_os_decommit(start, extra, stats);
        }
        return p;
    };
}
unsafe extern "C" fn mi_os_page_align_areax(
    mut conservative: bool,
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut newsize: *mut size_t,
) -> *mut libc::c_void {
    if !addr.is_null() && size > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"addr != NULL && size > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6842 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"mi_os_page_align_areax\0",
            ))
            .as_ptr(),
        );
    };
    if !newsize.is_null() {
        *newsize = 0 as libc::c_int as size_t;
    }
    if size == 0 as libc::c_int as size_t || addr.is_null() {
        return 0 as *mut libc::c_void;
    }
    let mut start: *mut libc::c_void = if conservative as libc::c_int != 0 {
        mi_align_up_ptr(addr, _mi_os_page_size())
    } else {
        mi_align_down_ptr(addr, _mi_os_page_size())
    };
    let mut end: *mut libc::c_void = if conservative as libc::c_int != 0 {
        mi_align_down_ptr(
            (addr as *mut uint8_t).offset(size as isize) as *mut libc::c_void,
            _mi_os_page_size(),
        )
    } else {
        mi_align_up_ptr(
            (addr as *mut uint8_t).offset(size as isize) as *mut libc::c_void,
            _mi_os_page_size(),
        )
    };
    let mut diff: ptrdiff_t =
        (end as *mut uint8_t).offset_from(start as *mut uint8_t) as libc::c_long;
    if diff <= 0 as libc::c_int as ptrdiff_t {
        return 0 as *mut libc::c_void;
    }
    if conservative as libc::c_int != 0 && diff as size_t <= size
        || !conservative && diff as size_t >= size
    {
    } else {
        _mi_assert_fail(
            b"(conservative && (size_t)diff <= size) || (!conservative && (size_t)diff >= size)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6851 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"mi_os_page_align_areax\0",
            ))
            .as_ptr(),
        );
    };
    if !newsize.is_null() {
        *newsize = diff as size_t;
    }
    return start;
}
unsafe extern "C" fn mi_os_page_align_area_conservative(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut newsize: *mut size_t,
) -> *mut libc::c_void {
    return mi_os_page_align_areax(1 as libc::c_int != 0, addr, size, newsize);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_commit(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut is_zero: *mut bool,
    mut tld_stats: *mut mi_stats_t,
) -> bool {
    let mut stats: *mut mi_stats_t = &mut _mi_stats_main;
    if !is_zero.is_null() {
        *is_zero = 0 as libc::c_int != 0;
    }
    _mi_stat_increase(&mut (*stats).committed, size);
    _mi_stat_counter_increase(&mut (*stats).commit_calls, 1 as libc::c_int as size_t);
    let mut csize: size_t = 0;
    let mut start: *mut libc::c_void =
        mi_os_page_align_areax(0 as libc::c_int != 0, addr, size, &mut csize);
    if csize == 0 as libc::c_int as size_t {
        return 1 as libc::c_int != 0;
    }
    let mut os_is_zero: bool = 0 as libc::c_int != 0;
    let mut err: libc::c_int = _mi_prim_commit(start, csize, &mut os_is_zero);
    if err != 0 as libc::c_int {
        _mi_warning_message(
            b"cannot commit OS memory (error: %d (0x%x), address: %p, size: 0x%zx bytes)\n\0"
                as *const u8 as *const libc::c_char,
            err,
            err,
            start,
            csize,
        );
        return 0 as libc::c_int != 0;
    }
    if os_is_zero as libc::c_int != 0 && !is_zero.is_null() {
        *is_zero = 1 as libc::c_int != 0;
    }
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn mi_os_decommit_ex(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut needs_recommit: *mut bool,
    mut tld_stats: *mut mi_stats_t,
) -> bool {
    let mut stats: *mut mi_stats_t = &mut _mi_stats_main;
    if !needs_recommit.is_null() {
    } else {
        _mi_assert_fail(
            b"needs_recommit!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6886 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 18], &[libc::c_char; 18]>(b"mi_os_decommit_ex\0"))
                .as_ptr(),
        );
    };
    _mi_stat_decrease(&mut (*stats).committed, size);
    let mut csize: size_t = 0;
    let mut start: *mut libc::c_void = mi_os_page_align_area_conservative(addr, size, &mut csize);
    if csize == 0 as libc::c_int as size_t {
        return 1 as libc::c_int != 0;
    }
    *needs_recommit = 1 as libc::c_int != 0;
    let mut err: libc::c_int = _mi_prim_decommit(start, csize, needs_recommit);
    if err != 0 as libc::c_int {
        _mi_warning_message(
            b"cannot decommit OS memory (error: %d (0x%x), address: %p, size: 0x%zx bytes)\n\0"
                as *const u8 as *const libc::c_char,
            err,
            err,
            start,
            csize,
        );
    }
    if err == 0 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"err == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            6896 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 18], &[libc::c_char; 18]>(b"mi_os_decommit_ex\0"))
                .as_ptr(),
        );
    };
    return err == 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_decommit(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut tld_stats: *mut mi_stats_t,
) -> bool {
    let mut needs_recommit: bool = false;
    return mi_os_decommit_ex(addr, size, &mut needs_recommit, tld_stats);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_reset(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut stats: *mut mi_stats_t,
) -> bool {
    let mut csize: size_t = 0;
    let mut start: *mut libc::c_void = mi_os_page_align_area_conservative(addr, size, &mut csize);
    if csize == 0 as libc::c_int as size_t {
        return 1 as libc::c_int != 0;
    }
    _mi_stat_increase(&mut (*stats).reset, csize);
    _mi_stat_counter_increase(&mut (*stats).reset_calls, 1 as libc::c_int as size_t);
    memset(start, 0 as libc::c_int, csize);
    let mut err: libc::c_int = _mi_prim_reset(start, csize);
    if err != 0 as libc::c_int {
        _mi_warning_message(
            b"cannot reset OS memory (error: %d (0x%x), address: %p, size: 0x%zx bytes)\n\0"
                as *const u8 as *const libc::c_char,
            err,
            err,
            start,
            csize,
        );
    }
    return err == 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_purge_ex(
    mut p: *mut libc::c_void,
    mut size: size_t,
    mut allow_reset: bool,
    mut stats: *mut mi_stats_t,
) -> bool {
    if mi_option_get(mi_option_purge_delay) < 0 as libc::c_int as libc::c_long {
        return 0 as libc::c_int != 0;
    }
    _mi_stat_counter_increase(&mut (*stats).purge_calls, 1 as libc::c_int as size_t);
    _mi_stat_increase(&mut (*stats).purged, size);
    if mi_option_is_enabled(mi_option_purge_decommits) as libc::c_int != 0 && !_mi_preloading() {
        let mut needs_recommit: bool = 1 as libc::c_int != 0;
        mi_os_decommit_ex(p, size, &mut needs_recommit, stats);
        return needs_recommit;
    } else {
        if allow_reset {
            _mi_os_reset(p, size, stats);
        }
        return 0 as libc::c_int != 0;
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_purge(
    mut p: *mut libc::c_void,
    mut size: size_t,
    mut stats: *mut mi_stats_t,
) -> bool {
    return _mi_os_purge_ex(p, size, 1 as libc::c_int != 0, stats);
}
unsafe extern "C" fn mi_os_protectx(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut protect: bool,
) -> bool {
    let mut csize: size_t = 0 as libc::c_int as size_t;
    let mut start: *mut libc::c_void = mi_os_page_align_area_conservative(addr, size, &mut csize);
    if csize == 0 as libc::c_int as size_t {
        return 0 as libc::c_int != 0;
    }
    let mut err: libc::c_int = _mi_prim_protect(start, csize, protect);
    if err != 0 as libc::c_int {
        _mi_warning_message(
            b"cannot %s OS memory (error: %d (0x%x), address: %p, size: 0x%zx bytes)\n\0"
                as *const u8 as *const libc::c_char,
            if protect as libc::c_int != 0 {
                b"protect\0" as *const u8 as *const libc::c_char
            } else {
                b"unprotect\0" as *const u8 as *const libc::c_char
            },
            err,
            err,
            start,
            csize,
        );
    }
    return err == 0 as libc::c_int;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_protect(mut addr: *mut libc::c_void, mut size: size_t) -> bool {
    return mi_os_protectx(addr, size, 1 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_unprotect(mut addr: *mut libc::c_void, mut size: size_t) -> bool {
    return mi_os_protectx(addr, size, 0 as libc::c_int != 0);
}
static mut mi_huge_start: uintptr_t = 0;
unsafe extern "C" fn mi_os_claim_huge_pages(
    mut pages: size_t,
    mut total_size: *mut size_t,
) -> *mut uint8_t {
    if !total_size.is_null() {
        *total_size = 0 as libc::c_int as size_t;
    }
    let size: size_t = (pages as libc::c_ulonglong).wrapping_mul(
        (1024 as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong),
    ) as size_t;
    let mut start: uintptr_t = 0 as libc::c_int as uintptr_t;
    let mut end: uintptr_t = 0 as libc::c_int as uintptr_t;
    let mut huge_start: uintptr_t = ::core::intrinsics::atomic_load_relaxed(&mut mi_huge_start);
    loop {
        start = huge_start;
        if start == 0 as libc::c_int as uintptr_t {
            start = (32 as libc::c_int as uintptr_t) << 40 as libc::c_int;
        }
        end = start.wrapping_add(size);
        if (end as libc::c_ulonglong).wrapping_rem(
            (1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
        ) == 0 as libc::c_int as libc::c_ulonglong
        {
        } else {
            _mi_assert_fail(
                b"end % MI_SEGMENT_SIZE == 0\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                6975 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                    b"mi_os_claim_huge_pages\0",
                ))
                .as_ptr(),
            );
        };
        let fresh34 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
            &mut mi_huge_start as *mut uintptr_t,
            *(&mut huge_start as *mut uintptr_t),
            end,
        );
        *(&mut huge_start as *mut uintptr_t) = fresh34.0;
        if fresh34.1 {
            break;
        }
    }
    if !total_size.is_null() {
        *total_size = size;
    }
    return start as *mut uint8_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_alloc_huge_os_pages(
    mut pages: size_t,
    mut numa_node: libc::c_int,
    mut max_msecs: mi_msecs_t,
    mut pages_reserved: *mut size_t,
    mut psize: *mut size_t,
    mut memid: *mut mi_memid_t,
) -> *mut libc::c_void {
    *memid = _mi_memid_none();
    if !psize.is_null() {
        *psize = 0 as libc::c_int as size_t;
    }
    if !pages_reserved.is_null() {
        *pages_reserved = 0 as libc::c_int as size_t;
    }
    let mut size: size_t = 0 as libc::c_int as size_t;
    let mut start: *mut uint8_t = mi_os_claim_huge_pages(pages, &mut size);
    if start.is_null() {
        return 0 as *mut libc::c_void;
    }
    let mut start_t: mi_msecs_t = _mi_clock_start();
    let mut page: size_t = 0 as libc::c_int as size_t;
    let mut all_zero: bool = 1 as libc::c_int != 0;
    while page < pages {
        let mut is_zero: bool = 0 as libc::c_int != 0;
        let mut addr: *mut libc::c_void = start.offset(
            (page as libc::c_ulonglong).wrapping_mul(
                (1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong),
            ) as isize,
        ) as *mut libc::c_void;
        let mut p: *mut libc::c_void = 0 as *mut libc::c_void;
        let mut err: libc::c_int = _mi_prim_alloc_huge_os_pages(
            addr,
            (1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong) as size_t,
            numa_node,
            &mut is_zero,
            &mut p,
        );
        if !is_zero {
            all_zero = 0 as libc::c_int != 0;
        }
        if err != 0 as libc::c_int {
            _mi_warning_message(
                b"unable to allocate huge OS page (error: %d (0x%x), address: %p, size: %zx bytes)\n\0"
                    as *const u8 as *const libc::c_char,
                err,
                err,
                addr,
                (1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong),
            );
            break;
        } else if p != addr {
            if !p.is_null() {
                _mi_warning_message(
                    b"could not allocate contiguous huge OS page %zu at %p\n\0" as *const u8
                        as *const libc::c_char,
                    page,
                    addr,
                );
                mi_os_prim_free(
                    p,
                    (1024 as libc::c_ulonglong)
                        .wrapping_mul(1024 as libc::c_ulonglong)
                        .wrapping_mul(1024 as libc::c_ulonglong) as size_t,
                    1 as libc::c_int != 0,
                    &mut _mi_stats_main,
                );
            }
            break;
        } else {
            page = page.wrapping_add(1);
            page;
            _mi_stat_increase(
                &mut _mi_stats_main.committed,
                (1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong) as size_t,
            );
            _mi_stat_increase(
                &mut _mi_stats_main.reserved,
                (1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong) as size_t,
            );
            if !(max_msecs > 0 as libc::c_int as mi_msecs_t) {
                continue;
            }
            let mut elapsed: mi_msecs_t = _mi_clock_end(start_t);
            if page >= 1 as libc::c_int as size_t {
                let mut estimate: mi_msecs_t = (elapsed as size_t
                    / page.wrapping_add(1 as libc::c_int as size_t)
                    * pages) as mi_msecs_t;
                if estimate > 2 as libc::c_int as mi_msecs_t * max_msecs {
                    elapsed = max_msecs + 1 as libc::c_int as mi_msecs_t;
                }
            }
            if !(elapsed > max_msecs) {
                continue;
            }
            _mi_warning_message(
                b"huge OS page allocation timed out (after allocating %zu page(s))\n\0" as *const u8
                    as *const libc::c_char,
                page,
            );
            break;
        }
    }
    if (page as libc::c_ulonglong).wrapping_mul(
        (1024 as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong),
    ) <= size as libc::c_ulonglong
    {
    } else {
        _mi_assert_fail(
            b"page*MI_HUGE_OS_PAGE_SIZE <= size\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7031 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"_mi_os_alloc_huge_os_pages\0",
            ))
            .as_ptr(),
        );
    };
    if !pages_reserved.is_null() {
        *pages_reserved = page;
    }
    if !psize.is_null() {
        *psize = (page as libc::c_ulonglong).wrapping_mul(
            (1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong),
        ) as size_t;
    }
    if page != 0 as libc::c_int as size_t {
        if !start.is_null() {
        } else {
            _mi_assert_fail(
                b"start != NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7035 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"_mi_os_alloc_huge_os_pages\0",
                ))
                .as_ptr(),
            );
        };
        *memid = _mi_memid_create_os(1 as libc::c_int != 0, all_zero, 1 as libc::c_int != 0);
        (*memid).memkind = MI_MEM_OS_HUGE;
        if (*memid).is_pinned as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"memid->is_pinned\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7038 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"_mi_os_alloc_huge_os_pages\0",
                ))
                .as_ptr(),
            );
        };
    }
    return (if page == 0 as libc::c_int as size_t {
        0 as *mut uint8_t
    } else {
        start
    }) as *mut libc::c_void;
}
unsafe extern "C" fn mi_os_free_huge_os_pages(
    mut p: *mut libc::c_void,
    mut size: size_t,
    mut stats: *mut mi_stats_t,
) {
    if p.is_null() || size == 0 as libc::c_int as size_t {
        return;
    }
    let mut base: *mut uint8_t = p as *mut uint8_t;
    while size as libc::c_ulonglong
        >= (1024 as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong)
    {
        mi_os_prim_free(
            base as *mut libc::c_void,
            (1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong) as size_t,
            1 as libc::c_int != 0,
            stats,
        );
        size = (size as libc::c_ulonglong).wrapping_sub(
            (1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong),
        ) as size_t as size_t;
        base = base.offset(
            (1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong)
                .wrapping_mul(1024 as libc::c_ulonglong) as isize,
        );
    }
}
#[unsafe(no_mangle)]
pub static mut _mi_numa_node_count: size_t = 0;
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_numa_node_count_get() -> size_t {
    let mut count: size_t = ::core::intrinsics::atomic_load_acquire(&mut _mi_numa_node_count);
    if count <= 0 as libc::c_int as size_t {
        let mut ncount: libc::c_long = mi_option_get(mi_option_use_numa_nodes);
        if ncount > 0 as libc::c_int as libc::c_long {
            count = ncount as size_t;
        } else {
            count = _mi_prim_numa_node_count();
            if count == 0 as libc::c_int as size_t {
                count = 1 as libc::c_int as size_t;
            }
        }
        ::core::intrinsics::atomic_store_release(&mut _mi_numa_node_count, count);
        _mi_verbose_message(
            b"using %zd numa regions\n\0" as *const u8 as *const libc::c_char,
            count,
        );
    }
    return count;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_numa_node_get(mut tld: *mut mi_os_tld_t) -> libc::c_int {
    let mut numa_count: size_t = _mi_os_numa_node_count();
    if numa_count <= 1 as libc::c_int as size_t {
        return 0 as libc::c_int;
    }
    let mut numa_node: size_t = _mi_prim_numa_node();
    if numa_node >= numa_count {
        numa_node = numa_node % numa_count;
    }
    return numa_node as libc::c_int;
}
#[inline]
unsafe extern "C" fn mi_page_queue_is_huge(mut pq: *const mi_page_queue_t) -> bool {
    return (*pq).block_size as libc::c_ulonglong
        == ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
            .wrapping_add(
                ::core::mem::size_of::<uintptr_t>() as libc::c_ulong as libc::c_ulonglong
            );
}
#[inline]
unsafe extern "C" fn mi_page_queue_is_full(mut pq: *const mi_page_queue_t) -> bool {
    return (*pq).block_size as libc::c_ulonglong
        == ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
            .wrapping_add(
                (2 as libc::c_int as libc::c_ulong)
                    .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong)
                    as libc::c_ulonglong,
            );
}
#[inline]
unsafe extern "C" fn mi_page_queue_is_special(mut pq: *const mi_page_queue_t) -> bool {
    return (*pq).block_size as libc::c_ulonglong
        > ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong);
}
#[inline]
unsafe extern "C" fn mi_bin(mut size: size_t) -> uint8_t {
    let mut wsize: size_t = _mi_wsize_from_size(size);
    let mut bin: uint8_t = 0;
    if wsize <= 1 as libc::c_int as size_t {
        bin = 1 as libc::c_int as uint8_t;
    } else if wsize <= 8 as libc::c_int as size_t {
        bin = (wsize.wrapping_add(1 as libc::c_int as size_t) & !(1 as libc::c_int) as size_t)
            as uint8_t;
    } else if wsize as libc::c_ulonglong
        > ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
            .wrapping_div(((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulonglong)
    {
        bin = 73 as libc::c_uint as uint8_t;
    } else {
        wsize = wsize.wrapping_sub(1);
        wsize;
        let mut b: uint8_t = mi_bsr(wsize) as uint8_t;
        bin = (((b as libc::c_int) << 2 as libc::c_int)
            + (wsize >> b as libc::c_int - 2 as libc::c_int & 0x3 as libc::c_int as size_t)
                as uint8_t as libc::c_int
            - 3 as libc::c_int) as uint8_t;
        if (bin as libc::c_uint) < 73 as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"bin < MI_BIN_HUGE\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7129 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 7], &[libc::c_char; 7]>(b"mi_bin\0")).as_ptr(),
            );
        };
    }
    if bin as libc::c_int > 0 as libc::c_int && bin as libc::c_uint <= 73 as libc::c_uint {
    } else {
        _mi_assert_fail(
            b"bin > 0 && bin <= MI_BIN_HUGE\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7131 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 7], &[libc::c_char; 7]>(b"mi_bin\0")).as_ptr(),
        );
    };
    return bin;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bin(mut size: size_t) -> uint8_t {
    return mi_bin(size);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_bin_size(mut bin: uint8_t) -> size_t {
    return _mi_heap_empty.pages[bin as usize].block_size;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_good_size(mut size: size_t) -> size_t {
    if size as libc::c_ulonglong
        <= ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
    {
        return _mi_bin_size(mi_bin(
            size.wrapping_add(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong),
        ));
    } else {
        return _mi_align_up(
            size.wrapping_add(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong),
            _mi_os_page_size(),
        );
    };
}
unsafe extern "C" fn mi_page_queue_contains(
    mut queue: *mut mi_page_queue_t,
    mut page: *const mi_page_t,
) -> bool {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7150 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"mi_page_queue_contains\0",
            ))
            .as_ptr(),
        );
    };
    let mut list: *mut mi_page_t = (*queue).first;
    while !list.is_null() {
        if ((*list).next).is_null() || (*(*list).next).prev == list {
        } else {
            _mi_assert_fail(
                b"list->next == NULL || list->next->prev == list\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7153 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                    b"mi_page_queue_contains\0",
                ))
                .as_ptr(),
            );
        };
        if ((*list).prev).is_null() || (*(*list).prev).next == list {
        } else {
            _mi_assert_fail(
                b"list->prev == NULL || list->prev->next == list\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7154 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                    b"mi_page_queue_contains\0",
                ))
                .as_ptr(),
            );
        };
        if list == page as *mut mi_page_t {
            break;
        }
        list = (*list).next;
    }
    return list == page as *mut mi_page_t;
}
unsafe extern "C" fn mi_heap_contains_queue(
    mut heap: *const mi_heap_t,
    mut pq: *const mi_page_queue_t,
) -> bool {
    return pq
        >= &*((*heap).pages).as_ptr().offset(0 as libc::c_int as isize) as *const mi_page_queue_t
        && pq
            <= &*((*heap).pages).as_ptr().offset(
                (73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) as isize,
            ) as *const mi_page_queue_t;
}
unsafe extern "C" fn mi_heap_page_queue_of(
    mut heap: *mut mi_heap_t,
    mut page: *const mi_page_t,
) -> *mut mi_page_queue_t {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7167 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_heap_page_queue_of\0"))
                .as_ptr(),
        );
    };
    let mut bin: uint8_t = (if mi_page_is_in_full(page) as libc::c_int != 0 {
        (73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint)
    } else if mi_page_is_huge(page) as libc::c_int != 0 {
        73 as libc::c_uint
    } else {
        mi_bin(mi_page_block_size(page)) as libc::c_uint
    }) as uint8_t;
    if bin as libc::c_uint <= (73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) {
    } else {
        _mi_assert_fail(
            b"bin <= MI_BIN_FULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7169 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_heap_page_queue_of\0"))
                .as_ptr(),
        );
    };
    let mut pq: *mut mi_page_queue_t =
        &mut *((*heap).pages).as_mut_ptr().offset(bin as isize) as *mut mi_page_queue_t;
    if mi_page_block_size(page) == (*pq).block_size
        || mi_page_is_huge(page) as libc::c_int != 0
            && mi_page_queue_is_huge(pq) as libc::c_int != 0
        || mi_page_is_in_full(page) as libc::c_int != 0
            && mi_page_queue_is_full(pq) as libc::c_int != 0
    {
    } else {
        _mi_assert_fail(
            b"(mi_page_block_size(page) == pq->block_size) || (mi_page_is_huge(page) && mi_page_queue_is_huge(pq)) || (mi_page_is_in_full(page) && mi_page_queue_is_full(pq))\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7173 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 22],
                &[libc::c_char; 22],
            >(b"mi_heap_page_queue_of\0"))
                .as_ptr(),
        );
    };
    return pq;
}
unsafe extern "C" fn mi_page_queue_of(mut page: *const mi_page_t) -> *mut mi_page_queue_t {
    let mut heap: *mut mi_heap_t = mi_page_heap(page);
    let mut pq: *mut mi_page_queue_t = mi_heap_page_queue_of(heap, page);
    return pq;
}
#[inline]
unsafe extern "C" fn mi_heap_queue_first_update(
    mut heap: *mut mi_heap_t,
    mut pq: *const mi_page_queue_t,
) {
    if mi_heap_contains_queue(heap, pq) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_contains_queue(heap,pq)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7183 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"mi_heap_queue_first_update\0",
            ))
            .as_ptr(),
        );
    };
    let mut size: size_t = (*pq).block_size;
    if size
        > (128 as libc::c_int as libc::c_ulong)
            .wrapping_mul(::core::mem::size_of::<*mut libc::c_void>() as libc::c_ulong)
    {
        return;
    }
    let mut page: *mut mi_page_t = (*pq).first;
    if ((*pq).first).is_null() {
        page = &raw mut _mi_page_empty;
    }
    let mut start: size_t = 0;
    let mut idx: size_t = _mi_wsize_from_size(size);
    let mut pages_free: *mut *mut mi_page_t = ((*heap).pages_free_direct).as_mut_ptr();
    if *pages_free.offset(idx as isize) == page {
        return;
    }
    if idx <= 1 as libc::c_int as size_t {
        start = 0 as libc::c_int as size_t;
    } else {
        let mut bin: uint8_t = mi_bin(size);
        let mut prev: *const mi_page_queue_t = pq.offset(-(1 as libc::c_int as isize));
        while bin as libc::c_int == mi_bin((*prev).block_size) as libc::c_int
            && prev
                > &mut *((*heap).pages)
                    .as_mut_ptr()
                    .offset(0 as libc::c_int as isize) as *mut mi_page_queue_t
                    as *const mi_page_queue_t
        {
            prev = prev.offset(-1);
            prev;
        }
        start = (1 as libc::c_int as size_t).wrapping_add(_mi_wsize_from_size((*prev).block_size));
        if start > idx {
            start = idx;
        }
    }
    if start <= idx {
    } else {
        _mi_assert_fail(
            b"start <= idx\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7204 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"mi_heap_queue_first_update\0",
            ))
            .as_ptr(),
        );
    };
    let mut sz: size_t = start;
    while sz <= idx {
        let ref mut fresh35 = *pages_free.offset(sz as isize);
        *fresh35 = page;
        sz = sz.wrapping_add(1);
        sz;
    }
}
unsafe extern "C" fn mi_page_queue_remove(
    mut queue: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7210 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_page_queue_remove\0"))
                .as_ptr(),
        );
    };
    if mi_page_block_size(page) == (*queue).block_size
        || mi_page_is_huge(page) as libc::c_int != 0
            && mi_page_queue_is_huge(queue) as libc::c_int != 0
        || mi_page_is_in_full(page) as libc::c_int != 0
            && mi_page_queue_is_full(queue) as libc::c_int != 0
    {
    } else {
        _mi_assert_fail(
            b"mi_page_block_size(page) == queue->block_size || (mi_page_is_huge(page) && mi_page_queue_is_huge(queue)) || (mi_page_is_in_full(page) && mi_page_queue_is_full(queue))\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7214 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 21],
                &[libc::c_char; 21],
            >(b"mi_page_queue_remove\0"))
                .as_ptr(),
        );
    };
    let mut heap: *mut mi_heap_t = mi_page_heap(page);
    if !((*page).prev).is_null() {
        (*(*page).prev).next = (*page).next;
    }
    if !((*page).next).is_null() {
        (*(*page).next).prev = (*page).prev;
    }
    if page == (*queue).last {
        (*queue).last = (*page).prev;
    }
    if page == (*queue).first {
        (*queue).first = (*page).next;
        if mi_heap_contains_queue(heap, queue) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"mi_heap_contains_queue(heap, queue)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7221 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"mi_page_queue_remove\0",
                ))
                .as_ptr(),
            );
        };
        mi_heap_queue_first_update(heap, queue);
    }
    (*heap).page_count = ((*heap).page_count).wrapping_sub(1);
    (*heap).page_count;
    (*page).next = 0 as *mut mi_page_s;
    (*page).prev = 0 as *mut mi_page_s;
    mi_page_set_in_full(page, 0 as libc::c_int != 0);
}
unsafe extern "C" fn mi_page_queue_push(
    mut heap: *mut mi_heap_t,
    mut queue: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
) {
    if mi_page_heap(page) == heap {
    } else {
        _mi_assert_fail(
            b"mi_page_heap(page) == heap\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7230 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_page_queue_push\0"))
                .as_ptr(),
        );
    };
    if !mi_page_queue_contains(queue, page) {
    } else {
        _mi_assert_fail(
            b"!mi_page_queue_contains(queue, page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7231 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_page_queue_push\0"))
                .as_ptr(),
        );
    };
    if mi_page_block_size(page) == (*queue).block_size
        || mi_page_is_huge(page) as libc::c_int != 0
            && mi_page_queue_is_huge(queue) as libc::c_int != 0
        || mi_page_is_in_full(page) as libc::c_int != 0
            && mi_page_queue_is_full(queue) as libc::c_int != 0
    {
    } else {
        _mi_assert_fail(
            b"mi_page_block_size(page) == queue->block_size || (mi_page_is_huge(page) && mi_page_queue_is_huge(queue)) || (mi_page_is_in_full(page) && mi_page_queue_is_full(queue))\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7237 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 19],
                &[libc::c_char; 19],
            >(b"mi_page_queue_push\0"))
                .as_ptr(),
        );
    };
    mi_page_set_in_full(page, mi_page_queue_is_full(queue));
    (*page).next = (*queue).first;
    (*page).prev = 0 as *mut mi_page_s;
    if !((*queue).first).is_null() {
        if ((*(*queue).first).prev).is_null() {
        } else {
            _mi_assert_fail(
                b"queue->first->prev == NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7242 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_page_queue_push\0",
                ))
                .as_ptr(),
            );
        };
        (*(*queue).first).prev = page;
        (*queue).first = page;
    } else {
        (*queue).last = page;
        (*queue).first = (*queue).last;
    }
    mi_heap_queue_first_update(heap, queue);
    (*heap).page_count = ((*heap).page_count).wrapping_add(1);
    (*heap).page_count;
}
unsafe extern "C" fn mi_page_queue_move_to_front(
    mut heap: *mut mi_heap_t,
    mut queue: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
) {
    if mi_page_heap(page) == heap {
    } else {
        _mi_assert_fail(
            b"mi_page_heap(page) == heap\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7253 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"mi_page_queue_move_to_front\0",
            ))
            .as_ptr(),
        );
    };
    if mi_page_queue_contains(queue, page) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_queue_contains(queue, page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7254 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"mi_page_queue_move_to_front\0",
            ))
            .as_ptr(),
        );
    };
    if (*queue).first == page {
        return;
    }
    mi_page_queue_remove(queue, page);
    mi_page_queue_push(heap, queue, page);
    if (*queue).first == page {
    } else {
        _mi_assert_fail(
            b"queue->first == page\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7258 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"mi_page_queue_move_to_front\0",
            ))
            .as_ptr(),
        );
    };
}
unsafe extern "C" fn mi_page_queue_enqueue_from_ex(
    mut to: *mut mi_page_queue_t,
    mut from: *mut mi_page_queue_t,
    mut enqueue_at_end: bool,
    mut page: *mut mi_page_t,
) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7261 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 30], &[libc::c_char; 30]>(
                b"mi_page_queue_enqueue_from_ex\0",
            ))
            .as_ptr(),
        );
    };
    let bsize: size_t = mi_page_block_size(page);
    if bsize == (*to).block_size && bsize == (*from).block_size
        || bsize == (*to).block_size && mi_page_queue_is_full(from) as libc::c_int != 0
        || bsize == (*from).block_size && mi_page_queue_is_full(to) as libc::c_int != 0
        || mi_page_is_huge(page) as libc::c_int != 0
            && mi_page_queue_is_huge(to) as libc::c_int != 0
        || mi_page_is_huge(page) as libc::c_int != 0
            && mi_page_queue_is_full(to) as libc::c_int != 0
    {
    } else {
        _mi_assert_fail(
            b"(bsize == to->block_size && bsize == from->block_size) || (bsize == to->block_size && mi_page_queue_is_full(from)) || (bsize == from->block_size && mi_page_queue_is_full(to)) || (mi_page_is_huge(page) && mi_page_queue_is_huge(to)) || (mi_page_is_huge(page) && mi_page_queue_is_full(to))\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7270 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 30],
                &[libc::c_char; 30],
            >(b"mi_page_queue_enqueue_from_ex\0"))
                .as_ptr(),
        );
    };
    let mut heap: *mut mi_heap_t = mi_page_heap(page);
    if !((*page).prev).is_null() {
        (*(*page).prev).next = (*page).next;
    }
    if !((*page).next).is_null() {
        (*(*page).next).prev = (*page).prev;
    }
    if page == (*from).last {
        (*from).last = (*page).prev;
    }
    if page == (*from).first {
        (*from).first = (*page).next;
        if mi_heap_contains_queue(heap, from) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"mi_heap_contains_queue(heap, from)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7277 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 30], &[libc::c_char; 30]>(
                    b"mi_page_queue_enqueue_from_ex\0",
                ))
                .as_ptr(),
            );
        };
        mi_heap_queue_first_update(heap, from);
    }
    if enqueue_at_end {
        (*page).prev = (*to).last;
        (*page).next = 0 as *mut mi_page_s;
        if !((*to).last).is_null() {
            if heap == mi_page_heap((*to).last) {
            } else {
                _mi_assert_fail(
                    b"heap == mi_page_heap(to->last)\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    7284 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 30], &[libc::c_char; 30]>(
                        b"mi_page_queue_enqueue_from_ex\0",
                    ))
                    .as_ptr(),
                );
            };
            (*(*to).last).next = page;
            (*to).last = page;
        } else {
            (*to).first = page;
            (*to).last = page;
            mi_heap_queue_first_update(heap, to);
        }
    } else if !((*to).first).is_null() {
        if heap == mi_page_heap((*to).first) {
        } else {
            _mi_assert_fail(
                b"heap == mi_page_heap(to->first)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7296 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 30], &[libc::c_char; 30]>(
                    b"mi_page_queue_enqueue_from_ex\0",
                ))
                .as_ptr(),
            );
        };
        let mut next: *mut mi_page_t = (*(*to).first).next;
        (*page).prev = (*to).first;
        (*page).next = next;
        (*(*to).first).next = page;
        if !next.is_null() {
            (*next).prev = page;
        } else {
            (*to).last = page;
        }
    } else {
        (*page).prev = 0 as *mut mi_page_s;
        (*page).next = 0 as *mut mi_page_s;
        (*to).first = page;
        (*to).last = page;
        mi_heap_queue_first_update(heap, to);
    }
    mi_page_set_in_full(page, mi_page_queue_is_full(to));
}
unsafe extern "C" fn mi_page_queue_enqueue_from(
    mut to: *mut mi_page_queue_t,
    mut from: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
) {
    mi_page_queue_enqueue_from_ex(to, from, 1 as libc::c_int != 0, page);
}
unsafe extern "C" fn mi_page_queue_enqueue_from_full(
    mut to: *mut mi_page_queue_t,
    mut from: *mut mi_page_queue_t,
    mut page: *mut mi_page_t,
) {
    mi_page_queue_enqueue_from_ex(to, from, 0 as libc::c_int != 0, page);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_queue_append(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
    mut append: *mut mi_page_queue_t,
) -> size_t {
    if mi_heap_contains_queue(heap, pq) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_contains_queue(heap,pq)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7325 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"_mi_page_queue_append\0"))
                .as_ptr(),
        );
    };
    if (*pq).block_size == (*append).block_size {
    } else {
        _mi_assert_fail(
            b"pq->block_size == append->block_size\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7326 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"_mi_page_queue_append\0"))
                .as_ptr(),
        );
    };
    if ((*append).first).is_null() {
        return 0 as libc::c_int as size_t;
    }
    let mut count: size_t = 0 as libc::c_int as size_t;
    let mut page: *mut mi_page_t = (*append).first;
    while !page.is_null() {
        ::core::intrinsics::atomic_store_release(&mut (*page).xheap, heap as uintptr_t);
        _mi_page_use_delayed_free(page, MI_USE_DELAYED_FREE, 0 as libc::c_int != 0);
        count = count.wrapping_add(1);
        count;
        page = (*page).next;
    }
    if ((*pq).last).is_null() {
        if ((*pq).first).is_null() {
        } else {
            _mi_assert_fail(
                b"pq->first==NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7335 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"_mi_page_queue_append\0",
                ))
                .as_ptr(),
            );
        };
        (*pq).first = (*append).first;
        (*pq).last = (*append).last;
        mi_heap_queue_first_update(heap, pq);
    } else {
        if !((*pq).last).is_null() {
        } else {
            _mi_assert_fail(
                b"pq->last!=NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7341 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"_mi_page_queue_append\0",
                ))
                .as_ptr(),
            );
        };
        if !((*append).first).is_null() {
        } else {
            _mi_assert_fail(
                b"append->first!=NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7342 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"_mi_page_queue_append\0",
                ))
                .as_ptr(),
            );
        };
        (*(*pq).last).next = (*append).first;
        (*(*append).first).prev = (*pq).last;
        (*pq).last = (*append).last;
    }
    return count;
}
#[inline]
unsafe extern "C" fn mi_page_block_at(
    mut page: *const mi_page_t,
    mut page_start: *mut libc::c_void,
    mut block_size: size_t,
    mut i: size_t,
) -> *mut mi_block_t {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7352 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_page_block_at\0"))
                .as_ptr(),
        );
    };
    if i <= (*page).reserved as size_t {
    } else {
        _mi_assert_fail(
            b"i <= page->reserved\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7353 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_page_block_at\0"))
                .as_ptr(),
        );
    };
    return (page_start as *mut uint8_t).offset((i * block_size) as isize) as *mut mi_block_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_use_delayed_free(
    mut page: *mut mi_page_t,
    mut delay: mi_delayed_t,
    mut override_never: bool,
) {
    while !_mi_page_try_use_delayed_free(page, delay, override_never) {
        mi_atomic_yield();
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_try_use_delayed_free(
    mut page: *mut mi_page_t,
    mut delay: mi_delayed_t,
    mut override_never: bool,
) -> bool {
    let mut tfreex: mi_thread_free_t = 0;
    let mut old_delay: mi_delayed_t = MI_USE_DELAYED_FREE;
    let mut tfree: mi_thread_free_t = 0;
    let mut yield_count: size_t = 0 as libc::c_int as size_t;
    loop {
        tfree = ::core::intrinsics::atomic_load_acquire(&mut (*page).xthread_free);
        tfreex = mi_tf_set_delayed(tfree, delay);
        old_delay = mi_tf_delayed(tfree);
        if (old_delay as libc::c_uint == MI_DELAYED_FREEING as libc::c_int as libc::c_uint)
            as libc::c_int as libc::c_long
            != 0
        {
            if yield_count >= 4 as libc::c_int as size_t {
                return 0 as libc::c_int != 0;
            }
            yield_count = yield_count.wrapping_add(1);
            yield_count;
            mi_atomic_yield();
        } else {
            if delay as libc::c_uint == old_delay as libc::c_uint {
                break;
            }
            if !override_never
                && old_delay as libc::c_uint == MI_NEVER_DELAYED_FREE as libc::c_int as libc::c_uint
            {
                break;
            }
        }
        if !(old_delay as libc::c_uint == MI_DELAYED_FREEING as libc::c_int as libc::c_uint || {
            let fresh36 = ::core::intrinsics::atomic_cxchgweak_release_relaxed(
                &mut (*page).xthread_free as *mut mi_thread_free_t,
                *(&mut tfree as *mut mi_thread_free_t),
                tfreex,
            );
            *(&mut tfree as *mut mi_thread_free_t) = fresh36.0;
            !fresh36.1
        }) {
            break;
        }
    }
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn _mi_page_thread_free_collect(mut page: *mut mi_page_t) {
    let mut head: *mut mi_block_t = 0 as *mut mi_block_t;
    let mut tfreex: mi_thread_free_t = 0;
    let mut tfree: mi_thread_free_t =
        ::core::intrinsics::atomic_load_relaxed(&mut (*page).xthread_free);
    loop {
        head = mi_tf_block(tfree);
        tfreex = mi_tf_set_block(tfree, 0 as *mut mi_block_t);
        let fresh37 = ::core::intrinsics::atomic_cxchgweak_acqrel_acquire(
            &mut (*page).xthread_free as *mut mi_thread_free_t,
            *(&mut tfree as *mut mi_thread_free_t),
            tfreex,
        );
        *(&mut tfree as *mut mi_thread_free_t) = fresh37.0;
        if fresh37.1 {
            break;
        }
    }
    if head.is_null() {
        return;
    }
    let mut max_count: size_t = (*page).capacity as size_t;
    let mut count: size_t = 1 as libc::c_int as size_t;
    let mut tail: *mut mi_block_t = head;
    let mut next: *mut mi_block_t = 0 as *mut mi_block_t;
    loop {
        next = mi_block_next(page, tail);
        if !(!next.is_null() && count <= max_count) {
            break;
        }
        count = count.wrapping_add(1);
        count;
        tail = next;
    }
    if count > max_count {
        _mi_error_message(
            14 as libc::c_int,
            b"corrupted thread-free list\n\0" as *const u8 as *const libc::c_char,
        );
        return;
    }
    mi_block_set_next(page, tail, (*page).local_free);
    (*page).local_free = head;
    (*page).used = ((*page).used as libc::c_int - count as uint16_t as libc::c_int) as uint16_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_free_collect(mut page: *mut mi_page_t, mut force: bool) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7491 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"_mi_page_free_collect\0"))
                .as_ptr(),
        );
    };
    if force as libc::c_int != 0 || !(mi_page_thread_free(page)).is_null() {
        _mi_page_thread_free_collect(page);
    }
    if !((*page).local_free).is_null() {
        if ((*page).free).is_null() as libc::c_int as libc::c_long != 0 {
            (*page).free = (*page).local_free;
            (*page).local_free = 0 as *mut mi_block_t;
            (*page).set_free_is_zero(0 as libc::c_int as uint8_t);
        } else if force {
            let mut tail: *mut mi_block_t = (*page).local_free;
            let mut next: *mut mi_block_t = 0 as *mut mi_block_t;
            loop {
                next = mi_block_next(page, tail);
                if next.is_null() {
                    break;
                }
                tail = next;
            }
            mi_block_set_next(page, tail, (*page).free);
            (*page).free = (*page).local_free;
            (*page).local_free = 0 as *mut mi_block_t;
            (*page).set_free_is_zero(0 as libc::c_int as uint8_t);
        }
    }
    if !force || ((*page).local_free).is_null() {
    } else {
        _mi_assert_fail(
            b"!force || page->local_free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7513 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"_mi_page_free_collect\0"))
                .as_ptr(),
        );
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_reclaim(mut heap: *mut mi_heap_t, mut page: *mut mi_page_t) {
    if mi_page_heap(page) == heap {
    } else {
        _mi_assert_fail(
            b"mi_page_heap(page) == heap\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7517 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_reclaim\0"))
                .as_ptr(),
        );
    };
    if mi_page_thread_free_flag(page) as libc::c_uint
        != MI_NEVER_DELAYED_FREE as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"mi_page_thread_free_flag(page) != MI_NEVER_DELAYED_FREE\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7518 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_reclaim\0"))
                .as_ptr(),
        );
    };
    let mut pq: *mut mi_page_queue_t = mi_page_queue(heap, mi_page_block_size(page));
    mi_page_queue_push(heap, pq, page);
}
unsafe extern "C" fn mi_page_fresh_alloc(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
    mut block_size: size_t,
    mut page_alignment: size_t,
) -> *mut mi_page_t {
    if !pq.is_null() {
    } else {
        _mi_assert_fail(
            b"pq != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7528 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_fresh_alloc\0"))
                .as_ptr(),
        );
    };
    if mi_heap_contains_queue(heap, pq) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_contains_queue(heap, pq)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7529 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_fresh_alloc\0"))
                .as_ptr(),
        );
    };
    if page_alignment > 0 as libc::c_int as size_t
        || block_size as libc::c_ulonglong
            > ((1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
        || block_size == (*pq).block_size
    {
    } else {
        _mi_assert_fail(
            b"page_alignment > 0 || block_size > MI_LARGE_OBJ_SIZE_MAX || block_size == pq->block_size\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7530 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 20],
                &[libc::c_char; 20],
            >(b"mi_page_fresh_alloc\0"))
                .as_ptr(),
        );
    };
    let mut page: *mut mi_page_t = _mi_segment_page_alloc(
        heap,
        block_size,
        page_alignment,
        &mut (*(*heap).tld).segments,
        &mut (*(*heap).tld).os,
    );
    if page.is_null() {
        return 0 as *mut mi_page_t;
    }
    if !pq.is_null() || mi_page_block_size(page) >= block_size {
    } else {
        _mi_assert_fail(
            b"pq!=NULL || mi_page_block_size(page) >= block_size\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7539 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_fresh_alloc\0"))
                .as_ptr(),
        );
    };
    let full_block_size: size_t = if pq.is_null() || mi_page_is_huge(page) as libc::c_int != 0 {
        mi_page_block_size(page)
    } else {
        block_size
    };
    if full_block_size >= block_size {
    } else {
        _mi_assert_fail(
            b"full_block_size >= block_size\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7541 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_fresh_alloc\0"))
                .as_ptr(),
        );
    };
    mi_page_init(heap, page, full_block_size, (*heap).tld);
    _mi_stat_increase(&mut (*(*heap).tld).stats.pages, 1 as libc::c_int as size_t);
    if !pq.is_null() {
        mi_page_queue_push(heap, pq, page);
    }
    return page;
}
unsafe extern "C" fn mi_page_fresh(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
) -> *mut mi_page_t {
    if mi_heap_contains_queue(heap, pq) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_contains_queue(heap, pq)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7549 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_page_fresh\0"))
                .as_ptr(),
        );
    };
    let mut page: *mut mi_page_t =
        mi_page_fresh_alloc(heap, pq, (*pq).block_size, 0 as libc::c_int as size_t);
    if page.is_null() {
        return 0 as *mut mi_page_t;
    }
    if (*pq).block_size == mi_page_block_size(page) {
    } else {
        _mi_assert_fail(
            b"pq->block_size==mi_page_block_size(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7552 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_page_fresh\0"))
                .as_ptr(),
        );
    };
    if pq == mi_page_queue(heap, mi_page_block_size(page)) {
    } else {
        _mi_assert_fail(
            b"pq==mi_page_queue(heap, mi_page_block_size(page))\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7553 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_page_fresh\0"))
                .as_ptr(),
        );
    };
    return page;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_delayed_free_all(mut heap: *mut mi_heap_t) {
    while !_mi_heap_delayed_free_partial(heap) {
        mi_atomic_yield();
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_delayed_free_partial(mut heap: *mut mi_heap_t) -> bool {
    let mut block: *mut mi_block_t =
        ::core::intrinsics::atomic_load_relaxed(&mut (*heap).thread_delayed_free);
    while !block.is_null() && {
        let fresh38 = ::core::intrinsics::atomic_cxchgweak_acqrel_acquire(
            &mut (*heap).thread_delayed_free as *mut *mut mi_block_t,
            *(&mut block as *mut *mut mi_block_t),
            0 as *mut mi_block_t,
        );
        *(&mut block as *mut *mut mi_block_t) = fresh38.0;
        !fresh38.1
    } {}
    let mut all_freed: bool = 1 as libc::c_int != 0;
    while !block.is_null() {
        let mut next: *mut mi_block_t = mi_block_nextx(
            heap as *const libc::c_void,
            block,
            ((*heap).keys).as_mut_ptr(),
        );
        if !_mi_free_delayed_block(block) {
            all_freed = 0 as libc::c_int != 0;
            let mut dfree: *mut mi_block_t =
                ::core::intrinsics::atomic_load_relaxed(&mut (*heap).thread_delayed_free);
            loop {
                mi_block_set_nextx(
                    heap as *const libc::c_void,
                    block,
                    dfree,
                    ((*heap).keys).as_mut_ptr(),
                );
                let fresh39 = ::core::intrinsics::atomic_cxchgweak_release_relaxed(
                    &mut (*heap).thread_delayed_free as *mut *mut mi_block_t,
                    *(&mut dfree as *mut *mut mi_block_t),
                    block,
                );
                *(&mut dfree as *mut *mut mi_block_t) = fresh39.0;
                if fresh39.1 {
                    break;
                }
            }
        }
        block = next;
    }
    return all_freed;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_unfull(mut page: *mut mi_page_t) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7579 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"_mi_page_unfull\0"))
                .as_ptr(),
        );
    };
    if mi_page_is_in_full(page) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_is_in_full(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7581 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"_mi_page_unfull\0"))
                .as_ptr(),
        );
    };
    if !mi_page_is_in_full(page) {
        return;
    }
    let mut heap: *mut mi_heap_t = mi_page_heap(page);
    let mut pqfull: *mut mi_page_queue_t = &mut *((*heap).pages)
        .as_mut_ptr()
        .offset((73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) as isize)
        as *mut mi_page_queue_t;
    mi_page_set_in_full(page, 0 as libc::c_int != 0);
    let mut pq: *mut mi_page_queue_t = mi_heap_page_queue_of(heap, page);
    mi_page_set_in_full(page, 1 as libc::c_int != 0);
    mi_page_queue_enqueue_from_full(pq, pqfull, page);
}
unsafe extern "C" fn mi_page_to_full(mut page: *mut mi_page_t, mut pq: *mut mi_page_queue_t) {
    if pq == mi_page_queue_of(page) {
    } else {
        _mi_assert_fail(
            b"pq == mi_page_queue_of(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7591 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_page_to_full\0"))
                .as_ptr(),
        );
    };
    if !mi_page_immediate_available(page) {
    } else {
        _mi_assert_fail(
            b"!mi_page_immediate_available(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7592 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_page_to_full\0"))
                .as_ptr(),
        );
    };
    if !mi_page_is_in_full(page) {
    } else {
        _mi_assert_fail(
            b"!mi_page_is_in_full(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7593 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_page_to_full\0"))
                .as_ptr(),
        );
    };
    if mi_page_is_in_full(page) {
        return;
    }
    mi_page_queue_enqueue_from(
        &mut *((*(mi_page_heap as unsafe extern "C" fn(*const mi_page_t) -> *mut mi_heap_t)(page))
            .pages)
            .as_mut_ptr()
            .offset((73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) as isize),
        pq,
        page,
    );
    _mi_page_free_collect(page, 0 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_abandon(mut page: *mut mi_page_t, mut pq: *mut mi_page_queue_t) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7599 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_abandon\0"))
                .as_ptr(),
        );
    };
    if pq == mi_page_queue_of(page) {
    } else {
        _mi_assert_fail(
            b"pq == mi_page_queue_of(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7601 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_abandon\0"))
                .as_ptr(),
        );
    };
    if !(mi_page_heap(page)).is_null() {
    } else {
        _mi_assert_fail(
            b"mi_page_heap(page) != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7602 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_abandon\0"))
                .as_ptr(),
        );
    };
    let mut pheap: *mut mi_heap_t = mi_page_heap(page);
    let mut segments_tld: *mut mi_segments_tld_t = &mut (*(*pheap).tld).segments;
    mi_page_queue_remove(pq, page);
    if mi_page_thread_free_flag(page) as libc::c_uint
        == MI_NEVER_DELAYED_FREE as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"mi_page_thread_free_flag(page)==MI_NEVER_DELAYED_FREE\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7606 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_abandon\0"))
                .as_ptr(),
        );
    };
    mi_page_set_heap(page, 0 as *mut mi_heap_t);
    let mut block: *mut mi_block_t = (*pheap).thread_delayed_free;
    while !block.is_null() {
        if _mi_ptr_page(block as *mut libc::c_void) != page {
        } else {
            _mi_assert_fail(
                b"_mi_ptr_page(block) != page\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7610 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_abandon\0"))
                    .as_ptr(),
            );
        };
        block = mi_block_nextx(
            pheap as *const libc::c_void,
            block,
            ((*pheap).keys).as_mut_ptr(),
        );
    }
    if (mi_page_heap(page)).is_null() {
    } else {
        _mi_assert_fail(
            b"mi_page_heap(page) == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7613 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_page_abandon\0"))
                .as_ptr(),
        );
    };
    _mi_segment_page_abandon(page, segments_tld);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_force_abandon(mut page: *mut mi_page_t) {
    let mut heap: *mut mi_heap_t = mi_page_heap(page);
    _mi_page_use_delayed_free(page, MI_NEVER_DELAYED_FREE, 0 as libc::c_int != 0);
    _mi_heap_delayed_free_all(heap);
    if (*page).capacity as libc::c_int == 0 as libc::c_int {
        return;
    }
    let mut pq: *mut mi_page_queue_t = mi_heap_page_queue_of(heap, page);
    if mi_page_all_free(page) {
        _mi_page_free(page, pq, 0 as libc::c_int != 0);
    } else {
        _mi_page_abandon(page, pq);
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_free(
    mut page: *mut mi_page_t,
    mut pq: *mut mi_page_queue_t,
    mut force: bool,
) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7630 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"_mi_page_free\0"))
                .as_ptr(),
        );
    };
    if pq == mi_page_queue_of(page) {
    } else {
        _mi_assert_fail(
            b"pq == mi_page_queue_of(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7632 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"_mi_page_free\0"))
                .as_ptr(),
        );
    };
    if mi_page_all_free(page) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_all_free(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7633 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"_mi_page_free\0"))
                .as_ptr(),
        );
    };
    if mi_page_thread_free_flag(page) as libc::c_uint
        != MI_DELAYED_FREEING as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"mi_page_thread_free_flag(page)!=MI_DELAYED_FREEING\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7634 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"_mi_page_free\0"))
                .as_ptr(),
        );
    };
    mi_page_set_has_aligned(page, 0 as libc::c_int != 0);
    let mut segments_tld: *mut mi_segments_tld_t =
        &mut (*(*(mi_page_heap as unsafe extern "C" fn(*const mi_page_t) -> *mut mi_heap_t)(page))
            .tld)
            .segments;
    mi_page_queue_remove(pq, page);
    mi_page_set_heap(page, 0 as *mut mi_heap_t);
    _mi_segment_page_free(page, force, segments_tld);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_page_retire(mut page: *mut mi_page_t) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7644 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"_mi_page_retire\0"))
                .as_ptr(),
        );
    };
    if mi_page_all_free(page) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_all_free(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7646 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"_mi_page_retire\0"))
                .as_ptr(),
        );
    };
    mi_page_set_has_aligned(page, 0 as libc::c_int != 0);
    let mut pq: *mut mi_page_queue_t = mi_page_queue_of(page);
    let bsize: size_t = mi_page_block_size(page);
    if !mi_page_queue_is_special(pq) as libc::c_int as libc::c_long != 0 {
        if (*pq).last == page && (*pq).first == page {
            _mi_stat_counter_increase(
                &mut _mi_stats_main.page_no_retire,
                1 as libc::c_int as size_t,
            );
            (*page).set_retire_expire(
                (if bsize as libc::c_ulonglong
                    <= ((1 as libc::c_ulonglong) << 13 as libc::c_int + 3 as libc::c_int)
                        .wrapping_div(4 as libc::c_int as libc::c_ulonglong)
                {
                    16 as libc::c_int
                } else {
                    16 as libc::c_int / 4 as libc::c_int
                }) as uint8_t,
            );
            let mut heap: *mut mi_heap_t = mi_page_heap(page);
            if pq >= ((*heap).pages).as_mut_ptr() {
            } else {
                _mi_assert_fail(
                    b"pq >= heap->pages\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    7656 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(
                        b"_mi_page_retire\0",
                    ))
                    .as_ptr(),
                );
            };
            let index: size_t =
                pq.offset_from(((*heap).pages).as_mut_ptr()) as libc::c_long as size_t;
            if index < (73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) as size_t
                && index < 73 as libc::c_uint as size_t
            {
            } else {
                _mi_assert_fail(
                    b"index < MI_BIN_FULL && index < MI_BIN_HUGE\0" as *const u8
                        as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    7658 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(
                        b"_mi_page_retire\0",
                    ))
                    .as_ptr(),
                );
            };
            if index < (*heap).page_retired_min {
                (*heap).page_retired_min = index;
            }
            if index > (*heap).page_retired_max {
                (*heap).page_retired_max = index;
            }
            if mi_page_all_free(page) as libc::c_int != 0 {
            } else {
                _mi_assert_fail(
                    b"mi_page_all_free(page)\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    7661 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(
                        b"_mi_page_retire\0",
                    ))
                    .as_ptr(),
                );
            };
            return;
        }
    }
    _mi_page_free(page, pq, 0 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_heap_collect_retired(mut heap: *mut mi_heap_t, mut force: bool) {
    let mut min: size_t =
        (73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) as size_t;
    let mut max: size_t = 0 as libc::c_int as size_t;
    let mut bin: size_t = (*heap).page_retired_min;
    while bin <= (*heap).page_retired_max {
        let mut pq: *mut mi_page_queue_t =
            &mut *((*heap).pages).as_mut_ptr().offset(bin as isize) as *mut mi_page_queue_t;
        let mut page: *mut mi_page_t = (*pq).first;
        if !page.is_null() && (*page).retire_expire() as libc::c_int != 0 as libc::c_int {
            if mi_page_all_free(page) {
                (*page).set_retire_expire((*page).retire_expire() - 1);
                (*page).retire_expire();
                if force as libc::c_int != 0
                    || (*page).retire_expire() as libc::c_int == 0 as libc::c_int
                {
                    _mi_page_free((*pq).first, pq, force);
                } else {
                    if bin < min {
                        min = bin;
                    }
                    if bin > max {
                        max = bin;
                    }
                }
            } else {
                (*page).set_retire_expire(0 as libc::c_int as uint8_t);
            }
        }
        bin = bin.wrapping_add(1);
        bin;
    }
    (*heap).page_retired_min = min;
    (*heap).page_retired_max = max;
}
unsafe extern "C" fn mi_page_free_list_extend_secure(
    heap: *mut mi_heap_t,
    page: *mut mi_page_t,
    bsize: size_t,
    extend: size_t,
    stats: *mut mi_stats_t,
) {
    if ((*page).free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7699 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                b"mi_page_free_list_extend_secure\0",
            ))
            .as_ptr(),
        );
    };
    if ((*page).local_free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->local_free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7700 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                b"mi_page_free_list_extend_secure\0",
            ))
            .as_ptr(),
        );
    };
    if ((*page).capacity as size_t).wrapping_add(extend) <= (*page).reserved as size_t {
    } else {
        _mi_assert_fail(
            b"page->capacity + extend <= page->reserved\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7702 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                b"mi_page_free_list_extend_secure\0",
            ))
            .as_ptr(),
        );
    };
    if bsize == mi_page_block_size(page) {
    } else {
        _mi_assert_fail(
            b"bsize == mi_page_block_size(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7703 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                b"mi_page_free_list_extend_secure\0",
            ))
            .as_ptr(),
        );
    };
    let page_area: *mut libc::c_void = mi_page_start(page) as *mut libc::c_void;
    let mut shift: size_t = 6 as libc::c_int as size_t;
    while extend >> shift == 0 as libc::c_int as size_t {
        shift = shift.wrapping_sub(1);
        shift;
    }
    let slice_count: size_t = (1 as libc::c_uint as size_t) << shift;
    let slice_extend: size_t = extend / slice_count;
    if slice_extend >= 1 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"slice_extend >= 1\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7711 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 32], &[libc::c_char; 32]>(
                b"mi_page_free_list_extend_secure\0",
            ))
            .as_ptr(),
        );
    };
    let mut blocks: [*mut mi_block_t; 64] = [0 as *mut mi_block_t; 64];
    let mut counts: [size_t; 64] = [0; 64];
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < slice_count {
        blocks[i as usize] = mi_page_block_at(
            page,
            page_area,
            bsize,
            ((*page).capacity as size_t).wrapping_add(i * slice_extend),
        );
        counts[i as usize] = slice_extend;
        i = i.wrapping_add(1);
        i;
    }
    counts[slice_count.wrapping_sub(1 as libc::c_int as size_t) as usize] = (counts
        [slice_count.wrapping_sub(1 as libc::c_int as size_t) as usize])
        .wrapping_add(extend % slice_count);
    let r: uintptr_t = _mi_heap_random_next(heap);
    let mut current: size_t = r.wrapping_rem(slice_count);
    counts[current as usize] = (counts[current as usize]).wrapping_sub(1);
    counts[current as usize];
    let free_start: *mut mi_block_t = blocks[current as usize];
    let mut rnd: uintptr_t = _mi_random_shuffle(r | 1 as libc::c_int as uintptr_t);
    let mut i_0: size_t = 1 as libc::c_int as size_t;
    while i_0 < extend {
        let round: size_t = i_0 % ((1 as libc::c_int) << 3 as libc::c_int) as size_t;
        if round == 0 as libc::c_int as size_t {
            rnd = _mi_random_shuffle(rnd);
        }
        let mut next: size_t = rnd >> 8 as libc::c_int as size_t * round
            & slice_count.wrapping_sub(1 as libc::c_int as size_t);
        while counts[next as usize] == 0 as libc::c_int as size_t {
            next = next.wrapping_add(1);
            next;
            if next == slice_count {
                next = 0 as libc::c_int as size_t;
            }
        }
        counts[next as usize] = (counts[next as usize]).wrapping_sub(1);
        counts[next as usize];
        let block: *mut mi_block_t = blocks[current as usize];
        blocks[current as usize] =
            (block as *mut uint8_t).offset(bsize as isize) as *mut mi_block_t;
        mi_block_set_next(page, block, blocks[next as usize]);
        current = next;
        i_0 = i_0.wrapping_add(1);
        i_0;
    }
    mi_block_set_next(page, blocks[current as usize], (*page).free);
    (*page).free = free_start;
}
#[inline(never)]
unsafe extern "C" fn mi_page_free_list_extend(
    page: *mut mi_page_t,
    bsize: size_t,
    extend: size_t,
    stats: *mut mi_stats_t,
) {
    if ((*page).free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7745 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_page_free_list_extend\0",
            ))
            .as_ptr(),
        );
    };
    if ((*page).local_free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->local_free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7746 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_page_free_list_extend\0",
            ))
            .as_ptr(),
        );
    };
    if ((*page).capacity as size_t).wrapping_add(extend) <= (*page).reserved as size_t {
    } else {
        _mi_assert_fail(
            b"page->capacity + extend <= page->reserved\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7748 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_page_free_list_extend\0",
            ))
            .as_ptr(),
        );
    };
    if bsize == mi_page_block_size(page) {
    } else {
        _mi_assert_fail(
            b"bsize == mi_page_block_size(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7749 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_page_free_list_extend\0",
            ))
            .as_ptr(),
        );
    };
    let page_area: *mut libc::c_void = mi_page_start(page) as *mut libc::c_void;
    let start: *mut mi_block_t =
        mi_page_block_at(page, page_area, bsize, (*page).capacity as size_t);
    let last: *mut mi_block_t = mi_page_block_at(
        page,
        page_area,
        bsize,
        ((*page).capacity as size_t)
            .wrapping_add(extend)
            .wrapping_sub(1 as libc::c_int as size_t),
    );
    let mut block: *mut mi_block_t = start;
    while block <= last {
        let mut next: *mut mi_block_t =
            (block as *mut uint8_t).offset(bsize as isize) as *mut mi_block_t;
        mi_block_set_next(page, block, next);
        block = next;
    }
    mi_block_set_next(page, last, (*page).free);
    (*page).free = start;
}
unsafe extern "C" fn mi_page_extend_free(
    mut heap: *mut mi_heap_t,
    mut page: *mut mi_page_t,
    mut tld: *mut mi_tld_t,
) {
    if ((*page).free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7771 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_extend_free\0"))
                .as_ptr(),
        );
    };
    if ((*page).local_free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->local_free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7772 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_extend_free\0"))
                .as_ptr(),
        );
    };
    if !((*page).free).is_null() {
        return;
    }
    if (*page).capacity as libc::c_int >= (*page).reserved as libc::c_int {
        return;
    }
    let mut page_size: size_t = 0;
    _mi_segment_page_start(_mi_page_segment(page), page, &mut page_size);
    _mi_stat_counter_increase(&mut (*tld).stats.pages_extended, 1 as libc::c_int as size_t);
    let bsize: size_t = mi_page_block_size(page);
    let mut extend: size_t =
        ((*page).reserved as libc::c_int - (*page).capacity as libc::c_int) as size_t;
    if extend > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"extend > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7781 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_extend_free\0"))
                .as_ptr(),
        );
    };
    let mut max_extend: size_t = if bsize >= (4 as libc::c_int * 1024 as libc::c_int) as size_t {
        1 as libc::c_int as size_t
    } else {
        (4 as libc::c_int * 1024 as libc::c_int) as size_t / bsize
    };
    if max_extend < 1 as libc::c_int as size_t {
        max_extend = 1 as libc::c_int as size_t;
    }
    if max_extend > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"max_extend > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7784 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_extend_free\0"))
                .as_ptr(),
        );
    };
    if extend > max_extend {
        extend = max_extend;
    }
    if extend > 0 as libc::c_int as size_t
        && extend.wrapping_add((*page).capacity as size_t) <= (*page).reserved as size_t
    {
    } else {
        _mi_assert_fail(
            b"extend > 0 && extend + page->capacity <= page->reserved\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7788 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_extend_free\0"))
                .as_ptr(),
        );
    };
    if extend < (1 as libc::c_ulong) << 16 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"extend < (1UL<<16)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7789 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(b"mi_page_extend_free\0"))
                .as_ptr(),
        );
    };
    if extend < 2 as libc::c_int as size_t || 0 as libc::c_int == 0 as libc::c_int {
        mi_page_free_list_extend(page, bsize, extend, &mut (*tld).stats);
    } else {
        mi_page_free_list_extend_secure(heap, page, bsize, extend, &mut (*tld).stats);
    }
    (*page).capacity =
        ((*page).capacity as libc::c_int + extend as uint16_t as libc::c_int) as uint16_t;
    _mi_stat_increase(&mut (*tld).stats.page_committed, extend * bsize);
}
unsafe extern "C" fn mi_page_init(
    mut heap: *mut mi_heap_t,
    mut page: *mut mi_page_t,
    mut block_size: size_t,
    mut tld: *mut mi_tld_t,
) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7801 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    let mut segment: *mut mi_segment_t = _mi_page_segment(page);
    if !segment.is_null() {
    } else {
        _mi_assert_fail(
            b"segment != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7803 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if block_size > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"block_size > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7804 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    mi_page_set_heap(page, heap);
    (*page).block_size = block_size;
    let mut page_size: size_t = 0;
    (*page).page_start = _mi_segment_page_start(segment, page, &mut page_size);
    if page_size / block_size < ((1 as libc::c_long) << 16 as libc::c_int) as size_t {
    } else {
        _mi_assert_fail(
            b"page_size / block_size < (1L<<16)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7810 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    (*page).reserved = (page_size / block_size) as uint16_t;
    if (*page).reserved as libc::c_int > 0 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"page->reserved > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7812 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    (*page).keys[0 as libc::c_int as usize] = _mi_heap_random_next(heap);
    (*page).keys[1 as libc::c_int as usize] = _mi_heap_random_next(heap);
    (*page).set_free_is_zero((*page).is_zero_init());
    if block_size > 0 as libc::c_int as size_t
        && _mi_is_power_of_two(block_size) as libc::c_int != 0
    {
        (*page).block_size_shift = mi_ctz(block_size) as uint8_t;
    } else {
        (*page).block_size_shift = 0 as libc::c_int as uint8_t;
    }
    if (*page).capacity as libc::c_int == 0 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"page->capacity == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7830 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if ((*page).free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7831 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if (*page).used as libc::c_int == 0 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"page->used == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7832 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if (*page).xthread_free == 0 as libc::c_int as mi_thread_free_t {
    } else {
        _mi_assert_fail(
            b"page->xthread_free == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7833 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if ((*page).next).is_null() {
    } else {
        _mi_assert_fail(
            b"page->next == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7834 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if ((*page).prev).is_null() {
    } else {
        _mi_assert_fail(
            b"page->prev == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7835 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if (*page).retire_expire() as libc::c_int == 0 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"page->retire_expire == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7836 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if !mi_page_has_aligned(page) {
    } else {
        _mi_assert_fail(
            b"!mi_page_has_aligned(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7837 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if (*page).keys[0 as libc::c_int as usize] != 0 as libc::c_int as uintptr_t {
    } else {
        _mi_assert_fail(
            b"page->keys[0] != 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7839 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if (*page).keys[1 as libc::c_int as usize] != 0 as libc::c_int as uintptr_t {
    } else {
        _mi_assert_fail(
            b"page->keys[1] != 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7840 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
    if (*page).block_size_shift as libc::c_int == 0 as libc::c_int
        || block_size == (1 as libc::c_int as size_t) << (*page).block_size_shift as libc::c_int
    {
    } else {
        _mi_assert_fail(
            b"page->block_size_shift == 0 || (block_size == ((size_t)1 << page->block_size_shift))\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7842 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 13],
                &[libc::c_char; 13],
            >(b"mi_page_init\0"))
                .as_ptr(),
        );
    };
    mi_page_extend_free(heap, page, tld);
    if mi_page_immediate_available(page) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_immediate_available(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7845 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_page_init\0")).as_ptr(),
        );
    };
}
unsafe extern "C" fn mi_page_is_expandable(mut page: *const mi_page_t) -> bool {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7849 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_page_is_expandable\0"))
                .as_ptr(),
        );
    };
    if (*page).capacity as libc::c_int <= (*page).reserved as libc::c_int {
    } else {
        _mi_assert_fail(
            b"page->capacity <= page->reserved\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7850 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_page_is_expandable\0"))
                .as_ptr(),
        );
    };
    return ((*page).capacity as libc::c_int) < (*page).reserved as libc::c_int;
}
unsafe extern "C" fn mi_page_queue_find_free_ex(
    mut heap: *mut mi_heap_t,
    mut pq: *mut mi_page_queue_t,
    mut first_try: bool,
) -> *mut mi_page_t {
    let mut count: size_t = 0 as libc::c_int as size_t;
    let mut candidate_count: size_t = 0 as libc::c_int as size_t;
    let mut page_candidate: *mut mi_page_t = 0 as *mut mi_page_t;
    let mut page: *mut mi_page_t = (*pq).first;
    while !page.is_null() {
        let mut next: *mut mi_page_t = (*page).next;
        count = count.wrapping_add(1);
        count;
        candidate_count = candidate_count.wrapping_add(1);
        candidate_count;
        _mi_page_free_collect(page, 0 as libc::c_int != 0);
        let immediate_available: bool = mi_page_immediate_available(page);
        if !immediate_available && !mi_page_is_expandable(page) {
            if !mi_page_is_in_full(page) && !mi_page_immediate_available(page) {
            } else {
                _mi_assert_fail(
                    b"!mi_page_is_in_full(page) && !mi_page_immediate_available(page)\0"
                        as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    7872 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                        b"mi_page_queue_find_free_ex\0",
                    ))
                    .as_ptr(),
                );
            };
            mi_page_to_full(page, pq);
        } else {
            if page_candidate.is_null() {
                page_candidate = page;
                candidate_count = 0 as libc::c_int as size_t;
            } else if (*page).used as libc::c_int >= (*page_candidate).used as libc::c_int {
                page_candidate = page;
            }
            if immediate_available as libc::c_int != 0
                || candidate_count > 8 as libc::c_int as size_t
            {
                if !page_candidate.is_null() {
                } else {
                    _mi_assert_fail(
                        b"page_candidate!=NULL\0" as *const u8 as *const libc::c_char,
                        b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                        7884 as libc::c_int as libc::c_uint,
                        (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                            b"mi_page_queue_find_free_ex\0",
                        ))
                        .as_ptr(),
                    );
                };
                break;
            }
        }
        page = next;
    }
    _mi_stat_counter_increase(&mut (*(*heap).tld).stats.searches, count);
    if !page_candidate.is_null() {
        page = page_candidate;
    }
    if !page.is_null() && !mi_page_immediate_available(page) {
        if mi_page_is_expandable(page) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"mi_page_is_expandable(page)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7902 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"mi_page_queue_find_free_ex\0",
                ))
                .as_ptr(),
            );
        };
        mi_page_extend_free(heap, page, (*heap).tld);
    }
    if page.is_null() {
        _mi_heap_collect_retired(heap, 0 as libc::c_int != 0);
        page = mi_page_fresh(heap, pq);
        if page.is_null() && first_try as libc::c_int != 0 {
            page = mi_page_queue_find_free_ex(heap, pq, 0 as libc::c_int != 0);
        }
    } else {
        mi_page_queue_move_to_front(heap, pq, page);
        (*page).set_retire_expire(0 as libc::c_int as uint8_t);
    }
    if page.is_null() || mi_page_immediate_available(page) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"page == NULL || mi_page_immediate_available(page)\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7916 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"mi_page_queue_find_free_ex\0",
            ))
            .as_ptr(),
        );
    };
    return page;
}
#[inline]
unsafe extern "C" fn mi_find_free_page(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
) -> *mut mi_page_t {
    let mut pq: *mut mi_page_queue_t = mi_page_queue(heap, size);
    let mut page: *mut mi_page_t = (*pq).first;
    if !page.is_null() {
        _mi_page_free_collect(page, 0 as libc::c_int != 0);
        if mi_page_immediate_available(page) {
            (*page).set_retire_expire(0 as libc::c_int as uint8_t);
            return page;
        }
    }
    return mi_page_queue_find_free_ex(heap, pq, 1 as libc::c_int != 0);
}
static mut deferred_free: Option<mi_deferred_free_fun> = None;
static mut deferred_arg: *mut libc::c_void = 0 as *const libc::c_void as *mut libc::c_void;
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_deferred_free(mut heap: *mut mi_heap_t, mut force: bool) {
    (*(*heap).tld).heartbeat = ((*(*heap).tld).heartbeat).wrapping_add(1);
    (*(*heap).tld).heartbeat;
    if deferred_free.is_some() && !(*(*heap).tld).recurse {
        (*(*heap).tld).recurse = 1 as libc::c_int != 0;
        deferred_free.expect("non-null function pointer")(
            force,
            (*(*heap).tld).heartbeat,
            ::core::intrinsics::atomic_load_relaxed(&mut deferred_arg),
        );
        (*(*heap).tld).recurse = 0 as libc::c_int != 0;
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_register_deferred_free(
    mut fn_0: Option<mi_deferred_free_fun>,
    mut arg: *mut libc::c_void,
) {
    ::core::ptr::write_volatile(
        &mut deferred_free as *mut Option<mi_deferred_free_fun>,
        fn_0,
    );
    ::core::intrinsics::atomic_store_release(&mut deferred_arg, arg);
}
unsafe extern "C" fn mi_huge_page_alloc(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut page_alignment: size_t,
) -> *mut mi_page_t {
    let mut block_size: size_t = _mi_os_good_alloc_size(size);
    if mi_bin(block_size) as libc::c_uint == 73 as libc::c_uint
        || page_alignment > 0 as libc::c_int as size_t
    {
    } else {
        _mi_assert_fail(
            b"mi_bin(block_size) == MI_BIN_HUGE || page_alignment > 0\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7956 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_huge_page_alloc\0"))
                .as_ptr(),
        );
    };
    let mut pq: *mut mi_page_queue_t = mi_page_queue(
        heap,
        ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
            .wrapping_add(1 as libc::c_int as libc::c_ulonglong) as size_t,
    );
    if mi_page_queue_is_huge(pq) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_queue_is_huge(pq)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7961 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_huge_page_alloc\0"))
                .as_ptr(),
        );
    };
    let mut page: *mut mi_page_t = mi_page_fresh_alloc(heap, pq, block_size, page_alignment);
    if !page.is_null() {
        if mi_page_block_size(page) >= size {
        } else {
            _mi_assert_fail(
                b"mi_page_block_size(page) >= size\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7965 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_huge_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        if mi_page_immediate_available(page) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"mi_page_immediate_available(page)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7966 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_huge_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        if mi_page_is_huge(page) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"mi_page_is_huge(page)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7967 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_huge_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        if (*_mi_page_segment(page)).page_kind as libc::c_uint
            == MI_PAGE_HUGE as libc::c_int as libc::c_uint
        {
        } else {
            _mi_assert_fail(
                b"_mi_page_segment(page)->page_kind == MI_PAGE_HUGE\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7968 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_huge_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        if (*_mi_page_segment(page)).used == 1 as libc::c_int as size_t {
        } else {
            _mi_assert_fail(
                b"_mi_page_segment(page)->used==1\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7969 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_huge_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        _mi_stat_increase(&mut (*(*heap).tld).stats.huge, mi_page_block_size(page));
        _mi_stat_counter_increase(
            &mut (*(*heap).tld).stats.huge_count,
            1 as libc::c_int as size_t,
        );
    }
    return page;
}
unsafe extern "C" fn mi_find_page(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut huge_alignment: size_t,
) -> *mut mi_page_t {
    let req_size: size_t =
        size.wrapping_sub(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong);
    if (req_size as libc::c_ulonglong
        > ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
            .wrapping_sub(
                ::core::mem::size_of::<mi_padding_t>() as libc::c_ulong as libc::c_ulonglong
            )
        || huge_alignment > 0 as libc::c_int as size_t) as libc::c_int as libc::c_long
        != 0
    {
        if (req_size > 9223372036854775807 as libc::c_long as size_t) as libc::c_int as libc::c_long
            != 0
        {
            _mi_error_message(
                75 as libc::c_int,
                b"allocation request is too large (%zu bytes)\n\0" as *const u8
                    as *const libc::c_char,
                req_size,
            );
            return 0 as *mut mi_page_t;
        } else {
            return mi_huge_page_alloc(heap, size, huge_alignment);
        }
    } else {
        if size >= ::core::mem::size_of::<mi_padding_t>() as libc::c_ulong {
        } else {
            _mi_assert_fail(
                b"size >= MI_PADDING_SIZE\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                7992 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"mi_find_page\0"))
                    .as_ptr(),
            );
        };
        return mi_find_free_page(heap, size);
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_malloc_generic(
    mut heap: *mut mi_heap_t,
    mut size: size_t,
    mut zero: bool,
    mut huge_alignment: size_t,
) -> *mut libc::c_void {
    if !heap.is_null() {
    } else {
        _mi_assert_fail(
            b"heap != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            7999 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_malloc_generic\0"))
                .as_ptr(),
        );
    };
    if !mi_heap_is_initialized(heap) as libc::c_int as libc::c_long != 0 {
        heap = mi_heap_get_default();
        if !mi_heap_is_initialized(heap) as libc::c_int as libc::c_long != 0 {
            return 0 as *mut libc::c_void;
        }
    }
    if mi_heap_is_initialized(heap) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_heap_is_initialized(heap)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8004 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_malloc_generic\0"))
                .as_ptr(),
        );
    };
    _mi_deferred_free(heap, 0 as libc::c_int != 0);
    _mi_heap_delayed_free_partial(heap);
    let mut page: *mut mi_page_t = mi_find_page(heap, size, huge_alignment);
    if page.is_null() as libc::c_int as libc::c_long != 0 {
        mi_heap_collect(heap, 1 as libc::c_int != 0);
        page = mi_find_page(heap, size, huge_alignment);
    }
    if page.is_null() as libc::c_int as libc::c_long != 0 {
        let req_size: size_t =
            size.wrapping_sub(::core::mem::size_of::<mi_padding_t>() as libc::c_ulong);
        _mi_error_message(
            12 as libc::c_int,
            b"unable to allocate memory (%zu bytes)\n\0" as *const u8 as *const libc::c_char,
            req_size,
        );
        return 0 as *mut libc::c_void;
    }
    if mi_page_immediate_available(page) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_immediate_available(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8017 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_malloc_generic\0"))
                .as_ptr(),
        );
    };
    if mi_page_block_size(page) >= size {
    } else {
        _mi_assert_fail(
            b"mi_page_block_size(page) >= size\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8018 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_malloc_generic\0"))
                .as_ptr(),
        );
    };
    if (zero as libc::c_int != 0 && mi_page_is_huge(page) as libc::c_int != 0) as libc::c_int
        as libc::c_long
        != 0
    {
        let mut p: *mut libc::c_void = _mi_page_malloc(heap, page, size);
        if !p.is_null() {
        } else {
            _mi_assert_fail(
                b"p != NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8021 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"_mi_malloc_generic\0",
                ))
                .as_ptr(),
            );
        };
        _mi_memzero_aligned(p, mi_page_usable_block_size(page));
        return p;
    } else {
        return _mi_page_malloc_zero(heap, page, size, zero);
    };
}
#[inline]
unsafe extern "C" fn rotl(mut x: uint32_t, mut shift: uint32_t) -> uint32_t {
    return x << shift | x >> (32 as libc::c_int as uint32_t).wrapping_sub(shift);
}
#[inline]
unsafe extern "C" fn qround(
    mut x: *mut uint32_t,
    mut a: size_t,
    mut b: size_t,
    mut c: size_t,
    mut d: size_t,
) {
    let ref mut fresh40 = *x.offset(a as isize);
    *fresh40 = (*fresh40).wrapping_add(*x.offset(b as isize));
    *x.offset(d as isize) = rotl(
        *x.offset(d as isize) ^ *x.offset(a as isize),
        16 as libc::c_int as uint32_t,
    );
    let ref mut fresh41 = *x.offset(c as isize);
    *fresh41 = (*fresh41).wrapping_add(*x.offset(d as isize));
    *x.offset(b as isize) = rotl(
        *x.offset(b as isize) ^ *x.offset(c as isize),
        12 as libc::c_int as uint32_t,
    );
    let ref mut fresh42 = *x.offset(a as isize);
    *fresh42 = (*fresh42).wrapping_add(*x.offset(b as isize));
    *x.offset(d as isize) = rotl(
        *x.offset(d as isize) ^ *x.offset(a as isize),
        8 as libc::c_int as uint32_t,
    );
    let ref mut fresh43 = *x.offset(c as isize);
    *fresh43 = (*fresh43).wrapping_add(*x.offset(d as isize));
    *x.offset(b as isize) = rotl(
        *x.offset(b as isize) ^ *x.offset(c as isize),
        7 as libc::c_int as uint32_t,
    );
}
unsafe extern "C" fn chacha_block(mut ctx: *mut mi_random_ctx_t) {
    let mut x: [uint32_t; 16] = [0; 16];
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < 16 as libc::c_int as size_t {
        x[i as usize] = (*ctx).input[i as usize];
        i = i.wrapping_add(1);
        i;
    }
    let mut i_0: size_t = 0 as libc::c_int as size_t;
    while i_0 < 20 as libc::c_int as size_t {
        qround(
            x.as_mut_ptr(),
            0 as libc::c_int as size_t,
            4 as libc::c_int as size_t,
            8 as libc::c_int as size_t,
            12 as libc::c_int as size_t,
        );
        qround(
            x.as_mut_ptr(),
            1 as libc::c_int as size_t,
            5 as libc::c_int as size_t,
            9 as libc::c_int as size_t,
            13 as libc::c_int as size_t,
        );
        qround(
            x.as_mut_ptr(),
            2 as libc::c_int as size_t,
            6 as libc::c_int as size_t,
            10 as libc::c_int as size_t,
            14 as libc::c_int as size_t,
        );
        qround(
            x.as_mut_ptr(),
            3 as libc::c_int as size_t,
            7 as libc::c_int as size_t,
            11 as libc::c_int as size_t,
            15 as libc::c_int as size_t,
        );
        qround(
            x.as_mut_ptr(),
            0 as libc::c_int as size_t,
            5 as libc::c_int as size_t,
            10 as libc::c_int as size_t,
            15 as libc::c_int as size_t,
        );
        qround(
            x.as_mut_ptr(),
            1 as libc::c_int as size_t,
            6 as libc::c_int as size_t,
            11 as libc::c_int as size_t,
            12 as libc::c_int as size_t,
        );
        qround(
            x.as_mut_ptr(),
            2 as libc::c_int as size_t,
            7 as libc::c_int as size_t,
            8 as libc::c_int as size_t,
            13 as libc::c_int as size_t,
        );
        qround(
            x.as_mut_ptr(),
            3 as libc::c_int as size_t,
            4 as libc::c_int as size_t,
            9 as libc::c_int as size_t,
            14 as libc::c_int as size_t,
        );
        i_0 = i_0.wrapping_add(2 as libc::c_int as size_t);
    }
    let mut i_1: size_t = 0 as libc::c_int as size_t;
    while i_1 < 16 as libc::c_int as size_t {
        (*ctx).output[i_1 as usize] = (x[i_1 as usize]).wrapping_add((*ctx).input[i_1 as usize]);
        i_1 = i_1.wrapping_add(1);
        i_1;
    }
    (*ctx).output_available = 16 as libc::c_int;
    (*ctx).input[12 as libc::c_int as usize] =
        ((*ctx).input[12 as libc::c_int as usize]).wrapping_add(1 as libc::c_int as uint32_t);
    if (*ctx).input[12 as libc::c_int as usize] == 0 as libc::c_int as uint32_t {
        (*ctx).input[13 as libc::c_int as usize] =
            ((*ctx).input[13 as libc::c_int as usize]).wrapping_add(1 as libc::c_int as uint32_t);
        if (*ctx).input[13 as libc::c_int as usize] == 0 as libc::c_int as uint32_t {
            (*ctx).input[14 as libc::c_int as usize] = ((*ctx).input[14 as libc::c_int as usize])
                .wrapping_add(1 as libc::c_int as uint32_t);
        }
    }
}
unsafe extern "C" fn chacha_next32(mut ctx: *mut mi_random_ctx_t) -> uint32_t {
    if (*ctx).output_available <= 0 as libc::c_int {
        chacha_block(ctx);
        (*ctx).output_available = 16 as libc::c_int;
    }
    let x: uint32_t = (*ctx).output[(16 as libc::c_int - (*ctx).output_available) as usize];
    (*ctx).output[(16 as libc::c_int - (*ctx).output_available) as usize] =
        0 as libc::c_int as uint32_t;
    (*ctx).output_available -= 1;
    (*ctx).output_available;
    return x;
}
#[inline]
unsafe extern "C" fn read32(mut p: *const uint8_t, mut idx32: size_t) -> uint32_t {
    let i: size_t = 4 as libc::c_int as size_t * idx32;
    return *p.offset(i.wrapping_add(0 as libc::c_int as size_t) as isize) as uint32_t
        | (*p.offset(i.wrapping_add(1 as libc::c_int as size_t) as isize) as uint32_t)
            << 8 as libc::c_int
        | (*p.offset(i.wrapping_add(2 as libc::c_int as size_t) as isize) as uint32_t)
            << 16 as libc::c_int
        | (*p.offset(i.wrapping_add(3 as libc::c_int as size_t) as isize) as uint32_t)
            << 24 as libc::c_int;
}
unsafe extern "C" fn chacha_init(
    mut ctx: *mut mi_random_ctx_t,
    mut key: *const uint8_t,
    mut nonce: uint64_t,
) {
    memset(
        ctx as *mut libc::c_void,
        0 as libc::c_int,
        ::core::mem::size_of::<mi_random_ctx_t>() as libc::c_ulong,
    );
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < 4 as libc::c_int as size_t {
        let mut sigma: *const uint8_t =
            b"expand 32-byte k\0" as *const u8 as *const libc::c_char as *mut uint8_t;
        (*ctx).input[i as usize] = read32(sigma, i);
        i = i.wrapping_add(1);
        i;
    }
    let mut i_0: size_t = 0 as libc::c_int as size_t;
    while i_0 < 8 as libc::c_int as size_t {
        (*ctx).input[i_0.wrapping_add(4 as libc::c_int as size_t) as usize] = read32(key, i_0);
        i_0 = i_0.wrapping_add(1);
        i_0;
    }
    (*ctx).input[12 as libc::c_int as usize] = 0 as libc::c_int as uint32_t;
    (*ctx).input[13 as libc::c_int as usize] = 0 as libc::c_int as uint32_t;
    (*ctx).input[14 as libc::c_int as usize] = nonce as uint32_t;
    (*ctx).input[15 as libc::c_int as usize] = (nonce >> 32 as libc::c_int) as uint32_t;
}
unsafe extern "C" fn chacha_split(
    mut ctx: *mut mi_random_ctx_t,
    mut nonce: uint64_t,
    mut ctx_new: *mut mi_random_ctx_t,
) {
    memset(
        ctx_new as *mut libc::c_void,
        0 as libc::c_int,
        ::core::mem::size_of::<mi_random_ctx_t>() as libc::c_ulong,
    );
    _mi_memcpy(
        ((*ctx_new).input).as_mut_ptr() as *mut libc::c_void,
        ((*ctx).input).as_mut_ptr() as *const libc::c_void,
        ::core::mem::size_of::<[uint32_t; 16]>() as libc::c_ulong,
    );
    (*ctx_new).input[12 as libc::c_int as usize] = 0 as libc::c_int as uint32_t;
    (*ctx_new).input[13 as libc::c_int as usize] = 0 as libc::c_int as uint32_t;
    (*ctx_new).input[14 as libc::c_int as usize] = nonce as uint32_t;
    (*ctx_new).input[15 as libc::c_int as usize] = (nonce >> 32 as libc::c_int) as uint32_t;
    if (*ctx).input[14 as libc::c_int as usize] != (*ctx_new).input[14 as libc::c_int as usize]
        || (*ctx).input[15 as libc::c_int as usize] != (*ctx_new).input[15 as libc::c_int as usize]
    {
    } else {
        _mi_assert_fail(
            b"ctx->input[14] != ctx_new->input[14] || ctx->input[15] != ctx_new->input[15]\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8104 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 13], &[libc::c_char; 13]>(b"chacha_split\0")).as_ptr(),
        );
    };
    chacha_block(ctx_new);
}
unsafe extern "C" fn mi_random_is_initialized(mut ctx: *mut mi_random_ctx_t) -> bool {
    return !ctx.is_null()
        && (*ctx).input[0 as libc::c_int as usize] != 0 as libc::c_int as uint32_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_random_split(
    mut ctx: *mut mi_random_ctx_t,
    mut ctx_new: *mut mi_random_ctx_t,
) {
    if mi_random_is_initialized(ctx) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_random_is_initialized(ctx)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8113 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_random_split\0"))
                .as_ptr(),
        );
    };
    if ctx != ctx_new {
    } else {
        _mi_assert_fail(
            b"ctx != ctx_new\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8114 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"_mi_random_split\0"))
                .as_ptr(),
        );
    };
    chacha_split(ctx, ctx_new as uintptr_t, ctx_new);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_random_next(mut ctx: *mut mi_random_ctx_t) -> uintptr_t {
    if mi_random_is_initialized(ctx) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_random_is_initialized(ctx)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8118 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"_mi_random_next\0"))
                .as_ptr(),
        );
    };
    return (chacha_next32(ctx) as uintptr_t) << 32 as libc::c_int
        | chacha_next32(ctx) as uintptr_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_os_random_weak(mut extra_seed: uintptr_t) -> uintptr_t {
    let mut x: uintptr_t =
        ::core::mem::transmute::<Option<unsafe extern "C" fn(uintptr_t) -> uintptr_t>, uintptr_t>(
            Some(_mi_os_random_weak as unsafe extern "C" fn(uintptr_t) -> uintptr_t),
        ) ^ extra_seed;
    x ^= _mi_prim_clock_now() as uintptr_t;
    let mut max: uintptr_t = ((x ^ x >> 17 as libc::c_int) & 0xf as libc::c_int as uintptr_t)
        .wrapping_add(1 as libc::c_int as uintptr_t);
    let mut i: uintptr_t = 0 as libc::c_int as uintptr_t;
    while i < max {
        x = _mi_random_shuffle(x);
        i = i.wrapping_add(1);
        i;
    }
    if x != 0 as libc::c_int as uintptr_t {
    } else {
        _mi_assert_fail(
            b"x != 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8134 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"_mi_os_random_weak\0"))
                .as_ptr(),
        );
    };
    return x;
}
unsafe extern "C" fn mi_random_init_ex(mut ctx: *mut mi_random_ctx_t, mut use_weak: bool) {
    let mut key: [uint8_t; 32] = [0; 32];
    if use_weak as libc::c_int != 0
        || !_mi_prim_random_buf(
            key.as_mut_ptr() as *mut libc::c_void,
            ::core::mem::size_of::<[uint8_t; 32]>() as libc::c_ulong,
        )
    {
        if !use_weak {
            _mi_warning_message(
                b"unable to use secure randomness\n\0" as *const u8 as *const libc::c_char,
            );
        }
        let mut x: uintptr_t = _mi_os_random_weak(0 as libc::c_int as uintptr_t);
        let mut i: size_t = 0 as libc::c_int as size_t;
        while i < 8 as libc::c_int as size_t {
            x = _mi_random_shuffle(x);
            *(key.as_mut_ptr() as *mut uint32_t).offset(i as isize) = x as uint32_t;
            i = i.wrapping_add(1);
            i;
        }
        (*ctx).weak = 1 as libc::c_int != 0;
    } else {
        (*ctx).weak = 0 as libc::c_int != 0;
    }
    chacha_init(ctx, key.as_mut_ptr() as *const uint8_t, ctx as uintptr_t);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_random_init(mut ctx: *mut mi_random_ctx_t) {
    mi_random_init_ex(ctx, 0 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_random_init_weak(mut ctx: *mut mi_random_ctx_t) {
    mi_random_init_ex(ctx, 1 as libc::c_int != 0);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_random_reinit_if_weak(mut ctx: *mut mi_random_ctx_t) {
    if (*ctx).weak {
        _mi_random_init(ctx);
    }
}
unsafe extern "C" fn mi_segment_queue_remove(
    mut queue: *mut mi_segment_queue_t,
    mut segment: *mut mi_segment_t,
) {
    if !((*segment).prev).is_null() {
        (*(*segment).prev).next = (*segment).next;
    }
    if !((*segment).next).is_null() {
        (*(*segment).next).prev = (*segment).prev;
    }
    if segment == (*queue).first {
        (*queue).first = (*segment).next;
    }
    if segment == (*queue).last {
        (*queue).last = (*segment).prev;
    }
    (*segment).next = 0 as *mut mi_segment_s;
    (*segment).prev = 0 as *mut mi_segment_s;
}
unsafe extern "C" fn mi_segment_enqueue(
    mut queue: *mut mi_segment_queue_t,
    mut segment: *mut mi_segment_t,
) {
    (*segment).next = 0 as *mut mi_segment_s;
    (*segment).prev = (*queue).last;
    if !((*queue).last).is_null() {
        if ((*(*queue).last).next).is_null() {
        } else {
            _mi_assert_fail(
                b"queue->last->next == NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8197 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_segment_enqueue\0",
                ))
                .as_ptr(),
            );
        };
        (*(*queue).last).next = segment;
        (*queue).last = segment;
    } else {
        (*queue).first = segment;
        (*queue).last = (*queue).first;
    };
}
unsafe extern "C" fn mi_segment_free_queue_of_kind(
    mut kind: mi_page_kind_t,
    mut tld: *mut mi_segments_tld_t,
) -> *mut mi_segment_queue_t {
    if kind as libc::c_uint == MI_PAGE_SMALL as libc::c_int as libc::c_uint {
        return &mut (*tld).small_free;
    } else if kind as libc::c_uint == MI_PAGE_MEDIUM as libc::c_int as libc::c_uint {
        return &mut (*tld).medium_free;
    } else {
        return 0 as *mut mi_segment_queue_t;
    };
}
unsafe extern "C" fn mi_segment_free_queue(
    mut segment: *const mi_segment_t,
    mut tld: *mut mi_segments_tld_t,
) -> *mut mi_segment_queue_t {
    return mi_segment_free_queue_of_kind((*segment).page_kind, tld);
}
unsafe extern "C" fn mi_segment_remove_from_free_queue(
    mut segment: *mut mi_segment_t,
    mut tld: *mut mi_segments_tld_t,
) {
    let mut queue: *mut mi_segment_queue_t = mi_segment_free_queue(segment, tld);
    let mut in_queue: bool = !queue.is_null()
        && (!((*segment).next).is_null()
            || !((*segment).prev).is_null()
            || (*queue).first == segment);
    if in_queue {
        mi_segment_queue_remove(queue, segment);
    }
}
unsafe extern "C" fn mi_segment_insert_in_free_queue(
    mut segment: *mut mi_segment_t,
    mut tld: *mut mi_segments_tld_t,
) {
    mi_segment_enqueue(mi_segment_free_queue(segment, tld), segment);
}
unsafe extern "C" fn mi_segment_page_size(mut segment: *const mi_segment_t) -> size_t {
    if (*segment).capacity > 1 as libc::c_int as size_t {
        if (*segment).page_kind as libc::c_uint <= MI_PAGE_MEDIUM as libc::c_int as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"segment->page_kind <= MI_PAGE_MEDIUM\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8226 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"mi_segment_page_size\0",
                ))
                .as_ptr(),
            );
        };
        return (1 as libc::c_int as size_t) << (*segment).page_shift;
    } else {
        if (*segment).page_kind as libc::c_uint >= MI_PAGE_LARGE as libc::c_int as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"segment->page_kind >= MI_PAGE_LARGE\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8230 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"mi_segment_page_size\0",
                ))
                .as_ptr(),
            );
        };
        return (*segment).segment_size;
    };
}
unsafe extern "C" fn mi_pages_purge_contains(
    mut page: *const mi_page_t,
    mut tld: *mut mi_segments_tld_t,
) -> bool {
    let mut p: *mut mi_page_t = (*tld).pages_purge.first;
    while !p.is_null() {
        if p == page as *mut mi_page_t {
            return 1 as libc::c_int != 0;
        }
        p = (*p).next;
    }
    return 0 as libc::c_int != 0;
}
unsafe extern "C" fn mi_page_not_in_queue(
    mut page: *const mi_page_t,
    mut tld: *mut mi_segments_tld_t,
) -> bool {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8270 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_page_not_in_queue\0"))
                .as_ptr(),
        );
    };
    if !((*page).next).is_null() || !((*page).prev).is_null() {
        if mi_pages_purge_contains(page, tld) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"mi_pages_purge_contains(page, tld)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8272 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"mi_page_not_in_queue\0",
                ))
                .as_ptr(),
            );
        };
        return 0 as libc::c_int != 0;
    } else {
        return (*tld).pages_purge.first != page as *mut mi_page_t
            && (*tld).pages_purge.last != page as *mut mi_page_t;
    };
}
unsafe extern "C" fn mi_segment_protect_range(
    mut p: *mut libc::c_void,
    mut size: size_t,
    mut protect: bool,
) {
    if protect {
        _mi_os_protect(p, size);
    } else {
        _mi_os_unprotect(p, size);
    };
}
unsafe extern "C" fn mi_segment_protect(
    mut segment: *mut mi_segment_t,
    mut protect: bool,
    mut tld: *mut mi_os_tld_t,
) {
    if 0 as libc::c_int != 0 as libc::c_int {
        let os_psize: size_t = _mi_os_page_size();
        if ((*segment).segment_info_size).wrapping_sub(os_psize)
            >= (::core::mem::size_of::<mi_segment_t>() as libc::c_ulong).wrapping_add(
                ((*segment).capacity)
                    .wrapping_sub(1 as libc::c_int as size_t)
                    .wrapping_mul(::core::mem::size_of::<mi_page_t>() as libc::c_ulong),
            )
        {
        } else {
            _mi_assert_fail(
                b"(segment->segment_info_size - os_psize) >= (sizeof(mi_segment_t) + ((segment->capacity - 1) * sizeof(mi_page_t)))\0"
                    as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8290 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<
                    &[u8; 19],
                    &[libc::c_char; 19],
                >(b"mi_segment_protect\0"))
                    .as_ptr(),
            );
        };
        if (segment as uintptr_t)
            .wrapping_add((*segment).segment_info_size)
            .wrapping_rem(os_psize)
            == 0 as libc::c_int as libc::c_ulong
        {
        } else {
            _mi_assert_fail(
                b"((uintptr_t)segment + segment->segment_info_size) % os_psize == 0\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8291 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_segment_protect\0",
                ))
                .as_ptr(),
            );
        };
        mi_segment_protect_range(
            (segment as *mut uint8_t)
                .offset((*segment).segment_info_size as isize)
                .offset(-(os_psize as isize)) as *mut libc::c_void,
            os_psize,
            protect,
        );
        if 0 as libc::c_int <= 1 as libc::c_int
            || (*segment).page_kind as libc::c_uint >= MI_PAGE_LARGE as libc::c_int as libc::c_uint
        {
        } else {
            _mi_assert_fail(
                b"MI_SECURE <= 1 || segment->page_kind >= MI_PAGE_LARGE\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8297 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_segment_protect\0",
                ))
                .as_ptr(),
            );
        };
        let mut start: *mut uint8_t = (segment as *mut uint8_t)
            .offset((*segment).segment_size as isize)
            .offset(-(os_psize as isize));
        if protect as libc::c_int != 0 && !(*segment).memid.initially_committed {
            if protect {
                if _mi_os_commit(
                    start as *mut libc::c_void,
                    os_psize,
                    0 as *mut bool,
                    (*tld).stats,
                ) {
                    mi_segment_protect_range(start as *mut libc::c_void, os_psize, protect);
                }
            }
        } else {
            mi_segment_protect_range(start as *mut libc::c_void, os_psize, protect);
        }
    }
}
unsafe extern "C" fn mi_page_purge(
    mut segment: *mut mi_segment_t,
    mut page: *mut mi_page_t,
    mut tld: *mut mi_segments_tld_t,
) {
    if (*page).is_committed() as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"page->is_committed\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8323 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_page_purge\0"))
                .as_ptr(),
        );
    };
    if (*page).segment_in_use() == 0 {
    } else {
        _mi_assert_fail(
            b"!page->segment_in_use\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8324 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_page_purge\0"))
                .as_ptr(),
        );
    };
    if !(*segment).allow_purge {
        return;
    }
    if (*page).used as libc::c_int == 0 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"page->used == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8326 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_page_purge\0"))
                .as_ptr(),
        );
    };
    if ((*page).free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8327 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 14], &[libc::c_char; 14]>(b"mi_page_purge\0"))
                .as_ptr(),
        );
    };
    let mut psize: size_t = 0;
    let mut start: *mut libc::c_void =
        mi_segment_raw_page_start(segment, page, &mut psize) as *mut libc::c_void;
    let needs_recommit: bool = _mi_os_purge(start, psize, (*tld).stats);
    if needs_recommit {
        (*page).set_is_committed(0 as libc::c_int as uint8_t);
    }
}
unsafe extern "C" fn mi_page_ensure_committed(
    mut segment: *mut mi_segment_t,
    mut page: *mut mi_page_t,
    mut tld: *mut mi_segments_tld_t,
) -> bool {
    if (*page).is_committed() != 0 {
        return 1 as libc::c_int != 0;
    }
    if (*segment).allow_decommit as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"segment->allow_decommit\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8336 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_page_ensure_committed\0",
            ))
            .as_ptr(),
        );
    };
    let mut psize: size_t = 0;
    let mut start: *mut uint8_t = mi_segment_raw_page_start(segment, page, &mut psize);
    let mut is_zero: bool = 0 as libc::c_int != 0;
    let gsize: size_t = if 0 as libc::c_int >= 2 as libc::c_int {
        _mi_os_page_size()
    } else {
        0 as libc::c_int as size_t
    };
    let mut ok: bool = _mi_os_commit(
        start as *mut libc::c_void,
        psize.wrapping_add(gsize),
        &mut is_zero,
        (*tld).stats,
    );
    if !ok {
        return 0 as libc::c_int != 0;
    }
    (*page).set_is_committed(1 as libc::c_int as uint8_t);
    (*page).used = 0 as libc::c_int as uint16_t;
    (*page).free = 0 as *mut mi_block_t;
    (*page).set_is_zero_init(is_zero as uint8_t);
    if gsize > 0 as libc::c_int as size_t {
        mi_segment_protect_range(
            start.offset(psize as isize) as *mut libc::c_void,
            gsize,
            1 as libc::c_int != 0,
        );
    }
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn mi_page_get_expire(mut page: *mut mi_page_t) -> uint32_t {
    return (*page).free as uintptr_t as uint32_t;
}
unsafe extern "C" fn mi_page_set_expire(mut page: *mut mi_page_t, mut expire: uint32_t) {
    (*page).free = expire as uintptr_t as *mut mi_block_t;
}
unsafe extern "C" fn mi_page_purge_set_expire(mut page: *mut mi_page_t) {
    if mi_page_get_expire(page) == 0 as libc::c_int as uint32_t {
    } else {
        _mi_assert_fail(
            b"mi_page_get_expire(page)==0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8360 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_page_purge_set_expire\0",
            ))
            .as_ptr(),
        );
    };
    let mut expire: uint32_t = (_mi_clock_now() as uint32_t as libc::c_long
        + mi_option_get(mi_option_purge_delay)) as uint32_t;
    mi_page_set_expire(page, expire);
}
unsafe extern "C" fn mi_page_purge_is_expired(
    mut page: *mut mi_page_t,
    mut now: mi_msecs_t,
) -> bool {
    let mut expire: int32_t = mi_page_get_expire(page) as int32_t;
    return now as int32_t - expire >= 0 as libc::c_int;
}
unsafe extern "C" fn mi_segment_schedule_purge(
    mut segment: *mut mi_segment_t,
    mut page: *mut mi_page_t,
    mut tld: *mut mi_segments_tld_t,
) {
    if (*page).segment_in_use() == 0 {
    } else {
        _mi_assert_fail(
            b"!page->segment_in_use\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8369 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_segment_schedule_purge\0",
            ))
            .as_ptr(),
        );
    };
    if mi_page_not_in_queue(page, tld) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_not_in_queue(page,tld)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8370 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_segment_schedule_purge\0",
            ))
            .as_ptr(),
        );
    };
    if _mi_page_segment(page) == segment {
    } else {
        _mi_assert_fail(
            b"_mi_page_segment(page)==segment\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8372 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_segment_schedule_purge\0",
            ))
            .as_ptr(),
        );
    };
    if !(*segment).allow_purge {
        return;
    }
    if mi_option_get(mi_option_purge_delay) == 0 as libc::c_int as libc::c_long {
        mi_page_purge(segment, page, tld);
    } else if mi_option_get(mi_option_purge_delay) > 0 as libc::c_int as libc::c_long {
        let mut pq: *mut mi_page_queue_t = &mut (*tld).pages_purge;
        mi_page_purge_set_expire(page);
        (*page).next = (*pq).first;
        (*page).prev = 0 as *mut mi_page_s;
        if ((*pq).first).is_null() {
            if ((*pq).last).is_null() {
            } else {
                _mi_assert_fail(
                    b"pq->last == NULL\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    8383 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                        b"mi_segment_schedule_purge\0",
                    ))
                    .as_ptr(),
                );
            };
            (*pq).last = page;
            (*pq).first = (*pq).last;
        } else {
            (*(*pq).first).prev = page;
            (*pq).first = page;
        }
    }
}
unsafe extern "C" fn mi_page_purge_remove(
    mut page: *mut mi_page_t,
    mut tld: *mut mi_segments_tld_t,
) {
    if mi_page_not_in_queue(page, tld) {
        return;
    }
    let mut pq: *mut mi_page_queue_t = &mut (*tld).pages_purge;
    if !pq.is_null() {
    } else {
        _mi_assert_fail(
            b"pq!=NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8395 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_page_purge_remove\0"))
                .as_ptr(),
        );
    };
    if (*page).segment_in_use() == 0 {
    } else {
        _mi_assert_fail(
            b"!page->segment_in_use\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8396 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_page_purge_remove\0"))
                .as_ptr(),
        );
    };
    if mi_page_get_expire(page) != 0 as libc::c_int as uint32_t {
    } else {
        _mi_assert_fail(
            b"mi_page_get_expire(page) != 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8397 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_page_purge_remove\0"))
                .as_ptr(),
        );
    };
    if mi_pages_purge_contains(page, tld) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_pages_purge_contains(page, tld)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8398 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_page_purge_remove\0"))
                .as_ptr(),
        );
    };
    if !((*page).prev).is_null() {
        (*(*page).prev).next = (*page).next;
    }
    if !((*page).next).is_null() {
        (*(*page).next).prev = (*page).prev;
    }
    if page == (*pq).last {
        (*pq).last = (*page).prev;
    }
    if page == (*pq).first {
        (*pq).first = (*page).next;
    }
    (*page).prev = 0 as *mut mi_page_s;
    (*page).next = (*page).prev;
    mi_page_set_expire(page, 0 as libc::c_int as uint32_t);
}
unsafe extern "C" fn mi_segment_remove_all_purges(
    mut segment: *mut mi_segment_t,
    mut force_purge: bool,
    mut tld: *mut mi_segments_tld_t,
) {
    if (*segment).memid.is_pinned {
        return;
    }
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < (*segment).capacity {
        let mut page: *mut mi_page_t =
            &mut *((*segment).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_t;
        if (*page).segment_in_use() == 0 {
            mi_page_purge_remove(page, tld);
            if force_purge as libc::c_int != 0 && (*page).is_committed() as libc::c_int != 0 {
                mi_page_purge(segment, page, tld);
            }
        } else {
            if mi_page_not_in_queue(page, tld) as libc::c_int != 0 {
            } else {
                _mi_assert_fail(
                    b"mi_page_not_in_queue(page,tld)\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    8417 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 29], &[libc::c_char; 29]>(
                        b"mi_segment_remove_all_purges\0",
                    ))
                    .as_ptr(),
                );
            };
        }
        i = i.wrapping_add(1);
        i;
    }
}
unsafe extern "C" fn mi_pages_try_purge(mut force: bool, mut tld: *mut mi_segments_tld_t) {
    if mi_option_get(mi_option_purge_delay) < 0 as libc::c_int as libc::c_long {
        return;
    }
    let mut now: mi_msecs_t = _mi_clock_now();
    let mut pq: *mut mi_page_queue_t = &mut (*tld).pages_purge;
    let mut page: *mut mi_page_t = (*pq).last;
    while !page.is_null()
        && (force as libc::c_int != 0 || mi_page_purge_is_expired(page, now) as libc::c_int != 0)
    {
        let prev: *mut mi_page_t = (*page).prev;
        mi_page_purge_remove(page, tld);
        mi_page_purge(_mi_page_segment(page), page, tld);
        page = prev;
    }
    (*pq).last = page;
    if !page.is_null() {
        (*page).next = 0 as *mut mi_page_s;
    } else {
        (*pq).first = 0 as *mut mi_page_t;
    };
}
unsafe extern "C" fn mi_segment_raw_page_size(mut segment: *const mi_segment_t) -> size_t {
    return if (*segment).page_kind as libc::c_uint == MI_PAGE_HUGE as libc::c_int as libc::c_uint {
        (*segment).segment_size
    } else {
        (1 as libc::c_int as size_t) << (*segment).page_shift
    };
}
unsafe extern "C" fn mi_segment_raw_page_start(
    mut segment: *const mi_segment_t,
    mut page: *const mi_page_t,
    mut page_size: *mut size_t,
) -> *mut uint8_t {
    let mut psize: size_t = mi_segment_raw_page_size(segment);
    let mut p: *mut uint8_t =
        (segment as *mut uint8_t).offset(((*page).segment_idx as size_t * psize) as isize);
    if (*page).segment_idx as libc::c_int == 0 as libc::c_int {
        p = p.offset((*segment).segment_info_size as isize);
        psize = psize.wrapping_sub((*segment).segment_info_size);
    }
    if !page_size.is_null() {
        *page_size = psize;
    }
    if (*page).block_size == 0 as libc::c_int as size_t
        || _mi_ptr_page(p as *mut libc::c_void) == page as *mut mi_page_t
    {
    } else {
        _mi_assert_fail(
            b"page->block_size == 0 || _mi_ptr_page(p) == page\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8458 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_segment_raw_page_start\0",
            ))
            .as_ptr(),
        );
    };
    if _mi_ptr_segment(p as *const libc::c_void) == segment as *mut mi_segment_t {
    } else {
        _mi_assert_fail(
            b"_mi_ptr_segment(p) == segment\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8459 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 26], &[libc::c_char; 26]>(
                b"mi_segment_raw_page_start\0",
            ))
            .as_ptr(),
        );
    };
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segment_page_start(
    mut segment: *const mi_segment_t,
    mut page: *const mi_page_t,
    mut page_size: *mut size_t,
) -> *mut uint8_t {
    let mut psize: size_t = 0;
    let mut p: *mut uint8_t = mi_segment_raw_page_start(segment, page, &mut psize);
    let block_size: size_t = mi_page_block_size(page);
    if block_size > 0 as libc::c_int as size_t
        && block_size as libc::c_ulonglong
            <= ((1 as libc::c_ulonglong)
                << 3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
                .wrapping_div(4 as libc::c_int as libc::c_ulonglong)
    {
        if (*segment).page_kind as libc::c_uint <= MI_PAGE_MEDIUM as libc::c_int as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"segment->page_kind <= MI_PAGE_MEDIUM\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8468 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                    b"_mi_segment_page_start\0",
                ))
                .as_ptr(),
            );
        };
        let mut adjust: size_t = block_size.wrapping_sub((p as uintptr_t).wrapping_rem(block_size));
        if adjust < block_size && psize >= block_size.wrapping_add(adjust) {
            p = p.offset(adjust as isize);
            psize = psize.wrapping_sub(adjust);
            if (p as uintptr_t).wrapping_rem(block_size) == 0 as libc::c_int as libc::c_ulong {
            } else {
                _mi_assert_fail(
                    b"(uintptr_t)p % block_size == 0\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    8473 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                        b"_mi_segment_page_start\0",
                    ))
                    .as_ptr(),
                );
            };
        }
    }
    if _mi_is_aligned(p as *mut libc::c_void, 16 as libc::c_int as size_t) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"_mi_is_aligned(p, MI_MAX_ALIGN_SIZE)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8476 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"_mi_segment_page_start\0",
            ))
            .as_ptr(),
        );
    };
    if block_size == 0 as libc::c_int as size_t
        || block_size as libc::c_ulonglong
            > ((1 as libc::c_ulonglong)
                << 3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
                .wrapping_div(4 as libc::c_int as libc::c_ulonglong)
        || _mi_is_aligned(p as *mut libc::c_void, block_size) as libc::c_int != 0
    {
    } else {
        _mi_assert_fail(
            b"block_size == 0 || block_size > MI_MAX_ALIGN_GUARANTEE || _mi_is_aligned(p,block_size)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8477 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 23],
                &[libc::c_char; 23],
            >(b"_mi_segment_page_start\0"))
                .as_ptr(),
        );
    };
    if !page_size.is_null() {
        *page_size = psize;
    }
    if _mi_ptr_page(p as *mut libc::c_void) == page as *mut mi_page_t {
    } else {
        _mi_assert_fail(
            b"_mi_ptr_page(p) == page\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8479 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"_mi_segment_page_start\0",
            ))
            .as_ptr(),
        );
    };
    if _mi_ptr_segment(p as *const libc::c_void) == segment as *mut mi_segment_t {
    } else {
        _mi_assert_fail(
            b"_mi_ptr_segment(p) == segment\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8480 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"_mi_segment_page_start\0",
            ))
            .as_ptr(),
        );
    };
    return p;
}
unsafe extern "C" fn mi_segment_calculate_sizes(
    mut capacity: size_t,
    mut required: size_t,
    mut pre_size: *mut size_t,
    mut info_size: *mut size_t,
) -> size_t {
    let minsize: size_t = (::core::mem::size_of::<mi_segment_t>() as libc::c_ulong)
        .wrapping_add(
            capacity
                .wrapping_sub(1 as libc::c_int as size_t)
                .wrapping_mul(::core::mem::size_of::<mi_page_t>() as libc::c_ulong),
        )
        .wrapping_add(16 as libc::c_int as libc::c_ulong);
    let mut guardsize: size_t = 0 as libc::c_int as size_t;
    let mut isize: size_t = 0 as libc::c_int as size_t;
    if 0 as libc::c_int == 0 as libc::c_int {
        isize = _mi_align_up(minsize, (16 as libc::c_int * 16 as libc::c_int) as size_t);
    } else {
        let page_size: size_t = _mi_os_page_size();
        isize = _mi_align_up(minsize, page_size);
        guardsize = page_size;
    }
    if !info_size.is_null() {
        *info_size = isize;
    }
    if !pre_size.is_null() {
        *pre_size = isize.wrapping_add(guardsize);
    }
    return (if required == 0 as libc::c_int as size_t {
        (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
    } else {
        _mi_align_up(
            required
                .wrapping_add(isize)
                .wrapping_add(2 as libc::c_int as size_t * guardsize),
            (256 as libc::c_int * 1024 as libc::c_int) as size_t,
        ) as libc::c_ulonglong
    }) as size_t;
}
unsafe extern "C" fn mi_segments_track_size(
    mut segment_size: libc::c_long,
    mut tld: *mut mi_segments_tld_t,
) {
    if segment_size >= 0 as libc::c_int as libc::c_long {
        _mi_stat_increase(&mut (*(*tld).stats).segments, 1 as libc::c_int as size_t);
    } else {
        _mi_stat_decrease(&mut (*(*tld).stats).segments, 1 as libc::c_int as size_t);
    }
    (*tld).count = ((*tld).count).wrapping_add(
        (if segment_size >= 0 as libc::c_int as libc::c_long {
            1 as libc::c_int
        } else {
            -(1 as libc::c_int)
        }) as size_t,
    );
    if (*tld).count > (*tld).peak_count {
        (*tld).peak_count = (*tld).count;
    }
    (*tld).current_size = ((*tld).current_size).wrapping_add(segment_size as size_t);
    if (*tld).current_size > (*tld).peak_size {
        (*tld).peak_size = (*tld).current_size;
    }
}
unsafe extern "C" fn mi_segment_os_free(
    mut segment: *mut mi_segment_t,
    mut segment_size: size_t,
    mut tld: *mut mi_segments_tld_t,
) {
    (*segment).thread_id = 0 as libc::c_int as libc::c_ulong;
    _mi_segment_map_freed_at(segment);
    mi_segments_track_size(-(segment_size as libc::c_long), tld);
    if (*segment).was_reclaimed {
        (*tld).reclaim_count = ((*tld).reclaim_count).wrapping_sub(1);
        (*tld).reclaim_count;
        (*segment).was_reclaimed = 0 as libc::c_int != 0;
    }
    if 0 as libc::c_int != 0 as libc::c_int {
        if !(*segment).memid.is_pinned {
        } else {
            _mi_assert_fail(
                b"!segment->memid.is_pinned\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8521 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_segment_os_free\0",
                ))
                .as_ptr(),
            );
        };
        mi_segment_protect(segment, 0 as libc::c_int != 0, (*tld).os);
    }
    let mut fully_committed: bool = 1 as libc::c_int != 0;
    let mut committed_size: size_t = 0 as libc::c_int as size_t;
    let page_size: size_t = mi_segment_raw_page_size(segment);
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < (*segment).capacity {
        let mut page: *mut mi_page_t =
            &mut *((*segment).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_t;
        if (*page).is_committed() != 0 {
            committed_size = committed_size.wrapping_add(page_size);
        }
        if (*page).is_committed() == 0 {
            fully_committed = 0 as libc::c_int != 0;
        }
        i = i.wrapping_add(1);
        i;
    }
    if fully_committed as libc::c_int != 0 && committed_size == segment_size
        || !fully_committed && committed_size < segment_size
    {
    } else {
        _mi_assert_fail(
            b"(fully_committed && committed_size == segment_size) || (!fully_committed && committed_size < segment_size)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8533 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 19],
                &[libc::c_char; 19],
            >(b"mi_segment_os_free\0"))
                .as_ptr(),
        );
    };
    _mi_arena_free(
        segment as *mut libc::c_void,
        segment_size,
        committed_size,
        (*segment).memid,
        (*tld).stats,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segments_collect(mut force: bool, mut tld: *mut mi_segments_tld_t) {
    mi_pages_try_purge(force, tld);
    if !_mi_is_main_thread() {
        if ((*tld).pages_purge.first).is_null() {
        } else {
            _mi_assert_fail(
                b"tld->pages_purge.first == NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8540 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"_mi_segments_collect\0",
                ))
                .as_ptr(),
            );
        };
        if ((*tld).pages_purge.last).is_null() {
        } else {
            _mi_assert_fail(
                b"tld->pages_purge.last == NULL\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8541 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(
                    b"_mi_segments_collect\0",
                ))
                .as_ptr(),
            );
        };
    }
}
unsafe extern "C" fn mi_segment_os_alloc(
    mut eager_delayed: bool,
    mut page_alignment: size_t,
    mut req_arena_id: mi_arena_id_t,
    mut pre_size: size_t,
    mut info_size: size_t,
    mut commit: bool,
    mut segment_size: size_t,
    mut tld: *mut mi_segments_tld_t,
    mut tld_os: *mut mi_os_tld_t,
) -> *mut mi_segment_t {
    let mut memid: mi_memid_t = mi_memid_s {
        mem: C2RustUnnamed_0 {
            os: mi_memid_os_info {
                base: 0 as *mut libc::c_void,
                alignment: 0,
            },
        },
        is_pinned: false,
        initially_committed: false,
        initially_zero: false,
        memkind: MI_MEM_NONE,
    };
    let mut allow_large: bool = !eager_delayed && 0 as libc::c_int == 0 as libc::c_int;
    let mut align_offset: size_t = 0 as libc::c_int as size_t;
    let mut alignment: size_t = ((1 as libc::c_ulonglong)
        << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
        as size_t;
    if page_alignment > 0 as libc::c_int as size_t {
        alignment = page_alignment;
        align_offset = _mi_align_up(
            pre_size,
            ((1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                as size_t,
        );
        segment_size = segment_size.wrapping_add(align_offset.wrapping_sub(pre_size));
    }
    let mut segment: *mut mi_segment_t = _mi_arena_alloc_aligned(
        segment_size,
        alignment,
        align_offset,
        commit,
        allow_large,
        req_arena_id,
        &mut memid,
        tld_os,
    ) as *mut mi_segment_t;
    if segment.is_null() {
        return 0 as *mut mi_segment_t;
    }
    if !memid.initially_committed {
        if !memid.is_pinned {
        } else {
            _mi_assert_fail(
                b"!memid.is_pinned\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8563 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 20], &[libc::c_char; 20]>(
                    b"mi_segment_os_alloc\0",
                ))
                .as_ptr(),
            );
        };
        let mut ok: bool = _mi_os_commit(
            segment as *mut libc::c_void,
            pre_size,
            0 as *mut bool,
            (*tld_os).stats,
        );
        if !ok {
            _mi_arena_free(
                segment as *mut libc::c_void,
                segment_size,
                0 as libc::c_int as size_t,
                memid,
                (*tld_os).stats,
            );
            return 0 as *mut mi_segment_t;
        }
    }
    (*segment).memid = memid;
    (*segment).allow_decommit = !memid.is_pinned;
    (*segment).allow_purge = (*segment).allow_decommit as libc::c_int != 0
        && mi_option_get(mi_option_purge_delay) >= 0 as libc::c_int as libc::c_long;
    (*segment).segment_size = segment_size;
    (*segment).subproc = (*tld).subproc;
    mi_segments_track_size(segment_size as libc::c_long, tld);
    _mi_segment_map_allocated_at(segment);
    return segment;
}
unsafe extern "C" fn mi_segment_alloc(
    mut required: size_t,
    mut page_kind: mi_page_kind_t,
    mut page_shift: size_t,
    mut page_alignment: size_t,
    mut req_arena_id: mi_arena_id_t,
    mut tld: *mut mi_segments_tld_t,
    mut os_tld: *mut mi_os_tld_t,
) -> *mut mi_segment_t {
    if required > 0 as libc::c_int as size_t
        && page_kind as libc::c_uint > MI_PAGE_LARGE as libc::c_int as libc::c_uint
        || required == 0 as libc::c_int as size_t
            && page_kind as libc::c_uint <= MI_PAGE_LARGE as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"(required > 0 && page_kind > MI_PAGE_LARGE)|| (required==0 && page_kind <= MI_PAGE_LARGE)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8583 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 17],
                &[libc::c_char; 17],
            >(b"mi_segment_alloc\0"))
                .as_ptr(),
        );
    };
    let mut capacity: size_t = 0;
    if page_kind as libc::c_uint == MI_PAGE_HUGE as libc::c_int as libc::c_uint {
        if page_shift
            == (3 as libc::c_int
                + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
                + 1 as libc::c_int) as size_t
            && required > 0 as libc::c_int as size_t
        {
        } else {
            _mi_assert_fail(
                b"page_shift == MI_SEGMENT_SHIFT + 1 && required > 0\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8586 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_segment_alloc\0"))
                    .as_ptr(),
            );
        };
        capacity = 1 as libc::c_int as size_t;
    } else {
        if required == 0 as libc::c_int as size_t && page_alignment == 0 as libc::c_int as size_t {
        } else {
            _mi_assert_fail(
                b"required == 0 && page_alignment == 0\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8590 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_segment_alloc\0"))
                    .as_ptr(),
            );
        };
        let mut page_size: size_t = (1 as libc::c_int as size_t) << page_shift;
        capacity = ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(page_size as libc::c_ulonglong) as size_t;
        if ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_rem(page_size as libc::c_ulonglong)
            == 0 as libc::c_int as libc::c_ulonglong
        {
        } else {
            _mi_assert_fail(
                b"MI_SEGMENT_SIZE % page_size == 0\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8593 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_segment_alloc\0"))
                    .as_ptr(),
            );
        };
        if capacity >= 1 as libc::c_int as size_t
            && capacity as libc::c_ulonglong
                <= ((1 as libc::c_ulonglong)
                    << 3 as libc::c_int
                        + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                    .wrapping_div((1 as libc::c_ulonglong) << 13 as libc::c_int + 3 as libc::c_int)
        {
        } else {
            _mi_assert_fail(
                b"capacity >= 1 && capacity <= MI_SMALL_PAGES_PER_SEGMENT\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8594 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_segment_alloc\0"))
                    .as_ptr(),
            );
        };
    }
    let mut info_size: size_t = 0;
    let mut pre_size: size_t = 0;
    let init_segment_size: size_t =
        mi_segment_calculate_sizes(capacity, required, &mut pre_size, &mut info_size);
    if init_segment_size >= required {
    } else {
        _mi_assert_fail(
            b"init_segment_size >= required\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8599 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_segment_alloc\0"))
                .as_ptr(),
        );
    };
    let eager_delayed: bool = page_kind as libc::c_uint
        <= MI_PAGE_MEDIUM as libc::c_int as libc::c_uint
        && _mi_current_thread_count() > 1 as libc::c_int as size_t
        && (*tld).peak_count < mi_option_get(mi_option_eager_commit_delay) as size_t;
    let eager: bool =
        !eager_delayed && mi_option_is_enabled(mi_option_eager_commit) as libc::c_int != 0;
    let init_commit: bool = eager;
    let mut segment: *mut mi_segment_t = mi_segment_os_alloc(
        eager_delayed,
        page_alignment,
        req_arena_id,
        pre_size,
        info_size,
        init_commit,
        init_segment_size,
        tld,
        os_tld,
    );
    if segment.is_null() {
        return 0 as *mut mi_segment_t;
    }
    if !segment.is_null()
        && (segment as uintptr_t as libc::c_ulonglong).wrapping_rem(
            (1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
        ) == 0 as libc::c_int as libc::c_ulonglong
    {
    } else {
        _mi_assert_fail(
            b"segment != NULL && (uintptr_t)segment % MI_SEGMENT_SIZE == 0\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8607 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_segment_alloc\0"))
                .as_ptr(),
        );
    };
    if if (*segment).memid.is_pinned as libc::c_int != 0 {
        (*segment).memid.initially_committed as libc::c_int
    } else {
        1 as libc::c_int
    } != 0
    {
    } else {
        _mi_assert_fail(
            b"segment->memid.is_pinned ? segment->memid.initially_committed : true\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8608 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_segment_alloc\0"))
                .as_ptr(),
        );
    };
    let mut ofs: ptrdiff_t = 48 as libc::c_ulong as ptrdiff_t;
    _mi_memzero(
        (segment as *mut uint8_t).offset(ofs as isize) as *mut libc::c_void,
        info_size.wrapping_sub(ofs as size_t),
    );
    let is_huge: bool = page_kind as libc::c_uint == MI_PAGE_HUGE as libc::c_int as libc::c_uint;
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < capacity {
        if i <= 255 as libc::c_int as size_t {
        } else {
            _mi_assert_fail(
                b"i <= 255\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8613 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 17], &[libc::c_char; 17]>(b"mi_segment_alloc\0"))
                    .as_ptr(),
            );
        };
        (*((*segment).pages).as_mut_ptr().offset(i as isize)).segment_idx = i as uint8_t;
        let ref mut fresh44 = *((*segment).pages).as_mut_ptr().offset(i as isize);
        (*fresh44).set_is_committed((*segment).memid.initially_committed as uint8_t);
        let ref mut fresh45 = *((*segment).pages).as_mut_ptr().offset(i as isize);
        (*fresh45).set_is_zero_init((*segment).memid.initially_zero as uint8_t);
        let ref mut fresh46 = *((*segment).pages).as_mut_ptr().offset(i as isize);
        (*fresh46).set_is_huge(is_huge as uint8_t);
        i = i.wrapping_add(1);
        i;
    }
    (*segment).page_kind = page_kind;
    (*segment).capacity = capacity;
    (*segment).page_shift = page_shift;
    (*segment).segment_info_size = pre_size;
    (*segment).thread_id = _mi_thread_id();
    (*segment).cookie = _mi_ptr_cookie(segment as *const libc::c_void);
    mi_segment_protect(segment, 1 as libc::c_int != 0, (*tld).os);
    if page_kind as libc::c_uint <= MI_PAGE_MEDIUM as libc::c_int as libc::c_uint {
        mi_segment_insert_in_free_queue(segment, tld);
    }
    return segment;
}
unsafe extern "C" fn mi_segment_free(
    mut segment: *mut mi_segment_t,
    mut force: bool,
    mut tld: *mut mi_segments_tld_t,
) {
    if !segment.is_null() {
    } else {
        _mi_assert_fail(
            b"segment != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8633 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_segment_free\0"))
                .as_ptr(),
        );
    };
    if (*segment).dont_free {
        return;
    }
    mi_segment_remove_all_purges(segment, 0 as libc::c_int != 0, tld);
    mi_segment_remove_from_free_queue(segment, tld);
    if ((*segment).next).is_null() {
    } else {
        _mi_assert_fail(
            b"segment->next == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8639 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_segment_free\0"))
                .as_ptr(),
        );
    };
    if ((*segment).prev).is_null() {
    } else {
        _mi_assert_fail(
            b"segment->prev == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8640 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_segment_free\0"))
                .as_ptr(),
        );
    };
    _mi_stat_decrease(
        &mut (*(*tld).stats).page_committed,
        (*segment).segment_info_size,
    );
    mi_segment_os_free(segment, (*segment).segment_size, tld);
}
unsafe extern "C" fn mi_segment_has_free(mut segment: *const mi_segment_t) -> bool {
    return (*segment).used < (*segment).capacity;
}
unsafe extern "C" fn mi_segment_page_claim(
    mut segment: *mut mi_segment_t,
    mut page: *mut mi_page_t,
    mut tld: *mut mi_segments_tld_t,
) -> bool {
    if _mi_page_segment(page) == segment {
    } else {
        _mi_assert_fail(
            b"_mi_page_segment(page) == segment\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8648 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_page_claim\0"))
                .as_ptr(),
        );
    };
    if (*page).segment_in_use() == 0 {
    } else {
        _mi_assert_fail(
            b"!page->segment_in_use\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8649 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_page_claim\0"))
                .as_ptr(),
        );
    };
    mi_page_purge_remove(page, tld);
    if !mi_page_ensure_committed(segment, page, tld) {
        return 0 as libc::c_int != 0;
    }
    (*page).set_segment_in_use(1 as libc::c_int as uint8_t);
    (*segment).used = ((*segment).used).wrapping_add(1);
    (*segment).used;
    if (*page).segment_in_use() as libc::c_int != 0
        && (*page).is_committed() as libc::c_int != 0
        && (*page).used as libc::c_int == 0 as libc::c_int
        && !mi_pages_purge_contains(page, tld)
    {
    } else {
        _mi_assert_fail(
            b"page->segment_in_use && page->is_committed && page->used==0 && !mi_pages_purge_contains(page,tld)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8654 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 22],
                &[libc::c_char; 22],
            >(b"mi_segment_page_claim\0"))
                .as_ptr(),
        );
    };
    if (*segment).used <= (*segment).capacity {
    } else {
        _mi_assert_fail(
            b"segment->used <= segment->capacity\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8655 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_page_claim\0"))
                .as_ptr(),
        );
    };
    if (*segment).used == (*segment).capacity
        && (*segment).page_kind as libc::c_uint <= MI_PAGE_MEDIUM as libc::c_int as libc::c_uint
    {
        if !mi_segment_has_free(segment) {
        } else {
            _mi_assert_fail(
                b"!mi_segment_has_free(segment)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8657 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"mi_segment_page_claim\0",
                ))
                .as_ptr(),
            );
        };
        mi_segment_remove_from_free_queue(segment, tld);
    }
    return 1 as libc::c_int != 0;
}
unsafe extern "C" fn mi_segment_page_clear(
    mut segment: *mut mi_segment_t,
    mut page: *mut mi_page_t,
    mut tld: *mut mi_segments_tld_t,
) {
    if (*page).segment_in_use() as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"page->segment_in_use\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8665 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_page_clear\0"))
                .as_ptr(),
        );
    };
    if mi_page_all_free(page) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_all_free(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8666 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_page_clear\0"))
                .as_ptr(),
        );
    };
    if (*page).is_committed() as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"page->is_committed\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8667 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_page_clear\0"))
                .as_ptr(),
        );
    };
    if mi_page_not_in_queue(page, tld) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_page_not_in_queue(page, tld)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8668 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_page_clear\0"))
                .as_ptr(),
        );
    };
    let mut inuse: size_t = (*page).capacity as size_t * mi_page_block_size(page);
    _mi_stat_decrease(&mut (*(*tld).stats).page_committed, inuse);
    _mi_stat_decrease(&mut (*(*tld).stats).pages, 1 as libc::c_int as size_t);
    (*page).set_is_zero_init(0 as libc::c_int as uint8_t);
    (*page).set_segment_in_use(0 as libc::c_int as uint8_t);
    let mut block_size: size_t = (*page).block_size;
    let mut block_size_shift: uint8_t = (*page).block_size_shift;
    let mut heap_tag: uint8_t = (*page).heap_tag;
    let mut page_start: *mut uint8_t = (*page).page_start;
    let mut capacity: uint16_t = (*page).capacity;
    let mut reserved: uint16_t = (*page).reserved;
    let mut ofs: ptrdiff_t = 2 as libc::c_ulong as ptrdiff_t;
    _mi_memzero(
        (page as *mut uint8_t).offset(ofs as isize) as *mut libc::c_void,
        (::core::mem::size_of::<mi_page_t>() as libc::c_ulong).wrapping_sub(ofs as libc::c_ulong),
    );
    (*page).capacity = capacity;
    (*page).reserved = reserved;
    (*page).block_size = block_size;
    (*page).block_size_shift = block_size_shift;
    (*page).heap_tag = heap_tag;
    (*page).page_start = page_start;
    (*segment).used = ((*segment).used).wrapping_sub(1);
    (*segment).used;
    mi_segment_schedule_purge(segment, page, tld);
    (*page).capacity = 0 as libc::c_int as uint16_t;
    (*page).reserved = 0 as libc::c_int as uint16_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segment_page_free(
    mut page: *mut mi_page_t,
    mut force: bool,
    mut tld: *mut mi_segments_tld_t,
) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8695 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"_mi_segment_page_free\0"))
                .as_ptr(),
        );
    };
    let mut segment: *mut mi_segment_t = _mi_page_segment(page);
    mi_pages_try_purge(0 as libc::c_int != 0, tld);
    mi_segment_page_clear(segment, page, tld);
    if (*segment).used == 0 as libc::c_int as size_t {
        mi_segment_free(segment, force, tld);
    } else if (*segment).used == (*segment).abandoned {
        mi_segment_abandon(segment, tld);
    } else if ((*segment).used).wrapping_add(1 as libc::c_int as size_t) == (*segment).capacity {
        if (*segment).page_kind as libc::c_uint <= MI_PAGE_MEDIUM as libc::c_int as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"segment->page_kind <= MI_PAGE_MEDIUM\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8708 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"_mi_segment_page_free\0",
                ))
                .as_ptr(),
            );
        };
        if (*segment).page_kind as libc::c_uint <= MI_PAGE_MEDIUM as libc::c_int as libc::c_uint {
            mi_segment_insert_in_free_queue(segment, tld);
        }
    }
}
unsafe extern "C" fn mi_segment_abandon(
    mut segment: *mut mi_segment_t,
    mut tld: *mut mi_segments_tld_t,
) {
    if (*segment).used == (*segment).abandoned {
    } else {
        _mi_assert_fail(
            b"segment->used == segment->abandoned\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8716 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_segment_abandon\0"))
                .as_ptr(),
        );
    };
    if (*segment).used > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"segment->used > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8717 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_segment_abandon\0"))
                .as_ptr(),
        );
    };
    mi_pages_try_purge(0 as libc::c_int != 0, tld);
    let force_purge: bool = (*segment).memid.memkind as libc::c_uint
        != MI_MEM_ARENA as libc::c_int as libc::c_uint
        || mi_option_is_enabled(mi_option_abandoned_page_purge) as libc::c_int != 0;
    mi_segment_remove_all_purges(segment, force_purge, tld);
    mi_segment_remove_from_free_queue(segment, tld);
    if ((*segment).next).is_null() && ((*segment).prev).is_null() {
    } else {
        _mi_assert_fail(
            b"segment->next == NULL && segment->prev == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8723 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_segment_abandon\0"))
                .as_ptr(),
        );
    };
    _mi_stat_increase(
        &mut (*(*tld).stats).segments_abandoned,
        1 as libc::c_int as size_t,
    );
    mi_segments_track_size(-((*segment).segment_size as libc::c_long), tld);
    (*segment).abandoned_visits = 0 as libc::c_int as size_t;
    if (*segment).was_reclaimed {
        (*tld).reclaim_count = ((*tld).reclaim_count).wrapping_sub(1);
        (*tld).reclaim_count;
        (*segment).was_reclaimed = 0 as libc::c_int != 0;
    }
    _mi_arena_segment_mark_abandoned(segment);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segment_page_abandon(
    mut page: *mut mi_page_t,
    mut tld: *mut mi_segments_tld_t,
) {
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8734 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"_mi_segment_page_abandon\0",
            ))
            .as_ptr(),
        );
    };
    if mi_page_thread_free_flag(page) as libc::c_uint
        == MI_NEVER_DELAYED_FREE as libc::c_int as libc::c_uint
    {
    } else {
        _mi_assert_fail(
            b"mi_page_thread_free_flag(page)==MI_NEVER_DELAYED_FREE\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8735 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"_mi_segment_page_abandon\0",
            ))
            .as_ptr(),
        );
    };
    if (mi_page_heap(page)).is_null() {
    } else {
        _mi_assert_fail(
            b"mi_page_heap(page) == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8736 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"_mi_segment_page_abandon\0",
            ))
            .as_ptr(),
        );
    };
    let mut segment: *mut mi_segment_t = _mi_page_segment(page);
    (*segment).abandoned = ((*segment).abandoned).wrapping_add(1);
    (*segment).abandoned;
    _mi_stat_increase(
        &mut (*(*tld).stats).pages_abandoned,
        1 as libc::c_int as size_t,
    );
    if (*segment).abandoned <= (*segment).used {
    } else {
        _mi_assert_fail(
            b"segment->abandoned <= segment->used\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8742 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"_mi_segment_page_abandon\0",
            ))
            .as_ptr(),
        );
    };
    if (*segment).used == (*segment).abandoned {
        mi_segment_abandon(segment, tld);
    }
}
unsafe extern "C" fn mi_segment_check_free(
    mut segment: *mut mi_segment_t,
    mut block_size: size_t,
    mut all_pages_free: *mut bool,
) -> bool {
    if ::core::intrinsics::atomic_load_relaxed(&mut (*segment).thread_id as *mut mi_threadid_t)
        == 0 as libc::c_int as mi_threadid_t
    {
    } else {
        _mi_assert_fail(
            b"mi_atomic_load_relaxed(&segment->thread_id) == 0\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8749 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_check_free\0"))
                .as_ptr(),
        );
    };
    let mut has_page: bool = 0 as libc::c_int != 0;
    let mut pages_used: size_t = 0 as libc::c_int as size_t;
    let mut pages_used_empty: size_t = 0 as libc::c_int as size_t;
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < (*segment).capacity {
        let mut page: *mut mi_page_t =
            &mut *((*segment).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_t;
        if (*page).segment_in_use() != 0 {
            pages_used = pages_used.wrapping_add(1);
            pages_used;
            _mi_page_free_collect(page, 0 as libc::c_int != 0);
            if mi_page_all_free(page) {
                pages_used_empty = pages_used_empty.wrapping_add(1);
                pages_used_empty;
                has_page = 1 as libc::c_int != 0;
            } else if mi_page_block_size(page) == block_size
                && mi_page_has_any_available(page) as libc::c_int != 0
            {
                has_page = 1 as libc::c_int != 0;
            }
        } else {
            has_page = 1 as libc::c_int != 0;
        }
        i = i.wrapping_add(1);
        i;
    }
    if pages_used == (*segment).used && pages_used >= pages_used_empty {
    } else {
        _mi_assert_fail(
            b"pages_used == segment->used && pages_used >= pages_used_empty\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8770 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_check_free\0"))
                .as_ptr(),
        );
    };
    if !all_pages_free.is_null() {
        *all_pages_free = pages_used.wrapping_sub(pages_used_empty) == 0 as libc::c_int as size_t;
    }
    return has_page;
}
unsafe extern "C" fn mi_segment_reclaim(
    mut segment: *mut mi_segment_t,
    mut heap: *mut mi_heap_t,
    mut requested_block_size: size_t,
    mut right_page_reclaimed: *mut bool,
    mut tld: *mut mi_segments_tld_t,
) -> *mut mi_segment_t {
    if !right_page_reclaimed.is_null() {
        *right_page_reclaimed = 0 as libc::c_int != 0;
    }
    if ::core::intrinsics::atomic_load_relaxed(&mut (*segment).thread_id as *mut mi_threadid_t)
        == 0 as libc::c_int as mi_threadid_t
        || ::core::intrinsics::atomic_load_relaxed(&mut (*segment).thread_id as *mut mi_threadid_t)
            == _mi_thread_id()
    {
    } else {
        _mi_assert_fail(
            b"mi_atomic_load_relaxed(&segment->thread_id) == 0 || mi_atomic_load_relaxed(&segment->thread_id) == _mi_thread_id()\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8778 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 19],
                &[libc::c_char; 19],
            >(b"mi_segment_reclaim\0"))
                .as_ptr(),
        );
    };
    if (*segment).subproc == (*(*heap).tld).segments.subproc {
    } else {
        _mi_assert_fail(
            b"segment->subproc == heap->tld->segments.subproc\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8779 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_segment_reclaim\0"))
                .as_ptr(),
        );
    };
    ::core::intrinsics::atomic_store_release(&mut (*segment).thread_id, _mi_thread_id());
    (*segment).abandoned_visits = 0 as libc::c_int as size_t;
    (*segment).was_reclaimed = 1 as libc::c_int != 0;
    (*tld).reclaim_count = ((*tld).reclaim_count).wrapping_add(1);
    (*tld).reclaim_count;
    mi_segments_track_size((*segment).segment_size as libc::c_long, tld);
    if ((*segment).next).is_null() && ((*segment).prev).is_null() {
    } else {
        _mi_assert_fail(
            b"segment->next == NULL && segment->prev == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8785 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_segment_reclaim\0"))
                .as_ptr(),
        );
    };
    _mi_stat_decrease(
        &mut (*(*tld).stats).segments_abandoned,
        1 as libc::c_int as size_t,
    );
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < (*segment).capacity {
        let mut page: *mut mi_page_t =
            &mut *((*segment).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_t;
        if (*page).segment_in_use() != 0 {
            if (*page).is_committed() as libc::c_int != 0 {
            } else {
                _mi_assert_fail(
                    b"page->is_committed\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    8791 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                        b"mi_segment_reclaim\0",
                    ))
                    .as_ptr(),
                );
            };
            if mi_page_not_in_queue(page, tld) as libc::c_int != 0 {
            } else {
                _mi_assert_fail(
                    b"mi_page_not_in_queue(page, tld)\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    8792 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                        b"mi_segment_reclaim\0",
                    ))
                    .as_ptr(),
                );
            };
            if mi_page_thread_free_flag(page) as libc::c_uint
                == MI_NEVER_DELAYED_FREE as libc::c_int as libc::c_uint
            {
            } else {
                _mi_assert_fail(
                    b"mi_page_thread_free_flag(page)==MI_NEVER_DELAYED_FREE\0" as *const u8
                        as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    8793 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                        b"mi_segment_reclaim\0",
                    ))
                    .as_ptr(),
                );
            };
            if (mi_page_heap(page)).is_null() {
            } else {
                _mi_assert_fail(
                    b"mi_page_heap(page) == NULL\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    8794 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                        b"mi_segment_reclaim\0",
                    ))
                    .as_ptr(),
                );
            };
            (*segment).abandoned = ((*segment).abandoned).wrapping_sub(1);
            (*segment).abandoned;
            if ((*page).next).is_null() {
            } else {
                _mi_assert_fail(
                    b"page->next == NULL\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    8796 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                        b"mi_segment_reclaim\0",
                    ))
                    .as_ptr(),
                );
            };
            _mi_stat_decrease(
                &mut (*(*tld).stats).pages_abandoned,
                1 as libc::c_int as size_t,
            );
            let mut target_heap: *mut mi_heap_t = _mi_heap_by_tag(heap, (*page).heap_tag);
            if target_heap.is_null() {
                target_heap = heap;
                _mi_error_message(
                    14 as libc::c_int,
                    b"page with tag %u cannot be reclaimed by a heap with the same tag (using heap tag %u instead)\n\0"
                        as *const u8 as *const libc::c_char,
                    (*page).heap_tag as libc::c_int,
                    (*heap).tag as libc::c_int,
                );
            }
            mi_page_set_heap(page, target_heap);
            _mi_page_use_delayed_free(page, MI_USE_DELAYED_FREE, 1 as libc::c_int != 0);
            _mi_page_free_collect(page, 0 as libc::c_int != 0);
            if mi_page_all_free(page) {
                mi_segment_page_clear(segment, page, tld);
            } else {
                _mi_page_reclaim(target_heap, page);
                if requested_block_size == mi_page_block_size(page)
                    && mi_page_has_any_available(page) as libc::c_int != 0
                    && heap == target_heap
                {
                    if !right_page_reclaimed.is_null() {
                        *right_page_reclaimed = 1 as libc::c_int != 0;
                    }
                }
            }
        }
        i = i.wrapping_add(1);
        i;
    }
    if (*segment).abandoned == 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"segment->abandoned == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8817 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(b"mi_segment_reclaim\0"))
                .as_ptr(),
        );
    };
    if (*segment).used == 0 as libc::c_int as size_t {
        if right_page_reclaimed.is_null() || !*right_page_reclaimed {
        } else {
            _mi_assert_fail(
                b"right_page_reclaimed == NULL || !(*right_page_reclaimed)\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8819 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 19], &[libc::c_char; 19]>(
                    b"mi_segment_reclaim\0",
                ))
                .as_ptr(),
            );
        };
        mi_segment_free(segment, 0 as libc::c_int != 0, tld);
        return 0 as *mut mi_segment_t;
    } else {
        if (*segment).page_kind as libc::c_uint <= MI_PAGE_MEDIUM as libc::c_int as libc::c_uint
            && mi_segment_has_free(segment) as libc::c_int != 0
        {
            mi_segment_insert_in_free_queue(segment, tld);
        }
        return segment;
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segment_attempt_reclaim(
    mut heap: *mut mi_heap_t,
    mut segment: *mut mi_segment_t,
) -> bool {
    if ::core::intrinsics::atomic_load_relaxed(&mut (*segment).thread_id as *mut mi_threadid_t)
        != 0 as libc::c_int as mi_threadid_t
    {
        return 0 as libc::c_int != 0;
    }
    if (*segment).subproc != (*(*heap).tld).segments.subproc {
        return 0 as libc::c_int != 0;
    }
    if !_mi_heap_memid_is_suitable(heap, (*segment).memid) {
        return 0 as libc::c_int != 0;
    }
    let target: libc::c_long = _mi_option_get_fast(mi_option_target_segments_per_thread);
    if target > 0 as libc::c_int as libc::c_long
        && target as size_t <= (*(*heap).tld).segments.count
    {
        return 0 as libc::c_int != 0;
    }
    if (*segment).memid.memkind as libc::c_uint == MI_MEM_ARENA as libc::c_int as libc::c_uint
        && (*(*heap).tld).segments.reclaim_count * 2 as libc::c_int as size_t
            > (*(*heap).tld).segments.count
    {
        return 0 as libc::c_int != 0;
    }
    if _mi_arena_segment_clear_abandoned(segment) {
        let mut res: *mut mi_segment_t = mi_segment_reclaim(
            segment,
            heap,
            0 as libc::c_int as size_t,
            0 as *mut bool,
            &mut (*(*heap).tld).segments,
        );
        if res == segment {
        } else {
            _mi_assert_fail(
                b"res == segment\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8841 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                    b"_mi_segment_attempt_reclaim\0",
                ))
                .as_ptr(),
            );
        };
        return !res.is_null();
    }
    return 0 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_abandoned_reclaim_all(
    mut heap: *mut mi_heap_t,
    mut tld: *mut mi_segments_tld_t,
) {
    let mut segment: *mut mi_segment_t = 0 as *mut mi_segment_t;
    let mut current: mi_arena_field_cursor_t = mi_arena_field_cursor_s {
        os_list_count: 0,
        start: 0,
        end: 0,
        bitmap_idx: 0,
        subproc: 0 as *mut mi_subproc_t,
        visit_all: false,
        hold_visit_lock: false,
    };
    _mi_arena_field_cursor_init(heap, (*tld).subproc, 1 as libc::c_int != 0, &mut current);
    loop {
        segment = _mi_arena_segment_clear_abandoned_next(&mut current);
        if segment.is_null() {
            break;
        }
        mi_segment_reclaim(
            segment,
            heap,
            0 as libc::c_int as size_t,
            0 as *mut bool,
            tld,
        );
    }
    _mi_arena_field_cursor_done(&mut current);
}
unsafe extern "C" fn segment_count_is_within_target(
    mut tld: *mut mi_segments_tld_t,
    mut ptarget: *mut size_t,
) -> bool {
    let target: size_t = mi_option_get_clamp(
        mi_option_target_segments_per_thread,
        0 as libc::c_int as libc::c_long,
        1024 as libc::c_int as libc::c_long,
    ) as size_t;
    if !ptarget.is_null() {
        *ptarget = target;
    }
    return target == 0 as libc::c_int as size_t || (*tld).count < target;
}
unsafe extern "C" fn mi_segment_get_reclaim_tries(mut tld: *mut mi_segments_tld_t) -> libc::c_long {
    let perc: size_t = mi_option_get_clamp(
        mi_option_max_segment_reclaim,
        0 as libc::c_int as libc::c_long,
        100 as libc::c_int as libc::c_long,
    ) as size_t;
    if perc <= 0 as libc::c_int as size_t {
        return 0 as libc::c_int as libc::c_long;
    }
    let total_count: size_t =
        ::core::intrinsics::atomic_load_relaxed(&mut (*(*tld).subproc).abandoned_count);
    if total_count == 0 as libc::c_int as size_t {
        return 0 as libc::c_int as libc::c_long;
    }
    let relative_count: size_t = if total_count > 10000 as libc::c_int as size_t {
        total_count / 100 as libc::c_int as size_t * perc
    } else {
        total_count * perc / 100 as libc::c_int as size_t
    };
    let mut max_tries: libc::c_long = (if relative_count <= 1 as libc::c_int as size_t {
        1 as libc::c_int as size_t
    } else if relative_count > 1024 as libc::c_int as size_t {
        1024 as libc::c_int as size_t
    } else {
        relative_count
    }) as libc::c_long;
    if max_tries < 8 as libc::c_int as libc::c_long && total_count > 8 as libc::c_int as size_t {
        max_tries = 8 as libc::c_int as libc::c_long;
    }
    return max_tries;
}
unsafe extern "C" fn mi_segment_try_reclaim(
    mut heap: *mut mi_heap_t,
    mut block_size: size_t,
    mut page_kind: mi_page_kind_t,
    mut reclaimed: *mut bool,
    mut tld: *mut mi_segments_tld_t,
) -> *mut mi_segment_t {
    *reclaimed = 0 as libc::c_int != 0;
    let mut max_tries: libc::c_long = mi_segment_get_reclaim_tries(tld);
    if max_tries <= 0 as libc::c_int as libc::c_long {
        return 0 as *mut mi_segment_t;
    }
    let mut result: *mut mi_segment_t = 0 as *mut mi_segment_t;
    let mut segment: *mut mi_segment_t = 0 as *mut mi_segment_t;
    let mut current: mi_arena_field_cursor_t = mi_arena_field_cursor_s {
        os_list_count: 0,
        start: 0,
        end: 0,
        bitmap_idx: 0,
        subproc: 0 as *mut mi_subproc_t,
        visit_all: false,
        hold_visit_lock: false,
    };
    _mi_arena_field_cursor_init(heap, (*tld).subproc, 0 as libc::c_int != 0, &mut current);
    while segment_count_is_within_target(tld, 0 as *mut size_t) as libc::c_int != 0
        && {
            let fresh47 = max_tries;
            max_tries = max_tries - 1;
            fresh47 > 0 as libc::c_int as libc::c_long
        }
        && {
            segment = _mi_arena_segment_clear_abandoned_next(&mut current);
            !segment.is_null()
        }
    {
        if (*segment).subproc == (*(*heap).tld).segments.subproc {
        } else {
            _mi_assert_fail(
                b"segment->subproc == heap->tld->segments.subproc\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8881 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                    b"mi_segment_try_reclaim\0",
                ))
                .as_ptr(),
            );
        };
        (*segment).abandoned_visits = ((*segment).abandoned_visits).wrapping_add(1);
        (*segment).abandoned_visits;
        let mut is_suitable: bool = _mi_heap_memid_is_suitable(heap, (*segment).memid);
        let mut all_pages_free: bool = false;
        let mut has_page: bool = mi_segment_check_free(segment, block_size, &mut all_pages_free);
        if all_pages_free {
            mi_segment_reclaim(
                segment,
                heap,
                0 as libc::c_int as size_t,
                0 as *mut bool,
                tld,
            );
        } else if has_page as libc::c_int != 0
            && (*segment).page_kind as libc::c_uint == page_kind as libc::c_uint
            && is_suitable as libc::c_int != 0
        {
            result = mi_segment_reclaim(segment, heap, block_size, reclaimed, tld);
            break;
        } else if (*segment).abandoned_visits > 3 as libc::c_int as size_t
            && is_suitable as libc::c_int != 0
        {
            mi_segment_reclaim(
                segment,
                heap,
                0 as libc::c_int as size_t,
                0 as *mut bool,
                tld,
            );
        } else {
            _mi_arena_segment_mark_abandoned(segment);
        }
    }
    _mi_arena_field_cursor_done(&mut current);
    return result;
}
unsafe extern "C" fn mi_segment_force_abandon(
    mut segment: *mut mi_segment_t,
    mut tld: *mut mi_segments_tld_t,
) {
    if (*segment).abandoned < (*segment).used {
    } else {
        _mi_assert_fail(
            b"segment->abandoned < segment->used\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8905 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_segment_force_abandon\0",
            ))
            .as_ptr(),
        );
    };
    if !(*segment).dont_free {
    } else {
        _mi_assert_fail(
            b"!segment->dont_free\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8906 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_segment_force_abandon\0",
            ))
            .as_ptr(),
        );
    };
    (*segment).dont_free = 1 as libc::c_int != 0;
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < (*segment).capacity {
        let mut page: *mut mi_page_t =
            &mut *((*segment).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_t;
        if (*page).segment_in_use() != 0 {
            if (*segment).used > 0 as libc::c_int as size_t {
            } else {
                _mi_assert_fail(
                    b"segment->used > 0\0" as *const u8 as *const libc::c_char,
                    b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                    8911 as libc::c_int as libc::c_uint,
                    (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                        b"mi_segment_force_abandon\0",
                    ))
                    .as_ptr(),
                );
            };
            if (*segment).used == ((*segment).abandoned).wrapping_add(1 as libc::c_int as size_t) {
                (*segment).dont_free = 0 as libc::c_int != 0;
                _mi_page_force_abandon(page);
                return;
            } else {
                _mi_page_force_abandon(page);
            }
        }
        i = i.wrapping_add(1);
        i;
    }
    (*segment).dont_free = 0 as libc::c_int != 0;
    if (*segment).used == (*segment).abandoned {
    } else {
        _mi_assert_fail(
            b"segment->used == segment->abandoned\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8923 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_segment_force_abandon\0",
            ))
            .as_ptr(),
        );
    };
    if (*segment).used == 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"segment->used == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8924 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_segment_force_abandon\0",
            ))
            .as_ptr(),
        );
    };
    if (*segment).used == 0 as libc::c_int as size_t {
        mi_segment_free(segment, 0 as libc::c_int != 0, tld);
    } else {
        mi_pages_try_purge(0 as libc::c_int != 0, tld);
    };
}
unsafe extern "C" fn mi_segments_try_abandon_to_target(
    mut heap: *mut mi_heap_t,
    mut target: size_t,
    mut tld: *mut mi_segments_tld_t,
) {
    if target <= 1 as libc::c_int as size_t {
        return;
    }
    let min_target: size_t = if target > 4 as libc::c_int as size_t {
        target * 3 as libc::c_int as size_t / 4 as libc::c_int as size_t
    } else {
        target
    };
    let mut i: libc::c_int = 0 as libc::c_int;
    while i < 64 as libc::c_int && (*tld).count >= min_target {
        let mut page: *mut mi_page_t = (*heap).pages
            [(73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) as usize]
            .first;
        while !page.is_null() && mi_page_is_huge(page) as libc::c_int != 0 {
            page = (*page).next;
        }
        if page.is_null() {
            break;
        }
        let mut segment: *mut mi_segment_t = _mi_page_segment(page);
        mi_segment_force_abandon(segment, tld);
        if page
            != (*heap).pages
                [(73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint) as usize]
                .first
        {
        } else {
            _mi_assert_fail(
                b"page != heap->pages[MI_BIN_FULL].first\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8945 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 34], &[libc::c_char; 34]>(
                    b"mi_segments_try_abandon_to_target\0",
                ))
                .as_ptr(),
            );
        };
        i += 1;
        i;
    }
}
unsafe extern "C" fn mi_segments_try_abandon(
    mut heap: *mut mi_heap_t,
    mut tld: *mut mi_segments_tld_t,
) {
    let mut target: size_t = 0 as libc::c_int as size_t;
    if segment_count_is_within_target(tld, &mut target) {
        return;
    }
    mi_segments_try_abandon_to_target(heap, target, tld);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_collect_reduce(mut target_size: size_t) {
    mi_collect(1 as libc::c_int != 0);
    let mut heap: *mut mi_heap_t = mi_heap_get_default();
    let mut tld: *mut mi_segments_tld_t = &mut (*(*heap).tld).segments;
    let mut target: size_t = (target_size as libc::c_ulonglong).wrapping_div(
        (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
    ) as size_t;
    if target == 0 as libc::c_int as size_t {
        target = mi_option_get_clamp(
            mi_option_target_segments_per_thread,
            1 as libc::c_int as libc::c_long,
            1024 as libc::c_int as libc::c_long,
        ) as size_t;
    }
    mi_segments_try_abandon_to_target(heap, target, tld);
}
unsafe extern "C" fn mi_segment_reclaim_or_alloc(
    mut heap: *mut mi_heap_t,
    mut block_size: size_t,
    mut page_kind: mi_page_kind_t,
    mut page_shift: size_t,
    mut tld: *mut mi_segments_tld_t,
    mut os_tld: *mut mi_os_tld_t,
) -> *mut mi_segment_t {
    if page_kind as libc::c_uint <= MI_PAGE_LARGE as libc::c_int as libc::c_uint {
    } else {
        _mi_assert_fail(
            b"page_kind <= MI_PAGE_LARGE\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8965 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"mi_segment_reclaim_or_alloc\0",
            ))
            .as_ptr(),
        );
    };
    if block_size as libc::c_ulonglong
        <= ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
    {
    } else {
        _mi_assert_fail(
            b"block_size <= MI_LARGE_OBJ_SIZE_MAX\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8966 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"mi_segment_reclaim_or_alloc\0",
            ))
            .as_ptr(),
        );
    };
    mi_segments_try_abandon(heap, tld);
    let mut reclaimed: bool = false;
    let mut segment: *mut mi_segment_t =
        mi_segment_try_reclaim(heap, block_size, page_kind, &mut reclaimed, tld);
    if segment.is_null()
        || _mi_arena_memid_is_suitable((*segment).memid, (*heap).arena_id) as libc::c_int != 0
    {
    } else {
        _mi_assert_fail(
            b"segment == NULL || _mi_arena_memid_is_suitable(segment->memid, heap->arena_id)\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8970 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"mi_segment_reclaim_or_alloc\0",
            ))
            .as_ptr(),
        );
    };
    if reclaimed {
        if !segment.is_null()
            && (*segment).page_kind as libc::c_uint == page_kind as libc::c_uint
            && page_kind as libc::c_uint <= MI_PAGE_LARGE as libc::c_int as libc::c_uint
        {
        } else {
            _mi_assert_fail(
                b"segment != NULL && segment->page_kind == page_kind && page_kind <= MI_PAGE_LARGE\0"
                    as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                8972 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<
                    &[u8; 28],
                    &[libc::c_char; 28],
                >(b"mi_segment_reclaim_or_alloc\0"))
                    .as_ptr(),
            );
        };
        return 0 as *mut mi_segment_t;
    } else if !segment.is_null() {
        return segment;
    }
    return mi_segment_alloc(
        0 as libc::c_int as size_t,
        page_kind,
        page_shift,
        0 as libc::c_int as size_t,
        (*heap).arena_id,
        tld,
        os_tld,
    );
}
unsafe extern "C" fn mi_segment_find_free(
    mut segment: *mut mi_segment_t,
    mut tld: *mut mi_segments_tld_t,
) -> *mut mi_page_t {
    if mi_segment_has_free(segment) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_segment_has_free(segment)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8981 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_segment_find_free\0"))
                .as_ptr(),
        );
    };
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < (*segment).capacity {
        let mut page: *mut mi_page_t =
            &mut *((*segment).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_t;
        if (*page).segment_in_use() == 0 {
            let mut ok: bool = mi_segment_page_claim(segment, page, tld);
            if ok {
                return page;
            }
        }
        i = i.wrapping_add(1);
        i;
    }
    if 0 as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"false\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8990 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 21], &[libc::c_char; 21]>(b"mi_segment_find_free\0"))
                .as_ptr(),
        );
    };
    return 0 as *mut mi_page_t;
}
unsafe extern "C" fn mi_segment_page_alloc_in(
    mut segment: *mut mi_segment_t,
    mut tld: *mut mi_segments_tld_t,
) -> *mut mi_page_t {
    if mi_segment_has_free(segment) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"mi_segment_has_free(segment)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            8994 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 25], &[libc::c_char; 25]>(
                b"mi_segment_page_alloc_in\0",
            ))
            .as_ptr(),
        );
    };
    return mi_segment_find_free(segment, tld);
}
unsafe extern "C" fn mi_segment_page_try_alloc_in_queue(
    mut heap: *mut mi_heap_t,
    mut kind: mi_page_kind_t,
    mut tld: *mut mi_segments_tld_t,
) -> *mut mi_page_t {
    let free_queue: *mut mi_segment_queue_t = mi_segment_free_queue_of_kind(kind, tld);
    let mut segment: *mut mi_segment_t = (*free_queue).first;
    while !segment.is_null() {
        if _mi_arena_memid_is_suitable((*segment).memid, (*heap).arena_id) as libc::c_int != 0
            && mi_segment_has_free(segment) as libc::c_int != 0
        {
            return mi_segment_page_alloc_in(segment, tld);
        }
        segment = (*segment).next;
    }
    return 0 as *mut mi_page_t;
}
unsafe extern "C" fn mi_segment_page_alloc(
    mut heap: *mut mi_heap_t,
    mut block_size: size_t,
    mut kind: mi_page_kind_t,
    mut page_shift: size_t,
    mut tld: *mut mi_segments_tld_t,
    mut os_tld: *mut mi_os_tld_t,
) -> *mut mi_page_t {
    let mut page: *mut mi_page_t = mi_segment_page_try_alloc_in_queue(heap, kind, tld);
    if page.is_null() {
        let segment: *mut mi_segment_t =
            mi_segment_reclaim_or_alloc(heap, block_size, kind, page_shift, tld, os_tld);
        if segment.is_null() {
            return 0 as *mut mi_page_t;
        }
        if (*segment).page_kind as libc::c_uint == kind as libc::c_uint {
        } else {
            _mi_assert_fail(
                b"segment->page_kind==kind\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                9011 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"mi_segment_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        if (*segment).used < (*segment).capacity {
        } else {
            _mi_assert_fail(
                b"segment->used < segment->capacity\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                9012 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"mi_segment_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        if _mi_arena_memid_is_suitable((*segment).memid, (*heap).arena_id) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"_mi_arena_memid_is_suitable(segment->memid, heap->arena_id)\0" as *const u8
                    as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                9013 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(
                    b"mi_segment_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        page = mi_segment_page_try_alloc_in_queue(heap, kind, tld);
    }
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9016 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 22], &[libc::c_char; 22]>(b"mi_segment_page_alloc\0"))
                .as_ptr(),
        );
    };
    *(mi_segment_raw_page_start(_mi_page_segment(page), page, 0 as *mut size_t))
        .offset(0 as libc::c_int as isize) = 0 as libc::c_int as uint8_t;
    return page;
}
unsafe extern "C" fn mi_segment_small_page_alloc(
    mut heap: *mut mi_heap_t,
    mut block_size: size_t,
    mut tld: *mut mi_segments_tld_t,
    mut os_tld: *mut mi_os_tld_t,
) -> *mut mi_page_t {
    return mi_segment_page_alloc(
        heap,
        block_size,
        MI_PAGE_SMALL,
        (13 as libc::c_int + 3 as libc::c_int) as size_t,
        tld,
        os_tld,
    );
}
unsafe extern "C" fn mi_segment_medium_page_alloc(
    mut heap: *mut mi_heap_t,
    mut block_size: size_t,
    mut tld: *mut mi_segments_tld_t,
    mut os_tld: *mut mi_os_tld_t,
) -> *mut mi_page_t {
    return mi_segment_page_alloc(
        heap,
        block_size,
        MI_PAGE_MEDIUM,
        (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)) as size_t,
        tld,
        os_tld,
    );
}
unsafe extern "C" fn mi_segment_large_page_alloc(
    mut heap: *mut mi_heap_t,
    mut block_size: size_t,
    mut tld: *mut mi_segments_tld_t,
    mut os_tld: *mut mi_os_tld_t,
) -> *mut mi_page_t {
    let mut segment: *mut mi_segment_t = mi_segment_reclaim_or_alloc(
        heap,
        block_size,
        MI_PAGE_LARGE,
        (3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))) as size_t,
        tld,
        os_tld,
    );
    if segment.is_null() {
        return 0 as *mut mi_page_t;
    }
    let mut page: *mut mi_page_t = mi_segment_find_free(segment, tld);
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9032 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"mi_segment_large_page_alloc\0",
            ))
            .as_ptr(),
        );
    };
    *(mi_segment_raw_page_start(segment, page, 0 as *mut size_t))
        .offset(0 as libc::c_int as isize) = 0 as libc::c_int as uint8_t;
    return page;
}
unsafe extern "C" fn mi_segment_huge_page_alloc(
    mut size: size_t,
    mut page_alignment: size_t,
    mut req_arena_id: mi_arena_id_t,
    mut tld: *mut mi_segments_tld_t,
    mut os_tld: *mut mi_os_tld_t,
) -> *mut mi_page_t {
    let mut segment: *mut mi_segment_t = mi_segment_alloc(
        size,
        MI_PAGE_HUGE,
        (3 as libc::c_int
            + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
            + 1 as libc::c_int) as size_t,
        page_alignment,
        req_arena_id,
        tld,
        os_tld,
    );
    if segment.is_null() {
        return 0 as *mut mi_page_t;
    }
    if (mi_segment_page_size(segment))
        .wrapping_sub((*segment).segment_info_size)
        .wrapping_sub(
            2 as libc::c_int as size_t
                * (if 0 as libc::c_int == 0 as libc::c_int {
                    0 as libc::c_int as size_t
                } else {
                    _mi_os_page_size()
                }),
        )
        >= size
    {
    } else {
        _mi_assert_fail(
            b"mi_segment_page_size(segment) - segment->segment_info_size - (2*(MI_SECURE == 0 ? 0 : _mi_os_page_size())) >= size\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9042 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 27],
                &[libc::c_char; 27],
            >(b"mi_segment_huge_page_alloc\0"))
                .as_ptr(),
        );
    };
    let mut page: *mut mi_page_t = mi_segment_find_free(segment, tld);
    if !page.is_null() {
    } else {
        _mi_assert_fail(
            b"page != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9048 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"mi_segment_huge_page_alloc\0",
            ))
            .as_ptr(),
        );
    };
    if (*page).is_huge() as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"page->is_huge\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9049 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                b"mi_segment_huge_page_alloc\0",
            ))
            .as_ptr(),
        );
    };
    let mut psize: size_t = 0;
    let mut start: *mut uint8_t = mi_segment_raw_page_start(segment, page, &mut psize);
    (*page).block_size = psize;
    if page_alignment > 0 as libc::c_int as size_t
        && (*segment).allow_decommit as libc::c_int != 0
        && (*page).is_committed() as libc::c_int != 0
    {
        let mut aligned_p: *mut uint8_t =
            _mi_align_up(start as uintptr_t, page_alignment) as *mut uint8_t;
        if _mi_is_aligned(aligned_p as *mut libc::c_void, page_alignment) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"_mi_is_aligned(aligned_p, page_alignment)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                9055 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"mi_segment_huge_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        if psize.wrapping_sub(aligned_p.offset_from(start) as libc::c_long as size_t) >= size {
        } else {
            _mi_assert_fail(
                b"psize - (aligned_p - start) >= size\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                9056 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 27], &[libc::c_char; 27]>(
                    b"mi_segment_huge_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        let mut decommit_start: *mut uint8_t =
            start.offset(::core::mem::size_of::<mi_block_t>() as libc::c_ulong as isize);
        let mut decommit_size: ptrdiff_t = aligned_p.offset_from(decommit_start) as libc::c_long;
        _mi_os_reset(
            decommit_start as *mut libc::c_void,
            decommit_size as size_t,
            (*os_tld).stats,
        );
    }
    return page;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segment_huge_page_reset(
    mut segment: *mut mi_segment_t,
    mut page: *mut mi_page_t,
    mut block: *mut mi_block_t,
) {
    if (*segment).page_kind as libc::c_uint == MI_PAGE_HUGE as libc::c_int as libc::c_uint {
    } else {
        _mi_assert_fail(
            b"segment->page_kind == MI_PAGE_HUGE\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9088 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"_mi_segment_huge_page_reset\0",
            ))
            .as_ptr(),
        );
    };
    if segment == _mi_page_segment(page) {
    } else {
        _mi_assert_fail(
            b"segment == _mi_page_segment(page)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9089 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"_mi_segment_huge_page_reset\0",
            ))
            .as_ptr(),
        );
    };
    if (*page).used as libc::c_int == 1 as libc::c_int {
    } else {
        _mi_assert_fail(
            b"page->used == 1\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9090 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"_mi_segment_huge_page_reset\0",
            ))
            .as_ptr(),
        );
    };
    if ((*page).free).is_null() {
    } else {
        _mi_assert_fail(
            b"page->free == NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9091 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 28], &[libc::c_char; 28]>(
                b"_mi_segment_huge_page_reset\0",
            ))
            .as_ptr(),
        );
    };
    if (*segment).allow_decommit as libc::c_int != 0 && (*page).is_committed() as libc::c_int != 0 {
        let mut usize: size_t = mi_usable_size(block as *const libc::c_void);
        if usize > ::core::mem::size_of::<mi_block_t>() as libc::c_ulong {
            usize = usize.wrapping_sub(::core::mem::size_of::<mi_block_t>() as libc::c_ulong);
            let mut p: *mut uint8_t =
                (block as *mut uint8_t)
                    .offset(::core::mem::size_of::<mi_block_t>() as libc::c_ulong as isize);
            _mi_os_reset(p as *mut libc::c_void, usize, &mut _mi_stats_main);
        }
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segment_page_alloc(
    mut heap: *mut mi_heap_t,
    mut block_size: size_t,
    mut page_alignment: size_t,
    mut tld: *mut mi_segments_tld_t,
    mut os_tld: *mut mi_os_tld_t,
) -> *mut mi_page_t {
    let mut page: *mut mi_page_t = 0 as *mut mi_page_t;
    if (page_alignment as libc::c_ulonglong
        > (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
            >> 1 as libc::c_int) as libc::c_int as libc::c_long
        != 0
    {
        if _mi_is_power_of_two(page_alignment) as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"_mi_is_power_of_two(page_alignment)\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                9105 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                    b"_mi_segment_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        if page_alignment as libc::c_ulonglong
            >= (1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
        {
        } else {
            _mi_assert_fail(
                b"page_alignment >= MI_SEGMENT_SIZE\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                9106 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                    b"_mi_segment_page_alloc\0",
                ))
                .as_ptr(),
            );
        };
        if (page_alignment as libc::c_ulonglong)
            < (1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
        {
            page_alignment = ((1 as libc::c_ulonglong)
                << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                as size_t;
        }
        page =
            mi_segment_huge_page_alloc(block_size, page_alignment, (*heap).arena_id, tld, os_tld);
    } else if block_size as libc::c_ulonglong
        <= ((1 as libc::c_ulonglong) << 13 as libc::c_int + 3 as libc::c_int)
            .wrapping_div(4 as libc::c_int as libc::c_ulonglong)
    {
        page = mi_segment_small_page_alloc(heap, block_size, tld, os_tld);
    } else if block_size as libc::c_ulonglong
        <= ((1 as libc::c_ulonglong) << 3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int))
            .wrapping_div(4 as libc::c_int as libc::c_ulonglong)
    {
        page = mi_segment_medium_page_alloc(heap, block_size, tld, os_tld);
    } else if block_size as libc::c_ulonglong
        <= ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
    {
        page = mi_segment_large_page_alloc(heap, block_size, tld, os_tld);
    } else {
        page =
            mi_segment_huge_page_alloc(block_size, page_alignment, (*heap).arena_id, tld, os_tld);
    }
    if page.is_null()
        || (mi_segment_page_size(_mi_page_segment(page))).wrapping_sub(
            (if 0 as libc::c_int == 0 as libc::c_int {
                0 as libc::c_int as size_t
            } else {
                _mi_os_page_size()
            }),
        ) >= block_size
    {
    } else {
        _mi_assert_fail(
            b"page == NULL || (mi_segment_page_size(_mi_page_segment(page)) - (MI_SECURE == 0 ? 0 : _mi_os_page_size())) >= block_size\0"
                as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9123 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<
                &[u8; 23],
                &[libc::c_char; 23],
            >(b"_mi_segment_page_alloc\0"))
                .as_ptr(),
        );
    };
    if page.is_null() || mi_page_not_in_queue(page, tld) as libc::c_int != 0 {
    } else {
        _mi_assert_fail(
            b"page == NULL || mi_page_not_in_queue(page, tld)\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9124 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"_mi_segment_page_alloc\0",
            ))
            .as_ptr(),
        );
    };
    if page.is_null() || (*_mi_page_segment(page)).subproc == (*tld).subproc {
    } else {
        _mi_assert_fail(
            b"page == NULL || _mi_page_segment(page)->subproc == tld->subproc\0" as *const u8
                as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9125 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 23], &[libc::c_char; 23]>(
                b"_mi_segment_page_alloc\0",
            ))
            .as_ptr(),
        );
    };
    return page;
}
unsafe extern "C" fn mi_segment_visit_page(
    mut page: *mut mi_page_t,
    mut visit_blocks: bool,
    mut visitor: Option<mi_block_visit_fun>,
    mut arg: *mut libc::c_void,
) -> bool {
    let mut area: mi_heap_area_t = mi_heap_area_s {
        blocks: 0 as *mut libc::c_void,
        reserved: 0,
        committed: 0,
        used: 0,
        block_size: 0,
        full_block_size: 0,
        heap_tag: 0,
    };
    _mi_heap_area_init(&mut area, page);
    if !visitor.expect("non-null function pointer")(
        0 as *const mi_heap_t,
        &mut area,
        0 as *mut libc::c_void,
        area.block_size,
        arg,
    ) {
        return 0 as libc::c_int != 0;
    }
    if visit_blocks {
        return _mi_heap_area_visit_blocks(&mut area, page, visitor, arg);
    } else {
        return 1 as libc::c_int != 0;
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segment_visit_blocks(
    mut segment: *mut mi_segment_t,
    mut heap_tag: libc::c_int,
    mut visit_blocks: bool,
    mut visitor: Option<mi_block_visit_fun>,
    mut arg: *mut libc::c_void,
) -> bool {
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i < (*segment).capacity {
        let page: *mut mi_page_t =
            &mut *((*segment).pages).as_mut_ptr().offset(i as isize) as *mut mi_page_t;
        if (*page).segment_in_use() != 0 {
            if heap_tag < 0 as libc::c_int || (*page).heap_tag as libc::c_int == heap_tag {
                if !mi_segment_visit_page(page, visit_blocks, visitor, arg) {
                    return 0 as libc::c_int != 0;
                }
            }
        }
        i = i.wrapping_add(1);
        i;
    }
    return 1 as libc::c_int != 0;
}
static mut mi_segment_map: [*mut mi_segmap_part_t; 196] =
    [0 as *const mi_segmap_part_t as *mut mi_segmap_part_t; 196];
unsafe extern "C" fn mi_segment_map_index_of(
    mut segment: *const mi_segment_t,
    mut create_on_demand: bool,
    mut idx: *mut size_t,
    mut bitidx: *mut size_t,
) -> *mut mi_segmap_part_t {
    if _mi_ptr_segment(segment.offset(1 as libc::c_int as isize) as *const libc::c_void)
        == segment as *mut mi_segment_t
    {
    } else {
        _mi_assert_fail(
            b"_mi_ptr_segment(segment + 1) == segment\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9169 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                b"mi_segment_map_index_of\0",
            ))
            .as_ptr(),
        );
    };
    *idx = 0 as libc::c_int as size_t;
    *bitidx = 0 as libc::c_int as size_t;
    if segment as uintptr_t as libc::c_ulonglong
        >= (48 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong)
            .wrapping_mul(
                (1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong),
            )
    {
        return 0 as *mut mi_segmap_part_t;
    }
    let segindex: uintptr_t = (segment as uintptr_t as libc::c_ulonglong).wrapping_div(
        (8 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul(
                (((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_sub(128 as libc::c_int as libc::c_ulonglong),
            )
            .wrapping_mul(
                (1 as libc::c_ulonglong)
                    << 3 as libc::c_int
                        + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
            ),
    ) as uintptr_t;
    if segindex as libc::c_ulonglong
        >= (48 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul(1024 as libc::c_ulonglong)
            .wrapping_mul(
                (1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong),
            )
            .wrapping_div(
                (8 as libc::c_int as libc::c_ulonglong)
                    .wrapping_mul(
                        (((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulonglong)
                            .wrapping_mul(1024 as libc::c_ulonglong)
                            .wrapping_sub(128 as libc::c_int as libc::c_ulonglong),
                    )
                    .wrapping_mul(
                        (1 as libc::c_ulonglong)
                            << 3 as libc::c_int
                                + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
                    ),
            )
            .wrapping_add(1 as libc::c_int as libc::c_ulonglong)
    {
        return 0 as *mut mi_segmap_part_t;
    }
    let mut part: *mut mi_segmap_part_t = ::core::intrinsics::atomic_load_relaxed(
        &mut *mi_segment_map.as_mut_ptr().offset(segindex as isize) as *mut *mut mi_segmap_part_t,
    );
    if part.is_null() {
        if !create_on_demand {
            return 0 as *mut mi_segmap_part_t;
        }
        let mut memid: mi_memid_t = mi_memid_s {
            mem: C2RustUnnamed_0 {
                os: mi_memid_os_info {
                    base: 0 as *mut libc::c_void,
                    alignment: 0,
                },
            },
            is_pinned: false,
            initially_committed: false,
            initially_zero: false,
            memkind: MI_MEM_NONE,
        };
        part = _mi_os_alloc(
            ::core::mem::size_of::<mi_segmap_part_t>() as libc::c_ulong,
            &mut memid,
            0 as *mut mi_stats_t,
        ) as *mut mi_segmap_part_t;
        if part.is_null() {
            return 0 as *mut mi_segmap_part_t;
        }
        let mut expected: *mut mi_segmap_part_t = 0 as *mut mi_segmap_part_t;
        let fresh48 = ::core::intrinsics::atomic_cxchg_release_relaxed(
            &mut *mi_segment_map.as_mut_ptr().offset(segindex as isize)
                as *mut *mut mi_segmap_part_t,
            *(&mut expected as *mut *mut mi_segmap_part_t),
            part,
        );
        *(&mut expected as *mut *mut mi_segmap_part_t) = fresh48.0;
        if !fresh48.1 {
            _mi_os_free(
                part as *mut libc::c_void,
                ::core::mem::size_of::<mi_segmap_part_t>() as libc::c_ulong,
                memid,
                0 as *mut mi_stats_t,
            );
            part = expected;
            if part.is_null() {
                return 0 as *mut mi_segmap_part_t;
            }
        }
    }
    if !part.is_null() {
    } else {
        _mi_assert_fail(
            b"part != NULL\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            9188 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 24], &[libc::c_char; 24]>(
                b"mi_segment_map_index_of\0",
            ))
            .as_ptr(),
        );
    };
    let offset: uintptr_t = (segment as uintptr_t as libc::c_ulonglong).wrapping_rem(
        (8 as libc::c_int as libc::c_ulonglong)
            .wrapping_mul(
                (((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_sub(128 as libc::c_int as libc::c_ulonglong),
            )
            .wrapping_mul(
                (1 as libc::c_ulonglong)
                    << 3 as libc::c_int
                        + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
            ),
    ) as uintptr_t;
    let bitofs: uintptr_t = (offset as libc::c_ulonglong).wrapping_div(
        (1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)),
    ) as uintptr_t;
    *idx = bitofs / (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as uintptr_t;
    *bitidx = bitofs % (((1 as libc::c_int) << 3 as libc::c_int) * 8 as libc::c_int) as uintptr_t;
    return part;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segment_map_allocated_at(mut segment: *const mi_segment_t) {
    if (*segment).memid.memkind as libc::c_uint == MI_MEM_ARENA as libc::c_int as libc::c_uint {
        return;
    }
    let mut index: size_t = 0;
    let mut bitidx: size_t = 0;
    let mut part: *mut mi_segmap_part_t =
        mi_segment_map_index_of(segment, 1 as libc::c_int != 0, &mut index, &mut bitidx);
    if part.is_null() {
        return;
    }
    let mut mask: uintptr_t = ::core::intrinsics::atomic_load_relaxed(
        &mut *((*part).map).as_mut_ptr().offset(index as isize) as *mut uintptr_t,
    );
    let mut newmask: uintptr_t = 0;
    loop {
        newmask = mask | (1 as libc::c_int as uintptr_t) << bitidx;
        let fresh49 = ::core::intrinsics::atomic_cxchgweak_release_relaxed(
            &mut *((*part).map).as_mut_ptr().offset(index as isize) as *mut uintptr_t,
            *(&mut mask as *mut uintptr_t),
            newmask,
        );
        *(&mut mask as *mut uintptr_t) = fresh49.0;
        if fresh49.1 {
            break;
        }
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_segment_map_freed_at(mut segment: *const mi_segment_t) {
    if (*segment).memid.memkind as libc::c_uint == MI_MEM_ARENA as libc::c_int as libc::c_uint {
        return;
    }
    let mut index: size_t = 0;
    let mut bitidx: size_t = 0;
    let mut part: *mut mi_segmap_part_t =
        mi_segment_map_index_of(segment, 0 as libc::c_int != 0, &mut index, &mut bitidx);
    if part.is_null() {
        return;
    }
    let mut mask: uintptr_t = ::core::intrinsics::atomic_load_relaxed(
        &mut *((*part).map).as_mut_ptr().offset(index as isize) as *mut uintptr_t,
    );
    let mut newmask: uintptr_t = 0;
    loop {
        newmask = mask & !((1 as libc::c_int as uintptr_t) << bitidx);
        let fresh50 = ::core::intrinsics::atomic_cxchgweak_release_relaxed(
            &mut *((*part).map).as_mut_ptr().offset(index as isize) as *mut uintptr_t,
            *(&mut mask as *mut uintptr_t),
            newmask,
        );
        *(&mut mask as *mut uintptr_t) = fresh50.0;
        if fresh50.1 {
            break;
        }
    }
}
unsafe extern "C" fn _mi_segment_of(mut p: *const libc::c_void) -> *mut mi_segment_t {
    if p.is_null() {
        return 0 as *mut mi_segment_t;
    }
    let mut segment: *mut mi_segment_t = _mi_ptr_segment(p);
    let mut index: size_t = 0;
    let mut bitidx: size_t = 0;
    let mut part: *mut mi_segmap_part_t =
        mi_segment_map_index_of(segment, 0 as libc::c_int != 0, &mut index, &mut bitidx);
    if part.is_null() {
        return 0 as *mut mi_segment_t;
    }
    let mask: uintptr_t = ::core::intrinsics::atomic_load_relaxed(
        &mut *((*part).map).as_mut_ptr().offset(index as isize) as *mut uintptr_t,
    );
    if (mask & (1 as libc::c_int as uintptr_t) << bitidx != 0 as libc::c_int as uintptr_t)
        as libc::c_int as libc::c_long
        != 0
    {
        let mut cookie_ok: bool =
            _mi_ptr_cookie(segment as *const libc::c_void) == (*segment).cookie;
        if cookie_ok as libc::c_int != 0 {
        } else {
            _mi_assert_fail(
                b"cookie_ok\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                9229 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_segment_of\0"))
                    .as_ptr(),
            );
        };
        return segment;
    }
    return 0 as *mut mi_segment_t;
}
unsafe extern "C" fn mi_is_valid_pointer(mut p: *const libc::c_void) -> bool {
    return _mi_arena_contains(p) as libc::c_int != 0 || !(_mi_segment_of(p)).is_null();
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_is_in_heap_region(mut p: *const libc::c_void) -> bool {
    return mi_is_valid_pointer(p);
}
unsafe extern "C" fn mi_is_in_main(mut stat: *mut libc::c_void) -> bool {
    return stat as *mut uint8_t >= &mut _mi_stats_main as *mut mi_stats_t as *mut uint8_t
        && (stat as *mut uint8_t)
            < (&mut _mi_stats_main as *mut mi_stats_t as *mut uint8_t)
                .offset(::core::mem::size_of::<mi_stats_t>() as libc::c_ulong as isize);
}
unsafe extern "C" fn mi_stat_update(mut stat: *mut mi_stat_count_t, mut amount: int64_t) {
    if amount == 0 as libc::c_int as int64_t {
        return;
    }
    if mi_is_in_main(stat as *mut libc::c_void) {
        let mut current: int64_t =
            mi_atomic_addi64_relaxed(&mut (*stat).current as *mut int64_t as *mut int64_t, amount);
        mi_atomic_maxi64_relaxed(
            &mut (*stat).peak as *mut int64_t as *mut int64_t,
            current + amount,
        );
        if amount > 0 as libc::c_int as int64_t {
            mi_atomic_addi64_relaxed(
                &mut (*stat).allocated as *mut int64_t as *mut int64_t,
                amount,
            );
        } else {
            mi_atomic_addi64_relaxed(&mut (*stat).freed as *mut int64_t as *mut int64_t, -amount);
        }
    } else {
        (*stat).current += amount;
        if (*stat).current > (*stat).peak {
            (*stat).peak = (*stat).current;
        }
        if amount > 0 as libc::c_int as int64_t {
            (*stat).allocated += amount;
        } else {
            (*stat).freed += -amount;
        }
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_stat_counter_increase(
    mut stat: *mut mi_stat_counter_t,
    mut amount: size_t,
) {
    if mi_is_in_main(stat as *mut libc::c_void) {
        mi_atomic_addi64_relaxed(
            &mut (*stat).count as *mut int64_t as *mut int64_t,
            1 as libc::c_int as int64_t,
        );
        mi_atomic_addi64_relaxed(
            &mut (*stat).total as *mut int64_t as *mut int64_t,
            amount as int64_t,
        );
    } else {
        (*stat).count += 1;
        (*stat).count;
        (*stat).total = ((*stat).total as size_t).wrapping_add(amount) as int64_t as int64_t;
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_stat_increase(mut stat: *mut mi_stat_count_t, mut amount: size_t) {
    mi_stat_update(stat, amount as int64_t);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_stat_decrease(mut stat: *mut mi_stat_count_t, mut amount: size_t) {
    mi_stat_update(stat, -(amount as int64_t));
}
unsafe extern "C" fn mi_stat_add(
    mut stat: *mut mi_stat_count_t,
    mut src: *const mi_stat_count_t,
    mut unit: int64_t,
) {
    if stat == src as *mut mi_stat_count_t {
        return;
    }
    if (*src).allocated == 0 as libc::c_int as int64_t
        && (*src).freed == 0 as libc::c_int as int64_t
    {
        return;
    }
    mi_atomic_addi64_relaxed(
        &mut (*stat).allocated as *mut int64_t as *mut int64_t,
        (*src).allocated * unit,
    );
    mi_atomic_addi64_relaxed(
        &mut (*stat).current as *mut int64_t as *mut int64_t,
        (*src).current * unit,
    );
    mi_atomic_addi64_relaxed(
        &mut (*stat).freed as *mut int64_t as *mut int64_t,
        (*src).freed * unit,
    );
    mi_atomic_addi64_relaxed(
        &mut (*stat).peak as *mut int64_t as *mut int64_t,
        (*src).peak * unit,
    );
}
unsafe extern "C" fn mi_stat_counter_add(
    mut stat: *mut mi_stat_counter_t,
    mut src: *const mi_stat_counter_t,
    mut unit: int64_t,
) {
    if stat == src as *mut mi_stat_counter_t {
        return;
    }
    mi_atomic_addi64_relaxed(
        &mut (*stat).total as *mut int64_t as *mut int64_t,
        (*src).total * unit,
    );
    mi_atomic_addi64_relaxed(
        &mut (*stat).count as *mut int64_t as *mut int64_t,
        (*src).count * unit,
    );
}
unsafe extern "C" fn mi_stats_add(mut stats: *mut mi_stats_t, mut src: *const mi_stats_t) {
    if stats == src as *mut mi_stats_t {
        return;
    }
    mi_stat_add(
        &mut (*stats).segments,
        &(*src).segments,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).pages,
        &(*src).pages,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).reserved,
        &(*src).reserved,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).committed,
        &(*src).committed,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).reset,
        &(*src).reset,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).purged,
        &(*src).purged,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).page_committed,
        &(*src).page_committed,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).pages_abandoned,
        &(*src).pages_abandoned,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).segments_abandoned,
        &(*src).segments_abandoned,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).threads,
        &(*src).threads,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).malloc,
        &(*src).malloc,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).segments_cache,
        &(*src).segments_cache,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).normal,
        &(*src).normal,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).huge,
        &(*src).huge,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(
        &mut (*stats).giant,
        &(*src).giant,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).pages_extended,
        &(*src).pages_extended,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).mmap_calls,
        &(*src).mmap_calls,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).commit_calls,
        &(*src).commit_calls,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).reset_calls,
        &(*src).reset_calls,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).purge_calls,
        &(*src).purge_calls,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).page_no_retire,
        &(*src).page_no_retire,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).searches,
        &(*src).searches,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).normal_count,
        &(*src).normal_count,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).huge_count,
        &(*src).huge_count,
        1 as libc::c_int as int64_t,
    );
    mi_stat_counter_add(
        &mut (*stats).guarded_alloc_count,
        &(*src).guarded_alloc_count,
        1 as libc::c_int as int64_t,
    );
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i <= 73 as libc::c_uint as size_t {
        if (*src).normal_bins[i as usize].allocated > 0 as libc::c_int as int64_t
            || (*src).normal_bins[i as usize].freed > 0 as libc::c_int as int64_t
        {
            mi_stat_add(
                &mut *((*stats).normal_bins).as_mut_ptr().offset(i as isize),
                &*((*src).normal_bins).as_ptr().offset(i as isize),
                1 as libc::c_int as int64_t,
            );
        }
        i = i.wrapping_add(1);
        i;
    }
}
unsafe extern "C" fn mi_printf_amount(
    mut n: int64_t,
    mut unit: int64_t,
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
    mut fmt: *const libc::c_char,
) {
    let mut buf: [libc::c_char; 32] = [0; 32];
    buf[0 as libc::c_int as usize] = 0 as libc::c_int as libc::c_char;
    let mut len: libc::c_int = 32 as libc::c_int;
    let mut suffix: *const libc::c_char = if unit <= 0 as libc::c_int as int64_t {
        b" \0" as *const u8 as *const libc::c_char
    } else {
        b"B\0" as *const u8 as *const libc::c_char
    };
    let base: int64_t = (if unit == 0 as libc::c_int as int64_t {
        1000 as libc::c_int
    } else {
        1024 as libc::c_int
    }) as int64_t;
    if unit > 0 as libc::c_int as int64_t {
        n *= unit;
    }
    let pos: int64_t = if n < 0 as libc::c_int as int64_t {
        -n
    } else {
        n
    };
    if pos < base {
        if n != 1 as libc::c_int as int64_t
            || *suffix.offset(0 as libc::c_int as isize) as libc::c_int != 'B' as i32
        {
            _mi_snprintf(
                buf.as_mut_ptr(),
                len as size_t,
                b"%lld   %-3s\0" as *const u8 as *const libc::c_char,
                n as libc::c_longlong,
                if n == 0 as libc::c_int as int64_t {
                    b"\0" as *const u8 as *const libc::c_char
                } else {
                    suffix
                },
            );
        }
    } else {
        let mut divider: int64_t = base;
        let mut magnitude: *const libc::c_char = b"K\0" as *const u8 as *const libc::c_char;
        if pos >= divider * base {
            divider *= base;
            magnitude = b"M\0" as *const u8 as *const libc::c_char;
        }
        if pos >= divider * base {
            divider *= base;
            magnitude = b"G\0" as *const u8 as *const libc::c_char;
        }
        let tens: int64_t = n / (divider / 10 as libc::c_int as int64_t);
        let whole: libc::c_long = tens / 10 as libc::c_int as int64_t;
        let frac1: libc::c_long = tens % 10 as libc::c_int as int64_t;
        let mut unitdesc: [libc::c_char; 8] = [0; 8];
        _mi_snprintf(
            unitdesc.as_mut_ptr(),
            8 as libc::c_int as size_t,
            b"%s%s%s\0" as *const u8 as *const libc::c_char,
            magnitude,
            if base == 1024 as libc::c_int as int64_t {
                b"i\0" as *const u8 as *const libc::c_char
            } else {
                b"\0" as *const u8 as *const libc::c_char
            },
            suffix,
        );
        _mi_snprintf(
            buf.as_mut_ptr(),
            len as size_t,
            b"%ld.%ld %-3s\0" as *const u8 as *const libc::c_char,
            whole,
            if frac1 < 0 as libc::c_int as libc::c_long {
                -frac1
            } else {
                frac1
            },
            unitdesc.as_mut_ptr(),
        );
    }
    _mi_fprintf(
        out,
        arg,
        if fmt.is_null() {
            b"%12s\0" as *const u8 as *const libc::c_char
        } else {
            fmt
        },
        buf.as_mut_ptr(),
    );
}
unsafe extern "C" fn mi_print_amount(
    mut n: int64_t,
    mut unit: int64_t,
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    mi_printf_amount(n, unit, out, arg, 0 as *const libc::c_char);
}
unsafe extern "C" fn mi_print_count(
    mut n: int64_t,
    mut unit: int64_t,
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    if unit == 1 as libc::c_int as int64_t {
        _mi_fprintf(
            out,
            arg,
            b"%12s\0" as *const u8 as *const libc::c_char,
            b" \0" as *const u8 as *const libc::c_char,
        );
    } else {
        mi_print_amount(n, 0 as libc::c_int as int64_t, out, arg);
    };
}
unsafe extern "C" fn mi_stat_print_ex(
    mut stat: *const mi_stat_count_t,
    mut msg: *const libc::c_char,
    mut unit: int64_t,
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
    mut notok: *const libc::c_char,
) {
    _mi_fprintf(
        out,
        arg,
        b"%10s:\0" as *const u8 as *const libc::c_char,
        msg,
    );
    if unit != 0 as libc::c_int as int64_t {
        if unit > 0 as libc::c_int as int64_t {
            mi_print_amount((*stat).peak, unit, out, arg);
            mi_print_amount((*stat).allocated, unit, out, arg);
            mi_print_amount((*stat).freed, unit, out, arg);
            mi_print_amount((*stat).current, unit, out, arg);
            mi_print_amount(unit, 1 as libc::c_int as int64_t, out, arg);
            mi_print_count((*stat).allocated, unit, out, arg);
        } else {
            mi_print_amount((*stat).peak, -(1 as libc::c_int) as int64_t, out, arg);
            mi_print_amount((*stat).allocated, -(1 as libc::c_int) as int64_t, out, arg);
            mi_print_amount((*stat).freed, -(1 as libc::c_int) as int64_t, out, arg);
            mi_print_amount((*stat).current, -(1 as libc::c_int) as int64_t, out, arg);
            if unit == -(1 as libc::c_int) as int64_t {
                _mi_fprintf(
                    out,
                    arg,
                    b"%24s\0" as *const u8 as *const libc::c_char,
                    b"\0" as *const u8 as *const libc::c_char,
                );
            } else {
                mi_print_amount(-unit, 1 as libc::c_int as int64_t, out, arg);
                mi_print_count(
                    (*stat).allocated / -unit,
                    0 as libc::c_int as int64_t,
                    out,
                    arg,
                );
            }
        }
        if (*stat).allocated > (*stat).freed {
            _mi_fprintf(out, arg, b"  \0" as *const u8 as *const libc::c_char);
            _mi_fprintf(
                out,
                arg,
                if notok.is_null() {
                    b"not all freed\0" as *const u8 as *const libc::c_char
                } else {
                    notok
                },
            );
            _mi_fprintf(out, arg, b"\n\0" as *const u8 as *const libc::c_char);
        } else {
            _mi_fprintf(out, arg, b"  ok\n\0" as *const u8 as *const libc::c_char);
        }
    } else {
        mi_print_amount((*stat).peak, 1 as libc::c_int as int64_t, out, arg);
        mi_print_amount((*stat).allocated, 1 as libc::c_int as int64_t, out, arg);
        _mi_fprintf(
            out,
            arg,
            b"%11s\0" as *const u8 as *const libc::c_char,
            b" \0" as *const u8 as *const libc::c_char,
        );
        mi_print_amount((*stat).current, 1 as libc::c_int as int64_t, out, arg);
        _mi_fprintf(out, arg, b"\n\0" as *const u8 as *const libc::c_char);
    };
}
unsafe extern "C" fn mi_stat_print(
    mut stat: *const mi_stat_count_t,
    mut msg: *const libc::c_char,
    mut unit: int64_t,
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    mi_stat_print_ex(stat, msg, unit, out, arg, 0 as *const libc::c_char);
}
unsafe extern "C" fn mi_stat_peak_print(
    mut stat: *const mi_stat_count_t,
    mut msg: *const libc::c_char,
    mut unit: int64_t,
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    _mi_fprintf(
        out,
        arg,
        b"%10s:\0" as *const u8 as *const libc::c_char,
        msg,
    );
    mi_print_amount((*stat).peak, unit, out, arg);
    _mi_fprintf(out, arg, b"\n\0" as *const u8 as *const libc::c_char);
}
unsafe extern "C" fn mi_stat_counter_print(
    mut stat: *const mi_stat_counter_t,
    mut msg: *const libc::c_char,
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    _mi_fprintf(
        out,
        arg,
        b"%10s:\0" as *const u8 as *const libc::c_char,
        msg,
    );
    mi_print_amount((*stat).total, -(1 as libc::c_int) as int64_t, out, arg);
    _mi_fprintf(out, arg, b"\n\0" as *const u8 as *const libc::c_char);
}
unsafe extern "C" fn mi_stat_counter_print_avg(
    mut stat: *const mi_stat_counter_t,
    mut msg: *const libc::c_char,
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    let avg_tens: int64_t = if (*stat).count == 0 as libc::c_int as int64_t {
        0 as libc::c_int as int64_t
    } else {
        (*stat).total * 10 as libc::c_int as int64_t / (*stat).count
    };
    let avg_whole: libc::c_long = avg_tens / 10 as libc::c_int as int64_t;
    let avg_frac1: libc::c_long = avg_tens % 10 as libc::c_int as int64_t;
    _mi_fprintf(
        out,
        arg,
        b"%10s: %5ld.%ld avg\n\0" as *const u8 as *const libc::c_char,
        msg,
        avg_whole,
        avg_frac1,
    );
}
unsafe extern "C" fn mi_print_header(mut out: Option<mi_output_fun>, mut arg: *mut libc::c_void) {
    _mi_fprintf(
        out,
        arg,
        b"%10s: %11s %11s %11s %11s %11s %11s\n\0" as *const u8 as *const libc::c_char,
        b"heap stats\0" as *const u8 as *const libc::c_char,
        b"peak   \0" as *const u8 as *const libc::c_char,
        b"total   \0" as *const u8 as *const libc::c_char,
        b"freed   \0" as *const u8 as *const libc::c_char,
        b"current   \0" as *const u8 as *const libc::c_char,
        b"unit   \0" as *const u8 as *const libc::c_char,
        b"count   \0" as *const u8 as *const libc::c_char,
    );
}
unsafe extern "C" fn mi_stats_print_bins(
    mut bins: *const mi_stat_count_t,
    mut max: size_t,
    mut fmt: *const libc::c_char,
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    let mut found: bool = 0 as libc::c_int != 0;
    let mut buf: [libc::c_char; 64] = [0; 64];
    let mut i: size_t = 0 as libc::c_int as size_t;
    while i <= max {
        if (*bins.offset(i as isize)).allocated > 0 as libc::c_int as int64_t {
            found = 1 as libc::c_int != 0;
            let mut unit: int64_t = _mi_bin_size(i as uint8_t) as int64_t;
            _mi_snprintf(
                buf.as_mut_ptr(),
                64 as libc::c_int as size_t,
                b"%s %3lu\0" as *const u8 as *const libc::c_char,
                fmt,
                i as libc::c_long,
            );
            mi_stat_print(&*bins.offset(i as isize), buf.as_mut_ptr(), unit, out, arg);
        }
        i = i.wrapping_add(1);
        i;
    }
    if found {
        _mi_fprintf(out, arg, b"\n\0" as *const u8 as *const libc::c_char);
        mi_print_header(out, arg);
    }
}
unsafe extern "C" fn mi_buffered_flush(mut buf: *mut buffered_t) {
    *((*buf).buf).offset((*buf).used as isize) = 0 as libc::c_int as libc::c_char;
    _mi_fputs((*buf).out, (*buf).arg, 0 as *const libc::c_char, (*buf).buf);
    (*buf).used = 0 as libc::c_int as size_t;
}
unsafe extern "C" fn mi_buffered_out(mut msg: *const libc::c_char, mut arg: *mut libc::c_void) {
    let mut buf: *mut buffered_t = arg as *mut buffered_t;
    if msg.is_null() || buf.is_null() {
        return;
    }
    let mut src: *const libc::c_char = msg;
    while *src as libc::c_int != 0 as libc::c_int {
        let mut c: libc::c_char = *src;
        if (*buf).used >= (*buf).count {
            mi_buffered_flush(buf);
        }
        if (*buf).used < (*buf).count {
        } else {
            _mi_assert_fail(
                b"buf->used < buf->count\0" as *const u8 as *const libc::c_char,
                b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
                9468 as libc::c_int as libc::c_uint,
                (*::core::mem::transmute::<&[u8; 16], &[libc::c_char; 16]>(b"mi_buffered_out\0"))
                    .as_ptr(),
            );
        };
        let fresh51 = (*buf).used;
        (*buf).used = ((*buf).used).wrapping_add(1);
        *((*buf).buf).offset(fresh51 as isize) = c;
        if c as libc::c_int == '\n' as i32 {
            mi_buffered_flush(buf);
        }
        src = src.offset(1);
        src;
    }
}
unsafe extern "C" fn _mi_stats_print(
    mut stats: *mut mi_stats_t,
    mut out0: Option<mi_output_fun>,
    mut arg0: *mut libc::c_void,
) {
    let mut buf: [libc::c_char; 256] = [0; 256];
    let mut buffer: buffered_t = {
        let mut init = buffered_s {
            out: out0,
            arg: arg0,
            buf: 0 as *mut libc::c_char,
            used: 0 as libc::c_int as size_t,
            count: 255 as libc::c_int as size_t,
        };
        init
    };
    buffer.buf = buf.as_mut_ptr();
    let mut out: Option<mi_output_fun> =
        Some(mi_buffered_out as unsafe extern "C" fn(*const libc::c_char, *mut libc::c_void) -> ());
    let mut arg: *mut libc::c_void = &mut buffer as *mut buffered_t as *mut libc::c_void;
    mi_print_header(out, arg);
    mi_stats_print_bins(
        ((*stats).normal_bins).as_mut_ptr(),
        73 as libc::c_uint as size_t,
        b"normal\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).normal,
        b"normal\0" as *const u8 as *const libc::c_char,
        if (*stats).normal_count.count == 0 as libc::c_int as int64_t {
            1 as libc::c_int as int64_t
        } else {
            -((*stats).normal.allocated / (*stats).normal_count.count)
        },
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).huge,
        b"huge\0" as *const u8 as *const libc::c_char,
        if (*stats).huge_count.count == 0 as libc::c_int as int64_t {
            1 as libc::c_int as int64_t
        } else {
            -((*stats).huge.allocated / (*stats).huge_count.count)
        },
        out,
        arg,
    );
    let mut total: mi_stat_count_t = {
        let mut init = mi_stat_count_s {
            allocated: 0 as libc::c_int as int64_t,
            freed: 0 as libc::c_int as int64_t,
            peak: 0 as libc::c_int as int64_t,
            current: 0 as libc::c_int as int64_t,
        };
        init
    };
    mi_stat_add(
        &mut total,
        &mut (*stats).normal,
        1 as libc::c_int as int64_t,
    );
    mi_stat_add(&mut total, &mut (*stats).huge, 1 as libc::c_int as int64_t);
    mi_stat_print(
        &mut total,
        b"total\0" as *const u8 as *const libc::c_char,
        1 as libc::c_int as int64_t,
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).malloc,
        b"malloc req\0" as *const u8 as *const libc::c_char,
        1 as libc::c_int as int64_t,
        out,
        arg,
    );
    _mi_fprintf(out, arg, b"\n\0" as *const u8 as *const libc::c_char);
    mi_stat_print_ex(
        &mut (*stats).reserved,
        b"reserved\0" as *const u8 as *const libc::c_char,
        1 as libc::c_int as int64_t,
        out,
        arg,
        b"\0" as *const u8 as *const libc::c_char,
    );
    mi_stat_print_ex(
        &mut (*stats).committed,
        b"committed\0" as *const u8 as *const libc::c_char,
        1 as libc::c_int as int64_t,
        out,
        arg,
        b"\0" as *const u8 as *const libc::c_char,
    );
    mi_stat_peak_print(
        &mut (*stats).reset,
        b"reset\0" as *const u8 as *const libc::c_char,
        1 as libc::c_int as int64_t,
        out,
        arg,
    );
    mi_stat_peak_print(
        &mut (*stats).purged,
        b"purged\0" as *const u8 as *const libc::c_char,
        1 as libc::c_int as int64_t,
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).page_committed,
        b"touched\0" as *const u8 as *const libc::c_char,
        1 as libc::c_int as int64_t,
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).segments,
        b"segments\0" as *const u8 as *const libc::c_char,
        -(1 as libc::c_int) as int64_t,
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).segments_abandoned,
        b"-abandoned\0" as *const u8 as *const libc::c_char,
        -(1 as libc::c_int) as int64_t,
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).segments_cache,
        b"-cached\0" as *const u8 as *const libc::c_char,
        -(1 as libc::c_int) as int64_t,
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).pages,
        b"pages\0" as *const u8 as *const libc::c_char,
        -(1 as libc::c_int) as int64_t,
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).pages_abandoned,
        b"-abandoned\0" as *const u8 as *const libc::c_char,
        -(1 as libc::c_int) as int64_t,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).pages_extended,
        b"-extended\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).page_no_retire,
        b"-noretire\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).arena_count,
        b"arenas\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).arena_crossover_count,
        b"-crossover\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).arena_rollback_count,
        b"-rollback\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).mmap_calls,
        b"mmaps\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).commit_calls,
        b"commits\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).reset_calls,
        b"resets\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).purge_calls,
        b"purges\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_counter_print(
        &mut (*stats).guarded_alloc_count,
        b"guarded\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    mi_stat_print(
        &mut (*stats).threads,
        b"threads\0" as *const u8 as *const libc::c_char,
        -(1 as libc::c_int) as int64_t,
        out,
        arg,
    );
    mi_stat_counter_print_avg(
        &mut (*stats).searches,
        b"searches\0" as *const u8 as *const libc::c_char,
        out,
        arg,
    );
    _mi_fprintf(
        out,
        arg,
        b"%10s: %5zu\n\0" as *const u8 as *const libc::c_char,
        b"numa nodes\0" as *const u8 as *const libc::c_char,
        _mi_os_numa_node_count(),
    );
    let mut elapsed: size_t = 0;
    let mut user_time: size_t = 0;
    let mut sys_time: size_t = 0;
    let mut current_rss: size_t = 0;
    let mut peak_rss: size_t = 0;
    let mut current_commit: size_t = 0;
    let mut peak_commit: size_t = 0;
    let mut page_faults: size_t = 0;
    mi_process_info(
        &mut elapsed,
        &mut user_time,
        &mut sys_time,
        &mut current_rss,
        &mut peak_rss,
        &mut current_commit,
        &mut peak_commit,
        &mut page_faults,
    );
    _mi_fprintf(
        out,
        arg,
        b"%10s: %5ld.%03ld s\n\0" as *const u8 as *const libc::c_char,
        b"elapsed\0" as *const u8 as *const libc::c_char,
        elapsed / 1000 as libc::c_int as size_t,
        elapsed % 1000 as libc::c_int as size_t,
    );
    _mi_fprintf(
        out,
        arg,
        b"%10s: user: %ld.%03ld s, system: %ld.%03ld s, faults: %lu, rss: \0" as *const u8
            as *const libc::c_char,
        b"process\0" as *const u8 as *const libc::c_char,
        user_time / 1000 as libc::c_int as size_t,
        user_time % 1000 as libc::c_int as size_t,
        sys_time / 1000 as libc::c_int as size_t,
        sys_time % 1000 as libc::c_int as size_t,
        page_faults,
    );
    mi_printf_amount(
        peak_rss as int64_t,
        1 as libc::c_int as int64_t,
        out,
        arg,
        b"%s\0" as *const u8 as *const libc::c_char,
    );
    if peak_commit > 0 as libc::c_int as size_t {
        _mi_fprintf(
            out,
            arg,
            b", commit: \0" as *const u8 as *const libc::c_char,
        );
        mi_printf_amount(
            peak_commit as int64_t,
            1 as libc::c_int as int64_t,
            out,
            arg,
            b"%s\0" as *const u8 as *const libc::c_char,
        );
    }
    _mi_fprintf(out, arg, b"\n\0" as *const u8 as *const libc::c_char);
}
static mut mi_process_start: mi_msecs_t = 0;
unsafe extern "C" fn mi_stats_get_default() -> *mut mi_stats_t {
    let mut heap: *mut mi_heap_t = mi_heap_get_default();
    return &mut (*(*heap).tld).stats;
}
unsafe extern "C" fn mi_stats_merge_from(mut stats: *mut mi_stats_t) {
    if stats != &mut _mi_stats_main as *mut mi_stats_t {
        mi_stats_add(&mut _mi_stats_main, stats);
        memset(
            stats as *mut libc::c_void,
            0 as libc::c_int,
            ::core::mem::size_of::<mi_stats_t>() as libc::c_ulong,
        );
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_stats_reset() {
    let mut stats: *mut mi_stats_t = mi_stats_get_default();
    if stats != &mut _mi_stats_main as *mut mi_stats_t {
        memset(
            stats as *mut libc::c_void,
            0 as libc::c_int,
            ::core::mem::size_of::<mi_stats_t>() as libc::c_ulong,
        );
    }
    memset(
        &mut _mi_stats_main as *mut mi_stats_t as *mut libc::c_void,
        0 as libc::c_int,
        ::core::mem::size_of::<mi_stats_t>() as libc::c_ulong,
    );
    if mi_process_start == 0 as libc::c_int as mi_msecs_t {
        mi_process_start = _mi_clock_start();
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_stats_merge() {
    mi_stats_merge_from(mi_stats_get_default());
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_stats_done(mut stats: *mut mi_stats_t) {
    mi_stats_merge_from(stats);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_stats_print_out(
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    mi_stats_merge_from(mi_stats_get_default());
    _mi_stats_print(&mut _mi_stats_main, out, arg);
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_stats_print(mut out: *mut libc::c_void) {
    mi_stats_print_out(
        ::core::mem::transmute::<*mut libc::c_void, Option<mi_output_fun>>(out),
        0 as *mut libc::c_void,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_thread_stats_print_out(
    mut out: Option<mi_output_fun>,
    mut arg: *mut libc::c_void,
) {
    _mi_stats_print(mi_stats_get_default(), out, arg);
}
static mut mi_clock_diff: mi_msecs_t = 0;
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_clock_now() -> mi_msecs_t {
    return _mi_prim_clock_now();
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_clock_start() -> mi_msecs_t {
    if mi_clock_diff as libc::c_double == 0.0f64 {
        let mut t0: mi_msecs_t = _mi_clock_now();
        mi_clock_diff = _mi_clock_now() - t0;
    }
    return _mi_clock_now();
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_clock_end(mut start: mi_msecs_t) -> mi_msecs_t {
    let mut end: mi_msecs_t = _mi_clock_now();
    return end - start - mi_clock_diff;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn mi_process_info(
    mut elapsed_msecs: *mut size_t,
    mut user_msecs: *mut size_t,
    mut system_msecs: *mut size_t,
    mut current_rss: *mut size_t,
    mut peak_rss: *mut size_t,
    mut current_commit: *mut size_t,
    mut peak_commit: *mut size_t,
    mut page_faults: *mut size_t,
) {
    let mut pinfo: mi_process_info_t = mi_process_info_s {
        elapsed: 0,
        utime: 0,
        stime: 0,
        current_rss: 0,
        peak_rss: 0,
        current_commit: 0,
        peak_commit: 0,
        page_faults: 0,
    };
    memset(
        &mut pinfo as *mut mi_process_info_t as *mut libc::c_void,
        0 as libc::c_int,
        ::core::mem::size_of::<mi_process_info_t>() as libc::c_ulong,
    );
    pinfo.elapsed = _mi_clock_end(mi_process_start);
    pinfo.current_commit = ::core::intrinsics::atomic_load_relaxed(
        &mut _mi_stats_main.committed.current as *mut int64_t as *mut int64_t,
    ) as size_t;
    pinfo.peak_commit = ::core::intrinsics::atomic_load_relaxed(
        &mut _mi_stats_main.committed.peak as *mut int64_t as *mut int64_t,
    ) as size_t;
    pinfo.current_rss = pinfo.current_commit;
    pinfo.peak_rss = pinfo.peak_commit;
    pinfo.utime = 0 as libc::c_int as mi_msecs_t;
    pinfo.stime = 0 as libc::c_int as mi_msecs_t;
    pinfo.page_faults = 0 as libc::c_int as size_t;
    _mi_prim_process_info(&mut pinfo);
    if !elapsed_msecs.is_null() {
        *elapsed_msecs = if pinfo.elapsed < 0 as libc::c_int as mi_msecs_t {
            0 as libc::c_int as size_t
        } else if pinfo.elapsed < 9223372036854775807 as libc::c_long {
            pinfo.elapsed as size_t
        } else {
            9223372036854775807 as libc::c_long as size_t
        };
    }
    if !user_msecs.is_null() {
        *user_msecs = if pinfo.utime < 0 as libc::c_int as mi_msecs_t {
            0 as libc::c_int as size_t
        } else if pinfo.utime < 9223372036854775807 as libc::c_long {
            pinfo.utime as size_t
        } else {
            9223372036854775807 as libc::c_long as size_t
        };
    }
    if !system_msecs.is_null() {
        *system_msecs = if pinfo.stime < 0 as libc::c_int as mi_msecs_t {
            0 as libc::c_int as size_t
        } else if pinfo.stime < 9223372036854775807 as libc::c_long {
            pinfo.stime as size_t
        } else {
            9223372036854775807 as libc::c_long as size_t
        };
    }
    if !current_rss.is_null() {
        *current_rss = pinfo.current_rss;
    }
    if !peak_rss.is_null() {
        *peak_rss = pinfo.peak_rss;
    }
    if !current_commit.is_null() {
        *current_commit = pinfo.current_commit;
    }
    if !peak_commit.is_null() {
        *peak_commit = pinfo.peak_commit;
    }
    if !page_faults.is_null() {
        *page_faults = pinfo.page_faults;
    }
}
#[inline]
unsafe extern "C" fn mi_prim_open(
    mut fpath: *const libc::c_char,
    mut open_flags: libc::c_int,
) -> libc::c_int {
    return syscall(
        2 as libc::c_int as libc::c_long,
        fpath,
        open_flags,
        0 as libc::c_int,
    ) as libc::c_int;
}
#[inline]
unsafe extern "C" fn mi_prim_read(
    mut fd: libc::c_int,
    mut buf: *mut libc::c_void,
    mut bufsize: size_t,
) -> ssize_t {
    return syscall(0 as libc::c_int as libc::c_long, fd, buf, bufsize);
}
#[inline]
unsafe extern "C" fn mi_prim_close(mut fd: libc::c_int) -> libc::c_int {
    return syscall(3 as libc::c_int as libc::c_long, fd) as libc::c_int;
}
#[inline]
unsafe extern "C" fn mi_prim_access(
    mut fpath: *const libc::c_char,
    mut mode: libc::c_int,
) -> libc::c_int {
    return syscall(21 as libc::c_int as libc::c_long, fpath, mode) as libc::c_int;
}
unsafe extern "C" fn unix_detect_overcommit() -> bool {
    let mut os_overcommit: bool = 1 as libc::c_int != 0;
    let mut fd: libc::c_int = mi_prim_open(
        b"/proc/sys/vm/overcommit_memory\0" as *const u8 as *const libc::c_char,
        0 as libc::c_int,
    );
    if fd >= 0 as libc::c_int {
        let mut buf: [libc::c_char; 32] = [0; 32];
        let mut nread: ssize_t = mi_prim_read(
            fd,
            &mut buf as *mut [libc::c_char; 32] as *mut libc::c_void,
            ::core::mem::size_of::<[libc::c_char; 32]>() as libc::c_ulong,
        );
        mi_prim_close(fd);
        if nread >= 1 as libc::c_int as ssize_t {
            os_overcommit = buf[0 as libc::c_int as usize] as libc::c_int == '0' as i32
                || buf[0 as libc::c_int as usize] as libc::c_int == '1' as i32;
        }
    }
    return os_overcommit;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_mem_init(mut config: *mut mi_os_mem_config_t) {
    let mut psize: libc::c_long = sysconf(_SC_PAGESIZE as libc::c_int);
    if psize > 0 as libc::c_int as libc::c_long {
        (*config).page_size = psize as size_t;
        (*config).alloc_granularity = psize as size_t;
        let mut pphys: libc::c_long = sysconf(_SC_PHYS_PAGES as libc::c_int);
        if pphys > 0 as libc::c_int as libc::c_long
            && (pphys as size_t)
                < (18446744073709551615 as libc::c_ulong).wrapping_div(psize as size_t)
        {
            (*config).physical_memory = pphys as size_t * psize as size_t;
        }
    }
    (*config).large_page_size = (2 as libc::c_int as libc::c_ulonglong)
        .wrapping_mul((1024 as libc::c_ulonglong).wrapping_mul(1024 as libc::c_ulonglong))
        as size_t;
    (*config).has_overcommit = unix_detect_overcommit();
    (*config).has_partial_free = 1 as libc::c_int != 0;
    (*config).has_virtual_reserve = 1 as libc::c_int != 0;
    if !mi_option_is_enabled(mi_option_allow_large_os_pages) {
        let mut val: libc::c_int = 0 as libc::c_int;
        if prctl(
            42 as libc::c_int,
            &mut val as *mut libc::c_int,
            0 as libc::c_int,
            0 as libc::c_int,
            0 as libc::c_int,
        ) != 0 as libc::c_int
        {
            val = 1 as libc::c_int;
            prctl(
                41 as libc::c_int,
                &mut val as *mut libc::c_int,
                0 as libc::c_int,
                0 as libc::c_int,
                0 as libc::c_int,
            );
        }
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_free(
    mut addr: *mut libc::c_void,
    mut size: size_t,
) -> libc::c_int {
    let mut err: bool = munmap(addr, size) == -(1 as libc::c_int);
    return if err as libc::c_int != 0 {
        *__errno_location()
    } else {
        0 as libc::c_int
    };
}
unsafe extern "C" fn unix_madvise(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut advice: libc::c_int,
) -> libc::c_int {
    let mut res: libc::c_int = madvise(addr, size, advice);
    return if res == 0 as libc::c_int {
        0 as libc::c_int
    } else {
        *__errno_location()
    };
}
unsafe extern "C" fn unix_mmap_prim(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut try_alignment: size_t,
    mut protect_flags: libc::c_int,
    mut flags: libc::c_int,
    mut fd: libc::c_int,
) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = 0 as *mut libc::c_void;
    if addr.is_null() {
        let mut hint: *mut libc::c_void = _mi_os_get_aligned_hint(try_alignment, size);
        if !hint.is_null() {
            p = mmap(
                hint,
                size,
                protect_flags,
                flags,
                fd,
                0 as libc::c_int as __off_t,
            );
            if p == -(1 as libc::c_int) as *mut libc::c_void || !_mi_is_aligned(p, try_alignment) {
                let mut err: libc::c_int = *__errno_location();
                _mi_trace_message(
                    b"unable to directly request hinted aligned OS memory (error: %d (0x%x), size: 0x%zx bytes, alignment: 0x%zx, hint address: %p)\n\0"
                        as *const u8 as *const libc::c_char,
                    err,
                    err,
                    size,
                    try_alignment,
                    hint,
                );
            }
            if p != -(1 as libc::c_int) as *mut libc::c_void {
                return p;
            }
        }
    }
    p = mmap(
        addr,
        size,
        protect_flags,
        flags,
        fd,
        0 as libc::c_int as __off_t,
    );
    if p != -(1 as libc::c_int) as *mut libc::c_void {
        return p;
    }
    return 0 as *mut libc::c_void;
}
unsafe extern "C" fn unix_mmap_fd() -> libc::c_int {
    return -(1 as libc::c_int);
}
unsafe extern "C" fn unix_mmap(
    mut addr: *mut libc::c_void,
    mut size: size_t,
    mut try_alignment: size_t,
    mut protect_flags: libc::c_int,
    mut large_only: bool,
    mut allow_large: bool,
    mut is_large: *mut bool,
) -> *mut libc::c_void {
    let mut p: *mut libc::c_void = 0 as *mut libc::c_void;
    let fd: libc::c_int = unix_mmap_fd();
    let mut flags: libc::c_int = 0x2 as libc::c_int | 0x20 as libc::c_int;
    if _mi_os_has_overcommit() {
        flags |= 0x4000 as libc::c_int;
    }
    if (large_only as libc::c_int != 0
        || _mi_os_use_large_page(size, try_alignment) as libc::c_int != 0)
        && allow_large as libc::c_int != 0
    {
        static mut large_page_try_ok: size_t = 0;
        let mut try_ok: size_t = ::core::intrinsics::atomic_load_acquire(&mut large_page_try_ok);
        if !large_only && try_ok > 0 as libc::c_int as size_t {
            let fresh52 = ::core::intrinsics::atomic_cxchg_acqrel_acquire(
                &mut large_page_try_ok,
                *&mut try_ok,
                try_ok.wrapping_sub(1 as libc::c_int as size_t),
            );
            *&mut try_ok = fresh52.0;
            fresh52.1;
        } else {
            let mut lflags: libc::c_int = flags & !(0x4000 as libc::c_int);
            let mut lfd: libc::c_int = fd;
            lflags |= 0x40000 as libc::c_int;
            static mut mi_huge_pages_available: bool = 1 as libc::c_int != 0;
            if (size as libc::c_ulonglong).wrapping_rem(
                (1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong),
            ) == 0 as libc::c_int as libc::c_ulonglong
                && mi_huge_pages_available as libc::c_int != 0
            {
                lflags = (lflags as libc::c_uint | (30 as libc::c_uint) << 26 as libc::c_int)
                    as libc::c_int;
            } else {
                lflags = (lflags as libc::c_uint | (21 as libc::c_uint) << 26 as libc::c_int)
                    as libc::c_int;
            }
            if large_only as libc::c_int != 0 || lflags != flags {
                *is_large = 1 as libc::c_int != 0;
                p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
                if p.is_null()
                    && lflags as libc::c_uint & (30 as libc::c_uint) << 26 as libc::c_int
                        == (30 as libc::c_uint) << 26 as libc::c_int
                {
                    mi_huge_pages_available = 0 as libc::c_int != 0;
                    _mi_warning_message(
                        b"unable to allocate huge (1GiB) page, trying large (2MiB) pages instead (errno: %i)\n\0"
                            as *const u8 as *const libc::c_char,
                        *__errno_location(),
                    );
                    lflags = (lflags as libc::c_uint & !((30 as libc::c_uint) << 26 as libc::c_int)
                        | (21 as libc::c_uint) << 26 as libc::c_int)
                        as libc::c_int;
                    p = unix_mmap_prim(addr, size, try_alignment, protect_flags, lflags, lfd);
                }
                if large_only {
                    return p;
                }
                if p.is_null() {
                    ::core::intrinsics::atomic_store_release(
                        &mut large_page_try_ok,
                        8 as libc::c_int as size_t,
                    );
                }
            }
        }
    }
    if p.is_null() {
        *is_large = 0 as libc::c_int != 0;
        p = unix_mmap_prim(addr, size, try_alignment, protect_flags, flags, fd);
        if !p.is_null() {
            if allow_large as libc::c_int != 0
                && _mi_os_use_large_page(size, try_alignment) as libc::c_int != 0
            {
                unix_madvise(p, size, 14 as libc::c_int) == 0 as libc::c_int;
            }
        }
    }
    return p;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_alloc(
    mut hint_addr: *mut libc::c_void,
    mut size: size_t,
    mut try_alignment: size_t,
    mut commit: bool,
    mut allow_large: bool,
    mut is_large: *mut bool,
    mut is_zero: *mut bool,
    mut addr: *mut *mut libc::c_void,
) -> libc::c_int {
    if size > 0 as libc::c_int as size_t && size % _mi_os_page_size() == 0 as libc::c_int as size_t
    {
    } else {
        _mi_assert_fail(
            b"size > 0 && (size % _mi_os_page_size()) == 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            11354 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_prim_alloc\0"))
                .as_ptr(),
        );
    };
    if commit as libc::c_int != 0 || !allow_large {
    } else {
        _mi_assert_fail(
            b"commit || !allow_large\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            11355 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_prim_alloc\0"))
                .as_ptr(),
        );
    };
    if try_alignment > 0 as libc::c_int as size_t {
    } else {
        _mi_assert_fail(
            b"try_alignment > 0\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            11356 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 15], &[libc::c_char; 15]>(b"_mi_prim_alloc\0"))
                .as_ptr(),
        );
    };
    *is_zero = 1 as libc::c_int != 0;
    let mut protect_flags: libc::c_int = if commit as libc::c_int != 0 {
        0x2 as libc::c_int | 0x1 as libc::c_int
    } else {
        0 as libc::c_int
    };
    *addr = unix_mmap(
        hint_addr,
        size,
        try_alignment,
        protect_flags,
        0 as libc::c_int != 0,
        allow_large,
        is_large,
    );
    return if !(*addr).is_null() {
        0 as libc::c_int
    } else {
        *__errno_location()
    };
}
unsafe extern "C" fn unix_mprotect_hint(mut err: libc::c_int) {}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_commit(
    mut start: *mut libc::c_void,
    mut size: size_t,
    mut is_zero: *mut bool,
) -> libc::c_int {
    *is_zero = 0 as libc::c_int != 0;
    let mut err: libc::c_int = mprotect(start, size, 0x1 as libc::c_int | 0x2 as libc::c_int);
    if err != 0 as libc::c_int {
        err = *__errno_location();
        unix_mprotect_hint(err);
    }
    return err;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_decommit(
    mut start: *mut libc::c_void,
    mut size: size_t,
    mut needs_recommit: *mut bool,
) -> libc::c_int {
    let mut err: libc::c_int = 0 as libc::c_int;
    err = unix_madvise(start, size, 4 as libc::c_int);
    *needs_recommit = 1 as libc::c_int != 0;
    mprotect(start, size, 0 as libc::c_int);
    return err;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_reset(
    mut start: *mut libc::c_void,
    mut size: size_t,
) -> libc::c_int {
    static mut advice: size_t = 8 as libc::c_int as libc::c_ulong;
    let mut oadvice: libc::c_int =
        ::core::intrinsics::atomic_load_relaxed(&mut advice) as libc::c_int;
    let mut err: libc::c_int = 0;
    loop {
        err = unix_madvise(start, size, oadvice);
        if !(err != 0 as libc::c_int && *__errno_location() == 11 as libc::c_int) {
            break;
        }
        *__errno_location() = 0 as libc::c_int;
    }
    if err != 0 as libc::c_int
        && *__errno_location() == 22 as libc::c_int
        && oadvice == 8 as libc::c_int
    {
        ::core::intrinsics::atomic_store_release(&mut advice, 4 as libc::c_int as size_t);
        err = unix_madvise(start, size, 4 as libc::c_int);
    }
    return err;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_protect(
    mut start: *mut libc::c_void,
    mut size: size_t,
    mut protect: bool,
) -> libc::c_int {
    let mut err: libc::c_int = mprotect(
        start,
        size,
        if protect as libc::c_int != 0 {
            0 as libc::c_int
        } else {
            0x1 as libc::c_int | 0x2 as libc::c_int
        },
    );
    if err != 0 as libc::c_int {
        err = *__errno_location();
    }
    unix_mprotect_hint(err);
    return err;
}
unsafe extern "C" fn mi_prim_mbind(
    mut start: *mut libc::c_void,
    mut len: libc::c_ulong,
    mut mode: libc::c_ulong,
    mut nmask: *const libc::c_ulong,
    mut maxnode: libc::c_ulong,
    mut flags: libc::c_uint,
) -> libc::c_long {
    return syscall(
        237 as libc::c_int as libc::c_long,
        start,
        len,
        mode,
        nmask,
        maxnode,
        flags,
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_alloc_huge_os_pages(
    mut hint_addr: *mut libc::c_void,
    mut size: size_t,
    mut numa_node: libc::c_int,
    mut is_zero: *mut bool,
    mut addr: *mut *mut libc::c_void,
) -> libc::c_int {
    let mut is_large: bool = 1 as libc::c_int != 0;
    *is_zero = 1 as libc::c_int != 0;
    *addr = unix_mmap(
        hint_addr,
        size,
        ((1 as libc::c_ulonglong)
            << 3 as libc::c_int + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
            as size_t,
        0x1 as libc::c_int | 0x2 as libc::c_int,
        1 as libc::c_int != 0,
        1 as libc::c_int != 0,
        &mut is_large,
    );
    if !(*addr).is_null()
        && numa_node >= 0 as libc::c_int
        && numa_node < 8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)
    {
        let mut numa_mask: libc::c_ulong = (1 as libc::c_ulong) << numa_node;
        let mut err: libc::c_long = mi_prim_mbind(
            *addr,
            size,
            1 as libc::c_int as libc::c_ulong,
            &mut numa_mask,
            (8 as libc::c_int * ((1 as libc::c_int) << 3 as libc::c_int)) as libc::c_ulong,
            0 as libc::c_int as libc::c_uint,
        );
        if err != 0 as libc::c_int as libc::c_long {
            err = *__errno_location() as libc::c_long;
            _mi_warning_message(
                b"failed to bind huge (1GiB) pages to numa node %d (error: %d (0x%x))\n\0"
                    as *const u8 as *const libc::c_char,
                numa_node,
                err,
                err,
            );
        }
    }
    return if !(*addr).is_null() {
        0 as libc::c_int
    } else {
        *__errno_location()
    };
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_numa_node() -> size_t {
    let mut node: libc::c_ulong = 0 as libc::c_int as libc::c_ulong;
    let mut ncpu: libc::c_ulong = 0 as libc::c_int as libc::c_ulong;
    let mut err: libc::c_long = syscall(
        309 as libc::c_int as libc::c_long,
        &mut ncpu as *mut libc::c_ulong,
        &mut node as *mut libc::c_ulong,
        0 as *mut libc::c_void,
    );
    if err != 0 as libc::c_int as libc::c_long {
        return 0 as libc::c_int as size_t;
    }
    return node;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_numa_node_count() -> size_t {
    let mut buf: [libc::c_char; 128] = [0; 128];
    let mut node: libc::c_uint = 0 as libc::c_int as libc::c_uint;
    node = 0 as libc::c_int as libc::c_uint;
    while node < 256 as libc::c_int as libc::c_uint {
        _mi_snprintf(
            buf.as_mut_ptr(),
            127 as libc::c_int as size_t,
            b"/sys/devices/system/node/node%u\0" as *const u8 as *const libc::c_char,
            node.wrapping_add(1 as libc::c_int as libc::c_uint),
        );
        if mi_prim_access(buf.as_mut_ptr(), 4 as libc::c_int) != 0 as libc::c_int {
            break;
        }
        node = node.wrapping_add(1);
        node;
    }
    return node.wrapping_add(1 as libc::c_int as libc::c_uint) as size_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_clock_now() -> mi_msecs_t {
    let mut t: timespec = timespec {
        tv_sec: 0,
        tv_nsec: 0,
    };
    clock_gettime(1 as libc::c_int, &mut t);
    return t.tv_sec * 1000 as libc::c_int as mi_msecs_t
        + t.tv_nsec / 1000000 as libc::c_int as mi_msecs_t;
}
unsafe extern "C" fn timeval_secs(mut tv: *const timeval) -> mi_msecs_t {
    return (*tv).tv_sec * 1000 as libc::c_long + (*tv).tv_usec / 1000 as libc::c_long;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_process_info(mut pinfo: *mut mi_process_info_t) {
    let mut rusage: rusage = rusage {
        ru_utime: timeval {
            tv_sec: 0,
            tv_usec: 0,
        },
        ru_stime: timeval {
            tv_sec: 0,
            tv_usec: 0,
        },
        c2rust_unnamed: C2RustUnnamed_1 { ru_maxrss: 0 },
        c2rust_unnamed_0: C2RustUnnamed_14 { ru_ixrss: 0 },
        c2rust_unnamed_1: C2RustUnnamed_13 { ru_idrss: 0 },
        c2rust_unnamed_2: C2RustUnnamed_12 { ru_isrss: 0 },
        c2rust_unnamed_3: C2RustUnnamed_11 { ru_minflt: 0 },
        c2rust_unnamed_4: C2RustUnnamed_10 { ru_majflt: 0 },
        c2rust_unnamed_5: C2RustUnnamed_9 { ru_nswap: 0 },
        c2rust_unnamed_6: C2RustUnnamed_8 { ru_inblock: 0 },
        c2rust_unnamed_7: C2RustUnnamed_7 { ru_oublock: 0 },
        c2rust_unnamed_8: C2RustUnnamed_6 { ru_msgsnd: 0 },
        c2rust_unnamed_9: C2RustUnnamed_5 { ru_msgrcv: 0 },
        c2rust_unnamed_10: C2RustUnnamed_4 { ru_nsignals: 0 },
        c2rust_unnamed_11: C2RustUnnamed_3 { ru_nvcsw: 0 },
        c2rust_unnamed_12: C2RustUnnamed_2 { ru_nivcsw: 0 },
    };
    getrusage(RUSAGE_SELF as libc::c_int, &mut rusage);
    (*pinfo).utime = timeval_secs(&mut rusage.ru_utime);
    (*pinfo).stime = timeval_secs(&mut rusage.ru_stime);
    (*pinfo).page_faults = rusage.c2rust_unnamed_4.ru_majflt as size_t;
    (*pinfo).peak_rss =
        (rusage.c2rust_unnamed.ru_maxrss * 1024 as libc::c_int as libc::c_long) as size_t;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_out_stderr(mut msg: *const libc::c_char) {
    fputs(msg, stderr);
}
unsafe extern "C" fn mi_get_environ() -> *mut *mut libc::c_char {
    return environ;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_getenv(
    mut name: *const libc::c_char,
    mut result: *mut libc::c_char,
    mut result_size: size_t,
) -> bool {
    if name.is_null() {
        return 0 as libc::c_int != 0;
    }
    let len: size_t = _mi_strlen(name);
    if len == 0 as libc::c_int as size_t {
        return 0 as libc::c_int != 0;
    }
    let mut env: *mut *mut libc::c_char = mi_get_environ();
    if env.is_null() {
        return 0 as libc::c_int != 0;
    }
    let mut i: libc::c_int = 0 as libc::c_int;
    while i < 10000 as libc::c_int && !(*env.offset(i as isize)).is_null() {
        let mut s: *const libc::c_char = *env.offset(i as isize);
        if _mi_strnicmp(name, s, len) == 0 as libc::c_int
            && *s.offset(len as isize) as libc::c_int == '=' as i32
        {
            _mi_strlcpy(
                result,
                s.offset(len as isize).offset(1 as libc::c_int as isize),
                result_size,
            );
            return 1 as libc::c_int != 0;
        }
        i += 1;
        i;
    }
    return 0 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_random_buf(
    mut buf: *mut libc::c_void,
    mut buf_len: size_t,
) -> bool {
    static mut no_getrandom: uintptr_t = 0;
    if ::core::intrinsics::atomic_load_acquire(&mut no_getrandom as *mut uintptr_t)
        == 0 as libc::c_int as uintptr_t
    {
        let mut ret: ssize_t = syscall(
            318 as libc::c_int as libc::c_long,
            buf,
            buf_len,
            1 as libc::c_int,
        );
        if ret >= 0 as libc::c_int as ssize_t {
            return buf_len == ret as size_t;
        }
        if *__errno_location() != 38 as libc::c_int {
            return 0 as libc::c_int != 0;
        }
        ::core::intrinsics::atomic_store_release(&mut no_getrandom, 1 as libc::c_int as uintptr_t);
    }
    let mut flags: libc::c_int = 0 as libc::c_int;
    flags |= 0o2000000 as libc::c_int;
    let mut fd: libc::c_int =
        mi_prim_open(b"/dev/urandom\0" as *const u8 as *const libc::c_char, flags);
    if fd < 0 as libc::c_int {
        return 0 as libc::c_int != 0;
    }
    let mut count: size_t = 0 as libc::c_int as size_t;
    while count < buf_len {
        let mut ret_0: ssize_t = mi_prim_read(
            fd,
            (buf as *mut libc::c_char).offset(count as isize) as *mut libc::c_void,
            buf_len.wrapping_sub(count),
        );
        if ret_0 <= 0 as libc::c_int as ssize_t {
            if *__errno_location() != 11 as libc::c_int && *__errno_location() != 4 as libc::c_int {
                break;
            }
        } else {
            count = count.wrapping_add(ret_0 as size_t);
        }
    }
    mi_prim_close(fd);
    return count == buf_len;
}
#[unsafe(no_mangle)]
pub static mut _mi_heap_default_key: pthread_key_t = -(1 as libc::c_int) as pthread_key_t;
unsafe extern "C" fn mi_pthread_done(mut value: *mut libc::c_void) {
    if !value.is_null() {
        _mi_thread_done(value as *mut mi_heap_t);
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_thread_init_auto_done() {
    if _mi_heap_default_key == -(1 as libc::c_int) as pthread_key_t {
    } else {
        _mi_assert_fail(
            b"_mi_heap_default_key == (pthread_key_t)(-1)\0" as *const u8 as *const libc::c_char,
            b"preprocess/mimalloc.c\0" as *const u8 as *const libc::c_char,
            11701 as libc::c_int as libc::c_uint,
            (*::core::mem::transmute::<&[u8; 31], &[libc::c_char; 31]>(
                b"_mi_prim_thread_init_auto_done\0",
            ))
            .as_ptr(),
        );
    };
    pthread_key_create(
        &mut _mi_heap_default_key,
        Some(mi_pthread_done as unsafe extern "C" fn(*mut libc::c_void) -> ()),
    );
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_thread_done_auto_done() {
    if _mi_heap_default_key != -(1 as libc::c_int) as pthread_key_t {
        pthread_key_delete(_mi_heap_default_key);
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_thread_associate_default_heap(mut heap: *mut mi_heap_t) {
    if _mi_heap_default_key != -(1 as libc::c_int) as pthread_key_t {
        pthread_setspecific(_mi_heap_default_key, heap as *const libc::c_void);
    }
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_is_redirected() -> bool {
    return 0 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_allocator_init(mut message: *mut *const libc::c_char) -> bool {
    if !message.is_null() {
        *message = 0 as *const libc::c_char;
    }
    return 1 as libc::c_int != 0;
}
#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_allocator_done() {}
unsafe extern "C" fn run_static_initializers() {
    _mi_page_empty = {
        let mut init = mi_page_s {
            segment_in_use_is_committed_is_zero_init_is_huge: [0; 1],
            free_is_zero_retire_expire: [0; 1],
            segment_idx: 0 as libc::c_int as uint8_t,
            capacity: 0 as libc::c_int as uint16_t,
            reserved: 0 as libc::c_int as uint16_t,
            flags: mi_page_flags_s {
                full_aligned: 0 as libc::c_int as uint8_t,
            },
            free: 0 as *mut mi_block_t,
            local_free: 0 as *mut mi_block_t,
            used: 0 as libc::c_int as uint16_t,
            block_size_shift: 0 as libc::c_int as uint8_t,
            heap_tag: 0 as libc::c_int as uint8_t,
            block_size: 0 as libc::c_int as size_t,
            page_start: 0 as *mut uint8_t,
            keys: [0 as libc::c_int as uintptr_t, 0 as libc::c_int as uintptr_t],
            xthread_free: 0 as libc::c_int as libc::c_ulong,
            xheap: 0 as libc::c_int as libc::c_ulong,
            next: 0 as *mut mi_page_s,
            prev: 0 as *mut mi_page_s,
        };
        init.set_segment_in_use(0 as libc::c_int as uint8_t);
        init.set_is_committed(0 as libc::c_int as uint8_t);
        init.set_is_zero_init(0 as libc::c_int as uint8_t);
        init.set_is_huge(0 as libc::c_int as uint8_t);
        init.set_free_is_zero(0 as libc::c_int as uint8_t);
        init.set_retire_expire(0 as libc::c_int as uint8_t);
        init
    };
    _mi_heap_empty = {
        let mut init = mi_heap_s {
            tld: 0 as *mut mi_tld_t,
            thread_delayed_free: 0 as *mut mi_block_t,
            thread_id: 0 as libc::c_int as mi_threadid_t,
            arena_id: 0 as libc::c_int,
            cookie: 0 as libc::c_int as uintptr_t,
            keys: [0 as libc::c_int as uintptr_t, 0 as libc::c_int as uintptr_t],
            random: {
                let mut init = mi_random_cxt_s {
                    input: [
                        0 as libc::c_int as uint32_t,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                    ],
                    output: [
                        0 as libc::c_int as uint32_t,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                    ],
                    output_available: 0 as libc::c_int,
                    weak: 1 as libc::c_int != 0,
                };
                init
            },
            page_count: 0 as libc::c_int as size_t,
            page_retired_min: (73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint)
                as size_t,
            page_retired_max: 0 as libc::c_int as size_t,
            next: 0 as *mut mi_heap_t,
            no_reclaim: 0 as libc::c_int != 0,
            tag: 0 as libc::c_int as uint8_t,
            pages_free_direct: [
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
            ],
            pages: [
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (2 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (3 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (4 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (5 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (6 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (7 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (8 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (10 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (12 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (14 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (16 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (20 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (24 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (28 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (32 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (40 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (48 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (56 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (64 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (80 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (96 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (112 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (128 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (160 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (192 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (224 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (256 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (320 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (384 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (448 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (512 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (640 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (768 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (896 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1024 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1280 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1536 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1792 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (2048 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (2560 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (3072 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (3584 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (4096 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (5120 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (6144 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (7168 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (8192 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (10240 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (12288 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (14336 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (16384 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (20480 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (24576 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (28672 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (32768 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (40960 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (49152 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (57344 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (65536 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (81920 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (98304 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (114688 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (131072 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (163840 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (196608 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (229376 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (262144 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (327680 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (393216 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (458752 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (524288 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: ((1 as libc::c_ulonglong)
                            << 3 as libc::c_int
                                + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
                            .wrapping_div(
                                ((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulonglong,
                            )
                            .wrapping_add(1 as libc::c_int as libc::c_ulonglong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong
                                as libc::c_ulonglong) as size_t,
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: ((1 as libc::c_ulonglong)
                            << 3 as libc::c_int
                                + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
                            .wrapping_div(
                                ((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulonglong,
                            )
                            .wrapping_add(2 as libc::c_int as libc::c_ulonglong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong
                                as libc::c_ulonglong) as size_t,
                    };
                    init
                },
            ],
        };
        init
    };
    tld_main = {
        let mut init = mi_tld_s {
            heartbeat: 0 as libc::c_int as libc::c_ulonglong,
            recurse: 0 as libc::c_int != 0,
            heap_backing: &mut _mi_heap_main,
            heaps: &mut _mi_heap_main,
            segments: {
                let mut init = mi_segments_tld_s {
                    small_free: {
                        let mut init = mi_segment_queue_s {
                            first: 0 as *mut mi_segment_t,
                            last: 0 as *mut mi_segment_t,
                        };
                        init
                    },
                    medium_free: {
                        let mut init = mi_segment_queue_s {
                            first: 0 as *mut mi_segment_t,
                            last: 0 as *mut mi_segment_t,
                        };
                        init
                    },
                    pages_purge: {
                        let mut init = mi_page_queue_s {
                            first: 0 as *mut mi_page_t,
                            last: 0 as *mut mi_page_t,
                            block_size: 0 as libc::c_int as size_t,
                        };
                        init
                    },
                    count: 0 as libc::c_int as size_t,
                    peak_count: 0 as libc::c_int as size_t,
                    current_size: 0 as libc::c_int as size_t,
                    peak_size: 0 as libc::c_int as size_t,
                    reclaim_count: 0 as libc::c_int as size_t,
                    subproc: &mut mi_subproc_default,
                    stats: &mut tld_main.stats,
                    os: &mut tld_main.os,
                };
                init
            },
            os: {
                let mut init = mi_os_tld_s {
                    region_idx: 0 as libc::c_int as size_t,
                    stats: &mut tld_main.stats,
                };
                init
            },
            stats: {
                let mut init = mi_stats_s {
                    segments: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    pages: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    reserved: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    committed: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    reset: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    purged: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    page_committed: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    segments_abandoned: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    pages_abandoned: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    threads: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    normal: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    huge: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    giant: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    malloc: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    segments_cache: {
                        let mut init = mi_stat_count_s {
                            allocated: 0 as libc::c_int as int64_t,
                            freed: 0 as libc::c_int as int64_t,
                            peak: 0 as libc::c_int as int64_t,
                            current: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    pages_extended: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    mmap_calls: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    commit_calls: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    reset_calls: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    purge_calls: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    page_no_retire: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    searches: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    normal_count: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    huge_count: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    arena_count: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    arena_crossover_count: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    arena_rollback_count: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    guarded_alloc_count: {
                        let mut init = mi_stat_counter_s {
                            total: 0 as libc::c_int as int64_t,
                            count: 0 as libc::c_int as int64_t,
                        };
                        init
                    },
                    normal_bins: [
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        {
                            let mut init = mi_stat_count_s {
                                allocated: 0 as libc::c_int as int64_t,
                                freed: 0 as libc::c_int as int64_t,
                                peak: 0 as libc::c_int as int64_t,
                                current: 0 as libc::c_int as int64_t,
                            };
                            init
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                        mi_stat_count_s {
                            allocated: 0,
                            freed: 0,
                            peak: 0,
                            current: 0,
                        },
                    ],
                };
                init
            },
        };
        init
    };
    _mi_heap_main = {
        let mut init = mi_heap_s {
            tld: &mut tld_main,
            thread_delayed_free: 0 as *mut mi_block_t,
            thread_id: 0 as libc::c_int as mi_threadid_t,
            arena_id: 0 as libc::c_int,
            cookie: 0 as libc::c_int as uintptr_t,
            keys: [0 as libc::c_int as uintptr_t, 0 as libc::c_int as uintptr_t],
            random: {
                let mut init = mi_random_cxt_s {
                    input: [
                        0x846ca68b as libc::c_uint,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                    ],
                    output: [
                        0 as libc::c_int as uint32_t,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                        0,
                    ],
                    output_available: 0 as libc::c_int,
                    weak: 1 as libc::c_int != 0,
                };
                init
            },
            page_count: 0 as libc::c_int as size_t,
            page_retired_min: (73 as libc::c_uint).wrapping_add(1 as libc::c_int as libc::c_uint)
                as size_t,
            page_retired_max: 0 as libc::c_int as size_t,
            next: 0 as *mut mi_heap_t,
            no_reclaim: 0 as libc::c_int != 0,
            tag: 0 as libc::c_int as uint8_t,
            pages_free_direct: [
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
                &raw mut _mi_page_empty,
            ],
            pages: [
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (2 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (3 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (4 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (5 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (6 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (7 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (8 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (10 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (12 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (14 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (16 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (20 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (24 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (28 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (32 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (40 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (48 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (56 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (64 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (80 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (96 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (112 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (128 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (160 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (192 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (224 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (256 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (320 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (384 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (448 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (512 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (640 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (768 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (896 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1024 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1280 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1536 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (1792 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (2048 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (2560 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (3072 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (3584 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (4096 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (5120 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (6144 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (7168 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (8192 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (10240 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (12288 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (14336 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (16384 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (20480 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (24576 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (28672 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (32768 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (40960 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (49152 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (57344 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (65536 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (81920 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (98304 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (114688 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (131072 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (163840 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (196608 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (229376 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (262144 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (327680 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (393216 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (458752 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: (524288 as libc::c_int as libc::c_ulong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong),
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: ((1 as libc::c_ulonglong)
                            << 3 as libc::c_int
                                + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
                            .wrapping_div(
                                ((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulonglong,
                            )
                            .wrapping_add(1 as libc::c_int as libc::c_ulonglong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong
                                as libc::c_ulonglong) as size_t,
                    };
                    init
                },
                {
                    let mut init = mi_page_queue_s {
                        first: 0 as *mut mi_page_t,
                        last: 0 as *mut mi_page_t,
                        block_size: ((1 as libc::c_ulonglong)
                            << 3 as libc::c_int
                                + (3 as libc::c_int + (13 as libc::c_int + 3 as libc::c_int)))
                            .wrapping_div(2 as libc::c_int as libc::c_ulonglong)
                            .wrapping_div(
                                ((1 as libc::c_int) << 3 as libc::c_int) as libc::c_ulonglong,
                            )
                            .wrapping_add(2 as libc::c_int as libc::c_ulonglong)
                            .wrapping_mul(::core::mem::size_of::<uintptr_t>() as libc::c_ulong
                                as libc::c_ulonglong) as size_t,
                    };
                    init
                },
            ],
        };
        init
    };
    options = [
        {
            let mut init = mi_option_desc_s {
                value: 1 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_show_errors,
                name: b"show_errors\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_show_stats,
                name: b"show_stats\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_verbose,
                name: b"verbose\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 1 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_eager_commit,
                name: b"eager_commit\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 2 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_arena_eager_commit,
                name: b"arena_eager_commit\0" as *const u8 as *const libc::c_char,
                legacy_name: b"eager_region_commit\0" as *const u8 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 1 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_purge_decommits,
                name: b"purge_decommits\0" as *const u8 as *const libc::c_char,
                legacy_name: b"reset_decommits\0" as *const u8 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_allow_large_os_pages,
                name: b"allow_large_os_pages\0" as *const u8 as *const libc::c_char,
                legacy_name: b"large_os_pages\0" as *const u8 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_reserve_huge_os_pages,
                name: b"reserve_huge_os_pages\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: -(1 as libc::c_int) as libc::c_long,
                init: UNINIT,
                option: mi_option_reserve_huge_os_pages_at,
                name: b"reserve_huge_os_pages_at\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_reserve_os_memory,
                name: b"reserve_os_memory\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_deprecated_segment_cache,
                name: b"deprecated_segment_cache\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_deprecated_page_reset,
                name: b"deprecated_page_reset\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_abandoned_page_purge,
                name: b"abandoned_page_purge\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_deprecated_segment_reset,
                name: b"deprecated_segment_reset\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 1 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_eager_commit_delay,
                name: b"eager_commit_delay\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 10 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_purge_delay,
                name: b"purge_delay\0" as *const u8 as *const libc::c_char,
                legacy_name: b"reset_delay\0" as *const u8 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_use_numa_nodes,
                name: b"use_numa_nodes\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_disallow_os_alloc,
                name: b"disallow_os_alloc\0" as *const u8 as *const libc::c_char,
                legacy_name: b"limit_os_alloc\0" as *const u8 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 100 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_os_tag,
                name: b"os_tag\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 32 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_max_errors,
                name: b"max_errors\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 32 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_max_warnings,
                name: b"max_warnings\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 10 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_max_segment_reclaim,
                name: b"max_segment_reclaim\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_destroy_on_exit,
                name: b"destroy_on_exit\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 1024 as libc::c_long * 1024 as libc::c_long,
                init: UNINIT,
                option: mi_option_arena_reserve,
                name: b"arena_reserve\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 10 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_arena_purge_mult,
                name: b"arena_purge_mult\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 1 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_purge_extend_delay,
                name: b"purge_extend_delay\0" as *const u8 as *const libc::c_char,
                legacy_name: b"decommit_extend_delay\0" as *const u8 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 1 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_abandoned_reclaim_on_free,
                name: b"abandoned_reclaim_on_free\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_disallow_arena_alloc,
                name: b"disallow_arena_alloc\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 400 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_retry_on_oom,
                name: b"retry_on_oom\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_visit_abandoned,
                name: b"visit_abandoned\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_guarded_min,
                name: b"guarded_min\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: (1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong) as libc::c_long,
                init: UNINIT,
                option: mi_option_guarded_max,
                name: b"guarded_max\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_guarded_precise,
                name: b"guarded_precise\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_guarded_sample_rate,
                name: b"guarded_sample_rate\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_guarded_sample_seed,
                name: b"guarded_sample_seed\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
        {
            let mut init = mi_option_desc_s {
                value: 0 as libc::c_int as libc::c_long,
                init: UNINIT,
                option: mi_option_target_segments_per_thread,
                name: b"target_segments_per_thread\0" as *const u8 as *const libc::c_char,
                legacy_name: 0 as *const libc::c_char,
            };
            init
        },
    ];
    mi_os_mem_config = {
        let mut init = mi_os_mem_config_s {
            page_size: 4096 as libc::c_int as size_t,
            large_page_size: 0 as libc::c_int as size_t,
            alloc_granularity: 4096 as libc::c_int as size_t,
            physical_memory: (32 as libc::c_int as libc::c_ulonglong).wrapping_mul(
                (1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong)
                    .wrapping_mul(1024 as libc::c_ulonglong),
            ) as size_t,
            virtual_address_bits: 48 as libc::c_int as size_t,
            has_overcommit: 1 as libc::c_int != 0,
            has_partial_free: 0 as libc::c_int != 0,
            has_virtual_reserve: 1 as libc::c_int != 0,
        };
        init
    };
}
#[used]
#[cfg_attr(target_os = "linux", unsafe(unsafe(link_section = ".init_array")))]
#[cfg_attr(target_os = "windows", unsafe(unsafe(link_section = ".CRT$XIB")))]
#[cfg_attr(
    target_os = "macos",
    unsafe(unsafe(link_section = "__DATA,__mod_init_func"))
)]
static INIT_ARRAY: [unsafe extern "C" fn(); 1] = [run_static_initializers];
