# Modern C++23 ALSA Module Wrapper

A dependency-free, zero-overhead, macro-encapsulated C++23 module abstraction wrapper over the Advanced Linux Sound Architecture (ALSA) kernel drivers.

## Overview

This repository provides a clean compile-time boundary over the legacy ALSA C library interface. By utilizing modern C++23 modules, it completely restricts macro pollution from leaking into external global namespaces while maintaining strict direct-to-metal translation. 

All functions are explicitly written as inline forwarding commands to dissolve abstraction boundaries entirely under compiler optimizations.

## Features

* Complete encapsulation of legacy global C macros.
* Strict compile-time translation unit isolation.
* Zero runtime performance or binary size overhead.
* Strongly-typed C++23 enums for stream, format, and hardware parameter control.
* Elimination of generic type-erased pointers via type-safe opaque structs.

## Prerequisites

To build and use this module, your system requires:
* Linux operating system
* ALSA development libraries (libasound2-dev or alsa-lib-devel)
* GCC 11+ supporting C++20/C++23 Modules (-fmodules-ts)

## Repository Structure

* `newALSA.cppm` : The core C++23 module interface and implementation containing the inline wrapper.
* `build.sh` : Single-line deployment script executing the standalone module compilation.

## Compilation

The wrapper is compiled as a standalone object format artifact without built-in executables or test benches. 

To build the module file independently, execute:
```bash
chmod +x build.sh
./build.sh
```

The underlying build command runs:
```bash
g++ -std=c++23 -fmodules-ts -O3 -c newALSA.cppm -o newALSA.o -lasound
```

This operations yields:
1. `newALSA.o` : The compiled object file ready to link into external applications.
2. `gcm.cache/` : The compiled module interface directory required by GCC for external module import operations.

## Integration Example

Once compiled via the standalone pipeline, you can import this module into your codebase without header includes:

```cpp
import newALSA;
import std;

int main() {
    alsa::pcm::Handle* pcm = nullptr;
    int err = alsa::pcm::open(&pcm, "default", alsa::pcm::StreamType::Playback, 0);
    if (err < 0) {
        std::cerr << alsa::strerror(err) << std::endl;
        return 1;
    }
    
    // Legacy macros like SND_PCM_STREAM_PLAYBACK are completely blocked here
    
    alsa::pcm::close(pcm);
    return 0;
}
```
## License


// Copyright (C) 2026 mxreal64
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://gnu.org>.
