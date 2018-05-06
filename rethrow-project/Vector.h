#pragma once

#include <glm/glm.hpp>

union Vec2
{
	float data[2];
	struct { float x, y; };
	struct { float w, h; };
};

inline Vec2 vec2(float x, float y) 			{ return Vec2{ x, y }; }

inline glm::vec2 vec2_to_glm(Vec2 v) 		{ return glm::vec2(v.x, v.y); }

inline Vec2 glm_to_vec2(glm::vec2 v) 		{ return vec2(v.x, v.y); }

inline bool operator==(Vec2 v1, Vec2 v2) 	{ return (v1.x == v2.x && v1.y == v2.y); }

inline bool operator!=(Vec2 v1, Vec2 v2) 	{ return !(v1 == v2); }

inline Vec2 operator*(Vec2 v, float a) 		{ return vec2(v.x*a, v.y*a); }

inline Vec2 operator/(Vec2 v, float a) 		{ return vec2(v.x/a, v.y/a); }

inline Vec2 operator+(Vec2 v1, Vec2 v2) 	{ return vec2(v1.x + v2.x, v1.y + v2.y); }

inline Vec2 operator-(Vec2 v) 				{ return vec2(-v.x, -v.y); }

inline Vec2 operator-(Vec2 v1, Vec2 v2) 	{ return v1 + (-v2); }

union Vec4
{
	float data[4];
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
};

inline Vec4 vec4(float x, float y, float z, float w) { return Vec4{ x, y, z, w }; }