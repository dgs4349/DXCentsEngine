#pragma once

#include "nlohmann/json.hpp"

#include "ISoundObject.hpp"

using namespace FlashBang;

class ISoundJson : ISoundObject, json
{
public:
	ISoundJson();
	~ISoundJson();

	std::vector<std::string> Files;

	void from_json(const json& j, ISoundJson& s) {
		ISoundObject::from_json(j, s);

		j.at("Files").get_to(s.Files);

		for (auto& el : j.items()) {
			std::string key = el.key();
			std::string newKey;
			if (!isalpha(key[0])) {
				newKey = key.substr(1, key.length() - 1);
				((json)*this)[newKey] = el.value();
			}
		}
	}

private:
	//_loopAction;
};
