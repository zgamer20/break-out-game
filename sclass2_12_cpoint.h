//在Point类中显示地表达this指针
#ifndef _SCLASS2_12_POINT_H_
#define _SCLASS2_12_POINT_H_
#include<iostream>
using namespace std;

class CPoint
{
public:
    CPoint(){}
    ~CPoint(){}
    CPoint(float,float);
    void Move(float,float);

    float x;
    float y;
};
#endif