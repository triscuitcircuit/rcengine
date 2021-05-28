extern crate libc;
use libc::{c_int, size_t};

#[no_mangle]
pub extern "C" fn hello(){
    println!("Hello from Rust");
}