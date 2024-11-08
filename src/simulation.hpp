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
    InitWindow(screenWidth, screenHeight, "Physics Automata");

    // Seed the random number generator
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    // Create a render texture
    renderTexture = LoadRenderTexture(screenWidth, screenHeight);

    // Create an array to hold pixel data
    pixels = new Color[screenWidth * screenHeight];

    SetTargetFPS(60);
  }

  ~Simulation() {
    // Free the pixel data array
    delete[] pixels;

    // Unload render texture
    UnloadRenderTexture(renderTexture);
    CloseWindow(); // Close window and OpenGL context
  }

  void loop() {
    while (!WindowShouldClose()) {
      mousePos = GetMousePosition();

      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        for (std::size_t i = 0; i < 16; i++) {
          for (std::size_t j = 0; j < 16; j++) {
            if (mousePos.y + i < screenHeight && mousePos.x + j < screenWidth) {
              elements[mousePos.y + i][mousePos.x + j] = Element(
                  RED, ElementFeatures::Moveable | ElementFeatures::Solid);
            }
          }
        }
      }

      if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        for (std::size_t i = 0; i < 16; i++) {
          for (std::size_t j = 0; j < 16; j++) {
            if (mousePos.y + i < screenHeight && mousePos.x + j < screenWidth) {
              elements[mousePos.y + i][mousePos.x + j] = Element(
                  YELLOW, ElementFeatures::Moveable | ElementFeatures::Gas);
            }
          }
        }
      }

      if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        for (std::size_t i = 0; i < 16; i++) {
          for (std::size_t j = 0; j < 16; j++) {
            if (mousePos.y + i < screenHeight && mousePos.x + j < screenWidth) {
              elements[mousePos.y + i][mousePos.x + j] = Element(
                  BLUE, ElementFeatures::Immovable | ElementFeatures::Solid);
            }
          }
        }
      }

      for (std::size_t y = screenHeight - 1; y > 0; y--) {
        for (std::size_t x = 0; x < screenWidth; x++) {
          pixels[y * screenWidth + x] = elements[y][x].GetColor();

          if (elements[y][x].GetFeatures() & ElementFeatures::Moveable &&
              elements[y][x].GetFeatures() & ElementFeatures::Gas) {
            std::vector<std::pair<std::int8_t, std::int8_t>> directions = {
                {0, 1},  // Down
                {-1, 1}, // Down-Left
                {1, 1}   // Down-Right
            };

            // Shuffle the directions to randomize the order
            std::shuffle(directions.begin(), directions.end(), rng);

            for (const auto &dir : directions) {
              std::int8_t dx = dir.first;  // Change in x
              std::int8_t dy = dir.second; // Change in y

              // Check if the new position is within bounds and empty
              if (y + dy < screenHeight && x + dx < screenWidth &&
                  elements[y + dy][x + dx].GetFeatures() == 0) {
                elements[y + dy][x + dx] = elements[y][x];
                elements[y][x] = Element{};
                break; // Exit the loop after moving
              }
            }
          }

          if (elements[y][x].GetFeatures() & ElementFeatures::Moveable &&
              elements[y][x].GetFeatures() & ElementFeatures::Solid) {
            // Check if the space directly below is empty
            if (y + 1 < screenHeight && elements[y + 1][x].GetFeatures() == 0) {
              elements[y + 1][x] = elements[y][x];
              elements[y][x] = Element{};
            } else {
              // If not moved down, check diagonal movements
              std::vector<std::pair<std::int8_t, std::int8_t>> directions = {
                  {-1, 1}, // Down-Left
                  {1, 1}   // Down-Right
              };

              // Shuffle the diagonal directions to randomize the order
              std::shuffle(directions.begin(), directions.end(), rng);

              for (const auto &dir : directions) {
                std::int8_t dx = dir.first;  // Change in x
                std::int8_t dy = dir.second; // Change in y

                // Check if the new position is within bounds and empty
                if (y + dy < screenHeight && x + dx < screenWidth &&
                    elements[y + dy][x + dx].GetFeatures() == 0) {
                  elements[y + dy][x + dx] = elements[y][x];
                  elements[y][x] = Element{};
                  break; // Exit the loop after moving
                }
              }
            }
          }
        }
      }

      // Update the texture with the pixel data
      UpdateTexture(renderTexture.texture, pixels);

      // Draw
      BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawTexture(renderTexture.texture, 0, 0, WHITE);
      EndDrawing();
    }
  }

private:
  // Initialization
  constexpr static std::size_t screenHeight = 600;
  constexpr static std::size_t screenWidth = 800;

  // Create a render texture
  RenderTexture2D renderTexture;

  // Create an array to hold pixel data
  Color *pixels;

  // Element array
  std::array<std::array<physics_automata::Element, screenWidth>, screenHeight>
      elements{};

  // Random number generator
  std::mt19937 rng; // Mersenne Twister random number generator

  Vector2 mousePos{};
};

} // namespace physics_automata
