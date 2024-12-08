unsafe extern "C" {
    #[link_name = "llvm.thread.pointer"]
    pub fn thread_pointer() -> *mut i8;
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn _mi_prim_thread_id() -> mi_threadid_t {
    unsafe {
        return thread_pointer() as _;
    }
}
