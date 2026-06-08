#include "Line.h"
//set up the line vertices and vertex attribute pointers
Line::Line(float pVert1[3], float pVert2[3]) {
	vert1 = pVert1;
	vert2 = pVert2;
	
	float total_verts[6] = { vert1[0], vert1[1], vert1[2], vert2[0], vert2[1], vert2[2]};
	
	//set vertex data
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &total_verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(NULL);
	std::cout << "CREATED VAO WITH ID " << VAO << '\n';
}

void Line::Draw(Shader_Program pShader) {
	pShader.Use_Program();
	pShader.set_uniforms();

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
}

//TODO since i only want to move the second vertex, I could change this to be done with glBufferSubData
void Line::change_vertices(float pVert1[3], float pVert2[3]) {
	vert1 = pVert1;
	vert2 = pVert2;

	float total_verts[6] = { vert1[0], vert1[1], vert1[2], vert2[0], vert2[1], vert2[2] };

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &total_verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}

