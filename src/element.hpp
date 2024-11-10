#pragma once

// libs
#include <raylib.h>

// std
#include <cstdint>
#include <utility>
#include <vector>

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

  Element(Color col, std::uint32_t feat,
          std::vector<std::vector<std::pair<std::int8_t, std::int8_t>>> dirs)
      : color(col), features(feat), directions(dirs) {}

  // Return elements color
  Color GetColor() const { return color; }

  // Return elements features bit field
  std::uint32_t GetFeatures() const { return features; }

  // Return elements possible moves
  std::vector<std::vector<std::pair<std::int8_t, std::int8_t>>>
  GetDirections() const {
    return directions;
  }

private:
  // Element color
  Color color = WHITE;

  // Element features bit field
  std::uint32_t features = 0;

  // Possible moves
  // First vector specifies order in which to consider the possible moves
  // Second vector specifies the pairs that will be randomly chosen
  std::vector<std::vector<std::pair<std::int8_t, std::int8_t>>> directions{};
};

} // namespace physics_automata
