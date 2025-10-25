//FRAGMENT SHADER
#version 460
//in vec3 front_color;
//in vec3 back_color;
in vec3 Color;
in vec2 tex_coord;

out vec4 FragColor; //define the output of the fragment shader

//from version 4.6 we can bind this sampler2D uniform to a texture unit
layout (binding = 0)uniform sampler2D Texture;
layout (binding  = 1) uniform sampler2D Texture2;

uniform vec3 object_color;

void main(){
	//if (gl_FrontFacing){
		//FragColor = vec4(front_color, 1.0) * vec4(object_color, 1.0);
		//FragColor = vec4(front_color, 1.0);
	//}
	//else{
		//FragColor = vec4(back_color, 1.0) * vec4(object_color, 1.0);
		//FragColor = mix(vec4(back_color, 1.0) * vec4(object_color, 1.0), vec4(1.0, 0.0, 0.0, 1.0), 0.7);
		//FragColor = vec4(0.0, 1.0, 0.0, 1.0);
		//FragColor = vec4(back_color, 1.0);
	//}
	
	FragColor = vec4(Color, 1.0) * vec4(object_color, 1.0);

}