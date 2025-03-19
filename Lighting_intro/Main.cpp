#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader_Program.h"
#include <glm.hpp>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include "Model.h"

#include <iostream>
#include <map>
#include <typeinfo>

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

std::map<unsigned int, bool> key_status = { {68, false}, {65, false}, {87, false}, {83, false}, {262, false}, {263, false}, {264, false}, {265, false}, {322, false}, {324, false}, {326, false}, {328, false} };
void process_input(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		key_status[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		key_status[key] = false;
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

	glfwSetKeyCallback(window, process_input);

	const GLubyte* texto = glGetString(GL_VERSION);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glViewport(0, 0, win_width, win_height);
	glClearColor(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	//set up the model
	//here we set up the various buffers and populate them with the info we find in the model
	glm::vec3 mat_reflectivity = glm::vec3(0.9f, 0.5f, 0.3f);
	Model* teacup = new Model("UTAH_BLEND.obj");
	
	//CONFIGURE SHADER
	Shader_Program shader_program = Shader_Program("C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Lighting_intro\\Vertex_Shader.glsl", "C:\\Users\\Alejandro\\source\\repos\\Lighting_intro\\Lighting_intro\\Fragment_Shader.glsl");
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


	//the matrix we use to add perspective to our game, this also converts our vertices to NDC(-1.0 to 1.0). 
	glm::mat4 projection_space = glm::mat4(1.0f);

	float x_angle = 0.0f;
	float y_angle = 0.0f;
	float z_angle = 0.0f;

	//Light Source
	glm::vec4 light_pos = glm::vec4(5.0f, 5.0f, 2.0f, 1.0f);
	glm::vec3 light = glm::vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);

	//RENDER LOOP
	while(!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//CHECK KEYBOARD INPUTS
		for (auto it = key_status.begin(); it != key_status.end(); ++it) {
			if ((*it).second == true) {
				
				//moving the vertices shoould be made after rotating the object, so rotations dont change the directions im moving the objects
				if ((*it).first == GLFW_KEY_D) {
					move_object_vertices = glm::translate(move_object_vertices, glm::vec3(move_vel, 0.0f, 0.0f));
				}
				if ((*it).first == GLFW_KEY_A) {
					move_object_vertices = glm::translate(move_object_vertices, glm::vec3(-1.0f * move_vel, 0.0f, 0.0f));
				}
				if ((*it).first == GLFW_KEY_W) {
					move_object_vertices = glm::translate(move_object_vertices, glm::vec3(0.0f, move_vel, 0.0f));
				}
				if ((*it).first == GLFW_KEY_S) {
					move_object_vertices = glm::translate(move_object_vertices, glm::vec3(0.0f, -1.0f * move_vel, 0.0f));
				}
				
				//rotations should always be in the same direction, independent from the facing of the object
				if ((*it).first == GLFW_KEY_RIGHT) {
					player_rotations_left_right = glm::rotate(player_rotations_left_right, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				if ((*it).first == GLFW_KEY_LEFT) {
					player_rotations_left_right = glm::rotate(player_rotations_left_right, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				if ((*it).first == GLFW_KEY_UP) {
					player_rotations_up_down = glm::rotate(player_rotations_up_down, glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				}
				if ((*it).first == GLFW_KEY_DOWN) {
					player_rotations_up_down = glm::rotate(player_rotations_up_down, glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				}
			}
		}

		local_to_world = glm::translate(local_to_world, glm::vec3(0.0f, 0.0f, 0.0f));
		view_space = glm::lookAt(camera_origin, camera_target, glm::vec3(0.0f, 1.0f, 0.0f));
		projection_space = glm::perspective(glm::radians(45.0f), (float)(win_width / win_height), 0.1f, 100.0f);

		shader_program.Set_Mat4("model",move_object_vertices * player_rotations_left_right * player_rotations_up_down * local_to_world);
		shader_program.Set_Mat4("view", view_space);
		shader_program.Set_Mat4("projection", projection_space);
		glm::mat4 model_view = view_space * move_object_vertices * player_rotations_left_right * player_rotations_up_down * local_to_world;
		
		shader_program.Set_Mat3("normal", glm::mat3(glm::vec3(model_view[0]), glm::vec3(model_view[1]), glm::vec3(model_view[2])));
		shader_program.Set_Mat4("modelview", model_view);

		shader_program.Set_Vec4("light_pos", glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));
		shader_program.Set_Vec3("mat_reflectivity", mat_reflectivity);
		shader_program.Set_Vec3("light", light);
		shader_program.Set_Vec3("object_color", glm::vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f));
		
		teacup->draw(shader_program);

		glfwSwapBuffers(window);
		glfwPollEvents();

		//reset movement matrices
		local_to_world = glm::mat4(1.0f);
		view_space = glm::mat4(1.0f);
		projection_space = glm::mat4(1.0f);
	}

	glfwTerminate();
	return 0;
}