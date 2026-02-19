#include "Resources.h"

void ResourceManager::LoadTextures()
{
	const std::string basePath = "res/textures/";

	auto loadTexture = [&](const std::string& name, const std::string& fileName)
		{
			std::string fullPath = basePath + fileName;
			Texture2D texture = LoadTexture(fullPath.c_str());
			if (texture.id == 0)
			{
				TraceLog(LOG_ERROR, "Failed to load texture: %s", fullPath.c_str());
				return;
			}
			
			textures[name] = texture;
			TraceLog(LOG_INFO, "Loaded texture %s", name.c_str());
		};
	loadTexture("player", "player.png");
	loadTexture("player-1", "player-1.png");
	loadTexture("player-2", "player-2.png");
	loadTexture("player-3", "player-3.png");
	loadTexture("player-left", "player-left.png");
	loadTexture("player-left-1", "player-left-1.png");
	loadTexture("player-left-2", "player-left-2.png");
	loadTexture("player-left-3", "player-left-3.png");
	loadTexture("player-right", "player-right.png");
	loadTexture("player-right-1", "player-right-1.png");
	loadTexture("player-right-2", "player-right-2.png");
	loadTexture("player-right-3", "player-right-3.png");
	loadTexture("player-top", "player-top.png");
	loadTexture("player-top-1", "player-top-1.png");
	loadTexture("player-top-2", "player-top-2.png");
	loadTexture("player-top-3", "player-top-3.png");
	loadTexture("obstacle", "obstacle.png");
	loadTexture("grass", "grass.jpg");
	loadTexture("restart", "restart.png");
	loadTexture("exit", "exit.png");
	loadTexture("enemy", "enemy.png");
	loadTexture("charmander", "charmander.png");
	loadTexture("litten", "litten.png");
	loadTexture("potion", "potionHP.png");
	loadTexture("bulbasaur", "bulbasaur.png");
	loadTexture("squirtle", "squirtle.png");
	loadTexture("cubone", "cubone.png");
	loadTexture("flareon", "flareon.png");

}

void ResourceManager::LoadSounds()
{
	const std::string basePath = "res/sounds/";

	auto loadSound = [&](const std::string& name, const std::string& fileName)
		{
			std::string fullPath = basePath + fileName;
			Sound sound = LoadSound(fullPath.c_str());
			if (!IsSoundValid(sound))
			{
				TraceLog(LOG_ERROR, "Failed to load sound: %s", fullPath.c_str());
				return;
			}

			sounds[name] = sound;
			TraceLog(LOG_INFO, "Loaded sound %s", name.c_str());
		};
	loadSound("move", "move.wav");
	loadSound("attack-player", "attack-player.wav");
	loadSound("attack-enemy", "attack-enemy.wav");
	loadSound("special-player", "special-player.wav");
	loadSound("special-enemy", "special-enemy.wav");
	loadSound("win-in-battle", "win-in-battle.wav");
	loadSound("victory", "victory.wav");


}

void ResourceManager::LoadFonts()
{
	const std::string basePath = "res/fonts/";

	auto loadFont = [&](const std::string& name, const std::string& fileName)
		{
			std::string fullPath = basePath + fileName;
			Font font = LoadFont(fullPath.c_str());
			if (1)
			{
				TraceLog(LOG_ERROR, "Failed to load texture: %s", fullPath.c_str());
				return;
			}

			fonts[name] = font;
			TraceLog(LOG_INFO, "Loaded textur %s", name.c_str());
		};
}

void ResourceManager::LoadMusic()
{
	const std::string basePath = "res/musics/";
	auto loadMusic = [&](const std::string& name, const std::string& fileName)
		{
			std::string fullPath = basePath + fileName;
			Music music = LoadMusicStream(fullPath.c_str());
			if (!IsMusicValid(music))
			{
				TraceLog(LOG_ERROR, "Failed to load music: %s", fullPath.c_str());
				return;
			}

			musics[name] = music;
			TraceLog(LOG_INFO, "Loaded music %s", name.c_str());
		};
	loadMusic("playing", "playing.mp3");
	loadMusic("battle", "battle.mp3");
	loadMusic("lose", "lose.mp3");
	loadMusic("start", "start.mp3");
}

void ResourceManager::Load()
{
	LoadTextures();
	LoadSounds();
	LoadFonts();
	LoadMusic();
}

void ResourceManager::Unload()
{
	for (auto& texture : textures)
	{
		UnloadTexture(texture.second);
	}
	textures.clear();
	for (auto& sound : sounds)
	{
		UnloadSound(sound.second);
	}
	sounds.clear();
	for (auto& font : fonts)
	{
		UnloadFont(font.second);
	}
	sounds.clear();
	for (auto& music : musics)
	{
		UnloadMusicStream(music.second);
	}
	musics.clear();
}

Texture2D& ResourceManager::GetTexture(const std::string& name)
{
	if (textures.find(name) == textures.end())
	{
		TraceLog(LOG_ERROR, "Texture not found: %s", name.c_str());
		throw std::runtime_error("Texture not found" + name);
	}
	return textures.at(name);
}

Sound ResourceManager::GetSound(const std::string name)
{
	if (sounds.find(name) == sounds.end())
	{
		TraceLog(LOG_ERROR, "Sound not found: %s", name.c_str());
		throw std::runtime_error("Sound not found" + name);
	}
	return sounds.at(name);
}

Music ResourceManager::GetMusic(const std::string name)
{

	if (musics.find(name) == musics.end())
	{
		TraceLog(LOG_ERROR, "Music not found: %s", name.c_str());
		throw std::runtime_error("Music not found " + name);
	}
	return musics.at(name);
}
