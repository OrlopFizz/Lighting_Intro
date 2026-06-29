#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>

#include "vector"
struct fpv_camera {
	public:
		float heading_angle;
		float elevation_angle;
		float move_speed;
		float rotation_speed;
		glm::vec3 camera_position;
		glm::vec3 forward_direction;
		glm::vec3 sideways_direction;
		glm::vec3 upwards_direction;

		fpv_camera(glm::vec3 pCamera_Pos, glm::vec3 pForward_dir, glm::vec3 pUpwards_dir, float pRotate_Speed, float pMove_Speed);

		glm::mat4 get_view_matrix();
		glm::mat4 get_projection_matrix(float win_width, float win_height);

		void change_camera_position(glm::vec3 moving_vector);
		void change_heading(float movement);
		void change_elevation(float movement);

		//TODO make methods to change the camera angles to a given angle. it must calculate how to get there from the current angle
		void change_heading_angle(float new_heading);
		void change_elevation_angle(float new_elevation);

		glm::vec3 get_camera_target();
};   