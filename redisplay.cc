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
    wclrtoeol(stdscr);
    for (auto c: _subeditor->_buffer) {
        waddch(stdscr, c);
    }

    wmove(stdscr, 5, 0);
    wclrtoeol(stdscr);
    wmove(stdscr, 4, 0);
    for (ptrdiff_t c = 0, l = _subeditor->_buffer.capacity(); c < l; c++) {
        if (c == internals._point) {
            mvwaddch(stdscr, 5, c, 'P' | A_BOLD);
        }
        if (c >= internals._gapStart && c < internals._gapEnd) {
            mvwaddch(stdscr, 4, c, '#' | A_BOLD);
        } else {
            if (isprint(_subeditor->_buffer._text[c])) {
                mvwaddch(stdscr, 4, c, _subeditor->_buffer._text[c]);
            } else {
                mvwaddch(stdscr, 4, c, '~');
            }
        }
    }

    wmove(stdscr, 7, 0);
    wclrtoeol(stdscr);
    wprintw(stdscr,
        "Buffer capacity = %d Gap start = %d Gap end = %d Point = %d Size = %d",
        internals._capacity, internals._gapStart, internals._gapEnd,
        internals._point, internals._size);

    wmove(stdscr, 1, _subeditor->point());
}