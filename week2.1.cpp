#include<iostream>
using namespace std;

class ScoreCalculator
{
public:
    int CalculateScore(int type){
        int baseScore=0;
        switch(type){
            case 1:
                baseScore+=10;
                break;
            case 2:
                baseScore+=20;
                break;
            case 3:
                baseScore-=5;
                break;
            default:
                baseScore=0;
                break;
        }
        return baseScore;
    }

    int CalculateScore(int type,int combo){
        int baseScore=0;
        switch(type){
            case 1:
                baseScore+=10;
                break;
            case 2:
                baseScore+=20;
                break;
            case 3:
                baseScore-=5;
                break;
            default:
                baseScore=0;
                break;
        }
        return baseScore+combo*2;
    }
};


int main(){
    ScoreCalculator sc;
    int normalScore=sc.CalculateScore(1);
    cout<<"普通砖块得分："<<normalScore<<endl;

    int goldComboScore=sc.CalculateScore(2,3);
    cout<<"金色砖块+3连击得分:"<<goldComboScore<<endl;
    return 0;
}