//析构函数在对象消亡时做一些扫尾工作
//格式：~类名();
//析构函数的函数名是类名前面多加了一个~,无参数和返回值

//构造函数：为创造对象分配空间，初始化对象成员
//格式：类名(参数表);
//函数名与类名相同，不具有返回值

#ifndef _SCLASS2_9_DATE_H_
#define _SCLASS2_9_DATE_H_
#include<iostream>
using namespace std;
class Date
{
public:
    Date();//无参数的构造函数
    Date(int y,int m,int d);//三个参数的构造函数
    ~Date();//析构函数
    void SetDate(int y,int m,int d);
    bool IsLeapYear();
    void Print();
private:
    int year,month,day;
};
#endif