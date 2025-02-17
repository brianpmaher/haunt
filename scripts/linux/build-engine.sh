#!/bin/bash

# Build the engine

# Find source files
sources=$(find engine/src -name "*.c")
sources="$sources engine/deps/glad/src/glad.c"

# Build the engine
assembly="haunt"
cflags="-g -shared -fPIC -Wall -Werror -Wno-gnu-folding-constant -Wno-unused-function -std=c17"
includes="-Iengine/src -Iengine/deps -Iengine/deps/glad/include"
linker="-lX11 -lGL -lm -lGLX"
defines="-D_DEBUG -DDLL_EXPORT -DPLATFORM_LINUX"

echo "Building $assembly..."
clang $sources $cflags -o bin/lib$assembly.so $defines $includes $linker

if [ $? -ne 0 ]; then
	echo "Failed to build $assembly"
	exit 1
fi

echo "Done building $assembly" 
