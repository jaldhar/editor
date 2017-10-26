// Subeditor -- manages text in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _SUBEDITOR_H_
#define _SUBEDITOR_H_

#include "buffer.h"

class Redisplay;

class Subeditor {
public:
    Subeditor();
    size_t bufferStart();
    size_t bufferEnd();
    size_t point();

    bool insertACharacter(bool& isArg, int& arg, bool& isExit, char c);
    bool moveBackwardACharacter(bool& isArg, int& arg, bool& isExit, char c);
    bool moveForwardACharacter(bool& isArg, int& arg, bool& isExit, char c);
    bool deletePreviousCharacter(bool& isArg, int& arg, bool& isExit, char c);
    bool quit(bool& isArg, int& arg, bool& isExit, char c);

private:
    friend Redisplay;

    Buffer<char>   _buffer;
};

#endif