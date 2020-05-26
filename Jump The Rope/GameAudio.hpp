#pragma once

#include "FlashBang/FlashBang.hpp"

using FlashBang::SoundContainer;

class GameAudio 
{
public:

	SoundEngine* engine;

	// todo: IGameAudio
	/*
	 * no real need to access soundEngine from game.cpp if
	 *	we already have a local member GameAudio
	 * Also allow GameAudio["MySound"]()
	 * TODO: top level attr to sound, access outside scenes? ["mysound"]() instead of ["myscene"]["mysound"]
	 */
	
	GameAudio()
	{

		engine = SoundEngine::Get();
		
		SoundContainer menu = R"(
			"Menu" : {
				"Container": {
					"v": 0.35,
					"type": "Playlist",
					"effects": [
						{ "key": "menuFade", "param": "v", "low": 0.35, "high": 0.0 }
					],
					"items": [
						{ "Scema": {
							"files": L"Assets/Audio/audio_background_|intro,loop|.wav",
							"keys": "|intro,loop|"
						}}
					]
				}
			}
		)"_json;

		SoundContainer game = R"(
			"Background": {
				"Container": {
					"v": 0.35,
					"type": "Playlist",
					"items" : [
						{ "Schema": {
							"files": L"Assets/Audio/audio_background_|intro,loop|.wav"
							"keys": "|intro,loop|",
						}}
					],
		)"_json;

		SoundContainer SFX = R"(
			"Jump": {
				"Container": {
					"v": 0.5,
					"p": 0.95,
					"type": "OneShot",
					"items": [
						{ "Schema": {
							"files": L"Assets/Audio/sfx/jump_[0-4].wav",
							"keys": "|0-4|",
						}}
					],
					"effects": [
						{ "key": "jumpPitch", "param": "pan", "low": -0.1, "high": 0.75 }
					],
				}
		)"_json;

		// todo: move to schema args
		menu[menu.size() - 1].Loop(-1);
		game[game.size() - 1].Loop(-1);

		// todo: simplify, add scene init to AddScene
		auto MenuScene = SoundEngine::Scene("MenuScene", &menu);
		auto GameScene = SoundEngine::Scene("GameScene", &game);
		auto SFXScene = SoundEngine::Scene("SFX", &SFX);

		// todo: AddScenes
		MenuScene.Start();
		GameScene.Start();
		SFXScene.Start();
	}

	void ToGame()
	{
		SoundEngine::Containers["GameScene"]();
	}

	void ToMenu()
	{
		SoundEngine::Containers["GameScene"].Stop();
	}
	
	~GameAudio()
	{
		engine->Release();
	}


};

