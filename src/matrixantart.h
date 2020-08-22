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


#ifndef MatrixAnts_H
#define MatrixAnts_H

#include "ilightshow.h"



class MatrixAntArt : public ILightShow
{
    Q_OBJECT
public:
    MatrixAntArt(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    ~MatrixAntArt();


    // ILightShow interface
public:
    void startShow();

private:
    void moveAnt(uint32_t x, uint32_t y);

private:
    int **_colRowData;
    uint32_t _antX;
    uint32_t _antY;
    int _antDirection;

};

#endif // MatrixAnts_H
