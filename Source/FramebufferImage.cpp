#include <Softerer/FramebufferImage.hpp>

namespace Softerer
{
	FramebufferImage::FramebufferImage(Pixel* data, size_t width, size_t height)
		:
		Image(data, width, height)
	{}

	FramebufferImage::~FramebufferImage()
	{}
}