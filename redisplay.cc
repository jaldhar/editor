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
    BufferInternals internals = _subeditor->_buffer.internals();

    wmove(stdscr, 1 , 0);
    for (auto c = _subeditor->bufferStart(); c != _subeditor->bufferEnd();
    c++) {
        waddch(stdscr, _subeditor->_buffer[c]);
    }

    wmove(stdscr, 4 , 0);
    for (ptrdiff_t c = 0, l = _subeditor->_buffer.size(); c < l; c++) {
        if (c == internals._point) {
            mvwaddch(stdscr, 5, c, 'P' | A_BOLD);
        }
        if (c >= internals._gapStart && c < internals._gapEnd) {
            mvwaddch(stdscr, 4, c, '#' | A_BOLD);
        } else {
            if (isprint(_subeditor->_buffer[c])) {
                mvwaddch(stdscr, 4, c, _subeditor->_buffer[c]);
            } else {
                mvwaddch(stdscr, 4, c, '~');
            }
        }
    }

    mvwprintw(stdscr, 7, 0, "Gap start = %d Gap end = %d Point = %d Count = %d",
        internals._gapStart, internals._gapEnd, internals._point, internals._count);

    wmove(stdscr, 1, internals._point);
}