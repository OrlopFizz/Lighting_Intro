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
		//CONSTRUCTOR
		Model(const char* path, glm::vec3 pLocal_Position) {
			loadModel(path);
			local_position = pLocal_Position;
		}
		//LOCATION INFO
		glm::vec3 local_position;
		glm::mat4 get_model_matrix();
		//SHADING AND RENDERING
		void draw(Shader_Program program);
	private:
		
		//mesh load aux members and methods
		std::vector<Mesh> meshes;
		std::string directory;

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);
};