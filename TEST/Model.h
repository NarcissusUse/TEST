#pragma once
#include"Mesh.h"
#include "stb_image.h"
class Model
{
public:
	Model(string const &path);
	void Draw(Shader &shader);

private:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;

	//function
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char* path, const string& directory);

};