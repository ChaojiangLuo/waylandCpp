#include <assert.h>

#include "Exception.hpp"
#include "Seat.hpp"

namespace Wayland
{
    Seat::Seat(wl_registry *registry, uint32_t id, uint32_t version) : Registry(registry, id, version),
                                                                       mSeat(nullptr),
                                                                       mLog("Seat")
    {
        try
        {
            init();
        }
        catch (const WlException &e)
        {
            release();

            throw;
        }
    }

    Seat::~Seat()
    {
        release();
    }

    wl_seat *Seat::seat()
    {
        return mSeat;
    }

    /*******************************************************************************
    * Private
    ******************************************************************************/
    void Seat::init()
    {
        mSeat = static_cast<wl_seat *>(
            wl_registry_bind(getRegistry(), getId(),
                             &wl_seat_interface, getVersion()));

        if (!mSeat)
        {
            throw WlException("Can't bind seat");
        }

        mKeyboard = new Keyboard(this);

        mPointer.reset(new Pointer(this));

        LOG(mLog, DEBUG) << "Create";
    }

    void Seat::release()
    {
        if (mKeyboard)
        {
            delete mKeyboard;
            mKeyboard = nullptr;
        }

        if (mPointer)
        {
            mPointer.reset();
        }

        if (mSeat)
        {
            wl_seat_destroy(mSeat);

            LOG(mLog, DEBUG) << "Delete";
        }
    }
} // namespace Wayland