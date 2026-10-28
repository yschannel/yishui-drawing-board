#include "PAGE.h"
#include "FANCTION.h"
#include "globals.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void redraw_all_polygons();

static const int color_values[] = {
    0x000000, // black
    0xFF0000, // red
    0xFFA500, // orange
    0xFFFF00, // yellow
    0x00FF00, // green
    0x00008B, // darkblue
    0x0000FF, // blue
    0x800080  // purple
};

const int color_values_count = sizeof(color_values) / sizeof(color_values[0]);

int PAGE_DRAWING() {
  // color selection
  if (IsKeyPressed(KEY_ZERO))
    selected_color_index = 0;
  if (IsKeyPressed(KEY_ONE))
    selected_color_index = 1;
  if (IsKeyPressed(KEY_TWO))
    selected_color_index = 2;
  if (IsKeyPressed(KEY_THREE))
    selected_color_index = 3;
  if (IsKeyPressed(KEY_FOUR))
    selected_color_index = 4;
  if (IsKeyPressed(KEY_FIVE))
    selected_color_index = 5;
  if (IsKeyPressed(KEY_SIX))
    selected_color_index = 6;
  if (IsKeyPressed(KEY_SEVEN))
    selected_color_index = 7;
  // polygon color selection
  if (IsKeyPressed(KEY_F1) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[0];
    redraw_all_polygons();
  }
  if (IsKeyPressed(KEY_F2) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[1];
    redraw_all_polygons();
  }
  if (IsKeyPressed(KEY_F3) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[2];
    redraw_all_polygons();
  }
  if (IsKeyPressed(KEY_F4) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[3];
    redraw_all_polygons();
  }
  if (IsKeyPressed(KEY_F5) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[4];
    redraw_all_polygons();
  }
  if (IsKeyPressed(KEY_F6) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[5];
    redraw_all_polygons();
  }
  if (IsKeyPressed(KEY_F7) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[6];
    redraw_all_polygons();
  }
  if (IsKeyPressed(KEY_F8) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[7];
    redraw_all_polygons();
  }
  // drawing mode selection
  if (IsKeyPressed(KEY_P))
    current_drawing_mode = mode_pencil;
  if (IsKeyPressed(KEY_E))
    current_drawing_mode = mode_eraser;
  if (IsKeyPressed(KEY_L))
    current_drawing_mode = mode_line;
  if (IsKeyPressed(KEY_R))
    current_drawing_mode = mode_rectangle;
  if (IsKeyPressed(KEY_O))
    current_drawing_mode = mode_circle;
  if (IsKeyPressed(KEY_M)) {
    state.previous_page = state.current_page;
    state.current_page = page_custom_drawing_input;
  }
  // brush size adjustment
  if (IsKeyPressed(KEY_UP))
    brush_size = (brush_size < 50.0f) ? brush_size + 1.0f : 50.0f;
  if (IsKeyPressed(KEY_DOWN))
    brush_size = (brush_size > 1.0f) ? brush_size - 1.0f : 1.0f;
  // clear canvas
  if (IsKeyPressed(KEY_SPACE)) {
    BeginTextureMode(canvas);
    ClearBackground(WHITE);
    EndTextureMode();
  }

  // quit drawing page
  if (IsKeyPressed(KEY_Q)) {
    state.previous_page = state.current_page;
    state.current_page = page_quit;
  }
  // update current color
  current_color = color_library[selected_color_index];
  // drawing start
  BeginTextureMode(canvas);
  // get mouse position
  Vector2 mouse_pos = GetMousePosition();
  // mouse input
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    drawing = true;
    start_pos = mouse_pos;
    end_pos = mouse_pos;
  }
  if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && drawing) {
    end_pos = mouse_pos;
    // mode_pencil and mode_eraser
    switch (current_drawing_mode) {
    case mode_pencil:
      DrawCircleV(mouse_pos, brush_size, current_color);
      break;
    case mode_eraser:
      DrawCircleV(mouse_pos, brush_size, WHITE);
      break;
    default:
      // draw shapes after mouse_button_released
      break;
    }
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && drawing) {
    drawing = false;
    switch (current_drawing_mode) {
    case mode_line:
      DrawLineEx(start_pos, end_pos, brush_size, current_color);
      break;
    case mode_rectangle: {
      Rectangle rect = {
          fminf(start_pos.x, end_pos.x), fminf(start_pos.y, end_pos.y),
          fabsf(start_pos.x - end_pos.x), fabsf(start_pos.y - end_pos.y)};
      DrawRectangleRec(rect, current_color);
      break;
    }
    case mode_circle: {
      float radius = vector2_distance(start_pos, end_pos);
      DrawCircleV(start_pos, radius, current_color);
      break;
    }
    default:
      break;
    }
  }

  // shift polygon filled
  if (IsKeyPressed(KEY_F)) {
    if (current_polygon_index >= 0 && current_polygon_index < total_polygons) {
      polygons[current_polygon_index].is_filled =
          !polygons[current_polygon_index].is_filled;
      // redraw
      need_redraw_all_polygons = true;
    }
  }
  if (need_redraw_all_polygons) {
    redraw_all_polygons();
    need_redraw_all_polygons = false;
  }

  // redraw previous polygon
  if (IsKeyPressed(KEY_U) && total_polygons > 0) {
    int last_index = total_polygons - 1;
    Polygon *last_poly = &polygons[last_index];

    if (total_polygons >= polygon_max_capacity) {
      resize_polygons_array(polygon_max_capacity + 10);
    }

    current_polygon_index = total_polygons;

    // copy polygon
    polygons[current_polygon_index].side = last_poly->side;
    polygons[current_polygon_index].color = last_poly->color;
    polygons[current_polygon_index].is_filled = last_poly->is_filled;
    polygons[current_polygon_index].point_count = last_poly->point_count;

    polygons[current_polygon_index].points =
        (Vector2 *)MemAlloc(last_poly->point_count * sizeof(Vector2));

    if (last_poly->point_count > 0 && last_poly->points != NULL) {

      for (int i = 0; i < last_poly->point_count; i++) {
        polygons[current_polygon_index].points[i].x =
            last_poly->points[i].x + 30;
        polygons[current_polygon_index].points[i].y =
            last_poly->points[i].y + 30;
      }
    } else {
      polygons[current_polygon_index].points = NULL;
    }

    total_polygons++;

    BeginTextureMode((canvas));

    Color poly_color;
    poly_color.r = (polygons[current_polygon_index].color >> 16) & 0xFF;
    poly_color.g = (polygons[current_polygon_index].color >> 8) & 0xFF;
    poly_color.b = polygons[current_polygon_index].color & 0xFF;
    poly_color.a = 255;

    // draw polygon
    if (polygons[current_polygon_index].is_filled) {
      DrawTriangleFan(polygons[current_polygon_index].points,
                      polygons[current_polygon_index].side, poly_color);
    } else {
      for (int i = 0; i < polygons[current_polygon_index].point_count; i++) {
        int j = (i + 1) % polygons[current_polygon_index].point_count;
        DrawLineEx(polygons[current_polygon_index].points[i],
                   polygons[current_polygon_index].points[j], brush_size,
                   poly_color);
      }
    }

    EndTextureMode();
  }

  EndTextureMode();

  return 0;
}

