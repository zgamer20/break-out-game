#include"sclass2_4_square.h"
Square::Square(double s)
{
    side=s;
}
double Square::GetSide()
{
    return side;
}
double Square::GetCircumference()
{
    return 4*side;
}
double Square::GetArea()
{
    return side*side;
}
void Square::Print()
{
    cout<<"边长为"<<side<<"的正方形周长是："
    <<GetCircumference()<<",面积是："
    <<GetArea()<<endl;
}