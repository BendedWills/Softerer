#include <Softerer/Image.hpp>
#include <cassert>

namespace Softerer
{
	Image::Image(Pixel* data, size_t width, size_t height)
		:
		m_Data(data),
		m_Width(width),
		m_Height(height)
	{
		assert(data != nullptr);
	}

	Image::Pixel* Image::GetData() const
	{
		return m_Data;
	}

	const size_t Image::GetWidth() const
	{
		return m_Width;
	}

	const size_t Image::GetHeight() const
	{
		return m_Height;
	}

}