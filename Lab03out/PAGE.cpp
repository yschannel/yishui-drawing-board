#include"PAGE.h"
#include"raylib.h"
#include"globals.h"
#include<math.h>
#include <stdlib.h>

int PAGE_DRAWING(){
    //color selection
    if(IsKeyPressed(KEY_ZERO)) selected_color_index=0;
    if(IsKeyPressed(KEY_ONE)) selected_color_index=1;
    if(IsKeyPressed(KEY_TWO)) selected_color_index=2;
    if(IsKeyPressed(KEY_THREE)) selected_color_index=3;
    if(IsKeyPressed(KEY_FOUR)) selected_color_index=4;
    if(IsKeyPressed(KEY_FIVE)) selected_color_index=5;
    if(IsKeyPressed(KEY_SIX)) selected_color_index=6;
    if(IsKeyPressed(KEY_SEVEN)) selected_color_index=7;
    //drawing mode selection
    if(IsKeyPressed(KEY_P))current_drawing_mode=mode_pencil;
    if(IsKeyPressed(KEY_E))current_drawing_mode=mode_eraser;
    if(IsKeyPressed(KEY_L))current_drawing_mode=mode_line;
    if(IsKeyPressed(KEY_R))current_drawing_mode=mode_rectangle;
    if(IsKeyPressed(KEY_C))current_drawing_mode=mode_circle;
    //brush size adjustment
    if(IsKeyPressed(KEY_UP))brush_size=(brush_size<50.0f)?brush_size+1.0f:50.0f;
    if(IsKeyPressed(KEY_DOWN))brush_size=(brush_size>1.0f)?brush_size-1.0f:1.0f;
    //clear canvas
    if(IsKeyPressed(KEY_SPACE)){
        BeginTextureMode(canvas);
        ClearBackground(WHITE);
        EndTextureMode();
    }
    
    //quit drawing page
    if(IsKeyPressed(KEY_Q)){
        state.previous_page=state.current_page;
        state.current_page=page_quit;
    }
    //update current color
    current_color = color_library[selected_color_index];
    //drawing start
    BeginTextureMode(canvas);
    //get mouse position
    Vector2 mouse_pos = GetMousePosition();
    //mouse input
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        drawing = true;
        start_pos = mouse_pos;
        end_pos = mouse_pos;
    }
    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON) && drawing){
        end_pos = mouse_pos;
        //mode_pencil and mode_eraser
        switch(current_drawing_mode){
            case mode_pencil:
                DrawCircleV(mouse_pos,brush_size,current_color);
                break;
            case mode_eraser:
                DrawCircleV(mouse_pos,brush_size,WHITE);
                break;
            default:
                //draw shapes after mouse_button_released
                break;
        }
    }
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)&&drawing){
        drawing = false;
        switch(current_drawing_mode){
            case mode_line:
                DrawLineEx(start_pos,end_pos,brush_size,current_color);
                break;
            case mode_rectangle:{
                Rectangle rect ={
                    fminf(start_pos.x,end_pos.x),
                    fminf(start_pos.y,end_pos.y),
                    fabsf(start_pos.x-end_pos.x),
                    fabsf(start_pos.y-end_pos.y)
                };
                DrawRectangleRec(rect,current_color);
                break;
            }
            case mode_circle:{
                float radius =vector2_distance(start_pos,end_pos);
                DrawCircleV(start_pos,radius,current_color);
                break;
            }    
            default:
                break;
        }
    }
    EndTextureMode();

    return 0;
}

int PAGE_FILE_NOT_READ(){
    if(IsKeyPressed(KEY_Y)){
        state.current_page=page_file;
    }else if(IsKeyPressed(KEY_Q)){
        state.previous_page=state.current_page;
        state.current_page=page_quit;
    }
    return 0;
}

int PAGE_FILE_READ(){
    if(IsKeyPressed(KEY_Y)){
        state.current_page=page_welcome;
    }else if(IsKeyPressed(KEY_N)){
        state.current_page=page_file;
    }else if(IsKeyPressed(KEY_Q)){
        state.previous_page=state.current_page;
        state.current_page=page_quit;
    }
    return 0;
}

int PAGE_FILE(){
    if(IsKeyPressed(KEY_A)){
            state.current_page=page_file_read;
        }else if(IsKeyPressed(KEY_B)){
            state.current_page=page_file_not_read;
        }else if(IsKeyPressed(KEY_Q)){
            state.previous_page=state.current_page;
            state.current_page=page_quit;
        }
    return 0;
}

int PAGE_QUIT(){
    if(IsKeyPressed(KEY_Y)){
        state.should_quit=true;
    }else if(IsKeyPressed(KEY_N)){
        state.current_page=state.previous_page;
    }
    return 0;
}

int PAGE_WELCOME(){
    if(IsKeyPressed(KEY_A)){
        state.current_page=page_drawing;
    }else if(IsKeyPressed(KEY_Q)){
        state.previous_page=state.current_page;
        state.current_page=page_quit;
    }
    return 0;
}

int PAGE_CUSTOM_DRAWING_INPUT(){
    Get_key temp={
        .content = "\0",
        .char_count = 0,
        .is_input_over = false
    };

    while(!temp.is_input_over){
        int character = GetCharPressed();
        //process character input
        if(temp.char_count < 255 && character > 0){
            temp.content[temp.char_count] = (char)character;
            temp.char_count++;
            temp.content[temp.char_count] = '\0';
        }

        if(IsKeyPressed(KEY_BACKSPACE)){
            temp.char_count--;
            if(temp.char_count < 0) temp.char_count = 0;
            temp.content[temp.char_count] = '\0';
        }

        if(IsKeyPressed(KEY_ENTER)){
            temp.is_input_over = true;
        }
    }
    
    while(temp.char_count > 0){
        char *current = temp.content;
        char *end;

        while(*current != '\0'){
            if(*current == 'a' || *current == 'A'){
                current++;
                polygons[current_polygon_index].side = strtol(current,&end,10);
                polygons[current_polygon_index].point_count = polygons[current_polygon_index].side;
                resize_point_array()
                current = end;
            }

            if(*current == 'b' || *current == 'B'){
                current++;
                polygons[current_polygon_index].color = strtol(current,&end,10);
                current = end;
            }

            if(*current == 'c' || *current == 'C'){
                current++;
                polygons[current_polygon_index].is_filled = strtol(current,&end,10);
                current = end;
            }

            if(*current == 'd' || *current == 'D'){
                int x = 0;
                int y = 0;
                while(*current != '\0'){
                    if(*current == 'd' || *current == 'D' || *current == ';'){
                        
                    }
                }
            }
        }
    }
}