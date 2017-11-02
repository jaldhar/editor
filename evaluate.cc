// Evaluate -- dispatches input to commands in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#include <cctype>
using namespace std;

#include <curses.h>
#include "evaluate.h"
#include "key.h"
#include "redisplay.h"
#include "subeditor.h"

Evaluate::Evaluate(Subeditor& subeditor, Key& key, Redisplay& redisplay) :
_keymap {
    { 0x06, &Subeditor::forward_char }, // CTRL-f
    { KEY_RIGHT, &Subeditor::forward_char },
    { 0x02, &Subeditor::backward_char }, // CTRL-b
    { KEY_LEFT, &Subeditor::backward_char },
    { 0x08, &Subeditor::backward_delete_char }, // CTRL-h
    { KEY_BACKSPACE, &Subeditor::backward_delete_char },
    { 0x04, &Subeditor::delete_char }, // CTRL-d
    { KEY_DC, &Subeditor::delete_char },
    { 0x11, &Subeditor::quit }, // CTRL-q
}, _subeditor{subeditor}, _key{key}, _redisplay{redisplay} {
}

bool Evaluate::operator()(int c) {
    bool isExit = false;
    bool isArg = false;
    int arg = 1;

    auto it = _keymap.find(c);
    if (it == _keymap.end()) {
        if (isprint(c)) {
            if (!_subeditor.self_insert(isArg, arg, isExit, c)) {
                _key.beep();
            }
        }
    } else {
        while (!(it->second)(_subeditor, isArg, arg, isExit, c)) {
            c = _key.get();
            _redisplay();
        }
    }

    return isExit;
}