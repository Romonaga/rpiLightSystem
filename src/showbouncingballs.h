#ifndef ShowBouncingBalls_H
#define ShowBouncingBalls_H

#include "ilightshow.h"



class ShowBouncingBalls : public ILightShow
{
    Q_OBJECT
public:
    ShowBouncingBalls(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    ~ShowBouncingBalls();

    // ILightShow interface
public:
    void startShow();

private:
    int numAliveNeighbours(int x, int y);
    void updateValues() ;

private:
    int** values_;
    int** newValues_;
    int delay_ms_;
    int r_;
    int g_;
    int b_;
    int width_;
    int height_;
    bool torus_;

};

#endif // ShowBouncingBalls_H
