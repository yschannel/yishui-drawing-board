#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    // 初始化窗口
    InitWindow(800, 600, "自定义多边形绘制");
    SetTargetFPS(60);
    
    int sides = 0;
    Vector2 *points = NULL;
    Color polyColor = BLUE;
    bool isFilled = false;
    bool inputComplete = false;
    
    // 控制台输入部分
    printf("=== 自定义多边形绘制 ===\n");
    
    // 获取边数
    printf("请输入多边形的边数: ");
    scanf("%d", &sides);
    
    if (sides < 3) {
        printf("错误：边数必须至少为3！\n");
        return -1;
    }
    
    // 分配内存存储顶点
    points = (Vector2 *)malloc(sides * sizeof(Vector2));
    if (points == NULL) {
        printf("内存分配失败！\n");
        return -1;
    }
    
    // 获取每个顶点的坐标
    printf("\n请输入每个顶点的坐标 (x y):\n");
    for (int i = 0; i < sides; i++) {
        printf("顶点 %d: ", i + 1);
        scanf("%f %f", &points[i].x, &points[i].y);
    }
    
    // 询问是否填充
    char fillChoice;
    printf("\n是否填充多边形？(y/n): ");
    scanf(" %c", &fillChoice);
    isFilled = (fillChoice == 'y' || fillChoice == 'Y');
    
    inputComplete = true;
    printf("\n输入完成！按ESC退出窗口。\n");
    
    // 主循环
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        if (inputComplete && points != NULL) {
            if (isFilled) {
                // 绘制填充多边形
                DrawTriangleFan(points, sides, Fade(polyColor, 0.7f));
                
                // 绘制轮廓以便看清边界
                DrawLineLoop(points, sides, DARKBLUE);
            } else {
                // 只绘制轮廓
                DrawLineLoop(points, sides, polyColor);
            }
            
            // 绘制顶点标记
            for (int i = 0; i < sides; i++) {
                DrawCircleV(points[i], 5, RED);
                DrawText(TextFormat("%d", i+1), points[i].x + 8, points[i].y - 8, 12, BLACK);
            }
            
            // 显示信息
            DrawText(TextFormat("多边形边数: %d", sides), 10, 10, 20, DARKGRAY);
            DrawText(TextFormat("填充: %s", isFilled ? "是" : "否"), 10, 40, 20, DARKGRAY);
            DrawText("红色圆圈标记顶点编号", 10, 70, 20, DARKGRAY);
        }
        
        EndDrawing();
    }
    
    // 清理内存
    if (points != NULL) {
        free(points);
    }
    
    CloseWindow();
    return 0;
}