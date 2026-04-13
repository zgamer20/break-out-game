#include<iostream>
using namespace std;
class A{
private:
    const int a;
    static const int b;//静态常成员
public:
    A(int i):a(i){}//:数据成员名() => 初始化列表
    void Print() const {cout<<a<<":"<<b<<endl;}
};
const int A::b=10;//静态成员类外赋值
int main()
{
    A a1(100),a2(0);
    a1.Print();
    a2.Print();
}