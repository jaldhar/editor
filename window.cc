// Window -- display routines for a simple text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <clocale>
#include <csignal>
#include <cstdlib>
using namespace std;

#include "subeditor.h"
#include "window.h"

static WINDOW* _statusWin;
static WINDOW* _titleWin;
static WINDOW* _viewport;

static int createStatusWindow(WINDOW* win, int /*cols*/) {
    _statusWin = win;

    return OK;
}

static int createTitleWindow(WINDOW* win, int /*cols*/) {
    _titleWin = win;

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
    ripoffline(-1, createStatusWindow);

    initscr();

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
    }

    resize();

    setTitle(display);
    return true;
}

int Window::fini() {
    end(0);

    return EXIT_SUCCESS; // never actually called as end() exits.
}

void Window::redisplay(Subeditor& subeditor) {

    wmove(_viewport, 0 , 0);
    wclrtoeol(_viewport);
    for (auto c: subeditor.buffer()) {
        waddch(_viewport, c);
    }

    wmove(_viewport, 0, subeditor.point());
    wrefresh(_viewport);
}

void Window::resize() {
    int lines = 0, cols = 0;
    getmaxyx(stdscr, lines, cols);

    wbkgd(stdscr, ' ');

    if (_viewport == NULL) {
        _viewport = subwin(stdscr, lines, cols, 0, 0);
    } else {
        wresize(_viewport, lines, cols);
    }
    wbkgd(_viewport, ' ' | COLOR_PAIR(1));

    wresize(_titleWin, 1, cols);
    wbkgd(_titleWin, ' ' | COLOR_PAIR(2));

    wresize(_statusWin, 1, cols);
    wbkgd(_statusWin, ' ' | COLOR_PAIR(1));
}

void Window::setTitle(const string& display) {
    int lines = 0, cols = 0;
    lines += 0; // just to prevent 'set but not used' compiler warning;

    getmaxyx(_titleWin, lines, cols);
    wclear(_titleWin);
    mvwaddstr(_titleWin, 0, (cols - display.length()) / 2, display.c_str());
    wnoutrefresh(_titleWin);
}

const WINDOW* Window::viewport() const {
    return _viewport;
}