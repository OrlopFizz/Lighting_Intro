//VERTEX SHADER
#version 460
layout(location = 0) in vec3 aPos; //we bind this input vector to a location for the vertex attrib arrays defined in code
layout(location = 1) in vec3 Normal_Coords;
layout(location = 2) in vec2 aTexCoord;

//output variables
out vec2 tex_coord;
out vec3 diffuse_light_intensity;
out vec3 specular_light_intensity;
out vec3 ambient_light_intensity;

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

void main(){
	//vertex position to clip space
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	//tex_coord = aTexCoord;
	tex_coord = vec2(1.0, 1.0);

	//ambient light
	ambient_light_intensity = ambient_light * ambient_mat_reflectivity;

	//diffuse light
	//calculting the light intensity in this vertex
	vec3 tnorm = normalize( normal * Normal_Coords );
    vec4 eyeCoords = modelview * vec4(aPos,1.0);
    vec3 s = normalize(vec3(light_pos - eyeCoords));
	
    diffuse_light_intensity = light * mat_reflectivity * max( dot( s, tnorm ), 0.0 );

	//specular light
	//vec3 spec = vec3(0.0);
	vec3 v = normalize(-eyeCoords.xyz);
	vec3 r = reflect(-s, tnorm);
	specular_light_intensity = specular_light * specular_mat_reflectivity * pow(max(dot(r, v), 0.0), shininnes);

	//specular_light_intensity = spec;
}