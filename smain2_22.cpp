#include<iostream>
using namespace std;
class Point
{
public:
    Point (int a,int b){x=a;y=b;}
    void Move(int a,int b){x+=a;y+=b;}
    void Print() const {cout<<"x="<<x<<" "<<"y="<<y<<endl;}
private:
    int x,y;
};

int main()
{
    const Point point1(10,10);
    //point1.Move(2,2);
	//常对象point1的数据不能被更改，所以错误
    point1.Print( );
    // 常对象不能调用非常成员函数print()
    //但是如果写成
    //void Print() const { ... } 常成员函数
    // 编译器：承诺不修改，可以调用
    //常成员函数不能修改数据成员的值
    //常对象只能调用类的常成员函数及类的静态成员函数 
    return 0;
}