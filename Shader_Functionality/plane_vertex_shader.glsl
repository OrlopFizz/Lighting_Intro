//PLANE VERTEX SHADER
#version 460
layout(location = 0) in vec3 aPos; //we bind this input vector to a location for the vertex attrib arrays defined in code
layout(location = 2) in vec2 aTexCoord;

//perspective stuff, this could go on a uniform block, tbh
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//output variables
out vec2 tex_coords;
out vec3 normal_coords;
out vec3 vert_pos;

void main(){	
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	tex_coords = aTexCoord;
	vert_pos = aPos;
}