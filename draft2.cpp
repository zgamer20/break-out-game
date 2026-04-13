#include<iostream>
using namespace std;

int i=10;

void fun()
{
    int i=20;
    cout<<"局部变量i="<<i;
    cout<<",全局变量i="<<::i;
}

int main()
{
    fun();
    return 0;
}