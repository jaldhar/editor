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

Buffer<char, Subeditor::BUFFERSIZE>& Subeditor::buffer() {
    return _buffer;
}

size_t Subeditor::point() {
    return static_cast<size_t>(_buffer.point());
}

bool Subeditor::self_insert(bool& /*isArg*/, int& arg,
bool& /*isExit*/, int c) {
    if (arg < 0) {
        arg = -arg;
    }

    while (arg-- > 0) {
        if (!_buffer.insert(c)) {
            break;
        }
    }

    return true;
}

bool Subeditor::backward_char(bool& /*isArg*/, int& arg,
bool& /*isExit*/, int /*c*/) {
    arg = -1;
    _buffer.pointMove(arg);
    return true;
}

bool Subeditor::forward_char(bool& /*isArg*/, int& arg,
bool& /*isExit*/, int /*c*/) {
    arg = 1;
    _buffer.pointMove(arg);
    return true;
}

bool Subeditor::backward_delete_char(bool& /*isArg*/, int& arg,
bool& /*isExit*/, int /*c*/) {
    while (arg-- > 0) {
        if (!_buffer.deletePrevious()) {
            break;
        }
    }
    return true;
}

bool Subeditor::delete_char(bool& /*isArg*/, int& arg, bool& /*isExit*/,
int /*c*/) {
    while (arg-- > 0) {
        if (!_buffer.deleteNext()) {
            break;
        }
    }
    return true;
}

bool Subeditor::beginning_of_line(bool& /*isArg*/, int& /*arg*/,
bool& /*isExit*/, int /*c*/) {
    auto result = _buffer.searchBackward('\n', _buffer.point());

    if (result == _buffer.npos) {
        _buffer.pointSet(_buffer.begin().pos());
    } else {
        _buffer.pointSet(result + 1);
    }

    return true;
}

bool Subeditor::end_of_line(bool& /*isArg*/, int& /*arg*/, bool& /*isExit*/,
int /*c*/) {
    auto result = _buffer.searchForward('\n', _buffer.point());

    if (result == _buffer.npos) {
        _buffer.pointSet(_buffer.end().pos());
    } else {
        _buffer.pointSet(result - 1);
    }

    return true;
}

bool Subeditor::next_line(bool& /*isArg*/, int& /*arg*/, bool& /*isExit*/,
int /*c*/) {
    auto result = _buffer.searchForward('\n', _buffer.point());

    if (result == _buffer.npos) {
        _buffer.pointSet(_buffer.end().pos());
    } else {
        _buffer.pointSet(result + 1);
    }

    return true;
}

bool Subeditor::previous_line(bool& /*isArg*/, int& /*arg*/, bool& /*isExit*/,
int /*c*/) {
    auto result = _buffer.searchBackward('\n', _buffer.point());

    if (result == _buffer.npos) {
        _buffer.pointSet(_buffer.begin().pos());
    } else {
        _buffer.pointSet(result - 1);
    }

    return true;
}

bool Subeditor::quit(bool& /*isArg*/, int& /*arg*/, bool& isExit,
int /*c*/) {
    isExit = true;

    return true;
}
