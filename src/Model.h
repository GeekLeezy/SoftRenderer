#pragma once

#include "Object.h"
#include "Global.h"

class Model
{
	public:
		vector<Object> objects;

		Model() = default;

		Model(const string& path)
		{
			loadObj(path);
		}

		Model& operator=(const Model& model) {
			if (&model == this)
				return *this;

			objects = model.objects;
			return *this;
		}

		void setMaterial(const int& id, const Material& m) {
			objects[id].material = m;
		}

		void loadObj(const string& path);

		~Model() = default;
};

