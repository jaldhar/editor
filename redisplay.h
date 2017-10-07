// Redisplay -- displays the output of a text editor (Interface)
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

#ifndef _REDISPLAY_H_
#define _REDISPLAY_H_

class Subeditor;

class Redisplay {
public:
    Redisplay(Subeditor* subeditor);
    void operator()();
private:
    Subeditor* _subeditor;
};

#endif