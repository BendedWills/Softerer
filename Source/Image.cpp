#include <Softerer/Image.hpp>
namespace Softerer
{
	Image::Image(size_t width, size_t height)
		:
		m_Width(width),
		m_Height(height)
	{}

	Image::~Image()
	{}

	Image::Pixel* Image::GetData() const
	{
		return m_ImageData;
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