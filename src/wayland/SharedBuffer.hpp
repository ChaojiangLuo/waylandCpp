/*
 * SharedBuffer.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#ifndef SRC_WAYLAND_SHAREDBUFFER_HPP_
#define SRC_WAYLAND_SHAREDBUFFER_HPP_

#include <memory>

#include <wayland-client.h>

#include <xen/be/Log.hpp>

namespace Wayland {

class SharedBuffer
{
public:

	SharedBuffer(wl_shm* sharedMemory, int fd, uint32_t width, uint32_t height,
				 uint32_t stride, uint32_t pixelFormat);

	~SharedBuffer();

	wl_buffer* getBuffer() const { return mBuffer; }

private:
	wl_buffer* mBuffer;
	wl_shm_pool* mPool;
	XenBackend::Log mLog;

	void init(wl_shm* sharedMemory, int fd, uint32_t width, uint32_t height,
			  uint32_t stride, uint32_t pixelFormat);
	void release();
};

}

#endif /* SRC_WAYLAND_SHAREDBUFFER_HPP_ */
