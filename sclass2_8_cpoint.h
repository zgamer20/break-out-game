#ifndef _SCLASS2_8_CPOINT_H_
#define _SCLASS2_8_CPOINT_H_
#include<iostream>
using namespace std;

class CPoint
{
public:
    CPoint(int x1,int y1);//声明构造函数
    CPoint(const CPoint&obj);//声明拷贝构造函数：类名(const 类名&对象名);
    int GetX();
    int GetY();
private:
    int x;
    int y;
};
#endif