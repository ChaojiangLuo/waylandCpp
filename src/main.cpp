#include <iostream>

#include "wayland/Display.hpp"

using std::exception;
using std::shared_ptr;

using XenBackend::Log;

using Wayland::Display;
using Wayland::Surface;
using Wayland::SharedBuffer;
using Wayland::SharedFile;

shared_ptr<Surface> gSurface1;
shared_ptr<Surface> gSurface2;

shared_ptr<SharedFile> gSharedFile1;
shared_ptr<SharedBuffer> gSharedBuffer1;
shared_ptr<SharedFile> gSharedFile2;
shared_ptr<SharedBuffer> gSharedBuffer2;

void drawFrame(shared_ptr<SharedFile> sharedFile1,
			   shared_ptr<SharedBuffer> sharedBuffer1,
			   shared_ptr<SharedFile> sharedFile2,
			   shared_ptr<SharedBuffer> sharedBuffer2)
{
	try
	{
		gSurface1->draw(sharedBuffer1 , std::bind(drawFrame,
												  sharedFile2, sharedBuffer2,
												  sharedFile1, sharedBuffer1));
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
		gSurface2 = display.getCompositor()->createSurface();

		auto shellSurface1 = display.getShell()->getShellSurface(gSurface1);
		auto shellSurface2 = display.getShell()->getShellSurface(gSurface2);

		shellSurface1->setTopLevel();
		shellSurface2->setTopLevel();

		gSharedFile1 = display.getSharedMemory()->
				createSharedFile(320, 240, 32);

		gSharedBuffer1 = display.getSharedMemory()->
								  createSharedBuffer(gSharedFile1, 320, 240,
										  gSharedFile1->getStride(),
								  WL_SHM_FORMAT_XRGB8888);

		gSharedFile2 = display.getSharedMemory()->
				createSharedFile(320, 240, 32);

		gSharedBuffer2 = display.getSharedMemory()->
								  createSharedBuffer(gSharedFile2, 320, 240,
										  gSharedFile2->getStride(),
								  WL_SHM_FORMAT_XRGB8888);

		LOG("Main", DEBUG) << "Buffer size: " << gSharedFile1->getSize();

		struct Rgb
		{
			uint8_t x;
			uint8_t r;
			uint8_t g;
			uint8_t b;
		};

		Rgb* data1 = static_cast<Rgb*>(gSharedFile1->getBuffer());
		Rgb* data2 = static_cast<Rgb*>(gSharedFile2->getBuffer());
		uint8_t value = 0;

		for (size_t i = 0; i < gSharedFile1->getSize() / sizeof(Rgb); i++)
		{
			data1[i].x = 0x00;
			data1[i].r = 0x00;
			data1[i].g = 0xFF;
			data1[i].b = 0x00;
		}

		for (size_t i = 0; i < gSharedFile2->getSize() / sizeof(Rgb); i++)
		{
			data2[i].x = 0x00;
			data2[i].r = 0xFF;
			data2[i].g = 0x00;
			data2[i].b = 0x00;
		}

		drawFrame(gSharedFile1, gSharedBuffer1, gSharedFile1, gSharedBuffer1);

		gSurface2->draw(gSharedBuffer2);

		std::string str;
		std::cin >> str;
	}
	catch(const exception& e)
	{
		LOG("Main", ERROR) << e.what();
	}

	return 0;
}

