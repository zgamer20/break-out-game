#include"sclass3_1_A.h"

void A::SetX(int i){x=i;}
int A::GetX() const {return x;}
void B::SetValue(int a,int b,int c,int d,int e)
{
    SetX(a);
    //x=a;  非法,公有继承,基类的私有数据x对派生类不可见
    y=b;
    z=c;
    m=d;
    n=e;
}
void B::Display() const
{
    //cout<<"x="<<x<<endl; x在派生类中不可访问
    cout<<"x="<<GetX()<<endl;
    cout<<"y="<<y<<endl;
    cout<<"m="<<m<<endl;
    cout<<"n="<<n<<endl;//派生类的私有成员在本类中可以访问
}