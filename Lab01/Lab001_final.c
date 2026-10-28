#include"raylib.h"
#include<math.h>
//Define a function that can calculate the distance between two points
float vector2_distance(Vector2 start_Pos,Vector2 end_Pos){
    float dx = end_Pos.x - start_Pos.x;
    float dy = end_Pos.y - start_Pos.y;
    return sqrtf(pow(dx,2)+pow(dy,2));
}
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

int main(void){
    //setting
    const int screen_width =1600;
    const int screen_height=900;
    InitWindow(screen_width,screen_height,"Yishui drawing board!");
    SetTargetFPS(60);

    //initialization
    program_state state={
        .current_page = page_file,
        .previous_page = page_file,
        .should_quit = false
    };

    //canvas variables
    RenderTexture2D canvas=LoadRenderTexture(screen_width,screen_height);
    Color current_color = BLACK;
    Color color_library[]={BLACK,RED,ORANGE,YELLOW,GREEN,DARKBLUE,BLUE,PURPLE};
    int color_count = sizeof(color_library)/sizeof(color_library[0]);
    int selected_color_index=0;
    drawing_mode current_drawing_mode = mode_pencil;
    Vector2 start_pos={0,0};
    Vector2 end_pos={0,0};
    bool drawing = false;
    float brush_size=1.0f;

    //initialize canvas
    BeginTextureMode(canvas);
    ClearBackground(WHITE);
    EndTextureMode();

    //main program
    while(!state.should_quit&&!WindowShouldClose()){
        switch(state.current_page){
            case page_file:
                if(IsKeyPressed(KEY_A)){
                    state.current_page=page_file_read;
                }else if(IsKeyPressed(KEY_B)){
                    state.current_page=page_file_not_read;
                }else if(IsKeyPressed(KEY_Q)){
                    state.previous_page=state.current_page;
                    state.current_page=page_quit;
                }
                break;

            case page_file_read:
                if(IsKeyPressed(KEY_Y)){
                    state.current_page=page_welcome;
                }else if(IsKeyPressed(KEY_N)){
                    state.current_page=page_file;
                }else if(IsKeyPressed(KEY_Q)){
                    state.previous_page=state.current_page;
                    state.current_page=page_quit;
                }
                break;

            case page_file_not_read:
                if(IsKeyPressed(KEY_Y)){
                    state.current_page=page_file;
                }else if(IsKeyPressed(KEY_Q)){
                    state.previous_page=state.current_page;
                    state.current_page=page_quit;
                }
                break;

            case page_welcome:
                if(IsKeyPressed(KEY_A)){
                    state.current_page=page_drawing;
                }else if(IsKeyPressed(KEY_Q)){
                    state.previous_page=state.current_page;
                    state.current_page=page_quit;
                }
                break;

            case page_drawing:
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
                        case mode_rectangle:
                            Rectangle rect ={
                                fminf(start_pos.x,end_pos.x),
                                fminf(start_pos.y,end_pos.y),
                                fabsf(start_pos.x-end_pos.x),
                                fabsf(start_pos.y-end_pos.y)
                            };
                            DrawRectangleRec(rect,current_color);
                            break;
                        case mode_circle:
                            float radius =vector2_distance(start_pos,end_pos);
                            DrawCircleV(start_pos,radius,current_color);
                            break;
                        default:
                            break;
                    }
                }
                EndTextureMode();
                break;

            case page_quit:
                if(IsKeyPressed(KEY_Y)){
                    state.should_quit=true;
                }else if(IsKeyPressed(KEY_N)){
                    state.current_page=state.previous_page;
                }
                break;
            
            default:
                if(IsKeyPressed(KEY_Q)){
                    state.previous_page=state.current_page;
                    state.current_page=page_quit;
                }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(state.current_page){
                case page_file:
                    ClearBackground(BEIGE);
                    DrawText("Welcome to Yishui darwing board\nWhat would you like to do?\nA.Read the file\nB.Do not read the file\nQ.quit",200,100,40,BLACK);
                    break;
                
                case page_file_read:
                    ClearBackground(BEIGE);
                    DrawText("Read successfully!\nDo you want to open it?\nY.Yes\nB.No\nQ.quit",200,100,40,BLACK);
                    break;

                case page_file_not_read:
                    ClearBackground(BEIGE);
                    DrawText("Not read!\nDo you want to back the file page?\nY.Yes\nQ.quit",200,100,40,BLACK);
                    break;

                case page_welcome:
                    ClearBackground(BEIGE);
                    DrawText("Welcome to Yishui darwing board\nWhat would you like to do?\nA.Drawing\nQ.quit",200,100,40,BLACK);
                    break;

                case page_drawing:
                    //make a canvas
                    DrawTextureRec(canvas.texture,
                        (Rectangle){0,0,(float)canvas.texture.width,(float)-canvas.texture.height},
                        (Vector2){0,0},WHITE);
                    //UI
                    DrawRectangle(0,0,screen_width,40,(Color){200,200,200,200});
                    DrawText("Current color:",10,10,20,DARKGRAY);
                    DrawRectangle(180,15,20,10,current_color);

                    const char* mode_text;
                    switch(current_drawing_mode){
                        case mode_pencil:mode_text="pencil";break;
                        case mode_eraser:mode_text="eraser";break;
                        case mode_line:mode_text="line";break;
                        case mode_rectangle:mode_text="rectangle";break;
                        case mode_circle:mode_text="circle";break;
                        default:mode_text="unknown";break;
                    }
                    DrawText(TextFormat("Current mode:%s",mode_text),600,10,20,DARKGRAY);
                    
                    DrawText(TextFormat("Brush size:%.0f",brush_size),1000,10,20,DARKGRAY);

                    DrawText("KEY MEAN:0~7color P:pencil E:eraser L:line R:rectangle C:circle UP/DOWN:brush_size SPACE:clear Q:quit",
                    10,screen_height-20,20,DARKGRAY);

                    //preview drawn shape
                    if(drawing&&((current_drawing_mode==mode_line)||(current_drawing_mode==mode_rectangle)||(current_drawing_mode==mode_circle))){
                        switch(current_drawing_mode){
                            case mode_line:
                                DrawLineEx(start_pos,end_pos,brush_size,current_color);
                                break;
                            case mode_rectangle:
                                Rectangle rect ={
                                    fminf(start_pos.x,end_pos.x),
                                    fminf(start_pos.y,end_pos.y),
                                    fabsf(start_pos.x-end_pos.x),
                                    fabsf(start_pos.y-end_pos.y)
                                };
                                DrawRectangleLinesEx(rect,brush_size,current_color);
                                break;
                            case mode_circle:
                                float radius =vector2_distance(start_pos,end_pos);
                                DrawCircleLines((int)start_pos.x,(int)start_pos.y,radius,current_color);
                                break;
                            default:
                                break;
                        }
                    }
                    break;

                case page_quit:
                    ClearBackground(BEIGE);
                    DrawText("Do you want to quit the program?\nY.yes\nN.No",200,100,40,BLACK );
                    break;

                default:
                    ClearBackground(BEIGE);
                    DrawText("404",200,100,40,BLACK);
            }
        EndDrawing();
    }
    UnloadRenderTexture(canvas);
    CloseWindow();
    return 0;
}