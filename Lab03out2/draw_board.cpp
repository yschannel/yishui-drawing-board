
#include "FANCTION.h"
#include "PAGE.h"
#include "globals.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>

// initialization
program_state state = {.current_page = page_file,
                       .previous_page = page_file,
                       .should_quit = false};

const int screen_width = 1600;
const int screen_height = 900;
// canvas variables
RenderTexture2D canvas;
Color current_color = BLACK;
Color color_library[] = {BLACK, RED,      ORANGE, YELLOW,
                         GREEN, DARKBLUE, BLUE,   PURPLE};
int color_count = sizeof(color_library) / sizeof(color_library[0]);
int selected_color_index = 0;
drawing_mode current_drawing_mode = mode_pencil;
Vector2 start_pos = {0, 0};
Vector2 end_pos = {0, 0};
bool drawing = false;
float brush_size = 1.0f;

// polygon canvas variables
Polygon *polygons = NULL;
int current_polygon_index = -1;
int total_polygons = 0;
int polygon_max_capacity = 20;

// polygon state
Polygon_input_state polygon_input = {.is_input_active = false,
                                     .current_step = 0,
                                     .target_sides = 0,
                                     .current_point_index = 0,
                                     .input_buffer = {0},
                                     .input_pos = 0,
                                     .input_complete = false};

bool need_redraw_all_polygons = false;

