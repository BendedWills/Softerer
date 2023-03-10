#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Softerer/Platform/Win32Surface.hpp>

#include <stdexcept>

namespace Softerer::Platform
{
	Win32Surface::Win32Surface(HINSTANCE hinst, HWND hwnd)
		:
		m_Hinst(hinst),
		m_Hwnd(hwnd)
	{
		m_Hdc = GetDC(m_Hwnd);
	}

	Win32Surface::~Win32Surface()
	{
		ReleaseDC(m_Hwnd, m_Hdc);
	}

	void Win32Surface::Present(const FramebufferImage& framebuffer)
	{
		BitBlt(m_Hdc, 0, 0, framebuffer.GetWidth(), framebuffer.GetHeight(),
			(HDC)framebuffer.GetBitmapRendererHandle(), 0, 0, SRCCOPY);
	}
}