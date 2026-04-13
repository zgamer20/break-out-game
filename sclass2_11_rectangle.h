#ifndef _SCLASS2_11_RECTANGLE_H_
#define _SCLASS2_11_RECTANGLE_H_
#include<iostream>
using namespace std;

class Rectangle
{
public:
    Rectangle();
    Rectangle(double,double);
    ~Rectangle(){};
    void SetSize(double,double);
    double GetLength();
    double GetWidth();
    double GetArea();
    void Print();
private:
    double length;
    double width;
};
#endif