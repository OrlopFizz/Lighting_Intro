//PLANE FRAGMENT SHADER
#version 460
in vec2 tex_coords;
in vec3 normal_coords;
in vec3 vert_pos;

out vec4 FragColor;

uniform sampler2D texture_1;
uniform vec3 test_color;

void main(){
	vec4 texture_color = texture(texture_1, tex_coords);
	FragColor = texture_color;
}