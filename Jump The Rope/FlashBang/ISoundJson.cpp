#include "ISoundJson.h"



ISoundJson::ISoundJson()
{
}


ISoundJson::~ISoundJson()
{
}

void ISoundJson::from_json(const json& j, ISoundJson& s) {
	ISoundObject::from_json(j, s);

	j.at("Files").get_to(s.Files);

	for (auto& el : j.items()) {
		std::string key = el.key();
		std::string newkey = "";
		if (!isalpha(key[0])) {
			newkey = key.substr(1, key.length - 1);
			((json) *this)[newkey] = el.value();
		}
	}
}