#pragma once

#include "nlohmann/json.hpp"
#include "FlashBang.hpp"

#include "ISoundObject.h"

class ISoundJson : ISoundObject, json
{
public:
	ISoundJson();
	~ISoundJson();
};