static void redraw_all_polygons() {
  // store canvas
  RenderTexture2D temp_canvas = LoadRenderTexture(screen_width, screen_height);
  BeginTextureMode(temp_canvas);
  ClearBackground(WHITE);

  // redraw all polygons
  for (int poly_idx = 0; poly_idx < total_polygons; poly_idx++) {
    Polygon *poly = &polygons[poly_idx];

    // check
    if (!poly || !poly->points || poly->point_count < 3) {
      continue;
    }

    Color poly_color;
    poly_color.r = (poly->color >> 16) & 0xFF;
    poly_color.g = (poly->color >> 8) & 0xFF;
    poly_color.b = poly->color & 0xFF;
    poly_color.a = 255;

    BeginTextureMode(canvas);

    if (poly->is_filled) {
      // calculate central point
      Vector2 center = {0, 0};
      for (int pt_idx = 0; pt_idx < poly->point_count; pt_idx++) {
        center.x += poly->points[pt_idx].x;
        center.y += poly->points[pt_idx].y;
      }
      center.x /= poly->point_count;
      center.y /= poly->point_count;

      // found a point array:central and all
      Vector2 *triangle_fan_vertices =
          (Vector2 *)MemAlloc((poly->point_count + 2) * sizeof(Vector2));
      triangle_fan_vertices[0] = center;
      for (int pt_idx = 0; pt_idx < poly->point_count; pt_idx++) {
        triangle_fan_vertices[pt_idx + 1] = poly->points[pt_idx];
      }
      triangle_fan_vertices[poly->point_count + 1] = poly->points[0]; // close

      DrawTriangleFan(triangle_fan_vertices, poly->point_count + 2, poly_color);
      MemFree(triangle_fan_vertices);
    } else {
      for (int pt_idx = 0; pt_idx < poly->point_count; pt_idx++) {
        int next_idx = (pt_idx + 1) % poly->point_count;
        DrawLineEx(poly->points[pt_idx], poly->points[next_idx], brush_size,
                   poly_color);
      }
    }

    EndTextureMode();

    // copy temp cavas to initial canvas
    BeginTextureMode(canvas);
    ClearBackground(WHITE);
    DrawTextureRec(temp_canvas.texture,
                   (Rectangle){0, 0, (float)temp_canvas.texture.width,
                               (float)-temp_canvas.texture.height},
                   (Vector2){0, 0}, WHITE);
    EndTextureMode();

    UnloadRenderTexture((temp_canvas));
  }
}

