#include<iostream>
using namespace std;
class A
{
public:
    A(){cout<<"调用类A的构造函数"<<endl;}
    ~A(){cout<<"调用类A的析构函数"<<endl;}
};
class B:public A
{
public:
    B(){cout<<"调用类B的构造函数"<<endl;}
    ~B(){cout<<"调用类B的析构函数"<<endl;}
//  在继承过程中，派生类不能继承基类的析构函数
//  所以派生类需要自己定义析构函数
};

int main()
{
    B b;
    // 析构派生类对象的步骤顺序是：
    // 先调用派生类的析构函数析构  派生类对象新增部分
    // 如果该派生类含有子对象，接下来就析构  子对象部分
    // 最后调用基类的析构函数析构  基类部分
    // 派生类的析构顺序恰好与构造顺序相反
    return 0;
}