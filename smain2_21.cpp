#include<iostream>
using namespace std;

void Print(const int &n);//常引用做形参
//格式：const 数据类型 & 引用名=变量名
//或者 数据类型 const & 引用名=变量名
//引用 = 变量的别名
//同一块内存，两个名字
//常用于函数参数，用来修改实参、提高效率
int main()
{
    int i=10;
    Print(i);
}
void Print(const int &n)
{
    cout<<n<<endl;
}