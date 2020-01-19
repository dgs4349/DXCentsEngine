#pragma once

#include "nlohmann/json.hpp"

#include "ISoundObject.hpp"

using namespace FlashBang;

class ISoundContainer : protected ISoundObject, public json
{
public:
	ISoundContainer(){}
	~ISoundContainer();

	std::vector<std::string> Files;

	static void from_json(const json& j, ISoundContainer& s) {

		/*
		 *	things we have to check for
		 *		- Items lists and sub Containers
		 *		
		 *		
		 * 
		 */
		auto end = j.end();
		if(
		
		if (j.find("f") != end)
		{
			if (j["f"].is_array())
			{
				for (auto el : j["f"]) s.Files.push_back(el);
			}
			else
			{
				s.Files.push_back(j["f"]);
			}
		}

		// we have to check for more containers here
		for (auto& el : j.items()) {
			if (!isalpha(el.key()[0])) {
				if()
				s.createSound_(el.key());
			}
		}


		ISoundObject::from_json(j, s);
	}

protected:

	virtual void createSound_(std::string const& key) = 0;
};
