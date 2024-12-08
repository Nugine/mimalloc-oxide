#[cfg(not(all(
    target_arch = "x86_64",
    target_os = "linux",
    target_env = "gnu",
    target_pointer_width = "64"
)))]
compile_error!("mimalloc_oxide only supports x86_64-unknown-linux-gnu now");

use c2rust_bitfields::BitfieldStruct;

unsafe extern "C" {
    #[link_name = "llvm.thread.pointer"]
    pub fn thread_pointer() -> *mut i8;
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_thread_id() -> mi_threadid_t {
    unsafe { thread_pointer() as _ }
}
