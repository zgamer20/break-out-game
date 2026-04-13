#ifndef _SCLASS2_2_CDEMO_H_
#define _SCLASS2_2_CDEMO_H_
#include<iostream>
using namespace std;

class CDemo
{
public:
    void Setl(int a)
    {
        i=a;
    }
    
    void SetJ(int b)
    {
        j=b;
    }

    void Getl(){
        cout<<"i="<<i<<endl;
    }
    void GetJ()
    {
        cout<<"j="<<j<<endl;
    }

protected:
    int j;

private:
    int i;
};
#endif