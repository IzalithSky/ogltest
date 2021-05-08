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
    bool gammaCorrection;

    Model(std::string const &path);

    void Draw(ShaderProgram *shader);
private:
    void loadModel(std::string const &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
