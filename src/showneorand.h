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

#ifndef SHOWShowNeoRand_H
#define SHOWShowNeoRand_H

#include "ilightshow.h"

class ShowNeoRand : public ILightShow
{
    Q_OBJECT
public:
    ShowNeoRand(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();

private:
    unsigned int random9();
    uint32_t Bow(int n);


private:

    unsigned int _rno;
    unsigned int _cno;
    int _tw;

    ws2811_led_t _blue;
    ws2811_led_t _green;
    ws2811_led_t _red;


};

#endif // SHOWShowNeoRand_H
