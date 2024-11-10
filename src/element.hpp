#pragma once

// libs
#include <raylib.h>

// std
#include <cstdint>

namespace physics_automata {

// Bit flags
enum ElementFeatures : std::uint32_t {
  Fluid = 1 << 0,
  Solid = 1 << 1,
  Gas = 1 << 2,
  Moveable = 1 << 3,
  Immovable = 1 << 4,
};

class Element {
public:
  // Default ctor
  Element() = default;

  // Initialize values
  Element(Color col, std::uint32_t feat) : color(col), features(feat) {}

  // Return elements color
  Color GetColor() const { return color; }

  // Return elements features bit field
  std::uint32_t GetFeatures() const { return features; }

private:
  // Element color
  Color color = WHITE;

  // Element features bit field
  std::uint32_t features = 0;
};

} // namespace physics_automata
