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
  Simulation() {
    // Create window and OpenGL context
    InitWindow(m_ScreenWidth, m_ScreenHeight, "Physics Automata");

    // Seed the random number generator
    m_Rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    // Create a render texture
    m_RenderTexture = LoadRenderTexture(m_ScreenWidth, m_ScreenHeight);

    // Create an array to hold pixel data
    m_PixelArray = new Color[m_ScreenWidth * m_ScreenHeight];

    // Set target FPS
    SetTargetFPS(60);
  }

  ~Simulation() {
    // Free the pixel data array
    delete[] m_PixelArray;

    // Unload render texture
    UnloadRenderTexture(m_RenderTexture);
    CloseWindow(); // Close window and OpenGL context
  }

  void loop() {
    while (!WindowShouldClose()) {
      Update();
      Draw();
    }
  }

private: // Methods
  void Update() {
    UpdateElements();

    ProcessMouseInput();

    UpdateTexture(m_RenderTexture.texture, m_PixelArray);
  }

  void Draw() const {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(m_RenderTexture.texture, 0, 0, WHITE);
    DrawRectangleLines(m_MousePos.x, m_MousePos.y,
                       static_cast<float>(m_DrawRadius),
                       static_cast<float>(m_DrawRadius), GREEN);
    EndDrawing();
  }

  void ProcessMouseInput() {
    m_DrawRadius =
        static_cast<std::uint32_t>(m_DrawRadius + GetMouseWheelMove());
    m_MousePos = GetMousePosition();

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      for (std::size_t i = 0; i < m_DrawRadius; i++) {
        for (std::size_t j = 0; j < m_DrawRadius; j++) {
          if (m_MousePos.y + i < m_ScreenHeight &&
              m_MousePos.x + j < m_ScreenWidth) {
            m_ElementsNew[m_MousePos.y + i][m_MousePos.x + j] = Element(
                RED, ElementFeatures::Moveable | ElementFeatures::Solid);
          }
        }
      }
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
      for (std::size_t i = 0; i < m_DrawRadius; i++) {
        for (std::size_t j = 0; j < m_DrawRadius; j++) {
          if (m_MousePos.y + i < m_ScreenHeight &&
              m_MousePos.x + j < m_ScreenWidth) {
            m_ElementsNew[m_MousePos.y + i][m_MousePos.x + j] = Element(
                YELLOW, ElementFeatures::Moveable | ElementFeatures::Gas);
          }
        }
      }
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      for (std::size_t i = 0; i < m_DrawRadius; i++) {
        for (std::size_t j = 0; j < m_DrawRadius; j++) {
          if (m_MousePos.y + i < m_ScreenHeight &&
              m_MousePos.x + j < m_ScreenWidth) {
            m_ElementsNew[m_MousePos.y + i][m_MousePos.x + j] = Element(
                BLUE, ElementFeatures::Immovable | ElementFeatures::Solid);
          }
        }
      }
    }
  }

  void UpdateElements() {
    m_ElementsOld = m_ElementsNew;

    for (std::size_t y = m_ScreenHeight - 1; y > 0; y--) {
      for (std::size_t x = 0; x < m_ScreenWidth; x++) {
        m_PixelArray[y * m_ScreenWidth + x] = m_ElementsOld[y][x].GetColor();

        if (m_ElementsOld[y][x].GetFeatures() & ElementFeatures::Moveable &&
            m_ElementsOld[y][x].GetFeatures() & ElementFeatures::Gas) {
          std::vector<std::pair<std::int8_t, std::int8_t>> directions = {
              {0, 1},  // Down
              {-1, 1}, // Down-Left
              {1, 1}   // Down-Right
          };

          // Shuffle the directions to randomize the order
          std::shuffle(directions.begin(), directions.end(), m_Rng);

          for (const auto &dir : directions) {
            std::int8_t dx = dir.first;  // Change in x
            std::int8_t dy = dir.second; // Change in y

            // Check if the new position is within bounds and empty
            if (y + dy < m_ScreenHeight && x + dx < m_ScreenWidth &&
                m_ElementsOld[y + dy][x + dx].GetFeatures() == 0) {
              m_ElementsNew[y + dy][x + dx] = m_ElementsOld[y][x];
              m_ElementsNew[y][x] = Element{};
              break; // Exit the loop after moving
            }
          }
        }

        if (m_ElementsOld[y][x].GetFeatures() & ElementFeatures::Moveable &&
            m_ElementsOld[y][x].GetFeatures() & ElementFeatures::Solid) {
          // Check if the space directly below is empty
          if (y + 1 < m_ScreenHeight &&
              m_ElementsOld[y + 1][x].GetFeatures() == 0) {
            m_ElementsNew[y + 1][x] = m_ElementsOld[y][x];
            m_ElementsNew[y][x] = Element{};
          } else {
            // If not moved down, check diagonal movements
            std::vector<std::pair<std::int8_t, std::int8_t>> directions = {
                {-1, 1}, // Down-Left
                {1, 1}   // Down-Right
            };

            // Shuffle the diagonal directions to randomize the order
            std::shuffle(directions.begin(), directions.end(), m_Rng);

            for (const auto &dir : directions) {
              std::int8_t dx = dir.first;  // Change in x
              std::int8_t dy = dir.second; // Change in y

              // Check if the new position is within bounds and empty
              if (y + dy < m_ScreenHeight && x + dx < m_ScreenWidth &&
                  m_ElementsOld[y + dy][x + dx].GetFeatures() == 0) {
                m_ElementsNew[y + dy][x + dx] = m_ElementsOld[y][x];
                m_ElementsNew[y][x] = Element{};
                break; // Exit the loop after moving
              }
            }
          }
        }
      }
    }
  }

private: // Attributes
  // Initialization
  constexpr static std::size_t m_ScreenHeight = 600;
  constexpr static std::size_t m_ScreenWidth = 800;

  // Create a render texture
  RenderTexture2D m_RenderTexture;

  // Create an array to hold pixel data
  Color *m_PixelArray;

  // Element array
  std::array<std::array<physics_automata::Element, m_ScreenWidth>,
             m_ScreenHeight>
      m_ElementsOld{};

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
