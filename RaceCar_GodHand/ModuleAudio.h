#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	// Plays a music file
	bool PlayMusic(const char* path, int loops = -1, float fadeTime = DEFAULT_MUSIC_FADE_TIME);
	bool HasFinished();

	// Load/Unload a WAV in memory
	uint LoadFx(const char* path);
	bool UnloadFx(uint index);
	bool SetFxVolume(uint index);

	// Get/Set music volume
	int GetMusicVolume();
	void SetMusicVolume(int volume);

	// Get/Set fx volume
	int GetFxVolume();

	// Play a previously loaded WAV
	int PlayFx(uint fx, int repeat = 0);
	int StopFx(int channel);

private:

	Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;

	int volumeFx;
};

#endif // __ModuleAudio_H__