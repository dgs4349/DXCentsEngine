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

	void Update();

	void MenuFade();

	enum SCENE { MENU, TO_MENU, TO_GAME, GAME };

	void SwitchScene(SCENE scene);

private:
	SoundEngine* soundEngine;

	Sound* menuIntro;
	Sound* menuLoop;

	std::vector<Sound*> jumpSfx[4];

	Sound* bgIntro;
	Sound* bgLoop;


	float menuVolume = 0.4f;
	bool menuFading = false;

};

