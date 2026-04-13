#include<iostream>
using namespace std;

class CDemo
{
public:
    CDemo(int ii=0):i(ii)//构造函数在对象创建时利用给定的值去初始化对象成员
    // (此时对象还未创建)
    //CDemo是带默认值ii的构造函数
    {
        j=i;
    }
    static int Incr()
    {
        return ++j;
        //静态函数访问对象成员：
        //不能直接写return i+j,因为无法确定是哪个对象的i
        //应将i所属的对象作为参数传到静态成员函数中
        //static int Incr(const CDemo &p)
        //{
        //  return p.i+j;
        //}
    }
private:
    int i;//对象独有
    static int j;//类内对象共享
};
int CDemo::j=0;
int main()
{
    cout<<"j="<<CDemo::Incr()<<endl;//通过类名访问静态成员函数
    return 0;
}