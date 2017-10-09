// Subeditor -- manages text in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <cstdlib>
#include <iterator>
using namespace std;

#include "subeditor.h"

Subeditor::Subeditor() : _buffer{0}, _point{_buffer.begin()},
    _gapStart{_buffer.begin()}, _gapEnd{_buffer.end() - 1} {
}

LOCATION Subeditor::bufferStart() {
    return _buffer.begin();
}

LOCATION Subeditor::bufferEnd() {
    return _buffer.end();
}

LOCATION Subeditor::pointGet() {
    return _point;
}

bool Subeditor::pointMove(int count) {
    if (compareLocations(_point + count, bufferStart()) == -1 ||
    compareLocations(_point + count, bufferEnd()) != -1) {
        return false;
    }

    advance(_point, count);

    return true;
}

// bool pointSet(LOCATION loc) {
//     return true;
// }

int Subeditor::compareLocations(LOCATION loc1, LOCATION loc2) {
    if (loc1 > loc2) {
        return 1;
    } else if (loc1 < loc2) {
        return -1;
    }

    return 0;
}

int Subeditor::locationToCount(LOCATION loc) {
    return loc - bufferStart();
}

LOCATION Subeditor::countToLocation(int count) {
    return bufferStart() + count;
}

bool Subeditor::insertChar(char c) {
    *_point = c;
    return pointMove(1);
}

bool Subeditor::insertACharacter(bool& /*isArg*/, int& arg, bool& /*isExit*/,
char c) {
    if (arg < 0) {
        arg = -arg;
    }

    while (arg-- > 0) {
        insertChar(c);
    }

    return true;
}

bool Subeditor::moveByCharacter(bool& /*isArg*/, int& arg, bool& /*isExit*/,
char /*c*/) {
    return pointMove(arg);
}

bool Subeditor::moveBackwardACharacter(bool& isArg, int& arg, bool& isExit,
char c) {
    arg = -1;
    return moveByCharacter(isArg, arg, isExit, c);
}

bool Subeditor::moveForwardACharacter(bool& isArg, int& arg, bool& isExit,
char c) {
    arg = 1;
    return moveByCharacter(isArg, arg, isExit, c);
}

bool Subeditor::quit(bool& /*isArg*/, int& /*arg*/, bool& isExit, char /*c*/) {
    isExit = true;

    return true;
}