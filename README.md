# Physics simulation using cellular automata
### Demo: https://lubiak.pages.dev/physics_automata/
### Showcase: https://video.infosec.exchange/w/p/nEq5vSe8VVvk9sDBqruDPW

# Build
* Executable:
```shell
# Generate files
cmake -S . -B build

# Build
cmake --build build
```

* Web:
```shell
# Generate files
emcmake cmake -S . -B build_web
# or
cmake -S . -B build_web -G Ninja -DPLATFORM=Web -DCMAKE_TOOLCHAIN_FILE=/usr/lib/emscripten/cmake/Modules/Platform/Emscripten.cmake

# Build
cmake --build build_web

# Host
emrun build_web/physics_automata.html
```

