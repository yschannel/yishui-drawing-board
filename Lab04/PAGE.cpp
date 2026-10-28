#include "PAGE.h"
#include "FANCTION.h"
#include "globals.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define MAX_FILENAME_LENGTH 64
static char input_filename[MAX_FILENAME_LENGTH] = {0};
static int filename_cursor = 0;

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

void redraw_canvas_from_shapes();

void redraw_canvas_with_polygons() {
  // clear canvas and then redraw all polygons
  BeginTextureMode(canvas);
  ClearBackground(WHITE);

  for (int i = 0; i < total_polygons; i++) {
    Polygon *p = &polygons[i];
    if (!p->points || p->point_count < 3) {
      continue;
    }

    Color c = {(unsigned char)((p->color >> 16) & 0xFF),
               (unsigned char)((p->color >> 8) & 0xFF),
               (unsigned char)(p->color & 0xFF), 255};

    if (p->is_filled) {
      int tri_count = 0;
      int *indices = triangulate_polygon(p->points, p->point_count, &tri_count);

      if(indices){
        for (int t = 0; t < tri_count;t++){
          int i0 = indices[t * 3 + 0];
          int i1 = indices[t * 3 + 1];
          int i2 = indices[t * 3 + 2];
          DrawTriangle(p->points[i0], p->points[i1], p->points[i2], c);
        }
        MemFree(indices);
      }
    } else {
      for (int j = 0; j < p->point_count; j++) {
        int k = (j + 1) % p->point_count;
        DrawLineEx(p->points[j], p->points[k], brush_size, c);
      }
    }
  }
  EndTextureMode();
}

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

    redraw_canvas_with_polygons();
  }
  if (IsKeyPressed(KEY_F2) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[1];

    redraw_canvas_with_polygons();
  }
  if (IsKeyPressed(KEY_F3) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[2];

    redraw_canvas_with_polygons();
  }
  if (IsKeyPressed(KEY_F4) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[3];

    redraw_canvas_with_polygons();
  }
  if (IsKeyPressed(KEY_F5) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[4];

    redraw_canvas_with_polygons();
  }
  if (IsKeyPressed(KEY_F6) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[5];

    redraw_canvas_with_polygons();
  }
  if (IsKeyPressed(KEY_F7) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[6];

    redraw_canvas_with_polygons();
  }
  if (IsKeyPressed(KEY_F8) && current_polygon_index >= 0 &&
      current_polygon_index < total_polygons && color_values_count > 0) {
    polygons[current_polygon_index].color = color_values[7];

    redraw_canvas_with_polygons();
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
    clear_all_shapes();
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

/*  BeginTextureMode(canvas);*/

  // get mouse position and dertermine whether it is in canvas
  bool mouse_in_canvas = false;
  Vector2 mouse_pos = GetMousePosition();
  if(mouse_pos.x>canvas_area.x && mouse_pos.x<canvas_area.x+canvas_area.width && mouse_pos.y>canvas_area.y && mouse_pos.y<canvas_area.y+canvas_area.height){
    mouse_in_canvas = true;
  }
  //calculate local coordinates relative to the canvas
  Vector2 local_mouse = {mouse_pos.x - canvas_area.x, mouse_pos.y - canvas_area.y};

  //new static state variable(for cross-frame persistence)
  static bool freehand_first_point = true;
  static Vector2 freehand_last_point;
  static Freehand_shape current_freehand_stroke = {0};

  // mouse input
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if(mouse_in_canvas){
      drawing = true;
      start_pos = local_mouse;
      end_pos = local_mouse;

      //initialize freehand mode
      if(current_drawing_mode == mode_pencil){
        freehand_first_point = true;
        freehand_last_point = local_mouse;
      }

    }
  }

  if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && current_drawing_mode == mode_eraser && mouse_in_canvas){
    BeginTextureMode(canvas);
    DrawCircleV(local_mouse, brush_size, WHITE);
    EndTextureMode();
  }else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && drawing && mouse_in_canvas) {
    end_pos = local_mouse;

    BeginTextureMode(canvas);
    ClearBackground(WHITE);

    redraw_canvas_from_shapes();

    //Color preview_color = (current_drawing_mode == mode_eraser) ? WHITE : current_color;
    //float preview_brush = brush_size;
    
    if(current_drawing_mode == mode_pencil){
      if(freehand_first_point ||vector2_distance(local_mouse, freehand_last_point) > 1.0f){
        if(freehand_first_point){
          //initialize stroke
          current_freehand_stroke.points = NULL;
          current_freehand_stroke.point_count = 0;
          current_freehand_stroke.brush_size = brush_size;
          current_freehand_stroke.color = Color_to_int(current_color);
          freehand_first_point = false;
        }

        //add new point
        Vector2 *new_points = (Vector2 *)MemAlloc(sizeof(Vector2) * (current_freehand_stroke.point_count + 1));
        for (int i = 0; i < current_freehand_stroke.point_count; i++){
          new_points[i] = current_freehand_stroke.points[i];
        }
        new_points[current_freehand_stroke.point_count] = local_mouse;
        if(current_freehand_stroke.points != NULL){
          MemFree(current_freehand_stroke.points);
        }

        current_freehand_stroke.points = new_points;
        current_freehand_stroke.point_count++;
        freehand_last_point = local_mouse;

        for (int i = 0; i < current_freehand_stroke.point_count; i++)
        {
          DrawCircleV(current_freehand_stroke.points[i], current_freehand_stroke.brush_size, current_color);
        }
      }
    }else if(current_drawing_mode == mode_line){
        DrawLineEx(start_pos, end_pos, brush_size, current_color);
    }else if(current_drawing_mode == mode_rectangle){
        Rectangle rect = {
            fminf(start_pos.x, end_pos.x),
            fminf(start_pos.y, end_pos.y),
            fabsf(end_pos.x - start_pos.x),
            fabsf(end_pos.y - start_pos.y)};
        DrawRectangleLinesEx(rect, (int)brush_size, current_color);
    }else if(current_drawing_mode == mode_circle){
        float radius = vector2_distance(start_pos, end_pos);
        DrawCircleLinesV(start_pos, radius, current_color);
    }


    EndTextureMode();
  }
  if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && drawing) {
    /*drawing = false;*/
    printf("Mouse released! drawing=%d, mode=%d\n", drawing, current_drawing_mode);

    if(mouse_in_canvas && drawing && current_drawing_mode != mode_eraser){
      Drawable_shape new_shape;

      if(current_drawing_mode == mode_pencil){
        if(!freehand_first_point && current_freehand_stroke.point_count > 0){
          //deep copied
          Freehand_shape copied;
          copied.point_count = current_freehand_stroke.point_count;
          copied.brush_size = current_freehand_stroke.brush_size;
          copied.color = current_freehand_stroke.color;
          copied.points = (Vector2 *)MemAlloc(sizeof(Vector2) * copied.point_count);
          for (int i = 0; i < copied.point_count;i++){
            copied.points[i] = current_freehand_stroke.points[i];
            
          }
          
          new_shape.type = SHAPE_FREEHAND;
          new_shape.data.freehand = copied;

          add_shape(new_shape);
          
          MemFree(current_freehand_stroke.points);
          current_freehand_stroke.points = NULL;
          current_freehand_stroke.point_count = 0;
        }
        freehand_first_point = true;
      }else if(current_drawing_mode == mode_line){
        Line_shape l = {start_pos, end_pos, brush_size, Color_to_int(current_color)};
        new_shape.type = SHAPE_LINE;
        new_shape.data.line = l;
        add_shape(new_shape);
      }else if(current_drawing_mode == mode_rectangle){
        Rect_shape r = {0};
        r.rect = (Rectangle){fminf(start_pos.x, end_pos.x),
           fminf(start_pos.y, end_pos.y),
           fabsf(end_pos.x - start_pos.x), 
           fabsf(end_pos.y - end_pos.y)};
        r.color = Color_to_int(current_color);
        new_shape.type = SHAPE_RECTANGLE;
        new_shape.data.rect = r;
        add_shape(new_shape);
        printf("Saving rectangle!\n");
      }else if(current_drawing_mode == mode_circle){
        Circle_shape c = {start_pos,
          vector2_distance(start_pos, end_pos),
          Color_to_int(current_color)};
        new_shape.type = SHAPE_CIRCLE;
        new_shape.data.circle = c;
        add_shape(new_shape);
      }

      redraw_canvas_from_shapes();
    }
    drawing = false;
    freehand_first_point = true;
  }

  // xxx shift polygon filled(not to essence)
  if (IsKeyPressed(KEY_F)) {
    if (current_polygon_index >= 0 && current_polygon_index < total_polygons) {
      polygons[current_polygon_index].is_filled =
          !polygons[current_polygon_index].is_filled;

      redraw_canvas_with_polygons();
    }
  }

  // redraw previous polygon
  if (IsKeyPressed(KEY_U) && total_polygons > 0) {
    // delete the last one
    total_polygons--;

    // free the memory
    if (polygons[total_polygons].points) {
      MemFree(polygons[total_polygons].points);
      polygons[total_polygons].points = NULL;
    }

    // process current polygon index
    if (total_polygons > 0) {
      current_polygon_index = total_polygons - 1;
    } else {
      current_polygon_index = -1;
    }

    // redraw the rest
    redraw_canvas_with_polygons();

    // draw a new
    state.previous_page = state.current_page;
    state.current_page = page_custom_drawing_input;
  }

  //save
  if (IsKeyPressed(KEY_S)) {
    state.previous_page = state.current_page;
    state.current_page = page_save_input;
  }

  //load
  if (IsKeyPressed(KEY_D)) {
    state.previous_page = state.current_page;
    state.current_page = page_load_select;
  }

  //preview polygons
  if (IsKeyPressed(KEY_V)) {
    state.previous_page = state.current_page;
    state.current_page = page_custom_drawing_output;
  }

  //File
  if (IsKeyPressed(KEY_I)) {
    state.previous_page = state.current_page;
    state.current_page = page_file;
  }

  return 0;
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
  DrawText("A. Save Drawing", 200, 200, 30, BLACK);
  DrawText("B. Load Drawing", 200, 250, 30, BLACK);
  DrawText("Q. Quit Drawing", 200, 300, 30, BLACK);
  DrawText("P. Back to previous page", 200, 350, 30, BLACK);
  if (IsKeyPressed(KEY_A)) {
    state.current_page = page_save_input;
  } else if (IsKeyPressed(KEY_B)) {
    state.current_page = page_load_select;
  } else if (IsKeyPressed(KEY_Q)) {
    state.previous_page = state.current_page;
    state.current_page = page_quit;
  }else if(IsKeyPressed(KEY_P)){
    state.current_page = state.previous_page;
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
  } else if (IsKeyPressed(KEY_F)) {
    state.previous_page = state.current_page;
    state.current_page = page_file;
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

    BeginTextureMode(canvas);
    ClearBackground(WHITE);
    EndTextureMode();
  }

  // input page
  DrawRectangle(200, 150, 1200, 600, (Color){240, 240, 240, 255});
  DrawRectangleLines(200, 150, 1200, 600, DARKGRAY);

  // step hint
  switch (polygon_input.current_step) {
  case 1: {
    DrawText(
    TextFormat("Now Polygon: %s (Press F)", 
        global_polygon_filled_mode ? "FILLED" : "WIREFRAME"),
    250, 170, 30,
    global_polygon_filled_mode ? GREEN : ORANGE
    );
    DrawText("Step1:input polygon's side(3-20)", 250, 200, 40, BLACK);
    DrawText("Please input a number,and then press 'ENTER' to confirm\n(Press F1 will back to drawing board)", 250,
             250, 30, DARKGRAY);
    break;
  }

  case 2: {
    DrawText(TextFormat("Step2:input point(%d/%d) position ",
                        polygon_input.current_point_index + 1,
                        polygon_input.target_sides),
             250, 200, 40, BLACK);
    DrawText("Format: x,y(EX:100,200)", 250, 250, 30, DARKGRAY);
    DrawText("Then press 'ENTER' to confirm,'B' to back previous point", 250,
             290, 30, DARKGRAY);
    DrawText("Input the points in COUNTERCLOCKWISE manner!!!!", 250, 500, 50,
             RED);
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
  if (IsKeyPressed(KEY_F1)) {
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
  //polygons[current_polygon_index].color = Color_to_int(current_color);
  if (selected_color_index >= 0 && selected_color_index < color_values_count) {
        polygons[current_polygon_index].color = color_values[selected_color_index];
    } else {
        polygons[current_polygon_index].color = 0x000000; // fallback black
    }
  
  

  polygons[current_polygon_index].is_filled = global_polygon_filled_mode;

  // manage memory to store points
  polygons[current_polygon_index].points =
      (Vector2 *)MemAlloc(sides * sizeof(Vector2));

  total_polygons++;

  printf("DEBUG: selected_color_index=%d, color_value=0x%06X\n", 
           selected_color_index, polygons[current_polygon_index].color);
}

static void save_point_to_polygon(int point_index, float x, float y) {
  if (current_polygon_index >= 0 && current_polygon_index < total_polygons &&
      point_index < polygons[current_polygon_index].side) {
    polygons[current_polygon_index].points[point_index].x = x;
    polygons[current_polygon_index].points[point_index].y = y;
  }
}

static void draw_current_polygon() {
  // unify status
  if(current_polygon_index >= 0 && current_polygon_index < total_polygons){
    Drawable_shape new_shape;
    new_shape.type = SHAPE_POLYGON;
    new_shape.data.polygon = polygons[current_polygon_index];
    //deep copy
    Polygon *src = &polygons[current_polygon_index];
    Polygon *dst = &new_shape.data.polygon;
    dst->points = (Vector2 *)MemAlloc(sizeof(Vector2) * src->point_count);
    for (int i = 0; i < src->point_count;i++){
      dst->points[i] = src->points[i];
    }

    add_shape(new_shape);
  }

  redraw_canvas_with_polygons();
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

// redraw canvas from shapes
void redraw_canvas_from_shapes(){
  // clear canvas and then redraw all shapes
  BeginTextureMode(canvas);
  ClearBackground(WHITE);

  for (int i = 0; i < total_shapes;i++){
    Drawable_shape *s = &all_shapes[i];
    switch (s->type){
      case SHAPE_POLYGON:{
        Polygon *poly = &s->data.polygon;
        Color c = Int_to_color(poly->color);

        if (poly->is_filled) {
          int tri_count = 0;
          int *indices = triangulate_polygon(poly->points, poly->point_count, &tri_count);

          if(indices){
            for (int t = 0; t < tri_count;t++){
              int i0 = indices[t * 3 + 0];
              int i1 = indices[t * 3 + 1];
              int i2 = indices[t * 3 + 2];
              DrawTriangle(poly->points[i0], poly->points[i1], poly->points[i2], c);
            }
            MemFree(indices);
          }
        } else {
          for (int j = 0; j < poly->point_count; j++) {
            int k = (j + 1) % poly->point_count;
            DrawLineEx(poly->points[j], poly->points[k], brush_size, c);
          }
        }
        break;
      }
      case SHAPE_LINE:{
        Line_shape *l = &s->data.line;
        DrawLineEx(l->p1, l->p2, l->brush_size, Int_to_color(l->color));
        break;
      }
      case SHAPE_RECTANGLE:{
        Rect_shape *r = &s->data.rect;
        DrawRectangleRec(r->rect, Int_to_color(r->color));
        break;
      }
      case SHAPE_CIRCLE:{
        Circle_shape *c = &s->data.circle;
        DrawCircleV(c->center, c->radius, Int_to_color(c->color));
        break;
      }
      case SHAPE_FREEHAND:{
        Freehand_shape* f = &s->data.freehand;
        Color col = Int_to_color(f->color);
        for(int j = 0; j < f->point_count;j++){
          DrawCircleV(f->points[j], f->brush_size, col); 
        }
        break;
      }
    }
  }
  EndTextureMode();
}

// not yet
int PAGE_SAVE_PROMPT() {
  // not yet.back to drawing
  state.current_page = page_drawing;
  return 0;
}

//file
int PAGE_SAVE_INPUT(){
  ClearBackground(BEIGE);
  DrawText("Save Drawing - Enter name (no .ydf):", 100, 100, 30, BLACK);
  DrawText("(F1.back to drawing board)", 100, 180, 30, BLACK);
  DrawText(input_filename, 100, 260, 30, DARKGRAY);
  DrawText("_", 100 + MeasureText(input_filename, 30), 260, 30, GRAY);

  //Text Input Processing
  if(IsKeyPressed(KEY_BACKSPACE) && filename_cursor > 0){
    input_filename[--filename_cursor] = '\0';
  }else if(filename_cursor < MAX_FILENAME_LENGTH - 1){
    for (int key = KEY_A; key <= KEY_Z;key++){
      if(IsKeyPressed(key)){
        input_filename[filename_cursor++] = 'a' + (key - KEY_A);
        input_filename[filename_cursor] = '\0';
      }
    }
    for (int key = KEY_ZERO; key <= KEY_NINE;key++){
      if(IsKeyPressed(key)){
        input_filename[filename_cursor++] = '0' + (key - KEY_ZERO);
        input_filename[filename_cursor] = '\0';
      }
    }
    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
      if(IsKeyPressed(KEY_MINUS)){
        input_filename[filename_cursor++] = '_';
        input_filename[filename_cursor] = '\0';
      }
    }
  }

  //Confirm Save
  if(IsKeyPressed(KEY_ENTER) && filename_cursor > 0){
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "drawings/%s.ydf", input_filename);
    save_drawing_to_file(full_path);

    //reset and back
    memset(input_filename, 0, sizeof(input_filename));
    filename_cursor = 0;
    state.current_page = page_drawing;
  }

  //cancel
  if(IsKeyPressed(KEY_F1)){
    memset(input_filename, 0, sizeof(input_filename));
    filename_cursor = 0;
    state.current_page = page_drawing;
  }

  return 0;
}

void PAGE_LOAD_SELECT(){
  ClearBackground(BEIGE);
  DrawText("Load Drawing - Select by number:", 100, 50, 30, BLACK);

  scan_saved_files();//flesh list

  if(saved_file_count == 0){
    DrawText("No .ydf files found in 'drawings/' folder.\nPress 'C' to back", 100, 150, 25, RED);
  }else {
    for (int i = 0; i < saved_file_count && i < 20;i++){
      char text[150];
      snprintf(text, sizeof(text), "%d. %s", i + 1, saved_filenames[i]);
      DrawText(text, 120, 100 + i * 40, 25, DARKGRAY);
    }
    DrawText("Press 1-9 or 0 (for 10th)", 100, 100 + saved_file_count * 40 + 20, 20, GRAY);
    DrawText("C. Back", 100, GetScreenHeight() - 50, 25, DARKGRAY);

    //Number Key Selection
    if(saved_file_count > 0){
      for (int i = 0; i < 10;i++){
        int key = (i == 0) ? KEY_ZERO : (KEY_ONE + i - 1);
        int index = (i == 0) ? 9 : (i - 1); //0 -> 10

        if(IsKeyPressed( key) && index < saved_file_count){
          char full_path[256];
          snprintf(full_path, sizeof(full_path), "drawings/%s.ydf", saved_filenames[index]);
          if(load_drawing_from_file(full_path)){
            state.current_page = page_drawing;
          }else {
            printf(" Failed to load: %s\n", full_path);
          }
          //load false.
          break;
        }
      }
    }

  }
  if(IsKeyPressed(KEY_C)){
    state.current_page = state.previous_page;
  }
}
