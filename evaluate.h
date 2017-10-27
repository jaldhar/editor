// Evaluate -- dispatches input to commands in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _EVALUATE_H_
#define _EVALUATE_H_

#include <functional>
#include <map>

class Subeditor;
using COMMAND = 
  std::function<bool(Subeditor*, bool& isArg, int& arg, bool& isExit, int c)>;

class Evaluate {
public:
    Evaluate(Subeditor* subeditor);
    bool operator()(int c);
private:
    std::map<int, COMMAND>  _keymap;
    Subeditor*              _subeditor;
};

#endif