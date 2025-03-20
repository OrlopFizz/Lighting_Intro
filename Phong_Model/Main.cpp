#define _USE_MATH_DEFINES
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader_Program.h"
#include <glm.hpp>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "Model.h"
#include "imgui_wrapper.h"
#include <math.h>

#include <iostream>
#include <map>
#include <typeinfo>

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

//TODO by now only moves in the x direction
glm::vec3 get_new_camera_target(float radians, float radious, glm::vec3 old_target) {
	old_target.x = radious * cos(radians);
	old_target.z = radious * sin(radians);

	return old_target;
}

glm::vec3 get_new_camera_target_2(float radians, float radious, glm::vec3 old_target) {
	old_target.z = radious * cos(radians);
	old_target.y = radious * sin(radians);

	return old_target;
}

void print_matrix(glm::mat4 pmatrix) {
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {
			std::cout << pmatrix[x][y] << " ";
		}
		std::cout << '\n';
	}
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
											{GLFW_KEY_KP_8, false}, {GLFW_KEY_KP_2, false} };

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
	GLFWwindow* window = glfwCreateWindow(win_width, win_height, "lighting intro", NULL, NULL);
	if (window == NULL){ //AN ERROR OCCURED WHEN CREATING THE WINDOW
		std::cout << "FAILED TO CREATE A GLFW WINDOW" << std::endl;	
		glfwTerminate(); 
		return -1;
	}

	glfwMakeContextCurrent(window); //make the window the current glfw and opengl context

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //load the opengl functions addresses, we pass the glfw function to get the addresses as a GLADLOADPROC
		std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
		return -1;
	}

	//starting imgui

	glfwSetKeyCallback(window, process_input);

	int max_uniforms;
	glGetIntegerv(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &max_uniforms);


	const GLubyte* texto = glGetString(GL_VERSION);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glViewport(0, 0, win_width, win_height);
	glClearColor(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//set up the model
	//here we set up the various buffers and populate them with the info we find in the model
	glm::vec3 diffuse_mat_reflectivity = glm::vec3(0.9f, 0.5f, 0.3f); //this is basically the color of the object, i think.
	glm::vec3 ambient_mat_reflectivity = glm::vec3(0.9f, 0.5f, 0.3f);
	Model* teacup = new Model("UTAH_BLEND.obj");
	
	//CONFIGURE SHADER
	Shader_Program shader_program = Shader_Program("Vertex_Shader.glsl", "Fragment_Shader.glsl");
	if (!shader_program.Use_Program()) {
		return -1;
	}
	//aka model matrix. Transforms the object to the world space. Translates, rotates, and scales the object to the parameters we need on our final project
	glm::mat4 local_to_world = glm::mat4(1.0f); 

	//player movements
	glm::mat4 player_rotations_left_right = glm::mat4(1.0f);
	glm::mat4 player_rotations_up_down = glm::mat4(1.0f);
	glm::mat4 move_object_vertices = glm::mat4(1.0f);
	//NOTE! it is important to  reset these evey frame! so subsequent calls 
	//we then generate the view space, aka "the camera". we move all vertices to put them in place where we would want them to be from a camera perspective. this uses translations,
	//rotations, and scaling.
	glm::mat4 view_space = glm::mat4(1.0f);

	//define the camera coordinates
	//first, camera position
	glm::vec3 camera_origin = glm::vec3(0.0f, 0.0f, 10.0f);
	//camera direction, vector from the camera target to the camera, this will work as our z axis
	glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
	float camera_speed = 0.1f;
	float current_rads_x = 3*M_PI_2;
	float current_rads_y = M_PI;
	float radious = 10.0;

	//the matrix we use to add perspective to our game, this also converts our vertices to NDC(-1.0 to 1.0). 
	glm::mat4 projection_space = glm::mat4(1.0f);

	float x_angle = 0.0f;
	float y_angle = 0.0f;
	float z_angle = 0.0f;

	//Light Source
	glm::vec4 light_pos = glm::vec4(0.0f, 0.0f, 10.0f, 1.0f);
	glm::vec3 diffuse_light = glm::vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);

	//ambient light
	glm::vec3 ambient_light = glm::vec3(0.4f, 0.4f, 0.4f);

	//DEBUG ACTIVE UNIFORMS
	GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};
	int numuniforms = 0;	
	glGetProgramInterfaceiv(shader_program.shader_program_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numuniforms);
	std::cout << "Active Unforms: " << std::endl;
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

	//RENDER LOOP
	bool change_imgui = false;
	bool show_imgui = false;
	imgui_wrapper* imgui = new imgui_wrapper(window);
	while(!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//CHECK KEYBOARD INPUTS
		for (auto it = key_status.begin(); it != key_status.end(); ++it) {
			if ((*it).second == true) {
				if ((*it).first == GLFW_KEY_KP_4) {
					current_rads_x -= 0.01f;
					camera_target = get_new_camera_target(current_rads_x, radious, camera_target);
				}
				if ((*it).first == GLFW_KEY_KP_6) {
					current_rads_x += 0.01f;
					camera_target = get_new_camera_target(current_rads_x, radious, camera_target);
				}
				if ((*it).first == GLFW_KEY_KP_8) {
					current_rads_y -= 0.01f;
					camera_target = get_new_camera_target_2(current_rads_y, radious, camera_target);
				}
				if ((*it).first == GLFW_KEY_KP_2) {
					current_rads_y += 0.01f;
					camera_target = get_new_camera_target_2(current_rads_y, radious, camera_target);
				}

				if ((*it).first == GLFW_KEY_D) {
					camera_origin.x += camera_speed;
				}
				if ((*it).first == GLFW_KEY_A) {
					camera_origin.x -= camera_speed;
				}
				if ((*it).first == GLFW_KEY_W) {
					camera_origin.z -= camera_speed;
				}
				if ((*it).first == GLFW_KEY_S) {
					camera_origin.z += camera_speed;
				}

				//rotations should always be in the same direction, independent from the facing of the object
				if ((*it).first == GLFW_KEY_RIGHT) {
					//player_rotations_left_right = glm::rotate(player_rotations_left_right, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					camera_origin.x += 0.1f;
				}
				if ((*it).first == GLFW_KEY_LEFT) {
					//player_rotations_left_right = glm::rotate(player_rotations_left_right, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					camera_origin.x -= 0.1f;
				}
				if ((*it).first == GLFW_KEY_UP) {
					//player_rotations_up_down = glm::rotate(player_rotations_up_down, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					camera_origin.z -= 0.1f;
				}
				if ((*it).first == GLFW_KEY_DOWN) {
					//player_rotations_up_down = glm::rotate(player_rotations_up_down, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					camera_origin.z += 0.1f;
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
		if (change_imgui == true){
			show_imgui = !show_imgui;
			change_imgui = false;
		}

		local_to_world = glm::translate(local_to_world, glm::vec3(0.0f, -1.0f, 0.0f));
		view_space = glm::lookAt(camera_origin, camera_target, glm::vec3(0.0f, 1.0f, 0.0f));
		projection_space = glm::perspective(glm::radians(45.0f), (float)(win_width / win_height), 0.1f, 100.0f);

		shader_program.Set_Mat4("model",move_object_vertices * player_rotations_left_right * player_rotations_up_down * local_to_world);
		shader_program.Set_Mat4("view", view_space);
		shader_program.Set_Mat4("projection", projection_space);
		glm::mat4 model_view = view_space * move_object_vertices * player_rotations_left_right * player_rotations_up_down * local_to_world;
		
		shader_program.Set_Mat3("normal", glm::mat3(glm::vec3(model_view[0]), glm::vec3(model_view[1]), glm::vec3(model_view[2])));
		shader_program.Set_Mat4("modelview", model_view);

		
		shader_program.Set_Vec4("light_pos", light_pos);
		shader_program.Set_Vec3("mat_reflectivity", diffuse_mat_reflectivity);
		shader_program.Set_Vec3("light", diffuse_light);
		shader_program.Set_Vec3("object_color", glm::vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f));

		shader_program.Set_Vec3("specular_light", glm::vec3(1.0f, 1.0f, 1.0f));
		shader_program.Set_Vec3("specular_mat_reflectivity", glm::vec3(0.8f, 0.8f, 0.8f));
		shader_program.Set_Float("shininnes", 100.0f);
		
		shader_program.Set_Vec3("ambient_light", ambient_light);
		shader_program.Set_Vec3("ambient_mat_reflectivity", ambient_mat_reflectivity);

		teacup->draw(shader_program);
		if (show_imgui == true){
			std::vector<std::string> value_titles{ "camara posicion x", "camara posicion y", "camara posicion z" };
			std::vector<std::string> values{ std::to_string(camera_origin.x), std::to_string(camera_origin.y), std::to_string(camera_origin.z) };
			imgui->add_values_to_window(&value_titles, &values);

			std::vector<std::string> value_titles_2{ "camera target x", "camera target y", "camera target z" };
			std::vector<std::string> values_2{ std::to_string(camera_target.x), std::to_string(camera_target.y), std::to_string(camera_target.z) };
			imgui->add_values_to_window(&value_titles_2, &values_2);

			std::vector<std::string> value_titles_3{ "light pos x", "light pos y", "light pos z" };
			std::vector<std::string> values_3{ std::to_string(light_pos.x), std::to_string(light_pos.y), std::to_string(light_pos.z) };
			imgui->add_values_to_window(&value_titles_3, &values_3);


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