# physics_automata
Physics simulation using cellular automata

# Build
* Executable:
```
cmake -S . -B build
cd build
cmake --build .
```

* Web:
```shell
cmake -S . -B build_web -G Ninja -DPLATFORM=Web "-DCMAKE_TOOLCHAIN_FILE=PATH_TO_EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
cd build_web
cmake --build .
```

# Run
* Executable:
```
./physics_automata
```

* Web:
```
emrun physics_automata.html
```

