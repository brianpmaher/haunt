#!/bin/bash

# Build the engine

# Find source files
sources=$(find engine/src -name "*.c")

# Build the engine
assembly="haunt"
cflags="-g -shared -Wall -Werror -Wno-gnu-folding-constant"
includes="-Iengine/src"
linker="-lX11"
defines="-D_DEBUG -DDLL_EXPORT"

echo "Building $assembly..."
clang $sources $cflags -o bin/lib$assembly.so $defines $includes $linker

if [ $? -ne 0 ]; then
    echo "Failed to build $assembly"
    exit 1
fi

echo "Done building $assembly" 