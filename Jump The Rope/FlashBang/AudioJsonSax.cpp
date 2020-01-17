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

	// names such as "_jump", do nothing
	if (!isalpha(val[0])) return true;
	
	// events as in "OnStart", do nothing
	if (val[0] == 'O') return true;

	val = tolower(val[0]);
	return true;
}