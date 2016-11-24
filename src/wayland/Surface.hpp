/*
 * Surface.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#ifndef SRC_WAYLAND_SURFACE_HPP_
#define SRC_WAYLAND_SURFACE_HPP_

#include <wayland-client.h>

#include <xen/be/Log.hpp>

namespace Wayland {

class Surface
{
public:

	Surface(wl_compositor* compositor);
	~Surface();

	wl_surface* getWlSurface() const { return mSurface; }

public:

	wl_surface* mSurface;
	XenBackend::Log mLog;

	void init(wl_compositor* compositor);
	void release();
};

}

#endif /* SRC_WAYLAND_SURFACE_HPP_ */
