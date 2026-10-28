#include"raylib.h"
int main(){
    const int screenWidth=800;
    const int screenHeight=450;
    InitWindow(screenWidth,screenHeight,"raylib [core] example-input mouse");

    Vector2 ballPosition={-100.0f,-100.0f};
    Color ballColor=DARKBLUE;

    SetTargetFPS(60);

    while (!WindowShouldClose()){
        if(IsKeyPressed(KEY_H)){
            if(IsCursorHidden()){
                ShowCursor();
            }else{
                HideCursor();
            }
        }
        ballPosition=GetMousePosition();
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))ballColor=MAROON;
        else if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))ballColor=LIME;
        else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))ballColor=DARKBLUE;
        else if(IsMouseButtonPressed(MOUSE_BUTTON_SIDE))ballColor=PURPLE;
        else if(IsMouseButtonPressed(MOUSE_BUTTON_EXTRA))ballColor=YELLOW;
        else if(IsMouseButtonPressed(MOUSE_BUTTON_FORWARD))ballColor=ORANGE;
        else if(IsMouseButtonPressed(MOUSE_BUTTON_BACK))ballColor=BEIGE;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(ballPosition,40,ballColor);
        DrawText("move ball with mouse and click mouse button to change color",10,10,20,DARKGRAY);
        DrawText("Press 'H' to toggle cursor visibility",10,30,20,DARKGRAY);

        if(IsCursorHidden())DrawText("CURSOR HIDDEN",20,60,20,RED);
        else DrawText("CURSOR VISIBLE",20,60,20,LIME);
        EndDrawing();
        }
        CloseWindow();
        return 0;
    
}