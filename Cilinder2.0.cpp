#include<iostream>
using namespace std;
const double PI=3.1415;

class Circle
{
public:
    void SetRadius(double r){radius=r;}
    double GetRadius() const {return radius;}
    double GetArea()const {return PI*radius*radius;}
    void Print()const
    {
        cout<<"半径为"<<radius<<
        "的圆的面积是"<<GetArea()<<endl;
    }
    //基类 Circle 的公有成员在派生类 Cylinder 里变成私有成员
private:
    double radius;
};

class Cylinder:private Circle//私有继承
{
public:
    void SetHeight(double h){height=h;}
    double GetHeight() const {return height;}
    void SetRadius(double r)
    //派生类定义与基类同名的公有成员函数，便于类外访问
    {
        Circle::SetRadius(r);
        //用作用域限定符表明调用基类同名函数
    }
    double GetArea() const
    {
        return 2*PI*GetRadius()*GetRadius()+2*PI*GetRadius()*height;
        //基类的公有成员GetRadius()在派生类中可见
        // 调用它可以间接访问在派生类中不可见的radius	
    }
    double GetVolume() const
    {
        return PI*GetRadius()*GetRadius()*height;
    }
    void Print() const
    {
        cout<<"底半径为"<<GetRadius()
        <<"的圆柱体的表面积是"<<GetArea()
        <<"体积是"<<GetVolume()<<endl;
    }
private:
    double height;
};
int main()
{
    Cylinder cylinder;
    cylinder.SetRadius(12.5);
    cylinder.SetHeight(10.5);
    cout<<"圆柱体的表面积是"<<cylinder.GetArea()<<endl;
    cout<<"圆柱体的体积是"<<cylinder.GetVolume()<<endl;

    return 0;
}