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
    page_quit
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


#endif