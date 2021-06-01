# RC-Engine
A game engine made using both Rust and C++.
The Rust language is used for the networking side of the engine, while C++ is used for standard rendering

## Compiling
Compilations are tested and ran on a Macintosh, so Windows compilation results may vary.
Make sure Rust Language is installed, as the RcNetwork library won't compile.

To make sure the Rust library compiles correctly, please make sure to 
put the source sdk folder for  [steamworks sdk](https://partner.steamgames.com/doc/sdk) within `RcEngine/external/RcNetwork/external`
, or you can compile without it by deleting the steam folder within `RcNetwork`and removing the reference in `Cargo.toml`

Work in progress

1.Clone the project with git clone recursive
```shell
git clone --recurse-submodules -j8 https:///github.com/triscuitcircuit/rcengine.git
cd rcengine
```
2. make a "build" directory within the main directory
```shell
 mkdir build
 cd build
```
3. run cmake within the build directory
```shell
cmake ..
```
# Contributions
While very early in development, feel free to comment changes you would
want in a simple game engine. This engine isn't meant to be a AAA engine by any means, and the scope
is to make a 2.5D editor and engine with network capabilities. 
# Special Thanks
Special thanks to TheCherno for his guide on game engine design!
