<div style="display: flex; align-items: center; padding: 32px; flex-direction: column; border-bottom: 1px solid gray;">
  <img src="./docs/media/haunt_logo_white.png" alt="haunt logo">
</div>

<div style="display: flex; justify-content: center; margin: 16px;">
  <a href="https://brainmade.org/">
    <img src="https://img.shields.io/badge/brainmade.org-FFFFFF?style=flat&logo=data%3Aimage%2Fsvg%2Bxml%3Bbase64%2CPHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIxZW0iIGhlaWdodD0iNzkiIHZpZXdCb3g9IjAgMCA2NyA3OSIgZmlsbD0ibm9uZSI%2BPHBhdGggZmlsbD0iIzAwMCIgZD0iTTUyLjYxMiA3OC43ODJIMjMuMzNhMi41NTkgMi41NTkgMCAwIDEtMi41Ni0yLjU1OHYtNy42NzdoLTcuOTczYTIuNTYgMi41NiAwIDAgMS0yLjU2LTIuNTZWNTUuMzE1bC04LjgyLTQuMzk3YTIuNTU5IDIuNTU5IDAgMCAxLS45ODYtMy43MWw5LjgwNy0xNC43MTR2LTQuMzVDMTAuMjQgMTIuNTk5IDIyLjg0MyAwIDM4LjM4OCAwIDUzLjkzMiAwIDY2LjUzNCAxMi42IDY2LjUzOCAyOC4xNDNjLS42MzIgMjcuODI0LTEwLjc2IDIzLjUxNi0xMS4xOCAzNC4wNDVsLS4xODcgMTQuMDM1YTIuNTkgMi41OSAwIDAgMS0uNzUgMS44MSAyLjU1IDIuNTUgMCAwIDEtMS44MDkuNzVabS0yNi43MjMtNS4xMTdoMjQuMTY0bC4yODYtMTQuNTQyYy0uMjYzLTYuNjU2IDExLjcxNi04LjI0MyAxMS4wOC0zMC43MzQtLjM1OC0xMi43MTMtMTAuMzEzLTIzLjI3MS0yMy4wMzEtMjMuMjcxLTEyLjcxOCAwLTIzLjAyOSAxMC4zMDctMjMuMDMyIDIzLjAyNXY1LjExN2MwIC41MDYtLjE1IDEtLjQzIDEuNDJsLTguNjMgMTIuOTQxIDcuNjQ1IDMuODJhMi41NTkgMi41NTkgMCAwIDEgMS40MTUgMi4yOTF2OS42OTdoNy45NzRhMi41NTkgMi41NTkgMCAwIDEgMi41NiAyLjU1OXY3LjY3N1oiLz48cGF0aCBmaWxsPSIjMDAwIiBkPSJNNDAuMzcyIDU4LjIyMlYzOC45MzRjLjExOCAwIC4yMzcuMDE4LjM1NS4wMTggOS43NjktLjAxMiAxNy4wNS05LjAxMiAxNS4wMjItMTguNTY3YTIuMzY2IDIuMzY2IDAgMCAwLTEuODIxLTEuODIyYy04LjEwNi0xLjczLTE2LjEyMSAzLjI5Mi0xOC4wOTggMTEuMzQxLS4wMjQtLjAyNC0uMDQzLS4wNS0uMDY2LS4wNzNhMTUuMzIzIDE1LjMyMyAwIDAgMC0xNC4wNi00LjE3IDIuMzY1IDIuMzY1IDAgMCAwLTEuODIxIDEuODJjLTIuMDI4IDkuNTU1IDUuMjUyIDE4LjU1NCAxNS4wMiAxOC41NjguMjM2IDAgLjQ5Mi0uMDI4LjczOC0uMDR2MTIuMjEzaDQuNzMxWm0yLjgzOS0zMi4xNDNhMTAuNjQ2IDEwLjY0NiAwIDAgMSA4LjEyNC0zLjEwNmMuMzUgNi4zNC00Ljg4OCAxMS41NzctMTEuMjI4IDExLjIzYTEwLjU4IDEwLjU4IDAgMCAxIDMuMTA0LTguMTI0Wk0yNy40MDMgMzguMTkzYTEwLjYwNyAxMC42MDcgMCAwIDEtMy4xMTgtOC4xMjNjNi4zNDQtLjM1OCAxMS41ODcgNC44ODYgMTEuMjI4IDExLjIzLTMuMDIzLjE2OS01Ljk3My0uOTYxLTguMTEtMy4xMDdaIi8%2BPC9zdmc%2B" alt="brainmade logo">
  </a>
</div>

Haunt is a game engine I'm developing for the kinds of games I want to make and how I want to make them.

More info to come...

![Clear colors](./docs/media/clear_colors.gif)

## Index

- [Attributions](ATTRIBUTION.md)
- [Contributors](CONTRIBUTORS.md)
- [License](LICENSE)

## Development

All scripts should be run from the project root directory.

Scripts for each host platform are location within `scripts/<platform>`. For example, Windows scripts are all within `scripts\windows`.

### Windows

To setup the project for the first time, or after changes have been made, run `scripts\windows\setup.bat` from a cmd terminal.

#### Scripts

- `setup.bat` - Setup the project and all of its dependencies
- `build-all.bat` - Build the engine and editor
- `build-engine.bat` - Build the engine
- `build-editor.bat` - Build the editor

### Linux

To setup the project for the first time, or after changes have been made, run `scripts/linux/setup.bat` from a bash terminal.

#### Scripts

- `setup.sh` - Setup the project and all of its dependencies
- `build-all.sh` - Build the engine and editor
- `build-engine.sh` - Build the engine
- `build-editor.sh` - Build the editor

### MacOS

<span style="color:yellow">MacOS platform setup has not yet started. Open to contributions.</span>.

## Platforms

Windows is the only officially supported target platform at the moment.

Haunt is intended to be a cross-platform engine, so Linux, MacOS, iOS, and Android support are all planned for future releases.

## Copyright

Unless otherwise indicated, the code in this repository is copyrighted by Brian Maher <https://brianpmaher.com>.

Sections of the code that are covered by other copyrights and licenses are explicitly indicated in the source files or in the [ATTRIBUTION.md](ATTRIBUTION.md) file.

The `engine/deps` directory contains third-party dependencies, which are covered by their own licenses and copyrights.

Copyright (C) 2025 Brian Maher <https://brianpmaher.com>

## License

Unless otherwise indicated, the code in this repository is licensed under the Apache License, Version 2.0. See the [LICENSE](LICENSE) file for more details.
