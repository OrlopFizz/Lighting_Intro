//FRAGMENT SHADER
#version 460
flat in vec3 Color;

out vec4 FragColor; //define the output of the fragment shader

uniform sampler2D texture1;
//from version 4.6 we can bind this sampler2D uniform to a texture unit
//layout (binding = 1) uniform sampler2D Texture2;

uniform vec3 object_color;

//DEBUG
in vec2 tex_coords;

void main(){
	FragColor = vec4(Color, 1.0) * vec4(object_color, 1.0);
	//FragColor = vec4(tex_coords, 0, 0);
}