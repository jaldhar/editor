// Subeditor -- manages text in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <algorithm>
#include <cstdlib>
#include <iterator>
using namespace std;

#include "subeditor.h"

Subeditor::Subeditor() : _buffer{0}, _point{0}, _count{0},
    _gapStart{_buffer.begin()}, _gapEnd{_buffer.end()} {
}

LOCATION Subeditor::bufferStart() {
    return countToLocation(0);
}

LOCATION Subeditor::bufferEnd() {
    return countToLocation(_count);
}

LOCATION Subeditor::pointGet() {
    return countToLocation(_point);
}

bool Subeditor::pointMove(int count) {
    LOCATION loc = countToLocation(_point + count);

    if (compareLocations(loc, bufferStart()) == -1 ||
    compareLocations(loc, bufferEnd()) != -1) {
        return false;
    }

    _point += count;

    return true;
}

bool Subeditor::pointSet(LOCATION loc) {
    _point = locationToCount(loc);
    return true;
}

int Subeditor::compareLocations(LOCATION loc1, LOCATION loc2) {
    if (loc1 > loc2) {
        return 1;
    } else if (loc1 < loc2) {
        return -1;
    }

    return 0;
}

size_t Subeditor::locationToCount(LOCATION loc) {
    size_t count = loc - _buffer.begin();
    if (loc > _gapEnd) {
        count -= (_gapEnd - _gapStart);
    }

    return count;
}

LOCATION Subeditor::countToLocation(size_t count) {
    LOCATION loc = _buffer.begin() + count;

    if (loc > _gapStart) {
        loc -= (_gapEnd - _gapStart);
    }

    return loc;
}

char Subeditor::getChar() {
    return *countToLocation(_point);
}

bool Subeditor::insertChar(char c) {
    moveGap();
    *_gapStart = c;
    advance(_gapStart, 1);
    _count++;
    _point++;
    return true;
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

void Subeditor::moveGap() {
    LOCATION ploc = countToLocation(_point);
    switch(compareLocations(ploc, _gapStart)) {
    case 1: { // point is after gapStart
            size_t n = ploc - _gapEnd;
            _gapStart = ploc;
            _gapEnd += n;
            copy(ploc, ploc + n, _gapEnd);
        }
        break;
    case -1: { // point is before _gapStart
            size_t n = _gapStart - ploc;
            _gapStart = ploc;
            _gapEnd -= n;
            copy(ploc, ploc + n, _gapEnd);
        }
        break;
    default:
        break;
    }
}