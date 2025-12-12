#pragma once
#include <GL/glew.h>
#include <vector>

namespace eng
{
	struct VertexElement
	{
		GLuint index; //Attribute location
		GLuint size; //Number of components in each set
		GLuint type; //Data type (GL_FLOAT) etc
		uint32_t offset; //Byte offset from start of vertex
	};

	struct VertexLayout 
	{
		std::vector<VertexElement> elements; //List of all vertex elements
		uint32_t stride = 0; //Total size of a single vertex
	};
}