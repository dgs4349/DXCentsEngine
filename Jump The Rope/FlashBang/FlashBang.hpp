#pragma once

#include <map>

namespace FlashBang {

    /*
        RELEASE 0 TODO:
            - move parse logic to a function call during load
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


    // forward declarations, so far no need to declare other functionality in forwards
    extern class Sound; // includes SoundObject, effects
    extern class SoundContainer;
    extern class SoundEngine;


    /*
        - Global Enums:
            - SOUND_STATE,
                - the current playback state of a sound:
                   - UNLOADED:  audio content is not loaded
                   - IDLE:      audio content is loaded but not scheduled to play
                   - PAUSED:    audio is paused
                   - QUEUED:    audio is next to be played after a previous SoundObject
                   - READY:     audio will be played on the following update frame
                   - PLAYING:   audio is playing
                   - FINISHING: audio is on the final playback loop
                   - COMPLETE:  audio has completed its loops, and will transition to IDLE
           
           - SOUND_PARAM:
                - parameters for audio content

            - AUTO_EFFECT:
                - audio effects independent of game vars, i.e. time, lfo's, other effects
                    - TIME: linear effect based on elapsed time

            - SOUNDCONTAINER_GROUP_TYPE:
                - SoundObject group type contained in SoundContainer json 'items' param
                    - CONTAINER: target item is another SoundContainer
                    - SCHEMA:    group of SoundObjects to be parsed from file and name schemas
            
            - SOUNDCONTAINER_PLAYBACK_BEHAVIOR:
                - defines the behavior when a child is played
                    - PLAYLIST: horizontal container, soundobjects are queued to be played
                                    following the previous sound's completion
                    - ONE_SHOT: sounds are independently triggered
    */


    enum class SOUND_STATE { UNLOADED, IDLE, PAUSED, QUEUED, READY, PLAYING, FINISHING, COMPLETE };
    enum class SOUND_PARAM : char { VOLUME = 'v', TUNE = 't', PAN = 'p', LOOP = 'l'};

    enum class AUTO_EFFECT : char { TIME = 'T' };

    enum class SOUNDOBJECT_ARG : char {
        FILE = 'f', KEY = 'k', EFFECTS = 'e', 
    };
    enum class EFFECT_ARG : char {
        KEY= 'k', PARAM= 'p', MIN = 'l', MAX = 'h', // 'low', 'high'
    };

    enum class SOUNDCONTAINER_ARG : char {
        PLAYBACK_BEHAVIOR = 'b',
        PLAYBACK_ORDER = 'o',
        ITEMS = 'i',
    };

    enum class SOUNDCONTAINER_ITEM_TYPE : char { CONTAINER, SCHEMA, SOUND, };
    static const std::map<std::string, SOUNDCONTAINER_ITEM_TYPE> SOUNDCONATINER_ITEM_TYPE_ARGS = {
        {"Container", CONTAINER},
        {"Schema", SCHEMA},
        {"Sound", SOUND}, // will be implied if previous items do not match
    }; 

       
    enum class SOUNDCONTAINER_PLAYBACK_BEHAVIOR : char { PLAYLIST = 'P', ONE_SHOT = 'O' };
    enum class SOUNDCONTAINER_PLAYBACK_ORDER : char { IN_ORDER = 'I', RANDOM = 'R', RANDOM_EACH = 'E', RANDOM_OTHER = 'O' };


    // char for parsing
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