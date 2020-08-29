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
// robert Winslow- Ported from demo example in  rpi-led-rgp-matrix.

#ifndef MatrixMazeCreator_H
#define MatrixMazeCreator_H

#include "ilightshow.h"
#include <random>

typedef enum
{
    Wall = 0,
    Passage = 1
} mazePathObjects;

typedef enum
{
    N = 0,
    E = 1,
    S = 2,
    W = 3
} mazeDirections;

class MatrixMazeCreator : public ILightShow
{
    Q_OBJECT
public:
    MatrixMazeCreator(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

private:
    void genMaze(uint32_t row, uint32_t col);

    // ILightShow interface
public:
    void startShow();

private:
    std::vector<std::vector<uint32_t>> _surface;


};

#endif // MatrixMazeCreator_H
