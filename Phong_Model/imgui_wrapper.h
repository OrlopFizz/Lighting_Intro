#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class imgui_wrapper {
	public:
		std::vector<std::pair<std::vector<std::string>*, std::vector<std::string>*>> windows_info;

		//constructor
		imgui_wrapper(GLFWwindow* window) { init(window); };
		//destructor
		~imgui_wrapper() { std::cout << "wrapper destructor called" << '\n'; shutdown(); };

		void init(GLFWwindow* window);
		void shutdown();
		void render();
		void create_listing_window(std::string title, std::vector<std::string> value_titles, std::vector<std::string> values);
		void add_values_to_window(std::vector<std::string>* value_titles, std::vector<std::string>* values);
};	