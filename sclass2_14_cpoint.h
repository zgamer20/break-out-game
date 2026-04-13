//静态数据成员为该类的所有对象共享
#ifndef _SCLASS_CPOINT_H_
#define _SCLASS_CPOINT_H_
#include<iostream>
using namespace std;
class Point
{
public:
    Point()
    {
        x=0;
        y=0;
        counter++;//创建对象,统计当前存在的点数
    }
    ~Point()
    {
        counter--;
    }
    Point(float x,float y)
    {
        this->x=x;
        this->y=y;
        counter++;//创建对象
    }
    void Move(float x,float y)
    {
        this->x=x;
        this->y=y;
    }
    float GetX()
    {
        return x;
    }
    float GetY()
    {
        return y;
    }
    //静态成员函数实现对静态数据成员的存取
    static int GetCounter()
    {
        return counter;
        //不能写 return counter+x+y;
        //因为静态成员函数不能直接访问普通数据成员
        //this指针为普通数据成员，静态成员函数也不能访问this指针
    }

private:
    float x;
    float y;
public:
    static int counter;//声明静态数据成员：static 数据类型 静态数据成员名
};
int Point::counter=0;//初始化：数据类型 类名::静态数据成员名=初始值
#endif
//静态成员不属于任何对象，属于整个类
// 程序一运行就存在，不管有没有创建对象
//所以不能放在构造函数里初始化,要在类外初始化