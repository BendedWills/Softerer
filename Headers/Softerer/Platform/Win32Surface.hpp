#pragma once

#include <Softerer/Surface.hpp>

#ifdef __INTELLISENSE__
#include <Windows.h>
#endif

namespace Softerer::Platform
{
	class Win32Surface : public Surface
	{
	public:
		Win32Surface(HINSTANCE hinst, HWND hwnd);
		~Win32Surface();

		Scope<FramebufferImage> CreateFramebufferImage(size_t width, size_t height) 
			override;
		void Present(const FramebufferImage& framebuffer) override;
	private:
		struct Win32FramebufferImage : public FramebufferImage
		{
			Win32FramebufferImage(HBITMAP bitmap, Pixel* data, size_t width,
				size_t height);
			~Win32FramebufferImage();
			
			HBITMAP m_Bitmap = nullptr;
		};

		HINSTANCE m_Hinst = nullptr;
		HWND m_Hwnd = nullptr;

		HDC m_Hdc = nullptr;
		HDC m_CompatHdc = nullptr;
	};
}