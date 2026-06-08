//VERTEX SHADER
#version 460
layout(location = 0) in vec3 aPos; //we bind this input vector to a location for the vertex attrib arrays defined in code
layout(location = 1) in vec3 Normal_Coords;
layout(location = 2) in vec2 aTexCoord;

//output variabless
//The flat keyword changes the interpolation method used. this one activates flat shading.
//With flat shading, the color passed to the fragment shader is the one calculated for the first or last vertex! so that one polygon has the same color!
//The provoking vertex can be configured using glProvokingVertex(GL_FIRST/LAST_VERTEX_CONVENTION). default is last
flat out vec3 Color;
//DEBUG
out vec2 tex_coords;

//light uniform struct

uniform vec3 object_color;

//perspective stuff, this could go on a uniform block, tbh
uniform mat3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelview;

//ambient reflection
uniform vec3 ambient_light;
uniform vec3 ambient_mat_reflectivity;

//diffuse reflection
uniform vec4 light_pos;
uniform vec3 mat_reflectivity;
uniform vec3 light;

//specular reflection
uniform vec3 specular_light;
uniform vec3 specular_mat_reflectivity;
uniform float shininnes;

void getCamSpace(out vec3 norm, out vec3 position){ //these parameters are set as where we place the results
	norm = normalize(normal * Normal_Coords); //we can access the public uniforms and variables from anywhere. in the program.
	position = (modelview * vec4(aPos, 1.0)).xyz;
}

vec3 phongmodel(vec3 eyeposition, vec3 tnorm){
	//ambient light
	vec3 ambient_light_intensity = ambient_light * ambient_mat_reflectivity;

	//diffuse light
	//calculting the light intensity in this vertex
    vec3 s = normalize(vec3(light_pos.xyz - eyeposition));
    vec3 diffuse_light_intensity = light * mat_reflectivity * max( dot( s, tnorm ), 0.0 );

	//specular light
	//vec3 spec = vec3(0.0);
	vec3 v = normalize(-eyeposition.xyz);
	vec3 r = reflect(-s, tnorm);
	vec3 specular_light_intensity = specular_light * specular_mat_reflectivity * pow(max(dot(r, v), 0.0), shininnes);
	
	return ambient_light_intensity + diffuse_light_intensity + specular_light_intensity; 
}

void main(){

	vec3 camNorm;
	vec3 camPosition;
	getCamSpace(camNorm, camPosition);

	vec3 v = normalize(-camPosition.xyz);
	float vDotn = dot(v, camNorm);
	if (vDotn >= 0){
		Color = phongmodel(camPosition, camNorm);
	}
	else{
		Color = phongmodel(camPosition, -camNorm);
	}

	//vertex position to clip space
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	tex_coords = aTexCoord;
}