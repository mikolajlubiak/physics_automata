#pragma once

// local
#include "element.hpp"

// libs
#include <raylib.h>

// std
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <random>

namespace physics_automata {

class Simulation {
public:
  // Initialize values
  Simulation();

  // Destroy objects and free memory
  ~Simulation();

  // Simulation loop
  void loop();

private: // Methods
  // Update simulation state
  void Update();

  // Draw simulation state
  void Draw() const;

  // Process mouse input
  void ProcessMouseInput();

  // Update the elements state
  void UpdateElements();

private: // Attributes
  // Simulation resolution
  constexpr static std::size_t m_ScreenHeight = 600;
  constexpr static std::size_t m_ScreenWidth = 800;

  // Render texture
  RenderTexture2D m_RenderTexture;

  // Array to hold pixel data
  Color *m_PixelArray;

  // Old element array
  std::array<std::array<physics_automata::Element, m_ScreenWidth>,
             m_ScreenHeight>
      m_ElementsOld{};

  // New element array
  std::array<std::array<physics_automata::Element, m_ScreenWidth>,
             m_ScreenHeight>
      m_ElementsNew{};

  // Random number generator
  std::mt19937 m_Rng;

  // Mouse position
  Vector2 m_MousePos{};

  // Mouse drawing radius
  std::uint32_t m_DrawRadius = 16;
};

} // namespace physics_automata
