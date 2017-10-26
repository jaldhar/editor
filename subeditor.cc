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
    return 0;
}

size_t Subeditor::bufferEnd() {
    return _buffer.size();
}

size_t Subeditor::point() {
    return static_cast<size_t>(_buffer.point());
}

bool Subeditor::insertACharacter(bool& /*isArg*/, int& arg,
bool& /*isExit*/, char c) {
    if (arg < 0) {
        arg = -arg;
    }

    while (arg-- > 0) {
        if (!_buffer.insert(c)) {
            return false;
        }
    }

    return true;
}

bool Subeditor::moveBackwardACharacter(bool& /*isArg*/, int& arg,
bool& /*isExit*/, char /*c*/) {
    arg = -1;
    _buffer.pointMove(arg);
    return true;
}

bool Subeditor::moveForwardACharacter(bool& /*isArg*/, int& arg,
bool& /*isExit*/, char /*c*/) {
    arg = 1;
    _buffer.pointMove(arg);
    return true;
}

bool Subeditor::deletePreviousCharacter(bool& /*isArg*/, int& arg,
    bool& /*isExit*/, char /*c*/) {
    while (arg-- > 0) {
        if (_buffer.deletePrevious()) {
            return false;
        }
    }
    return true;
}

bool Subeditor::deleteNextCharacter(bool& /*isArg*/, int& arg,
    bool& /*isExit*/, char /*c*/) {
    while (arg-- > 0) {
        if (_buffer.deleteNext()) {
            return false;
        }
    }
    return true;
}

bool Subeditor::quit(bool& /*isArg*/, int& /*arg*/, bool& isExit, char /*c*/) {
    isExit = true;

    return true;
}
