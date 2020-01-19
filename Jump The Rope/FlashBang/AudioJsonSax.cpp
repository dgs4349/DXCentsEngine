#pragma once

#include "AudioJsonSax.h";

using namespace FlashBang;

AudioJsonSax::AudioJsonSax() {

}

AudioJsonSax::~AudioJsonSax() {
}


bool AudioJsonSax::string(string_t& val) {

}

bool AudioJsonSax::start_object(std::size_t elements) {

}

bool AudioJsonSax::end_object() {
}

bool AudioJsonSax::start_array(std::size_t elements) {
}

bool AudioJsonSax::end_array() {
}


bool AudioJsonSax::key(string_t& val) {
	if (isalpha(val[0])) return SerializeParamEvent(val);
	return true;
}

bool AudioJsonSax::SerializeParamEvent(string_t& val) {
	/*
		Converts params such as "vol" or "volume" to consitent keywords like "v"
		Does NOT convert events such as "OnStart"
	*/

	// TODO: debug all param/events,
		// - if debug, print out all received args or output to file
	
	// names such as "_jump", do nothing
	// todo: output what named sounds were received
	if (!isalpha(val[0])) return true;

	// events such as in OnStart or descriptors such as Container
	// todo: output what was received and what the action will do
	if (isupper(val[0])) return true;
	

	// if nothing above matches, return just the first (lowercased) letter
	// todo: output what we received, what it will be (if recognized) interpreted as, and what that might do
	val = tolower(val[0]);
	return true;
}