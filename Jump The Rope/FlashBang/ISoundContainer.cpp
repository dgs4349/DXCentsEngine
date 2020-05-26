#include "ISoundContainer.hpp"
#include <boost/algorithm/string.hpp>

void ISoundContainer::parseParam_(const std::string& key, const json& j)
{
	switch (static_cast<SOUNDCONTAINER_ARG>(key[0])) {
	case SOUNDCONTAINER_ARG::PLAYBACK_BEHAVIOR:
		j[key].get_to(playbackBehavior_);
		break;
	case SOUNDCONTAINER_ARG::PLAYBACK_ORDER:
		j[key].get_to(playbackOrder_);
		break;
	case SOUNDCONTAINER_ARG::ITEMS:
		parseItems_(j[key]);
		break;

	default:
		SoundObject::parseParam_(key, j);
		break;
	}
}

void ISoundContainer::parseItems_(const json& items) {
	// iterate through array
	// check if Container/Schema
	// default: parse soundobject

	for (auto [key, value] : items.items()) {
		if (getItemType_(key) == SOUNDCONTAINER_ITEM_TYPE::SCHEMA) {
			parseSchema_(value);
		}
		else createSound_(value);
	}
}

void ISoundContainer::parseSchema_(const json& schema) {
	// this is gross, clean this up later

	std::string fileStr;
	std::string keyStr;

	std::string fileKey; //keep keys for properly copying
	std::string keyKey;

	for (auto [key, value] : schema.items()) {
		switch (static_cast<SOUNDOBJECT_ARG>(key[0])) {
		case SOUNDOBJECT_ARG::FILE:
			fileKey = key;
			value.get_to(fileStr);
			break;
		case SOUNDOBJECT_ARG::KEY:
			keyKey = key;
			value.get_to(keyStr);
			break;
		default: break;
		}
	}

	try {
		const auto files = processSchemaString_(fileStr);
		const auto keys = processSchemaString_(keyStr);

		json j = schema;
		for (int i = 0; i < files.size(); ++i) {
			j[fileKey] = files[i];
			j[keyKey] = keys[i];
			createSound_(j);
		}
	}
	catch (std::exception& e) {
		printf(e.what());
		throwSchemaError_(fileStr, keyStr);
	};
}


// TODO, Schema param? say the last sound should play infinitely?
std::vector<std::string> const& ISoundContainer::processSchemaString_(
	const std::string& str) const {
	auto split = std::vector<std::string>(3);

	//pad string in case no prefix or suffix
	boost::split(split, " " + str + " ", boost::is_any_of('|'));

	// remove all spaces, also removes padding
	for (auto i : split) boost::erase_all(i, ' ');

	if (split.size() <= 2) throw std::exception(); //missing |

	// split = ["prefix", "args", "suffix"], split[1] is args

	// make sure there are no ' ' characters in array items
	std::vector<std::string> parsedArgs;
	std::vector<std::string> results;

	// parse a list such as |a,b,c|, this is pretty straight forward
	if (split[1].find(',') != std::string::npos)
	{
		// just parse the array, "a,b,c" => {a, b, c}
		boost::split(parsedArgs, split[1], boost::is_any_of(','));

		// for each item {a,b,c} add "aud" + "a" + ".wav
		for (auto item : parsedArgs) {
			results.push_back(split[0] + item + split[2]);
		}
	}

	// parse a range such as |0-4-1| into a for loop, much harder tbh
	else if (split[1].find('-') != std::string::npos)
	{
		// "0-4-1" => {0, 4, 1}, from-to-step
		boost::split(parsedArgs, split[1], boost::is_any_of('-'));

		// for (i = 0; i < 4; i += 1), add "aud" + i + ".wav"
		for (
			auto i = std::stoi(parsedArgs[0]);
			i < std::stoi(parsedArgs[1]);
			i += (parsedArgs.size() > 2) ? std::stoi(parsedArgs[2]) : 1
			)
		{
			results.push_back(parsedArgs[0] + std::to_string(i) + parsedArgs[2]);
		}
	}

	return results;
}

void ISoundContainer::throwSchemaError_(
	const std::string& files, const std::string& keys) {
	const auto message = R"(
		Error parsing |...| in provided Schema file or key string!
		Please double check formatting rules:
		- Range: |start-stop-step| as in "sound|0-4|.wav" or "sound|0-8-2|.wav"
		- List:  |a,b,c| as in "audio|a,b,c|.wav"
			- list items that include spaces will not parse correctly: |a, two words, c| => [a, twowords, c]
		- 1 file and 1 key to each sound
		- only one parse per string supported
	)";

	const auto vars = "\nFiles String: " + files + "\nKeys String: " + keys;

	throw std::exception((message + vars).c_str());
}