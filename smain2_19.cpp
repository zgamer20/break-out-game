//友元类
#include<iostream>
#include<cstring>
using namespace std;

class A
{
private:
        friend class B;//友元类声明
        // 放 private、public、protected 效果一模一样
        char*name;
        int age;
public:
        A(const char *str,int n)//带参的构造函数
        {
            name=new char[strlen(str)+1];
            strcpy(name,str);
            age=n;
        }
};

class B
{
public:
    void Show(const A & x)
    {
        cout<<"姓名："<<x.name<<endl;
        cout<<"年龄："<<x.age<<endl;
    }
};
int main()
{
    A a("左力",19);
    B b;
    b.Show(a);
    return 0;
}