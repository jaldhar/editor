// Evaluate -- dispatches input to commands in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <algorithm>
#include <cctype>
using namespace std;

#include <curses.h>
#include "buffer.h"
#include "redisplay.h"
#include "subeditor.h"

Redisplay::Redisplay(Subeditor* subeditor) :  _subeditor{subeditor} {
}

void Redisplay::operator()() {
    wmove(stdscr, 1 , 0);
    for (auto c = _subeditor->bufferStart(); c != _subeditor->bufferEnd();
    c++) {
        waddch(stdscr, *_subeditor->_buffer[c]);
    }
}