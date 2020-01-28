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

		auto end = j.end();

		if (j.find("i") != end)
		{
			/*
			 *	Possible "i" (item) key types:
			 *
			 *	// named sound objects:
			 *		containerExample1{ Container {
			 *			items {
			 *				$fire{ ... }
			 *				$reload{ ... }
			 *			}
			 *		}}
			 *
			 * // arrays of anonymous sound objects (will be indexed):
			 *		containerExample2{ Container {
			 *			items{
			 *				{ f: "file1.wav", v: 0.5f }
			 *				{ f: "filesomethingelse.wav", p: -.8f, v: 5f }
			 *			}
			 *		}}
			 *
			 */

			if(j["i"].is_array()) {
				for (auto el : j["i"].get<std::vector<json>>()) s.parseSoundObject_(el);
			}
			else {
				for (auto el : j["i"].items()) s.parseSoundObject_(el.key(), el.value());
			}
			
		}

		// we'll need to parse files here but also on named sound files as well
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

		// iterate for custom items
		for (auto& el : j.items()) {
			if (!isalpha(el.key()[0])) s.parseSoundObject_(el.key(), el.value());
		}

		ISoundObject::from_json(j, s);
	}

protected:
	
	virtual ISoundObject* createSound_(json const& j) = 0;
	virtual ISoundObject* createSoundContainer_(json const& attr) = 0;

	virtual ISoundObject* createSound_(std::string const& key, json const& j) = 0;
	virtual ISoundObject* createSoundContainer_(std::string const& key, json const& attr) = 0;

	virtual int addSoundObject_(ISoundObject const& soundObject) = 0;
	virtual int addSoundObject_(std::string const& key, ISoundObject const& soundObject) = 0;

	void parseAddKey_(std::string const& originalKey, int soundObjectIndex)
	{
		((json)*this)[originalKey] = soundObjectIndex;
		((json)*this)[originalKey.substr(1, originalKey.length())] = soundObjectIndex;
	}

	void parseSoundObject_(json const& j)
	{
		if (j.find("Container") != j.end()) createSoundContainer_(j["Container"]);
		else createSound_(j);
	}
	
	void parseSoundObject_(std::string const& key, json const& j)
	{
		if (j.find("Container") != j.end()) createSoundContainer_(key, j["Container"]);
		else createSound_(key, j);
	}
	
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
		else try
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
				// just parse the array, "a,b,c" => {a, b, c}
				std::vector<std::string> listItems;
				boost::split(listItems, fileStrings[1], boost::is_any_of(','));

				// for each item {a,b,c} add "aud" + "a" + ".wav
				for (auto item : listItems) {
					s.Files.push_back(fileStrings[0] + item + fileStrings[2]);
				}
			}

			// parse a range such as |0-4-1| into a for loop, much harder tbh
			else if(fileStrings[1].find('-') != std::string::npos)
			{
				// "0-4-1" => {0, 4, 1}, from-to-step
				std::vector<std::string> forParams;
				boost::split(forParams, fileStrings[1], boost::is_any_of('-'));

				// for (i = 0; i < 4; i += 1), add "aud" + i + ".wav"
				for (
					int i = std::stoi(forParams[0]);
						i < std::stoi(forParams[1]);
						i += (forParams.size() > 2) ? std::stoi(forParams[2]) : 1
					)
				{
					s.Files.push_back(fileStrings[0] + std::to_string(i) + fileStrings[2]);
				}
			}
		}
		catch(const std::exception & e)
		{
			const auto message = R"(
	Error parsing |...| in filename! Please double check formatting rules:
	Range: |start-stop-step| as in "audio_|0-4|.wav" or "audio_|0-8-2|.wav"
	List: |a,b,c| as in "audio_|a,b,c|.wav"

	Received string: )";

			const auto messageString = std::string(message) + f + "\n";
			
			printf(messageString.c_str());
			printf("Exception: %s", e.what());
		}
	}
	
	static void parseFiles_(ISoundContainer &s, std::vector<std::string>& f)
	{
		// both parseFile and parseFiles are very slow and big
		//	we should find a way to reduce complexity or cache when complete
		//	perhaps output into a faster, trusted json template that we can import risky
		for (auto el : f) { parseFile_(s, el); }
	}

};
