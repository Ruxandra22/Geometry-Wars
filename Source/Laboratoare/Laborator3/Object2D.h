#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = true);
	Mesh* CreateTriangle(std::string name, glm::vec3 leftBottomCorner, float length, /*glm::vec3 color,*/ bool fill = true);
	Mesh * CreateProjectiles(std::string name, glm::vec3 leftBottomCorner, float length, bool fill);
	Mesh* CreateLifes(std::string name, glm::vec3 leftBottomCorner, float length, bool fill = true);
}

