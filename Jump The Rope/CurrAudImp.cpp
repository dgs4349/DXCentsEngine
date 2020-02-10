#include "CurrAudImp.h"

CurrAudImp::CurrAudImp()
{
}


CurrAudImp::~CurrAudImp()
{
	jumpSfx.clear();
	soundEngine->Release();
}

void CurrAudImp::Init()
{

	soundEngine->Init();
	// loading audio here for now
	bgIntro = soundEngine->CreateSound(L"Assets/Audio/audio_background_intro.wav");
	bgLoop = soundEngine->CreateSound(L"Assets/Audio/audio_background_loop.wav", true);
	bgIntro->Link(bgLoop);
	bgIntro->Set(0.35f);

	Sound::RTPCParams* introParams = bgIntro->CreateRTPCParams();

	bgIntro->Bind(introParams->pitch, &ropeSpeed, -0.01f, 0.75f, startRopeSpeed, speedIncreaseMax);

	menuIntro = soundEngine->CreateSound(L"Assets/Audio/audio_menu_intro.wav");
	menuLoop = soundEngine->CreateSound(L"Assets/Audio/audio_menu_loop.wav", true);
	menuIntro->Link(menuLoop);
	menuIntro->Set(menuVolume);
	menuIntro->PlayOnUpdate();

	jumpSfx.push_back(soundEngine->CreateSound(L"Assets/Audio/sfx/jump_0.wav"));
	jumpSfx.push_back(soundEngine->CreateSound(L"Assets/Audio/sfx/jump_1.wav"));
	jumpSfx.push_back(soundEngine->CreateSound(L"Assets/Audio/sfx/jump_2.wav"));
	jumpSfx.push_back(soundEngine->CreateSound(L"Assets/Audio/sfx/jump_3.wav"));
	for (int i = 0; i < jumpSfx.size(); i++) jumpSfx[i]->Set(0.5f, 0.f, 0.95f);
}

void CurrAudImp::Update(float deltaTime, float totalTime)
{
	soundEngine->Update(deltaTime, totalTime);
}

void CurrAudImp::Suspend()
{
	soundEngine->Suspend();
}

void CurrAudImp::Resume()
{
	soundEngine->Resume();
}


void CurrAudImp::OneHit(SFX_ONE_HIT event)
{
	switch (event) {
	case JUMP:
		Jump();
		break;
	}
}

void CurrAudImp::SwitchScene(SCENE scene)
{
	switch (scene) {
	case TO_GAME:
		if (!menuFading) {
			// TODO: this can be changed to an RTPC of the menu buttons

			menuIntro->Fade(menuVolume, 0.f, menuFadeLength);
			//menuIntro->Stop(true);
			menuFading = true;
		}
		break;
	case GAME:
		menuFading = false;
		bgIntro->PlayOnUpdate();
		break;
	case TO_MENU:
		bgIntro->Stop(true);
		//menuIntro->Play();
		menuIntro->Fade(0.f, menuVolume, menuFadeLength);
		jumpCount = 0;
		break;
	case MENU:
		if (menuFading) {
			//soundEngine->ReplaceFade(menuFadeOut, menuFadeIn);
			menuIntro->Fade(0.f, menuVolume, menuFadeLength);
			//menuIntro->Play();
			menuFading = false;
		}
		break;
	}
}

void CurrAudImp::Jump()
{
	jumpSfx[jumpCount % 4]->Play();
	jumpCount++;
}
