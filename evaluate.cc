// Evaluate -- dispatches input to commands in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <cctype>
using namespace std;

#include <curses.h>
#include "evaluate.h"
#include "subeditor.h"

Evaluate::Evaluate(Subeditor* subeditor) : _keymap {
    { 0x06, &Subeditor::moveForwardACharacter }, // CTRL-f
    { KEY_RIGHT, &Subeditor::moveForwardACharacter },
    { 0x02, &Subeditor::moveBackwardACharacter }, // CTRL-b
    { KEY_LEFT, &Subeditor::moveBackwardACharacter },
    { 0x08, &Subeditor::deletePreviousCharacter }, // CTRL-h
    { KEY_BACKSPACE, &Subeditor::deletePreviousCharacter },
    { 0x04, &Subeditor::deleteNextCharacter },
    { KEY_DC, &Subeditor::deleteNextCharacter },
    { 0x11, &Subeditor::quit }, // CTRL-q
}, _subeditor{subeditor} {
}

bool Evaluate::operator()(char c) {
    bool isExit = false;
    bool isArg = false;
    int arg = 1;

    auto it = _keymap.find(c);
    if (it == _keymap.end()) {
        if (isprint(c)) {
            if (!_subeditor->insertACharacter(isArg, arg, isExit, c)) {
                // _key.beep();
            }
        }
    } else {
        while (!(it->second)(_subeditor, isArg, arg, isExit, c)) {
            // c = _key.get();
            // redisplay();
        }
    }

    return isExit;
}