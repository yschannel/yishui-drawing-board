#include"raylib.h"
#include<stdio.h>

typedef struct{
    char content[251];
    int char_count;
    _Bool is_input_over;
}Get_key;

int main(){
    Get_key domo1={
        .content = "\0",
        .char_count = 0,
        .is_input_over = false
    };

    while(!domo1.is_input_over){
        int character = GetCharPressed();
        //process character input
        if(character > 0 && domo1.char_count < 250){
            domo1.content[domo1.char_count] = (char) c;
            domo1.content[domo1.char_count+1] = '\0';
            domo1.char_count++;
        }

        if(IsKeyPressed(KEY_BACKSPACE)){
            domo1.char_count--;
            if(domo1.char_count < 0) domo1.char_count = 0;
            domo1.content[domo1.char_count] = '\0';
        }

        if(IsKeyPressed(KEY_ENTER)){
            domo1.is_input_over = true;
        }
    }

    BeginDrawing();
        ClearBackground(RAYWHITE);

        Drawtext();
        DrawRectangle();
        Drawtext();
}