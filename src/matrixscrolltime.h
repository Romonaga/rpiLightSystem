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


#ifndef MatrixScrollTime_H
#define MatrixScrollTime_H

#include "ilightshow.h"



class MatrixScrollTime : public ILightShow
{
    Q_OBJECT
public:
    MatrixScrollTime(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();

private:
    void scrollTime(QString msg, int maxRows, int maxCols, int rowStart, int drawCol, ws2811_led_t color,  ws2811_led_t *snapShotBuffer, int delay);

private:

};

#endif // MatrixScrollTime_H
