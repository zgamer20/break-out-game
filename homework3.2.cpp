#include<iostream>
#include<string>
using namespace std;

class Person
{
protected:
    string name;
    int age;
    char sex;
    string address;
    string phone;
    float salary;
public:
    Person();
    Person(string n,int a,char s,string add,string ph,float sa);
    void Show() const;
};

Person::Person(){};
Person::Person(string n, int a, char s, string add, string ph, float sa)
    : name(n), age(a), sex(s), address(add), phone(ph), salary(sa){}

void Person::Show() const
{
    cout<<"姓名："<<name
    <<","<<"年龄："<<age
    <<","<<"性别："<<sex
    <<","<<"住址："<<address
    <<","<<"电话："<<phone
    <<","<<"工资:"<<salary<<endl;
}

class Teacher:public virtual Person
{
protected:
    string title;
public:
    Teacher();
    Teacher(string n,int a,char s,string add,string ph,float sa,string t);
    void Show() const;
};

Teacher::Teacher(){};
Teacher::Teacher(string n, int a, char s, string add, string ph, float sa, string t)
    :Person(n, a, s, add, ph, sa),title(t){}
void Teacher::Show()const
{
    cout<<"[教师信息]";
    Person::Show();
    cout<<"职称："<<title<<endl;
}

class Cadre:public virtual Person
{
protected:
    string position;
public:
    Cadre();
    Cadre(string n, int a, char s, string add, string ph, float sa, string pos);
    void Show() const;
};

Cadre::Cadre(){};
Cadre::Cadre(string n, int a, char s, string add, string ph, float sa, string pos)
    :Person(n, a, s, add, ph, sa),position(pos) {}
void Cadre::Show()const
{
    cout<<"[干部信息]";
    Person::Show();
    cout<<"职务："<<position<<endl;
}

class TeacherCadre:public Teacher,public Cadre
{
public:
    TeacherCadre();
    TeacherCadre(string n, int a, char s, string add, string ph, float sa, string pos, string t);
    void Show() const;
};
TeacherCadre::TeacherCadre(){};
TeacherCadre::TeacherCadre(string n, int a, char s, string add, string ph, float sa, string pos, string t)
    : Person(n, a, s, add, ph, sa), Teacher(n, a, s, add, ph, sa, t), Cadre(n, a, s, add, ph, sa, pos) {}

void TeacherCadre::Show()const
{
    cout<<"=====教师干部信息====="<<endl;
    Person::Show();
    cout<<"职务："<<position<<"职称："<<title<<endl;
    cout<<"====================="<<endl;
}

int main(){
    Teacher t("Lisa", 35, 'F', "文化路12号", "13888888", 8000, "高级讲师");
    Cadre c("Jack", 40, 'M', "北京路5号", "13999999", 9000, "教研室主任");
    TeacherCadre tc("Mark", 42, 'M', "长江路8号", "13666666", 12000, "副院长", "副教授");

    t.Show();
    cout << endl;
    c.Show();
    cout << endl;
    tc.Show();
    return 0;
}