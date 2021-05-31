use std::net::{TcpListener,TcpStream};
use std::thread;

use std::io::{Read,Write,Error};
use std::collections::HashMap;

fn handle_client(mut stream: TcpStream) -> Result<(), Error>{
    println!("Incoming connection from: {}", stream.peer_addr()?);
    let mut buf = [0;512];
    loop{
        let bytes_read = stream.read(&mut buf)?;
        if bytes_read ==0 {return Ok(());}
        stream.write(&buf[..bytes_read])?;
    }
}
pub fn tcp_server(){

    let listener = TcpListener::bind("0.0.0.0:8888")
        .expect("Could not bind socket");
    // new thread here to prevent lockup
    thread::spawn(move||{
        println!("server activated");
        for stream in listener.incoming(){
            match stream{
                Err(e) => {
                    eprintln!("failed: {}", e)
                }
                Ok(stream)=>{

                    thread::spawn(move||{
                        handle_client(stream)
                            .unwrap_or_else(|error|eprintln!("{:?}",error))
                    });
                }
            };
        }
    });
}