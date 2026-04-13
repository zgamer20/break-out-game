//友元函数与成员函数的比较
#include<iostream>
using namespace std;
class CSample
{
public:
        CSample(){};//空构造函数
        void MemberFunc(int a){i=a;}//成员函数
        friend void FriendFunc(CSample*,int);//声明友元函数
private:
        int i;
};
void FriendFunc(CSample*p,int a)
{
    p->i=a;
    cout<<"i="<<p->i<<endl;
}
int main()
{
    CSample*p=new CSample;
    FriendFunc(p,3);
    delete p;
    return 0;
}