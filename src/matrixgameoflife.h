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
// robert Winslow - Ported from demo example in  rpi-led-rgp-matrix.

#ifndef MatrixGameOfLife_H
#define MatrixGameOfLife_H

#include "ilightshow.h"

class MatrixGameOfLife : public ILightShow
{
    Q_OBJECT
public:
    MatrixGameOfLife(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    ~MatrixGameOfLife();

private:
    void updateValues();
    int numAliveNeighbours(int x, int y);

    // ILightShow interface
public:
    void startShow();

private:
    int** _values;
    int** _newValues;
    bool _torus;

};

#endif // MatrixGameOfLife_H
