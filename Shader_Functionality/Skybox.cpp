#include "Skybox.h"
/*
#include "stb_image.h"

#include <glad/glad.h>
#include <iostream>
*/
/*
2, 6, 7,2, 3, 7,0, 4, 5,0, 1, 5,0, 2, 6,0, 4, 6,1, 3, 7,1, 5, 7,0, 2, 3,0, 1, 3,4, 6, 7,4, 5, 7
*/
Skybox::Skybox(std::map<const char*, const char*> pMap, Shader_Program pProgram) {
	path_dictionary = pMap;
	program = pProgram;
	//set vertex info
	/*
	Vertex* vert_1 = new Vertex();
	vert_1->position_coords = glm::vec3(-1.0f, -1.0f, 0.5f);
	Vertex* vert_2 = new Vertex();
	vert_1->position_coords = glm::vec3(1.0f, -1.0f, 0.5f);
	Vertex* vert_3 = new Vertex();
	vert_1->position_coords = glm::vec3(-1.0f, 1.0f, 0.5f);
	Vertex* vert_4 = new Vertex();
	vert_1->position_coords = glm::vec3(1.0f, 1.0f, 0.5f);
	Vertex* vert_5 = new Vertex();
	vert_1->position_coords = glm::vec3(-1.0f, -1.0f, -0.5f);
	Vertex* vert_6 = new Vertex();
	vert_1->position_coords = glm::vec3(1.0f, -1.0f, -0.5f);
	Vertex* vert_7 = new Vertex();
	vert_1->position_coords = glm::vec3(-1.0f, 1.0f, -0.5f);
	Vertex* vert_8 = new Vertex();
	vert_1->position_coords = glm::vec3(1.0f, 1.0f, -0.5f);

	vertex_list.push_back(*vert_1);
	vertex_list.push_back(*vert_2);
	vertex_list.push_back(*vert_3);
	vertex_list.push_back(*vert_4);
	vertex_list.push_back(*vert_5);
	vertex_list.push_back(*vert_6);
	vertex_list.push_back(*vert_7);
	vertex_list.push_back(*vert_8);
	*/
	vertex_list = { -1.0f, -1.0f, 0.5f,
1.0f, -1.0f, 0.5f,
-1.0f, 1.0f, 0.5f,
1.0f, 1.0f, 0.5f,
-1.0f, -1.0f, -0.5f,
1.0f, -1.0f, -0.5f,
-1.0f, 1.0f, -0.5f,
1.0f, 1.0f, -0.5f };
	//set indices info
	indexes_list = { 2, 6, 7, 2, 3, 7, 0, 4, 5, 0, 1, 5, 0, 2, 6, 0, 4, 6, 1, 3, 7, 1, 5, 7, 0, 2, 3, 0, 1, 3, 4, 6, 7, 4, 5, 7 };

	//set cube vertex data
	set_cube_vertex_data();
	texture_id = set_textures();
}

void Skybox::set_cube_vertex_data() {
	//create buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind the buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//populate the buffers
	glBufferData(GL_ARRAY_BUFFER, vertex_list.size() * sizeof(float), &vertex_list[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_list.size() * sizeof(unsigned int), &indexes_list[0], GL_STATIC_DRAW);
	
	//setup the vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(NULL);
	std::cout << "CREATED VAO WITH ID " << VAO << '\n';
}

//given faces tectures, we load this textures
unsigned int Skybox::set_textures() {
	unsigned int texture_id;

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); //R equates to Z dimension

	for (auto dic_entry : path_dictionary) {
		const char* face = dic_entry.first;
		const char* path = dic_entry.second;
		//read texture
		int width, height, nrchannels;
		unsigned char* texture_image = stbi_load(path, &width, &height, &nrchannels, 0);
		if (!texture_image) {
			std::cout << "SKYBOX FACE " << face << " TEXTURE FAILED TO LOAD" << '\n';
			return -1;
		}
		float format = GL_RGB;
		if (nrchannels == 4) {
			format = GL_RGBA;
		}
		std::cout << "SKYBOX FACE " << face << " NUMBER OF CHANNELS " << nrchannels << '\n';
		int target = target_dictionary[face];
		glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_image);
		stbi_image_free(texture_image);
	}
	return texture_id;
}

void Skybox::set_rendering_info() {
	for (const auto& info : mat4_rendering_info) {
		program.Set_Mat4(info.first, info.second);
	}

	for (const auto& info : mat3_rendering_info) {
		program.Set_Mat3(info.first, info.second);
	}

	for (const auto& info : vec4_rendering_info) {
		program.Set_Vec4(info.first, info.second);
	}

	for (const auto& info : vec3_rendering_info) {
		program.Set_Vec3(info.first, info.second);
	}

	for (const auto& info : float_rendering_info) {
		program.Set_Float(info.first, info.second);
	}
}

void Skybox::draw() {
	glActiveTexture(GL_TEXTURE1);
	glBindTextureUnit(1, texture_id);
	glDepthMask(GL_FALSE);
	program.Use_Program();
	set_rendering_info();

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexes_list.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);
	glDepthMask(GL_TRUE);
}