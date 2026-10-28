#include "raylib.h"
#include<math.h>
typedef struct {
    float x;
    float y;
} Vector_2;
float Vector2Distance(Vector_2 startPos,Vector_2 endPos){
    float dx = endPos.x-startPos.x;
    float dy = endPos.y-startPos.y;
    return sqrtf(dx*dx+dy*dy);
}
// 定义可能的页面
typedef enum {
    page_welcome,
    page_drawing,
    page_file,
    page_file_read,
    page_file_not_read,
    page_quit
} program_page;

// 绘图模式枚举
typedef enum {
    MODE_PENCIL = 0,
    MODE_LINE,
    MODE_RECTANGLE,
    MODE_CIRCLE,
    MODE_ERASER
} DrawMode;

// 记录程序状态
typedef struct {
    program_page current_page;
    program_page previous_page;
    bool should_quit;
} program_state;

int main(void) {
    // 设置
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Yishui drawing board!");
    SetTargetFPS(60);
    
    // 初始化程序状态
    program_state state = {
        .current_page = page_file,
        .previous_page = page_file,
        .should_quit = false
    };
    
    // 画板相关变量
    RenderTexture2D canvas = LoadRenderTexture(screenWidth, screenHeight);
    Color currentColor = BLACK;
    Color colors[] = {BLACK, RED, GREEN, BLUE, YELLOW, PURPLE, ORANGE};
    int colorCount = sizeof(colors) / sizeof(colors[0]);
    int selectedColorIndex = 0;
    DrawMode currentMode = MODE_PENCIL;
    Vector_2 startPos = {0};
    Vector_2 endPos = {0};
    bool drawing = false;
    float brushSize = 5.0f;
    
    // 初始化画布为白色
    BeginTextureMode(canvas);
    ClearBackground(WHITE);
    EndTextureMode();
    
    // 主程序循环
    while (!state.should_quit && !WindowShouldClose()) {
        // 页面切换逻辑
        switch (state.current_page) {
            case page_file:
                if (IsKeyPressed(KEY_A)) {
                    state.current_page = page_file_read;
                } else if (IsKeyPressed(KEY_B)) {
                    state.current_page = page_file_not_read;
                } else if (IsKeyPressed(KEY_Q)) {
                    state.previous_page = state.current_page;
                    state.current_page = page_quit;
                }
                break;

            case page_file_read:
                if (IsKeyPressed(KEY_Y)) {
                    state.current_page = page_welcome;
                } else if (IsKeyPressed(KEY_N)) {
                    state.current_page = page_file;
                } else if (IsKeyPressed(KEY_Q)) {
                    state.previous_page = state.current_page;
                    state.current_page = page_quit;
                }
                break;

            case page_file_not_read:
                if (IsKeyPressed(KEY_Y)) {
                    state.current_page = page_file;
                } else if (IsKeyPressed(KEY_Q)) {
                    state.previous_page = state.current_page;
                    state.current_page = page_quit;
                }
                break;

            case page_welcome:
                if (IsKeyPressed(KEY_A)) {
                    state.current_page = page_drawing;
                } else if (IsKeyPressed(KEY_Q)) {
                    state.previous_page = state.current_page;
                    state.current_page = page_quit;
                }
                break;

            case page_drawing:
                // 画板功能处理
                // 处理颜色选择
                if (IsKeyPressed(KEY_ONE)) selectedColorIndex = 0;
                if (IsKeyPressed(KEY_TWO)) selectedColorIndex = 1;
                if (IsKeyPressed(KEY_THREE)) selectedColorIndex = 2;
                if (IsKeyPressed(KEY_FOUR)) selectedColorIndex = 3;
                if (IsKeyPressed(KEY_FIVE)) selectedColorIndex = 4;
                if (IsKeyPressed(KEY_SIX)) selectedColorIndex = 5;
                if (IsKeyPressed(KEY_SEVEN)) selectedColorIndex = 6;
                
                // 处理绘图模式切换
                if (IsKeyPressed(KEY_P)) currentMode = MODE_PENCIL;
                if (IsKeyPressed(KEY_L)) currentMode = MODE_LINE;
                if (IsKeyPressed(KEY_R)) currentMode = MODE_RECTANGLE;
                if (IsKeyPressed(KEY_C)) currentMode = MODE_CIRCLE;
                if (IsKeyPressed(KEY_E)) currentMode = MODE_ERASER;
                
                // 处理画笔粗细调整
                if (IsKeyPressed(KEY_UP)) brushSize += 1.0f;
                if (IsKeyPressed(KEY_DOWN)) brushSize = (brushSize > 1.0f) ? brushSize - 1.0f : 1.0f;
                
                // 清空画布
                if (IsKeyPressed(KEY_SPACE)) {
                    BeginTextureMode(canvas);
                    ClearBackground(WHITE);
                    EndTextureMode();
                }
                
                // 退出画板页面
                if (IsKeyPressed(KEY_Q)) {
                    state.previous_page = state.current_page;
                    state.current_page = page_quit;
                }
                
                // 更新当前颜色
                currentColor = colors[selectedColorIndex];
                
                // 开始绘制到渲染纹理
                BeginTextureMode(canvas);
                
                // 获取鼠标位置
                Vector_2 mousePos = GetMousePosition();
                
                // 处理鼠标输入
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    drawing = true;
                    startPos = mousePos;
                    endPos = mousePos;
                }
                
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && drawing) {
                    endPos = mousePos;
                    
                    // 根据当前模式绘制
                    switch (currentMode) {
                        case MODE_PENCIL:
                            DrawCircleV(mousePos, brushSize, currentColor);
                            break;
                            
                        case MODE_ERASER:
                            DrawCircleV(mousePos, brushSize, WHITE);
                            break;
                            
                        default:
                            // 对于形状，我们在鼠标释放时绘制
                            break;
                    }
                }
                
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && drawing) {
                    drawing = false;
                    
                    // 绘制最终形状
                    switch (currentMode) {
                        case MODE_LINE:
                            DrawLineEx(startPos, endPos, brushSize, currentColor);
                            break;
                            
                        case MODE_RECTANGLE: {
                            Rectangle rect = {
                                fminf(startPos.x, endPos.x),
                                fminf(startPos.y, endPos.y),
                                fabsf(endPos.x - startPos.x),
                                fabsf(endPos.y - startPos.y)
                            };
                            DrawRectangleRec(rect, currentColor);
                            break;
                        }
                            
                        case MODE_CIRCLE: {
                            float radius = Vector2Distance(startPos, endPos);
                            DrawCircleV(startPos, radius, currentColor);
                            break;
                        }
                            
                        default:
                            break;
                    }
                }
                
                EndTextureMode();
                break;

            case page_quit:
                if (IsKeyPressed(KEY_Y)) {
                    state.should_quit = true;
                } else if (IsKeyPressed(KEY_N)) {
                    state.current_page = state.previous_page;
                }
                break;
            
            default:
                if (IsKeyPressed(KEY_Q)) {
                    state.previous_page = state.current_page;
                    state.current_page = page_quit;
                }
        }

        // 开始绘制
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        switch (state.current_page) {
            case page_file:
                ClearBackground(BEIGE);
                DrawText("Welcome to Yishui drawing board\nWhat would you like to do?\nA.Read the file\nB.Do not read the file\nQ.quit", 
                         200, 100, 40, BLACK);
                break;
                
            case page_file_read:
                ClearBackground(BEIGE);
                DrawText("Read successfully!\nDo you want to open it?\nY.Yes\nN.No\nQ.quit", 
                         200, 100, 40, BLACK);
                break;

            case page_file_not_read:
                ClearBackground(BEIGE);
                DrawText("Not read!\nDo you want to back the file page?\nY.Yes\nQ.quit", 
                         200, 100, 40, BLACK);
                break;

            case page_welcome:
                ClearBackground(BEIGE);
                DrawText("Welcome to Yishui drawing board\nWhat would you like to do?\nA.Drawing\nQ.quit", 
                         200, 100, 40, BLACK);
                break;

            case page_drawing:
                // 绘制画布
                DrawTextureRec(canvas.texture, 
                              (Rectangle){0, 0, (float)canvas.texture.width, (float)-canvas.texture.height}, 
                              (Vector_2){0, 0}, WHITE);
                
                // 绘制UI
                DrawRectangle(0, 0, screenWidth, 40, (Color){200, 200, 200, 200});
                
                // 显示当前颜色
                DrawText("当前颜色:", 10, 10, 20, DARKGRAY);
                DrawRectangle(100, 10, 20, 20, currentColor);
                
                // 显示当前模式
                const char* modeText;
                switch (currentMode) {
                    case MODE_PENCIL: modeText = "铅笔"; break;
                    case MODE_LINE: modeText = "直线"; break;
                    case MODE_RECTANGLE: modeText = "矩形"; break;
                    case MODE_CIRCLE: modeText = "圆形"; break;
                    case MODE_ERASER: modeText = "橡皮"; break;
                    default: modeText = "未知"; break;
                }
                DrawText(TextFormat("当前模式: %s", modeText), 150, 10, 20, DARKGRAY);
                
                // 显示画笔粗细
                DrawText(TextFormat("画笔粗细: %.0f", brushSize), 300, 10, 20, DARKGRAY);
                
                // 显示帮助信息
                DrawText("快捷键: 1-7颜色 P铅笔 L直线 R矩形 C圆形 E橡皮 上下键调整粗细 空格清空 Q退出", 
                         10, screenHeight - 30, 20, DARKGRAY);
                
                // 如果正在绘制形状，显示预览
                if (drawing && (currentMode == MODE_LINE || currentMode == MODE_RECTANGLE || currentMode == MODE_CIRCLE)) {
                    Vector_2 mousePos = GetMousePosition();
                    switch (currentMode) {
                        case MODE_LINE:
                            DrawLineEx(startPos, mousePos, brushSize, currentColor);
                            break;
                            
                        case MODE_RECTANGLE: {
                            Rectangle rect = {
                                fminf(startPos.x, mousePos.x),
                                fminf(startPos.y, mousePos.y),
                                fabsf(mousePos.x - startPos.x),
                                fabsf(mousePos.y - startPos.y)
                            };
                            DrawRectangleLinesEx(rect, brushSize, currentColor);
                            break;
                        }
                            
                        case MODE_CIRCLE: {
                            float radius = Vector2Distance(startPos, mousePos);
                            DrawCircleLines((int)startPos.x, (int)startPos.y, radius, currentColor);
                            break;
                        }
                            
                        default:
                            break;
                    }
                }
                break;

            case page_quit:
                ClearBackground(BEIGE);
                DrawText("Do you want to quit the program?\nY.yes\nN.No", 
                         200, 100, 40, BLACK);
                break;

            default:
                ClearBackground(BEIGE);
                DrawText("404", 200, 100, 40, BLACK);
        }
        
        EndDrawing();
    }
    
    // 清理资源
    UnloadRenderTexture(canvas);
    CloseWindow();
    
    return 0;
}