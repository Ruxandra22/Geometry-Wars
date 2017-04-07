#include "Object2D.h"

#include <Core/Engine.h>

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateLifes(std::string name, glm::vec3 leftBottomCorner, float length, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner),
		VertexFormat(corner + glm::vec3(length / 7 , 0, 0)),
		VertexFormat(corner + glm::vec3(length / 7, length, 0)),
		VertexFormat(corner + glm::vec3(0, length, 0))
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_QUADS);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, /*glm::vec3 color,*/ bool fill)
{
	//glm::vec3 corner = leftBottomCorner; // - jumatate de nava 
	glm::vec3 corner = leftBottomCorner - length / 2;
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner/*, color*/),
		VertexFormat(corner + glm::vec3(length / 2, length / 2, 0)/*, color*/),
		VertexFormat(corner + glm::vec3(length, 0, 0)/*, color*/),
		VertexFormat(corner + glm::vec3(0, length, 0)/*, color*/),
		VertexFormat(corner + glm::vec3(length, length, 0)/*, color*/),
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices = {0, 1, 2, 3, 4};

	if (!fill) {
		triangle->SetDrawMode(GL_TRIANGLES);
	}
	else {
		// draw the triangle. Add the right indice(middle point)
		indices.push_back(1);
	}

	triangle->InitFromData(vertices, indices);
	return triangle;
}

Mesh* Object2D::CreateProjectiles(std::string name, glm::vec3 leftBottomCorner, float length, bool fill)
{
	glm::vec3 corner = leftBottomCorner - length / 20;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner),
		VertexFormat(corner + glm::vec3(length / 10 , 0, 0)),
		VertexFormat(corner + glm::vec3(length / 10, length / 10, 0)),
		VertexFormat(corner + glm::vec3(0, length / 10, 0))
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };

	if (!fill) {
		square->SetDrawMode(GL_QUADS);
	}
	else {
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}