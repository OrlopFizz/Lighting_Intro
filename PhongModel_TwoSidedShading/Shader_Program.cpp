#include "Shader_Program.h"
#include <typeinfo>
Shader_Program::Shader_Program(const char* vert_shader_path, const char* frag_shader_path) {
	Read_Compile_Shader_Source(vert_shader_path, frag_shader_path);
}

void Shader_Program::Read_Compile_Shader_Source(const char* vert_shader_path, const char* frag_shader_path) {
	std::ifstream vert_shader_stream;
	std::ifstream frag_shader_stream;

	std::string vertex_code;
	std::string fragment_code;

	std::stringstream string_stream;
	//READ VERTEX FILE
	try {
		vert_shader_stream.open(vert_shader_path);
		string_stream << vert_shader_stream.rdbuf();
		vertex_code = string_stream.str();
	}
	catch (std::ifstream::failure failure) {
		std::cout << "ERROR: VERTEX SHADER FILES NOT SUCCESFULLY READ";
	}
	string_stream.str("");
	string_stream.clear();

	//READ FRAGMENT FILE
	try {
		frag_shader_stream.open(frag_shader_path);
		string_stream << frag_shader_stream.rdbuf();
		fragment_code = string_stream.str();
	}
	catch (std::ifstream::failure failure) {
		std::cout << "ERROR: FRAGMENT SHADER FILES NOT SUCCESFULLY READ";
	}

	//CREATE THE SHADERS
	const char* shader_code_char = vertex_code.c_str();
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &shader_code_char, NULL);
	glCompileShader(vertex_shader);

	shader_code_char = fragment_code.c_str();
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &shader_code_char, NULL);
	glCompileShader(fragment_shader);

	int success;
	char infolog[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, 512, NULL, infolog);
		std::cout << "ERROR: VERTEX SHADER NOT COMPILED SUCCESFULLY\n" << infolog;
	}
	else {
		glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment_shader, 512, NULL, infolog);
			std::cout << "ERROR: FRAGMENT SHADER NOT COMPILED SUCCESSFULLY\n" << infolog;
		}
	}
	if (success) {
		Link_Create_Shader_Program(vertex_shader, fragment_shader);
	}
}

void Shader_Program::Link_Create_Shader_Program(unsigned int vert_shader, unsigned int frag_shader) {
	unsigned int temp_shader_program = glCreateProgram();
	glAttachShader(temp_shader_program, vert_shader);
	glAttachShader(temp_shader_program, frag_shader);
	glLinkProgram(temp_shader_program);

	int success;
	char infolog[512];
	glGetProgramiv(temp_shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(temp_shader_program, 512, NULL, infolog);
		std::cout << "ERROR: SHADER PROGRAM LINKING FAILED\n" << infolog;
	}
	else {
		shader_program_id = temp_shader_program;
		std::cout << "SHADER PROGRAM COMPILED AND LINKED SUCCESSFULLY. ID IS: " << shader_program_id << '\n';
		glDeleteShader(vert_shader);
		glDeleteShader(frag_shader);
	}
}

bool Shader_Program::Use_Program() {
	if (shader_program_id != 0) {
		glUseProgram(shader_program_id);
		return true;
	}
	else {
		std::cout << "A PROGRAM HAS NOT BEEN CREATED OR HAS FAILED TO BE CREATED" << '\n';
		return false;
	}
}

void Shader_Program::Set_Bool(const std::string& name, bool value) const {
	if (shader_program_id != 0) {
		int uniform_location = glGetUniformLocation(shader_program_id, name.c_str());
		glUniform1i(uniform_location, (int)value);
	}
	else {
		std::cout << "A PROGRAM HAS NOT BEEN CREATED OR HAS FAILED TO BE CREATED" << '\n';
	}
}

void Shader_Program::Set_Int(const std::string& name, int value) const {
	int uniform_location = glGetUniformLocation(shader_program_id, name.c_str());
	if (uniform_location == -1) {
		std::cout << "UNIFORM ERROR: UNIFORM WITH NAME " << name.c_str() << " NOT FOUND" << '\n';
	}
	else {
		glUniform1i(uniform_location, value);
	}
}

void Shader_Program::Set_Float(const std::string& name, float value) const {
	int uniform_location = glGetUniformLocation(shader_program_id, name.c_str());
	if (uniform_location == -1) {
		std::cout << "UNIFORM ERROR: UNIFORM WITH NAME " << name.c_str() << " NOT FOUND OR NOT ACTIVE" << '\n';
	}
	else {
		glUniform1f(uniform_location, value);
	}
}

void Shader_Program::Set_Mat4(const std::string& name, glm::mat4 matrix_data) const {
	int uniform_location = glGetUniformLocation(shader_program_id, name.c_str());
	if (uniform_location == -1) {
		std::cout << "UNIFORM ERROR: UNIFORM WITH NAME " << name.c_str() << " NOT FOUND OR NOT ACTIVE" << '\n';
	}
	else {
		glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(matrix_data));
	}
}

void Shader_Program::Set_Mat3(const std::string& name, glm::mat3 matrix_data) const {
	int uniform_location = glGetUniformLocation(shader_program_id, name.c_str());
	if (uniform_location == -1) {
		std::cout << "UNIFORM ERROR: UNIFORM WITH NAME " << name.c_str() << " NOT FOUND OR NOT ACTIVE" << '\n';
	}
	else {
		glUniformMatrix3fv(uniform_location, 1, GL_FALSE, glm::value_ptr(matrix_data));
	}
}

void Shader_Program::Set_Vec3(const std::string& name, glm::vec3 vec_data) const
{
	int uniform_location = glGetUniformLocation(shader_program_id, name.c_str());
	if (uniform_location == -1) {
		std::cout << "UNIFORM ERROR: UNIFORM WITH NAME " << name.c_str() << " NOT FOUND OR NOT ACTIVE" << '\n';
	}
	else {
		glUniform3fv(uniform_location, 1, glm::value_ptr(vec_data));
	}
}

void Shader_Program::Set_Vec4(const std::string& name, glm::vec4 vec_data) {
	int uniform_location = glGetUniformLocation(shader_program_id, name.c_str());
	if (uniform_location == -1) {
		std::cout << "UNIFORM ERROR: UNIFORM WITH NAME " << name.c_str() << " NOT FOUND OR NOT ACTIVE" << '\n';
	}
	else {
		auto val_pt = glm::value_ptr(vec_data);
		glUniform4fv(uniform_location, 1, glm::value_ptr(vec_data));
		//glUniform4f(uniform_location, vec_data[0], vec_data[1], vec_data[2], vec_data[3]);
	}
}
