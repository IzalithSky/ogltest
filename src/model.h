#ifndef MODEL_H
#define MODEL_H

#include <assimp/scene.h>

#include "mesh.h"
#include "ShaderProgram.h"

#include <string>
#include <vector>

class Model {
public:
    std::vector<Texture> textures_loaded;
    std::vector<Mesh> meshes;
    std::string directory;

    Model(std::string const &path, ShaderProgram *shader, bool flipTexture = true);

    void Draw();
private:
    ShaderProgram *shader;
    bool flipTexture;
    
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
