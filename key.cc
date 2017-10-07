// Key -- key-handling routines for a simple text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

using namespace std;

#include <curses.h>
#include "key.h"

bool Key::init() {
    raw();
    noecho();
    nonl();
    idlok(stdscr, TRUE);
    keypad(stdscr, TRUE);
    intrflush(stdscr, FALSE);
    nodelay(stdscr, FALSE);

    return true;
}

bool Key::fini() {
    return true;
}

char Key::get() {
    return (char)getch();
}