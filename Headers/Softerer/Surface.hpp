#pragma once

#include <Softerer/Common/Ref.hpp>

#include <Softerer/FramebufferImage.hpp>

namespace Softerer
{
	class Surface
	{
	public:
		virtual ~Surface() = 0;

		virtual void Present(const FramebufferImage& framebuffer) = 0;
	};
}