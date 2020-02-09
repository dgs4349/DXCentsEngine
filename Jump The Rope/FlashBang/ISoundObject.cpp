#include "ISoundObject.hpp"

ISoundObject* ISoundObject::CopyParams(const ISoundObject& s)
{
	if (this != &s)
	{
		volume_ = s.volume_;
		tune_ = s.tune_;
		pan_ = s.pan_;
		loop_ = s.loop_;
	}
	return this;
}


void ISoundObject::from_json(const json& j, ISoundObject& s) {
	std::string key;
	for (auto i : j.items()) {
		key = i.key();
		s.parseParam_(key, j);
	}
}

void ISoundObject::parseParam_(std::string& key, const json& j)
{
	if (islower(key[0]))
	{
		switch (key[0])
		{
		case 'v': j[key].get_to(volume_); break;
		case 't': j[key].get_to(tune_); break;
		case 'p': j[key].get_to(pan_); break;
		case 'l': j[key].get_to(loop_); break;

		case 'f': parseFile_(key, j);  break;
		case 'e': parseEffects_(key, j);break;

		default: break;
		}
	}
}

void ISoundObject::parseFile_(std::string& fileKey, const json& j)
{
	j[fileKey].get_to(File);
}

void ISoundObject::parseEffects_(std::string& effectsKey, const json& j)
{
	// effect json syntax: "name": { "vol": { float min, float max } }
	for (auto effect : j[effectsKey].items()) {

		std::string EffectName = effect.key();

		auto effectArgs = effect.value().items()[0]; // "vol": { float min, float max }

		ParamSetterFunc func = GetParamSetFunc(effectArgs.key()[0], *this);

		AddEffect(
			EffectName,
			new Effect(
				func, effectArgs.value()[0], effectArgs.value()[1]
			));
	}
}
