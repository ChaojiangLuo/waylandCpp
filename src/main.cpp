#include <iostream>

#include "wayland/Display.hpp"

using std::exception;

using XenBackend::Log;

using Wayland::Display;

int main()
{
	try
	{
		XenBackend::Log::setLogLevel("DEBUG");

		Display display;

		auto surface = display.getCompositor()->createSurface();

		auto shellSurface = display.getShell()->getShellSurface(surface);

		auto sharedFile = display.getSharedMemory()->createSharedFile(320, 240, 32);

		std::string str;
		std::cin >> str;
	}
	catch(const exception& e)
	{
		LOG("Main", ERROR) << e.what();
	}

	return 0;
}

