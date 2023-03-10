#include <Softerer/FramebufferImage.hpp>

#include <stdexcept>
#include <Windows.h>

namespace Softerer
{
	FramebufferImage::FramebufferImage(size_t width, size_t height)
		:
		Image(width, height)
	{
		HDC nullHdc = GetDC(NULL);
		HDC hdc = CreateCompatibleDC(nullHdc);
		ReleaseDC(NULL, nullHdc);

		BITMAPINFO info{};
		info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biBitCount = sizeof(Image::Pixel) * 8;
		info.bmiHeader.biWidth = width;
		info.bmiHeader.biHeight = height;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biCompression = BI_RGB;

		m_InternalHandle = (void*)CreateDIBSection(hdc, &info, 
			DIB_RGB_COLORS, (void**)&m_ImageData, NULL, NULL);
		if (!m_ImageData || !m_InternalHandle)
			throw std::runtime_error("Failed to create bitmap");

		SelectObject(hdc, (HGDIOBJ)m_InternalHandle);

		m_BitmapRendererHandle = hdc;
	}

	FramebufferImage::~FramebufferImage()
	{
		if (!m_InternalHandle)
			return;

		DeleteObject((HGDIOBJ)m_InternalHandle);
		DeleteDC((HDC)m_BitmapRendererHandle);
	}

	FramebufferImage::FramebufferImage(FramebufferImage&& other)
		:
		Image(other.GetWidth(), other.GetHeight())
	{
		m_BitmapRendererHandle = other.m_BitmapRendererHandle;
		m_InternalHandle = other.m_InternalHandle;

		other.m_InternalHandle = nullptr;
	}

	const void* FramebufferImage::GetBitmapRendererHandle() const
	{
		return m_BitmapRendererHandle;
	}

	const void* FramebufferImage::GetInternalHandle() const
	{
		return m_InternalHandle;
	}
}