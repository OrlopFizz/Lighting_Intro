#include "Model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <typeinfo>

float distance_between_points(glm::vec3 p1, glm::vec3 p2) {
	float inner = pow((p2[0] - p1[0]),2) + pow((p2[1] - p1[1]), 2);
	return sqrt(inner);
}

unsigned int load_texture(const std::string& path) {
	//load the texture
	int width, height, nrchannels;
	unsigned char* texture_image = stbi_load(path.c_str(), &width, &height, &nrchannels, 0);
	if (!texture_image) {
		std::cout << "TEXTURE FAILED TO LOAD" << '\n';
		return -1;
	}
	unsigned int texture_id;
	
	glCreateTextures(GL_TEXTURE_2D, 1, &texture_id); //just creates n buffers of type target. does NOT bind them to that target
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//bind texture image data
	float format = GL_RGB;
	if (nrchannels == 4) {
		format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_image);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(texture_image);
	return texture_id;
}

glm::mat4 Model::get_model_matrix() {
	glm::mat4 model_matrix = glm::mat4(1.0f);
	model_matrix = glm::translate(model_matrix, local_position);
	return model_matrix;
}

void Model::set_camera_info(fpv_camera camera, float win_width, float win_height) {
	
	mat4_rendering_info["view"] = camera.get_view_matrix(); //este sale de la camara
	mat4_rendering_info["projection"] = camera.get_projection_matrix(win_width, win_height); //este sale de un calculo aparte de nuestro mundo, valdria la pena pasarlo a la camara
}

void Model::set_material_info() {
	mat4_rendering_info["model"] = get_model_matrix(); //este sale del modelo
	vec3_rendering_info["mat_reflectivity"] = diffuse_mat_reflectivity; //viene del objeto
	vec3_rendering_info["object_color"] = object_color; //es una caracteristica del modelo
	vec3_rendering_info["specular_mat_reflectivity"] = specular_mat_reflectivity; //es una propiedad del modelo
	vec3_rendering_info["ambient_mat_reflectivity"] = ambient_mat_reflectivity; //es una propiedad del modelo
	float_rendering_info["shininnes"] = shininnes; //es una propiedad del modelo
}

void Model::set_light_info(light pLight) {
	vec4_rendering_info["light_pos"] = pLight.light_pos;
	vec3_rendering_info["light"] = pLight.diffuse_light;
	vec3_rendering_info["specular_light"] = pLight.specular_light;
	vec3_rendering_info["ambient_light"] = pLight.ambient_light;
}

void Model::set_rendering_info() {
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

void Model::draw()
{
	//set up program uniforms
	program.Use_Program();
	set_rendering_info();
	for (Mesh mesh : meshes) {
		mesh.draw(program);
	}
}

void Model::loadModel(std::string path)
{
	//assimp imports the file using an importer and reading the file, it also receives certain hexes to mark if we want extra processing
	Assimp::Importer model_importer;
	const aiScene* scene = model_importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR IMPORTING THE SCENE WITH ASSIMP\n" << model_importer.GetErrorString() << '\n';
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//process all the meshes in the current node
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		//the meshes list in the node contains indices that actually correspond to the meshes list inside the scene. 
		//we could obviate all nodes, but this helps us establish the parent child relationship between nodes
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
		this->meshes.push_back(processMesh(mesh, scene));
	}
	//then process the nodes children
	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	float min_dist = 999999.0f;
	glm::vec3 min_vert = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 min_normal = glm::vec3(0.0f, 0.0f, 0.0f);
	//process vertices
	for (int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vert;

		vert.position_coords.x = mesh->mVertices[i].x;
		vert.position_coords.y = mesh->mVertices[i].y;
		vert.position_coords.z = mesh->mVertices[i].z;

		vert.normal_coords.x = mesh->mNormals[i].x;
		vert.normal_coords.y = mesh->mNormals[i].y;
		vert.normal_coords.z = mesh->mNormals[i].z;

		float dist = distance_between_points(vert.position_coords, glm::vec3(5.0f, 5.0f, 2.0f));
		if (dist < min_dist) {
			min_dist = dist;
			min_vert = vert.position_coords;
			min_normal = vert.normal_coords;
		}

		//are there texture coordinates?
		if (mesh->mTextureCoords[0]) {
			vert.texture_coords.x = mesh->mTextureCoords[0][i].x;
			vert.texture_coords.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			vert.texture_coords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vert);
	}

	//process indices
	for (int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//load textures
	if (mesh->mMaterialIndex >= 0) { //does the mesh have a material?
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		
		std::vector<Texture> diffuse_maps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

		std::vector<Texture> specular_maps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	}
	return *(new Mesh(vertices, indices, textures));
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name)
{
	std::vector<Texture> ret_vector;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		
		texture.id = load_texture(str.C_Str());
		texture.type = type_name;
		texture.path = str.C_Str();
		ret_vector.push_back(texture);
	}

	return ret_vector;
}
