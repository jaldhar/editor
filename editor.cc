// editor -- a simple text editor
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

using namespace std;

#include "evaluate.h"
#include "key.h"
#include "redisplay.h"
#include "subeditor.h"
#include "window.h"

void redisplay() {
}

int main(int argc, const char* argv[]) {
    Window window;
    Key key;
    Subeditor subeditor;
    Evaluate evaluate(&subeditor);
    Redisplay redisplay(&subeditor);
    window.init("Editor");
    key.init();
    
    int c;
    redisplay();
    while(true) {
        c = key.get();
        if (evaluate(c)) {
            break;
        }
        redisplay();
    }

    key.fini();
    return window.fini();
}