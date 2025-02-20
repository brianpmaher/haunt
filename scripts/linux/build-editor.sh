#!/bin/bash

# Build the editor

# Find source files
sources=$(find editor/src -name "*.c")

# Build the editor
assembly="editor"
cflags="-g -Wall -Werror -Wno-gnu-folding-constant -Wno-unused-function -std=c17"
includes="-Ieditor/src -Iengine/src"
linker="-Lbin -lhaunt -lX11 -lm"
defines="-D_DEBUG -DDLL_IMPORT"

echo "Building $assembly..."
clang $sources $cflags -o bin/$assembly $defines $includes $linker

if [ $? -ne 0 ]; then
	echo "Failed to build $assembly"
	exit 1
fi

echo "Done building $assembly" 
