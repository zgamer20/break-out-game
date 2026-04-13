//常成员函数函数重载
#include<iostream>
using namespace std;

class A
{
private:
    int w,h;
public:
    int GetValue() const {return w*h;}
    int GetValue() {return w+h;}
    A(int x,int y) {w=x,h=y;}
    A(){};
};
int main()
{
    A const a(3,4);
    A c(2,6);
    cout<<a.GetValue()<<endl<<c.GetValue()<<endl;
    return 0;
}