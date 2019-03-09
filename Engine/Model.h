#pragma once
#include <Vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Vec2.h"
#include "Vec3.h"
#include "Triangle.h"

class Model
{
public:
	Model(std::vector<Vec3> vertices, std::vector<Vec3> normals, std::vector<Triangle<int>> triangles);
	Model(std::string filePath);
	void ConsoleDebug();
	~Model();
	std::vector<Vec3> vertices;
	std::vector<Vec2> textureVertices;
	std::vector<Vec3> normals;
	std::vector<Triangle<int>> triangles;
private:
	Vec3 MakeVec3FromLine(std::string& line);
	Vec2 MakeVec2FromLine(std::string& line);
	int ProcessTriangleItem(std::string& line, std::string& delimiter);
};