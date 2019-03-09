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
	Vec3 MakeVectorFromLine(std::string& line);
	void ConsoleDebug();
	~Model();
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Triangle<int>> triangles;


};

