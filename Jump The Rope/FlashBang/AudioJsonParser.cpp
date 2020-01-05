#pragma once

#include "AudioJsonParser.h";

using namespace FlashBang;

AudioJsonParser::AudioJsonParser() {

}

AudioJsonParser::~AudioJsonParser() {
}


bool AudioJsonParser::string(string_t& val) {

}

bool AudioJsonParser::start_object(std::size_t elements) {

}

bool AudioJsonParser::end_object() {
}

bool AudioJsonParser::start_array(std::size_t elements) {
}

bool AudioJsonParser::end_array() {
}


bool AudioJsonParser::key(string_t& val) {
	if (std::isupper(val[0])) {

	}
	return true;
}

bool AudioJsonParser::ParseCommand(string_t& val) {
	switch (val.length) {
	case 1: 
		break;
	case 3:
		break;
	}


}