int PAGE_FILE_NOT_READ() {
  if (IsKeyPressed(KEY_Y)) {
    state.current_page = page_file;
  } else if (IsKeyPressed(KEY_Q)) {
    state.previous_page = state.current_page;
    state.current_page = page_quit;
  }
  return 0;
}

int PAGE_FILE_READ() {
  if (IsKeyPressed(KEY_Y)) {
    state.current_page = page_welcome;
  } else if (IsKeyPressed(KEY_N)) {
    state.current_page = page_file;
  } else if (IsKeyPressed(KEY_Q)) {
    state.previous_page = state.current_page;
    state.current_page = page_quit;
  }
  return 0;
}

int PAGE_FILE() {
  if (IsKeyPressed(KEY_A)) {
    state.current_page = page_file_read;
  } else if (IsKeyPressed(KEY_B)) {
    state.current_page = page_file_not_read;
  } else if (IsKeyPressed(KEY_Q)) {
    state.previous_page = state.current_page;
    state.current_page = page_quit;
  }
  return 0;
}

int PAGE_QUIT() {
  if (IsKeyPressed(KEY_Y)) {
    state.should_quit = true;
  } else if (IsKeyPressed(KEY_N)) {
    state.current_page = state.previous_page;
  }
  return 0;
}

int PAGE_WELCOME() {
  if (IsKeyPressed(KEY_A)) {
    state.current_page = page_drawing;
  } else if (IsKeyPressed(KEY_Q)) {
    state.previous_page = state.current_page;
    state.current_page = page_quit;
  } else if (IsKeyPressed(KEY_V)) {
    state.current_page = page_custom_drawing_output;
  }
  return 0;
}

// page_custom_drawing_input

static void handle_polygon_input();
static void process_current_input();
static void create_new_polygon(int sides);
static void save_point_to_polygon(int point_index, float x, float y);
static void draw_current_polygon();
static void reset_polygon_input_state();

