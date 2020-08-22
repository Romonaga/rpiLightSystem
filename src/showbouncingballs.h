// Copyright (C) 2020 Robert Winslow <robertwinslow@comcast.net>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//


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
