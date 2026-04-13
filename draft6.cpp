#include<iostream>
#include<cstring>
using namespace std;

int main()
{
    char dest[100]="destination",src[100]="source";
    strcpy(dest,src);
    cout<<"strcpy(dest,src),"<<"dest:"<<dest<<endl;

    strncpy(dest,src,4);
    dest[4]='\0';
    cout<<"strncpy(dest,src,4),"<<"dest:"<<dest<<endl;

    strcpy(dest,"Hello ");
    strcpy(src,"world");
    strcat(dest,src);
    cout<<"strcat(dest,src),"<<"dest:"<<dest<<endl;

    return 0;
}