/*
 * Compositor.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#ifndef SRC_WAYLAND_COMPOSITOR_HPP_
#define SRC_WAYLAND_COMPOSITOR_HPP_

#include <memory>

#include <xen/be/Log.hpp>

#include "Registry.hpp"
#include "Surface.hpp"

namespace Wayland {

class Compositor : public Registry
{
public:

	Compositor(wl_registry* registry, uint32_t id, uint32_t version);
	~Compositor();

	std::shared_ptr<Surface> createSurface();

private:
	wl_compositor* mCompositor;
	XenBackend::Log mLog;

	void init();
	void release();
};

}

#endif /* SRC_WAYLAND_COMPOSITOR_HPP_ */
