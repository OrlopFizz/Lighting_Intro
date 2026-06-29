//FRAGMENT SHADER
#version 460
in vec3 Color;
in vec3 vert_pos;

//define PRNG struct
struct random{
	float current;

	float modulus;
	float seed;
	float multiplier;
	float increment;
};

void random::setup(float pModulus, float pSeed, float pMult, float pInc){
	modulus = pModulus;
	seed = pSeed;
	multiplier = pMult;
	increment = pInc;
}

void random::next(){
	current = mod(multiplier*current+increment, modulus);
}

float random::get_next_random(){
	next();
	return current / modulus;
}

out vec4 FragColor; //define the output of the fragment shader

uniform vec3 object_color;

//subroutines setup
subroutine vec4 FragmentFunctionType ();
subroutine uniform FragmentFunctionType FragmentFunction;

random rand;

//regular color info from the vertex shader
subroutine (FragmentFunctionType) vec4 regular_func(){
	return vec4(Color, 1.0) * vec4(object_color, 1.0);
}

//return a random color for every fragment 
subroutine (FragmentFunctionType) vec4 random_colors_for_fragments(){
	float random_value = rand.get_next_random();
	return vec4(vert_pos, 1.0);
}

//discard fragments given a filter
subroutine (FragmentFunctionType) vec4 discard_fragments(){
	if(vert_pos.z > 0){
		discard;
	}
	return vec4(Color, 1.0) * vec4(object_color, 1.0);
}

void main(){
	//LCG random number generator
	float modulus = pow(2, 31);
	float seed = 1781589697;
	float a = 1103515245;
	float c = 12345;
	float current = seed;

	rand.setup(modulus, seed, a, c);
	FragColor = FragmentFunction();
}