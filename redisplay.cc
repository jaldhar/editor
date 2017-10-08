// Evaluate -- dispatches input to commands in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <cctype>
using namespace std;

#include <curses.h>
#include "redisplay.h"
#include "subeditor.h"

Redisplay::Redisplay(Subeditor* subeditor) :  _subeditor{subeditor} {
}

void Redisplay::operator()() {
    wmove(stdscr, 1 , 0);
    for (auto& c: _subeditor->_buffer) {
        if (isprint(c)) {
            waddch(stdscr, c);
        } else {
            waddch(stdscr, '~' | A_BOLD);
        }
    }
    wmove(stdscr, 1, _subeditor->pointGet());
}