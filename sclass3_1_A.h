#ifndef _SCLASS_3_1_A_H_
#define _SCLASS_3_1_A_H_
#include<iostream>
using namespace std;

class A
{
private:
    int x;
protected:
    int y;
public:
    void SetX(int i);//为成员x赋值
    int GetX() const;//获取x的值
    int z;//基类的公有数据成员
};

class B:public A//派生类B公有继承基类A
{
protected:
    int m;
private:
    int n;
public:
    void SetValue(int,int,int,int,int);
    //为基类和本类所有数据成员赋值
    void Display() const;
};
#endif