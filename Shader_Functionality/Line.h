#pragma once
#include "Shader_Program.h"
class Line {
	public:
		float* vert1;
		float* vert2;

		unsigned int VBO {0};
		unsigned int VAO {0};

		Line(float pVert1[3], float pVert2[3]);
		void Draw(Shader_Program pShader);
		void change_vertices(float pVert1[3], float pVert2[3]);
};