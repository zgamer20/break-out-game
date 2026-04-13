#include"sclass2_10_cpoint.h"
int main()
{
    CPoint*p=new CPoint[5];//声明动态对象数组
    int i;
    for(i=0;i<5;i++)
    {
        p[i].Move(i,i);//对象调用成员函数：对象用. 指针用->
    }
    for(i=0;i<5;i++)
    {
        cout<<p[i].x<<","<<p[i].y<<endl;
    }
    delete []p;//释放数组
    return 0;
}