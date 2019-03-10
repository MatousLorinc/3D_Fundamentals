#include "Model.h"

Model::Model(std::vector<Vec3> vertices, std::vector<Vec3> normals, std::vector<Triangle> triangles) :
	vertices(vertices),
	normals(normals),
	triangles(triangles)
{

}

Model::Model(std::string filePath)
{
	using namespace std;
	ifstream inFile;
	// open the file stream
	inFile.open(filePath);
	// check if opening a file failed
	if (inFile.fail()) {
		std::cerr << "Error opeing a file" << std::endl;
		inFile.close();
		exit(1);
	}
	string line;
	while (getline(inFile, line))
	{
		if (line[0] == 'v')
		{
			if (line[1] == 'n')
			{
				// process normal vertex
				line = line.substr(3, line.length());
				normals.push_back(MakeVec3FromLine(line));
			}
			else
			{
				// process texture vertex
				if (line[1] == 't')
				{
					line = line.substr(3, line.length());
					textureVertices.push_back(MakeVec2FromLine(line));
				}
				// process object vertex
				else
				{
					line = line.substr(2, line.length());
					vertices.push_back(MakeVec3FromLine(line));
				}
			}
		}
		if (line[0] == 'f')
		{
			string token;
			string delimiterItem = " ";
			string tmp = "/";
			size_t pos = line.find(tmp);
			bool test = line[pos + 1] == '/';
			// process model without UV mapping
			if (test)
			{
				// process vertex triangle
				line = line.substr(2, line.length());
				int v[3];
				int n[3];
				for (int i = 0; i < 3; i++)
				{
					string delimiter = "//";
					v[i] = ProcessTriangleItem(line, delimiter);
					delimiter = " ";
					n[i] = ProcessTriangleItem(line, delimiter);
				}
				//vi[2] = stoi(token)-1;
				printf("%i %i %i / %i \n ", v[0], v[1], v[2], n[0]);
				triangles.push_back({ v[0],v[1],v[2],n[0] });
			}
			// process model with UV mapping
			else
			{
				// process vertex triangle
				line = line.substr(2, line.length());
				int v[3];
				int t[3];
				int n[3];
				for (int i = 0; i < 3; i++)
				{
					string delimiter = "/";
					v[i] = ProcessTriangleItem(line, delimiter);
					delimiter = "/";
					t[i] = ProcessTriangleItem(line, delimiter);
					delimiter = " ";
					n[i] = ProcessTriangleItem(line, delimiter);
				}
				//vi[2] = stoi(token)-1;
				printf("%i %i %i / %i \n ", v[0], v[1], v[2], n[0]);
				triangles.push_back({ v[0],v[1],v[2],n[0],t[0],t[1],t[2] });
			}
		}
	}
	// close the file stream
	inFile.close();
}

Vec3 Model::MakeVec3FromLine(std::string& line)
{
	using namespace std;
	float normalsValues[3];

	string delimiter = " ";
	size_t pos = 0;
	string token;

	for (int i = 0; i < 2; i++)
	{
		pos = line.find(delimiter);
		token = line.substr(0, pos);
		line.erase(0, pos + delimiter.length());
		normalsValues[i] = stof(token);
	}
	normalsValues[2] = stof(line);
	return Vec3(normalsValues[0], normalsValues[1], normalsValues[2]);
}

Vec2 Model::MakeVec2FromLine(std::string& line)
{
	using namespace std;
	float vt[3];

	string delimiter = " ";
	size_t pos = 0;
	string token;

	for (int i = 0; i < 1; i++)
	{
		pos = line.find(delimiter);
		token = line.substr(0, pos);
		line.erase(0, pos + delimiter.length());
		vt[i] = stof(token);
	}
	vt[1] = stof(line);
	return Vec2({ vt[0],vt[1] });
}

int Model::ProcessTriangleItem(std::string & line, std::string & delimiter)
{
	size_t pos = line.find(delimiter);
	std::string token = line.substr(0, pos);
	line.erase(0, pos + delimiter.length());
	return stoi(token) - 1;
}

void Model::ConsoleDebug()
{
	printf("VERTICIES\n");
	for (int i = 0; i < vertices.size(); i++)
	{
		printf("v[%i] = ({%F},{%F},{%F})\n", i, vertices[i].x, vertices[i].y, vertices[i].z);
	}
	printf("TEXTURE VERTICES\n");
	for (int i = 0; i < textureVertices.size(); i++)
	{
		printf("vt[%i] = ({%F},{%F})\n", i, textureVertices[i].x, textureVertices[i].y);
	}
	printf("NORMALS\n");
	for (int i = 0; i < normals.size(); i++)
	{
		printf("v[%i] = ({%F},{%F},{%F})\n", i, normals[i].x, normals[i].y, normals[i].z);
	}
	printf("TRIANGLES\n");
	for (int i = 0; i < triangles.size(); i++)
	{
		printf("  v(%i %i %i)\n  vt(%i %i %i)\n  vn = %i\n\n", triangles[i].v0, triangles[i].v1, triangles[i].v2, triangles[i].vt0, triangles[i].vt1, triangles[i].vt2, triangles[i].vn);
	}
}

Model::~Model()
{
}
