#ifndef SRC_WAYLAND_POINTER_HPP_
#define SRC_WAYLAND_POINTER_HPP_

#include <list>
#include <unordered_map>

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

    class PointerListener
    {
    public:
        virtual void pointerEnter(uint32_t serial, wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy) = 0;
        virtual void pointerLeave(uint32_t serial, wl_surface *surface) = 0;
        virtual void pointerMotion(uint32_t time, wl_fixed_t sx, wl_fixed_t sy) = 0;
        virtual void pointerButton(uint32_t serial, uint32_t time, uint32_t button, bool pressed) = 0;
        virtual void pointerAxis(uint32_t time, uint32_t axis, wl_fixed_t value) = 0;
        virtual void pointerFrame() = 0;
        virtual void pointerAxisSource(uint32_t axis_source) = 0;
        virtual void pointerAxisStop(uint32_t time, uint32_t axis) = 0;
        virtual void pointerAxisDiscrete(uint32_t axis, int32_t discrete) = 0;
    };

    class Pointer
    {
    private:
        wl_pointer *mPointer;
        static const wl_pointer_listener s_listener;

        Seat *mSeat;
        XenBackend::Log mLog;

        uint32_t mPointerSerial;

        void init();
        void release();

        std::unordered_map<std::string, PointerListener *> mListerMap;

    public:
        Pointer(Seat *seat);
        ~Pointer();

        uint32_t serial()
        {
            return mPointerSerial;
        }

        void enter(uint32_t serial, wl_surface *surface, wl_fixed_t sx, wl_fixed_t sy);
        void leave(uint32_t serial, wl_surface *surface);
        void motion(uint32_t time, wl_fixed_t sx, wl_fixed_t sy);
        void button(uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
        void axis(uint32_t time, uint32_t axis, wl_fixed_t value);
        void frame();
        void axisSource(uint32_t axis_source);
        void axisStop(uint32_t time, uint32_t axis);
        void axisDiscrete(uint32_t axis, int32_t discrete);

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

        int addPointerListener(std::string name, PointerListener *listener);
    };
} // namespace Wayland

#endif