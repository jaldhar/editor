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

Subeditor::Subeditor() : _buffer() {
}

size_t Subeditor::bufferStart() {
    return _buffer.begin().pos();
}

size_t Subeditor::bufferEnd() {
    return _buffer.end().pos();
}

bool Subeditor::insertACharacter(bool& /*isArg*/, int& arg,
bool& /*isExit*/, char c) {
    if (arg < 0) {
        arg = -arg;
    }

    while (arg-- > 0) {
        _buffer.insert(c);
    }

    return true;
}

bool Subeditor::moveBackwardACharacter(bool& /*isArg*/, int& arg,
bool& /*isExit*/, char /*c*/) {
    arg = -1;
    return _buffer.pointMove(arg);
}

bool Subeditor::moveForwardACharacter(bool& /*isArg*/, int& arg,
bool& /*isExit*/, char /*c*/) {
    arg = 1;
    return _buffer.pointMove(arg);
}

bool Subeditor::quit(bool& /*isArg*/, int& /*arg*/, bool& isExit, char /*c*/) {
    isExit = true;

    return true;
}