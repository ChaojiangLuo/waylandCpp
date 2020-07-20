#include <iostream>

#include "wayland/Display.hpp"

#include "wayland/Activity.hpp"

using std::exception;
using std::shared_ptr;

using XenBackend::Log;

using Wayland::Display;
using Wayland::SharedBuffer;
using Wayland::SharedFile;
using Wayland::Surface;

shared_ptr<Surface> gSurface1;
shared_ptr<Surface> gSurface2;

shared_ptr<SharedFile> gSharedFile1;
shared_ptr<SharedBuffer> gSharedBuffer1;
shared_ptr<SharedFile> gSharedFile2;
shared_ptr<SharedBuffer> gSharedBuffer2;

int main()
{
	try
	{
		XenBackend::Log::setLogLevel("DEBUG");

		Activity activity(Rect(320, 240));
		activity.updateData(FormatInfo(0x00, 0x00, 0xFF, 0x00));
		activity.draw();

		std::string str;
		std::cin >> str;
	}
	catch (const exception &e)
	{
		LOG("Main", ERROR) << e.what();
	}

	return 0;
}
