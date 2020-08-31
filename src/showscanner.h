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

#ifndef ShowScanner_H
#define ShowScanner_H

#include "ilightshow.h"

class ShowScanner : public ILightShow
{
    Q_OBJECT
public:
    ShowScanner(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();

private:
    ws2811_led_t dimcolor(ws2811_led_t color, int width) ;


};

#endif // ShowScanner_H
