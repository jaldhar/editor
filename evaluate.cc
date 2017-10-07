// Evaluate -- dispatches input to commands in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <cctype>
using namespace std;

#include "evaluate.h"
#include "subeditor.h"

Evaluate::Evaluate(Subeditor* subeditor) : _keymap {
    { 0x11, &Subeditor::quit }
}, _subeditor{subeditor} {
}

bool Evaluate::operator()(char c) {
    bool isExit = false;
    bool isArg = false;
    int arg = 1;

    auto it = _keymap.find(c);
    if (it == _keymap.end()) {
        if (isprint(c)) {
            if (!_subeditor->insertChar(c)) {
                // _key.beep();
            }
            return false;
        }
    } else {
        while (!(it->second)(_subeditor, isArg, arg, isExit, c)) {
            // c = _key.get();
            // redisplay();
        }
    }

    return isExit;
}