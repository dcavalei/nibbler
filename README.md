# nibbler

TODO

## Install Dependencies (Ubuntu 22.04)

### OpenGL -> freeglut3
```shell
sudo apt update
sudo apt install freeglut3 freeglut3-dev
```

### Place Holder

## How to build
```shell
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=<Debug|Release>
cmake --build . --target all

## How to Play
# ./nibbler/src/nibbler ./nibbler/lib/opengl-plugin/libopengl-plugin.so ./nibbler/lib/opengl-plugin/libopengl-plugin.so ./nibbler/lib/opengl-plugin/libopengl-plugin.so
# Look at the terminal to understand the game state, there's no graphics yet.
# Press Enter twice. Use ASDW keys to play.
```
