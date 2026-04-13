#ifndef _SCLASS2_5_DATE_H_
#define _SCLASS2_5_DATE_H_
#include<iostream>
using namespace std;

class Date
{
public:
    Date();
    Date(int y,int m,int d);
    void SetDate(int y,int m,int d);
    bool IsLeapYear();
    void Print();
private:
    int year,month,day;
};
#endif