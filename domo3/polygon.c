#include<stdio.h>
#include"raylib.h"

typedef struct{
    int side;
    Color color;
    Vector2 *points;
    _Bool is_filled;
    float line_width;

}Shape;

int main(){
    Shape shape1={
        .side = 5,
        .color = BLUE,
        .points = NULL,
        .is_filled =false,
        .line_width = 5.0f
    }

    //绘制
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if(shape1.is_filled){
        DrawTriangleFan(shape1.points,shape1.side,Fade(shape1.color,shape1.line_width));

        DrawLineLoop(shape1.points,shape1.side,DARKBLUE);
    }else{
        DrawLineLoop(shape1.points,shape1.side,shape1.color);
    }

    //点标记
    for (int i = 0;i < shape1.side;i++){
        DrawCircleV(shape1.points[i],5,RED);
    }

    EndDrawing();

    //清理内存
    if(shape1.points != NULL){
        free(shape1.points);
    }
    
}