#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "Shader_Program.h"
#include "camera.h"
#include "Light.h"
#include "Mesh.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
//TODO every model should also store the information they intend to pass onto the the shader program.
//when we define a model, it should also contain information about the material, shinniness, location, how it interacts with different lights, etc.
class Model {
	public:
		//CONSTRUCTOR
		Model(const char* path, glm::vec3 pLocal_Position, Shader_Program pProgram) {
			loadModel(path);
			local_position = pLocal_Position;
			program = pProgram;
		}
		Model(const char* path, glm::vec3 pLocal_Position, glm::vec3 pDiffuse_Mat_reflectivity, glm::vec3 pObject_color, glm::vec3 pSpecular_mat_reflectivity, glm::vec3 pAmbient_mat_reflectivity, float pShininnes , Shader_Program pProgram) {
			loadModel(path);
			local_position = pLocal_Position;
			diffuse_mat_reflectivity = pDiffuse_Mat_reflectivity;
			object_color = pObject_color;
			specular_mat_reflectivity = pSpecular_mat_reflectivity;
			ambient_mat_reflectivity = pAmbient_mat_reflectivity;
			shininnes = pShininnes;
			program = pProgram;
		}
		//LOCATION INFO
		glm::vec3 local_position;
		glm::mat4 get_model_matrix();

		//material info, this should be stored in the rendering info
		glm::vec3 diffuse_mat_reflectivity;
		glm::vec3 object_color;
		glm::vec3 specular_mat_reflectivity;
		glm::vec3 ambient_mat_reflectivity;
		float shininnes;

		void set_camera_info(fpv_camera camera, float win_width, float win_height);
		void set_material_info(); //TODO refine way to change material info, right now it sets everything up whenever we call it
		void set_light_info(light pLight);

		//SHADING AND RENDERING
		//we store the different rendering information the model would need to be properly rendered. This info is later passed to the shader program.
		std::map<std::string, glm::mat4> mat4_rendering_info = {};
		std::map<std::string, glm::mat3> mat3_rendering_info = {};
		std::map<std::string, glm::vec4> vec4_rendering_info = {};
		std::map<std::string, glm::vec3> vec3_rendering_info = {};
		std::map<std::string, float> float_rendering_info = {};
		void set_rendering_info();
		void draw();
	private:
		
		//mesh load aux members and methods
		std::vector<Mesh> meshes;
		std::string directory;

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);

		Shader_Program program;
};