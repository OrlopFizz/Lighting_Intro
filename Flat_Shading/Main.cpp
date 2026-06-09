#define _USE_MATH_DEFINES
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader_Program.h"
#include "glm.hpp"
#include "Model.h"
#include "imgui_wrapper.h"
#include "math.h"
#include "camera.h"
#include "Skybox.h"
#include "Line.h"

#include <iostream>
#include <map>
#include <typeinfo>
#include <fstream>
#include <sstream>

float lerp(float t, float x0, float x1) {
	return (1 - t) * x0 + t * x1;
}

float clamp(float t, float min, float max) {
	return glm::min(glm::max(t, min), max);
}

const char* getTypeString(GLenum type) {
	// There are many more types than are covered here, but
	// these are the most common in these examples.
	switch (type) {
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_DOUBLE:
		return "double";
	case GL_INT:
		return "int";
	case GL_UNSIGNED_INT:
		return "unsigned int";
	case GL_BOOL:
		return "bool";
	case GL_FLOAT_MAT2:
		return "mat2";
	case GL_FLOAT_MAT3:
		return "mat3";
	case GL_FLOAT_MAT4:
		return "mat4";
	default:
		return "?";
	}
}

glm::vec3 rotate_point(glm::vec3& camera_target, float rads, glm::vec3 axis) {
	glm::mat4 rotation_mat = glm::mat4(1.0f);
	rotation_mat = glm::rotate(rotation_mat, glm::radians(rads), axis);
	glm::vec3 ret_vector = glm::vec4(camera_target.x, camera_target.y, camera_target.z, 0.0f) * rotation_mat;
	return  ret_vector;
}

void print_matrix3(glm::mat4 pmatrix) {
	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			std::cout << pmatrix[x][y] << " ";
		}
		std::cout << '\n';
	}
}

void print_vector(glm::vec3 pvector) {
	for (int y = 0; y < 3; ++y) {
		std::cout << pvector[y] << " ";
	}
	std::cout << '\n';
}

void print_vector(glm::vec4 pvector) {
	for (int y = 0; y < 4; ++y) {
		std::cout << pvector[y] << " ";
	}
	std::cout << '\n';
}

std::map<unsigned int, bool> key_status = { {68, false}, {65, false}, {87, false}, {83, false}, {262, false}, {263, false}, {264, false},
											{265, false}, {322, false}, {324, false}, {326, false}, {GLFW_KEY_KP_4, false}, {GLFW_KEY_KP_6, false},
											{GLFW_KEY_KP_8, false}, {GLFW_KEY_KP_2, false}, {GLFW_KEY_KP_5, false} };

std::map<unsigned int, bool> key_status_2 = { {32, false} };

void process_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS && key_status.find(key) != key_status.end()) {
		key_status[key] = true;
	}
	else if (action == GLFW_RELEASE && key_status.find(key) != key_status.end()) {
		key_status[key] = false;
	}
	//non sticky keys
	if (action == GLFW_RELEASE && key_status_2.find(key) != key_status_2.end()) {
		key_status_2[key] = true;
	}
}

//double current_mouse_posx = 0.0f;
//double current_mouse_posy = 0.0f;
/*
double last_mouse_posx = 0.0f;
double last_mouse_posy = 0.0f;
std::map<const char*, bool> mouse_mov_status = { {"up", false}, {"down", false}, {"left", false}, {"right", false}};
*/
float old_mouse_pos[2] = {0.0f, 0.0f};
glm::vec2 mouse_movement_vector{ 0.0f, 0.0f };
int right_count = 0;
int left_count = 0;
fpv_camera* camera = new fpv_camera(glm::vec3(10.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), (M_PI / 180)/2, 0.05f);
void process_mouse_movement(GLFWwindow* window, double xpos, double ypos) {
	//TODO mouse movement working, but seems kinda snappy, this is due to the changes in direction being done instantly.
	//maybe lerp this shit?
	//depending on how much the mouse moves, we calculate a new angle to traverse to.
	//since we want the movement to be smooth, we must also lerp the angles between our current angle, and the new angle
	mouse_movement_vector = {old_mouse_pos[0] - xpos, ypos - old_mouse_pos[1]};
	
	float new_heading = camera->heading_angle + mouse_movement_vector[0] * 0.01f;
	float new_elevation = clamp(camera->elevation_angle + mouse_movement_vector[1] * 0.01f, -M_PI / 2, M_PI / 2);

	camera->change_heading_angle(new_heading);
	camera->change_elevation_angle(new_elevation);
	
	old_mouse_pos[0] = xpos;
	old_mouse_pos[1] = ypos;
}

