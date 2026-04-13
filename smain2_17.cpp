//友元函数
//计算两点距离并输出
#include<iostream>
#include<math.h>
using namespace std;
class CPoint
{
public:
    CPoint(double xx=0,double yy=0)
    {
        x=xx;
        y=yy;
    }
    double GetX()
    {
        return x;
    }
    double GetY()
    {
        return y;
    }
    friend double FDist(CPoint*p1,CPoint*p2);
    //更常用的常引用写法：
    //friend double FDist(const CPoint &p1,const CPoint &p2)
private:
    double x,y;
};
//友元函数可以是普通函数，也可以是其他类的成员函数
//虽然在类中声明，但不属于该类的成员函数
//比如
// class B; 前置声明
// class A {
// private:
//     int x;
//     // 声明 B 的成员函数为 A 的友元
//     friend void B::func(A& a); fun在A中声明，但不属于A的成员函数
//     fun是相对于A的其他类B的成员函数
// };

// class B {
// public:
//     void func(A& a) {
//         // 可以访问 A 的 private
//         a.x = 10;
//     }
// };
double FDist(CPoint*p1,CPoint*p2)
{
    double x=p1->x-p2->x;
    double y=p1->y-p2->y;
    return sqrt(x*x+y*y);
};
//double FDist(const CPoint &p1,const CPoint &p2)
//{
//  double x=p1.x-p2.x;
//  double y=p1.y-p2.y;
//  return sqrt(x*x+y*y);
//}
int main()
{
    CPoint*p1=new CPoint(1,1);
    CPoint*p2=new CPoint(4,5);
    //CPoint a(1,1);
    //CPoint b(4,5);
    cout<<"The distance is:";
    cout<<FDist(p1,p2)<<endl;
    //cout<<FDist(a,b)<<endl;
    delete p1;
    delete p2;
    return 0;
}