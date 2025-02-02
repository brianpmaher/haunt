#!/bin/bash

# Build the engine and editor

echo "Building all..."

# Build the engine
./scripts/linux/build-engine.sh
if [ $? -ne 0 ]; then
    exit 1
fi

# Build the editor
./scripts/linux/build-editor.sh
if [ $? -ne 0 ]; then
    exit 1
fi

echo "Done building all" 