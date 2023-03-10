#pragma once

#include <cstdint>

namespace Softerer
{
	class Image
	{
	public:
		struct Pixel
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		};

		Image(Pixel* data, size_t width, size_t height);

		Pixel* GetData() const;
		const size_t GetWidth() const;
		const size_t GetHeight() const;
	private:
		Pixel* m_Data = nullptr;
		size_t m_Width = 0;
		size_t m_Height = 0;
	};
}