#include "Model.h"

void Model::loadObj(const string& path) 
{
	ifstream in(path);

	if (!in)
	{
		std::cout << "Open Object File Error !" << std::endl;
		return;
	}

	vector<vec3> vertexs;
	vector<vec3> normals;
	vector<vec2> textures;

	string L;

	int curObjNums = -1;
	bool isFinish = false;
	char trash;

	while (!in.eof())
	{
		getline(in, L);

		if (!L.compare(0, 2, "v "))
		{
			if (!isFinish)
			{
				curObjNums++;
				Object obj;
				objects.push_back(obj);

				isFinish = true;
			}

			istringstream iss(L);
			iss >> trash;

			vec3 v;
			iss >> v.x;
			iss >> v.y;
			iss >> v.z;
			vertexs.push_back(v);
			continue;
		}

		if (!L.compare(0, 3, "vn "))
		{
			istringstream iss(L);
			iss >> trash >> trash;

			vec3 n;
			iss >> n.x;
			iss >> n.y;
			iss >> n.z;
			normals.push_back(n);
			continue;
		}

		if (!L.compare(0, 3, "vt "))
		{
			istringstream iss(L);
			iss >> trash >> trash;

			vec3 t;
			iss >> t.x;
			iss >> t.y;
			//t.y = 1 - t.y;
			//iss >> t.z;
			t.z = 1.0f;
			textures.push_back(vec2(t.x, t.y));
			continue;
		}

		if (!L.compare(0, 2, "f "))
		{
			if (isFinish)
				isFinish = false;

			istringstream iss(L);
			iss >> trash;

			int vIndex, vtIndex, vnIndex;
			int offset = objects[curObjNums].mesh.VBO.size();

			for (int i = 0; i < 3; i++)
			{
				iss >> vIndex >> trash >> vtIndex >> trash >> vnIndex;
				Vertex vertex(vertexs[vIndex - 1], vec4(1.0f, 1.0f, 1.0f, 1.0f), normals[vnIndex - 1], textures[vtIndex - 1]);
				objects[curObjNums].mesh.VBO.push_back(vertex);
				objects[curObjNums].mesh.EBO.push_back(offset + i);
			}
			continue;

		}
	}
}
