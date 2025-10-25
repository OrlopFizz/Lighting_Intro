#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "Shader_Program.h"
#include "Mesh.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
class Model {
	public:
		Model(const char* path) {
			loadModel(path);
		}
		void draw(Shader_Program program);
	private:
		std::vector<Mesh> meshes;
		std::string directory;

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
};