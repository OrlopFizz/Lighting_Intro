#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> pverts, std::vector<unsigned int> pind, std::vector<Texture> ptex)
{
	(*this).vertices = pverts;
	(*this).indices = pind;
	(*this).textures = ptex;
	SetUpMesh();
}


//setup all buffers, VAO, VBO, AND EBO to render. Also setup vertex attribs
void Mesh::SetUpMesh()
{
	//generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind the buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	
	//populate the buffers
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//setup the vertex positions
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal_coords));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));

	glBindVertexArray(NULL);
}

void Mesh::draw(Shader_Program& pshader)
{	
	
	//apriori, we dont know how many textures a mesh could use, but since we know opengl eliminates unused uniforms. we define some uniforms(the maximum we plan to use on all shaders)
	//and if a shader really uses all those shaders, there is samplers named with naming convention texture_*type**number*. eg: texture_diffuse1, texture_specular2
	unsigned int diffuse_num = 1;
	unsigned int specular_num = 1;
	for (unsigned int i = 0; i < textures.size(); ++i) { //all textures in the mesh
		glActiveTexture(GL_TEXTURE0 + i); //we can actually sum texture targets to get the current one, if i = 1, then we are targeting GL_TEXTURE1
		std::string number; //number of the texture
		std::string name = textures[i].type; //the type of texture

		if (name == "texture_diffuse") {
			number = std::to_string(diffuse_num++); //get the number as the diffuse_texture to target, and also add 1 to the count
		}
		else if (name == "texture_specular") {
			number = std::to_string(specular_num++); //get the number as the specular_texture to target, and also add 1 to the count
		}
		//glBindTexture(GL_TEXTURE_2D, textures[i].id);
		glBindTextureUnit(i, textures[i].id);
	}

	pshader.Use_Program();
	//draw the mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(NULL);

}