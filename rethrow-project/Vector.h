#pragma once

union Vec2
{
	float data[2];
	struct { float x, y; };
};

inline Vec2 vec2(float x, float y)
{
	return Vec2{ x, y };
}

union Vec4
{
	float data[4];
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
};

inline Vec4 vec4(float x, float y, float z, float w)
{
	return Vec4{ x, y, z, w };
}