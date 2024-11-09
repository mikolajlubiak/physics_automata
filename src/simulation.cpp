// header
#include "simulation.hpp"

namespace physics_automata {

// Initialize values
Simulation::Simulation() {
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

// Destroy objects and free memory
Simulation::~Simulation() {
  // Free the pixel data array
  delete[] m_PixelArray;

  // Unload render texture
  UnloadRenderTexture(m_RenderTexture);
  CloseWindow(); // Close window and OpenGL context
}

// Simulation loop
void Simulation::loop() {
  while (!WindowShouldClose()) {
    Update();
    Draw();
  }
}

// Update simulation state
void Simulation::Update() {
  UpdateElements();

  ProcessMouseInput();

  UpdateTexture(m_RenderTexture.texture, m_PixelArray);
}

// Draw simulation state
void Simulation::Draw() const {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  DrawTexture(m_RenderTexture.texture, 0, 0, WHITE);
  DrawRectangleLines(m_MousePos.x, m_MousePos.y,
                     static_cast<float>(m_DrawRadius),
                     static_cast<float>(m_DrawRadius), GREEN);
  EndDrawing();
}

// Process mouse input
void Simulation::ProcessMouseInput() {
  // Update mouse related data
  m_DrawRadius = static_cast<std::uint32_t>(m_DrawRadius + GetMouseWheelMove());
  m_MousePos = GetMousePosition();

  // Movable solid
  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    for (std::size_t i = 0; i < m_DrawRadius; i++) {
      for (std::size_t j = 0; j < m_DrawRadius; j++) {
        if (m_MousePos.y + i < m_ScreenHeight &&
            m_MousePos.x + j < m_ScreenWidth) {
          m_ElementsNew[m_MousePos.y + i][m_MousePos.x + j] =
              Element(RED, ElementFeatures::Moveable | ElementFeatures::Solid);
        }
      }
    }
  }

  // Movable gas
  if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
    for (std::size_t i = 0; i < m_DrawRadius; i++) {
      for (std::size_t j = 0; j < m_DrawRadius; j++) {
        if (m_MousePos.y + i < m_ScreenHeight &&
            m_MousePos.x + j < m_ScreenWidth) {
          m_ElementsNew[m_MousePos.y + i][m_MousePos.x + j] =
              Element(YELLOW, ElementFeatures::Moveable | ElementFeatures::Gas);
        }
      }
    }
  }

  // Immovable solid
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

// Update the elements state
void Simulation::UpdateElements() {
  m_ElementsOld = m_ElementsNew;

  for (std::size_t y = m_ScreenHeight - 1; y > 0; y--) {
    for (std::size_t x = 0; x < m_ScreenWidth; x++) {
      // Update the pixel array
      m_PixelArray[y * m_ScreenWidth + x] = m_ElementsOld[y][x].GetColor();

      // If movable gas
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

      // If movable solid
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

} // namespace physics_automata
