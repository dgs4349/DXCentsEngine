#pragma once

#include "FlashBang_Fwd.hpp"
#include "Effect.hpp"

using namespace FlashBang;

/*
 * Currently: middleman between sounds and sound engine so both can run
 *				independently of each other without dependency collisions
 *				
 * TODO: move functionality here, to prevent redundant function calls
 *
 *
 *              SoundConnectionsManager
 *                   ^        ^
 *                  /          \   
 *             SoundEngine -> Sound
 *
 *             SoundCM -X> Sound
 *             SoundCM -X> SoundEngine
 *
 *         - SoundEngine will have an instance of ConnectionsManager,
 *         - Connection logic moved here, SoundEngine handles moving Effect containers
 *         - SoundEngine will collect items during end of update cycle
 */


 // EffectControl: container for game var data
 // TODO: from_json, move to smart container, just make less messy please
struct EffectControl
{
	float* controlVarPtr;
	std::string soundKey;
	std::string effectKey;
	float controlMin = 0.f;
	float controlMax = 0.f;
	Effect::Connection to_connection() const
	{
		return { controlVarPtr, controlMin, controlMin };
	};
};

class SoundConnectionsManager
{
public:

	/*
	 * Clear 
	 * 
	 */
	bool FreeControlRegisterOnUnload = true;
	float FreeControlRegisterDelay = 100.f;
	static void FreeControlRegister() { instance_->unregisterTimer_ = 0.f; }


	// todo: investigate a url-type path for soundContainer.../sound/effect for easy traversal


	void RegisterEffectControl(EffectControl const& control);
	void RegisterEffectControl(float* controlVarPtr, const char* soundKeyCStr, const char* effectKeyCStr, float controlMin, float controlMax);
	void RegisterEffectControls(std::vector<EffectControl> const& controls);


	static float StaticRegisterEffectControl(float initialValue, EffectControl const& control)
	{
		instance_->RegisterEffectControl(control);
		return initialValue;
	}
	

	static DirectXSoundEffect LoadDirectXSoundEffect(const wchar_t* fileStr)
	{
		if (!initiated_) GetOnce();
		return std::make_unique<DirectX::SoundEffect>(instance_->dxAudioEngine_.get(), fileStr);
	}






	/*
	 * I am okay with game-side logic managing instances of SoundEngine
	 * I am not comfortable with Sounds doing so, simply too many calls to too many instances
	 * For logic that Sounds need to call, these should be static
	 *
	 * TODO: move copy-args to smart pointers
	 */
	static void QueueUnregisterEffectControls(const std::string& soundKey) {
		instance_->unregisterCache_.push_back(soundKey);
		if (instance_->FreeControlRegisterOnUnload)  instance_->unregisterTimer_ = 0.f;
	}
	static void QueueUnregisterEffectControls(const std::vector<std::string> soundKeys) {
		for (auto e : soundKeys)  instance_->unregisterCache_.push_back(e);
		if (instance_->FreeControlRegisterOnUnload)  instance_->unregisterTimer_ = 0.f;
	}

	static std::vector<std::pair<std::string, EffectControl>>&
		GetEffectControls(std::string const& soundKey)
	{
		return instance_->effectControls_[soundKey];
	}



	void Update(float deltaTime)
	{
		if (unregisterTimer_ >= 0.f)
		{
			unregisterTimer_ += deltaTime;
			if (unregisterTimer_ > FreeControlRegisterDelay)
			{
				// TODO: find thread-safe solution to allow non-blocking clear
				const auto it = unregisterCache_.begin();

				for (int i = unregisterCache_.size() - 1; i >= 0; --i)
				{
					effectControls_.erase(unregisterCache_[i]);
					unregisterCache_.erase(it + i);
				}

				unregisterTimer_ = -1.f;
			}
		}
	}
	
	
	static SoundConnectionsManager* Get() { addRef_(); return GetOnce(); }
	
	static SoundConnectionsManager* GetOnce()
	{
		if (instance_ == nullptr) { instance_ = new SoundConnectionsManager(); }
		return instance_;
	}

	void Release()
	{
		releaseRef_();

		if ((refs_ == 0) && (instance_ != nullptr)) {
			delete instance_;
			instance_ = nullptr;
		}

	}
	
private:


	std::vector<const std::string> unregisterCache_;
	float unregisterTimer_ = -1.f;

	// list of effect key-value pairs, assigned to a sound key
	// soundKey: [effectKey: {effect...}, ...]
	std::unordered_map<std::string,
		std::vector<
		std::pair<std::string, EffectControl>>> effectControls_;

	std::unique_ptr<DirectX::AudioEngine> dxAudioEngine_;

	static SoundConnectionsManager* instance_;

	static bool initiated_;

	static int refs_;
	static void addRef_() { ++refs_; }
	static void releaseRef_() { --refs_; }
	
	SoundConnectionsManager()
	{
		std::unique_ptr<DirectX::AudioEngine> dxAudioEngine_;
	}
	~SoundConnectionsManager() { dxAudioEngine_.release(); };
	
};
