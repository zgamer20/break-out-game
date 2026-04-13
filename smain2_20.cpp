#include<iostream>
using namespace std;

class Node
{
public:
    Node(int e,Node*p)
    {
        data=e;
        next=p;
    }
    friend class Stack;
private:
    int data;
    Node*next;
};


class Stack
{
public:
    Stack(){top=NULL;}
    ~Stack(){while(top!=NULL)Pop();}
    void Push(int e);
    int Pop();
private:
    Node*top;
};

void Stack::Push(int e)
{
    Node*p=new Node(e,top);
    top=p;
}
int Stack::Pop()
{
    Node*p=top;
    if(top!=NULL)
    {
        top=top->next;
        int e=p->data;
        delete p;
        return e;
    }
    else
    {
        cout<<"栈已空！"<<endl;
        return 0;
    }
}

int main()
{
    Stack s;
    s.Push(1);
    s.Push(2);
    s.Push(3);
    for(int i=0;i<3;i++)
    {
        cout<<s.Pop()<<" ";
    }
    cout<<endl;
    return 0;
}