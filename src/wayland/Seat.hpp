#ifndef SRC_WAYLAND_SEAT_HPP_
#define SRC_WAYLAND_SEAT_HPP_

#include "Log.hpp"

#include "Registry.hpp"

namespace Wayland
{
    class Seat : public Registry
    {
    public:
        ~Seat();

    private:
        friend class Display;

        Seat(wl_registry *registry, uint32_t id, uint32_t version);

        wl_seat *mSeat;
        XenBackend::Log mLog;

        void init();
        void release();
    };

} // namespace Wayland
#endif