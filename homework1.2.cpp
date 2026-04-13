#include<iostream>
using namespace std;
int main()
{
    printf("你考了多少分？(0~100)\n");
    int grade;
    scanf("%d",&grade);

    if(grade>=90&&grade<=100) printf("优\n");
    else if(grade>=80&&grade<90) printf("良\n");
    else if(grade>=60&&grade<80) printf("中\n");
    else if(grade>=0&&grade<60) printf("差\n");

    return 0;
}