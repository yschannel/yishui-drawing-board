#include "raylib.h"

int main() {
  // set up
        //draw
    struct darw{
      char draw_board_type;
      int parameter[10];
      Color draw_board_color;
    };
    struct darw draw_board[10000];
    int draw_board_index = 1;
        //font
    Color font_color = BLACK;
    int font_size = 40;
    int Font_Max =10;//How many font cloud exits
    Font font[Font_Max];
    font[0]=GetFontDefault();
        //draw mode
    int draw_mode;
    int draw_size=50;
    Color draw_color=BLACK;
        //mode
    int quiz =0;
    int lastmode;
    int mode =0;//0 represent""Welcome Mode"
        //window
    const int window_width = 1400, window_height = 1280;
    InitWindow(window_width,window_height,"Welcome to VimPainter!");
    SetTargetFPS(60);
        //mouse
    _Bool last_mousedown;
    Vector2 draw_start_mouse =GetMousePosition();

        //welcome boat
    int wel_boat_x=-300;

    while(quiz==0){
        if(WindowShouldClose()){
            lastmode=mode;
            mode = 4;
        }//detect windowshouldclose

        switch (mode){
            //mode 0"welcome"
            case 0:
                if(IsKeyDown(KEY_A)){
                    mode = 1;
                }else if(IsKeyDown(KEY_S)){
                    mode = 2;
                }else if(IsKeyDown(KEY_D)){
                    mode =3;
                }else if(IsKeyDown(KEY_F)){
                    lastmode = mode;
                    mode = 4;
                }
                lastmode = 0;
                //move the boat
                wel_boat_x += GetFrameTime()*100.0;
                if(wel_boat_x >=window_width)wel_boat_x=-300;
                break;
            //mode drawing
            case 1:
            if(IsKeyDown(KEY_C)){
                draw_mode=1;//mode1:circle
            }if(IsKeyDown(KEY_L)){
                draw_mode=2;
            }
            if(IsKeyReleased(KEY_U)){
              if(draw_board_index!=1){
                draw_board_index--;
              }
            }
            break;
            //mode quit
            case 4:
                if(IsKeyDown(KEY_Y)){
                    quiz =1;
                }else if (IsKeyDown(KEY_N)){
                    quiz = 0;
                    mode = lastmode;
                }
                break;
            default:
                if(IsKeyDown(KEY_Q)){
                    mode = lastmode;
                }
            }
        //darw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            switch(mode){
                //case welcome
                case 0:
                    ClearBackground(BLUE);
                    DrawText("What would you like to do?\nA  Create a new file.\nS  Open a file\nD  Setting\nF  :wq",200,200,font_size,font_color);
                    DrawText("              |\\\n"
                                                "              |  \\\n"
                                                "              |    \\\n"
                                                "              |    /\n"
                                                "              |  /\n"
                                                "              |/\n"
                                                "_________|____________\n"
                                                "\\@//////////////\n"
                                                "  \\________________/\n ",wel_boat_x,400,font_size, font_color);
                    break;
                //case welcome
                case 1:
                //load from data
                for(int i = 0;i <= draw_board_index-1;i++){
                  switch(draw_board[i].draw_board_type){
                    //line
                      case 'l' :
                          DrawLine(draw_board[i].parameter[0], draw_board[i].parameter[1], draw_board[i].parameter[2], draw_board[i].parameter[3],draw_board[i].draw_board_color );
                          break;
                    //circle
                      case 'c':
                          DrawCircle(draw_board[i].parameter[0], draw_board[i].parameter[1],draw_board[i].parameter[2],draw_board[i].draw_board_color);
                          break;
                  }
                }
                //draw mouse
                    switch(draw_mode){
                        case 1:
                        //circle mode
                            DrawCircle(GetMousePosition().x, GetMousePosition().y,draw_size,draw_color);
                            DrawText("Circle",10,1250,20,font_color);
                            if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                              draw_board[draw_board_index].draw_board_type = 'c';
                              draw_board[draw_board_index].parameter[0]=GetMousePosition().x;
                              draw_board[draw_board_index].parameter[1]=GetMousePosition().y;
                              draw_board[draw_board_index].parameter[2]=draw_size;
                              draw_board[draw_board_index].draw_board_color=draw_color;
                              draw_board_index++;
                            }
                            break;
                        //line mode
                        case 2:
                            DrawText("Line",10,1250,20,font_color);
                           if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
                            if(last_mousedown==false){
                                draw_start_mouse=GetMousePosition();
                            }else{
                                DrawLine(draw_start_mouse.x,draw_start_mouse.y,GetMousePosition().x,GetMousePosition().y,draw_color);
                            
                            }
                           }
                           if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                              draw_board[draw_board_index].draw_board_type = 'l';
                              draw_board[draw_board_index].parameter[0]=draw_start_mouse.x;
                              draw_board[draw_board_index].parameter[1]=draw_start_mouse.y;
                              draw_board[draw_board_index].parameter[2]=GetMousePosition().x;
                              draw_board[draw_board_index].parameter[3]=GetMousePosition().y;
                              draw_board[draw_board_index].draw_board_color=draw_color;
                              draw_board_index++;
                           }
                           break; 
                    }
                break;
                //case quit
                case 4:
                    ClearBackground(RAYWHITE);
                    DrawText("Would you want to quit?[Y/N]",200,200,font_size,font_color);
                    break;
                default:
                    ClearBackground(RAYWHITE);
                    DrawText("Welcome to 404 Page!",200,200, font_size, font_color);
    
            }
        last_mousedown =IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        EndDrawing();


    }
    
    CloseWindow();

}
