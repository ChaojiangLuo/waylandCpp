/*
 * ShellSurface.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#ifndef SRC_WAYLAND_SHELLSURFACE_HPP_
#define SRC_WAYLAND_SHELLSURFACE_HPP_

#include <memory>

#include <wayland-client.h>

#include <xen/be/Log.hpp>

#include "Surface.hpp"

namespace Wayland {

class ShellSurface
{
public:

	ShellSurface(wl_shell* shell, std::shared_ptr<Surface> surface);
	~ShellSurface();

	void setTopLevel();

public:

	wl_shell_surface* mShellSurface;
	std::shared_ptr<Surface> mSurfacePtr;

	XenBackend::Log mLog;

	void init(wl_shell* shell);
	void release();
};

}

#endif /* SRC_WAYLAND_SHELLSURFACE_HPP_ */
