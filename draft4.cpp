#include<iostream>
using namespace std;56

int max(int a,int b)
{
    if(b>a) return b;
    else return a;
}

float max(float a,float b,float c)
{
    if(b>a) a=b;
    if(c>a) a=c;
    return a;
}

int main()
{
    int a,b;
    float d,e,f;
    cin>>a>>b>>d>>e>>f;
    int m;
    m=max(a,b);
    cout<<"max_i="<<m<<endl;
    float n;
    n=max(d,e,f);
    cout<<"max_f="<<n<<endl;
    return 0;
}