#pragma once
#include <glm.hpp>
//TODO turn this into a class for every light we define on our scene
struct light {
	glm::vec4 light_pos;
	glm::vec3 diffuse_light;
	glm::vec3 specular_light;
	glm::vec3 ambient_light;

	light(glm::vec4 pos, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient) {
		light_pos = pos;
		diffuse_light = diffuse;
		specular_light = specular;
		ambient_light = ambient;
	}
};
