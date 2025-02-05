# Haunt Game Engine Roadmap

## Version 0.1.0

- [x] Project setup
  - [x] VSCode project for windows
  - [x] VSCode project for linux
  - [x] Windows build system
  - [x] Add gitattributes
  - [x] Linux build system
- [ ] Planning
  - [ ] Plan engine architecture
  - [ ] Plan roadmap
- [ ] Core systems
  - [x] Logging
  - [x] Memory
  - [ ] Input
    - [x] Input system
    - [ ] Test all input types
    - [x] Win32 Platform Input
    - [ ] Linux Platform Input
  - [ ] Event
    - [x] Event system
    - [x] Test all engine events
    - [ ] Test custom events
    - [x] Win32 Platform Event
    - [ ] Linux Platform Event
- [ ] Memory_Arena
  - [ ] platform_memory_reserve, platform_memory_commit
- [ ] Collection
  - [ ] Dynamic_Array
  - [ ] Array
  - [ ] Hash_Table
  - [ ] String
- [ ] Platform
  - [x] Windows - Win32
  - [x] Linux - X11
- [ ] Renderer
  - [x] OpenGL init
    - [x] Windows
    - [ ] Linux
  - [ ] Support gl debug callback
  - [ ] Render triangle
  - [ ] Render quad
  - [ ] Render text
  - [ ] Research and plan renderer abstraction
- [ ] Editor
  - [ ] Open a window
- [ ] Event system improvements
  - [ ] Multithreaded event system
  - [ ] Multi-frame event handling
    - Use a free-list?
- [ ] Document functions
- [ ] Multiple windows
  - [ ] Windows
  - [ ] Linux
- [ ] MacOS platform
- [ ] Build improvements
  - [ ] Use msvc compiler for Windows
  - [ ] Use single translation unit builds
    - [ ] Windows
    - [ ] Linux
