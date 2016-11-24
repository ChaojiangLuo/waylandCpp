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

	}
	catch(const exception& e)
	{
		LOG("Main", ERROR) << e.what();
	}

	return 0;
}

