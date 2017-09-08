#pragma once
#include <WS2tcpip.h>

class Registry {
public:
	static VOID Clean();
	static VOID Position_Create();
	static VOID Position_Delete();
	static POINT Position_Get();
	static VOID Startup_Create();
	static VOID Startup_Delete();
	static LSTATUS Startup_Status();
private:
};