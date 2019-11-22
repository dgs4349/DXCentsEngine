#pragma once
#include "SoundEngine.h"
#include <vector>

class CurrAudImp
{
public:
	CurrAudImp();
	~CurrAudImp();

	float menuFadeLength = 1.0f;

	void Init();

	/*
		todo:
			- Update can be replaced by no args call via parallel lifecycle loops
			- Suspend and Resume we will probably still need to hook in
			
			- all music calls could be made defferred, but all sfx calls need to be instant, 
				- how can we manage this?
	*/
	void Update(float deltaTime, float totalTime);

	// hook this in via some sort of HookLifeCycles method
	void Suspend();
	void Resume();

	// move this somehow else, even strings could work?
	enum SCENE { MENU, TO_MENU, TO_GAME, GAME };
	enum SFX_ONE_HIT { JUMP };
	// bit field struct?
	
	// okay for sfx, we need some kind of Event system for single playbacks?
	//	enums would not be perfect because I imagine this might eventually be too much on the stack?
	//		(i dont know i should probably consult the prof)
	// for now just a jump function?
	void OneHit(SFX_ONE_HIT event);
		

	void SwitchScene(SCENE scene);

private:
	SoundEngine* soundEngine;

	Sound* menuIntro;
	Sound* menuLoop;

	std::vector<Sound*> jumpSfx = std::vector<Sound*>(4);

	Sound* bgIntro;
	Sound* bgLoop;


	float menuVolume = 0.4f;
	bool menuFading = false;

	int jumpCount = 0;
	void Jump();
};

