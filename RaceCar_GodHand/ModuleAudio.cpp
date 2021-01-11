#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"

#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ModuleAudio::ModuleAudio(Application* app, bool start_enabled) : Module(app, start_enabled), music(NULL)
{}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init()
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// load support for the OGG format
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	p2List_item<Mix_Chunk*>* item;

	for(item = fx.getFirst(); item != NULL; item = item->next)
	{
		Mix_FreeChunk(item->data);
	}

	fx.clear();
	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}

bool ModuleAudio::PlayMusic(const char* path, int loops, float fade_time)
{
	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		return false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, loops, (int)(fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				return false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, loops) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				return false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return true;
}

uint ModuleAudio::LoadFx(const char* path)
{
	uint ret = 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count();
	}

	return ret;
}

bool ModuleAudio::UnloadFx(uint index)
{
	Mix_Chunk* chunk = NULL;

	if (fx.at(index - 1, chunk) == true)
	{
		Mix_FreeChunk(chunk);
		return fx.del(fx.findNode(chunk));
	}

	return false;
}

int ModuleAudio::PlayFx(uint id, int repeat)
{
	Mix_Chunk* chunk = NULL;

	if (fx.at(id - 1, chunk) == true)
	{
		if (id > 0 && id <= fx.count())
		{
			return Mix_PlayChannel(-1, chunk, repeat);
		}
	}
}

int ModuleAudio::StopFx(int channel)
{
	return Mix_HaltChannel(channel);
}

bool ModuleAudio::SetFxVolume(uint index)
{
	Mix_Chunk* chunk = NULL;

	if (fx.at(index - 1, chunk) == true)
	{
		Mix_VolumeChunk(chunk, volumeFx);
		return true;
	}

	return false;
}

int ModuleAudio::GetMusicVolume()
{
	return Mix_VolumeMusic(-1);
}

void ModuleAudio::SetMusicVolume(int volume)
{
	Mix_VolumeMusic(volume);
}

int ModuleAudio::GetFxVolume()
{
	return volumeFx;
}

bool ModuleAudio::HasFinished()
{
	switch (Mix_PlayingMusic())
	{
	case 0:
		return true;
	case 1:
		return false;
	default:
		return false;
	}
}