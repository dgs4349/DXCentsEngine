#pragma once

#include "nlohmann/json.hpp"

#include "ISoundObject.hpp"

using namespace FlashBang;

class ISoundContainer : ISoundObject, json
{
public:
	ISoundContainer(){}
	~ISoundContainer();

	std::vector<std::string> Files;

	static void from_json(const json& j, ISoundContainer& s) {

		ISoundObject::from_json(j, s);
		
		if (j.find("f") != j.end())
		{
			if (j["f"].is_array())
			{
				for (auto e : j["f"]) s.Files.push_back(e);
			}
			else
			{
				s.Files.push_back(j["f"]);
			}
		}
		
		for (auto& el : j.items()) {
			if (!isalpha(el.key()[0])) s.createSound_(el.key());
		}
	}

protected:

	virtual void createSound_(std::string const& key) = 0;
};
