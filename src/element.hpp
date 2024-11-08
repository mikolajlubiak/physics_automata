#pragma once

// libs
#include <raylib.h>

// std
#include <cstdint>

namespace physics_automata {

enum ElementFeatures : std::uint32_t {
  Fluid = 1 << 0,
  Solid = 1 << 1,
  Gas = 1 << 2,
  Moveable = 1 << 3,
  Immovable = 1 << 4,
};

class Element {
public:
  Element() {}

  Element(Color col, std::uint32_t feat) : color(col), features(feat) {}

  Color GetColor() { return color; }
  std::uint32_t GetFeatures() { return features; }

private:
  Color color = WHITE;
  std::uint32_t features = 0;
};

} // namespace physics_automata
