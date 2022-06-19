#pragma once
#include <WS2tcpip.h>
template<class Derived>
class Dialog;

template<class Derived>
class Dialog {
private:
    Dialog() {};
public:
    static void Destroy();
    static HWND Handle_Get();
    static void Handle_Set(HWND Handle);
private:
    static HWND Handle_;
};

template<class Derived>
HWND Dialog<Derived>::Handle_ = NULL;

//public:

template<class Derived>
void Dialog<Derived>::Destroy() {
    DestroyWindow(Handle_);
}

template<class Derived>
HWND Dialog<Derived>::Handle_Get() {
    return Handle_;
}

template<class Derived>
void Dialog<Derived>::Handle_Set(HWND Handle) {
    Handle_ = Handle;
}
