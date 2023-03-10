#pragma once

#include <Softerer/Image.hpp>

namespace Softerer
{
	class FramebufferImage : public Image
	{
	public:
		FramebufferImage(Pixel* data, size_t width, size_t height);
		virtual ~FramebufferImage() = 0;
	};
}