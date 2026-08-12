//PHONG (per fragment shading) FRAGMENT SHADER
//we mainly just interpolate and mix things here.
#version 460
//light uniform struct

uniform struct lightInfo{
	vec4 position;
	vec3 ambient_light;
	vec3 diffuse_light;
	vec3 specular_light;
} lights[1];

in vec3 vert_position;
in vec3 vert_normal;

out vec4 FragColor;

//subroutines
subroutine vec3 FragmentFunctionType (lightInfo light, vec3 eyeposition, vec3 tnorm);
subroutine uniform FragmentFunctionType FragmentFunction;

//Color of the object
uniform vec3 object_color;

//ambient reflection
uniform vec3 ambient_mat_reflectivity;

//diffuse reflection
uniform vec3 mat_reflectivity;

//specular reflection
uniform vec3 specular_mat_reflectivity;
uniform float shininnes;

subroutine (FragmentFunctionType) vec3 phongmodel(lightInfo light, vec3 eyeposition, vec3 tnorm){
	//ambient light
	vec3 ambient_light_intensity = light.ambient_light * ambient_mat_reflectivity;

	//diffuse light
	//calculting the light intensity in this vertex. 
	vec3 s;
	if (light.position.w == 0.0f){
		s = normalize(light.position.xyz); // the light is a directional light
	}
	else{
	    s = normalize(light.position.xyz - eyeposition); //the light is a positional light
	}
	vec3 diffuse_light_intensity = light.diffuse_light * mat_reflectivity * max( dot( s, tnorm ), 0.0 );

	//specular light
	vec3 v = normalize(-eyeposition.xyz);
	vec3 r = reflect(-s, tnorm);
	vec3 specular_light_intensity = light.specular_light * specular_mat_reflectivity * pow(max(dot(r, v), 0.0), shininnes);
	
	return ambient_light_intensity + diffuse_light_intensity + specular_light_intensity; 
}

subroutine (FragmentFunctionType) vec3 blinnphongmodel(lightInfo light, vec3 eyeposition, vec3 tnorm){
	//ambient light
	vec3 ambient_light_intensity = light.ambient_light * ambient_mat_reflectivity;

	//diffuse light
	//calculting the light intensity in this vertex. 
	vec3 s;
	if (light.position.w == 0.0f){
		s = normalize(light.position.xyz); // the light is a directional light
	}
	else{
	    s = normalize(light.position.xyz - eyeposition); //the light is a positional light
	}
	vec3 diffuse_light_intensity = light.diffuse_light * mat_reflectivity * max( dot( s, tnorm ), 0.0 );

	//specular light
	vec3 v = normalize(-eyeposition.xyz);
	vec3 h = normalize(v + s);
	vec3 specular_light_intensity = light.specular_light * specular_mat_reflectivity * pow(max(dot(h, tnorm), 0.0), shininnes);
	
	return ambient_light_intensity + diffuse_light_intensity + specular_light_intensity; 
}

void main(){
	vec3 v = normalize(-vert_position.xyz);
	float vDotn = dot(v, vert_normal);
	if (vDotn >= 0){
		for (int i = 0; i < 1; i++){
			FragColor += vec4(FragmentFunction(lights[0], vert_position, normalize(vert_normal)), 1.0f);// * vec4(object_color, 1.0f);
		}
	}
	else{
		for (int i = 0; i < 1; i++){
			FragColor += vec4(FragmentFunction(lights[0], vert_position, normalize(-vert_normal)), 1.0f);// * vec4(object_color, 1.0f);
		}
	}
}