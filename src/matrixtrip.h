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


#ifndef MatrixTrip_H
#define MatrixTrip_H

#include "ilightshow.h"





class MatrixTrip : public ILightShow
{
    Q_OBJECT
public:
    MatrixTrip(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    void showDisplay(char* display);

    // ILightShow interface
public:
    void startShow();

private:
    void midPointCircleDraw(int x_centre, int y_centre, int r) ;

    void circlePoints(int cx, int cy, int x, int y);
    void circleMidpoint(int xCenter, int yCenter, int radius);


};

#endif // MatrixTrip_H