std::string read_file(std::string path) {
	std::ifstream file(path);
	std::stringstream content_stream;
	content_stream << file.rdbuf();
	return content_stream.str();
}

/*
* ------------------------------//--------------------------
* ------------------------------//--------------------------
* ------------------------------//--------------------------
* MAIN LOOP
* ------------------------------//--------------------------
* ------------------------------//--------------------------
* ------------------------------//--------------------------
*/
int main() {
	float move_vel = 0.01f; //movement velocity

	//INIT GLFW AND CREATE AN OPENGL CONTEXT
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_API, GLFW_OPENGL_API);

	int win_width = 800;
	int win_height = 800;
	int half_width = 400;
	int half_height = 400;
	GLFWwindow* window = glfwCreateWindow(win_width, win_height, "lighting intro", NULL, NULL);
	if (window == NULL) { //AN ERROR OCCURED WHEN CREATING THE WINDOW
		std::cout << "FAILED TO CREATE A GLFW WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //make the window the current glfw and opengl context

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //load the opengl functions addresses, we pass the glfw function to get the addresses as a GLADLOADPROC
		std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
		return -1;
	}

	glfwSetKeyCallback(window, process_input);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, process_mouse_movement);

	int max_uniforms;
	glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &max_uniforms);

	const GLubyte* texto = glGetString(GL_VERSION);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glViewport(0, 0, win_width, win_height);
	glClearColor(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//set up the model
	//here we set up the various buffers and populate them with the info we find in the model
	glm::vec3 diffuse_mat_reflectivity = glm::vec3(0.9f, 0.5f, 0.3f); //this is basically the color of the object, i think.
	glm::vec3 ambient_mat_reflectivity = glm::vec3(0.9f, 0.5f, 0.3f);
	Model* teacup = new Model("C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\UTAH_BLEND.obj", glm::vec3(0.0f, 0.0f, 0.0f));
	Model* plane = new Model("C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\grass_plane.obj", glm::vec3(0.0f, 0.0f, 0.0f));
	float vert1[3] = {0.0f, 0.0f, 0.0f};
	float vert2[3] = {10.0f, 10.0f, 0.f};
	Line* line = new Line(vert1, vert2);

	std::map<const char*, const char*> texture_dict = {
		{"front", "C:\\Users\\Alejandro\\Pictures\\purple_skybox\\front.png"},
		{"back", "C:\\Users\\Alejandro\\Pictures\\purple_skybox\\back.png"},
		{"right", "C:\\Users\\Alejandro\\Pictures\\purple_skybox\\east.png"},
		{"left", "C:\\Users\\Alejandro\\Pictures\\purple_skybox\\west.png"},
		{"up", "C:\\Users\\Alejandro\\Pictures\\purple_skybox\\top.png"},
		{"down", "C:\\Users\\Alejandro\\Pictures\\purple_skybox\\bottom.png"}
	};
	Skybox* skybox = new Skybox(texture_dict);

	//CONFIGURE SHADER
	Shader_Program shader_program = Shader_Program("C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\Vertex_Shader.glsl", "C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\Fragment_Shader.glsl"); //this shader is for the teacup
	Shader_Program shader_program_plane = Shader_Program("C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\plane_vertex_shader.glsl", "C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\plane_fragment_shader.glsl"); //this shader is for the plane we walk on
	Shader_Program shader_program_skybox = Shader_Program("C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\skybox_vertex_shader.glsl", "C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\skybox_fragment_shader.glsl");
	Shader_Program shader_program_line = Shader_Program("C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\Line_Vertex_Shader.glsl", "C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Flat_Shading\\Line_Fragment_Shader.glsl");

	//aka model matrix. Transforms the object to the world space. Translates, rotates, and scales the object to the parameters we need on our final project
	glm::mat4 local_to_world = glm::mat4(1.0f);

	//NOTE! it is important to  reset these evey frame! so subsequent calls 
	//we then generate the view space, aka "the camera". we move all vertices to put them in place where we would want them to be from a camera perspective. this uses translations,
	//rotations, and scaling.
	glm::mat4 view_space = glm::mat4(1.0f);

	//define the camera coordinates and its values
	//fpv_camera* camera = new fpv_camera(glm::vec3(10.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.05f, 0.5f);

	float radious = 10.0;

	//the matrix we use to add perspective to our game, this also converts our vertices to NDC(-1.0 to 1.0). 
	glm::mat4 projection_space = glm::mat4(1.0f);

	float x_angle = 0.0f;
	float y_angle = 0.0f;
	float z_angle = 0.0f;

	//Light Source
	glm::vec4 light_pos = glm::vec4(0.0f, 0.0f, 10.0f, 1.0f);
	glm::vec3 diffuse_light = glm::vec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);

	//ambient light
	glm::vec3 ambient_light = glm::vec3(0.4f, 0.4f, 0.4f);

	//DEBUG ACTIVE UNIFORMS
	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };
	int numuniforms = 0;
	glGetProgramInterfaceiv(shader_program.shader_program_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numuniforms);
	std::cout << "Teacup Active Unforms: " << std::endl;
	for (int i = 0; i < numuniforms; ++i) {
		int results[4];
		glGetProgramResourceiv(shader_program.shader_program_id, GL_UNIFORM, i, 4, properties, 4, NULL, results);
		if (results[3] != -1) {
			continue;
		}
		int nameBuffsize = results[0] + 1;
		char* name = new char[nameBuffsize];
		glGetProgramResourceName(shader_program.shader_program_id, GL_UNIFORM, i, nameBuffsize, NULL, name);
		printf("%-5d %s (%s)", results[2], name, getTypeString(results[1]));
		std::cout << std::endl;
		delete[] name;
	}
	glGetProgramInterfaceiv(shader_program_plane.shader_program_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numuniforms);
	std::cout << "Plane Active Unforms: " << std::endl;
	for (int i = 0; i < numuniforms; ++i) {
		int results[4];
		glGetProgramResourceiv(shader_program_plane.shader_program_id, GL_UNIFORM, i, 4, properties, 4, NULL, results);
		if (results[3] != -1) {
			continue;
		}
		int nameBuffsize = results[0] + 1;
		char* name = new char[nameBuffsize];
		glGetProgramResourceName(shader_program_plane.shader_program_id, GL_UNIFORM, i, nameBuffsize, NULL, name);
		printf("%-5d %s (%s)", results[2], name, getTypeString(results[1]));
		std::cout << std::endl;
		delete[] name;
	}
	glGetProgramInterfaceiv(shader_program_skybox.shader_program_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numuniforms);
	std::cout << "Plane Active Unforms: " << std::endl;
	for (int i = 0; i < numuniforms; ++i) {
		int results[4];
		glGetProgramResourceiv(shader_program_skybox.shader_program_id, GL_UNIFORM, i, 4, properties, 4, NULL, results);
		if (results[3] != -1) {
			continue;
		}
		int nameBuffsize = results[0] + 1;
		char* name = new char[nameBuffsize];
		glGetProgramResourceName(shader_program_skybox.shader_program_id, GL_UNIFORM, i, nameBuffsize, NULL, name);
		printf("%-5d %s (%s)", results[2], name, getTypeString(results[1]));
		std::cout << std::endl;
		delete[] name;
	}
	//get instructions
	std::string instructions = read_file("C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Phong_Model\\instructions.txt");

	//RENDER LOOP
	bool change_imgui = false;
	bool show_imgui = false;
	imgui_wrapper* imgui = new imgui_wrapper(window);

	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//CHECK KEYBOARD INPUTS
		//TODO try and deforest this with a lookup table
		for (auto it = key_status.begin(); it != key_status.end(); ++it) {
			if ((*it).second == true) {

				if ((*it).first == GLFW_KEY_KP_4) {
					//camera->raise_heading_angle(glm::vec3(0.0f, 1.0f, 0.0f));
					camera->change_heading_angle(M_PI / 2);
				}
				if ((*it).first == GLFW_KEY_KP_6) {
					//camera->lower_heading_angle(-glm::vec3(0.0f, 1.0f, 0.0f));
					camera->change_heading_angle(-M_PI / 2);
				}
				if ((*it).first == GLFW_KEY_KP_5) {
					camera->change_heading_angle(0.0f);
					camera->change_elevation_angle(0.0f);
				}

				if ((*it).first == GLFW_KEY_KP_8) {
					camera->change_elevation_angle(M_PI/6);
				}
				if ((*it).first == GLFW_KEY_KP_2) {
					camera->change_elevation_angle(-M_PI/6);
				}

				if ((*it).first == GLFW_KEY_D) {
					camera->change_camera_position(-(camera->sideways_direction));
				}
				if ((*it).first == GLFW_KEY_A) {
					camera->change_camera_position(camera->sideways_direction);
				}
				if ((*it).first == GLFW_KEY_W) {
					//when moving forward, we move towards the camera_direction
					camera->change_camera_position(camera->forward_direction);
				}
				if ((*it).first == GLFW_KEY_S) {
					//when moving backwards, we move away from the camera direction
					camera->change_camera_position(-(camera->forward_direction));
				}
			}
		}
		for (auto it = key_status_2.begin(); it != key_status_2.end(); ++it) {
			if ((*it).second == true) {
				if ((*it).first == GLFW_KEY_SPACE) {
					change_imgui = true;
					key_status_2[(*it).first] = false;
				}
			}
		}

		//change imgui status
		if (change_imgui == true) {
			show_imgui = !show_imgui;
			change_imgui = false;
		}

		//calculate new camera angles
		camera->move_through_path();

		//NORMAL SHADER UNIFORMS
		local_to_world = teacup->get_model_matrix();//translate local coordinates to put the model on the given world coordinates
		view_space = camera->get_view_matrix();
		projection_space = glm::perspective(glm::radians(45.0f), (float)(win_width / win_height), 0.1f, 1000.0f);
		glm::mat4 model_view_teacup = view_space * local_to_world;

		shader_program.mat4_uniforms["model"] = local_to_world;
		shader_program.mat4_uniforms["view"] = view_space;
		shader_program.mat4_uniforms["projection"] = projection_space;
		shader_program.mat4_uniforms["modelview"] = model_view_teacup;

		shader_program.mat3_uniforms["normal"] = glm::mat3(glm::vec3(model_view_teacup[0]), glm::vec3(model_view_teacup[1]), glm::vec3(model_view_teacup[2]));

		shader_program.vec4_uniforms["light_pos"] = light_pos;

		shader_program.vec3_uniforms["mat_reflectivity"] = diffuse_mat_reflectivity;
		shader_program.vec3_uniforms["light"] = diffuse_light;
		shader_program.vec3_uniforms["object_color"] = glm::vec3(191.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
		shader_program.vec3_uniforms["specular_light"] = glm::vec3(1.0f, 1.0f, 1.0f);
		shader_program.vec3_uniforms["specular_mat_reflectivity"] = glm::vec3(0.8f, 0.8f, 0.8f);
		shader_program.vec3_uniforms["ambient_light"] = ambient_light;
		shader_program.vec3_uniforms["ambient_light"] = ambient_mat_reflectivity;

		shader_program.float_uniforms["shininnes"] = 100.0f;

		//PLANE SHADER UNIFORMS
		shader_program_plane.mat4_uniforms["model"] = plane->get_model_matrix();
		shader_program_plane.mat4_uniforms["view"] = view_space;
		shader_program_plane.mat4_uniforms["projection"] = projection_space;

		//SKYBOX SHADER UNIFORMS
		shader_program_skybox.mat4_uniforms["view"] = glm::mat4(glm::mat3(view_space)); //we get rid if the translation part of the view matrix by turning it into a mat3.
		shader_program_skybox.mat4_uniforms["projection"] = projection_space;

		//CALCULATE NEW VERTICES TO DRAW A LINE OF
		float vert1[3] = { 0.0f, 0.0f, 0.0f };
		//float vert2[3] = { (mouse_pos[0] - half_width) / 400 , (half_height - mouse_pos[1]) / 400, 0.0f };
		float vert2[3] = { mouse_movement_vector[0], mouse_movement_vector[1], 0.0f};
		line->change_vertices(vert1, vert2);

		skybox->draw(shader_program_skybox);
		plane->draw(shader_program_plane);
		teacup->draw(shader_program);
		if (show_imgui == true) {
			std::vector<std::string> value_titles{ "camara posicion x", "camara posicion y", "camara posicion z" };
			std::vector<std::string> values{ std::to_string(camera->camera_position.x), std::to_string(camera->camera_position.y), std::to_string(camera->camera_position.z) };
			imgui->add_values_to_window(&value_titles, &values);

			std::vector<std::string> value_titles_2{ "camera heading angle", "camera rotation speed" };
			std::vector<std::string> values_2{ std::to_string(camera->heading_angle * (180 / M_PI)), std::to_string(camera->rotation_speed) };
			imgui->add_values_to_window(&value_titles_2, &values_2);

			std::vector<std::string> value_titles_6{ "camera elevation angle", "camera rotation speed" };
			std::vector<std::string> values_6{ std::to_string(camera->elevation_angle * (180 / M_PI)), std::to_string(camera->rotation_speed) };
			imgui->add_values_to_window(&value_titles_6, &values_6);

			std::vector<std::string> value_titles_3{ "light pos x", "light pos y", "light pos z" };
			std::vector<std::string> values_3{ std::to_string(light_pos.x), std::to_string(light_pos.y), std::to_string(light_pos.z) };
			imgui->add_values_to_window(&value_titles_3, &values_3);

			std::vector<std::string> value_titles_4{ "camera direction x", "camera direction y", "camera direction z" };
			std::vector<std::string> values_4{ std::to_string(camera->forward_direction.x), std::to_string(camera->forward_direction.y), std::to_string(camera->forward_direction.z) };
			imgui->add_values_to_window(&value_titles_4, &values_4);

			std::vector<std::string> value_titles_5{ "camera direction sideways x", "camera direction sideways y", "camera direction sideways z" };
			std::vector<std::string> values_5{ std::to_string(camera->sideways_direction.x), std::to_string(camera->sideways_direction.y), std::to_string(camera->sideways_direction.z) };
			imgui->add_values_to_window(&value_titles_5, &values_5);

			std::vector<std::string> value_titles_7 {"Mouse position x", "Mouse position y"};
			std::vector<std::string> values_7 { std::to_string(old_mouse_pos[0]), std::to_string(old_mouse_pos[1]) };
			imgui->add_values_to_window(&value_titles_7, &values_7);
			
			std::vector<std::string> value_titles_8{ "Right Count", "Left Count" };
			std::vector<std::string> values_8{ std::to_string(right_count), std::to_string(left_count) };
			imgui->add_values_to_window(&value_titles_8, &values_8);

			std::vector<std::string> value_titles_9{"rotation path count"};
			std::vector<std::string> values_9{ std::to_string(camera->heading_angle_path.get_lenght())};
			imgui->add_values_to_window(&value_titles_9, &values_9);

			imgui->add_text_window("instructions", instructions);

			line->Draw(shader_program_line);
			imgui->render();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		//reset movement matrices
		local_to_world = glm::mat4(1.0f);
		view_space = glm::mat4(1.0f);
		projection_space = glm::mat4(1.0f);
	}
	glfwTerminate();
	delete imgui;
	return 0;
}