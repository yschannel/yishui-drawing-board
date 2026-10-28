#include<stdio.h>
int main(){
    int choice;
    printf("欢迎您进入Yishui画板，请根据菜单选择您想进行的操作：\n");
    printf("1.读取文件\n");
    printf("2.不读取文件\n");
    printf("请输入你的选择：");
    scanf("%d",&choice);
    if(choice==1){
        printf("已读取");
        getchar();
        int second_choice;
        printf("欢迎您进入Yishui画板,请根据菜单选择选择您想进行的操作:\n");
        printf("1.清除屏幕\n");
        printf("2.作画\n");
        printf("3.退出\n");
        printf("请输入您的选择：");
        scanf("%d",&second_choice);
        switch(second_choice){
            case 1:
            break;
            case 2:
            break;
            case 3:
            break;
            default:
            printf(" ");
        }
    }
    if(choice==2)printf("未读取");
    return 0;
}