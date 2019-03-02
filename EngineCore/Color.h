#pragma once

struct Color
{
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 1;

	const Color() {}

	Color(float r, float g, float b, float a = 1)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color(float grey, float alpha = 1)
	{
		r = grey;
		b = grey;
		g = grey;
		a = alpha;
	}

	inline Color GreyScale()
	{
		return Color(0.21f * r + 0.72f * g + 0.07f * b, a);
	}
};