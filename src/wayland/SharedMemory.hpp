/*
 * SharedMemory.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#ifndef SRC_WAYLAND_SHAREDMEMORY_HPP_
#define SRC_WAYLAND_SHAREDMEMORY_HPP_

#include <memory>

#include <xen/be/Log.hpp>

#include "Registry.hpp"
#include "SharedFile.hpp"

namespace Wayland {

class SharedMemory : public Registry
{
public:

	SharedMemory(wl_registry* registry, uint32_t id, uint32_t version);
	~SharedMemory();

	std::shared_ptr<SharedFile> createSharedFile(uint32_t width,
												 uint32_t height,
												 uint32_t bpp);

private:

	wl_shm* mSharedMemory;
	XenBackend::Log mLog;

	void init();
	void release();
};

}

#endif /* SRC_WAYLAND_SHAREDMEMORY_HPP_ */
