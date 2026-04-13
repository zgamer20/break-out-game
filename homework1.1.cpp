#include<iostream>
#include<cstring>
using namespace std;

int main()
{
    cout<<"现在在下雨吗？"<<endl;
    cout<<"请输入'Y'或'N'"<<endl;

    char reply;
    cin>>reply;

    if(reply=='Y'){
        cout<<"现在正在下雨"<<endl;
    }
    else if(reply=='N'){
        cout<<"现在没有下雨"<<endl;
    }

    return 0;
}