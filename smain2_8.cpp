#include"sclass2_8_cpoint.h"
int main()
{
    CPoint a(20,50);//用构造函数创建了一个点a(20,50)
    CPoint b(a);
    CPoint c=a;//这两行用拷贝构造函数以a克隆出点b,c
    cout<<"a("<<a.GetX()<<","<<a.GetY()<<")"<<endl;
    return 0;
}