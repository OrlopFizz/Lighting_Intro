//Phong (per fragment shading) VERTEX SHADER
//this shader only transforms vertex data to camera coordinates
#version 460
layout(location = 0) in vec3 aPos; //we bind this input vector to a location for the vertex attrib arrays defined in code
layout(location = 1) in vec3 Normal_Coords;

//output variabless
out vec3 vert_position; //position of the vertex
out vec3 vert_normal; //normal of the vertex

//uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

mat4 modelview = view * model;
mat3 normal_modelview = mat3(vec3(modelview[0]), vec3(modelview[1]), vec3(modelview[2]));

void getCamSpace(out vec3 norm, out vec3 position){ //these parameters are set as where we place the results
	norm = normalize(normal_modelview * Normal_Coords); //convert the vertex normal from local to camera space
	position = (modelview * vec4(aPos, 1.0)).xyz; //convert the vertex position from local space to camera space
}

void main(){
	//vertex position to clip space
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	getCamSpace(vert_normal, vert_position);
}