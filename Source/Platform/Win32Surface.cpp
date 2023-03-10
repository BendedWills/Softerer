#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Softerer/Platform/Win32Surface.hpp>

#include <stdexcept>

namespace Softerer::Platform
{
	Win32Surface::Win32FramebufferImage::Win32FramebufferImage(HBITMAP bitmap, Pixel* data, size_t width, size_t height)
		:
		FramebufferImage(data, width, height),
		m_Bitmap(bitmap)
	{}

	Win32Surface::Win32FramebufferImage::~Win32FramebufferImage()
	{
		DeleteObject(m_Bitmap);
	}

	Win32Surface::Win32Surface(HINSTANCE hinst, HWND hwnd)
		:
		m_Hinst(hinst),
		m_Hwnd(hwnd)
	{
		m_Hdc = GetDC(m_Hwnd);
		m_CompatHdc = CreateCompatibleDC(m_Hdc);
	}

	Win32Surface::~Win32Surface()
	{
		DeleteDC(m_CompatHdc);
		ReleaseDC(m_Hwnd, m_Hdc);
	}

	Scope<FramebufferImage> Win32Surface::CreateFramebufferImage(size_t width, 
		size_t height)
	{
		BITMAPINFO info{};
		info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biBitCount = sizeof(Image::Pixel) * 8;
		info.bmiHeader.biWidth = width;
		info.bmiHeader.biHeight = height;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biCompression = BI_RGB;

		Image::Pixel* data = nullptr;
		HBITMAP bitmap = CreateDIBSection(m_Hdc, &info, DIB_RGB_COLORS, 
			(void**)&data, NULL, NULL);
		if (!data)
			throw std::runtime_error("Failed to create bitmap");

		return std::make_unique<Win32FramebufferImage>(bitmap, data, width, height);
	}

	void Win32Surface::Present(const FramebufferImage& framebuffer)
	{
		const Win32FramebufferImage& win32Image = 
			(const Win32FramebufferImage&)framebuffer;

		SelectObject(m_CompatHdc, win32Image.m_Bitmap);
		BitBlt(m_Hdc, 0, 0, win32Image.GetWidth(), win32Image.GetHeight(),
			m_CompatHdc, 0, 0, SRCCOPY);
	}
}