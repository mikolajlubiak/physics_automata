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

  // Turn button index to its X coordinate
  template <typename T> T ButtonIndexToPos(const T index) const {
    return m_ButtonOffset * (index + 1) + m_ButtonWidth * index;
  }

  // Check if button is clicked
  // Return whether the mouse pointer is over the button
  bool CheckButton(const std::size_t index, const Element &element);

  // Draw button
  void DrawButton(const std::size_t index, const char *text,
                  const std::uint32_t element_features) const;

private: // Attributes
  // Simulation resolution
  constexpr static std::size_t m_ScreenHeight = 600;
  constexpr static std::size_t m_ScreenWidth = 800;

  // Define button dimensions and positions
  const std::uint32_t m_ButtonWidth = 100;
  const std::uint32_t m_ButtonHeight = 30;
  const std::uint32_t m_ButtonY = 10;
  const std::uint32_t m_ButtonOffset = 10;

  const std::uint32_t m_SecondButtonPosX = m_ButtonOffset * 2 + m_ButtonWidth;
  const std::uint32_t m_ThirdButtonPosX =
      m_ButtonOffset * 3 + m_ButtonWidth * 2;
  const std::uint32_t m_FourthButtonPosX =
      m_ButtonOffset * 4 + m_ButtonWidth * 3;

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

  //
  Element m_ElementToDraw;
};

} // namespace physics_automata
