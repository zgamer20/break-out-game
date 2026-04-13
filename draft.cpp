#include<iostream>
using namespace std;
int main()
{
    int i=10;
    char c='a';
    float f=20.325;
    int*pi=&i;

    const char*pa="Hello World!";
    cout<<i<<'\t'<<c<<'\t'<<f<<endl;
    cout<<pi<<'\t'<<pa<<endl;
    cout<<"这是一个串常量！";


    int num;
    char str;
    float decimal;

    cin>>num>>str>>decimal;
    cout<<"num="<<num<<endl;
    cout<<"str="<<str<<endl;
    cout<<"decimal="<<decimal<<endl;

    return 0;
}