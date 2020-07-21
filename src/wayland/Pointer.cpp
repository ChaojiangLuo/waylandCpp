#include <assert.h>

#include "Exception.hpp"
#include "Pointer.hpp"
#include "Seat.hpp"

namespace Wayland
{
    Pointer::Pointer(Seat *seat) : mSeat(seat), mLog("Pointer")
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

    Pointer::~Pointer()
    {
        release();
    }

    void Pointer::enterCallback(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface,
                                wl_fixed_t sx, wl_fixed_t sy)
    {
        auto p = reinterpret_cast<Pointer *>(data);
        assert(p->mPointer == pointer);

        DLOG(p->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << serial << " " << sx << " " << sy;
    }

    void Pointer::leaveCallback(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface)
    {
        auto p = reinterpret_cast<Pointer *>(data);
        assert(p->mPointer == pointer);

        DLOG(p->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << serial;
    }

    void Pointer::motionCallback(void *data, wl_pointer *pointer, uint32_t time, wl_fixed_t sx, wl_fixed_t sy)
    {
        auto p = reinterpret_cast<Pointer *>(data);
        assert(p->mPointer == pointer);

        DLOG(p->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << time << " " << sx << " " << sy;
    }

    void Pointer::buttonCallback(void *data, wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state)
    {
        auto p = reinterpret_cast<Pointer *>(data);
        assert(p->mPointer == pointer);

        DLOG(p->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << serial << " " << time << " " << button << " " << state;

        p->buttonStateChanged(serial, time, button, state == WL_POINTER_BUTTON_STATE_RELEASED);
    }

    void Pointer::axisCallback(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value)
    {
        auto p = reinterpret_cast<Pointer *>(data);
        assert(p->mPointer == pointer);

        DLOG(p->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << time << " " << axis << " " << value;
    }

    void Pointer::frameCallback(void *data, wl_pointer *pointer)
    {
        auto p = reinterpret_cast<Pointer *>(data);
        assert(p->mPointer == pointer);

        DLOG(p->mLog, DEBUG) << "ut-gfx-wl:" << __func__;
    }

    void Pointer::axisSourceCallback(void *data, wl_pointer *pointer, uint32_t axis_source)
    {
        auto p = reinterpret_cast<Pointer *>(data);
        assert(p->mPointer == pointer);

        DLOG(p->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << axis_source;
    }

    void Pointer::axisStopCallback(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis)
    {
        auto p = reinterpret_cast<Pointer *>(data);
        assert(p->mPointer == pointer);

        DLOG(p->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << time << " " << axis;
    }

    void Pointer::axisDiscreteCallback(void *data, wl_pointer *pointer, uint32_t axis, int32_t discrete)
    {
        auto p = reinterpret_cast<Pointer *>(data);
        assert(p->mPointer == pointer);

        DLOG(p->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << discrete << " " << axis;
    }

    int Pointer::addPointerListener(std::string name, PointerListener *listener)
    {
        int ret = 0;
        if (mListerMap.find(name) != mListerMap.end())
        {
            mListerMap.erase(name);
            ret = 1;
        }
        mListerMap.emplace(name, listener);
        return ret;
    }

    void Pointer::buttonStateChanged(uint32_t serial, uint32_t time, uint32_t button, bool pressed)
    {
        for (auto iter = mListerMap.begin(); iter != mListerMap.end(); ++iter)
        {
            iter->second->buttonStateChanged(serial, time, button, pressed);
        }
    }

    /*******************************************************************************
    * Private
    ******************************************************************************/
    void Pointer::init()
    {
        mPointer = wl_seat_get_pointer(mSeat->seat());

        if (!mPointer)
        {
            LOG(mLog, DEBUG) << "wl_seat_get_pointer failed";
            return;
        }

        wl_pointer_add_listener(mPointer, &s_listener, this);

        LOG(mLog, DEBUG) << "Create";
    }

    void Pointer::release()
    {
        if (mPointer)
        {
            wl_pointer_release(mPointer);
            LOG(mLog, DEBUG) << "Delete";
        }
    }

    const wl_pointer_listener Pointer::s_listener = {
        enterCallback,
        leaveCallback,
        motionCallback,
        buttonCallback,
        axisCallback,
        frameCallback,
        axisSourceCallback,
        axisStopCallback,
        axisDiscreteCallback};

} // namespace Wayland
