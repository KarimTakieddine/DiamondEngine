# DiamondEngine

A long-lasting, ongoing project aimed at the creation of a standalone game engine and the necessary tools to be able to independently develop games. This project is dear to my heart as it was the first C++ project I ever worked on, and being a fan of games and the process of building them, it inevitably and quite quickly spiralled into this almost decade-long adventure I have been happily enjoying since :)

## Features

- Support for both 2D and 3D rendering using the modern OpenGL programmable rendering pipeline
- Building scenes with multiple instances, each given life by way of a library of built-in rendering or behavioural components
- Ability to extend the code base by supplying user-defined rendering or behavioural components for runtime use
- 2D smooth collision detection and impulse resolution
- 2D Animations (Currently frame-by-frame however working on integrating the [Spine
](https://esotericsoftware.com/) C++ runtime to enable importing of skeletal animation data)
- Keyboard and controller support using XInput on Windows
- A functional 2D audio engine making use of the [irrKlang](https://www.ambiera.com/irrklang/) library and supporting multiple formats such as mp3 or wav
- Ability to render fonts to screen and capture debugging events on one or more console windows in game
- Configuration and level parsing / loading from XML

## Architecture

The engine project (which itself is a static library), is split up into the following broad areas, each corresponding to a feature and its own static library dependency:

- The [audio](https://github.com/KarimTakieddine/DiamondEngine/tree/master/lib/audio) library, managing the playback of 2D audio<br />

- The [config](https://github.com/KarimTakieddine/DiamondEngine/tree/master/lib/config) library, responsible for wrapping the config classes corresponding to instances and their respective rendering/behavioural components as well as parsing into such data from XML<br />

- The [debug](https://github.com/KarimTakieddine/DiamondEngine/tree/master/lib/debug) library, where specific runtime debug events and their respective handling is defined<br />

- The [font](https://github.com/KarimTakieddine/DiamondEngine/tree/master/lib/font) library, housing the logic for parsing combined font texture files and rendering associated character combinations to one or more text windows, each with their own configurable styley<br />

- The [game](https://github.com/KarimTakieddine/DiamondEngine/tree/master/lib/game) library,  dictating how game instances are updated, rendered and more generally managed and governing the interactions between them (e.g. physics or user interaction)<br />

- The [graphics](https://github.com/KarimTakieddine/DiamondEngine/tree/master/lib/graphics) library, responsible for fast runtime memory allocations, copying memory to the GPU, drawing and grouping of instances by render type<br />

- The [input](https://github.com/KarimTakieddine/DiamondEngine/tree/master/lib/input) library, charged with monitoring input events and allowing for accurate queries of the current state of user interaction on a frame-by-frame basis<br />

## Build Requirements

- **A 64-bit Windows Operating System (cross-platform audio support currently being investigated)**
- **OpenGL Library version 4.6 and above**
- **CMake version 3.22 and above**
- **A working C++ compiler supporting the C++17 standard**
- **A bit of patience (optional) :)**

Run CMake on the root CMakeLists.txt in the repository folder and you're all set!

## Usage Example

The [ProjectDiamond](https://github.com/KarimTakieddine/ProjectDiamond) application, making use of this very engine as a submodule and leveraging the library to do some really neat stuff. It's cool, I promise!

## License

[MIT](https://opensource.org/license/mit)

Copyright 2025 Karim Takieddine

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
