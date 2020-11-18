// Works Cited
// https://docs.microsoft.com/en-us/windows/win32/learnwin32/learn-to-program-for-windows
// https://docs.microsoft.com/en-us/windows/win32/api/_winmsg/
// http://www.winprog.org/tutorial/start.html
// https://wiki.winehq.org/List_Of_Windows_Messages

// https://docs.microsoft.com/en-us/windows/win32/gdi/device-contexts
// https://docs.microsoft.com/en-us/windows/win32/opengl 
// https://docs.microsoft.com/en-us/windows/win32/opengl/creating-a-rendering-context-and-making-it-current

#include <windows.h>
#include <process.h>

HGLRC rContext;
HDC deviceContext;

// --------------------------------------------------------------------------------------- //
//                                      Events                                             //
// --------------------------------------------------------------------------------------- // 

void runEvtHandler() {
    MSG msg = { 0 }; 

    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK evtHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CLOSE: 
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY: // delete device context
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(rContext);
            MessageBox(NULL, L"Closing", L"Yes", MB_ICONEXCLAMATION | MB_OK);
            PostQuitMessage(1);
            open = False;
            break;

        case WM_LBUTTONDOWN:
            DestroyWindow(hwnd);
            break;

        case WM_CREATE:
            deviceContext = GetDC(hwnd);

            // Pixel Format
            PIXELFORMATDESCRIPTOR pFormat = {
                sizeof(PIXELFORMATDESCRIPTOR),                              // nSize
                1,                                                          // nVersion
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,                    // dwFlags
                PFD_TYPE_RGBA,                                              // iPixelType
                24,                                                         // cColorBits
                0, 0, 0, 0, 0, 0,                                           // c...Bits/Shifts
                0,                                                          // cAlphaBits
                0,                                                          // cAlphaShift (ignored)
                0,
                0, 0, 0, 0,
                32, 
                0,
                0,
                PFD_MAIN_PLANE,
                0,
                0, 0, 0
            };

            int pIndex = ChoosePixelFormat(deviceContext, &pFormat);
            SetPixelFormat(deviceContext, pIndex, &pFormat);

            rContext = wglCreateContext(deviceContext);

            //optomize
            if(rContext != NULL) {
                wglMakeCurrent(NULL, NULL);
            }

            else {
                MessageBox(NULL, L"Unable to create rendering context", L"Oopsie", MB_ICONSTOP | MB_OK);
                DestroyWindow(hwnd);
            }
            return DefWindowProc(hwnd, msg, wParam, lParam);
            break;
        
        default: 
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    
    return 0; 
}

// --------------------------------------------------------------------------------------- //
//                                      Windows                                            //
// --------------------------------------------------------------------------------------- //

void startWindow(HWND hwnd, int nCmdShow) {
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
}

HWND makeWindow(int posX, int posY, int sX, int sY) {
    const wchar_t className[] = L"Class";
    HINSTANCE handle = GetModuleHandle(NULL);

    WNDCLASS props = { 0 }; 
    props.style = 0;
    props.lpfnWndProc = evtHandler;
    props.hInstance = handle;
    props.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
    props.lpszClassName = className;

    
    if(!RegisterClass(&props)) {
        MessageBox(NULL, L"Unable to register window", L"Oopsie", MB_ICONSTOP | MB_OK);
        return NULL;
    }

    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,           // Extended Window Style
        className,                  // Class Name
        L"Thing",                   // Title
        WS_OVERLAPPEDWINDOW,        // Window Style
        posX, posY, sX, sY,         // Position and Size
        NULL, NULL, handle, NULL    // Handles
    );

    if(hwnd == NULL) {
        MessageBox(NULL, L"Can't create window??", L"Oopsie", MB_ICONERROR | MB_OK);
        return NULL;
    }
    return hwnd;
}