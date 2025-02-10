# Haunt Game Engine Roadmap

## Version 0.1.0 - Hello Haunt

Objective: Provide all of the foundation necessary to make a basic 2D or 3D game for Windows and Linux.

- Project
  - [x] VSCode project for windows
  - [x] VSCode project for linux
  - [x] Windows build system
  - [x] Add gitattributes
  - [x] Linux build system
  - [ ] Setup CMake
  - [ ] Separate public functionality into `include` directory to keep separate from internal headers
  - [ ] Document public types and functions
  - [ ] Generate documentation from `include` directory
  - [ ] Support headless (windowless?) mode for being able to write scripts in C while reusing the library functionality from Haunt
    - This will also be a necessity for any server development support we choose to include in the future.
  - [ ] Setup .clang-format for automatic formatting
  - [ ] Cleanup unnecessary forward declarations... It makes things messy
  - [ ] Trim down function names. Consider putting entire header declaration into haunt.h, similar to raylib.h for a single place to view and document all types and functions.
- [x] Logging
- Memory
  - [ ] Memory_Arena
    - Arena should be the foundation for all memory allocations in Haunt.
      - Make sure we define an arena for anything that needs to allocate memory.
  - [ ] platform_memory_reserve, platform_memory_commit
- Window
  - [x] Create and display window
  - [ ] Show engine version in window title
- Input
  - [x] Input system
  - [x] Win32 Platform Input
    - [x] Test key, mouse button, mouse move, and scroll events
  - [ ] Linux Platform Input
    - [ ] Test key, mouse button, mouse move, and scroll events
- Event
  - [x] Event system
  - [ ] Test custom events
  - [x] Win32 Platform Event
    - [x] Test engine events
  - [ ] Linux Platform Event
    - [ ] Test engine events
- Math
  - [x] Include all common linear algebra functions
- Collections
  - [ ] Dynamic_Array
  - [ ] Array
  - [ ] Hash_Table
  - [ ] String
- Platform
  - [x] Windows - Win32
  - [x] Linux - X11
- Renderer
  - [x] OpenGL 4.6 init
    - [x] Windows
    - [ ] Linux
  - [x] Support gl debug callback
  - [ ] Compile shaders
  - [ ] Render triangle
  - [ ] Render quad
  - [ ] Render text
  - [ ] Render sprite animation
- Camera
  - [ ] Plan camera system
- Audio
  - [ ] Audio system
  - [ ] Play music
  - [ ] Play one-shot audio
- Collision
  - [ ] Plan collision system
- [ ] Editor
  - [x] Open a window
  - [ ] Show logo

## Backlog

- [ ] Event system improvements
  - [ ] Multithreaded event system
  - [ ] Multi-frame event handling
    - Use a free-list?
- [ ] Multiple windows
  - [ ] Windows
  - [ ] Linux
- [ ] MacOS platform
