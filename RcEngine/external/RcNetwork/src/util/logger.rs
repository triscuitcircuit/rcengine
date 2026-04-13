extern crate chrono;
extern crate log;


use std::fs;
use std::io::{Write, Error};
use log::{Record, LevelFilter, Metadata};
use std::fs::File;
use self::log::Log;
use simplelog::Config;


pub struct FileLogger{
    log_file: String,
    filter_level: LevelFilter,
}

impl FileLogger{
    pub fn new(log_file: &str, filter_level: LevelFilter)-> Self{
        Self{
            log_file: log_file.to_owned(),
            filter_level,
        }
    }
}

impl log::Log for FileLogger{
    fn enabled(&self, metadata:&Metadata)->bool{
        metadata.level() <= self.filter_level
    }

    fn log(&self, record: &Record) {
        if self.enabled(record.metadata()){
            let mut retries = 0;
            let mut file = loop{
                let file = fs::OpenOptions::new()
                    .create(true)
                    .append(true)
                    .open(&self.log_file);
                let file = match file {
                    Ok(f) => f,
                    Err(e)=>{
                        if retries >= 10{
                            eprintln!("Could not write to log file: {}",retries);
                            return;
                        }
                        retries+=1;
                        std::thread::sleep(std::time::Duration::from_micros(5));
                        continue;
                    },
                };
                break file;
            };
            let dt = chrono::Local::now();
            let time = dt.format("%Y-%m-%d %H:%M:%S");

            if let Err(e) = writeln!(file, "[{}]  [{}] {}", time, record.level(),record.args()){
                eprintln!("Couldn't write to log file: {}",e);
            }

        }
    }

    fn flush(&self) {}
}
impl simplelog::SharedLogger for FileLogger{
    fn level(&self) -> LevelFilter {
        todo!()
    }

    fn config(&self) -> Option<&Config> {
        todo!()
    }

    fn as_log(self: Box<Self>) -> Box<dyn Log> {
        todo!()
    }
}