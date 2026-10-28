#include"raylib.h"
#include<math.h>
#include"globals.h"
#include"PAGE.h"
//initialization
program_state state={
    .current_page = page_file,
    .previous_page = page_file,
    .should_quit = false
};

const int screen_width =1600;
const int screen_height=900;
//canvas variables
RenderTexture2D canvas;
Color current_color = BLACK;
Color color_library[]={BLACK,RED,ORANGE,YELLOW,GREEN,DARKBLUE,BLUE,PURPLE};
int color_count = sizeof(color_library)/sizeof(color_library[0]);
int selected_color_index=0;
drawing_mode current_drawing_mode = mode_pencil;
Vector2 start_pos={0,0};
Vector2 end_pos={0,0};
bool drawing = false;
float brush_size=1.0f;

//Define a function that can calculate the distance between two points
float vector2_distance(Vector2 start_Pos,Vector2 end_Pos){
    float dx = end_Pos.x - start_Pos.x;
    float dy = end_Pos.y - start_Pos.y;
    return sqrtf(pow(dx,2)+pow(dy,2));
}

int main(void){
    //setting

    InitWindow(screen_width,screen_height,"Yishui drawing board!");
    SetTargetFPS(60);

    //initialize canvas
    canvas=LoadRenderTexture(screen_width,screen_height);
    BeginTextureMode(canvas);
    ClearBackground(WHITE);
    EndTextureMode();

    //main program
    while(!state.should_quit&&!WindowShouldClose()){
        switch(state.current_page){
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

            case page_quit:
                PAGE_QUIT();
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
                            case mode_rectangle:{
                                Rectangle rect ={
                                    fminf(start_pos.x,end_pos.x),
                                    fminf(start_pos.y,end_pos.y),
                                    fabsf(start_pos.x-end_pos.x),
                                    fabsf(start_pos.y-end_pos.y)
                                };
                                DrawRectangleLinesEx(rect,brush_size,current_color);
                                break;
                            }
                            case mode_circle:{
                                float radius =vector2_distance(start_pos,end_pos);
                                DrawCircleLines((int)start_pos.x,(int)start_pos.y,radius,current_color);
                                break;
                            }
                            default:{
                                break;
                            }
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