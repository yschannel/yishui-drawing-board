#include "raylib.h"


int main(void){
    const int ScreenWidth = 1200;
    const int Screenheigh = 800;

    InitWindow(ScreenWidth, Screenheigh, "Aphex Twin");
    SetTargetFPS(120);
    while(!(WindowShouldClose())){
        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircleV(GetMousePosition(), 30, RED);
        EndDrawing();
    }


}