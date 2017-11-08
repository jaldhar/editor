// editor -- a simple text editor
//
// By Jaldhar H. Vyas <jaldhar@braincells.com>
// Copyright (C) 2017, Consolidated Braincells Inc. All rights reserved.
// "Do what thou wilt" shall be the whole of the license.

using namespace std;

#include "evaluate.h"
#include "key.h"
#include "subeditor.h"
#include "window.h"

void redisplay() {
}

int main(int argc, const char* argv[]) {
    Window window;
    Key key;
    Subeditor subeditor;
    window.init("Editor");
    Evaluate evaluate(subeditor, key, window);
    key.init();
    
    int c;
    window.redisplay(subeditor);
    while(true) {
        c = key.get();
        if (c == KEY_RESIZE) { // Special NCurses SIGWINCH handler.
            window.resize();
        } else if (evaluate(c)) {
            break;
        }
        window.redisplay(subeditor);
    }

    key.fini();
    return window.fini();
}