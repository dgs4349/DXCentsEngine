#include "SoundConnectionsManager.hpp"

using namespace FlashBang;


void SoundConnectionsManager::RegisterEffectControl(EffectControl const& control)
{
	effectControls_[control.soundKey].push_back(
		std::make_pair(control.effectKey, control)
	);
}

void SoundConnectionsManager::RegisterEffectControl(float* controlVarPtr, const char* soundKeyCStr, const char* effectKeyCStr, float controlMin, float controlMax)
{
	EffectControl e = {
		controlVarPtr,
		std::string(soundKeyCStr),
		std::string(effectKeyCStr),
		controlMin,
		controlMax };
	RegisterEffectControl(e);
}

void SoundConnectionsManager::RegisterEffectControls(std::vector<EffectControl> const& controls)
{
	for (auto c : controls)
		effectControls_[c.soundKey].push_back(
			std::make_pair(c.effectKey, c));
}