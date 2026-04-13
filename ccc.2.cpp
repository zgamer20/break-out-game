#include<iostream>
using namespace std;

class Base1
{
protected:
    int a;
public:
    Base1(int xx){a=xx;cout<<"构造Base1(a=xx),"<<"a="<<a<<endl;}
    ~Base1(){cout<<"析构Base1"<<endl;}
};

class Base2
{
protected:
    int b;
public:
    Base2(int yy){b=yy;cout<<"构造Base2(b=yy),"<<"b="<<b<<endl;}
    ~Base2(){cout<<"析构Base2"<<endl;}
};

class Derived : public Base1 , public Base2
{
private:
    int c;
public:
    Derived(int xx,int yy,int zz):Base1(xx),Base2(yy){c=zz;cout<<"构造Derived(c=zz),"<<"c="<<c<<endl;}
    ~Derived(){cout<<"析构Derived"<<endl;}
};

int main()
{
    Derived derived(10,20,30);
    
    return 0;
}