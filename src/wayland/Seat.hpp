#ifndef SRC_WAYLAND_SEAT_HPP_
#define SRC_WAYLAND_SEAT_HPP_

#include <memory>

#include "Keyboard.hpp"
#include "Log.hpp"
#include "Pointer.hpp"

#include "Registry.hpp"

namespace Wayland
{
    class Seat : public Registry
    {
    public:
        ~Seat();
        wl_seat* seat();
    private:
        friend class Display;

        Seat(wl_registry *registry, uint32_t id, uint32_t version);

        wl_seat *mSeat;
        XenBackend::Log mLog;

        Keyboard* mKeyboard;

        std::shared_ptr<Pointer> mPointer;

        void init();
        void release();
    };

} // namespace Wayland
#endif