/*
 * SharedBuffer.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#ifndef SRC_WAYLAND_SHAREDBUFFER_HPP_
#define SRC_WAYLAND_SHAREDBUFFER_HPP_

#include <xen/be/Log.hpp>

#include "Registry.hpp"

namespace Wayland {

class SharedMemory : public Registry
{
public:

	SharedMemory(wl_registry* registry, uint32_t id, uint32_t version);
	~SharedMemory();

private:

	wl_shm* mSharedMemory;
	XenBackend::Log mLog;

	void init();
	void release();
};

}

#endif /* SRC_WAYLAND_SHAREDBUFFER_HPP_ */
