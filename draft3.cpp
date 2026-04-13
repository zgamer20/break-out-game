#include<iostream>
using namespace std;

inline int add(int x,int y)//或者add(int,int)也是对的
{
    return x+y;
}

int main()
{
    for(int i=10;i>=0;i--){
        cout<<add(i,i)<<" ";
    }
    return 0;
}