#pragma once

#include <Softerer/Image.hpp>

namespace Softerer
{
	class FramebufferImage : public Image
	{
	public:
		FramebufferImage(size_t width, size_t height);
		~FramebufferImage();
		FramebufferImage(FramebufferImage&& other);

		const void* GetBitmapRendererHandle() const;
		const void* GetInternalHandle() const;
	private:
		void* m_BitmapRendererHandle = nullptr;
		void* m_InternalHandle = nullptr;
	};
}