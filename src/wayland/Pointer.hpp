#ifndef SRC_WAYLAND_POINTER_HPP_
#define SRC_WAYLAND_POINTER_HPP_

#include "Log.hpp"
#include <wayland-client.h>

namespace Wayland
{
    class Seat;

    class pointer_data
    {
        wl_surface *surface;
        wl_buffer *buffer;
        int32_t hot_spot_x;
        int32_t hot_spot_y;
        wl_surface *target_surface;
    };

    class Pointer
    {
    private:
        wl_pointer *mPointer;
        static const wl_pointer_listener s_listener;

        Seat *mSeat;
        XenBackend::Log mLog;

        void init();
        void release();

    public:
        Pointer(Seat *seat);
        ~Pointer();

        static void enterCallback(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface,
                                  wl_fixed_t sx, wl_fixed_t sy);
        static void leaveCallback(void *data, wl_pointer *pointer, uint32_t serial, wl_surface *surface);
        static void motionCallback(void *data, wl_pointer *pointer, uint32_t time, wl_fixed_t sx, wl_fixed_t sy);
        static void buttonCallback(void *data, wl_pointer *pointer, uint32_t serial, uint32_t time,
                                   uint32_t button, uint32_t state);
        static void axisCallback(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value);
        static void frameCallback(void *data, wl_pointer *pointer);
        static void axisSourceCallback(void *data, wl_pointer *pointer, uint32_t axis_source);
        static void axisStopCallback(void *data, wl_pointer *pointer, uint32_t time, uint32_t axis);
        static void axisDiscreteCallback(void *data, wl_pointer *pointer, uint32_t axis, int32_t discrete);
    };
} // namespace Wayland

#endif