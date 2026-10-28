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
//record the page status
typedef struct{
    program_page current_page;
    program_page previous_page;
    bool should_quit;
}program_state;

int main(){
    //setting
    const int screenWidth =1600;
    const int screenHeight=900;
    InitWindow(screenWidth,screenHeight,"Yishui drawing board!");
    SetTargetFPS(60);
    //initialization
    program_state state={
        .current_page = page_file,
        .previous_page = page_file,
        .should_quit = false
    };
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
                if(IsKeyPressed(KEY_Q)){
                    state.previous_page=state.current_page;
                    state.current_page=page_quit;
                }
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
                    ClearBackground(BEIGE);
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
    CloseWindow();
    return 0;
}