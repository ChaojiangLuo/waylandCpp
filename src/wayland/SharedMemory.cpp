/*
 * SharedMemory.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#include "SharedMemory.hpp"

#include "Exception.hpp"

namespace Wayland {

/*******************************************************************************
 * SharedMemory
 ******************************************************************************/

SharedMemory::SharedMemory(wl_registry* registry, uint32_t id, uint32_t version) :
	Registry(registry, id, version),
	mSharedMemory(nullptr),
	mLog("SharedMemory")
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

SharedMemory::~SharedMemory()
{
	release();
}

/*******************************************************************************
 * Public
 ******************************************************************************/

/*******************************************************************************
 * Private
 ******************************************************************************/

void SharedMemory::init()
{
	mSharedMemory = static_cast<wl_shm*>(
			wl_registry_bind(getRegistry(), getId(),
							 &wl_shm_interface, getVersion()));

	if (!mSharedMemory)
	{
		throw WlException("Can't bind shared memory");
	}

	LOG(mLog, DEBUG) << "Create";
}

void SharedMemory::release()
{
	if (mSharedMemory)
	{
		wl_shm_destroy(mSharedMemory);

		LOG(mLog, DEBUG) << "Delete";
	}
}

}
