#pragma once

#include "FlashBang.hpp"
#include <nlohmann/json.hpp>

using nlohmann::json;

using namespace FlashBang;


class ISoundObject
{
public:
	ISoundObject();
	~ISoundObject();

	float	Volume	= 0.0f;
	float	Tune	= 0.0f;
	float	Pan		= 0.0f;
	int		Order	= 0;

	void from_json(const json& j, ISoundObject& s);
};

