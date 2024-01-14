#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


#include <map>
#include <string>
#include <vector>

#include "../glad/include/glad/glad.h"
#include "../game_structures/bullet.h"

#include "../shaders_textures/shader.h"
#include "../shaders_textures/texture.h"




// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager
{
public:
    // resource storage
    static std::map<std::string, Shader>    Shaders;
    static std::map<std::string, Texture2D> Textures;
    static std::map<std::string, std::vector<int>> Levels;
    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader    LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
    // retrieves a stored sader
    static Shader    GetShader(std::string name);
    // loads (and generates) a texture from file
    static Texture2D LoadTexture(const char *file, bool alpha, std::string name);
    // retrieves a stored texture
    static Texture2D GetTexture(std::string name);
    
    static std::vector<int> LoadBulletList(const char* file, std::string name);

    // retrieves the list of bullets of a level
    static std::vector<int> GetBulletList(std::string name);
    // properly de-allocates all loaded resources
    static void      Clear();

   

    static void writeBulletListOnFile(const char* file, std::vector<int> list);
private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() { }
    // loads and generates a shader from file
    static Shader    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    // loads a single texture from file
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
    
    
   
    static std::vector<int> loadBulletListFromFile(const char* file);
    

    
    
    

};

#endif