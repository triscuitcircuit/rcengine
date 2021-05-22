# RC-Engine
A game engine made using both Rust and C++.
The Rust language is used for the networking side of the engine, while C++ is used for standard rendering

## Compiling
Compilations are tested and ran on a Macintosh, so Windows compilation results may vary.
Make sure Rust Language is installed, as the RcNetwork library won't compile without it.
1. make a "build" directory within the main directory
```shell
 mkdir build
 cd build
```
2. run cmake within the build directory
```shell
cmake ..
```
#Special Thanks
Special thanks to TheCherno for his guide on game engine design!