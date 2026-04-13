#include<iostream>
using namespace std;
class Circle
{
private:
    double radius;
public:
    Circle(int r)
    {
        radius=r;
    }
    double GetRadius();
    double Circumference();
    double Area();
    void Print();
};

double Circle::GetRadius()
{
    return radius;
}
double Circle::Circumference()
{
    return 3.14*radius*2;
}
double Circle::Area()
{
    return 3.14*radius*radius;
}
void Circle::Print()
{
    cout<<"半径是："<<radius<<endl;
    cout<<"周长是："<<Circumference()<<endl;
    cout<<"面积是："<<Area()<<endl;
}

int main()
{
    Circle c(5);
    c.Print();
    return 0;
}