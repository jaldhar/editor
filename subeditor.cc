// Subeditor -- manages text in a text editor (Implementation)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

using namespace std;

#include "subeditor.h"

bool Subeditor::quit(bool& /*isArg*/, int& /*arg*/, bool& isExit, char /*c*/) {
    isExit = true;

    return true;
}