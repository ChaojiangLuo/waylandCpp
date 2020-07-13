#include <assert.h>

#include "Exception.hpp"
#include "Keyboard.hpp"
#include "Seat.hpp"

namespace Wayland
{
    Keyboard::Keyboard(Seat* seat) : mSeat(seat), mLog("Keyboard")
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

    void Keyboard::keymapCallback(void *data, wl_keyboard *keyboard, uint32_t format, int fd, uint32_t size)
    {
        auto k = reinterpret_cast<Keyboard *>(data);
        assert(k->mKeyboard == keyboard);
        if (format != WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1)
        {
            return;
        }

        DLOG(k->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << fd << " " << size;
        //emit k->q->keymapChanged(fd, size);
    }

    void Keyboard::enterCallback(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface, wl_array *keys)
    {
        auto k = reinterpret_cast<Keyboard *>(data);
        assert(k->mKeyboard == keyboard);

        DLOG(k->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << serial << " " << keys;
        //k->enter(serial, surface, keys);
    }

    void Keyboard::leaveCallback(void *data, wl_keyboard *keyboard, uint32_t serial, wl_surface *surface)
    {
        auto k = reinterpret_cast<Keyboard *>(data);
        assert(k->mKeyboard == keyboard);

        DLOG(k->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << serial;
        //k->leave(serial);
    }

    void Keyboard::keyCallback(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
    {
        auto k = reinterpret_cast<Keyboard *>(data);
        assert(k->mKeyboard == keyboard);
        auto toState = [state] {
            if (state == WL_KEYBOARD_KEY_STATE_RELEASED)
            {
                return KeyState::Released;
            }
            else
            {
                return KeyState::Pressed;
            }
        };
        DLOG(k->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << serial << " " << time << " " << key << " " << state;
        //emit k->q->keyChanged(key, toState(), time);
    }

    void Keyboard::modifiersCallback(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t modsDepressed,
                                     uint32_t modsLatched, uint32_t modsLocked, uint32_t group)
    {
        auto k = reinterpret_cast<Keyboard *>(data);
        assert(k->mKeyboard == keyboard);

        DLOG(k->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << serial << " " << modsDepressed << " " << modsLatched << " " << modsLocked << " " << group;
        //emit k->q->modifiersChanged(modsDepressed, modsLatched, modsLocked, group);
    }

    void Keyboard::repeatInfoCallback(void *data, wl_keyboard *keyboard, int32_t charactersPerSecond, int32_t delay)
    {
        auto k = reinterpret_cast<Keyboard *>(data);
        assert(k->mKeyboard == keyboard);

        DLOG(k->mLog, DEBUG) << "ut-gfx-wl:" << __func__ << " " << charactersPerSecond << " " << delay;
        /*
        k->repeatInfo.charactersPerSecond = qMax(charactersPerSecond, 0);
        k->repeatInfo.delay = qMax(delay, 0);
        emit k->q->keyRepeatChanged();
        */
    }

    const wl_keyboard_listener Keyboard::s_listener = {
        keymapCallback,
        enterCallback,
        leaveCallback,
        keyCallback,
        modifiersCallback,
        repeatInfoCallback
    };

    /*******************************************************************************
    * Private
    ******************************************************************************/
    void Keyboard::init()
    {
        mKeyboard = wl_seat_get_keyboard(mSeat->seat());

        wl_keyboard_add_listener(mKeyboard, &s_listener, this);

        LOG(mLog, DEBUG) << "Create";
    }

    void Keyboard::release()
    {
        if (mKeyboard) {
            wl_keyboard_release(mKeyboard);
            LOG(mLog, DEBUG) << "Delete";
        }
    }
} // namespace Wayland