int PAGE_CUSTOM_DRAWING_INPUT() {
  // iniitalization
  if (!polygon_input.is_input_active) {
    polygon_input.is_input_active = true;
    polygon_input.current_step = 1;
    polygon_input.current_point_index = 0;
    polygon_input.input_buffer[0] = '\0';
    polygon_input.input_pos = 0;
    polygon_input.input_complete = false;
  }

  // input page
  DrawRectangle(200, 150, 1200, 600, (Color){240, 240, 240, 255});
  DrawRectangleLines(200, 150, 1200, 600, DARKGRAY);

  // step hint
  switch (polygon_input.current_step) {
  case 1: {
    DrawText("Step1:input polygon's side(3-20)", 250, 200, 40, BLACK);
    DrawText("Please input a number,and then press 'ENTER' to confirm", 250,
             250, 30, DARKGRAY);
    break;
  }

  case 2: {
    DrawText(TextFormat("Step2:input point(%d/%d) position",
                        polygon_input.current_point_index + 1,
                        polygon_input.target_sides),
             250, 200, 40, BLACK);
    DrawText("Format: x,y(EX:100,200)", 250, 250, 30, DARKGRAY);
    DrawText("Then press 'ENTER' to confirm,'B' to back previous point", 250,
             290, 30, DARKGRAY);
    break;
  }
  }

  // input display
  DrawRectangle(250, 350, 1100, 50, LIGHTGRAY);
  DrawRectangleLines(250, 350, 1100, 50, BLACK);
  DrawText("Input: ", 200, 355, 30, BLACK);

  DrawText(polygon_input.input_buffer, 290, 355, 30, BLACK);

  // draw cursor
  if (((int)(GetTime() * 2) % 2) == 0) {
    int text_width = MeasureText(polygon_input.input_buffer, 30);
    DrawRectangle(290 + text_width, 350, 2, 30, BLACK);
  }

  // points display
  if (polygon_input.current_step == 2 &&
      polygon_input.current_point_index > 0) {
    DrawText("Inputed points:", 250, 420, 25, DARKGRAY);
    for (int i = 0; i < polygon_input.current_point_index; i++) {
      if (current_polygon_index >= 0 &&
          polygons[current_polygon_index].points) {
        Vector2 point = polygons[current_polygon_index].points[i];
        DrawText(TextFormat("point %d:(%.0f,%.0f)", i + 1, point.x, point.y),
                 250, 450 + i * 25, 22, BLUE);
      }
    }
  }

  // handle polygon input
  handle_polygon_input();

  // if input complete,back to drawing page
  if (polygon_input.input_complete) {
    reset_polygon_input_state();
    state.current_page = page_drawing;
  }

  return 0;
}

static void handle_polygon_input() {
  int key = GetCharPressed();
  while (key > 0) {
    if (polygon_input.input_pos < 255 &&
        ((key >= '0' && key <= '9') || key == ',' || key == '-' ||
         key == '.')) {
      polygon_input.input_buffer[polygon_input.input_pos] = (char)key;
      polygon_input.input_pos++;
      polygon_input.input_buffer[polygon_input.input_pos] = '\0';
    }
    key = GetCharPressed();
  }

  // backspace
  if (IsKeyPressed(KEY_BACKSPACE) && polygon_input.input_pos > 0) {
    polygon_input.input_pos--;
    polygon_input.input_buffer[polygon_input.input_pos] = '\0';
  }

  // enter
  if (IsKeyPressed(KEY_ENTER) && polygon_input.input_pos > 0) {
    process_current_input();
  }

  // back to previous point
  if (polygon_input.current_step == 2 && IsKeyPressed(KEY_B)) {
    if (polygon_input.current_point_index > 0) {
      polygon_input.current_point_index--;
      polygon_input.input_buffer[0] = '\0';
      polygon_input.input_pos = 0;
    }
  }

  // esc to cancel input
  if (IsKeyPressed(KEY_ESCAPE)) {
    reset_polygon_input_state();
    state.current_page = page_drawing;
  }
}

