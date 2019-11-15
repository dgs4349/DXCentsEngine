#include "ALAudioHandler.h"



ALAudioHandler::ALAudioHandler()
{
}

ALAudioHandler::ALAudioHandler(PLATFORM platform)
{
}


ALAudioHandler::~ALAudioHandler()
{
}

void ALAudioHandler::Init()
{
	// Initialization
	Device = alcOpenDevice(NULL); // select the "preferred device"

	if (Device) {
		Context = alcCreateContext(Device, NULL);
		alcMakeContextCurrent(Context);
	}

	//// Check for EAX 2.0 support
	//g_bEAX = alIsExtensionPresent("EAX2.0");

	// Generate Buffers
	alGetError(); // clear error code

	ALenum error = AL_NO_ERROR;

	alGenBuffers(NUM_BUFFERS, g_Buffers);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alGenBuffers :", error);
		return;
	}

	// Load test.wav
	LoadWAVFILE("test.wav", &format, &data, &size, &freq, &loop);
	alex

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alutLoadWAVFile test.wav : ", error);
		alDeleteBuffers(NUM_BUFFERS, g_Buffers);
		return;
	}

	// Copy test.wav data into AL Buffer 0
	alBufferData(g_Buffers[0], format, data, size, freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alBufferData buffer 0 : ", error);
		alDeleteBuffers(NUM_BUFFERS, g_Buffers);
		return;
	}

	// Unload test.wav
	unloadWAV(format, data, size, freq);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alutUnloadWAV : ", error);
		alDeleteBuffers(NUM_BUFFERS, g_Buffers);
		return;
	}

	// Generate Sources
	alGenSources(1, source);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alGenSources 1 : ", error);
		return;
	}

	// Attach buffer 0 to source
	alSourcei(source[0], AL_BUFFER, g_Buffers[0]);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		DisplayALError("alSourcei AL_BUFFER 0 : ", error);
	}

	// Exit
	Context = alcGetCurrentContext();
	Device = alcGetContextsDevice(Context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

void ALAudioHandler::Update(float deltaTime, float totalTime)
{
}

void ALAudioHandler::Suspend()
{
}

void ALAudioHandler::Resume()
{
}

void ALAudioHandler::LoadWAVFILE()
{
	switch (CURRENT_PLATFORM) {
	case WINDOWS:
			LoadWAVFILE()
		break;
	case MAC:
		break;
	case LINUX:

	}
}

void ALAudioHandler::SetListener(int index)
{
	SetListener(Listeners[index]);
}

void ALAudioHandler::SetListener(AL_LISTENER listener)
{
	alListenerfv(AL_POSITION, listener.POS);
	alListenerfv(AL_VELOCITY, listener.VEL);
	alListenerfv(AL_ORIENTATION, listener.ORI);
}
