#include "Model.h"


Model::Model(std::vector<Vec3> verticies, std::vector<Vec3> normals, std::vector<Triangle<int>> triangles) :
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
				// process normal
				line = line.substr(3, line.length());
				normals.push_back(MakeVectorFromLine(line));
			}
			else
			{
				// process vertex
				line = line.substr(2, line.length());
				vertices.push_back(MakeVectorFromLine(line));
			}
		}
		if (line[0] == 'f')
		{
			// process vertex triangle
			line = line.substr(2, line.length());
			int vi[3];
			int ni[3];
			string delimiterPair = "//";
			string delimiterItem = " ";
			size_t pos = 0;
			string token;
			for (int i = 0; i < 3; i++)
			{
				pos = line.find(delimiterPair);
				token = line.substr(0, pos);
				line.erase(0, pos + delimiterPair.length());
				vi[i] = stoi(token) - 1;

				pos = line.find(delimiterItem);
				token = line.substr(0, pos);
				line.erase(0, pos + delimiterItem.length());
				ni[i] = stoi(token) - 1;
			}
			//vi[2] = stoi(token) - 1;
			//printf("%i %i %i / %i \n ", vi[0], vi[1], vi[2], ni[0]);
			triangles.push_back({ vi[0],vi[1],vi[2],ni[0] });
		}
	}
	// close the file stream
	inFile.close();
}

Vec3 Model::MakeVectorFromLine(std::string& line)
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

void Model::ConsoleDebug()
{
	printf("VERTICIES\n");
	for (int i = 0; i < vertices.size(); i++)
	{
		printf("v[%i] = ({%F},{%F},{%F})\n", i, vertices[i].x, vertices[i].y, vertices[i].z);
	}
	printf("NORMALS\n");
	for (int i = 0; i < normals.size(); i++)
	{
		printf("v[%i] = ({%F},{%F},{%F})\n", i, normals[i].x, normals[i].y, normals[i].z);
	}
	printf("TRIANGLES\n");
	for (int i = 0; i < triangles.size(); i++)
	{
		printf("%i %i %i // %i\n", triangles[i].v0, triangles[i].v1, triangles[i].v2, triangles[i].vn);
	}
}

Model::~Model()
{
}

