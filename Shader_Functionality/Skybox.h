#pragma once
#include <vector>
#include <map>
#include <string>
#include <glad/glad.h>
#include "Mesh.h"

#include "stb_image.h"

#include <glad/glad.h>
#include <iostream>
/*
A skybox consists of the 6 textures that form a cube. 
we map every texture with its face, and the OpenGL texture target to render it
maps are populated with the following strings:
->up : up_texture_path : GL_TEXTURE_CUBE_MAP_POSITIVE_Y
->down : down_texture_path : GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
->front : front_texture_path : GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
->back : bottom_texture_path : GL_TEXTURE_CUBE_MAP_POSITIVE_Z
->right : right_texture_path : GL_TEXTURE_CUBE_MAP_POSITIVE_X
->left : left_texture_path : GL_TEXTURE_CUBE_MAP_NEGATIVE_X
*/
class Skybox {
	public:
		Skybox(std::map<const char*, const char*> pMap, Shader_Program pProgram);
		//we store the different rendering information the model would need to be properly rendered. This info is later passed to the shader program.
		std::map<std::string, glm::mat4> mat4_rendering_info = {};
		std::map<std::string, glm::mat3> mat3_rendering_info = {};
		std::map<std::string, glm::vec4> vec4_rendering_info = {};
		std::map<std::string, glm::vec3> vec3_rendering_info = {};
		std::map<std::string, float> float_rendering_info = {};
		void set_rendering_info();
		void draw();
	private:
		std::map<const char*, const char*> path_dictionary;
		std::map<const char*, int> target_dictionary = {
				{"front", GL_TEXTURE_CUBE_MAP_POSITIVE_Z},
				{"back", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z},
				{"right", GL_TEXTURE_CUBE_MAP_POSITIVE_X},
				{"left", GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
				{"up", GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
				{"down", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y}
		};
		//render data id
		Shader_Program program;
		unsigned int VBO { 0 };
		unsigned int EBO { 0 };
		unsigned int VAO { 0 };

		unsigned int texture_id;

		std::vector<float> vertex_list;
		std::vector<unsigned int> indexes_list;

		void set_cube_vertex_data();
		unsigned int set_textures();
};