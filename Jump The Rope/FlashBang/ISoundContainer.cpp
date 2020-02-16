#include "ISoundContainer.hpp"

void ISoundContainer::from_json(const json& j, ISoundContainer& s) {

	std::string key;

	for (auto i : j.items()) {

		// bit redundant, but for-index would increase complexity
		key = i.key();

		if (!isalpha(key[0])) {
			s.parseSoundObject_(key, i.value());
		}
		else {
			switch ((SOUNDCONTAINER_ARG) key[0]) {

			case SOUNDCONTAINER_ARG::TYPE: //type
				s.type_ = (SOUNDCONTAINER_TYPE)(i.value().get<std::string>()[0]);
				break;
			case SOUNDCONTAINER_ARG::PLAYBACK:
				s.playback_ = (SOUNDCONTAINER_PLAYBACK)(i.value().get<std::string>()[0]);
				break;
			default:
				s.parseParam_(key, j);
				break;
			}
		}
	}
}

void ISoundContainer::parseParam_(std::string& key, const json& j)
{
	if (key[0] == 'i')
	{
		if (j[key].is_array()) {
			// iterate through each element in ARRAY
			for (auto el : j[key].get<std::vector<json>>())
				parseSoundObject_(el);
		}
		else {
			// iterate through each key-value pair in OBJECT
			for (auto el : j[key].items())
				parseSoundObject_(el.key(), el.value());
		}
	}
	SoundObject::parseParam_(key, j);
}

void ISoundContainer::parseFile_(std::string& fileKey, const json& j)
{
	if (j[fileKey].is_array())
	{
		auto files = j[fileKey].get <std::vector<std::string>>();

		for (size_t i = 0; i < files.size(); i++)
			checkSplitThenAddFileString_(files[i]);
	}
	else
	{
		auto f = j[fileKey].get<std::string>();
		checkSplitThenAddFileString_(f);
	}
}

void ISoundContainer::parseAddSoundObjectKey_(std::string const& originalKey, int soundObjectIndex)
{
	((json)*this)[originalKey] = soundObjectIndex;
	((json)*this)[originalKey.substr(1, originalKey.length())] = soundObjectIndex;
}

void ISoundContainer::parseSoundObject_(json const& j)
{
	if (j.find("Container") != j.end()) createSoundContainer_(j["Container"]);
	else createSound_(j);
}

void ISoundContainer::parseSoundObject_(std::string const& key, json const& j)
{
	if (j.find("Container") != j.end()) createSoundContainer_(key, j["Container"]);
	else createSound_(key, j);
}

bool ISoundContainer::checkFileString_(std::string& s)
{
	return s.find('|') == std::string::npos;
}

void ISoundContainer::checkSplitThenAddFileString_(std::string& f)
{
	/*
	 * called after a check for '|' in the file string, we need to parse multiple strings
	 *		if we have something like |a, b, c| in our filename, than we need to
	 *			iterate on and switch out a, then b, then c in our filenames
	 *		if we have something like |0-4-1| (equivalent to |0-4|) than we need to
	 *			iterate in a for loop to add integers 0-4, step by 1, to our filenames
	 */
	if (!checkFileString_(f))
	{
		Files.push_back(f);
	}
	else try {
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
				Files.push_back(fileStrings[0] + item + fileStrings[2]);
			}
		}

		// parse a range such as |0-4-1| into a for loop, much harder tbh
		else if (fileStrings[1].find('-') != std::string::npos)
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
				Files.push_back(fileStrings[0] + std::to_string(i) + fileStrings[2]);
			}
		}
	}
	catch (const std::exception & e)
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
