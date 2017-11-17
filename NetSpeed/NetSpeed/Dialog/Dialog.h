#pragma once
#include <WS2tcpip.h>
template<class Derived>
class Dialog;

template<class Derived>
class Dialog {
public:
	static VOID	Destroy();
	static HWND Handle_Get();
	static VOID Handle_Set(HWND Handle);
private:
	static HWND Handle_;
};

template<class Derived>
HWND	Dialog<Derived>::Handle_ = NULL;

//public:

template<class Derived>
VOID Dialog<Derived>::Destroy() {
	DestroyWindow(Handle_);
}

template<class Derived>
HWND Dialog<Derived>::Handle_Get() {
	return Handle_;
}

template<class Derived>
VOID Dialog<Derived>::Handle_Set(HWND Handle) {
	Handle_ = Handle;
}
