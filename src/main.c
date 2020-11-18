#ifndef UNICODE
#define UNICODE
#endif

#define True 1
#define False 0
typedef char bool; 

bool open = True;

#include <stdio.h>
#include "winapi stuff.h"

// OPENGL LIBS
#include <GL/GL.h>
#include <GL/GLU.h>

int a = 0;
HWND hwnd;

void window() {
    hwnd = makeWindow(0, 0, 800, 800);
    startWindow(hwnd, 5);
    runEvtHandler();
    open = False;
}

int main() { // rendering context??
    _beginthread(window, 0, NULL);
    wglMakeCurrent(deviceContext, rContext);
 
    while(open == True) {
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();
    }
    
    return 0;
}
