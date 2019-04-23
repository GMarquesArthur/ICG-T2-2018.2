#include<vector>
#include <glm/glm.hpp>
#include<stdio.h>
// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <string>
#include <cstring>

struct objeto{
    
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec2> indices;
    
};

bool objetoloader(objeto * obj,char * path);

void pipeline( objeto* obj);