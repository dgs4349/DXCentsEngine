#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>

#include "SoundEngine.h"

using json = nlohmann::json;
using namespace FlashBang;

class AudioJsonParser : public json::json_sax_t
{
public:
	AudioJsonParser();
	~AudioJsonParser();

	bool string(string_t& val) override;

	bool start_object(std::size_t elements) override;
	bool end_object() override;

	bool start_array(std::size_t elements) override;
	bool end_array() override;

	bool key(string_t& val) override;

private:
	bool ParseCommand(string_t& val);
};

