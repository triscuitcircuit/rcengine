
RC-Engine
=============================
A game engine made using both Rust and C++.
The Rust language is used for the networking side of the engine, while C++ is used for standard rendering

## Compiling
Compilations are tested and ran on a Macintosh, so Windows compilation results may vary.
Make sure Rust Language is installed, as the RcNetwork library won't compile.

To make sure the Rust library compiles correctly, please make sure to 
put the source sdk folder for  [steamworks sdk](https://partner.steamgames.com/doc/sdk) within `RcEngine/external/RcNetwork/external`
, or you can compile without it by deleting the steam folder within `RcNetwork`and removing the reference in `Cargo.toml`

Work in progress

Clone the project with git clone recursive
```shell
git clone --recurse-submodules -j8 https:///github.com/triscuitcircuit/rcengine.git
cd rcengine
```
<details> <summary>Standard Cmake build</summary>
1. make a "build" directory within the main directory

```shell
 mkdir build
 cd build
```
2. run cmake within the build directory
```shell
cmake ..
```
</details>

<details>
<summary>Xcode build from script
</summary>

1. Go to the scripts directory and run the "generate-xcode.sh" script

OR
2. Run the script through the MacOS terminal
```shell
bash generate-xcode.sh
```
Then open up the Xcode project that was generated in the "xcode-build" folder
</details>

<details>
<summary>Visual Studio build from .bat
</summary>
On Windows, you can run the "generate-VSproj.bat" file located in the "scripts" folder. 

This will generate a Visual Studio project within a Visual Studio 2019 project within a folder called "visual-studio-build". 

</details>

While very early in development, feel free to comment changes you would
want in a simple game engine. This engine isn't meant to be a AAA engine by any means, and the scope
is to make a 2.5D editor and engine with network capabilities. 
# Special Thanks
- Special thanks to TheCherno for his guide on game engine design!
- A special shout-out to 0x (https://github.com/0xworks) for helping with my OpenGL questions (And help with `OpenGLTexture.cpp`)!! 
- A special thanks to [Light3039](https://github.com/Light3039) on github for texture debug help( and for the `Tiles.png` debug texture)!!!! 
