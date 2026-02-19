#pragma once
#include<raylib.h>
#include<iostream>
#include<string>
#include<unordered_map>
#include<filesystem>


class ResourceManager
{
	static std::unordered_map<std::string, Texture2D> textures;
	static std::unordered_map<std::string, Sound> sounds;
	static std::unordered_map<std::string, Font> fonts;
	static std::unordered_map<std::string, Music> musics;
	static void LoadTextures();
	static void LoadSounds();
	static void LoadFonts();
	static void LoadMusic();

public:
	static void Load();
	static void Unload();
	static Texture2D& GetTexture(const std::string&);
	static Sound GetSound(const std::string);
	static Music GetMusic(const std::string);
};

inline std::unordered_map<std::string, Texture2D> ResourceManager::textures;
__declspec(selectany) std::unordered_map<std::string, Sound> ResourceManager::sounds;
__declspec(selectany) std::unordered_map<std::string, Font> ResourceManager::fonts;
inline std::unordered_map<std::string, Music> ResourceManager::musics;