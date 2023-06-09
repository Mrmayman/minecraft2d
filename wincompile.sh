#!/bin/bash
x86_64-w64-mingw32-g++ -o\
./bin/game.exe ./src/main.cpp ./src/entity.cpp ./src/nutils.cpp ./src/blockproperties.cpp ./src/tick.cpp ./src/world.cpp \
-I./SDL2_root/x86_64-w64-mingw32/include/ \
-I./SDL2_root/x86_64-w64-mingw32/include/SDL2/ \
-I./SDL2_image/x86_64-w64-mingw32/include/ \
-I./SDL2_ttf/x86_64-w64-mingw32/include/ \
-L./SDL2_root/x86_64-w64-mingw32/lib/ \
-L./SDL2_image/x86_64-w64-mingw32/lib/ \
-L./SDL2_ttf/x86_64-w64-mingw32/lib/ \
-lSDL2 -lSDL2_image -lSDL2_ttf -static-libstdc++
