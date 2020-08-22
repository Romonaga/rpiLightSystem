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


#ifndef MatrixKaleidoscope_H
#define MatrixKaleidoscope_H

#include "ilightshow.h"



class MatrixKaleidoscope : public ILightShow
{
    Q_OBJECT
public:
    MatrixKaleidoscope(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    ~MatrixKaleidoscope();


    // ILightShow interface
public:
    void startShow();

private:
    void updateValues();

private:
    int _height;
    int** _values;
    int** _newValues;


};

#endif // ShowBouncingBalls_H
