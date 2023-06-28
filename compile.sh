#!/bin/sh

# Libraries to locate
libraries="
libsdl2-dev
libsdl2-image-dev
libsdl2-ttf-dev
libsdl2-2.0.0
libsdl2-image-2.0.0
libsdl2-ttf-2.0.0
"

# Check if any library is missing
missing_lib=false

for library in $libraries; do
  if ! locate "$library" >/dev/null; then
    echo "Library $library not found. Please install the required libraries."
    missing_lib=true
  fi
done

# Exit if any library is missing
if [ "$missing_lib" = true ]; then
  exit 1
fi

g++ ./src/main.cpp ./src/entity.cpp ./src/nutils.cpp ./src/blockproperties.cpp ./src/tick.cpp ./src/world.cpp -o ./bin/a.out -lSDL2 -lSDL2_image -lSDL2_ttf -O2