// process current input
static void process_current_input() {
  switch (polygon_input.current_step) {
  case 1: {
    polygon_input.target_sides = atoi(polygon_input.input_buffer);

    if (polygon_input.target_sides >= 3 && polygon_input.target_sides <= 20) {
      create_new_polygon(polygon_input.target_sides);

      polygon_input.current_step = 2;
      polygon_input.current_point_index = 0;
      polygon_input.input_buffer[0] = '\0';
      polygon_input.input_pos = 0;
    } else {
      polygon_input.input_buffer[0] = '\0';
      polygon_input.input_pos = 0;
    }
    break;
  }

  case 2: {
    float x = 0, y = 0;
    if (sscanf(polygon_input.input_buffer, "%f,%f", &x, &y) == 2) {
      if (x >= 0 && x <= screen_width && y >= 0 && y <= screen_height) {
        save_point_to_polygon(polygon_input.current_point_index, x, y);
        polygon_input.current_point_index++;

        if (polygon_input.current_point_index >= polygon_input.target_sides) {
          draw_current_polygon();

          polygon_input.input_complete = true;
        } else {
          polygon_input.input_buffer[0] = '\0';
          polygon_input.input_pos = 0;
        }
      } else {
        polygon_input.input_buffer[0] = '\0';
        polygon_input.input_pos = 0;
      }

    } else {
      polygon_input.input_buffer[0] = '\0';
      polygon_input.input_pos = 0;
    }
    break;
  }
  }
}

static void create_new_polygon(int sides) {
  if (total_polygons >= polygon_max_capacity) {
    resize_polygons_array(polygon_max_capacity + 10);
  }

  current_polygon_index = total_polygons;
  // initialize polygon
  polygons[current_polygon_index].side = sides;
  polygons[current_polygon_index].point_count = sides;
  polygons[current_polygon_index].color = 0xFF0000; // red
  polygons[current_polygon_index].is_filled = 0;

  // manage memory to store points
  polygons[current_polygon_index].points =
      (Vector2 *)MemAlloc(sides * sizeof(Vector2));

  total_polygons++;
}

static void save_point_to_polygon(int point_index, float x, float y) {
  if (current_polygon_index >= 0 && current_polygon_index < total_polygons &&
      point_index < polygons[current_polygon_index].side) {
    polygons[current_polygon_index].points[point_index].x = x;
    polygons[current_polygon_index].points[point_index].y = y;
  }
}

static void draw_current_polygon() {
  if (current_polygon_index >= 0 && current_polygon_index < total_polygons) {

    Polygon *poly = &polygons[current_polygon_index];

    Color poly_color;
    poly_color.r = (poly->color >> 16) & 0xFF;
    poly_color.g = (poly->color >> 8) & 0xFF;
    poly_color.b = poly->color & 0xFF;
    poly_color.a = 255;

    BeginTextureMode(canvas);

    if (poly->is_filled) {
      DrawTriangleFan(polygons[current_polygon_index].points,
                      polygons[current_polygon_index].side, poly_color);
    } else {
      for (int i = 0; i < poly->point_count; i++) {
        int j = (i + 1) % poly->point_count;
        DrawLineEx(poly->points[i], poly->points[j], brush_size, poly_color);
      }
    }

    EndTextureMode();
  }
}

static void reset_polygon_input_state() {
  polygon_input.is_input_active = false;
  polygon_input.current_step = 0;
  polygon_input.target_sides = 0;
  polygon_input.current_point_index = 0;
  polygon_input.input_buffer[0] = '\0';
  polygon_input.input_pos = 0;
  polygon_input.input_complete = false;
}

void save_darwing_to_file() {}

int PAGE_CUSTOM_DRAWING_OUTPUT() {
  // drawing page
  DrawRectangle(400, 300, 800, 300, (Color){240, 240, 240, 255});
  DrawRectangleLines(400, 300, 800, 300, DARKGRAY);
  DrawText("preview", 550, 330, 40, BLACK);

  // display polygons information
  if (total_polygons > 0) {
    DrawText(TextFormat("finish %d polygons", total_polygons), 450, 400, 30,
             DARKGRAY);
  } else {
    DrawText("no polygons", 500, 400, 30, DARKGRAY);
  }

  DrawText("B to back,Q to quit", 420, 500, 25, DARKGRAY);

  // process press
  if (IsKeyPressed(KEY_B)) {
    state.current_page = page_drawing;
  } else if (IsKeyPressed(KEY_Q)) {
    state.previous_page = state.current_page;
    state.current_page = page_quit;
  }

  return 0;
}

// not yet
int PAGE_SAVE_PROMPT() {
  // not yet.back to drawing
  state.current_page = page_drawing;
  return 0;
}