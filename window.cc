// Window -- display routines for a simple text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <clocale>
#include <csignal>
#include <cstdlib>
using namespace std;

#include "window.h"

static WINDOW* titleWin;
static int     titleCols;

static int createTitleWindow(WINDOW* win, int cols) {
    titleWin = win;
    titleCols = cols;

    return OK;
}

static void end(int /* sig */) {
    curs_set(1);
    endwin();
    clear();
    exit(EXIT_SUCCESS);
}

bool Window::init(string display) {
    setlocale(LC_ALL, "POSIX");

    struct sigaction act;
    act.sa_handler = end;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGSEGV, &act, NULL);

    ripoffline(1, createTitleWindow);

    initscr();

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
    }

    wbkgd(titleWin, ' ' | COLOR_PAIR(2));
    wclear(titleWin);
    mvwaddstr(titleWin, 0, (titleCols - display.length()) / 2,
        display.c_str());
    wnoutrefresh(titleWin);

    return true;
}

int Window::fini() {
    end(0);

    return EXIT_SUCCESS; // never actually called as end() exits.
}
