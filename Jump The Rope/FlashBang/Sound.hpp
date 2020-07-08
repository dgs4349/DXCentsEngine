#pragma once

#include "FlashBang_Fwd.hpp"
#include "SoundObject.hpp"
#include "SoundConnectionsManager.hpp"

class FlashBang::Sound : public SoundObject
{
public:
	~Sound() { unload_(); }

	Sound(const json& j) {
		from_json(j, *this);
	}
	Sound(const std::string& s) {
		const json j = s;
		from_json(j, *this);
	}

	Sound& This() { return *this; }

	SoundObject& operator=(const json& j) override {
		auto* sound = new Sound(j);
		return  *sound;
	}
	SoundObject& operator=(const std::string& s) override {
		Sound* sound = new Sound(s);
		return *sound;
	}

	DirectXSoundEffect DXSoundEffect;
	DirectXSoundEffectInstance DXSoundEffectInstance;

	void Unload() override { unload_(); }

	virtual void Load() override {
		// should we check for '|' characters?

		// should we keep File as a wstring?
		const wchar_t* file_str = std::wstring(File.begin(), File.end()).c_str();
		DXSoundEffect = SoundConnectionsManager::LoadDirectXSoundEffect(file_str);

		// note: we can add flags here such as NoPitch or ReverbUseFilters
		DXSoundEffectInstance = DXSoundEffect->CreateInstance();

		SetParams();
		State(SOUND_STATE::IDLE);
	}

private:
	Sound() = default;
	void unload_() {
		if (DXSoundEffectInstance->GetState() != DirectX::STOPPED) {
			DXSoundEffectInstance->Stop(true);
		}
		DXSoundEffectInstance.release();
		DXSoundEffect.release();
		State(SOUND_STATE::UNLOADED);

		SoundConnectionsManager::QueueUnregisterEffectControls(Key);
	}
	
protected:
	float handleVolume_(float val) override {
		if(Loaded()) DXSoundEffectInstance->SetVolume(val);
		return val;
	}
	float handlePan_(float val)	override {
		if (Loaded()) DXSoundEffectInstance->SetPan(val);
		return val;
	}

	float handleTune_(float val) override {
		if (Loaded()) DXSoundEffectInstance->SetPitch(val);
		duration_ = getDuration_(val);
		return val;
	}

	virtual float getDuration_(float tune) {
		if (tune == tune_ && duration_ >= 0.f) return duration_;

		// if no tune (pitch) effects applied, sample duration is uneffected;
		if (tune == 0.f) return DXSoundEffect->GetSampleDuration();

		// tune pitches up to 1 octave(duration*2) or down one octave (duration * 0.5)
		return DXSoundEffect->GetSampleDuration() * pow(2, tune);
	}

	virtual void handlePlay_()		override { DXSoundEffectInstance->Play(loop_ != 0); }
	virtual void handlePause_()		override { DXSoundEffectInstance->Pause(); }
	virtual void handleResume_()	override { DXSoundEffectInstance->Resume(); }
	virtual void handleFinish_()	override { DXSoundEffectInstance->Stop(false); }
	virtual void handleStop_()		override { DXSoundEffectInstance->Stop(true); }
};