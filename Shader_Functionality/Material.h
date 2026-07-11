#pragma once
#include <glm.hpp>
struct Material {
	//TODO make struct that hold the material information, ieL reflectivity, shinniness, color, etc. even texture info
	glm::vec3 ambient_reflectivity;
	glm::vec3 diffuse_reflectivity;
	glm::vec3 specular_reflectivity;
	float shininess;
	glm::vec3 color; //rgb between 0.0f and 1.0f

	Material() {
		ambient_reflectivity = glm::vec3(1.0f);
		diffuse_reflectivity = glm::vec3(1.0f);
		specular_reflectivity = glm::vec3(1.0f);
		shininess = 0.0f;
		color = glm::vec3(1.0f);
	}

	Material(glm::vec3 pAmbient_reflectivity, glm::vec3 pDiffuse_reflectivity, glm::vec3 pSpecular_reflectivity, float pShininess, glm::vec3 pColor) {
		ambient_reflectivity = pAmbient_reflectivity;
		diffuse_reflectivity = pDiffuse_reflectivity;
		specular_reflectivity = pSpecular_reflectivity;
		shininess = pShininess;
		color = pColor;
	}
};