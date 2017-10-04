// Key -- key-handling routines for a simple text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.
#ifndef _KEY_H_
#define _KEY_H_

class Key {
public:
    bool init();
    bool fini();
    char get();
};

#endif