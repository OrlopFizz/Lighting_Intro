#pragma once
#include <glm.hpp>
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <gtc/type_ptr.hpp>
//to create a shader program, i need to read and compile a given GLSL source code for both vertex and fragment shader
//link them together, and then create a shader program. After this, its good practice to delete both compiled shaders 
class Shader_Program {
	public:
		unsigned int shader_program_id {0};
	
		Shader_Program(const char* vert_shader_path, const char* frag_shader_path); //constructor, does everything and ultimately generates the shader_program_id
	
		void Read_Compile_Shader_Source(const char* vert_shader_path, const char* frag_shader_path);
		void Link_Create_Shader_Program(unsigned int vert_shader, unsigned int frag_shader);

		bool Use_Program();
	
		void Set_Bool(const std::string& name, bool value) const;
		void Set_Int(const std::string& name, int value) const;
		void Set_Float(const std::string& name, float value) const;
		void Set_Mat4(const std::string& name, glm::mat4 matrix_data) const;
		void Set_Mat3(const std::string& name, glm::mat3 matrix_data) const;
		void Set_Vec3(const  std::string& name, glm::vec3 vec_data) const;
		void Set_Vec4(const std::string& name, glm::vec4 vec_data);
};