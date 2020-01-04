#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>

#include "SoundEngine.h"

using json = nlohmann::json;
using namespace FlashBang;

class AudioJsonParser
{
public:
	AudioJsonParser();
	~AudioJsonParser();
};

