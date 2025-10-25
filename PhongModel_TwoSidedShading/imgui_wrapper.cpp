#include "imgui_wrapper.h"

void imgui_wrapper::init(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
}

void imgui_wrapper::shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void imgui_wrapper::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//render code
	//set up windows to render
	//ImGui::ShowDemoWindow();
	int index = 1;
	for (auto it = windows_info.begin(); it != windows_info.end(); ++it) {
		std::vector<std::string>* value_titles = (*it).first;
		std::vector<std::string>* values = (*it).second;
		create_listing_window("ventana " + std::to_string(index), *value_titles, *values);
		index++;
	}
	windows_info.clear();

	for (auto it = text_windows.begin(); it != text_windows.end(); ++it) {
		create_text_window((*it).first, (*it).second);
	}
	text_windows.clear();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//create the window with values
void imgui_wrapper::create_listing_window(std::string title, std::vector<std::string> value_titles, std::vector<std::string> values) {
	ImGui::Begin(title.c_str());
	for (int i = 0; i < value_titles.size(); ++i) {
		//ImGui::Text(value_titles[i].c_str(), values[i]);
		std::string text = value_titles[i] + " : " + values[i];
		ImGui::Text(text.c_str());
	}
	ImGui::End();
}
//add values 
void imgui_wrapper::add_values_to_window(std::vector<std::string>* value_titles, std::vector<std::string>* values) {
	std::pair<std::vector<std::string>*, std::vector<std::string>*> new_pair { value_titles, values };
	windows_info.push_back(new_pair);
}

void imgui_wrapper::create_text_window(std::string title, std::string body) {
  	ImGui::Begin(title.c_str());
	ImGui::Text(body.c_str());
	ImGui::End();
}

void imgui_wrapper::add_text_window(std::string title, std::string body)
{
	std::pair<std::string, std::string> new_pair {title, body};
	text_windows.push_back(new_pair);
}

