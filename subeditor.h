// Subeditor -- manages text in a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _SUBEDITOR_H_
#define _SUBEDITOR_H_

#include "buffer.h"

class Subeditor {
    static const std::size_t BUFFERSIZE = 80;

public:
    Subeditor();

    Buffer<char, BUFFERSIZE>& buffer();
    size_t point();

    bool self_insert(bool& isArg, int& arg, bool& isExit, int c);
    bool backward_char(bool& isArg, int& arg, bool& isExit,int c);
    bool forward_char(bool& isArg, int& arg, bool& isExit, int c);
    bool backward_delete_char(bool& isArg, int& arg, bool& isExit, int c);
    bool delete_char(bool& isArg, int& arg, bool& isExit, int c);
    bool quit(bool& isArg, int& arg, bool& isExit, int c);

private:
    Buffer<char, BUFFERSIZE>   _buffer;
};

#endif