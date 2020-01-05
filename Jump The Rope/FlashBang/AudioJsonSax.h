#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>
#include <exception>

#include "FlashBang.hpp"

using json = nlohmann::json;
using namespace FlashBang;

class AudioJsonSax : public json::json_sax_t
{
public:
	AudioJsonSax();
	~AudioJsonSax();

	bool string(string_t& val) override;

	bool start_object(std::size_t elements) override;
	bool end_object() override;

	bool start_array(std::size_t elements) override;
	bool end_array() override;

	bool key(string_t& val) override;

private:
	bool SerializeParamEvent(string_t& val);
};
