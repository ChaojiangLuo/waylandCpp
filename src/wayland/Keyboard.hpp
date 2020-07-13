#ifndef SRC_WAYLAND_KEYBOARD_HPP_
#define SRC_WAYLAND_KEYBOARD_HPP_

#include "Log.hpp"
#include <wayland-client.h>

namespace Wayland
{
    class Seat;
    class Keyboard
    {
    private:
        wl_keyboard *mKeyboard;
        static const wl_keyboard_listener s_listener;

        Seat* mSeat;
        XenBackend::Log mLog;

        void init();
        void release();
    public:
        enum class KeyState {
            Released,
            Pressed
        };

        Keyboard(Seat* seat);
        ~Keyboard();

        static void keymapCallback(void *data, wl_keyboard *keyboard, uint32_t format, int fd, uint32_t size);
        static void enterCallback(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface, wl_array *keys);
        static void leaveCallback(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface);
        static void keyCallback(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
        static void modifiersCallback(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t modsDepressed,
                                      uint32_t modsLatched, uint32_t modsLocked, uint32_t group);
        static void repeatInfoCallback(void *data, wl_keyboard *keyboard, int32_t charactersPerSecond, int32_t delay);
    };
} // namespace Wayland

#endif