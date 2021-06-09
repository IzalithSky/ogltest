#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

#include "mesh.h"
#include "ShaderProgram.h"
#include "logger.h"

#include <string>
#include <vector>
#include <set>

class Model {
public:
    std::set<Texture> texturesInfo;
    std::vector<Mesh> meshes;
    std::string directory;
    bool flipTexture;

    Model(std::string const &path, bool flipTexture = true);
private:
    Logger log;
    
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(
        aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
