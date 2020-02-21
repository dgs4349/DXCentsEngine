#pragma once

#include <map>

namespace FlashBang {

    /*
        RELEASE 0 TODO:
            - Generic numerical/vector type for effect
            - Reverb + positional audio
            - Dynamic loading/unloading of container children through SoundEngine
                - Number of queues ahead to load
                - number of queues after to load
                - sound by sound flag as safe to unload/load
            - Debug lol
                - Needs some kind of automated or otherwise unit testing
                    - application which loads scenarios, tells the qa/tester
                        what desired behavior/sound should be
                    - This _has_ to be done by ear, because audio parameters and
                        correct behavior can't always be detected in code
                        - examples: 
                            - does this sound actually play/sound correct
                            - is this reverb correct
                            - do position/velocity *of audio* match the visuals
                            - is there audio input delay
                        - these *could* be detected by tapping into audio device, but
                            it will be *SO* much easier to do this manually/by ear

        RELEASE 1 TODO:
            - Wavebanks, ogg, and mp3 support
            - Investigate multithreading/threadsafe operations to more implicitly hook into game
            - GameParam type, where it can be supplied game side and be connected automatically
                - we could perhaps automatically assign behavior flags to implicitly connect game vars
                - but this would be getting really close to other common implementations, and adds a
                    layer of abstraction that might not be necessary. But will explore anyways
    
    
    */








    // basically longer than ever needed, but should still allow in-bounds int arithmatic
    const int LOOP_CAP = 1000000000;

    // forward declarations, so far no need to declare other functionality in forwards
    extern class Sound; // includes SoundObject, effects
    extern class SoundContainer;
    extern class SoundEngine;

    // yes this one's plural, it just makes more sense in this instance
	enum class COMMON_EFFECTS : char { TIME='T' };

    // basic parameters are lowercase to match common arg rules
    enum class EFFECT_PARAMETER : char { VOLUME='v', TUNE='t', PAN='p' };

    // this is an int class, which is useful with state_ < READY
	enum class SOUND_STATE { UNLOADED, IDLE, PAUSED, QUEUED, READY, PLAYING, FINISHING, COMPLETE };

    // char for parsing
    enum class SOUNDCONTAINER_TYPE : char { PLAYLIST= 'P', ONE_SHOT= 'O' };
    enum class SOUNDCONTAINER_PLAYBACK : char { IN_ORDER='I', RANDOM='R', RANDOM_EACH='E', RANDOM_OTHER='O' };

	/* investigate including these
	
    static const XAUDIO2FX_REVERB_I3DL2_PARAMETERS gReverbPresets[] =
    {
        XAUDIO2FX_I3DL2_PRESET_DEFAULT,             // Reverb_Off
        XAUDIO2FX_I3DL2_PRESET_DEFAULT,             // Reverb_Default
        XAUDIO2FX_I3DL2_PRESET_GENERIC,             // Reverb_Generic
        XAUDIO2FX_I3DL2_PRESET_FOREST,              // Reverb_Forest
        XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,          // Reverb_PaddedCell
        XAUDIO2FX_I3DL2_PRESET_ROOM,                // Reverb_Room
        XAUDIO2FX_I3DL2_PRESET_BATHROOM,            // Reverb_Bathroom
        XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,          // Reverb_LivingRoom
        XAUDIO2FX_I3DL2_PRESET_STONEROOM,           // Reverb_StoneRoom
        XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,          // Reverb_Auditorium
        XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,         // Reverb_ConcertHall
        XAUDIO2FX_I3DL2_PRESET_CAVE,                // Reverb_Cave
        XAUDIO2FX_I3DL2_PRESET_ARENA,               // Reverb_Arena
        XAUDIO2FX_I3DL2_PRESET_HANGAR,              // Reverb_Hangar
        XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,     // Reverb_CarpetedHallway
        XAUDIO2FX_I3DL2_PRESET_HALLWAY,             // Reverb_Hallway
        XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,       // Reverb_StoneCorridor
        XAUDIO2FX_I3DL2_PRESET_ALLEY,               // Reverb_Alley
        XAUDIO2FX_I3DL2_PRESET_CITY,                // Reverb_City
        XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,           // Reverb_Mountains
        XAUDIO2FX_I3DL2_PRESET_QUARRY,              // Reverb_Quarry
        XAUDIO2FX_I3DL2_PRESET_PLAIN,               // Reverb_Plain
        XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,          // Reverb_ParkingLot
        XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,           // Reverb_SewerPipe
        XAUDIO2FX_I3DL2_PRESET_UNDERWATER,          // Reverb_Underwater
        XAUDIO2FX_I3DL2_PRESET_SMALLROOM,           // Reverb_SmallRoom
        XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,          // Reverb_MediumRoom
        XAUDIO2FX_I3DL2_PRESET_LARGEROOM,           // Reverb_LargeRoom
        XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,          // Reverb_MediumHall
        XAUDIO2FX_I3DL2_PRESET_LARGEHALL,           // Reverb_LargeHall
        XAUDIO2FX_I3DL2_PRESET_PLATE,               // Reverb_Plate
    };
	*/

    /*
    We can also look into some quick default sound library includes
    https://github.com/SimonDarksideJ/XNAGameStudio/wiki/SoundLab
    */
}