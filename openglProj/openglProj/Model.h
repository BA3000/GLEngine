#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include <string>
#include <iostream>

#include "stb_image.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"


unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model
{
public:
	bool gammaCorrection;
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	Model(std::string const &path, bool gamma = false) : gammaCorrection(gamma) {
		loadModel(path);
	};
	void Draw(Shader* shader);
	~Model();

private:
	void loadModel(std::string const path);
	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};


#endif