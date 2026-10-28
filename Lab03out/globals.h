#ifndef GLOBALS_H
#define GLOBALS_H

#include"raylib.h"

//define possible pages
typedef enum{
    page_welcome,
    page_drawing,
    page_file,
    page_file_read,
    page_file_not_read,
    page_quit,
    page_custom_drawing_input,
    page_custom_drawing_output
}program_page;

//drawing mode
typedef enum{
    mode_pencil = 0,
    mode_eraser,
    mode_line,
    mode_rectangle,
    mode_circle
}drawing_mode;

//record the page status
typedef struct{
    program_page current_page;
    program_page previous_page;
    bool should_quit;
}program_state;

//declare variables
extern program_state state;

extern RenderTexture2D canvas;
extern Color current_color;
extern Color color_library[];
extern int selected_color_index;
extern drawing_mode current_drawing_mode;
extern Vector2 start_pos;
extern Vector2 end_pos;
extern bool drawing;
extern float brush_size;

//declare function
float vector2_distance(Vector2 start_Pos, Vector2 end_Pos);

//points information
typedef struct{
    int side;
    Vector2* points;
    int color;
    int is_filled;
    int point_count;
}Polygon;

//get key
typedef struct{
    char content[256];
    int char_count;
    bool is_input_over;
}Get_key;


//declare polygon canvas variables
extern Polygon* polygons;
extern int current_polygon_index;
extern int total_polygons;
extern int polyhon_max_capacity;



#endif