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
	


	/* 
		Notes:

		key ALWAYS before type
			"Menu": { "Container": {...} }
		even for Schema, where a key isn't necessary
			"MenuSchema": { "Schema": {...} }
	
	*/

	SoundContainer* menu;
	SoundContainer* game;
	SoundContainer* sfx;

	GameAudio()
	{

		engine = SoundEngine::Get();


		json menuJson = 
R"(
{
	"Menu": 
	{
		"Container": 
		{
			"v": 0.35,
			"behavior": "Playlist",
			"effects": 
			{
				"menuFade": 
				{ 
					"param": "vol", 
					"min": 0.35, 
					"max": 0.0
				}
			},
			"items": 
			{
				"menuSchema":
				{
					"Schema": 
					{
						"files": "Assets/Audio/audio_background_|intro,loop|.wav",
						"keys": "|intro,loop|"
					}
				}
			}
		}
	}
}
)"_json;

	
		json gameJson = 
R"(
{
	"Background": 
	{
		"Container": 
		{
			"v": 0.35,
			"behavior": "Playlist",
			"items" : 
			{
				"bgAudioSchema":
				{ 
					"Schema": 
					{
						"files": "Assets/Audio/audio_background_|intro,loop|.wav",
						"keys": "|intro,loop|"
					}
				}
			}
		}
	}
}
)"_json;


		json sfxJson =
R"(
{
	"Jump": 
	{
		"Container": 
		{
			"v": 0.5,
			"p": 0.95,
			"behavior": "OneShot",
			"items": 
			{ 
				"jumpSchema":
				{
					"Schema": 
					{
						"files": "Assets/Audio/sfx/jump_|0-4|.wav",
						"keys": "|0-4|"
					}
				}
			},
			"effects": 
			{
				"jumpTune": 
				{
					"param": "tune",
					"min": -0.1,
					"max": 0.75
				}
			}
		}
	}
}
)"_json;

		menu = new SoundContainer(menuJson);
		game = new SoundContainer(gameJson);
		sfx = new SoundContainer(sfxJson);

		// todo: move to schema args
		menu[menu->size() - 1].Loop(-1);
		game[game->size() - 1].Loop(-1);

		// todo: simplify, add scene init to AddScene
		auto MenuScene = SoundEngine::Scene("MenuScene", menu);
		auto GameScene = SoundEngine::Scene("GameScene", game);
		auto SFXScene = SoundEngine::Scene("SFX", sfx);

		/************
			START IS CREATING SHALLOW COPIES
			instance->StartScene(this) is causing issue
		
		*/


		// todo: AddScenes
		MenuScene.Start();
		GameScene.Start();
		SFXScene.Start();

		GameSceneContainer = &SoundEngine::Containers["GameScene"];

		

		//SoundEngine::Scene& scene = SoundEngine::Scenes["GameScene"];

		//GameScene.Stop();

		//  tODO: make From/OnStateChange to return a reference to newly created
		//		functionoids to handle deletion in parent

	}


	SoundContainer* GameSceneContainer;

	void ToGame()
	{
		SoundEngine::Containers["GameScene"]();
	}

	void ToMenu()
	{
		// first one works, second one throws read access issue
		GameSceneContainer->Stop();

		SoundContainer* ptrcontainer = &SoundEngine::Containers["GameScene"];
		ptrcontainer->Stop();

		SoundContainer& container = SoundEngine::Containers["GameScene"];
		container.Stop();


		SoundEngine::Containers["GameScene"].Stop();
	}
	
	~GameAudio()
	{
		engine->Release(); // should clear menu/game/sfx?
	}


};

