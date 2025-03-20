//FRAGMENT SHADER
#version 460
in vec3 light_intensity;
in vec2 tex_coord;

out vec4 FragColor; //define the output of the fragment shader

//from version 4.6 we can bind this sampler2D uniform to a texture unit
layout (binding = 0)uniform sampler2D Texture;
layout (binding  = 1) uniform sampler2D Texture2;

uniform vec3 object_color;

void main(){
	//FragColor = frag_color; 
	//FragColor = texture(Texture, tex_coord); 
	//vec4 brick_tex = texture(Texture, tex_coord);
	//vec4 blood_tex = texture(Texture2, tex_coord);
	//blood_tex.rgb = vec3(1.0, 1.0, 1.0);
	//FragColor = mix(brick_tex.rgb, blood_tex.rgb, blood_tex.a);
	//vec4 color = vec4(255.0/255.0, 0.0/255.0, 0.0/255.0, 0.0);
	//vec4 color = vec4(0.8608, 0.4782, 0.2869, 0.0);
	//vec4 intensity = vec4(1.0, 1.0, 1.0, 1.0);
	//vec4 intensity = vec4(light_intensity, 1.0);
	//FragColor = vec4(ambient_light_intensity, 1.0) * vec4(light_intensity, 1.0) * vec4(object_color.xyz, 1.0);
	FragColor = vec4(light_intensity, 1.0) * vec4(object_color, 1.0);
}