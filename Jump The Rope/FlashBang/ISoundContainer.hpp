#pragma once

#include "nlohmann/json.hpp"
#include <boost/algorithm/string.hpp>

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

		if (j.find("i") != end)
		{
			// recurse, not an array, rest of objects can be added normally
			if (j["i"].is_object()) from_json(j["i"], s);
			else
			{
				s.createSound_()
			}
		}

		
		if (j.find("f") != end)
		{
			if (j["f"].is_array())
			{
				auto f = j["f"].get <std::vector<std::string>>();
				parseFiles_(s, f);

			}
			else
			{
				auto f = j["f"].get<std::string>();
				parseFile_(s, f);
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

	static void parseFile_(ISoundContainer &s, std::string &f)
	{
		// if we don't find any special characters just add it to s.Files and move on
		if(f.find('|') == std::string::npos)
		{
			s.Files.push_back(f);
		}
		
		/*
		 * if we find '|' in the file string, than we need to parse multiple strings
		 *		if we have something like |a, b, c| in our filename, than we need to
		 *			iterate on and switch out a, then b, then c in our filenames
		 *		if we have something like |0-4-1| (equivalent to |0-4|) than we need to
		 *			iterate in a for loop to add integers 0-4, step by 1, to our filenames
		 */
		else
		{
			// aud|0-4|.wav => {aud, 0-4, .wav}; aud|a,b,c|.wav => {aud, "a,b,c", .wav}
			std::vector<std::string> fileStrings;
			boost::split(fileStrings, f, boost::is_any_of('|'));

			// make sure there are no ' ' characters by accident
			// should note that there should be NO spaces in any array item
			boost::erase_all(fileStrings[1], ' ');

			// parse a list such as |a,b,c|, this is pretty straight forward
			if (fileStrings[1].find(',') != std::string::npos)
			{

			}

			// parse a range such as |0-4-1| into a for loop, much harder tbh
			else if(fileStrings[1].find('-') != std::string::npos)
			{
				// "0-4-1" => {0, 4, 1}, from-to-step
				std::vector<std::string> forParams;
				boost::split(forParams, 
					fileStrings[1], 
					boost::is_any_of('-'));

				// for (i = 0; i < 4; i += 1), add "aud" + i + ".wav"
				for (
					int i = std::stoi(forParams[0]);
						i < std::stoi(forParams[1]);
						i += (forParams.size() > 2) ? std::stoi(forParams[2]) : 1
					)
				{
					s.Files.push_back(
						fileStrings[0] + 
						std::to_string(i) + 
						fileStrings[2]);
				}
			}
		}
	}
	
	static void parseFiles_(ISoundContainer &s, std::vector<std::string>& f)
	{
		for(auto el : f)
		{
			/*std::string s = "scott>=tiger>=mushroom";
			std::string delimiter = ">=";

			size_t pos = 0;
			std::string token;
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
				std::cout << token << std::endl;
				s.erase(0, pos + delimiter.length());
			}
			std::cout << s << std::endl;*/

			
		}
	}
};
