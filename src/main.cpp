// local
#include "simulation.hpp"

// libs
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// std
#include <cstdlib>

physics_automata::Simulation sim{};
void loop() { sim.loop(); }

int main() {

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(loop, 0, 1);
#else
  while (!WindowShouldClose()) {
    loop();
  }
#endif

  return EXIT_SUCCESS;
}
