#pragma once
#include <cstdint>

union Color
{
	struct Channels
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	uint32_t rgba;
	FLOAT fRGBA[4];
	Channels channels;
};