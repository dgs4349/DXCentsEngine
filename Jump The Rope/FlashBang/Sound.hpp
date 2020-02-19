#pragma once

#include "FlashBang.hpp"
#include "SoundObject.hpp"
#include "ISoundContainer.hpp"
#include "SoundEngine.hpp"

#include <Audio.h>

class FlashBang::Sound : public SoundObject
{
public:
	~Sound() { unload_(); }

	Sound(const json& j) {
		from_json(j, *this);
	}
	Sound(const std::string& s) {
		json j = s;
		from_json(j, *this);
	}

	SoundObject& operator=(const json& j) override {
		Sound* sound = new Sound(j);
		return  *sound;
	}
	SoundObject& operator=(const std::string& s) override {
		Sound* sound = new Sound(s);
		return *sound;
	}

	std::unique_ptr<DirectX::SoundEffect> DirectXSoundEffect;
	std::unique_ptr<DirectX::SoundEffectInstance> DirectXSoundEffectInstance;

	void Unload() override { unload_(); }

	virtual void Load() override {
		// should we check for '|' characters?

		// should we keep File as a wstring?
		const wchar_t* file_str = std::wstring(File.begin(), File.end()).c_str();
		DirectXSoundEffect = SoundEngine::LoadSoundDX(file_str);

		// note: we can add flags here such as NoPitch or ReverbUseFilters
		DirectXSoundEffectInstance = DirectXSoundEffect->CreateInstance();

		SetParams();
		State(SOUND_STATE::IDLE);
	}

private:
	Sound() = default;
	void unload_() {
		if (DirectXSoundEffectInstance->GetState() != DirectX::STOPPED) {
			DirectXSoundEffectInstance->Stop(true);
		}
		DirectXSoundEffectInstance.release();
		DirectXSoundEffect.release();
		State(SOUND_STATE::UNLOADED);
	}

protected:
	float handleVolume_(float val) override { 
		DirectXSoundEffectInstance->SetVolume(val); 
		return val; 
	}
	float handlePan_(float val)	override {
		DirectXSoundEffectInstance->SetPan(val);
		return val;
	}

	float handleTune_(float val) override {
		DirectXSoundEffectInstance->SetPitch(val); 
		duration_ = getDuration_();
		return val;
	}

	virtual float getDuration_() {
		// if no tune (pitch) effects applied, sample duration is uneffected;
		if (tune_ == 0.f) return DirectXSoundEffect->GetSampleDuration();

		// tune pitches up to 1 octave(duration*2) or down one octave (duration * 0.5)
		return DirectXSoundEffect->GetSampleDuration() * pow(2, tune_);
	}

	virtual void handlePlay_()		override { DirectXSoundEffectInstance->Play(loop_ != 0); }
	virtual void handlePause_()		override { DirectXSoundEffectInstance->Pause(); }
	virtual void handleResume_()	override { DirectXSoundEffectInstance->Resume(); }
	virtual void handleFinish_()	override { DirectXSoundEffectInstance->Stop(false); }
	virtual void handleStop_()		override { DirectXSoundEffectInstance->Stop(true); }
};
