#!/bin/bash

# Check for required tools
echo "Checking for required tools..."

# git
if ! command -v git &> /dev/null; then
	echo "Git is not installed. Please install Git and try again."
	exit 1
fi
echo "git found"

# clang
if ! command -v clang &> /dev/null; then
	echo "Clang is not installed. Please install Clang and try again."
	exit 1
fi
echo "clang found"

# X11 development files
if ! pkg-config --exists x11; then
	echo "X11 development files not found. Please install them (e.g., libx11-dev on Ubuntu)"
	exit 1
fi
echo "X11 development files found"

# Set up submodules
echo "Setting up submodules..."
git submodule update --init --recursive

# Create bin directory
mkdir -p bin

echo "Setup complete" 
