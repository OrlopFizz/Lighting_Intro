//VERTEX SHADER
#version 460
layout(location = 0) in vec3 aPos; //we bind this input vector to a location for the vertex attrib arrays defined in code
layout(location = 1) in vec3 Normal_Coords;
layout(location = 2) in vec2 aTexCoord;

//output variables
out vec2 tex_coord;
out vec3 light_intensity;

//perspective stuff, this could go on a uniform block, tbh
uniform mat3 normal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelview;

//diffuse reflection
uniform vec4 light_pos;
uniform vec3 mat_reflectivity;
uniform vec3 light;

void main(){
	//vertex position to clip space
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	tex_coord = aTexCoord;

	//calculting the light intensity in this vertex
	vec3 tnorm = normalize( normal * Normal_Coords );
    vec4 eyeCoords = modelview * vec4(aPos,1.0);
    vec3 s = normalize(vec3(light_pos - eyeCoords));

	//light_intensity = vec3(light_pos.x, light_pos.y, light_pos.z);
    light_intensity = light * mat_reflectivity * max( dot( s, tnorm ), 0.0 );
	//light_intensity = vec3(5, 5, 2);
}