int main(void) {
  // setting

  InitWindow(screen_width, screen_height, "Yishui drawing board!");
  SetTargetFPS(60);

  // initialize canvas
  canvas = LoadRenderTexture(screen_width, screen_height);
  BeginTextureMode(canvas);
  ClearBackground(WHITE);
  EndTextureMode();

  initialize_canvas();

  // main program
  while (!state.should_quit && !WindowShouldClose()) {
    switch (state.current_page) {
    case page_file:
      PAGE_FILE();
      break;

    case page_file_read:
      PAGE_FILE_READ();
      break;

    case page_file_not_read:
      PAGE_FILE_NOT_READ();
      break;

    case page_welcome:
      PAGE_WELCOME();
      break;

    case page_drawing:
      PAGE_DRAWING();
      break;

    case page_custom_drawing_input:
      PAGE_CUSTOM_DRAWING_INPUT();
      break;

    case page_quit:
      PAGE_QUIT();
      break;

    case page_custom_drawing_output:
      PAGE_CUSTOM_DRAWING_OUTPUT();
      break;

    case page_save_prompt:
      PAGE_SAVE_PROMPT();
      break;

    default:
      if (IsKeyPressed(KEY_Q)) {
        state.previous_page = state.current_page;
        state.current_page = page_quit;
      }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    switch (state.current_page) {
    case page_file:
      ClearBackground(BEIGE);
      DrawText("Welcome to Yishui darwing board\nWhat would you like to "
               "do?\nA.Read the file\nB.Do not read the file\nQ.quit",
               200, 100, 40, BLACK);
      break;

    case page_file_read:
      ClearBackground(BEIGE);
      DrawText(
          "Read successfully!\nDo you want to open it?\nY.Yes\nB.No\nQ.quit",
          200, 100, 40, BLACK);
      break;

    case page_file_not_read:
      ClearBackground(BEIGE);
      DrawText("Not read!\nDo you want to back the file page?\nY.Yes\nQ.quit",
               200, 100, 40, BLACK);
      break;

    case page_welcome:
      ClearBackground(BEIGE);
      DrawText("Welcome to Yishui darwing board\nWhat would you like to "
               "do?\nA.Drawing\nQ.quit",
               200, 100, 40, BLACK);
      break;

    case page_drawing:
      // make a canvas
      DrawTextureRec(canvas.texture,
                     (Rectangle){0, 0, (float)canvas.texture.width,
                                 (float)-canvas.texture.height},
                     (Vector2){0, 0}, WHITE);
      // UI
      DrawRectangle(0, 0, screen_width, 40, (Color){200, 200, 200, 200});
      DrawText("Current color:", 10, 10, 20, DARKGRAY);
      DrawRectangle(180, 15, 20, 10, current_color);

      const char *mode_text;
      switch (current_drawing_mode) {
      case mode_pencil:
        mode_text = "pencil";
        break;
      case mode_eraser:
        mode_text = "eraser";
        break;
      case mode_line:
        mode_text = "line";
        break;
      case mode_rectangle:
        mode_text = "rectangle";
        break;
      case mode_circle:
        mode_text = "circle";
        break;
      default:
        mode_text = "unknown";
        break;
      }
      DrawText(TextFormat("Current mode:%s", mode_text), 600, 10, 20, DARKGRAY);

      DrawText(TextFormat("Brush size:%.0f", brush_size), 1000, 10, 20,
               DARKGRAY);

      DrawText("KEY MEAN:0~7color P:pencil E:eraser L:line R:rectangle "
               "O:circle UP/DOWN:brush_size SPACE:clear Q:quit",
               10, screen_height - 25, 20, DARKGRAY);

      DrawText("POLY MODE: M:input F:fillshift F1~8:color U:redraw", 10,
               screen_height - 50, 20, DARKGRAY);

      // display adding polygon information
      if (current_polygon_index >= 0 &&
          current_polygon_index < total_polygons) {
        Polygon *current_poly = &polygons[current_polygon_index];

        DrawText(TextFormat("Current poly:%dsides,%s", current_poly->side,
                            current_poly->is_filled ? "Filled" : "Shape"),
                 10, 50, 20, DARKGRAY);

        DrawText("Points:", 10, 80, 18, DARKGRAY);
        for (int i = 0; i < current_poly->point_count && i < 5; i++) {
          DrawText(TextFormat("point%d:(%.0f,%.0f)", i + 1,
                              current_poly->points[i].x,
                              current_poly->points[i].y),
                   10, 105 + i * 20, 16, GRAY);
        }
        if (current_poly->point_count > 5) {
          DrawText("...", 10, 105 + 5 * 20, 16, GRAY);
        }
      }

      // preview drawn shape
      if (drawing && ((current_drawing_mode == mode_line) ||
                      (current_drawing_mode == mode_rectangle) ||
                      (current_drawing_mode == mode_circle))) {
        switch (current_drawing_mode) {
        case mode_line:
          DrawLineEx(start_pos, end_pos, brush_size, current_color);
          break;
        case mode_rectangle: {
          Rectangle rect = {
              fminf(start_pos.x, end_pos.x), fminf(start_pos.y, end_pos.y),
              fabsf(start_pos.x - end_pos.x), fabsf(start_pos.y - end_pos.y)};
          DrawRectangleLinesEx(rect, brush_size, current_color);
          break;
        }
        case mode_circle: {
          float radius = vector2_distance(start_pos, end_pos);
          DrawCircleLines((int)start_pos.x, (int)start_pos.y, radius,
                          current_color);
          break;
        }
        default: {
          break;
        }
        }
      }
      break;

    case page_quit:
      ClearBackground(BEIGE);
      DrawText("Do you want to quit the program?\nY.yes\nN.No", 200, 100, 40,
               BLACK);
      break;

    case page_custom_drawing_output:
      ClearBackground(BEIGE);
      PAGE_CUSTOM_DRAWING_OUTPUT();
      break;

    case page_save_prompt:
      ClearBackground(BEIGE);
      DrawText("unfinish", 200, 100, 40, BLACK);
      DrawText("B to back", 200, 200, 40, BLACK);
      if (IsKeyPressed(KEY_B)) {
        state.current_page = state.previous_page;
      }
      break;

    default:
      ClearBackground(BEIGE);
      DrawText("404", 200, 100, 40, BLACK);
    }
    EndDrawing();
  }

  for (int i = 0; i < total_polygons; i++) {
    if (polygons[i].points) {
      MemFree(polygons[i].points);
      polygons[i].points = NULL;
    }
  }
  if (polygons) {
    MemFree(polygons);
    polygons = NULL;
  }
  UnloadRenderTexture(canvas);
  CloseWindow();
  return 0;
}
