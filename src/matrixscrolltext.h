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

#ifndef MatrixScrollText_H
#define MatrixScrollText_H

#include "ilightshow.h"





class MatrixScrollText : public ILightShow
{
    Q_OBJECT
public:
    MatrixScrollText(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);
    ~MatrixScrollText();

    // ILightShow interface
public:
    void startShow();


private:


    
private:





};

#endif // ShowBouncingBalls_H
