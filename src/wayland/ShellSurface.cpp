/*
 * ShellSurface.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#include "ShellSurface.hpp"

#include "Exception.hpp"

using std::shared_ptr;

namespace Wayland {

/*******************************************************************************
 * ShellSurface
 ******************************************************************************/

ShellSurface::ShellSurface(wl_shell* shell, shared_ptr<Surface> surface) :
	mShellSurface(nullptr),
	mSurfacePtr(surface),
	mLog("ShellSurface")
{
	try
	{
		init(shell);
	}
	catch(const WlException& e)
	{
		release();

		throw;
	}
}

ShellSurface::~ShellSurface()
{
	release();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

void ShellSurface::setTopLevel()
{
	LOG(mLog, DEBUG) << "Set top level";

	wl_shell_surface_set_toplevel(mShellSurface);
}

/*******************************************************************************
 * Private
 ******************************************************************************/

void ShellSurface::init(wl_shell* shell)
{
	mShellSurface = wl_shell_get_shell_surface(shell,
											   mSurfacePtr->getWlSurface());

	if (!mShellSurface)
	{
		throw WlException("Can't create shell surface");
	}

	LOG(mLog, DEBUG) << "Create";
}

void ShellSurface::release()
{
	if (mShellSurface)
	{
		wl_shell_surface_destroy(mShellSurface);

		LOG(mLog, DEBUG) << "Delete";
	}
}

}
