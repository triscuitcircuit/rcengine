mod util;

extern crate libc;
use libc::{c_int, size_t};
use crate::util::client::tcp_client;
use crate::util::server::tcp_server;

//Entry point
#[no_mangle]
pub extern "C" fn rc_entry(){
    println!("Entrypoint rc_network");
}
#[no_mangle]
pub extern "C" fn rc_tcp_client(){
    tcp_client();
}
#[no_mangle]
pub extern "C" fn rc_tcp_server(){
    tcp_server();
}