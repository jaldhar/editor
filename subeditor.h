// Subeditor -- manages text in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _SUBEDITOR_H_
#define _SUBEDITOR_H_

#include <array>

using BUFFER = std::array<char, 80>;

class Subeditor {
public:
    Subeditor();
    bool insertChar(char c);
    bool quit(bool& isArg, int& arg, bool& isExit, char c);
    // TODO: make these private.
    BUFFER _buffer;
    BUFFER::iterator _point;
};

#endif