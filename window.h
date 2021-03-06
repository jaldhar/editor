// Window -- display routines for a simple text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <curses.h>
#include <string>

class Subeditor;

class Window {
public:
    bool init(std::string display);
    int  fini();
    void redisplay(Subeditor& subeditor);
    void resize();
    void setTitle(const std::string& display);
    const WINDOW* viewport() const;
};

#endif