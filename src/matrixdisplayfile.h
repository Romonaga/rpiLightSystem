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

#ifndef MatrixCreateDisplayFile_H
#define MatrixCreateDisplayFile_H

#include "ilightshow.h"
#include <vector>





class MatrixCreateDisplayFile : public ILightShow
{
    Q_OBJECT
public:
    MatrixCreateDisplayFile(Ws2811Wrapper* ledWrapper, const LedLightShows &lightShow, const QString &showParms);

    // ILightShow interface
public:
    void startShow();

private:


private:
    std::vector<unsigned char> _jpgBuffer;


};

#endif // MatrixCreateDisplayFile_H
