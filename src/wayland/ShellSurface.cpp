/*
 * ShellSurface.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: al1
 */

#include "ShellSurface.hpp"

#include "Exception.hpp"

using std::shared_ptr;

namespace Wayland
{

	/*******************************************************************************
 * ShellSurface
 ******************************************************************************/

	ShellSurface::ShellSurface(wl_shell *shell, shared_ptr<Surface> surface) : mShellSurface(nullptr),
																			   mSurfacePtr(surface),
																			   mLog("ShellSurface")
	{
		try
		{
			init(shell);
		}
		catch (const WlException &e)
		{
			LOG(mLog, DEBUG) << "init failed " << e.what();
			release();

			throw;
		}
	}

	ShellSurface::~ShellSurface()
	{
		release();
	}

	/*******************************************************************************
 	* Public
 	******************************************************************************/
	void ShellSurface::setTopLevel()
	{
		LOG(mLog, DEBUG) << "Set top level";

		wl_shell_surface_set_toplevel(mShellSurface);
	}

	void ShellSurface::setTitle(std::string name)
	{
		LOG(mLog, DEBUG) << __func__ << "ut-gfx " << name.c_str();
		wl_shell_surface_set_title(mShellSurface, name.c_str());
	}

	void ShellSurface::pong(uint32_t serial)
	{
		LOG(mLog, DEBUG) << __func__ << "ut-gfx " << serial;
		wl_shell_surface_pong(mShellSurface, serial);
	}

	void ShellSurface::move(Seat *seat, uint32_t serial)
	{
		wl_shell_surface_move(mShellSurface, seat->seat(), serial);
	}

	void ShellSurface::resize(Seat *seat, uint32_t serial, uint32_t edges)
	{
		wl_shell_surface_resize(mShellSurface, seat->seat(), serial, edges);
	}

	void ShellSurface::setTransient(Surface *parent, int32_t x, int32_t y,
									uint32_t flags)
	{
		wl_shell_surface_set_transient(mShellSurface, parent->surface(), x, y, flags);
	}

	void ShellSurface::setFullscreen(uint32_t method, uint32_t framerate,
									 struct wl_output *output)
	{
		wl_shell_surface_set_fullscreen(mShellSurface, method, framerate, output);
	}

	void ShellSurface::setPopup(Seat *seat, uint32_t serial, Surface *parent,
								int32_t x, int32_t y, uint32_t flags)
	{
		wl_shell_surface_set_popup(mShellSurface, seat->seat(), serial, parent->surface(), x, y,
								   flags);
	}

	void ShellSurface::setMaximized(struct wl_output *output)
	{
		wl_shell_surface_set_maximized(mShellSurface, output);
	}

	void ShellSurface::setClass(const char *class_)
	{
		wl_shell_surface_set_class(mShellSurface, class_);
	}

	/*******************************************************************************
 * Private
 ******************************************************************************/
	void ShellSurface::sPingHandler(void *data, wl_shell_surface *shell_surface,
									uint32_t serial)
	{
		static_cast<ShellSurface *>(data)->pingHandler(serial);
	}

	void ShellSurface::sConfigHandler(void *data, wl_shell_surface *shell_surface,
									  uint32_t edges, int32_t width, int32_t height)
	{
		static_cast<ShellSurface *>(data)->configHandler(edges, width, height);
	}

	void ShellSurface::sPopupDone(void *data, wl_shell_surface *shell_surface)
	{
		static_cast<ShellSurface *>(data)->popupDone();
	}

	void ShellSurface::pingHandler(uint32_t serial)
	{
		DLOG(mLog, DEBUG) << "Ping handler: " << serial;

		wl_shell_surface_pong(mShellSurface, serial);
	}

	void ShellSurface::configHandler(uint32_t edges, int32_t width, int32_t height)
	{
		DLOG(mLog, DEBUG) << "Config handler, edges: " << edges
						  << ", width: " << width << ", height: " << height;
	}

	void ShellSurface::popupDone()
	{
		DLOG(mLog, DEBUG) << "Popup done";
	}

	void ShellSurface::init(wl_shell *shell)
	{
		mShellSurface = wl_shell_get_shell_surface(shell, mSurfacePtr->mSurface);

		if (!mShellSurface)
		{
			throw WlException("Can't create shell surface");
		}

		mListener = {sPingHandler, sConfigHandler, sPopupDone};

		if (wl_shell_surface_add_listener(mShellSurface, &mListener, this) < 0)
		{
			throw WlException("Can't add listener");
		}

		LOG(mLog, DEBUG) << "Create";
	}

	void ShellSurface::release()
	{
		if (mShellSurface)
		{
			wl_shell_surface_destroy(mShellSurface);

			LOG(mLog, DEBUG) << "Delete";
		}
	}

} // namespace Wayland
