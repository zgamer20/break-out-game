#ifndef _SCLASS2_3_DATE_H_
#define _SCLASS2_3_DATE_H_

#include<iostream>
using namespace std;

class Date
{
public:
    Date();//构造函数
    void SetDate(int y,int m,int d);
    bool IsLeapYear();
    void Print();

private:
    int year,month,day;
};
#endif