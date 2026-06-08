#define _USE_MATH_DEFINES
#include "camera.h"
#include <math.h>

void fpv_camera::move_through_path() {
	if (heading_angle_path.get_lenght() != 0) {
		float new_angle = heading_angle_path.dequeue();
		change_heading_angle(new_angle);
		std::cout << new_angle << '\n';
	}
}

//some of these vectors dont change when we change the euler angles.
//eg: sideways vector only changes when we change heading, upward remains the same.
//when we elevate, upwards vector changes, but sideways remains the same.
fpv_camera::fpv_camera(glm::vec3 pCamera_Pos, glm::vec3 pForward_dir, glm::vec3 pUpwards_dir, float pRotate_Speed, float pMove_Speed) {
	camera_position = pCamera_Pos;
	forward_direction = pForward_dir;
	upwards_direction = pUpwards_dir;
	sideways_direction = glm::cross(-forward_direction, upwards_direction);
	heading_angle = 0.0f;
	elevation_angle = 0.0f;
	rotation_speed = pRotate_Speed;
	move_speed = pMove_Speed;
}

glm::mat4 fpv_camera::get_view_matrix()
{
	return glm::lookAt(camera_position, get_camera_target(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void fpv_camera::change_camera_position(glm::vec3 move_direction) {
	glm::mat4 translation = glm::mat4(1.0f);
	translation = glm::translate(translation, move_direction * move_speed);
	camera_position = translation * glm::vec4(camera_position, 1);
}


void fpv_camera::change_heading(float movement) {
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, movement, glm::vec3(0.0f, 1.0f, 0.0f));
	forward_direction = rotation * glm::vec4(forward_direction, 1.0f);
	upwards_direction = rotation * glm::vec4(upwards_direction, 1.0f);
	sideways_direction = glm::cross(-forward_direction, upwards_direction);
}

void fpv_camera::change_elevation(float movement) {
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, movement, sideways_direction);
	forward_direction = rotation * glm::vec4(forward_direction, 1.0f);
	upwards_direction = rotation * glm::vec4(upwards_direction, 1.0f);
	sideways_direction = glm::cross(-forward_direction, upwards_direction);
}

void fpv_camera::change_heading_angle(float new_heading) {
	float movement = new_heading - heading_angle;
	//heading_angle = new_heading;
	change_heading(movement);
}

void fpv_camera::change_elevation_angle(float new_elevation) {
	float movement = new_elevation - elevation_angle;
	elevation_angle = new_elevation;
	change_elevation(movement);
}

//since the camera position is in world space, and forward direction is in camera local space
//to get the target, we move the camera position into the forward direction
glm::vec3 fpv_camera::get_camera_target() {
	glm::mat4 translation = glm::mat4(1.0f);
	translation = glm::translate(translation, forward_direction);
	glm::vec4 world_forward_direction = translation * glm::vec4(camera_position, 1.0f);
	return glm::vec3(world_forward_direction);
}