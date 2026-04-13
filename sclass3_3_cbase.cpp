#include"sclass3_3_cbase.h"
void CBase::SetValue(int a,int b)
{
    n=a;
    m=b;
    cout<<"基类成员n="<<n<<endl;
    cout<<"基类成员m="<<m<<endl;
}
void CBase::CalcuValue()
{
    cout<<"n*m="<<m*n<<endl;
}
void CDerived::SetJ(){j=2*m;}
//派生类成员可以访问基类的 保护 数据m
void CDerived::SetK()
{
    k=0;
    //k=n非法！派生类成员不能访问基类的 私有 数据n
}
void CDerived::DisplayVal()const
{
    cout<<"派生类成员j="<<j<<endl;
    cout<<"派生类成员k="<<k<<endl;
}