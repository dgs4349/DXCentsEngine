#include "ISoundObject.h"



ISoundObject::ISoundObject()
{
}


ISoundObject::~ISoundObject()
{
}

void ISoundObject::from_json(const json& j, ISoundObject& s) {
	j.at("v").get_to(s.Volume);
	j.at("t").get_to(s.Tune);
	j.at("p").get_to(s.Pan);
	j.at("o").get_to(s.Order);

	j.at("f").get_to(s.File);
}