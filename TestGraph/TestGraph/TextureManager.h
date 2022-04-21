#define NOMINMAX 1
#include <TGUI/TGUI.hpp>
#include <unordered_map>
#include <map>
#include <string>

class TextureManager
{
	static std::unordered_map<std::string, sf::Texture> textures;
public:
	static void LoadTexture(std::string textureName);
	static sf::Texture& GetTexture(std::string textureName);
	static void Clear();
};

