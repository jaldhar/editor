// Evaluate -- dispatches input to commands in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

using namespace std;

#include "evaluate.h"
#include "subeditor.h"

Evaluate::Evaluate(Subeditor* subeditor) : _keymap {
    { 'q', &Subeditor::quit }
}, _subeditor{subeditor} {
}

bool Evaluate::operator()(char c) {
    bool isExit = false;
    bool isArg = false;
    int arg = 1;

    auto it = _keymap.find(c);
    if (it == _keymap.end()) {
        return false;
    }

    while (!(it->second)(_subeditor, isArg, arg, isExit, c)) {
        // c = _key.get();
        // redisplay();
    }

    return isExit;
}