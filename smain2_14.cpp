#include"sclass2_14_cpoint.h"
int main()
{
    Point*p1=new Point(1,1);
    Point*p2=new Point(1,3);
    Point*p3=new Point(3,3);
    cout<<"已创建的点数："<<Point::GetCounter()<<endl;
    //静态成员函数通过作用域分辨符(::)来访问
    //语法格式:  类名::静态成员函数名(参数表)
    delete p1;
    delete p2;
    delete p3;
    return 0;
}