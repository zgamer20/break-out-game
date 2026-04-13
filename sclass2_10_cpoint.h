#ifndef _SCLASS2_10_CPOINT_H_
#define _SCLASS2_10_CPOINT_H_
#include<iostream>
using namespace std;

class CPoint
{
public:
    CPoint(){}
    ~CPoint(){}
    CPoint(float x1,float y1);
    void Move(float x1,float y1);

    float x;
    float y;
};
#endif