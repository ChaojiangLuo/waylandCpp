/*
 * Surface.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#include "Surface.hpp"

#include "Exception.hpp"

namespace Wayland {

/*******************************************************************************
 * Surface
 ******************************************************************************/

Surface::Surface(wl_compositor* compositor) :
	mSurface(nullptr),
	mLog("Surface")
{
	try
	{
		init(compositor);
	}
	catch(const WlException& e)
	{
		release();

		throw;
	}
}

Surface::~Surface()
{
	release();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

/*******************************************************************************
 * Private
 ******************************************************************************/

void Surface::init(wl_compositor* compositor)
{
	mSurface = wl_compositor_create_surface(compositor);

	if (!mSurface)
	{
		throw WlException("Can't create surface");
	}

	LOG(mLog, DEBUG) << "Create";
}

void Surface::release()
{
	if (mSurface)
	{
		wl_surface_destroy(mSurface);

		LOG(mLog, DEBUG) << "Delete";
	}
}

}
