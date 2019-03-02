#pragma once

struct Color3
{
	float r = 0;
	float g = 0;
	float b = 0;

	Color3() {}

	Color3(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}

	Color3(float grey)
	{
		r = grey;
		b = grey;
		g = grey;
	}

	inline Color3 GreyScale()
	{
		return Color3(0.21f * r + 0.72f * g + 0.07f * b);
	}
};

struct Color : Color3
{
	float a = 1;

	Color() {}

	Color(Color3 color, float a = 1)
	{
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
		this->a = a;
	}

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