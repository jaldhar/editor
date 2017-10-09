// Subeditor -- manages text in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _SUBEDITOR_H_
#define _SUBEDITOR_H_

#include <array>

static constexpr size_t BUFFERSIZE = 80;
using BUFFER = std::array<char, BUFFERSIZE>;
using LOCATION = BUFFER::iterator;

class Subeditor {
public:
    Subeditor();
    LOCATION bufferStart();
    LOCATION bufferEnd();
    LOCATION pointGet();
    bool pointMove(int count);
    // bool pointSet(LOCATION loc);

    int compareLocations(LOCATION loc1, LOCATION loc2);
    int locationToCount(LOCATION loc);
    LOCATION countToLocation(int count);

    bool insertChar(char c);
    bool moveByCharacter(bool& isArg, int& arg, bool& isExit, char c);

    bool insertACharacter(bool& isArg, int& arg, bool& isExit, char c);
    bool moveBackwardACharacter(bool& isArg, int& arg, bool& isExit, char c);
    bool moveForwardACharacter(bool& isArg, int& arg, bool& isExit, char c);
    bool quit(bool& isArg, int& arg, bool& isExit, char c);
private:
    BUFFER _buffer;
    LOCATION _point;
};

#endif