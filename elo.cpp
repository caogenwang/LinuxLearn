#include <iostream>
#include <cmath>
#include <unistd.h>
using namespace std;
 
#define  ELO_RESULT_WIN  1
#define  ELO_RESULT_LOSS  -1
#define  ELO_RESULT_TIE  0
 
class Elo{
public:
    Elo(int __ratingA ,int __ratingB,int result)
        :ratingA(__ratingA),ratingB(__ratingB){
            switch(result)
            {
                case 1:
                    setResult(ELO_RESULT_WIN);
                    break;
                case -1:
                    setResult(ELO_RESULT_LOSS);
                    break;
                case 0:
                    setResult(ELO_RESULT_TIE);
                    break;
                default:
                    break;
            }
    }
    void setratingA(int ratingA)
    {
        this->ratingA = ratingA;
    }

    void setratingB(int ratingB)
    {
        this->ratingB = ratingB;
    }
    int getratingA()
    {
        return this->ratingA;
    }
    
    int getratingB()
    {
        return this->ratingB;
    }

    void setResult(int result){
        float scoreAwin = computeScore(ratingA,ratingB);
        float scoreBwin = computeScore(ratingB,ratingA);//修正为float scoreBwin = 1 - scoreAwin；
        float score_adjust = 0.0;
        if (result == ELO_RESULT_WIN)
            score_adjust = 1.0;
        else if(result == ELO_RESULT_LOSS)
            score_adjust = 0.0;
        else
            score_adjust = 0.5;

        ratingA = ratingA + computeK(ratingA) * (score_adjust - scoreAwin);
        ratingB = ratingB - computeK(ratingB) * (score_adjust - scoreBwin);
        sleep(2);
    }

private:
    int computeK(int rating){
        if (rating >= 2400)
            return 16;
        else if(rating >= 2100)
            return 24;
        else
            return 36;
    }

    float computeScore(int rating1, int rating2){
        return 1 / (1+pow(10, (rating1 - rating2) / 400));
    }

private:
    int ratingA;
    int ratingB;
};

int main()
{
    int player1 = 1500;
    int player2 = 1600;
    cout<<"round 1,begin()"<<endl;
    Elo elo1(player1,player2,1);
    cout<<"ratingA:"<<elo1.getratingA()<<endl;
    cout<<"ratingB:"<<elo1.getratingB()<<endl;

    cout<<"round 2,begin()"<<endl;
    Elo elo2(elo1.getratingA(),elo1.getratingB(),1);
    cout<<"ratingA:"<<elo2.getratingA()<<endl;
    cout<<"ratingB:"<<elo2.getratingB()<<endl;

    cout<<"round 3,begin()"<<endl;
    Elo elo3(elo2.getratingA(),elo2.getratingB(),1);
    cout<<"ratingA:"<<elo3.getratingA()<<endl;
    cout<<"ratingB:"<<elo3.getratingB()<<endl;

    exit(0);
}