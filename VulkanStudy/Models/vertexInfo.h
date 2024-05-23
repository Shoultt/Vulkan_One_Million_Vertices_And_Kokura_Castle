#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Models/modelReader.h"

modelReader chaika;
modelReader monster;
modelReader sphere;

struct Vertex { glm::vec3 positions; glm::vec3 colors; };
std::vector<Vertex> vertices = {
	{{-1.0f,-1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	{{ 1.0f,-1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
	{{ 1.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f}},
	{{-1.0f,-1.0f,-0.9f}, {1.0f, 0.0f, 0.0f}},
	{{ 1.0f,-1.0f,-0.9f}, {0.0f, 1.0f, 0.0f}},
	{{-1.0f, 1.0f,-0.9f}, {0.0f, 0.0f, 1.0f}},
	{{ 1.0f, 1.0f,-0.9f}, {1.0f, 0.0f, 1.0f}},
};

std::vector<uint16_t> indices = {
	2, 1, 0,
	2, 3, 1,
	6, 5, 4,
	6, 7, 5
};