#include <iostream>

#include "wayland/Display.hpp"

using std::exception;
using std::shared_ptr;

using XenBackend::Log;

using Wayland::Display;
using Wayland::Surface;
using Wayland::SharedBuffer;

shared_ptr<Surface> gSurface1;
shared_ptr<SharedBuffer> gSharedBuffer1;
shared_ptr<Surface> gSurface2;
shared_ptr<SharedBuffer> gSharedBuffer2;

void drawFrame()
{
	try
	{
		LOG("Main", DEBUG) << "Draw frame";

		gSurface1->draw(gSharedBuffer1, drawFrame);
	}
	catch(const exception& e)
	{
		LOG("Main", ERROR) << e.what();
	}
}

int main()
{
	try
	{
		XenBackend::Log::setLogLevel("DEBUG");

		Display display;

		display.start();

		gSurface1 = display.getCompositor()->createSurface();

		auto shellSurface1 = display.getShell()->getShellSurface(gSurface1);

		shellSurface1->setTopLevel();

		auto sharedFile = display.getSharedMemory()->
				createSharedFile(320, 240, 32);

		gSharedBuffer1 = display.getSharedMemory()->
				createSharedBuffer(sharedFile, 320, 240,
								  sharedFile->getStride(),
								  WL_SHM_FORMAT_XRGB8888);

		LOG("Main", DEBUG) << "Buffer size: " << sharedFile->getSize();

		drawFrame();

		std::string str;
		std::cin >> str;
	}
	catch(const exception& e)
	{
		LOG("Main", ERROR) << e.what();
	}

	return 0;
}

