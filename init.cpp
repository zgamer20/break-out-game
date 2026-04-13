#include<iostream>
using namespace std;
class A
{
private:
    int a;
public:
    A(int x):a(x)
    {
        cout << "构造 A:a = " << a << endl;
    }
    int getA() const
    {
        return a;
    }
};

class S
{
private:
    int s;
public :
    S(int y):s(y)
    {
        cout << "构造 S:s = " << s << endl;
    }
    int getS() const 
    {
        return s;
    }
};

class B:public A
{
private:
    S objS;
    int b;
public:
    B(int x,int y,int z):A(x),objS(y),b(z)

    // 如果基类定义了无参数的构造函数
    // 或由编译器自动生成缺省构造函数
    // (缺省构造函数 = 默认构造函数 = 不带任何参数的构造函数)
    // 或构造函数的所有参数都有默认值时
    // 在派生类构造函数的定义中可以省略对基类构造函数的调用
    // 即初始化列表省略“基类名（参数表）”
    // 如果此派生类也没有子对象需要初始化
    // 那么就可以完全省略派生类构造函数的初始化列表
    
    // class A				
    // {
    // public:
    //     A(int x=0):a(x){}	//构造函数参数有默认值	
    // private:
    //     int a;
    // };

    // class B:public A		
    // {
    // public:
    //     B(){}			//省略初始化列表
    // };

    {
        cout << "构造 B:b = " << b << endl;
    }
    int getB() const
    {
        return b;
    }
    int getObjS() const 
    {
        return objS.getS();
    }
};

//有多个基类时
// 处于同一层次的各个基类的构造函数的调用顺序
// 取决于派生类对基类的声明所指定的顺序（自左向右）
// 按照派生类中声明的各个基类的先后顺序来逐一提供参数 
// class C: public B, public A
// {
// public:
// 	C(int x,int y,int z):B(y),A(x),c(z){ cout << "派生类C" << endl; }
// private:
// 	int c;
// };
// 程序运行时屏幕输出如下：
// 	基类B
// 	基类A
// 	派生类C



// 如果派生类的基类也是一个派生类
// 则每个派生类只需负责其直接基类的构造，依次上溯
// class B:public A	//派生类
// {
// public:
// 	B(int x, int y):A(x), b(y){}
// private:
// 	int b;
// };
// class C: public B	//派生类的派生类
// {
// public:
// 	C(int x,int y,int z):B(x,y),c(z){}
// private:
// 	int c;
// };


int main()
{
    cout << "=== 开始创建 B obj(1,2,3) ===" << endl;
    B obj(1,2,3); 
    //派生类构造函数执行顺序是：
    // 先调用 基类 的构造函数
    // 如果有 子对象 随后调用子对象的构造函数
    //最后调用 派生类 的构造函数。
    cout << "=== 创建完成，开始打印内部值 ===" << endl;
    cout << "obj 里的 A::a = " << obj.getA() << endl;
    cout << "obj 里的 S::s = " << obj.getObjS() << endl;
    cout << "obj 里的 B::b = " << obj.getB() << endl;
    return 0;
}