/*
 * Display.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#include "Display.hpp"

#include "Exception.hpp"

using std::shared_ptr;

namespace Wayland {

/*******************************************************************************
 * Display
 ******************************************************************************/

Display::Display() :
	mDisplay(nullptr),
	mRegistry(nullptr),
	mLog("Display")
{
	try
	{
		init();
	}
	catch(const WlException& e)
	{
		release();

		throw;
	}
}

Display::~Display()
{
	release();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

shared_ptr<Compositor> Display::getCompositor() const
{
	if (!mCompositor)
	{
		throw WlException("Can't get compositor");
	}

	return mCompositor;
}

shared_ptr<Shell> Display::getShell() const
{
	if (!mShell)
	{
		throw WlException("Can't get shell");
	}

	return mShell;
}

std::shared_ptr<SharedMemory> Display::getSharedMemory() const
{
	if (!mSharedMemory)
	{
		throw WlException("Can't get shared memory");
	}

	return mSharedMemory;
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void Display::sRegistryHandler(void *data, wl_registry *registry, uint32_t id,
							   const char *interface, uint32_t version)
{
	static_cast<Display*>(data)->registryHandler(registry, id, interface,
												 version);
}

void Display::sRegistryRemover(void *data, struct wl_registry *registry,
							   uint32_t id)
{
	static_cast<Display*>(data)->registryRemover(registry, id);
}

void Display::registryHandler(wl_registry *registry, uint32_t id,
							  const std::string& interface, uint32_t version)
{
	LOG(mLog, DEBUG) << "Registry event, itf: " << interface << ", id: " << id
					 << ", version: " << version;

	if (interface == "wl_compositor")
	{
		mCompositor.reset(new Compositor(registry, id, version));
	}

	if (interface == "wl_shell")
	{
		mShell.reset(new Shell(registry, id, version));
	}

	if (interface == "wl_shm")
	{
		mSharedMemory.reset(new SharedMemory(registry, id, version));
	}
}

void Display::registryRemover(wl_registry *registry, uint32_t id)
{
	LOG(mLog, DEBUG) << "Registry removed event, id: " << id;

	if (mCompositor && id == mCompositor->getId())
	{
		mCompositor.reset();
	}

	if (mShell && id == mShell->getId())
	{
		mShell.reset();
	}

	if (mSharedMemory && id == mSharedMemory->getId())
	{
		mSharedMemory.reset();
	}
}

void Display::init()
{
	mDisplay = wl_display_connect(nullptr);

	if (!mDisplay)
	{
		throw WlException("Can't connect to display");
	}

	LOG(mLog, DEBUG) << "Connected";

	mRegistryListener = {sRegistryHandler, sRegistryRemover};

	mRegistry = wl_display_get_registry(mDisplay);

	if (!mRegistry)
	{
		throw WlException("Can't get registry");
	}

	wl_registry_add_listener(mRegistry, &mRegistryListener, this);

	wl_display_dispatch(mDisplay);
	wl_display_roundtrip(mDisplay);
}

void Display::release()
{
	if (mRegistry)
	{
		wl_registry_destroy(mRegistry);
	}

	if (mDisplay)
	{
		wl_display_flush(mDisplay);
		wl_display_disconnect(mDisplay);

		LOG(mLog, DEBUG) << "Disconnected";
	}
}

}
