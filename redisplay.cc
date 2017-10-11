// Evaluate -- dispatches input to commands in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <algorithm>
#include <cctype>
using namespace std;

#include <curses.h>
#include "redisplay.h"
#include "subeditor.h"

Redisplay::Redisplay(Subeditor* subeditor) :  _subeditor{subeditor} {
}

void Redisplay::operator()() {
    size_t gapStart = _subeditor->locationToCount(_subeditor->_gapStart);
    size_t gapEnd = _subeditor->locationToCount(_subeditor->_gapEnd);
    size_t point = _subeditor->locationToCount(_subeditor->pointGet());
    size_t count = _subeditor->_count;

    wmove(stdscr, 1 , 0);
    for (size_t c = 0, l = _subeditor->_buffer.size(); c < l; c++) {
        if (c >= gapStart && c < gapEnd) {
            continue;
        }
        waddch(stdscr, _subeditor->_buffer[c]);
    }

    wmove(stdscr, 4 , 0);
    for (size_t c = 0, l = _subeditor->_buffer.size(); c < l; c++) {
        if (c == _subeditor->_point) {
            waddch(stdscr, 'P' | A_BOLD);
        } else if (c >= gapStart && c < gapEnd) {
            waddch(stdscr, '#' | A_BOLD);
        } else {
            if (isprint(_subeditor->_buffer[c])) {
                waddch(stdscr, _subeditor->_buffer[c]);
            } else {
                waddch(stdscr, '~');
            }
        }
    }

    mvwprintw(stdscr, 7, 0, "Gap start = %d Gap end = %d Point = %d Count = %d",
        gapStart, gapEnd, point, count);

    wmove(stdscr, 1, point);
}