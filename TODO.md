# Haunt Game Engine Roadmap

## Version 0.1.0 - Hello Haunt

Objective: Provide all of the foundation necessary to make a basic 2D or 3D game for Windows and Linux.

- Project setup
  - [x] VSCode project for windows
  - [x] VSCode project for linux
  - [x] Windows build system
  - [x] Add gitattributes
  - [x] Linux build system
  - [ ] Separate public functionality into `include` directory to keep separate from internal headers
  - [ ] Document public types and functions
  - [ ] Support headless (windowless?) mode for being able to write scripts in C while reusing the library functionality from Haunt
    - This will also be a necessity for any server development support we choose to include in the future.
  - [ ] Generate documentation from `include` directory
  - [ ] Setup .clang-format for automatic formatting
- Planning
  - [ ] Plan engine architecture
  - [ ] Plan roadmap
- Core systems
  - [x] Logging
  - Memory
    - [ ] Memory_Arena
      - Arena should be the foundation for all memory allocations in Haunt.
        - Make sure we define an arena for anything that needs to allocate memory.
  - [ ] platform_memory_reserve, platform_memory_commit
  - [ ] Input
    - [x] Input system
    - [ ] Test all input types
      - [ ] BUG: Mouse event fires down before pressed
    - [x] Win32 Platform Input
    - [ ] Linux Platform Input
  - [ ] Event
    - [x] Event system
    - [x] Test all engine events
    - [ ] Test custom events
    - [x] Win32 Platform Event
    - [ ] Linux Platform Event
- Math
  - [ ] Include all common linear algebra functions
- Collections
  - [ ] Dynamic_Array
  - [ ] Array
  - [ ] Hash_Table
  - [ ] String
- Platform
  - [x] Windows - Win32
  - [x] Linux - X11
- Renderer
  - [x] OpenGL init
    - [x] Windows
    - [ ] Linux
  - [ ] Support gl debug callback
  - [ ] Render triangle
  - [ ] Render quad
  - [ ] Render text
  - [ ] Render sprite animation
- Camera
  - [ ] Plan camera system
- Collision
  - [ ] Plan collision system

## Backlog

- [ ] Editor
  - [ ] Open a window
- [ ] Event system improvements
  - [ ] Multithreaded event system
  - [ ] Multi-frame event handling
    - Use a free-list?
- [ ] Multiple windows
  - [ ] Windows
  - [ ] Linux
- [ ] MacOS platform
