/*
 * ShellSurface.hpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#ifndef SRC_WAYLAND_SHELLSURFACE_HPP_
#define SRC_WAYLAND_SHELLSURFACE_HPP_

#include <memory>

#include <wayland-client.h>

#include "Log.hpp"

#include "Surface.hpp"
#include "Seat.hpp"

namespace Wayland
{

	class ShellSurface
	{
	public:
		~ShellSurface();

		void setTopLevel();

		void setTitle(std::string name);

		void pong(uint32_t serial);
		void move(Seat *seat, uint32_t serial);
		void resize(Seat *seat, uint32_t serial, uint32_t edges);
		void setTransient(Surface *parent, int32_t x, int32_t y, uint32_t flags);
		void setFullscreen(uint32_t method, uint32_t framerate,
						   struct wl_output *output);
		void setPopup(Seat *seat, uint32_t serial, Surface *parent, int32_t x,
					  int32_t y, uint32_t flags);
		void setMaximized(struct wl_output *output);
		void setClass(const char *class_);

		std::shared_ptr<Surface> getSurface() const { return mSurfacePtr; }

	public:
		ShellSurface(wl_shell *shell, std::shared_ptr<Surface> surface);

		wl_shell_surface *mShellSurface;
		std::shared_ptr<Surface> mSurfacePtr;
		wl_shell_surface_listener mListener;

		XenBackend::Log mLog;

		static void sPingHandler(void *data, wl_shell_surface *shell_surface,
								 uint32_t serial);
		static void sConfigHandler(void *data, wl_shell_surface *shell_surface,
								   uint32_t edges, int32_t width, int32_t height);
		static void sPopupDone(void *data, wl_shell_surface *shell_surface);

		void pingHandler(uint32_t serial);
		void configHandler(uint32_t edges, int32_t width, int32_t height);
		void popupDone();

		void init(wl_shell *shell);
		void release();
	};

} // namespace Wayland

#endif /* SRC_WAYLAND_SHELLSURFACE_HPP_ */
