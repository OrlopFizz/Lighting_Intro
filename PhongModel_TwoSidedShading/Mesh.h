#include <glm.hpp>
#include <string>
#include <vector>
#include "Shader_Program.h"
#pragma once

//WE DEFINE CERTAIN STRUCTS TO HELP US DEFINE MESHES. DEFINED AS THE SIMPLEST DRAWABLE ELEMENT INSIDE A 3D OBJECT MODEL. A MODEL BEING MADE FROM ONE OR MORE MESHES
//WE CREATE STRUCTS FOR VERTICES, AND TEXTURES.

struct Vertex {
	glm::vec3 position_coords{}; //position of the vertex
	glm::vec3 normal_coords{}; //vector pointing to the normal of the vertex
	glm::vec2 texture_coords{}; // coordinate for the texture UV mapping on that vertex
};

struct Texture {
	unsigned int id; //id of the texture program
	std::string type; //the type, can be a diffuse or specular map
	std::string path;
};

class Mesh {
	public:
		//mesh data
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> pverts, std::vector<unsigned int>pind, std::vector<Texture> ptex);
		void draw(Shader_Program &pshader);//passing a shader so we can set various uniforms before drawing.
	private:
		//render data for this mesh
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
		
		void SetUpMesh(); //set up the buffers
};

